//
// Created by jl on 11/6/20.
//

#ifndef CPLUSPLUS_STORAGEFILE_H
#define CPLUSPLUS_STORAGEFILE_H

#include <cstdlib>
#include <string>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>

class StorageFile {
private:
    std::string _filepath;

    Poco::Dynamic::Var _readFileContents();
    void _writeFileContents(const Poco::JSON::Object::Ptr& object);

public:
    explicit StorageFile(std::string filepath);

    // TODO: Maybe subclass `StorageFile` -- specific keys in JSON storage file
    void updateLastKnownIpAddress(const std::string& newIpAddress);
    std::string lastKnownIpAddress();
};

#endif //CPLUSPLUS_STORAGEFILE_H
