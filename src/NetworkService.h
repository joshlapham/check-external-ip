//
// Created by jl on 11/6/20.
//

#ifndef CPLUSPLUS_NETWORKSERVICE_H
#define CPLUSPLUS_NETWORKSERVICE_H

#include <cstdlib>
#include <string>

class NetworkService
{
private:
    std::string _url;

public:
    explicit NetworkService(std::string);
    std::string fetchPublicIpAddress();
};

#endif //CPLUSPLUS_NETWORKSERVICE_H
