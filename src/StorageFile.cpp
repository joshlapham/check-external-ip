//
// Created by jl on 11/6/20.
//

#include "StorageFile.h"

#include <iostream>
#include <fstream>

// TODO: How to get rid of clang tidy warning here?
const std::string StorageFile::JSON_KEY_LAST_KNOWN_IP_ADDRESS = "lastKnownIpAddress";

StorageFile::StorageFile(std::string filepath)
{
    _filepath = std::move(filepath);
}

void StorageFile::_writeFileContents(const Poco::JSON::Object::Ptr& object) {
    std::ostringstream oss;
    Poco::JSON::Stringifier::stringify(object, oss);

    std::ofstream storage_file(_filepath);

    if (storage_file.is_open()) {
        storage_file << oss.str();
        storage_file.close();
    }
}

Poco::Dynamic::Var StorageFile::_readFileContents() {
    std::string line;
    std::ifstream storage_file(_filepath);
    Poco::JSON::Parser parser;
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
        // TODO: Should be throwing an exception here; or returning something else
//        cout << "Unable to open file";
        return "Unable to open file";
    }

    return result;
}

void StorageFile::updateLastKnownIpAddress(const std::string& newIpAddress) {
    Poco::Dynamic::Var result = _readFileContents();
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    object->set(JSON_KEY_LAST_KNOWN_IP_ADDRESS, newIpAddress);

    _writeFileContents(object);
}

std::string StorageFile::lastKnownIpAddress() {
    Poco::Dynamic::Var result = _readFileContents();
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    Poco::Dynamic::Var last_known_ip = object->get(JSON_KEY_LAST_KNOWN_IP_ADDRESS);

    std::string last_known_ip_str = last_known_ip.toString();

    return last_known_ip_str;
}
