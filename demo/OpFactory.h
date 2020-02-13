#pragma once

#include <string>
#include <memory>
#include <functional>
#include <string>
#include <map>

class OpBuilder;

class Op;

class OpFactory 
{
public:
    using Factory = std::function<std::shared_ptr<Op>(OpBuilder&)>;
    OpFactory(const std::string& name, Factory maker);

    static std::shared_ptr<Op> make(const std::string& func, OpBuilder& builder);
    
private:
    static std::map<std::string, Factory>* _factories;
};

