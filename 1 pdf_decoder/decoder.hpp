#ifndef PDF_DECODER
#define PDF_DECODER

#include"cli_interface.hpp"

#include<vector>
#include<string>
#include<map> 
#include<set> 

namespace PDF_Decoder {
    using std::vector;
    using std::string;
    using std::map; 
    using std::set; 
    
    map<char,set<char>> get_encoding(const string& cipher, const string& plain) {
        map<char,set<char>> result = {}; 
        for (int i = 0; i < cipher.size() && i < plain.size(); ++i) 
            result[cipher[i]].insert(plain[i]); 
        return result; 
    } 
    map<char,set<char>> get_encoding(const vector<string>& args) { 
        return get_encoding(read_file(read_arg("--sample_ciphertext", "sample_ciphertext.txt", args)), \
                            read_file(read_arg("--sample_plaintext",  "sample_plaintext.txt", args)) ); 
    }
    
    
    string decode(const map<char,set<char>>& encoding, const string& ciphertext) {
        string plaintext = ""; 
        for (const char c : ciphertext) 
            if (encoding.count(c)) 
                plaintext += *( ( encoding.at(c) ).begin() ); 
            else
                plaintext += "_";
        return plaintext; 
    }
    string decode(const map<char,set<char>>& encoding, const vector<string>& args) { return decode(encoding, read_file(read_arg("--ciphertext", "ciphertext.txt", args))); }
    
    
} // namespace PDF_Decoder

#endif