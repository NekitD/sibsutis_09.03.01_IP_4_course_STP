#pragma once // Защита от двойного включения
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

vector<string> searchSubstring(
    const vector<string>& text,
    string sub,
    bool i, bool v, bool n, bool w, bool x
);

bool filter(const string& str, const string& sub, bool v, bool w, bool x);

int processFile( 
    string filename, 
    string sub,  
    bool i, bool v, bool n, 
    bool h, bool H, bool c, bool l, 
    bool w, bool x, bool q,
    string& output,
    size_t& match_count
);