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
        createFilePre(programa, file_name, ".PRE");
        return 0;
    }
    if(op == "-o"){ // .OBJ
        //preprocess(programa); comentado se devemos rodar em etapas // op -o deve receber arquivo .PRE
        //objeto = singlePass(programa);
        //createFileObj(objeto, file_name, ".OBJ");
        return 0;
    }
}
