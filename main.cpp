#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>


#include "tokenizer.h"
#include "auxiliares.h"
#include "stages.h"



using namespace std;



int main(int argc, char *argv[]){
    std::string op = argv[1];
    std::string arquivo = argv[2];
    std::string file_name = arquivo.substr(0, arquivo.find_last_of('.'));
    std::string extension;
    string objeto;

    vector<vector<string>> programa = tokenParser(arquivo);
    if(op == "-p"){ // .PRE
        preprocess(programa);
        create::createFilePre(programa, file_name, ".pre");
        return 0;
    }
    if(op == "-o"){ // .OBJ

        vector<int> objeto;
        multimap <string,int> tuso;
        map <string,int> tdef;
        vector<int> real;
        int ligacao;

        preprocess(programa); // op -o deve receber arquivo .PRE comentar essa linha no final
        auto results = singlePass(programa);
        std::tie(objeto, tuso, tdef, real, ligacao) = results;
        create::createFileObject(objeto, tuso, tdef, real, ligacao, file_name,".obj");
        return 0;
    }
}
