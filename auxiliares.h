#ifndef AUXILIARES_H_INCLUDED
#define AUXILIARES_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;


vector<string> separateOperands(string ops) {
    string tmp;
    stringstream ss(ops);
    vector<string> words;

    while(getline(ss, tmp, ',')) {
        words.push_back(tmp);
    }

    return words;
}


bool isLabel(string token) {
    if (token[token.length()-1] == ':') {
        return true;
    }
    return false;
}

bool inMap(const std::string& token, const std::map<std::string, int>& ts) {
    try {
        ts.at(token);
        return true;
    } catch (const std::out_of_range&) {
        return false;
    }
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
            cout << ("VALOR INVALIDO PARA ARGUMENTO DE IF - ERRO SINTATICO");
        }
    }
}

bool isValidLabel(string token) {
    if (token.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != string::npos) {
        return false;
    }
    return true;
}
#endif
