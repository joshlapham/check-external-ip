//
// Created by jl on 11/6/20.
//

#ifndef CPLUSPLUS_IPADDRESSSTORAGEFILE_H
#define CPLUSPLUS_IPADDRESSSTORAGEFILE_H

#include "StorageFile.h"

class IPAddressStorageFile : public StorageFile {
private:
    static const std::string JSON_KEY_LAST_KNOWN_IP_ADDRESS;

public:
    void updateLastKnownIpAddress(const std::string& newIpAddress);
    std::string lastKnownIpAddress();

    explicit IPAddressStorageFile(std::string filepath);
};

#endif //CPLUSPLUS_IPADDRESSSTORAGEFILE_H
