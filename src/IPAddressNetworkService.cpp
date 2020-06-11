//
// Created by jl on 11/6/20.
//

#include <utility>

#include <Poco/AutoPtr.h>

#include "IPAddressNetworkService.h"

IPAddressNetworkService::IPAddressNetworkService(std::string url) : NetworkService(std::move(url)) {
    // TODO: Need to do anything here?
}

std::string IPAddressNetworkService::fetchPublicIpAddress() {
    // Make network request; parse current IP address from response
    std::string currentPublicIpAddress = _makeHTTPSGETRequest();

    // Strip newline character
    currentPublicIpAddress.erase(std::remove(currentPublicIpAddress.begin(), currentPublicIpAddress.end(), '\n'), currentPublicIpAddress.end());

    return currentPublicIpAddress;
}
