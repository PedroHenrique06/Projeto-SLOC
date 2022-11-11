#ifndef CONTA_LINHA_HPP
#define CONTA_LINHA_HPP

#include <iostream>

bool Linha_branca(std::string l);

bool Multi_linhas_comentario(std::string l, bool* f, unsigned long* comment);

bool Linha_comentario(std::string l);

bool Linha_codigo(std::string l);

#endif