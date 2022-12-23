#include <iostream>
#include <fstream>
#include <algorithm>
#include "counting.h"
#include "treatment_of_data.h"


// Função para realizar contagem e criar o banco de dados.
std::vector<FileInfo> count_lines(std::vector<std::string> src_files){
  std::vector<FileInfo> db;
  FileInfo file;
  std::vector<std::string>::iterator iter;
  for(iter = src_files.begin(); iter != src_files.end(); iter++){
    //Testa se o arquivo existe.
      if (file_exists(*iter)){
        // Preenche a base de dados.
        db.push_back(counting(*iter));
      }
  }
  return db;
}

//Essa função realiza a contagem das linhas (em branco, de código, de comentario) e retorna um FileInfo com essas informações.
FileInfo counting(std::string nome_arquivo){
    std::fstream arquivo;
    std::string linha;
    FileInfo file;
    bool flag{false};
    
    arquivo.open(nome_arquivo, std::ios::in);
    
    if(arquivo.is_open()){
      file.filename = nome_arquivo;
      if(nome_arquivo.rfind(".cpp") != std::string::npos && (nome_arquivo.length() - nome_arquivo.rfind(".cpp")) == 4) file.type = CPP;
        else if(nome_arquivo.rfind(".c") != std::string::npos && (nome_arquivo.length() - nome_arquivo.rfind(".c")) == 2) file.type = C;
        else if(nome_arquivo.rfind(".hpp") != std::string::npos && (nome_arquivo.length() - nome_arquivo.rfind(".hpp")) == 4) file.type = HPP;
        else if(nome_arquivo.rfind(".h") != std::string::npos && (nome_arquivo.length() - nome_arquivo.rfind(".h")) == 2) file.type = H;
        else file.type = UNDEF;
      // Percore o arquivo linha a linha.
      while(getline(arquivo, linha)){
        // Retira todos os espaços em branco da string.
        linha.erase(std::remove_if(linha.begin(), linha.end(), ::isspace), linha.end());
          if(count_blank(linha) && flag == false){
              file.n_blank++;
          }
          else{
            if(count_comment(linha, &flag, &file.n_comments)){
              file.n_comments++; 
            }
            if(count_comment(linha) && flag == false){
              file.n_comments++;
            }
            if(count_code(linha) && flag == false){
              file.n_loc++;
            }
          } 
        // Conta as linhas totais do arquivo.
        file.n_lines++;
      }
    }
    else{
      std::cout << "Erro, não foi possível abrir o arquivo." << nome_arquivo << std::endl;
      return (FileInfo)NULL;
    }

  arquivo.close();
  
  return file;
}

// Testa se a string está vazia.
bool count_blank(std::string l){
  return l.empty();
}

// Testa se existe um comentário do tipo "/**/" e conta suas linhas.  
bool count_comment(std::string l, bool* f, unsigned long* comment){
  for(unsigned int i{0}; i<l.size(); i++){
      if(l[i] == '/' && l[i+1] == '*'){
        *f = true;
      }
      if(f){
        if(l[i] == '*' && l[i+1] == '/'){
          *comment += 1;
          *f = false;
        }
      }
  }

  return *f;
}

// Testa se existe cometário do tipo "//" e conta suas linhas.
bool count_comment(std::string l){
    bool b{false};
    for(unsigned int i=0; i<l.size(); i++){
        if(l[i] == '/' && l[i+1] == '/'){
            b = true;
        }
  }
    return b;
}

// Testa se é uma linha de código, caso seja, conta-a.
bool count_code(std::string l){
    bool a{false};
    if(l[0] != '/' && l[0] != '*'){
      a = true;
    }
    return a;
}
