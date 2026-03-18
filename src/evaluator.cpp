#include "evaluator.h"
#include <iostream>
#include <sstream>

namespace script_lang {

Evaluator::Evaluator() {
    environment = std::make_shared<Environment>();
    initBuiltins();
}

void Evaluator::initBuiltins() {
    builtins["len"] = [](const std::vector<ObjectPtr>& args) -> ObjectPtr {
        if (args.size() != 1) {
            return makeError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
        }
        
        if (auto str = std::dynamic_pointer_cast<String>(args[0])) {
            return makeInteger(static_cast<int>(str->value.length()));
        } else if (auto arr = std::dynamic_pointer_cast<Array>(args[0])) {
            return makeInteger(static_cast<int>(arr->elements.size()));
        } else {
            return makeError("argument to `len` not supported, got " + 
                           std::to_string(static_cast<int>(args[0]->type())));
        }
    };
    
    builtins["puts"] = [](const std::vector<ObjectPtr>& args) -> ObjectPtr {
        for (const auto& arg : args) {
            std::cout << arg->inspect() << std::endl;
        }
        return makeNull();
    };
    
    builtins["first"] = [](const std::vector<ObjectPtr>& args) -> ObjectPtr {
        if (args.size() != 1) {
            return makeError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
        }
        
        if (auto arr = std::dynamic_pointer_cast<Array>(args[0])) {
            if (!arr->elements.empty()) {
                return arr->elements[0];
            }
            return makeNull();
        } else {
            return makeError("argument to `first` must be ARRAY, got " + 
                           std::to_string(static_cast<int>(args[0]->type())));
        }
    };
    
    builtins["last"] = [](const std::vector<ObjectPtr>& args) -> ObjectPtr {
        if (args.size() != 1) {
            return makeError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
        }
        
        if (auto arr = std::dynamic_pointer_cast<Array>(args[0])) {
            if (!arr->elements.empty()) {
                return arr->elements[arr->elements.size() - 1];
            }
            return makeNull();
        } else {
            return makeError("argument to `last` must be ARRAY, got " + 
                           std::to_string(static_cast<int>(args[0]->type())));
        }
    };
    
    builtins["rest"] = [](const std::vector<ObjectPtr>& args) -> ObjectPtr {
        if (args.size() != 1) {
            return makeError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=1");
        }
        
        if (auto arr = std::dynamic_pointer_cast<Array>(args[0])) {
            if (arr->elements.size() > 1) {
                std::vector<ObjectPtr> elements(arr->elements.begin() + 1, arr->elements.end());
                return makeArray(std::move(elements));
            }
            return makeArray({});
        } else {
            return makeError("argument to `rest` must be ARRAY, got " + 
                           std::to_string(static_cast<int>(args[0]->type())));
        }
    };
    
    builtins["push"] = [](const std::vector<ObjectPtr>& args) -> ObjectPtr {
        if (args.size() != 2) {
            return makeError("wrong number of arguments. got=" + std::to_string(args.size()) + ", want=2");
        }
        
        if (auto arr = std::dynamic_pointer_cast<Array>(args[0])) {
            auto newElements = arr->elements;
            newElements.push_back(args[1]);
            return makeArray(std::move(newElements));
        } else {
            return makeError("argument to `push` must be ARRAY, got " + 
                           std::to_string(static_cast<int>(args[0]->type())));
        }
    };
}

ObjectPtr Evaluator::evaluate(const Program* program) {
    return evalProgram(program);
}

ObjectPtr Evaluator::evalProgram(const Program* program) {
    ObjectPtr result = makeNull();
    
    for (const auto& statement : program->statements) {
        result = evalStatement(statement.get());
        
        if (auto returnValue = std::dynamic_pointer_cast<ReturnValue>(result)) {
            return returnValue->value;
        } else if (auto error = std::dynamic_pointer_cast<Error>(result)) {
            return error;
        }
    }
    
    return result;
}

ObjectPtr Evaluator::evalBlockStatement(const BlockStatement* block) {
    ObjectPtr result = makeNull();
    
    for (const auto& stmt : block->statements) {
        result = evalStatement(stmt.get());
        
        if (std::dynamic_pointer_cast<ReturnValue>(result) || 
            std::dynamic_pointer_cast<Error>(result)) {
            return result;
        }
    }
    
    return result;
}

ObjectPtr Evaluator::evalStatement(const Statement* stmt) {
    if (!stmt) return makeNull();
    
    if (auto letStmt = dynamic_cast<const LetStatement*>(stmt)) {
        auto val = evalExpression(letStmt->value.get());
        if (std::dynamic_pointer_cast<Error>(val)) return val;
        environment->set(letStmt->name->value, val);
        return val;
    } else if (auto returnStmt = dynamic_cast<const ReturnStatement*>(stmt)) {
        auto val = evalExpression(returnStmt->returnValue.get());
        if (std::dynamic_pointer_cast<Error>(val)) return val;
        return makeReturnValue(val);
    } else if (auto exprStmt = dynamic_cast<const ExpressionStatement*>(stmt)) {
        return evalExpression(exprStmt->expression.get());
    }
    
    return makeNull();
}

ObjectPtr Evaluator::evalExpression(const Expression* expression) {
    if (!expression) return makeNull();
    
    if (auto ident = dynamic_cast<const Identifier*>(expression)) {
        return evalIdentifier(ident);
    } else if (auto integer = dynamic_cast<const IntegerLiteral*>(expression)) {
        return makeInteger(integer->value);
    } else if (auto flt = dynamic_cast<const FloatLiteral*>(expression)) {
        return makeFloat(flt->value);
    } else if (auto str = dynamic_cast<const StringLiteral*>(expression)) {
        return makeString(str->value);
    } else if (auto boolean = dynamic_cast<const BooleanLiteral*>(expression)) {
        return makeBoolean(boolean->value);
    } else if (auto prefix = dynamic_cast<const PrefixExpression*>(expression)) {
        auto right = evalExpression(prefix->right.get());
        if (std::dynamic_pointer_cast<Error>(right)) return right;
        return evalPrefixExpression(prefix->op, right);
    } else if (auto infix = dynamic_cast<const InfixExpression*>(expression)) {
        auto left = evalExpression(infix->left.get());
        if (std::dynamic_pointer_cast<Error>(left)) return left;
        auto right = evalExpression(infix->right.get());
        if (std::dynamic_pointer_cast<Error>(right)) return right;
        return evalInfixExpression(infix->op, left, right);
    } else if (auto ifExpr = dynamic_cast<const IfExpression*>(expression)) {
        return evalIfExpression(ifExpr);
    } else if (auto fn = dynamic_cast<const FunctionLiteral*>(expression)) {
        std::vector<std::string> params;
        for (const auto& param : fn->parameters) {
            params.push_back(param->value);
        }
        // Note: This creates a non-owning shared_ptr. The underlying data is owned by the AST.
        // This is safe as long as the AST outlives the function execution.
        auto bodyPtr = std::shared_ptr<BlockStatement>(const_cast<BlockStatement*>(fn->body.get()), [](BlockStatement*){});
        return makeFunction(params, bodyPtr, environment);
    } else if (auto call = dynamic_cast<const CallExpression*>(expression)) {
        auto function = evalExpression(call->function.get());
        if (std::dynamic_pointer_cast<Error>(function)) return function;
        
        auto args = evalExpressions(call->arguments);
        if (args.size() == 1 && std::dynamic_pointer_cast<Error>(args[0])) {
            return args[0];
        }
        
        return applyFunction(function, args);
    }
    
    return makeNull();
}

ObjectPtr Evaluator::evalPrefixExpression(const std::string& op, ObjectPtr right) {
    if (op == "!") {
        if (auto boolean = std::dynamic_pointer_cast<Boolean>(right)) {
            return makeBoolean(!boolean->value);
        } else if (std::dynamic_pointer_cast<Null>(right)) {
            return makeBoolean(true);
        }
        return makeBoolean(false);
    } else if (op == "-") {
        if (auto integer = std::dynamic_pointer_cast<Integer>(right)) {
            return makeInteger(-integer->value);
        } else if (auto flt = std::dynamic_pointer_cast<Float>(right)) {
            return makeFloat(-flt->value);
        } else {
            return makeError("unknown operator: -" + right->inspect());
        }
    } else {
        return makeError("unknown operator: " + op + right->inspect());
    }
}

ObjectPtr Evaluator::evalInfixExpression(const std::string& op, ObjectPtr left, ObjectPtr right) {
    // Integer operations
    if (auto leftInt = std::dynamic_pointer_cast<Integer>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            int leftVal = leftInt->value;
            int rightVal = rightInt->value;
            
            if (op == "+") return makeInteger(leftVal + rightVal);
            if (op == "-") return makeInteger(leftVal - rightVal);
            if (op == "*") return makeInteger(leftVal * rightVal);
            if (op == "/") {
                if (rightVal == 0) return makeError("division by zero");
                return makeInteger(leftVal / rightVal);
            }
            if (op == "%") return makeInteger(leftVal % rightVal);
            if (op == "<") return makeBoolean(leftVal < rightVal);
            if (op == ">") return makeBoolean(leftVal > rightVal);
            if (op == "<=") return makeBoolean(leftVal <= rightVal);
            if (op == ">=") return makeBoolean(leftVal >= rightVal);
            if (op == "==") return makeBoolean(leftVal == rightVal);
            if (op == "!=") return makeBoolean(leftVal != rightVal);
        }
    }
    // Float operations
    else if (auto leftFloat = std::dynamic_pointer_cast<Float>(left)) {
        double leftVal = leftFloat->value;
        double rightVal = 0;
        
        if (auto rightFloat = std::dynamic_pointer_cast<Float>(right)) {
            rightVal = rightFloat->value;
        } else if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            rightVal = static_cast<double>(rightInt->value);
        } else {
            return makeError("type mismatch: " + left->inspect() + " " + op + " " + right->inspect());
        }
        
        if (op == "+") return makeFloat(leftVal + rightVal);
        if (op == "-") return makeFloat(leftVal - rightVal);
        if (op == "*") return makeFloat(leftVal * rightVal);
        if (op == "/") {
            if (rightVal == 0) return makeError("division by zero");
            return makeFloat(leftVal / rightVal);
        }
        if (op == "<") return makeBoolean(leftVal < rightVal);
        if (op == ">") return makeBoolean(leftVal > rightVal);
        if (op == "<=") return makeBoolean(leftVal <= rightVal);
        if (op == ">=") return makeBoolean(leftVal >= rightVal);
        if (op == "==") return makeBoolean(leftVal == rightVal);
        if (op == "!=") return makeBoolean(leftVal != rightVal);
    }
    // String operations
    else if (auto leftStr = std::dynamic_pointer_cast<String>(left)) {
        if (op == "+") {
            if (auto rightStr = std::dynamic_pointer_cast<String>(right)) {
                return makeString(leftStr->value + rightStr->value);
            }
        }
        if (op == "==") {
            if (auto rightStr = std::dynamic_pointer_cast<String>(right)) {
                return makeBoolean(leftStr->value == rightStr->value);
            }
        }
        if (op == "!=") {
            if (auto rightStr = std::dynamic_pointer_cast<String>(right)) {
                return makeBoolean(leftStr->value != rightStr->value);
            }
        }
    }
    // Boolean operations
    else if (auto leftBool = std::dynamic_pointer_cast<Boolean>(left)) {
        if (op == "==") {
            if (auto rightBool = std::dynamic_pointer_cast<Boolean>(right)) {
                return makeBoolean(leftBool->value == rightBool->value);
            }
        }
        if (op == "!=") {
            if (auto rightBool = std::dynamic_pointer_cast<Boolean>(right)) {
                return makeBoolean(leftBool->value != rightBool->value);
            }
        }
    }
    
    return makeError("unknown operator: " + left->inspect() + " " + op + " " + right->inspect());
}

ObjectPtr Evaluator::evalIfExpression(const IfExpression* ie) {
    auto condition = evalExpression(ie->condition.get());
    if (std::dynamic_pointer_cast<Error>(condition)) return condition;
    
    // Check if condition is truthy
    bool isTruthy = false;
    if (auto boolCondition = std::dynamic_pointer_cast<Boolean>(condition)) {
        isTruthy = boolCondition->value;
    } else if (auto intCondition = std::dynamic_pointer_cast<Integer>(condition)) {
        isTruthy = intCondition->value != 0;
    } else if (auto floatCondition = std::dynamic_pointer_cast<Float>(condition)) {
        isTruthy = floatCondition->value != 0.0;
    } else if (auto strCondition = std::dynamic_pointer_cast<String>(condition)) {
        isTruthy = !strCondition->value.empty();
    } else if (std::dynamic_pointer_cast<Null>(condition)) {
        isTruthy = false;
    } else {
        // For other types (like functions), consider them truthy
        isTruthy = true;
    }
    
    if (isTruthy) {
        return evalBlockStatement(ie->consequence.get());
    } else if (ie->alternative) {
        return evalBlockStatement(ie->alternative.get());
    } else {
        return makeNull();
    }
}

ObjectPtr Evaluator::evalIdentifier(const Identifier* ident) {
    // Check built-ins first
    auto builtinIt = builtins.find(ident->value);
    if (builtinIt != builtins.end()) {
        return makeBuiltin(builtinIt->second);
    }
    
    // Check environment
    auto val = environment->get(ident->value);
    if (val) {
        return val;
    }
    
    return makeError("identifier not found: " + ident->value);
}

std::vector<ObjectPtr> Evaluator::evalExpressions(const std::vector<std::unique_ptr<Expression>>& exps) {
    std::vector<ObjectPtr> result;
    for (const auto& e : exps) {
        auto evaluated = evalExpression(e.get());
        if (std::dynamic_pointer_cast<Error>(evaluated)) {
            return {evaluated};
        }
        result.push_back(evaluated);
    }
    return result;
}

std::shared_ptr<Environment> Evaluator::extendFunctionEnv(const Function* fn, const std::vector<ObjectPtr>& args) {
    auto env = std::make_shared<Environment>(fn->env);
    
    for (size_t i = 0; i < fn->parameters.size(); ++i) {
        env->set(fn->parameters[i], args[i]);
    }
    
    return env;
}

ObjectPtr Evaluator::applyFunction(ObjectPtr fn, const std::vector<ObjectPtr>& args) {
    if (auto function = std::dynamic_pointer_cast<Function>(fn)) {
        auto extendedEnv = extendFunctionEnv(function.get(), args);
        auto oldEnv = environment;
        environment = extendedEnv;
        
        auto evaluated = evalBlockStatement(function->body.get());
        environment = oldEnv;
        
        if (auto returnValue = std::dynamic_pointer_cast<ReturnValue>(evaluated)) {
            return returnValue->value;
        }
        return evaluated;
    } else if (auto builtin = std::dynamic_pointer_cast<Builtin>(fn)) {
        return builtin->fn(args);
    }
    
    return makeError("not a function: " + fn->inspect());
}

} // namespace script_lang