// Tools for helping to process expression and type strings
// Copyright 2023 Hewlett Packard Enterprise Development LP.

#include "parse.h"

const char* ExprMatcher::m_openers = "{}()\"'<>";
const char* ExprMatcher::m_closers = "}{)(\"'><";

ExprMatcher::ExprMatcher(char open) {
    char lookc = char{};
    if (auto op = strchr(m_openers,open)) lookc = m_closers[op-m_openers];
    else if (::isdigit(open)) lookc = '1';
    else if (::isalnum(open) || open == '_') lookc = 'A';
    else if (::isspace(open)) lookc = 'W';
    else lookc = 'w';
    m_lookFor = lookc;
}

bool ExprMatcher::operator()(char c) {
    if (!c) return true;

    if (m_escape) { m_escape = false; return false; }
    else if (c == '\\') { m_escape = true; return false; }

    auto lookc = m_lookFor.back();
    auto found = false;
    switch (lookc) {
    case 'A': found = !::isalnum(c) && c != '_'; break;
    case '1': found = !::isdigit(c); break;
    case 'W': found = !::isspace(c); break;
    case 'w': found = ::isspace(c); break;
    default: found = (c == lookc); break;
    };
    if (found) {
        m_lookFor.pop_back();
        return m_lookFor.empty();
    } else if (lookc == '"' || lookc == '\'') {
        return false;
    } else if (auto nest = strchr(m_openers,c)) {
        m_lookFor.push_back(m_closers[nest-m_openers]);
    }
    return false;
}
    
const char* findExprMatch(const char* s) {
    return findExprMatchT(CStrIt(s), CStrIt(s,CStrIt::END)).p;
}

size_t findExprMatch(const std::string& s, size_t pos) {
    return findExprMatchT(s.begin()+pos, s.end()) - s.begin();
}

size_t findExprMatch(const std::string_view& sv, size_t pos) {
    return findExprMatchT(sv.begin()+pos, sv.end()) - sv.begin();
}
