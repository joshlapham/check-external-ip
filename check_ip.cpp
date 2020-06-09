#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>

using namespace std;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco::Net;
using namespace Poco;

class StorageFile
{
private:
    std::string _filepath;

public:
    StorageFile(std::string);

    std::string checkIpInStorageFile(std::string response_ip_address)
    {
        string line;
        ifstream storage_file(_filepath);
        Parser parser;
        std::string jsonContents;
        Var result;

        // Read contents
        if (storage_file.is_open())
        {
            while (getline(storage_file, line))
            {
                // cout << line << '\n'; // Debugging
                
                result = parser.parse(line);
            }

            storage_file.close();
        }
        else
        {
            // TODO: Should be throwing an exception here
            cout << "Unable to open file";
            return "Unable to open file";
        }

        Object::Ptr object = result.extract<Object::Ptr>();

        // TODO: Make `lastKnownIpAddress` string a constant
        Var last_known_ip = object->get("lastKnownIpAddress");

        std::string last_known_ip_str = last_known_ip.toString();

        return last_known_ip_str;
    }
};

StorageFile::StorageFile(std::string filepath)
{
    _filepath = filepath;
}

class NoStorageFileException : public exception
{
    virtual const char *what() const throw()
    {
        return "Storage filepath environment variable was not set; a storage filepath is required";
    }
} NoStorageFileException;

std::string fetchPublicIpAddress(std::string url) {
    // Make network request; parse current IP address from response
    URI uri(url);
    const Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
    
    // cout << uri.getHost() << endl;
    // cout << uri.getPort() << endl;

    // Parse URL path; handle if none
    string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);

    session.sendRequest(req);

    // Parse response
    HTTPResponse res;

    cout << res.getStatus() << " " << res.getReason() << endl; // Debugging

    std::string currentPublicIpAddress;
    istream &is = session.receiveResponse(res);
    StreamCopier::copyToString(is, currentPublicIpAddress);

    return currentPublicIpAddress;
}

int main()
{
    // Parse environment variables
    try
    {
        // TODO: Throw exception & stop execution if any of these environment variables are null
        const char *storage_filepath = std::getenv("JL_CHECK_IP_STORAGE_FILEPATH");
        const char *url = std::getenv("JL_CHECK_IP_URL");
        // const char *vpn_profile_filepath = std::getenv("JL_CHECK_IP_VPN_PROFILE_FILEPATH");

        if (storage_filepath == NULL)
        {
            throw NoStorageFileException;
        }

        // Debugging
        // cout << storage_filepath << endl;
        // cout << url << endl;
        // cout << vpn_profile_filepath << endl;
        cout << "Fetching current public IP address .." << endl;

        std::string currentPublicIpAddress = fetchPublicIpAddress(url);

        cout << "Fetched current public IP address: " << currentPublicIpAddress << endl;

        // Load storage file; check last known IP address value in storage file
        StorageFile storage_file_obj(storage_filepath);

        std::string lastKnownIpAddress = storage_file_obj.checkIpInStorageFile("123.456.0");

        cout << "Last known IP (from storage file): " << lastKnownIpAddress << endl;

        // TODO: Finish implementing -- compare IP addresses, act accordingly
        // TODO: Think there might be a line break at the end of `currentPublicIpAddress` value maybe
        bool bothIpAddressesAreTheSame = currentPublicIpAddress == lastKnownIpAddress;

        cout << "IP addresses match: " << bothIpAddressesAreTheSame;

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}