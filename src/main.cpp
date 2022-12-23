/*!
 * @file main.cpp
 * @description
 * This program implements a single line of code count for C/C++ programs.
 * @author	Add your name here
 * @date	September, 9th 2022.
 * @remark On 2022-09-9 changed ...
 */

#include <sys/types.h>
#include <iostream>
#include "treatment_of_data.h"
#include "counting.h"
#include "classes_structs.h"
#include "print.h"


int main(int argc, char* argv[]){
  RunningOpt runop = cmd_arguments(argc, argv);
  auto src_files = list_of_src_files(runop);
  auto database = count_lines(src_files);

  if(!database.empty()){
    show_info(database, runop);
  }

  return EXIT_SUCCESS;
}