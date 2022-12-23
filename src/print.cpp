#include <iostream>
#include <algorithm>
#include "print.h"
#include "treatment_of_data.h"
#include "counting.h"


// Função para mostrar informações de como usar o Sloc.
void usage(){
  std::cout << "\tWelcome to sloc, version 1.0, (c) 2022 Selan, DIMAp, UFRN.\n"
                << "\tUsage: sloc [options] file/directory\n"
                << "\tOptions:\n"
                << "\t-h/--help       Display this information.\n"
                << "\t-r              Look for files recursively in the directory provided.\n"
                << "\t-s f/t/c/b/s/a  Sort table in ascending order by (f)ilename, (t) filetype,\n"
                << "\t\t\t(c)omments, (b)lank lines, (s)loc, or (a)ll.\n"
                << "\t\t\tDefault is to show files in ordem of appearance.\n"
                << "\t-S f/t/c/b/s/a  Same as above, but in descending order.\n";
}

// Imprime a tabela
void show_info(std::vector<FileInfo> database, RunningOpt runOpt){
  if(runOpt.descending){
    switch (runOpt.orderOpt){
    // Decrescente por nome do arquivo
    case f: 
      std::sort(std::begin(database), std::end(database), compare_filename_descending);
      break;
    //Decrescente por tipo do arquivo.
    case t: 
      std::sort(std::begin(database), std::end(database), compare_type_descending);
      break;
    // Decrescente por linha comentários.
    case c: 
      std::sort(std::begin(database), std::end(database), compare_comment_descending);
      break;
    // Decrescente por linha brancas.
    case b: 
      std::sort(std::begin(database), std::end(database), compare_blank_descending);
      break;
    // Decrescente por linha código.
    case s: 
      std::sort(std::begin(database), std::end(database), compare_code_descending);
      break;
    // Decrescente por linha brancas.
    case a: 
      std::sort(std::begin(database), std::end(database), compare_total_descending);
      break;
    default:
      break;
    }
  }
  else if(runOpt.ascending){
    switch (runOpt.orderOpt){
    // Crescente por nome do arquivo.
    case f: 
      std::sort(std::begin(database), std::end(database), compare_filename_ascending);
      break;
    // Crescente por tipo do arquivo.
    case t: 
      std::sort(std::begin(database), std::end(database), compare_type_ascending);
      break;
    // Crescente por linha comentarios.
    case c: 
      std::sort(std::begin(database), std::end(database), compare_code_ascending);
      break;
    // Crescente por linha brancas.
    case b: 
      std::sort(std::begin(database), std::end(database), compare_blank_ascending);
      break;
    // Crescente por linha código
    case s: 
      std::sort(std::begin(database), std::end(database), compare_code_ascending);
      break;
    // Crescente por linha totais
    case a: 
      std::sort(std::begin(database), std::end(database), compare_total_ascending);
      break;
    default:
      break;
    }
  }
  
  std::string num_of_spaces =  spaces();
  
  if(database.size() != 0) std::cout << "Total de Aquivos: " << database.size() << "\n";
    int branco{0}, comentario{0}, codigo{0}, total{0};
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------\n"
              << "Filename" << num_of_spaces << "     "
              <<  "Type        " 
              << "Comment           "     
              << "Blank           "
              << "Code           "
              << "All\n"
              << "--------------------------------------------------------------------------------------------------------------------------------\n";
    
    for(auto file : database){
      float p1, p2, p3;

      p1 = (file.n_loc*100)/file.n_lines;
      p2 = (file.n_comments*100)/file.n_lines;
      p3 = (file.n_blank*100)/file.n_lines;

      branco += file.n_blank;
      comentario += file.n_comments;
      codigo += file.n_loc;
      total += file.n_lines;

      std::cout.precision(1);
      std::cout<< file.filename << "           " 
                << type_extenso(file.type) << "           "
                << file.n_comments <<  "(" << std::fixed << p2 << "%)" << "           "
                << file.n_blank << "(" << std::fixed << p3 << "%)" << "           " 
                << file.n_loc << "(" << std::fixed << p1 << "%)" << "           " 
                << file.n_lines
                << "\n--------------------------------------------------------------------------------------------------------------------------------\n";
  }
  std::cout << "Sum\t\t\t\t\t\t"
              << codigo << "\t\t\t"
              << comentario << "\t\t"
              << branco << "\t\t\t"
              << total << "\n";
}
