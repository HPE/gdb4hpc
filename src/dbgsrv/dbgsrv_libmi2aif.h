/******************************************************************************\
 * Header file for libmi to aif conversion routines
 *
 * Copyright 2011-2013,2020-2023 Hewlett Packard Enterprise Development LP.	All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/
#pragma once

// requisite includes
#include "MI.h"
#include "ais.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <regex>

class AIFStream; class MIWriteBuffer; class LazyMIVar; class Chunker;

// Class to handle templated types.   For templated classes, gdb will report some
// types in terms of template arguments, e.g. std::shared_ptr<T> where it
// reported the template mapping earlier with "[with T = int]"
class PTypeFields {
    // derive class fields and parent classes from the result of a ptype command
public:
    PTypeFields(MICommand& sentCmd, bool isFortran);
    PTypeFields() = default; // construct empty

    const std::vector<std::string>& parentClasses() const { return m_parentClasses; }
    const std::vector<std::string>& fields() const { return m_fields; }

    // name, type
    std::pair<std::string, std::string>
    parseField(const std::string& type, const std::string& fieldStr);

private:
    class Templates {
    public:
        Templates(MIList* oobs);
        Templates() = default;
        std::string operator()(std::string&& type);
    private:
        std::vector<std::pair<std::regex,std::string>> m_typeMap;
    };
    
    void initFortranFields(MICommand& sentCmd);

    std::vector<std::string> m_parentClasses;
    std::vector<std::string> m_fields;
    Templates m_typeMapper;
};

class Mi2Aif {
public:
    Mi2Aif(MISession* session);

    std::shared_ptr<MIVar> createMIVar(const char *);
    AIFStream* getAIF(std::shared_ptr<MIVar>);
    int getChildren(MIVar *);
    char* getVarValue(char *);
    MISession* getSession() const { return m_session; }
    std::string getPTypeAlias(const std::string& type, bool isExpression, bool strict);
    PTypeFields getPTypeFields(const std::string& type, bool isFortran);

    std::string nextChunk(Chunker& chunker, bool& isFinal);
    std::unique_ptr<Chunker> makeChunker(const std::string& var);
    std::unique_ptr<Chunker> makeChunker(std::shared_ptr<MIVar> var);
    void initTopRef(std::shared_ptr<LazyMIVar> var);
    std::shared_ptr<ais::Type> getKnownType(const std::string& name);
    std::shared_ptr<ais::Type> addKnownType(const std::string& name,
                                            std::shared_ptr<ais::Type> type);

    size_t swizzlePointer(const std::string& type, void* val);
    std::pair<std::string, std::string> extractMetaInfo(const char* expr);

private:
    void writeVal(std::shared_ptr<MIVar> var, int refIdx);
    void sendMICommand(MICommand* command);
    std::shared_ptr<MIVar> createMIVarWithMetaInfo(const char* expr, const std::string& metaInfo);
    void parse(std::shared_ptr<ais::Type> type, const char* gdbRes, int refIdx);

    MISession* m_session;
    size_t m_depth;
    std::vector<std::shared_ptr<LazyMIVar>> m_refs;
    std::unordered_map<void*,size_t> m_refMap;

    std::shared_ptr<MIWriteBuffer> m_writeBuffer;
    ais::Writer m_writer;

    std::map<std::string, std::shared_ptr<ais::Type>> m_types;
};

// Top leval function to get values from the debugger.
AIFStream* GetAIFVar(MISession *, const char *);
char* EvalExpression(MISession* session, const char* expression);

namespace TypeParser {
std::shared_ptr<ais::Type> getType(Mi2Aif& mi, MIVar* var, const char* typeStr);
}

extern const std::string ANONYMOUS_NAMESPACE;

