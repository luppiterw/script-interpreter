#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "ast.h"
#include "object.h"
#include "environment.h"
#include <memory>
#include <unordered_map>
#include <functional>

namespace script_lang {

class Evaluator {
private:
    std::shared_ptr<Environment> environment;
    
    // Built-in functions
    std::unordered_map<std::string, BuiltinFunction> builtins;
    
    void initBuiltins();
    ObjectPtr evalProgram(const Program* program);
    ObjectPtr evalBlockStatement(const BlockStatement* block);
    ObjectPtr evalStatement(const Statement* stmt);
    ObjectPtr evalExpression(const Expression* expression);
    ObjectPtr evalPrefixExpression(const std::string& op, ObjectPtr right);
    ObjectPtr evalInfixExpression(const std::string& op, ObjectPtr left, ObjectPtr right);
    ObjectPtr evalIfExpression(const IfExpression* ie);
    ObjectPtr evalIdentifier(const Identifier* ident);
    ObjectPtr evalIndexExpression(ObjectPtr left, ObjectPtr index);
    ObjectPtr evalWhileStatement(const WhileStatement* ws);
    std::vector<ObjectPtr> evalExpressions(const std::vector<std::unique_ptr<Expression>>& exps);
    ObjectPtr applyFunction(ObjectPtr fn, const std::vector<ObjectPtr>& args);
    std::shared_ptr<Environment> extendFunctionEnv(const Function* fn, const std::vector<ObjectPtr>& args);
    
public:
    Evaluator();
    ObjectPtr evaluate(const Program* program);
    std::shared_ptr<Environment> getEnvironment() const { return environment; }
};

} // namespace script_lang

#endif // EVALUATOR_H