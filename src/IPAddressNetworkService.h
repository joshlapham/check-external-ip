//
// Created by jl on 11/6/20.
//

#ifndef CPLUSPLUS_IPADDRESSNETWORKSERVICE_H
#define CPLUSPLUS_IPADDRESSNETWORKSERVICE_H

#include "NetworkService.h"

class IPAddressNetworkService : public NetworkService {
public:
    std::string fetchPublicIpAddress();

    explicit IPAddressNetworkService(std::string url);
};

#endif //CPLUSPLUS_IPADDRESSNETWORKSERVICE_H
