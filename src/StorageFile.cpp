//
// Created by jl on 11/6/20.
//

#include "StorageFile.h"

#include <fstream>

#include <Poco/JSON/Parser.h>

StorageFile::StorageFile(std::string filepath)
{
    _filepath = std::move(filepath);
}

Poco::Dynamic::Var StorageFile::_readFileContents() {
    std::string line;
    std::ifstream storage_file(_filepath);
    Poco::JSON::Parser parser;
    std::string jsonContents;
    Poco::Dynamic::Var result;

    // Read contents
    // TODO: DRY
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

void StorageFile::updateLastKnownIpAddress() {
//    std::string line;
//    std::ifstream storage_file(_filepath);
//    Poco::JSON::Parser parser;

    // Read contents
    Poco::Dynamic::Var result = _readFileContents();

/*
def update_ip_in_storage_file(filepath, response_ip_address):
    try:
        with open(filepath, 'r+') as storage_file:
            contents = json.load(storage_file)
            contents['lastKnownIpAddress'] = response_ip_address
            storage_file.write(json.dumps(contents))

    except ValueError:
        # If we got here, then storage file contents was empty
        print('Storage file contents was empty')

    except OSError:
        # If we got here; then storage file failed to update with new IP address
        print('Failed to update IP address in storage file; overwriting file contents ..')

    finally:
        with open(filepath, 'w') as storage_file:
            contents = {
                "lastKnownIpAddress": response_ip_address
            }

            storage_file.write(json.dumps(contents))
*/

}

std::string StorageFile::lastKnownIpAddress() {
//    std::string line;
//    std::ifstream storage_file(_filepath);
//    Poco::JSON::Parser parser;
//    std::string jsonContents;

    // Read contents
    Poco::Dynamic::Var result = _readFileContents();

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    // TODO: Make `lastKnownIpAddress` string a constant
    Poco::Dynamic::Var last_known_ip = object->get("lastKnownIpAddress");

    std::string last_known_ip_str = last_known_ip.toString();

    return last_known_ip_str;
}
