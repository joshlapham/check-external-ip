//
// Created by jl on 11/6/20.
//

#ifndef CPLUSPLUS_STORAGEFILE_H
#define CPLUSPLUS_STORAGEFILE_H

#include <cstdlib>
#include <string>

#include <Poco/Dynamic/Var.h>

class StorageFile {
private:
    std::string _filepath;

    Poco::Dynamic::Var _readFileContents();

public:
    explicit StorageFile(std::string filepath);

    void updateLastKnownIpAddress();
    std::string lastKnownIpAddress();
};

#endif //CPLUSPLUS_STORAGEFILE_H
