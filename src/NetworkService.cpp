//
// Created by jl on 11/6/20.
//

#include "NetworkService.h"

#include <utility>

#include <Poco/URI.h>
#include <Poco/AutoPtr.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Path.h>

#include "Exceptions.h"

NetworkService::NetworkService(std::string url)
{
    _url = std::move(url);
}

std::string NetworkService::_makeHTTPSGETRequest() {
    Poco::URI uri(_url);
    const Poco::AutoPtr<Poco::Net::Context> context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
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

    std::string responseStr;
    std::istream &is = session.receiveResponse(res);
    Poco::StreamCopier::copyToString(is, responseStr);

    return responseStr;
}
