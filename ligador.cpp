#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include "tokenizer.h"

using namespace std;

// para compilar
// g++ -o ligador ligador.cpp
// ./ligador prog1.obj

tuple<std::multimap<std::string, int>, std::map<std::string, int>, vector<int>, vector<int>> parseFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::multimap<std::string, int> USO;
    std::map<std::string, int> DEF;
    std::string currentTag;
    vector<int> objeto;
    vector<int> real;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }

        std::istringstream iss(line);
        std::string tag;
        tag = iss.str();

        if (tag == "USO" || tag == "DEF" || tag == "REAL" || tag == "OBJ") {
            currentTag = tag;
            continue; // Skip to the next line
        }

        if (currentTag == "USO" || currentTag == "DEF" || currentTag == "REAL" || currentTag == "OBJ") {
            std::string key;
            string linha;
            int value;
            if (currentTag == "USO") {
                while (iss >> key >> value) {
                    USO.insert(std::make_pair(key, value));
                }
            } else if (currentTag == "DEF") {
                while (iss >> key >> value) {
                    DEF[key] = value;
                }
            }
            else if(currentTag == "REAL"){
                linha = iss.str();
                for (char bit:linha){
                    int bitValue = (bit == '0') ? 0 : 1;
                    // Add to the vector
                    real.push_back(bitValue);
                }
            }
            else if(currentTag == "OBJ"){
                int item;
                while (iss >> item){
                    objeto.push_back(item);
                }
            }
        }
    }

    return make_tuple(USO, DEF, real, objeto);
}



int main(int argc, char* argv[]) {
    multimap<string, int> USO;
    map<string, int> DEF;
    vector<int> REAL;
    vector<int> OBJETO;

    if (argc < 2) {
    cout << "ERRO: NAO FOI PASSADO ARGUMENTO PARA O LIGADOR." << endl;
    return 0;
    }

    vector<string> arquivos_obj;

    string arquivo = argv[1];

    auto results = parseFile(arquivo);
    std::tie(USO, DEF, REAL, OBJETO) = results;

    std::cout << "USO:\n";
    for (const auto& pair : USO) {
        std::cout << "(" << pair.first << ", " << pair.second << ")\n";
    }

    std::cout << "\nDEF:\n";
    for (const auto& pair : DEF) {
        std::cout << "(" << pair.first << ", " << pair.second << ")\n";
    }

    std::cout << "\nREAL:\n";
    for (int num : REAL) {
        std::cout << num << " ";
        }
    std::cout << std::endl;;

    std::cout << "\nOBJ:\n";
    for (int num : OBJETO) {
        std::cout << num << " ";
        }
    std::cout << std::endl;;


    return 0;
}
