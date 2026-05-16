//  g++ main.cpp decoder.hpp cli_interface.hpp -o pdf_decoder && ./pdf_decoder

#include"decoder.hpp" 

#include"cli_interface.hpp" // to_string() 

#include<iostream>

using namespace PDF_Decoder; 
using namespace std;

int main(int argc, char *argv[]) {
    const vector<string> args = {argv + 1, argv + argc}; 

    auto encoding = get_encoding(args); 
    cout << to_string(encoding) << "\n";     
    cout << decode(encoding, args) << "\n";     
    
    return 0;
}



    
