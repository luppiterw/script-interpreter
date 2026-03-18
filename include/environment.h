#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace script_lang {

class Environment : public std::enable_shared_from_this<Environment> {
private:
    std::unordered_map<std::string, ObjectPtr> store;
    std::shared_ptr<Environment> outer;

public:
    Environment() : outer(nullptr) {}
    
    explicit Environment(std::shared_ptr<Environment> outer) : outer(std::move(outer)) {}
    
    ObjectPtr get(const std::string& name) {
        auto it = store.find(name);
        if (it != store.end()) {
            return it->second;
        }
        
        if (outer) {
            return outer->get(name);
        }
        
        return nullptr;
    }
    
    ObjectPtr set(const std::string& name, ObjectPtr obj) {
        store[name] = std::move(obj);
        return store[name];
    }
    
    // Update an existing variable in this or any enclosing scope
    bool update(const std::string& name, ObjectPtr obj) {
        auto it = store.find(name);
        if (it != store.end()) {
            it->second = std::move(obj);
            return true;
        }
        if (outer) {
            return outer->update(name, obj);
        }
        return false;
    }
    
    std::shared_ptr<Environment> getOuter() const {
        return outer;
    }
    
    // Create a new enclosed environment
    std::shared_ptr<Environment> newEnclosedEnvironment() {
        return std::make_shared<Environment>(shared_from_this());
    }
};

} // namespace script_lang

#endif // ENVIRONMENT_H