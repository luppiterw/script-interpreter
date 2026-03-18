#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <variant>
#include <iostream>
#include <functional>

namespace script_lang {

enum class ObjectType {
    INTEGER,
    FLOAT,
    STRING,
    BOOLEAN,
    NULL_,
    RETURN_VALUE,
    ERROR,
    FUNCTION,
    BUILTIN,
    ARRAY,
    HASH,
    CLASS,
    INSTANCE
};

class Object {
public:
    virtual ~Object() = default;
    virtual ObjectType type() const = 0;
    virtual std::string inspect() const = 0;
};

using ObjectPtr = std::shared_ptr<Object>;
using BuiltinFunction = std::function<ObjectPtr(const std::vector<ObjectPtr>&)>;

class Integer : public Object {
public:
    int value;
    
    explicit Integer(int v) : value(v) {}
    ObjectType type() const override { return ObjectType::INTEGER; }
    std::string inspect() const override { return std::to_string(value); }
};

class Float : public Object {
public:
    double value;
    
    explicit Float(double v) : value(v) {}
    ObjectType type() const override { return ObjectType::FLOAT; }
    std::string inspect() const override { return std::to_string(value); }
};

class String : public Object {
public:
    std::string value;
    
    explicit String(const std::string& v) : value(v) {}
    ObjectType type() const override { return ObjectType::STRING; }
    std::string inspect() const override { return "\"" + value + "\""; }
};

class Boolean : public Object {
public:
    bool value;
    
    explicit Boolean(bool v) : value(v) {}
    ObjectType type() const override { return ObjectType::BOOLEAN; }
    std::string inspect() const override { return value ? "true" : "false"; }
};

class Null : public Object {
public:
    ObjectType type() const override { return ObjectType::NULL_; }
    std::string inspect() const override { return "null"; }
};

class ReturnValue : public Object {
public:
    ObjectPtr value;
    
    explicit ReturnValue(ObjectPtr v) : value(std::move(v)) {}
    ObjectType type() const override { return ObjectType::RETURN_VALUE; }
    std::string inspect() const override { 
        return value ? value->inspect() : "null"; 
    }
};

class Error : public Object {
public:
    std::string message;
    
    explicit Error(const std::string& msg) : message(msg) {}
    ObjectType type() const override { return ObjectType::ERROR; }
    std::string inspect() const override { return "ERROR: " + message; }
};

class Function : public Object {
public:
    std::vector<std::string> parameters;
    std::shared_ptr<class BlockStatement> body;
    std::shared_ptr<class Environment> env;
    
    Function(std::vector<std::string> params, 
             std::shared_ptr<class BlockStatement> b,
             std::shared_ptr<class Environment> e)
        : parameters(std::move(params)), body(std::move(b)), env(std::move(e)) {}
    
    ObjectType type() const override { return ObjectType::FUNCTION; }
    std::string inspect() const override {
        std::string result = "fn";
        result += "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i];
            if (i < parameters.size() - 1) {
                result += ", ";
            }
        }
        result += ") {\n";
        result += "\n}";
        return result;
    }
};

class Builtin : public Object {
public:
    BuiltinFunction fn;
    
    explicit Builtin(BuiltinFunction f) : fn(std::move(f)) {}
    ObjectType type() const override { return ObjectType::BUILTIN; }
    std::string inspect() const override { return "builtin function"; }
};

class Array : public Object {
public:
    std::vector<ObjectPtr> elements;
    
    explicit Array(std::vector<ObjectPtr> elems) : elements(std::move(elems)) {}
    ObjectType type() const override { return ObjectType::ARRAY; }
    std::string inspect() const override {
        std::string result = "[";
        for (size_t i = 0; i < elements.size(); ++i) {
            result += elements[i]->inspect();
            if (i < elements.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }
};

class HashPair {
public:
    ObjectPtr key;
    ObjectPtr value;
    
    HashPair(ObjectPtr k, ObjectPtr v) : key(std::move(k)), value(std::move(v)) {}
};

class Hash : public Object {
public:
    std::unordered_map<std::string, HashPair> pairs;
    
    explicit Hash(std::unordered_map<std::string, HashPair> p) : pairs(std::move(p)) {}
    ObjectType type() const override { return ObjectType::HASH; }
    std::string inspect() const override {
        std::string result = "{";
        bool first = true;
        for (const auto& pair : pairs) {
            if (!first) {
                result += ", ";
            }
            result += pair.second.key->inspect() + ": " + pair.second.value->inspect();
            first = false;
        }
        result += "}";
        return result;
    }
};

class Class : public Object {
public:
    std::string name;
    std::unordered_map<std::string, ObjectPtr> methods;
    
    Class(const std::string& n, std::unordered_map<std::string, ObjectPtr> m)
        : name(n), methods(std::move(m)) {}
    
    ObjectType type() const override { return ObjectType::CLASS; }
    std::string inspect() const override { return "class " + name; }
};

class Instance : public Object {
public:
    std::shared_ptr<Class> classObj;
    std::unordered_map<std::string, ObjectPtr> fields;
    
    Instance(std::shared_ptr<Class> cls) : classObj(std::move(cls)) {}
    
    ObjectType type() const override { return ObjectType::INSTANCE; }
    std::string inspect() const override {
        return classObj->name + " instance";
    }
};

// Helper functions
inline ObjectPtr makeInteger(int value) {
    return std::make_shared<Integer>(value);
}

inline ObjectPtr makeFloat(double value) {
    return std::make_shared<Float>(value);
}

inline ObjectPtr makeString(const std::string& value) {
    return std::make_shared<String>(value);
}

inline ObjectPtr makeBoolean(bool value) {
    return std::make_shared<Boolean>(value);
}

inline ObjectPtr makeNull() {
    return std::make_shared<Null>();
}

inline ObjectPtr makeReturnValue(ObjectPtr value) {
    return std::make_shared<ReturnValue>(std::move(value));
}

inline ObjectPtr makeError(const std::string& message) {
    return std::make_shared<Error>(message);
}

inline ObjectPtr makeFunction(std::vector<std::string> parameters,
                             std::shared_ptr<class BlockStatement> body,
                             std::shared_ptr<class Environment> env) {
    return std::make_shared<Function>(std::move(parameters), std::move(body), std::move(env));
}

inline ObjectPtr makeBuiltin(BuiltinFunction fn) {
    return std::make_shared<Builtin>(std::move(fn));
}

inline ObjectPtr makeArray(std::vector<ObjectPtr> elements) {
    return std::make_shared<Array>(std::move(elements));
}

inline ObjectPtr makeHash(std::unordered_map<std::string, HashPair> pairs) {
    return std::make_shared<Hash>(std::move(pairs));
}

} // namespace script_lang

#endif // OBJECT_H