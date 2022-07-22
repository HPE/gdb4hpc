#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"

//PartNode root=nullptr;
std::vector<std::string> sourceList;

void StoreSourcePath(char * source){
    if (source != nullptr){
    sourceList.push_back(std::string(source));
    }
}

std::string GetFullPath(const std::string& relativePath){
    if (!relativePath.empty() && !sourceList.empty()){
        for(auto &it : sourceList){
            if (it.find(relativePath) != std::string::npos){
                return it;
            }
        }
    }
    PrintOutError("Could not retrieve full path of file: %s\n", relativePath.c_str());
    return relativePath;
}
