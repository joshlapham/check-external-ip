//
// Created by jl on 11/6/20.
//

#include "StorageFile.h"

#include <fstream>

#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

StorageFile::StorageFile(std::string filepath)
{
    _filepath = std::move(filepath);
}

std::string StorageFile::lastKnownIpAddress() {
    std::string line;
    std::ifstream storage_file(_filepath);
    Poco::JSON::Parser parser;
    std::string jsonContents;
    Poco::Dynamic::Var result;

    // Read contents
    if (storage_file.is_open())
    {
        while (getline(storage_file, line))
        {
            result = parser.parse(line);
        }

        storage_file.close();
    }
    else
    {
        // TODO: Should be throwing an exception here
//        cout << "Unable to open file";
        return "Unable to open file";
    }

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    // TODO: Make `lastKnownIpAddress` string a constant
    Poco::Dynamic::Var last_known_ip = object->get("lastKnownIpAddress");

    std::string last_known_ip_str = last_known_ip.toString();

    return last_known_ip_str;
}
