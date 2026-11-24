#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include "searcher.h"

using namespace std;
namespace fs = std::filesystem;

struct SearchOptions {
    string sub;
    bool i = false, v = false, n = false, h = false, H = false, 
         c = false, l = false, w = false, x = false, q = false, r = false;
};

struct SharedState {
    queue<string> file_queue;
    mutex queue_mtx;
    // condition_variable queue_cv;
    // bool producer_done = false;

    mutex output_mtx;
    string global_output;
    int final_exit_code = 1; 

    mutex statistic_mtx;
    size_t total_files = 0;
    size_t total_matches = 0;
    size_t total_errors = 0;
};

void worker_thread(SharedState& state, const SearchOptions& opts) {
    while (true) {
        string filepath;
        {
            lock_guard<mutex> lock(state.queue_mtx);
            // state.queue_cv.wait(lock, [&state] { 
            //     return !state.file_queue.empty() || state.producer_done; 
            // });

            if (state.file_queue.empty()) {
                break;
            }

            filepath = state.file_queue.front();
            state.file_queue.pop();
        }

        string out;
        size_t matches = 0;
        int code = processFile(filepath, opts.sub, opts.i, opts.v, opts.n, 
                               opts.h, opts.H, opts.c, opts.l, opts.w, opts.x, opts.q, 
                               out, matches);
        
        {
            lock_guard<mutex> lock(state.output_mtx);
            state.global_output += out;
        }
        
        {
            lock_guard<mutex> lock(state.statistic_mtx);
            state.total_files++;
            state.total_matches += matches;
            if (code == 2) {
                state.total_errors++;
                state.final_exit_code = 2;
                //cout << "Error: Could not open file " << filepath << "\n";
            } else if (code == 0 && state.final_exit_code != 2) {
                state.final_exit_code = 0;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SearchOptions opts{};
    int num_threads = 1;
    string search_path = "";
    bool path_found = false;
    bool sub_found = false;
    bool parse_options = true;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (parse_options && arg == "--") {
            parse_options = false;
            continue;
        }

        if (parse_options && !arg.empty() && arg[0] == '-') {
            if (arg == "-i") opts.i = true;
            else if (arg == "-v") opts.v = true;
            else if (arg == "-n") opts.n = true;
            else if (arg == "-h") opts.h = true;
            else if (arg == "-H") opts.H = true;
            else if (arg == "-c") opts.c = true;
            else if (arg == "-l") opts.l = true;
            else if (arg == "-w") opts.w = true;
            else if (arg == "-x") opts.x = true;
            else if (arg == "-q") opts.q = true;
            else if (arg == "-r") opts.r = true;
            else if (arg == "-j") {
                if (i + 1 < argc) {
                    num_threads = stoi(argv[++i]);
                } else {
                    cerr << "Error: -j requires an argument\n";
                    return 2;
                }
            } else {
                cerr << "Error: Unknown option " << arg << "\n";
                return 2;
            }
        } else {
            if (!sub_found) {
                opts.sub = arg;
                sub_found = true;
            } else if (!path_found) {
                search_path = arg;
                path_found = true;
            } else {
                cerr << "Error: Too many arguments\n";
                return 2;
            }
        }
    }

    if (!sub_found) {
        cerr << "Usage: pgrep [OPTIONS] <substring> [path]\n";
        return 2;
    }
    
    if (!path_found) {
        search_path = "./files/";
    }

    if (num_threads < 1) num_threads = 1;

    SharedState state;
    auto start_time = chrono::high_resolution_clock::now();
    

    fs::path root_path("./files/" + search_path);

    if (fs::exists(root_path)) {
        if (opts.r) {
            auto options = fs::directory_options::skip_permission_denied;
            for (const auto& entry : fs::recursive_directory_iterator(root_path, options)) {
                if (entry.is_regular_file()) {
                    {
                        lock_guard<mutex> lock(state.queue_mtx);
                        state.file_queue.push(entry.path().string());
                    }
                }
            }
        } else {
            if (fs::is_regular_file(root_path)) {
                {
                    lock_guard<mutex> lock(state.queue_mtx);
                    state.file_queue.push(root_path.string());
                }
            } else if (fs::is_directory(root_path)) {
                for (const auto& entry : fs::directory_iterator(root_path)) {
                    if (entry.is_regular_file()) {
                        {
                            lock_guard<mutex> lock(state.queue_mtx);
                            state.file_queue.push(entry.path().string());
                        }
                    }
                }
            }
        }
    } else {
        cout << "Error: Path does not exist: " << search_path << "\n";
        state.final_exit_code = 2;
    }

    vector<thread> workers;
    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back(worker_thread, ref(state), cref(opts));
    }

    for (auto& t : workers) {
        if (t.joinable()) t.join();
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    if (!opts.q) {
        cout << state.global_output;
    }

    cout << endl;
    cout << endl;
    cout << "Processed: " << state.total_files << " files, " 
         << state.total_matches << " matches, " 
         << state.total_errors << " errors, " 
         << elapsed.count() << " s\n";
         
    return state.final_exit_code;
}