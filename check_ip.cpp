#include <iostream>
#include <fstream>
#include <string>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

using namespace std;
using namespace Poco::JSON;
using namespace Poco::Dynamic;


// def check_ip_in_storage_file(filepath, response_ip_address):
//     with open(filepath, 'r') as storage_file:
//         contents = json.load(storage_file)
//         last_known_ip = contents['lastKnownIpAddress']

//         return last_known_ip

//     return None


std::string checkIpInStorageFile(std::string filepath, std::string response_ip_address) {
    string line;
    ifstream storage_file(filepath);
    Parser parser;
    std::string jsonContents;
    Var result;

    // Read contents
    if (storage_file.is_open()) {
        while (getline(storage_file, line)) {
            cout << line << '\n';
            result = parser.parse(line);
        }

        storage_file.close();

    } else {
        cout << "Unable to open file";
        return "Unable to open file";
    }

    // cout << result;

    Object::Ptr object = result.extract<Object::Ptr>();

    Var last_known_ip = object->get("lastKnownIpAddress");

    std::string last_known_ip_str = last_known_ip.toString();

    return last_known_ip_str;
}

int main() {
    std::string last_known_ip = checkIpInStorageFile("check_ip.json", "123.456.0");

    cout << "Last known IP: " << last_known_ip << endl;

    return 0;
}