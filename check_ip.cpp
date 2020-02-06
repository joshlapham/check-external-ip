#include "Poco/JSON/Parser.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;



// def check_ip_in_storage_file(filepath, response_ip_address):
//     with open(filepath, 'r') as storage_file:
//         contents = json.load(storage_file)
//         last_known_ip = contents['lastKnownIpAddress']

//         return last_known_ip

//     return None


string checkIpInStorageFile(string filepath, string response_ip_address) {
    string line;
    ifstream storage_file(filepath);
    Parser parser;
    std::string jsonContents;
    Var result;

    // Read contents
    if (storage_file.is_open()) {
        while (getline(storage_file, line)) {
            // cout << line << '\n';
            result = parser.parse(line);
        }

        storage_file.close();

    } else {
        cout << "Unable to open file";
    }

    cout << result;
}

int main() {
    checkIpInStorageFile('test.txt', '123.456.0');

    return 0;
}