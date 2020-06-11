#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include <Poco/Net/HTTPSClientSession.h>

#include <Poco/Logger.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/Message.h>

#include "src/StorageFile.h"
#include "src/NetworkService.h"

using namespace std;
//using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace Poco::Net;
using namespace Poco;

class NoStorageFileException : public std::exception
{
    const char *what() const noexcept override
    {
        return "Storage filepath environment variable was not set; a storage filepath is required";
    }
} NoStorageFileException;

// TODO: Finish implementing this function
void updateIpAddressInStorageFile(const std::string& filepath, const std::string& publicIpAddress) {}

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
void updateVPNConfigFile(const std::string& filepath, const std::string& publicIpAddress) {}

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

        if (storage_filepath == nullptr)
        {
            throw NoStorageFileException;
        }

        // Initialise logger
        auto *pFCConsole = new FormattingChannel(new PatternFormatter("%Y-%m-%d %H:%M:%S.%c %t"));
        pFCConsole->setChannel(new ConsoleChannel);
        pFCConsole->open();
        Logger &logger = Logger::create("ConsoleLogger", pFCConsole, Message::PRIO_INFORMATION);

        NetworkService networkService(url);

        logger.information("Fetching current public IP address ..");

        std::string currentPublicIpAddress = networkService.fetchPublicIpAddress();

        logger.information("Fetched current public IP address: %s", currentPublicIpAddress);

        // Load storage file; check last known IP address value in storage file
        StorageFile storageFile(storage_filepath);
        std::string lastKnownIpAddress = storageFile.lastKnownIpAddress();

        logger.information("Last known IP (from storage file): %s", lastKnownIpAddress);

        // Check if IP addresses match
        bool ipAddressesMatch = currentPublicIpAddress == lastKnownIpAddress;

        logger.information("IP addresses match: %b", ipAddressesMatch);

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