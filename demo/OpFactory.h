#pragma once

#include <string>
#include <memory>
#include <functional>
#include <string>
#include <map>

class Op; class OpBuilder;

//! Abstract fatory for mapping a string name to operator constructor.
/*! When creating a new operator, it just needs a line like this:
  static OpFactory factory("more", More::make);
  to register the operator with the parsing system.
*/
class OpFactory 
{
public:
    using Factory = std::function<std::shared_ptr<Op>(OpBuilder&)>;
    OpFactory(const std::string& name, Factory maker);

    static std::shared_ptr<Op> make(const std::string& func, OpBuilder& builder);
    
private:
    static std::map<std::string, Factory>* _factories;
};

