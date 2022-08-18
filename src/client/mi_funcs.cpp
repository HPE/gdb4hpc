// mi_funcs.cpp - Command line interface routines.
// Copyright 2022 Hewlett Packard Enterprise Development LP.

#include "client.h"
#include "useful.h"
#include <map>

std::string launcher_args;
std::string num_ranks;

std::string break_insert_cmd(char *);
std::string break_delete_cmd(char *);
std::string exec_args_cmd(char *);
std::string file_exec_and_sym_cmd(char *);
std::string thread_select_cmd(char *);
std::string var_create_cmd(char *);
typedef std::string (*fp)(char *);

//map for commands that have to be parsed for cli command
std::map<std::string, fp> func_cmd_map={{"-break-delete", &break_delete_cmd },
                                {"-break-insert", &break_insert_cmd },
                                {"-exec-arguments", &exec_args_cmd },
                                {"-file-exec-and-symbols", &file_exec_and_sym_cmd},
                                {"-thread-select", &thread_select_cmd},
                                {"3-var-create", &var_create_cmd}};

//map for commands that have a default cli command
std::map<std::string, std::string> return_cmd_map={{"-break-list", "info breakpoints"},
                                {"-exec-continue", "continue"},
                                {"-exec-next", "next"},
                                {"-exec-run", "-exec-run"},
                                {"-exec-step", "step"},
                                {"-file-list-exec-source-files", "info sources"},
                                {"1-stack-list-variables", "info locals"},
                                {"1-thread-info", "info threads -v"}};

int launch_count=0;

std::string convertMiToCli(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    std::string token_str=std::string(token);

    token_str.erase(token_str.find_last_not_of(" \n\r\t")+1);

    std::string result_str;
    if (func_cmd_map.find(token_str)==func_cmd_map.end()){
        if (return_cmd_map.find(token_str)==return_cmd_map.end()){
            //if cmd is not found in either map, return original cmd
            result_str=cmd;
        }else{
            result_str=return_cmd_map.at(token_str);
        }
    }else{
        result_str=func_cmd_map.at(token_str)(cmd);
    }

    return result_str;
}

std::string break_insert_cmd(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    token = strtok(nullptr, " ");
    return "break " + std::string(token);
}

std::string break_delete_cmd(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    token = strtok(nullptr, " ");
    return "delete "+ std::string(token);
}

std::string exec_args_cmd(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    launcher_args="";
    while ((token=strtok(nullptr," "))){
        if (!strcmp(token, "-r")){
            token=strtok(nullptr," \n");
            num_ranks=token;
        }else{
            if (!launcher_args.empty()){
                launcher_args.append(" ");
            }
            launcher_args.append(token);
        }
    }
    launcher_args.erase(std::remove(launcher_args.begin(), launcher_args.end(), '\n'),
        launcher_args.end());
    //this command sets launcher args and does not need to be processed. Setting to exit will skip command processing
    return "ignore";
}

std::string file_exec_and_sym_cmd(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    token = strtok(nullptr, " ");
    std::string app="";
    app.append(std::to_string(launch_count));
    if (!num_ranks.empty()){
        app.append("{");
        app.append(num_ranks);
        app.append("}");
    }
    launch_count++;
    if (!launcher_args.empty()){
        return "launch $App"+app+" "+std::string(token)+" -a \""+launcher_args+"\"";
    }else{
        return "launch $App"+app+" "+std::string(token);
    }
}

std::string thread_select_cmd(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    token = strtok(nullptr, " ");
    return "thread "+std::string(token);
}

std::string var_create_cmd(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    token = strtok(nullptr, " ");
    token = strtok(nullptr, " ");
    token = strtok(nullptr, " ");
    std::string str2 = strdup(token);
    return "print "+str2.substr(1,str2.length()-3);
}
