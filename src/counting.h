#ifndef COUNTING_H
#define COUNTING_H

#include <vector>
#include "classes_structs.h"

std::vector<FileInfo> count_lines(std::vector<std::string>);
FileInfo counting(std::string);
bool count_blank(std::string);
bool count_comment(std::string);
bool count_comment(std::string, bool*, unsigned long*);
bool count_code(std::string);
std::string spaces();


#endif