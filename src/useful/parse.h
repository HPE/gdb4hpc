// Tools for helping to process expression and type strings
// Copyright 2023 Hewlett Packard Enterprise Development LP.
#pragma once

#include <iterator>
#include <cstring>

//-----------------------------------------------------------------------
// CStrIt is an iterator that lets you use stl (begin,end) algorithms without
// needed to use strlen to find the end until it's required by operations,
// which normally shouldn't happen during forward iteration.
//---------------------------------------------------------------------------
struct CStrIt {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = char;
    using difference_type = int;
    using pointer = const char*;
    using reference = const char&;

    // enum for the overload that refers to the end of the string
    enum end_type { END };

    CStrIt(const char* loc) : p(loc), str(nullptr) {}
    CStrIt(const char* loc, end_type) : p(nullptr), str(loc) {}
    CStrIt() : p(nullptr), str(nullptr) {}

    char operator*() const { return *p; }
    const char* operator->() const { return p; }
    CStrIt& operator++() { ++p; return *this; }
    CStrIt operator++(int) { CStrIt prev=*this; ++p; return prev; }

    CStrIt& operator--() { --resolve(); return *this; }
    CStrIt operator--(int) { CStrIt prev=*this; ++resolve(); return prev; }

    CStrIt& operator+=(difference_type n) { p = resolve() + n; return *this; }
    CStrIt& operator-=(difference_type n) { p = resolve() - n; return *this; }

    char operator[](difference_type n) { return resolve()[n]; }

    const char*& resolve() const { if (!p && str) { for (p=str; *p; ++p); } return p; }
    bool isNull() const { return !p && !str; }
    bool isTerm() const { return (p ? !*p : (str!=nullptr)); }

    mutable const char* p;
    const char* str;
};

inline bool operator==(CStrIt lhs, CStrIt rhs) {
    if (lhs.isNull()) return rhs.isNull();
    else if (lhs.isTerm()) return rhs.isTerm();
    else return lhs.p == rhs.p;
}

inline bool operator!=(CStrIt lhs, CStrIt rhs) { return !(lhs==rhs); }

inline CStrIt operator+(CStrIt it, CStrIt::difference_type n) {
    return CStrIt(it.resolve() + n);
}
inline CStrIt operator+(CStrIt::difference_type n, CStrIt it) {
    return CStrIt(it.resolve() + n);
}
inline CStrIt operator-(CStrIt it, CStrIt::difference_type n) {
    return CStrIt(it.resolve() - n);
}
inline CStrIt::difference_type operator-(CStrIt a, CStrIt b) {
    return a.resolve() - b.resolve();
}

//===========================================================================
// Lambda to use when searching for matching delimiter of {[(<"' or [}])>"'
// (it can be used in reverse).   It handles the usual pitfalls with delimiters,
// nesting, quotes, and escaped quotes.
//
// Provisionally, it can also recognize decimal numbers, identifies, or whitespace
// in which case it returns true on the first non matching character, i.e. the
// typical one-past semantic.
//
// Note that backslashes are not handled properly when working in reverse.
//===========================================================================
class ExprMatcher {
public:
    ExprMatcher(char open);
    bool operator()(char c);
private:
    static const char* m_openers;
    static const char* m_closers;
    std::string m_lookFor;
    bool m_escape = false;
    
};

// Given a value with *beg referring to a delimiter : {[(<"''">)]}
// return an iterator pointing to its matching delimiter accounting
// for nestings, quoting and escapes
template<class ITER>
ITER findExprMatchT(ITER beg, ITER end) {
    if (beg == end)
        return end;
    
    for (ExprMatcher m(*beg); ++beg != end && !m(*beg); ) {}
    return beg;
}

// Return a pointer to the delimiter '">)]} matching s[0] 
// or a pointer to the end of s accounting for nesting
// quoting and escapes
const char* findExprMatch(const char* s);

// Return the index to delimiter '">)]} matching s[pos]
// accounting for nesting quoting and escapes
size_t findExprMatch(const std::string& s, size_t pos);
