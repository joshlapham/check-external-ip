//
// Created by jl on 11/6/20.
//

#include "IPAddressStorageFile.h"

#include <utility>

// TODO: How to get rid of clang tidy warning here?
const std::string IPAddressStorageFile::JSON_KEY_LAST_KNOWN_IP_ADDRESS = "lastKnownIpAddress";

IPAddressStorageFile::IPAddressStorageFile(std::string filepath) : StorageFile(std::move(filepath)) {
    // TODO: Need to do anything here?
}

void IPAddressStorageFile::updateLastKnownIpAddress(const std::string& newIpAddress) {
    Poco::Dynamic::Var result = _readFileContents();
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    object->set(JSON_KEY_LAST_KNOWN_IP_ADDRESS, newIpAddress);

    _writeFileContents(object);
}

std::string IPAddressStorageFile::lastKnownIpAddress() {
    Poco::Dynamic::Var result = _readFileContents();
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    Poco::Dynamic::Var last_known_ip = object->get(JSON_KEY_LAST_KNOWN_IP_ADDRESS);

    std::string last_known_ip_str = last_known_ip.toString();

    return last_known_ip_str;
}
