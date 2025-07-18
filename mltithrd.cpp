#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <zlib.h>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int numThreads = 4;
vector<vector<char>> compressedChunks(numThreads);
vector<double> threadTimes(numThreads);

void compressChunk(const vector<char>& input, vector<char>& output, int index) {
    auto start = high_resolution_clock::now();

    uLongf destLen = compressBound(input.size());
    output.resize(destLen);

    int res = compress(reinterpret_cast<Bytef*>(&output[0]), &destLen,
                       reinterpret_cast<const Bytef*>(&input[0]), input.size());

    if (res != Z_OK) {
        cerr << "[Thread " << index << "] Compression failed.\n";
        output.clear();
        return;
    }

    output.resize(destLen);
    auto end = high_resolution_clock::now();
    threadTimes[index] = duration<double, std::milli>(end - start).count();

    cout << "[Thread " << index << "] Compressed Chunk " << (index + 1)
         << " in " << static_cast<int>(threadTimes[index]) << " ms\n";
}

int main() {
    string inputFile = "input.txt";
    string outputFile = "compressed.bin";

   
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cerr << " Failed to open input.txt\n";
        return 1;
    }
    vector<char> fileData((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    double inputSizeMB = static_cast<double>(fileData.size()) / (1024 * 1024);
    size_t chunkSize = fileData.size() / numThreads;

    
    vector<vector<char>> chunks(numThreads);
    for (int i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? fileData.size() : start + chunkSize;
        chunks[i] = vector<char>(fileData.begin() + start, fileData.begin() + end);
    }

    cout << "=== Compression Started ===\n";
    cout << "Input File Size: " << fixed << setprecision(1) << inputSizeMB << " MB\n";
    cout << "Number of Threads: " << numThreads << "\n\n";

    
    vector<thread> threads;
    auto mt_start = high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i)
        threads.emplace_back(compressChunk, ref(chunks[i]), ref(compressedChunks[i]), i);

    for (auto& t : threads) t.join();

    auto mt_end = high_resolution_clock::now();
    int mt_duration = static_cast<int>(duration_cast<milliseconds>(mt_end - mt_start).count());

    
    ofstream out(outputFile, ios::binary);
    for (const auto& chunk : compressedChunks) {
        uint32_t size = chunk.size();
        out.write(reinterpret_cast<char*>(&size), sizeof(size));
        out.write(chunk.data(), chunk.size());
    }
    out.close();

    
    auto st_start = high_resolution_clock::now();
    for (const auto& chunk : chunks) {
        vector<char> temp(compressBound(chunk.size()));
        uLongf tempSize = temp.size();
        compress(reinterpret_cast<Bytef*>(temp.data()), &tempSize,
                 reinterpret_cast<const Bytef*>(chunk.data()), chunk.size());
    }
    auto st_end = high_resolution_clock::now();
    int st_duration = static_cast<int>(duration_cast<milliseconds>(st_end - st_start).count());

    double outputSizeMB = static_cast<double>(filesystem::file_size(outputFile)) / (1024 * 1024);
    double speedup = round((double)st_duration / mt_duration * 10) / 10.0;

    cout << "\nTotal Compression Time (Multithreaded): " << mt_duration << " ms\n";
    cout << "Total Compression Time (Single-threaded): " << st_duration << " ms\n";
    cout << "\n Performance Gain: " << speedup << "x Faster Using Threads\n";
    cout << "Compressed Output Size: " << fixed << setprecision(1) << outputSizeMB << " MB\n";
    cout << "=== Compression Complete ===\n";

    return 0;
}
