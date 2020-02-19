#pragma once

#include <iostream>

//! Wrap a single value of arbitary type.  
/*! This class only currently handles ints and doubles but it
  anticipates either a C++ wrapper for AIF or a full C++ replacement
  for AIF.   

  This class is similar to std::any but I anticipate we will need
  more specialized processing than that provided by std::any.   So 
  it makes sense to start with a customized form.

  I could potentially see a possibility of providing type specialized 
  operators, but in general it seems like a combinatorial nightmare.
  This leads to the conclusion - apparently the same one as the original
  guard-rd - that the sensible approach is push more inteligence into
  the data type.
*/
class Value
{
public:
    Value();
    Value(double v);
    Value(int v);

    Value& operator=(int v) { m_type = INT, m_v.i = v; return *this; }
    Value& operator=(double v) { m_type = DOUBLE, m_v.d = v; return *this; }

    bool to(int& ival) const;
    bool to(double& dval) const;

    bool isDefined() const { return m_type != NONE; }

    ~Value();
    Value(const Value& rhs);
    Value& operator=(const Value& rhs);
    
private:
    enum Type { NONE, INT, DOUBLE };
    Type m_type;
    union {
        int i;
        double d;
    } m_v;
};

bool operator==(const Value& a, const Value& b);
bool operator!=(const Value& a, const Value& b);
bool operator<(const Value& a, const Value& b);
bool operator<=(const Value& a, const Value& b);
bool operator>(const Value& a, const Value& b);
bool operator>=(const Value& a, const Value& b);

Value operator-(const Value& a, const Value& b);
Value operator+(const Value& a, const Value& b);

std::ostream& operator<<(std::ostream& str, const Value& a);


