#ifndef AUXILIARES_H_INCLUDED
#define AUXILIARES_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;



bool isLabel(string token) {
    if (token[token.length()-1] == ':') {
        return true;
    }
    return false;
}

bool inMap(string token, map <string,int> ts) {
    if (ts.find(token) == ts.end()) {
        return false;
    }
    return true;
}



int getValue(const std::string& str, const std::map<std::string, int>& mp) {
    if (inMap(str, mp)) {
        auto it = mp.find(str);
        return it->second; // Return value associated with str if found in mp
    } else {
        // If str is not found in mp, try to convert str to int
        try {
            return std::stoi(str); // Convert str to int
        } catch (const std::invalid_argument& e) {
            // Handle case where str cannot be converted to int
            std::cerr << "Error: " << e.what() << std::endl;
            // Return a default value or throw an exception as needed
            throw std::runtime_error("Valor invalido para argumento de if - erro sintatico");
        }
    }
}

#endif
