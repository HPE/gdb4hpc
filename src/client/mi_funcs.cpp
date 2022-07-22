// mi_funcs.cpp - Command line interface routines.
// Copyright 2022 Hewlett Packard Enterprise Development LP.

#include "client.h"
#include "useful.h"

std::string launcher_args;
std::string num_ranks;

int launch_count=0;

char * convertMiToCli(char * cmd){
    char * temp = strdup(cmd);
    char * token = strtok(temp, " ");
    char * result_str = nullptr;
    //z-cleanup
    printf("token:%s\n", token);
    if (!strcmp( token, "-break-insert")){
        char * file =nullptr;
        token = strtok(nullptr, " ");
        file=strdup(token);
        Asprintf(&result_str, "break %s", file);
    }else if (!strcmp( token, "-break-delete")){
        char * bkpt = nullptr;
        token = strtok(nullptr, " ");
        bkpt=strdup(token);
        Asprintf(&result_str, "delete %s", bkpt);
    }else if (!strcmp( token, "3-var-create")){
        token = strtok(nullptr, " ");
        token = strtok(nullptr, " ");
        token = strtok(nullptr, " ");
        char * str;
        std::string str2 = strdup(token);
        str = const_cast<char*>((str2.substr(1,str2.length()-3)).c_str());
        Asprintf(&result_str, "print %s", str);
    }else if (!strcmp( token, "-break-list\n")){
        Asprintf(&result_str, "info breakpoints");
    }else if (!strcmp( token, "1-thread-info\n")){
        Asprintf(&result_str, "info threads -v");
    }else if (!strcmp( token, "-thread-select")){
        token = strtok(nullptr, " ");
        Asprintf(&result_str, "thread %s",token);
    }else if (!strcmp(token, "1-stack-list-variables")){
        Asprintf(&result_str, "info locals");
    }else if (!strcmp( token, "-exec-step\n")){
        Asprintf(&result_str, "step");
    }else if (!strcmp( token, "-exec-next\n")){
        Asprintf(&result_str, "next");
    }else if (!strcmp( token, "-exec-continue\n")){
        Asprintf(&result_str, "continue");
    }else if (!strcmp( token, "-exec-arguments")){
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
        Asprintf(&result_str, "unknown");
    }else if (!strcmp( token, "-file-exec-and-symbols")){
        token = strtok(nullptr, " ");
        //z-fix remove newline from token
        printf(token);
        std::string app="";
        app.append(std::to_string(launch_count));
        if (!num_ranks.empty()){
            app.append("{");
            app.append(num_ranks);
            app.append("}");
        }
        if (!launcher_args.empty()){
            Asprintf(&result_str, "launch $App%s %s -a \"%s\"", app.c_str(), token, launcher_args.c_str());
        }else{
            Asprintf(&result_str, "launch $App%s %s", app.c_str(), token);
        }
        printf("result_str: %s\n", result_str);
        launch_count++;
    }else if (!strcmp( token, "-list-target-features")){
        return token;
    }else if (!strcmp( token, "-list-features\n")){
        return token;
    }else if(!strcmp(token, "-file-list-exec-source-files\n")){
        Asprintf(&result_str, "info sources");
    }else if(!strcmp(token, "-exec-run\n")){
        Asprintf(&result_str, token);
    }else{
        Asprintf(&result_str, cmd);
    }
    Free(cmd);
    return result_str;
}

char * mi_response(const char * message, const char * type, const char * payload) {
    char * result_str = nullptr;

    if (payload == nullptr){
        Asprintf(&result_str, MI_SIMPLE_RESULT, message);
    }else{
        Asprintf(&result_str, MI_RESULT, message,type,payload);
    }
    return result_str;
}
