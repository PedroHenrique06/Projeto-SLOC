#include <iostream>
#include "conta_linha.hpp"

using std::string;

//Testa se a string está vazia e retorna "true" ou "false".
bool Linha_branca(string l){
  return l.empty();
}

//Testa se existe um comentário do tipo "/*" e então cria uma flag que tem valor "true" até encontrar "*/"  
bool Multi_linhas_comentario(std::string l, bool* f, unsigned long* comment){
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


//Testa se existe um "//" na linha, caso exista, conta comentário.
bool Linha_comentario(string l){
    bool b{false};
    for(unsigned int i=0; i<l.size(); i++){
        if(l[i] == '/' && l[i+1] == '/'){
            b = true;
        }
  }
    return b;
}

//Testa se a linha começa com "/", caso contrário, conta como código.
bool Linha_codigo(string l){
    bool a{false};
    if(l[0] != '/' && l[0] != '*'){
      a = true;
    }
    return a;
}