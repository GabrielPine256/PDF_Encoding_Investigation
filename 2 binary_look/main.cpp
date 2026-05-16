// g++ main.cpp -o binary_look
// ./binary_look sample_ciphertext.txt sample_plaintext.txt

#include <iostream>
#include <fstream>
#include<vector> 
#include <string>
#include <bitset>


constexpr int panel = 40;

using namespace std; 

int main(int argc, char* argv[]) {
    const vector<string> args = {argv + 1, argv + argc}; 

    if (args.size() < 2) {
        cerr << "Usage: " << argv[0] << " <path/to/ciphertext> <path/to/plaintext>" << '\n'; 
        return 1;
    }

    ifstream ciphertext(args[0]);
    ifstream plaintext(args[1]);
    ofstream output("output.txt");
    
    if (!ciphertext.is_open() || !plaintext.is_open()) {
        cerr << "Error: Could not open input files" << '\n';
        return 1;
    }
    
    if (!output.is_open()) {
        cerr << "Error: Could not open output.txt" << '\n'; 
        return 1;
    }

    string left = "";
    string right = "";
    char c1 = '\0', c2 = '\0';

while (true) {
    bool read1 = (bool)ciphertext.get(c1);
    bool read2 = (bool)plaintext.get(c2);
    
    if (!read1 && !read2) break;
    
    if (read1) {
        left += bitset<8>(static_cast<unsigned char>(c1)).to_string();
    }
    if (read2) {
        right += bitset<8>(static_cast<unsigned char>(c2)).to_string();
    }
    
    while ((left.size() >= panel || right.size() >= panel) && (!left.empty() || !right.empty())) {
        string l = (left.size() >= panel) ? left.substr(0, panel) : left;
        string r = (right.size() >= panel) ? right.substr(0, panel) : right;
        
        l.resize(panel, ' ');
        r.resize(panel, ' ');
        
        output << l << " | " << r << '\n';
        
        left = (left.size() > panel) ? left.substr(panel) : "";
        right = (right.size() > panel) ? right.substr(panel) : "";
    }
}
    
    // Handle remaining characters
    while (!left.empty() || !right.empty()) {
        string l = (left.size() >= panel) ? left.substr(0, panel) : left;
        string r = (right.size() >= panel) ? right.substr(0, panel) : right;
        
        l.resize(panel, ' ');
        r.resize(panel, ' ');
        
        output << l << " | " << r << endl;
        
        left = (left.size() > panel) ? left.substr(panel) : "";
        right = (right.size() > panel) ? right.substr(panel) : "";
    }
    
    ciphertext.close();
    plaintext.close();
    output.close();
    return 0;
}

