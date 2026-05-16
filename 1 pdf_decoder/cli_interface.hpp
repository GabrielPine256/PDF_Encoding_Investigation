#ifndef PDF_DECODER_CLI_INTERFACE
#define PDF_DECODER_CLI_INTERFACE
// The functions that process CLI arguments. 

// It is intended that the user has the following definition in main() 
// const vector<string> args = {argv + 1, argv + argc}; 

#include<vector>
#include<string>
#include<map> 
#include<set> 
#include<algorithm> // find() 
#include<fstream>   // ifstream 

namespace PDF_Decoder {
    using std::vector;
    using std::string;
    using std::map; 
    using std::set; 
    using std::ifstream; 

/*
// This function can be overloaded for different default_value types. 
int read_arg(const string& arg, const int default_value, const vector<string>& args) {
    
    int result = default_value; 
    
    auto iter = find(args.begin(), args.end(), arg);
    if (iter != (args.end()-1) && iter != args.end())
        try { result = stoi(*(iter+1)); } catch (const std::invalid_argument&) {}
    
    return result; 
} */

string read_arg(const string& arg, const string& default_value, const vector<string>& args) {
    
    string result = default_value; 
        
    auto iter = find(args.begin(), args.end(), arg);
    if (iter != (args.end()-1) && iter != args.end())
        result = *(iter+1); 
        
    return result; 
} 


string read_file(const string& filename) {
    string result = ""; 
    
    ifstream ifs(filename); 
    if (!ifs) { return result; } 
    
    char c = '\0';
    while (ifs.get(c)) { result += c; }
    
    ifs.close();   
    return result;
}


string to_string(const map<char,set<char>>& encoding) {
    string result = ""; 
    
    for (auto it = encoding.begin(); it != encoding.end(); ++it) {
        if (it->first == '\n') {  result += "\\n\t";            }
        else                   { (result += it->first) += "\t"; }
        for (const char& j : it->second) { 
            if (j == '\n') {  result += "\\n ";    }
            else           { (result += j) += " "; }
        } 
        result += "\n"; 
    }
    
    return result; 
}

} // namespace PDF_Decoder

#endif 