#include <string.h>
#include <fstream>
#include <dirent.h>
#include "treatment_of_data.h"
#include "print.h"

#define PATH_MAX 4096

//== Funções auxiliares

// Retorna verdadeiro se o arquivo existe e pode ser aberto.
bool file_exists(const std::string &str) {
  std::ifstream fs{ str };
  return fs.is_open();
}

// Função para armazenar/tratar os argumentos de linha de comando.
RunningOpt cmd_arguments(int argc, char* argv[]){
  RunningOpt runOpt;
  if(argc == 1) usage();
  if(argc == 2){
    std::string args{argv[1]};
    //Não pode passar esses argumentos sem o nome de arquivo ou diretório.
    if(args == "-s" || args == "-S" || args == "-r"){
      std::cout << "Erro! Falta passar o arquivo ou diretório ou um campo de ordenação!\n";
      usage();
    }
    // Usuário está com dúvida de como usar o Sloc.
    else if(args == "-h" || args == "--help") usage();
    // Precisar ser um arquivo ou diretorio.
    else runOpt.input_name = args;

    return runOpt;
  }
  else{
    for(int i = 1; i < argc; i++){ 
      std::string args{argv[i]};
      std::string args2{"-s"};

      // Existe mais de dois argumetos, contando com nome do programa.
      if(args == "-"){
        std::cout << "ERROR!" << std::endl;
        usage();
        runOpt.input_name = "";
        return runOpt;
      }
      // Identificar se o proximo args é uma opção de ordenação.
      if(i+1 != argc && (args == "-S" || args == "-s")){
        // Definindo se a ordenação será crescente ou decrescente.
        runOpt.ascending = args == "-s" ? true : false;
        runOpt.descending = args == "-S" ? true : false;

        // Verificar se o próximo argumento é um campo de ordenação válido.
        std::string prox_args{argv[++i]};
        // Campos de ordenação válidos.
        std::string campos_ordenacao{"ftcbsa"};
        size_t result = campos_ordenacao.find(prox_args);

        // Testa se foi encontrado algum campo de ordenação válido.
        if (result != std::string::npos){ 
          // Testes para armazenar do campo de ordenação que foi encontrado.
          if(i+1 == argc && runOpt.input_name == ""){
            std::cout << "ERROR! Falta passar o arquivo ou diretório\n";
            usage();
            return runOpt;
          }
          else if(prox_args == "f") runOpt.orderOpt = f;
          else if(prox_args == "t") runOpt.orderOpt = t;
          else if(prox_args == "c") runOpt.orderOpt = c;
          else if(prox_args == "b") runOpt.orderOpt = b;
          else if(prox_args == "s") runOpt.orderOpt = s;
          else if(prox_args == "a") runOpt.orderOpt = a;
        } 
        // Não encontrou um campo de ordenação válido.
        else{
          std::cout << "ERROR! Falta passar o Campo de Ordenação!\n";
          // Mostra ajuda.
          usage();  
          runOpt.input_name = "";
          runOpt.orderOpt = undef;
          return runOpt;
        }
      }
      else if(i+1 == argc && (args == "-s" || args == "-S")){
        std::cout << "ERROR! Falta passar o Campo de Ordenação!\n";
        // Mostra ajuda.
        usage();
        runOpt.input_name = "";
        return runOpt;  
      }
      // Verificar se existe recursividade, próximo argumento deverás ser um diretório.
      else if(args == "-r"){
        if(i+1 == argc && runOpt.input_name == "") {
          std::cout << "ERROR! Falta passar o arquivo ou diretório\n";
          usage();
          return runOpt;
        }
        else{
          runOpt.recursive = true;
        }
      }
      // Única opção, se usuário não cometeu erro, vai ser um arquivo/diretório.
      else if(runOpt.input_name == ""){
        runOpt.input_name = args;
      }
    }
  }
  return runOpt;
}

// Função para buscar arquivos sem recursividade.
std::vector<std::string> search_file_directory(std::string diretorio){

  struct dirent *pDirent;
  DIR *pDir;
  
  std::vector<std::string> vector_arquivos;
  char entrada[PATH_MAX];

  // Garante que o diretório pode ser aberto.
  pDir = opendir(diretorio.c_str());
  if (pDir == NULL) {
    vector_arquivos.push_back(diretorio);
    return vector_arquivos;
    
  }
  else{
    // Processa cada entrada.
    while ((pDirent = readdir(pDir)) != NULL) {
      snprintf(entrada,sizeof(entrada),"%s/%s", diretorio.c_str(), pDirent->d_name);
      std::string arquivos{entrada};

      // Verifica se é um arquivo válido.
      if ((arquivos.rfind(".cpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".cpp")) == 4)
       || (arquivos.rfind(".c") != std::string::npos && (arquivos.length() - arquivos.rfind(".c")) == 2)
       || (arquivos.rfind(".hpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".hpp")) == 4)
       || (arquivos.rfind(".h") != std::string::npos && (arquivos.length() - arquivos.rfind(".h")) == 2)){
        vector_arquivos.push_back(arquivos);
      }
    }

    // Fecha diretório e sai.
    closedir(pDir);
  }
  return vector_arquivos;
}

std::vector<std::string> vector_arquivos;
// Função para pegar os arquivos/diretórios de forma recusiva da RunningOpt e armazenar em um vector.
std::vector<std::string> search_file_directory_recursively(std::string diretorio){
  char entrada[PATH_MAX];
  struct dirent* entry;

  // Verificando se é um diretório!
  DIR* dir = opendir(diretorio.c_str());

  // Testa se foi possível abrir o diretório.
  if (dir == NULL)
  {
    vector_arquivos.push_back(diretorio);
    return vector_arquivos;
  }
  else{

    while ((entry = readdir(dir)) != NULL)
    {
      // Retorna a string formatada com os nomes dos diretórios/arquivos encontrados.
      snprintf(entrada,sizeof(entrada),"%s/%s", diretorio.c_str(), entry->d_name); 

      // Caso haja um subdiretório o percorre também.
      if ((strcmp("build",entry->d_name) != 0) &&  (strcmp(".",entry->d_name) != 0) && (strcmp("..",entry->d_name) != 0)
        && (entry->d_type == DT_DIR))
      {
          search_file_directory_recursively(entrada);
      }
      else if (entry->d_type == DT_REG){ 
      std::string arquivos{entrada};
        // Procurando arquivos válidos.
        if ((arquivos.rfind(".cpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".cpp")) == 4)
        || (arquivos.rfind(".c") != std::string::npos && (arquivos.length() - arquivos.rfind(".c")) == 2)
        || (arquivos.rfind(".hpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".hpp")) == 4)
        || (arquivos.rfind(".h") != std::string::npos && (arquivos.length() - arquivos.rfind(".h")) == 2)){
          vector_arquivos.push_back(arquivos);
        }
      }
    }
    closedir(dir);
  }
  return vector_arquivos;
}

// Função para armazenar os arquivos válidos encontrados numa lista.
std::vector<std::string> list_of_src_files(RunningOpt runOpt){
  std::vector<std::string> vector_de_arquivos;
  if(runOpt.recursive){
    vector_de_arquivos = search_file_directory_recursively(runOpt.input_name);
    return vector_de_arquivos;
  }
  else{
    vector_de_arquivos = search_file_directory(runOpt.input_name);
    return vector_de_arquivos;
  }
}

// Funções de ordenação decrescente.
bool compare_filename_descending(const FileInfo & a, const FileInfo & b){
  return (a.filename > b.filename);
}
bool compare_type_descending(const FileInfo & a, const FileInfo & b){
  return (a.type > b.type);
}
bool compare_blank_descending(const FileInfo & a, const FileInfo & b){
  return (a.n_blank > b.n_blank);
}
bool compare_comment_descending (const FileInfo & a, const FileInfo & b){
  return (a.n_comments > b.n_comments);
}
bool compare_code_descending(const FileInfo & a, const FileInfo & b){
  return (a.n_loc > b.n_loc);
}
bool compare_total_descending(const FileInfo & a, const FileInfo & b){
  return (a.n_lines > b.n_lines);
}

// Funções para ordenação crescente.
bool compare_filename_ascending(const FileInfo & a, const FileInfo & b){
  return (a.filename < b.filename);
}
bool compare_type_ascending(const FileInfo & a, const FileInfo & b){
  return (a.type < b.type);
}
bool compare_blank_ascending(const FileInfo & a, const FileInfo & b){
  return (a.n_blank < b.n_blank);
}
bool compare_comment_ascending(const FileInfo & a, const FileInfo & b){
  return (a.n_comments < b.n_comments);
}
bool compare_code_ascending(const FileInfo & a, const FileInfo & b){
  return (a.n_loc < b.n_loc);
}
bool compare_total_ascending(const FileInfo & a, const FileInfo & b){
  return (a.n_lines < b.n_lines);
}

// Função para pegar o Tipo(cabeçalho ou código) do arquivo por extenso.
std::string type_extenso(size_t x){
  switch (x)
  {
  case 0:
    return "C";
    break;
  case 1:
    return "C++";
    break;
  case 2:
    return "H";
    break;
  case 3: 
    return "HPP";
    break;
  default:
    return "";
    break;
  }
}