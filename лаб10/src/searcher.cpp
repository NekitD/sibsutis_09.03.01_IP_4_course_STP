#include "searcher.h"
#include <fstream>

vector<string> searchSubstring(
    const vector<string>& text,
    string sub,
    bool i, // no register
    bool v, // invert
    bool n, // print num of line
    bool w, // word
    bool x  // full string
){
    vector<string> result;
    
    if (i) {
        transform(sub.begin(), sub.end(), sub.begin(), 
            [](unsigned char c){ return tolower(c); });
        for(size_t idx = 0; idx < text.size(); idx++){
            string str = text[idx];
            transform(str.begin(), str.end(), str.begin(), 
                [](unsigned char c){ return tolower(c); });
        }
    }

    for(size_t idx = 0; idx < text.size(); idx++){
        string str = text[idx];

        if(filter(str, sub, v, w, x)){
            if (n) {
                result.push_back(to_string(idx + 1) + ":" + text[idx]);
            } else {
                result.push_back(text[idx]);
            }
        }
    }
    return result;
}

bool isWord(const string& str, size_t pos, size_t len) {
    if (pos > 0 && str[pos - 1] != ' ') return false;
    if (pos + len < str.size() && str[pos + len] != ' ') return false;
    return true;
}

bool filter(const string& str, const string& sub, bool v, bool w, bool x){
    size_t pos = str.find(sub);
    bool match = false;

    if (x) {
        match = (str == sub);
    } else if (w) {
        while (pos != string::npos) {
            if (isWord(str, pos, sub.size())) {
                match = true;
                break;
            }
            pos = str.find(sub, pos + 1); 
        }
    } else {
        match = (pos != string::npos);
    }

    return v ? !match : match;
}

int processFile( 
    string filename, 
    string sub,  
    bool i, bool v, bool n, 
    bool h, bool H, bool c, bool l, 
    bool w, bool x, bool q,
    string& output,
    size_t& match_count
)
{
    output = "";
    vector<string> text;
    
    ifstream fin(filename);
    if (!fin.is_open()) {
        return 2;
    }
    string line;
    while (getline(fin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        text.push_back(line);
    }
    fin.close();

    vector<string> result = searchSubstring(text, sub, i, v, n, w, x);
    size_t count = result.size();
    match_count = count;

    if (q) return (count > 0) ? 0 : 1;

    if (l) {
        if (count > 0) {
            output += filename + "\n";
            return 0;
        }
        return 1;
    }

    if (c) {
        output += filename + ":" + to_string(count) + "\n";
        return (count > 0) ? 0 : 1;
    }

    if (count > 0) {
        if (h && !H) output += filename + ":";
        for (const string& str : result) {
            output += str + "\n";
        }
        return 0;
    }

    return 1;
}