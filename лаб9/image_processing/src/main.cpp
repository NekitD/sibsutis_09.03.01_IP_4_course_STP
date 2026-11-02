#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

#include "Image.h"

using namespace std;

namespace fs = std::filesystem;

typedef struct results {
    int success{0};
    int failed{0};
} results;


void SeqProcess(const vector<string>&, const string&, int, results&);
void ParProc(const vector<string>&, const string&, int, int, results&);
void Thread(std::queue<string>&, std::mutex&, std::mutex&, const string&, int, results&);

bool isValidOperation(int op);
bool isValidMode(int mode);
string generateOutputPath(const string& inputPath, const string& outDir);

int main() {
    string input = "./images", output = "./processed";
    int operation = 1, mode = 1, nt = 2;
    while(true){
        cout << "Enter input directory: ";
        cin >> input;
        cout << "Enter output directory: ";
        cin >> output;
        cout << "Scanning input directory..." << endl;
        cout << "Scanning input directory..." << endl;
        vector<string> imageFiles;
        for (const auto& entry : fs::directory_iterator(input)) {
            if (entry.is_regular_file()) {
                string ext = entry.path().extension().string();
                transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp" || ext == ".tga" || ext == ".webp") {
                    imageFiles.push_back(entry.path().string());
                }
            }
        }

        if (imageFiles.empty()) {
            cout << "No images found in the input directory. Exiting." << endl;
            return 0; 
        }
        cout << "Found " << imageFiles.size() << " images." << endl;

        cout << "Select processing operation: " << endl;
        cout << "1. Grayscale" << endl;
        cout << "2. Invert" << endl;
        cout << "3. Grayscale + Invert" << endl;
        cout << "Enter operation number: ";
        cin >> operation;
        if(!isValidOperation(operation)){
            cout << "INVALID OPERATION" << endl;
            return 1;
        } 
        cout << "Select processing mode: " << endl;
        cout << "1. Sequential processing" << endl;
        cout << "2. Parallel processing" << endl;
        cout << "Enter mode number: ";
        cin >> mode;
        if(!isValidMode(mode)){
            cout << "INVALID MODE" << endl;
            return 1;
        } 
        if(mode == 2){
            cout << "Enter number of threads: ";
            cin >> nt;
            if (nt <= 0){
                cout << "INVALID NUMBER OF THREADS" << endl;
                return 1;
            }
        }

        results res;
        auto startTime = std::chrono::high_resolution_clock::now();
        switch(mode){
            case 1: SeqProcess(imageFiles, output, operation, res);
            case 2: ParProc(imageFiles, output, operation, nt, res);
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;

        string modeStr = (mode == 1) ? "Sequential" : "Parallel";
        string opStr;
        if (operation == 1) opStr = "Grayscale";
        else if (operation == 2) opStr = "Invert";
        else opStr = "Grayscale + Invert";

        cout << "\n========================================\n";
        cout << "          PROCESSING SUMMARY\n";
        cout << "========================================\n";
        cout << "Mode:                    " << modeStr << "\n";
        cout << "Operation:               " << opStr << "\n";
        if (mode == 2) {
            cout << "Worker threads:          " << nt << "\n";
        }
        cout << "Total images:            " << imageFiles.size() << "\n";
        cout << "Successfully processed:  " << res.success << "\n";
        cout << "Failed:                  " << res.failed << "\n";
        cout << "Processing time:         " << std::fixed << std::setprecision(3) << elapsed.count() << " seconds\n";
        cout << "Results directory:       " << output << "\n";
        cout << "========================================\n";
    }
    return 0;
}

void SeqProcess(const vector<string>& files, const string& outDir, int op, results& res) {
    for (const auto& file : files) {
        Image img;
        if (!img.load(file)) {
            cout << "Failed to load " << file << endl;
            res.failed++;
            continue;
        }

        if (op == 1 || op == 3) img.grayscale();
        if (op == 2 || op == 3) img.invert();

        string outPath = generateOutputPath(file, outDir);
        if (!img.save(outPath)) {
            cout << "Failed to save " << outPath << endl;
            res.failed++;
            continue;
        }

        res.success++;
        cout << "Processed: " << file << endl;
    }
}


void ParProc(const vector<string>& files, const string& outDir, int op, int nt, results& res) {
    std::queue<string> taskQueue;
    for (const auto& file : files) {
        taskQueue.push(file);
    }

    std::mutex queueMutex;
    std::mutex coutMutex;

    std::vector<std::thread> threads;
    for (int i = 0; i < nt; ++i) {
        threads.emplace_back(Thread, 
            std::ref(taskQueue), 
            std::ref(queueMutex), 
            std::ref(coutMutex), 
            std::cref(outDir), 
            op, 
            std::ref(res)
        );
    }

    for (auto& t : threads) {
        t.join();
    }
}

void Thread(
    std::queue<string>& taskQueue,
    std::mutex& queueMutex,
    std::mutex& coutMutex,
    const string& outDir,
    int op,
    results& res
) {
    while (true) {
        string file;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (taskQueue.empty()) break;
            file = taskQueue.front();
            taskQueue.pop();
        }

        Image img;
        if (!img.load(file)) {
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                cout << "Failed to load: " << file << endl;
            }
            res.failed++;
            continue;
        }

        if (op == 1 || op == 3) img.grayscale();
        if (op == 2 || op == 3) img.invert();

        string outPath = generateOutputPath(file, outDir);
        if (!img.save(outPath)) {
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                cout << "Failed to save: " << outPath << endl;
            }
            res.failed++;
            continue;
        }

        res.success++;

        {
            std::lock_guard<std::mutex> lock(coutMutex);
            cout << "Processed: " << file << endl;
        }
    }
}


bool isValidOperation(int op) {
    return op >= 1 && op <= 3;
}

bool isValidMode(int mode) {
    return mode == 1 || mode == 2;
}

string generateOutputPath(const string& inputPath, const string& outDir) {
    fs::path p(inputPath);
    string filename = p.filename().string();
    string newFilename = filename + ".processed.png";
    return (fs::path(outDir) / newFilename).string();
}