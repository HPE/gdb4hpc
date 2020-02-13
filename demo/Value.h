#pragma once

#include <iostream>

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


