//
// Created by jl on 11/6/20.
//

#include "NetworkService.h"
#include "Exceptions.h"

#include <utility>

#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Format.h>
#include <Poco/StreamCopier.h>

NetworkService::NetworkService(std::string url)
{
    _url = std::move(url);
}

std::string NetworkService::fetchPublicIpAddress() {
    // Make network request; parse current IP address from response
    Poco::URI uri(_url);
    const Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);

    // Parse URL path; handle if none
    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);

    session.sendRequest(req);

    // Parse response
    Poco::Net::HTTPResponse res;

    // TODO: Make status code value a constant
    if (res.getStatus() != 200)
    {
        throw InvalidHTTPResponseException;
    }

    std::string currentPublicIpAddress;
    std::istream &is = session.receiveResponse(res);
    Poco::StreamCopier::copyToString(is, currentPublicIpAddress);

    // Strip newline character
    // TODO: Should probably move this logic elsewhere
    currentPublicIpAddress.erase(std::remove(currentPublicIpAddress.begin(), currentPublicIpAddress.end(), '\n'), currentPublicIpAddress.end());

    return currentPublicIpAddress;
}
