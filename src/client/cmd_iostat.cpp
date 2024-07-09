// cmd_iostat.cpp - Fortran IOSTAT table.
// Copyright 2022-2023 Hewlett Packard Enterprise Development LP.

#include "config.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <regex>
#include <map>

#include "client.h"
#include "useful.h"

std::map<int, std::string> iostat_map;
int initialized_iostat_map=0;


int initialize_map (){
    std::string cce_version="";
    std::stringstream path;
    int base=0;

    //get the path to the liberrno.h file
    char * ftn_x686_64 = std::getenv("FTN_X86_64");
    if (ftn_x686_64==nullptr){
        PrintOutError("Cannot find $FTN_X86_64\n");
        return 0;
    }
    std::string line_path(ftn_x686_64);
    line_path.erase(std::remove(line_path.begin(), line_path.end(), '\n'), line_path.cend());
    path<<line_path<<"/include/craylibs/liberrno.h";
    //Read from liberrno.h file to get iostat codes and their definitions

    //Example line to add error code to table:#define FERDPEOF -(BASE+1) /* Tried to read past end of file       */
    static std::regex numRE(R"(#define .* (-?)\(BASE\+([0-9]+)\)[ \t]*\/\*(.*)\*\/)");
    
    //Example line to define BASE:#define BASE    4000
    static std::regex baseRE(R"(#define[ \t]BASE[ \t]([0-9]*))");

    std::ifstream liberrno_file (path.str().c_str());
    std::string line;
    while ( getline (liberrno_file,line) ){
        std::smatch match;
        if (std::regex_match(line, match, baseRE)) {
            //New base definition
            base = std::stoi(match[1].str());
        }else if (std::regex_match(line, match, numRE)) {
            //get error code and description
            auto negative = match[1].str(), addition = match[2].str(), description = match[3].str();
            int num = base+(std::stoi(addition));
            if (!negative.empty()){
                num=-num;
            }
            iostat_map.insert({num,description});
        }
    }
    return 1;
}

static int cmd_info_iostat_cmd(int argc, const char* const argv[]) {
    std::string arg = ArgsToString(&argv[1], argc - 1);

    if (!initialized_iostat_map){
        try{
            initialized_iostat_map=initialize_map();
        }catch (std::exception const &e) {
            PrintOutError("Exception Caught when retrieving map of codes: %s",e.what());
            return 0;
        }
    }

    // Print output
    if (!arg.empty()){
        try{
            int code = std::stoi(arg);
            if (iostat_map.find(code)!=iostat_map.end()){
                PrintOutMsg("%d\t\t%s\n",code, iostat_map[code].c_str());
                std::stringstream cmd;
                cmd << "explain lib-" << abs(code);
                std::system(cmd.str().c_str());
            }else{
                PrintOutError("Cannot find fortran error string for code %d.\n", code);
                return 0;
            }
        }catch (std::exception const &e) {
            PrintOutError("Exception Caught when retrieving provided code: %s",e.what());
            return 0;
        }
    }else{
        PrintOutMsg(" CODE\t\t DESCRIPTION\n");
        for (auto const &pair: iostat_map) {
            PrintOutMsg("%6d\t\t%s\n",pair.first,pair.second.c_str());
        }
    }
    return 1;
}

void ConsoleCommandsFactory::insert_iostat_commands(CommandNode& root) {
    root.addSubCmd("info iostat", nullptr, proc_optional,
                   cmd_info_iostat_cmd, cmd_info_iostat_help,
                   "Display table with iostat codes for Fortran.");
}
