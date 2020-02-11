#include "Value.h"

Value::Value() : m_type(NONE) {}

Value::Value(double v) : m_type(DOUBLE) {
    m_v.d = v;
}

Value::Value(int v) : m_type(INT) {
    m_v.i = v;
}

Value::~Value() {}              // no pointer values yet

Value::Value(const Value& rhs) : m_type(rhs.m_type)
{
    switch (m_type) {
    case NONE:
        break;
    case INT:
        m_v.i = rhs.m_v.i;
        break;
    case DOUBLE:
        m_v.d = rhs.m_v.d;
        break;
    }
}

Value& Value::operator=(const Value& rhs)
{
    m_type = rhs.m_type;
    switch (m_type) {
    case NONE:
        break;
    case INT:
        m_v.i = rhs.m_v.i;
        break;
    case DOUBLE:
        m_v.d = rhs.m_v.d;
        break;
    }
    return *this;
}

bool Value::to(int& val) const { 
    switch (m_type) {
    case NONE:
        return false;
    case INT:
        val = m_v.i;
        return true;
    case DOUBLE:
        return false;
    }
}

bool Value::to(double& val) const {
    switch (m_type) {
    case NONE:
        return false;
    case INT:
        val = m_v.i;
        return true;
    case DOUBLE:
        val = m_v.d;
        return true;
    }
}
bool operator==(const Value& a, const Value& b) {
    int ia, ib; double da, db;
    if (a.to(ia) && b.to(ib))
        return ia == ib;
    else if (a.to(da) && b.to(db))
        return da == db;
    else
        return false;
}
bool operator!=(const Value& a, const Value& b) {
    return !(a == b);
}

bool operator<(const Value& a, const Value& b)
{
    int ia, ib; double da, db;
    if (a.to(ia) && b.to(ib))
        return ia < ib;
    else if (a.to(da) && b.to(db))
        return da < db;
    else
        return false;

}
bool operator<=(const Value& a, const Value& b) {
    return !(a > b);
}
bool operator>(const Value& a, const Value& b) {
    int ia, ib; double da, db;
    if (a.to(ia) && b.to(ib))
        return ia < ib;
    else if (a.to(da) && b.to(db))
        return da < db;
    else
        return false;
}
bool operator>=(const Value& a, const Value& b)
{
    return !(a < b);
}

Value operator-(const Value& a, const Value& b) {
    int ia, ib; double da, db;
    if (a.to(ia) && b.to(ib))
        return Value(ia-ib);
    else if (a.to(da) && b.to(db))
        return Value(da-db);
    else
        return Value();
}
Value operator+(const Value& a, const Value& b)
{
    int ia, ib; double da, db;
    if (a.to(ia) && b.to(ib))
        return Value(ia+ib);
    else if (a.to(da) && b.to(db))
        return Value(da+db);
    else
        return Value();
}

std::ostream& operator<<(std::ostream& str, const Value& a)
{
    int i; double d;
    if (a.to(i))
        return str << i;
    else if (a.to(d))
        return str << d;
    else
        return str << "UNDEFINED";
}
