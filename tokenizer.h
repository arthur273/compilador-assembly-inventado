#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>

#include "auxiliares.h"

using namespace std;

// tirar comentarios
// tirar espacos desnecessarios
// tirar enters desnecessarios - se tem label: seguido de enter
// transformar numeros hexa para decimal

bool apenasLabel (vector<string> linha){
    if ((linha.size() == 1) && (isLabel(linha[0]))){
        return true;
    } else{
        return false;
    }
}


vector<vector<string>> tokenParser(string arquivo){
    ifstream file(arquivo);
    string str;
    string label;
    bool label_flag = false;
    vector<vector<string>> programa;
    vector<string> linha;
    string hex_prefix = "0X";
    while (getline(file, str)) {
        transform(str.begin(), str.end(), str.begin(), ::toupper); // tranforma em maiusculo
        istringstream iss(str.substr(0, str.find(';'))); // remove comentarios. Possivel erro: é possivel que haja espaço a mais no final da linha
        string token;

        if (str.length() != 0){ // Ignora linhas vazias

            while (getline(iss, token, ' ')){
                // token = token.c_str();
                token.erase(std::remove(token.begin(), token.end(), ' '), token.end());

                if (!token.empty()){ // remove espaços desnecessarios

                    if (token.find(hex_prefix, 0) == 0){
                        token = token.substr(hex_prefix.length());
                        token = to_string(stoul(token, nullptr, 16));
                    }
                    if (label_flag){
                        label_flag = false;
                        linha.push_back(label);
                    }
                    linha.push_back(token);

                }
            }
            if (apenasLabel(linha)){
                label_flag = true;
                label = linha[0];
                linha.clear();
                //continue;
            }
            if (linha.size() != 0 && !label_flag){ // Como eliminamos na linha 18, precisamos checar se a linha nao é vazia
                programa.push_back(linha);
                linha.clear();
            }

        }
    }
    return programa;

}

#endif //TOKENIZER_H_INCLUDED
