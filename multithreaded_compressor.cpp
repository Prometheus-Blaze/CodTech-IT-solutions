#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

const size_t CHUNK_SIZE = 1024 * 1024; // 1MB
mutex io_mutex;

// Simple Run-Length Encoding
string rleCompress(const string &data) {
    string compressed = "";
    for (size_t i = 0; i < data.length(); ++i) {
        int count = 1;
        while (i + 1 < data.length() && data[i] == data[i + 1]) {
            count++;
            i++;
        }
        compressed += data[i] + to_string(count);
    }
    return compressed;
}

string rleDecompress(const string &data) {
    string decompressed = "";
    for (size_t i = 0; i < data.length(); ++i) {
        char ch = data[i];
        string countStr = "";
        while (i + 1 < data.length() && isdigit(data[i + 1])) {
            countStr += data[++i];
        }
        int count = stoi(countStr);
        decompressed.append(count, ch);
    }
    return decompressed;
}

void compressChunk(const string &chunk, string &output, int id) {
    string result = rleCompress(chunk);
    lock_guard<mutex> lock(io_mutex);
    output += result;
    cout << "Thread " << id << " compressed its chunk.\n";
}

void decompressChunk(const string &chunk, string &output, int id) {
    string result = rleDecompress(chunk);
    lock_guard<mutex> lock(io_mutex);
    output += result;
    cout << "Thread " << id << " decompressed its chunk.\n";
}

void processFile(const string &filename, bool compress) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Unable to open file.\n";
        return;
    }

    vector<string> chunks;
    while (!file.eof()) {
        char buffer[CHUNK_SIZE];
        file.read(buffer, CHUNK_SIZE);
        chunks.push_back(string(buffer, file.gcount()));
    }
    file.close();

    vector<thread> threads;
    string finalOutput = "";
    int threadId = 0;

    for (const string &chunk : chunks) {
        if (compress)
            threads.emplace_back(compressChunk, ref(chunk), ref(finalOutput), threadId++);
        else
            threads.emplace_back(decompressChunk, ref(chunk), ref(finalOutput), threadId++);
    }

    for (auto &t : threads) t.join();

    string outFilename = compress ? "compressed.rle" : "decompressed.txt";
    ofstream outFile(outFilename, ios::binary);
    outFile << finalOutput;
    outFile.close();

    cout << "File " << (compress ? "compressed" : "decompressed") << " successfully into " << outFilename << "\n";
}

int main() {
    int choice;
    string filename;

    cout << "=== Multithreaded File Compression Tool ===\n";
    cout << "Enter filename to process: ";
    cin >> filename;

    cout << "1. Compress File\n";
    cout << "2. Decompress File\n";
    cout << "Enter choice: ";
    cin >> choice;

    bool compress = (choice == 1);
    processFile(filename, compress);

    return 0;
}
