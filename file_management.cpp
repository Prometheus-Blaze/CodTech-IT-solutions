#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void writeToFile(const string &filename) {
    ofstream file(filename); // opens in write mode (overwrites)
    if (!file) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    string data;
    cout << "Enter data to write to the file (type 'END' to finish):\n";
    cin.ignore(); // clear input buffer
    while (getline(cin, data)) {
        if (data == "END") break;
        file << data << endl;
    }

    file.close();
    cout << "Data written successfully.\n";
}

void appendToFile(const string &filename) {
    ofstream file(filename, ios::app); // opens in append mode
    if (!file) {
        cout << "Error opening file for appending!" << endl;
        return;
    }

    string data;
    cout << "Enter data to append (type 'END' to finish):\n";
    cin.ignore();
    while (getline(cin, data)) {
        if (data == "END") break;
        file << data << endl;
    }

    file.close();
    cout << "Data appended successfully.\n";
}

void readFromFile(const string &filename) {
    ifstream file(filename); // opens in read mode
    if (!file) {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    string line;
    cout << "\n--- File Contents ---\n";
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
    cout << "--- End of File ---\n";
}

int main() {
    string filename = "data.txt";
    int choice;

    do {
        cout << "\n=== File Management Tool ===\n";
        cout << "1. Write to file (overwrite)\n";
        cout << "2. Append to file\n";
        cout << "3. Read file\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeToFile(filename);
                break;
            case 2:
                appendToFile(filename);
                break;
            case 3:
                readFromFile(filename);
                break;
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
