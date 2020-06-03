#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

using namespace std;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

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
                cout << line << '\n';
                result = parser.parse(line);
            }

            storage_file.close();
        }
        else
        {
            cout << "Unable to open file";
            return "Unable to open file";
        }

        Object::Ptr object = result.extract<Object::Ptr>();

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

int main()
{
    // Parse environment variables
    // TODO: Throw exception & stop execution if any of these environment variables are null
    try
    {
        const char *url = std::getenv("JL_CHECK_IP_URL");
        const char *storage_filepath = std::getenv("JL_CHECK_IP_STORAGE_FILEPATH");
        const char *vpn_profile_filepath = std::getenv("JL_CHECK_IP_VPN_PROFILE_FILEPATH");

        if (storage_filepath == NULL)
        {
            throw NoStorageFileException;
        }

        cout << url << endl;
        cout << storage_filepath << endl;
        cout << vpn_profile_filepath << endl;

        StorageFile storage_file_obj(storage_filepath);

        std::string last_known_ip = storage_file_obj.checkIpInStorageFile("123.456.0");

        cout << "Last known IP: " << last_known_ip << endl;

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}