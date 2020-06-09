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

#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/Message.h"

using namespace std;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco::Net;
using namespace Poco;

using Poco::Logger;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::Message;

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

class InvalidHTTPResponseException : public exception
{
    virtual const char *what() const throw()
    {
        return "HTTP response code was not 200";
    }
} InvalidHTTPResponseException;

std::string fetchPublicIpAddress(std::string url) {
    // Make network request; parse current IP address from response
    URI uri(url);
    const Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
    
    // Parse URL path; handle if none
    string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);

    session.sendRequest(req);

    // Parse response
    HTTPResponse res;

    // TODO: Make status code value a constant
    if (res.getStatus() != 200)
    {
        throw InvalidHTTPResponseException;
    }

    std::string currentPublicIpAddress;
    istream &is = session.receiveResponse(res);
    StreamCopier::copyToString(is, currentPublicIpAddress);

    // Strip newline character
    // TODO: Should probably move this logic elsewhere
    currentPublicIpAddress.erase(std::remove(currentPublicIpAddress.begin(), currentPublicIpAddress.end(), '\n'), currentPublicIpAddress.end());

    return currentPublicIpAddress;
}

// TODO: Finish implementing this function
void updateIpAddressInStorageFile(std::string filepath, std::string publicIpAddress) {}

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

// TODO: Finish implementing this function
void updateVPNConfigFile(std::string filepath, std::string publicIpAddress) {}

/*
def update_vpn_config_file(filepath, response_ip_address):
    cli_command = '/bin/sed -i "/remote /c\\remote {} 1194" {}'.format(response_ip_address, filepath)
    subprocess.Popen(cli_command, shell=True)
*/

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

        // Initialise logger
        FormattingChannel *pFCConsole = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %t"));
        pFCConsole->setChannel(new ConsoleChannel);
        pFCConsole->open();
        Logger &logger = Logger::create("ConsoleLogger", pFCConsole, Message::PRIO_INFORMATION);

        logger.information("Fetching current public IP address ..");

        std::string currentPublicIpAddress = fetchPublicIpAddress(url);

        logger.information("Fetched current public IP address: %s", currentPublicIpAddress);

        // Load storage file; check last known IP address value in storage file
        StorageFile storageFile(storage_filepath);

        // TODO: Review this; we don't use the parameter that is passed here
        std::string lastKnownIpAddress = storageFile.checkIpInStorageFile("123.456.0");

        logger.information("Last known IP (from storage file): %s", lastKnownIpAddress);

        // Check if IP addresses match
        bool ipAddressesMatch = currentPublicIpAddress == lastKnownIpAddress;

        logger.information("IP addresses match: %d", ipAddressesMatch);

        // TODO: Implement better exception handling
        // TODO: Update VPN profile file
        // TODO: Send notifications
        /*
        # If different, then update storage file
        if last_known_ip != response_ip_address:
            logger.info('Last known IP does not match current IP!')

            update_ip_in_storage_file(storage_filepath, response_ip_address)

            logger.info('Successfully updated IP address in storage file')

            # Update VPN config file
            logger.info('Updating VPN profile ..')

            update_vpn_config_file(vpn_profile_filepath, response_ip_address)

            logger.info('Successfully updated IP address in VPN profile')

            if args.notify:
                logger.info('Sending notification ..')
                _notify("Updated IP Address", response_ip_address, prowl_title)
        */

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}