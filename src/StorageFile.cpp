//
// Created by jl on 11/6/20.
//

#include "StorageFile.h"

#include <iostream>
#include <fstream>

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

