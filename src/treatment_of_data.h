#ifndef TREAT_DATA_H
#define TREAT_DATA_H

#include <vector>
#include "classes_structs.h"

RunningOpt cmd_arguments(int, char* argv[]);
bool file_exists(const std::string&);
std::vector<std::string> search_file_directory(std::string);
std::vector<std::string> search_file_directory_recursively(std::string);
std::vector<std::string> list_of_src_files(RunningOpt);
bool compare_filename_descending(const FileInfo&, const FileInfo&);
bool compare_type_descending(const FileInfo&, const FileInfo&);
bool compare_blank_descending(const FileInfo&, const FileInfo&);
bool compare_comment_descending(const FileInfo&, const FileInfo&);
bool compare_code_descending(const FileInfo&, const FileInfo&);
bool compare_total_descending(const FileInfo&, const FileInfo&);
bool compare_filename_ascending(const FileInfo&, const FileInfo&);
bool compare_type_ascending(const FileInfo&, const FileInfo&);
bool compare_blank_ascending(const FileInfo&, const FileInfo&);
bool compare_comment_ascending(const FileInfo&, const FileInfo&);
bool compare_code_ascending(const FileInfo&, const FileInfo&);
bool compare_total_ascending(const FileInfo&, const FileInfo&);
std::string type_extenso(size_t);

#endif