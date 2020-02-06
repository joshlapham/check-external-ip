#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string line;
    ifstream file("test.txt");

    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << '\n';
        }

        file.close();

    } else {
        cout << "Unable to open file";
    }

    return 0;
}