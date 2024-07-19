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
    map<string,int> DEF_geral;
    vector<int> OBJ_final;
    int current_file_displacement = 0 ;
    string arquivo_final;

    if (argc < 2) {
    cout << "ERRO: NAO FOI PASSADO ARGUMENTO PARA O LIGADOR." << endl;
    return 0;
    }

    // CRIACAO DE TABELA DE DEFINICOES GERAL
    for (int i = 1; i < argc; ++i) {
         string arquivo = argv[i];
        if ( i ==1 ){
            arquivo_final = arquivo;
        }
        auto results = parseFile(arquivo);
        std::tie(USO, DEF, REAL, OBJETO) = results;
        for (const auto& pair : DEF) {
            const string& key = pair.first;
            int originalValue = pair.second;

            // Calculate new value by adding variableToAdd
            int newValue = originalValue + current_file_displacement;
            cout  << key << " " << originalValue << " " << current_file_displacement;
            // Store the updated value in resultMap
            DEF_geral[key] = newValue;
        }
        current_file_displacement += OBJETO.size();
    }

    // SUBSTITUICAO NOS OBJETOS UTILIZANDO TABELA DE USO
    for (int i = 1; i < argc; ++i) {
        string arquivo = argv[i];
        auto results = parseFile(arquivo);
        std::tie(USO, DEF, REAL, OBJETO) = results;
        for (const auto& pair : USO) {
            OBJETO[pair.second] = DEF_geral[pair.first];
        }
        OBJ_final.insert(OBJ_final.end(), OBJETO.begin(), OBJETO.end());
    }
    create::createFileExe(OBJ_final, arquivo_final,".e");
    return 0;
}
