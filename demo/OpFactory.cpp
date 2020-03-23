
#include "OpFactory.h"
#include <iostream>

std::map<std::string, OpFactory::Factory> *OpFactory::_factories = 0;

OpFactory::OpFactory(const std::string& name, Factory maker)
{
    if (!_factories)
        _factories = new std::map<std::string, Factory>;

	_factories->insert(make_pair(name,maker));
}

std::shared_ptr<Op> OpFactory::make(const std::string& func, OpBuilder& args)
{
    auto loc = _factories->find(func);
    if (loc == _factories->end()) {
        std::cerr << "no operator " << func << "\n";
        return nullptr;
    } else {
        return loc->second(args);
    }
}
    
