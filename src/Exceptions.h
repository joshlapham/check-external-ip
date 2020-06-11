//
// Created by jl on 11/6/20.
//

#ifndef CPLUSPLUS_EXCEPTIONS_H
#define CPLUSPLUS_EXCEPTIONS_H

#include <cstdlib>
#include <iostream>

class InvalidHTTPResponseException : public std::exception
{
    const char *what() const noexcept override
    {
        return "HTTP response code was not 200";
    }
} InvalidHTTPResponseException;

#endif //CPLUSPLUS_EXCEPTIONS_H
