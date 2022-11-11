/*!
 * @file main.cpp
 * @description
 * This program implements a single line of code count for C/C++ programs.
 * @author	Add your name here
 * @date	September, 9th 2022.
 * @remark On 2022-09-9 changed ...
 */
 
#include <ctype.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <vector>
#include <string.h>
using std::map;
#include <string>
using std::string;
#include "conta_linha.hpp"
#define PATH_MAX 4096

//== Enumerations

/// This enumeration lists all the supported languages.
enum lang_type_e {
  C = 0,  //!< C language
  CPP,    //!< C++ language
  H,      //!< C/C++ header
  HPP,    //!< C++ header
  UNDEF,  //!< Undefined type.
};

/// This enumeration list all the possibility ordernation
enum order_possibility {
  f = 0, //!< ordena por nome de arquivo.
  t,     //!< ordena por tipo de arquivo.
  c,     //!< ordena por linhas de comentarios.
  b,     //!< ordena por linhas em branco.
  s,     //!< ordena por numero de linhas de codigo.
  a,     //!< ordena por numero total de linhas.
  undef, //!< ordenação indefinida.
};

//== Class/Struct declaration

/// Integer type for counting lines.
using count_t = unsigned long;
/// Stores the file information we are collecting.
class FileInfo {
 public:
  string filename;     //!< the filename.
  lang_type_e type;    //!< the language type.
  count_t n_blank;     //!< # of blank lines in the file.
  count_t n_comments;  //!< # of comment lines.
  count_t n_loc;       //!< # lines of code.
  count_t n_lines;     //!< # of lines.

 public:
  /// Ctro.
  FileInfo(const string &fn = "",
           lang_type_e t = UNDEF,
           count_t nb = 0,
           count_t nc = 0,
           count_t nl = 0,
           count_t ni = 0)
      : filename{ fn }, type{ t }, n_blank{ nb }, n_comments{ nc }, n_loc{ nl }, n_lines{ ni } {
    /* empty*/
  }
};

/// The running options provided via CLI.
struct RunningOpt {
  string input_name = "";  //!< This might be a filename or a directory.
  bool recursive = false; //!< This indicates whether to look for recursion in subdirectories.
  bool ascending = false; //!< This indicates if it is ascending order.
  bool descending = false; //!< This indicates if it is descending order.
  order_possibility orderOpt = undef;
};

//== Aux functions

/// Returns true if file exists and can be opened.
bool file_exists(const string &str) {
  std::ifstream fs{ str };
  return fs.is_open();
}

// funcao para mostrar informacoes de como usar sloc!
void print_help(){
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

// função para armazenar/tratar os argumentos de linha de comando!
RunningOpt parse_cmd_line(int argc, char *argv[]){
  RunningOpt runOpt;
  if(argc == 1) print_help();
  if(argc == 2){
    string args{argv[1]};
    if(args == "-s" || args == "-S" || args == "-r"){ //Não pode passar esses argumentos sem o nome de arquivo ou directorio
      std::cout << "Erro! Falta passar o arquivo ou diretório ou um campo de ordenação!\n";
      print_help();
    }
    else if(args == "-h" || args == "--help") print_help(); // usuário está com dúvida de como usar o sloc!
    else runOpt.input_name = args; //precisar ser um arquivo ou diretorio;

    return runOpt;
  }
  else
  {
    for(int i = 1; i < argc; i++){ 
      std::string args{argv[i]};
      std::string args2{"-s"};

      //exite mais de dois argumeto, contando com nome do programa!
      if(args == "-"){
        std::cout << "ERROR!\n";
        print_help();
        runOpt.input_name = "";
        return runOpt;
      }
      if(i+1 != argc && (args == "-S" || args == "-s")) //identificar se o proximo args é uma opcao de ordenação.
      {
        runOpt.ascending = args == "-s" ? true : false; // definindo se a ordenação será crescente !
        runOpt.descending = args == "-S" ? true : false; // definindo se a ordenação será decrescente!

        //vereificar se o proximo argumento é um campo de ordenação válido?
        std::string prox_args{argv[++i]};
        std::string campos_ordenacao{"ftcbsa"}; //campos de ordenação validos!
        size_t result = campos_ordenacao.find(prox_args); 

        if (result != std::string::npos){ // foi encontrado algum campo de ordenação valido? 
          // testes para armazenar o campo de ordenação que foi encontrado!
          if(i+1 == argc && runOpt.input_name == ""){
            std::cout << "ERROR! Falta passar o arquivo ou diretório\n";
            print_help();
            return runOpt;
          }
          else if(prox_args == "f") runOpt.orderOpt = f;
          else if(prox_args == "t") runOpt.orderOpt = t;
          else if(prox_args == "c") runOpt.orderOpt = c;
          else if(prox_args == "b") runOpt.orderOpt = b;
          else if(prox_args == "s") runOpt.orderOpt = s;
          else if(prox_args == "a") runOpt.orderOpt = a;
        } 
        else //não encontrou um campo de ordenação valido!
        {
          std::cout << "ERROR! Falta passar o Campo de Ordenação!\n";
          // deve ser mostrado a saida de --help aqui!
          print_help();  
          runOpt.input_name = "";
          runOpt.orderOpt = undef;
          return runOpt;
        }
      }
      else if(i+1 == argc && (args == "-s" || args == "-S")){
        std::cout << "ERROR! Falta passar o Campo de Ordenação!\n";
        // deve ser mostrado a saida de --help aqui!
        print_help();
        runOpt.input_name = "";
        return runOpt;  
      }
      else if(args == "-r") // verificar se exite recursividade, proximo argumento provavelmente um diretorio!
        if(i+1 == argc && runOpt.input_name == "") {
          std::cout << "ERROR! Falta passar o arquivo ou diretório\n";
          print_help();
          return runOpt;
        }
        else runOpt.recursive = true;
      else if(runOpt.input_name == ""){ // unica opcao, se usuario não cometeu erro, vai ser um arquivo/diretorio!
        runOpt.input_name = args;
      }
    }
  }
  return runOpt;
}

//função para buscar arquivos sem recursividade!!
std::vector<std::string> busca_dir_arquivo(std::string diretorio)
{

  struct dirent *pDirent;
  DIR *pDir;
  
  std::vector<std::string> vector_arquivos;
  char entrada[PATH_MAX];

  // Ensure we can open directory.
  pDir = opendir(diretorio.c_str());
  if (pDir == NULL) {
    vector_arquivos.push_back(diretorio);
    return vector_arquivos;
    
  }
  else{
    // Process each entry.
    while ((pDirent = readdir(pDir)) != NULL) {
      snprintf(entrada,sizeof(entrada),"%s/%s", diretorio.c_str(), pDirent->d_name);
      std::string arquivos{entrada};

      if ((arquivos.rfind(".cpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".cpp")) == 4)
       || (arquivos.rfind(".c") != std::string::npos && (arquivos.length() - arquivos.rfind(".c")) == 2)
       || (arquivos.rfind(".hpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".hpp")) == 4)
       || (arquivos.rfind(".h") != std::string::npos && (arquivos.length() - arquivos.rfind(".h")) == 2)){ //verefica se é um arquivo valido!
        vector_arquivos.push_back(arquivos);
      }
    }

    // Close directory and exit.
    closedir(pDir);
  }
  return vector_arquivos;

}



//função para pegar os arquivos/diretorios de forma recusiva da RunningOpt e armazenar num vector!
std::vector<std::string> vector_arquivos; 
std::vector<std::string> busca_dir_arquivo_recursiva(std::string diretorio)
{
  char entrada[PATH_MAX];
  struct dirent* entry;

  //verificando se é um diretorio!
  DIR* dir = opendir(diretorio.c_str());

  
  if (dir == NULL) //foi possivel abrir o diretorio??
  {
    vector_arquivos.push_back(diretorio);
    return vector_arquivos;
  }
  else{

    while ((entry = readdir(dir)) != NULL)
    {
      //retorna a string formatada com os nomes dos diretorios ou arquivos encontrados! ex: diretorio/subdiretorio/arquivo
      snprintf(entrada,sizeof(entrada),"%s/%s", diretorio.c_str(), entry->d_name); 

      //caso encontrado outro diretorio dentro do anterior, é realizado uma chamada recursiva para pecorrer os subdiretorios e encontrar os arquivos!
      if ((strcmp("build",entry->d_name) != 0) &&  (strcmp(".",entry->d_name) != 0) && (strcmp("..",entry->d_name) != 0)
        && (entry->d_type == DT_DIR))//existe um subdiretorio no diretorio atual??
      {
          //realiza chamada recursiva !
          busca_dir_arquivo_recursiva(entrada);
      }
      else if (entry->d_type == DT_REG){ //encontrou um arquivo??
      std::string arquivos{entrada};
      //procurando arquivos validos!!
      if ((arquivos.rfind(".cpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".cpp")) == 4)
       || (arquivos.rfind(".c") != std::string::npos && (arquivos.length() - arquivos.rfind(".c")) == 2)
       || (arquivos.rfind(".hpp") != std::string::npos && (arquivos.length() - arquivos.rfind(".hpp")) == 4)
       || (arquivos.rfind(".h") != std::string::npos && (arquivos.length() - arquivos.rfind(".h")) == 2)){ //verefica se é um arquivo valido!
        vector_arquivos.push_back(arquivos);
      }

      }
    }
    closedir(dir);
  }
  return vector_arquivos;
}

//Essa função conta as linhas (em branco, de código, de comentario) e retorna um FileInfo com essas informações
FileInfo contagem(string nome_arquivo){
    std::fstream arquivo;
    string linha;
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
      //percore o arquivo linha a linha
      while(getline(arquivo, linha)){
        //tira todos os espaços em branco da string
        linha.erase(std::remove_if(linha.begin(), linha.end(), ::isspace), linha.end());
          //conta linhas brancas
          if(Linha_branca(linha) && flag == false){
              file.n_blank++;
          }
          else{
            //conta comentarios com "/*"
            if(Multi_linhas_comentario(linha, &flag, &file.n_comments)){
              file.n_comments++; 
            }
            //conta comentarios com "//"
            if(Linha_comentario(linha) && flag == false){
              file.n_comments++;
            }
          //conta linhas de código
            if(Linha_codigo(linha) && flag == false){
              file.n_loc++;
            }
          } 
        //conta as linhas totais do arquivo
        file.n_lines++;
      }
    }
    else{
      std::cout << "Erro, não foi possível abrir o arquivo" << nome_arquivo << std::endl;
      return (FileInfo)NULL;
    }

  arquivo.close();
  
  return file;
}

//função para armazenar os arquivos validos encontrados numa lista;
std::vector<std::string> list_of_src_files(RunningOpt runOpt){
  std::vector<std::string> vector_de_arquivos;
  if(runOpt.recursive){
    vector_de_arquivos = busca_dir_arquivo_recursiva(runOpt.input_name);
    return vector_de_arquivos;
  }
  else{
    vector_de_arquivos = busca_dir_arquivo(runOpt.input_name);
    return vector_de_arquivos;
  }
}

//função para realizar contagem e criar o banco de dados!
std::vector<FileInfo> count_lines(std::vector<std::string> src_files){
  std::vector<FileInfo> db;
  FileInfo file;
  std::vector<std::string>::iterator iter;
  for(iter = src_files.begin(); iter != src_files.end(); iter++){
    //Testa se o arquivo existe e então chama a função de contagem
      if (file_exists(*iter)){
        db.push_back(contagem(*iter));
      }
  }
  return db;
}

//funções de ordenação decrescente!
bool compare_filiname(const FileInfo & a, const FileInfo & b){
  return (a.filename > b.filename);
}
bool compare_type(const FileInfo & a, const FileInfo & b){
  return (a.type > b.type);
}
bool compare_l_bracas(const FileInfo & a, const FileInfo & b){
  return (a.n_blank > b.n_blank);
}
bool compare_l_comentarios(const FileInfo & a, const FileInfo & b){
  return (a.n_comments > b.n_comments);
}
bool compare_l_sloc(const FileInfo & a, const FileInfo & b){
  return (a.n_loc > b.n_loc);
}
bool compare_l_total(const FileInfo & a, const FileInfo & b){
  return (a.n_lines > b.n_lines);
}

//funções para ordenação crescente!
bool compare_filiname_crescente(const FileInfo & a, const FileInfo & b){
  return (a.filename < b.filename);
}
bool compare_type_crescente(const FileInfo & a, const FileInfo & b){
  return (a.type < b.type);
}
bool compare_l_bracas_crescente(const FileInfo & a, const FileInfo & b){
  return (a.n_blank < b.n_blank);
}
bool compare_l_comentarios_crescente(const FileInfo & a, const FileInfo & b){
  return (a.n_comments < b.n_comments);
}
bool compare_l_sloc_crescente(const FileInfo & a, const FileInfo & b){
  return (a.n_loc < b.n_loc);
}
bool compare_l_total_crescente(const FileInfo & a, const FileInfo & b){
  return (a.n_lines < b.n_lines);
}

//função para pegar o Type do arquivo por extenso!
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

//Imprime a tabela
void show_info(std::vector<FileInfo> database, RunningOpt runOpt){
  if(runOpt.descending){
    switch (runOpt.orderOpt)
    {
    case f: //decrescente por nome do arquivo
      std::sort(std::begin(database), std::end(database), compare_filiname);
      break;
    case t: //decrescente por tipo do arquivo
      std::sort(std::begin(database), std::end(database), compare_type);
      break;
    case c: //decrescente por linha comentarios
      std::sort(std::begin(database), std::end(database), compare_l_comentarios);
      break;
    case b: //decrescente por linha brancas
      std::sort(std::begin(database), std::end(database), compare_l_bracas);
      break;
    case s: //decrescente por linha codigo
      std::sort(std::begin(database), std::end(database), compare_l_sloc);
      break;
    case a: //decrescente por linha brancas
      std::sort(std::begin(database), std::end(database), compare_l_total);
      break;
    default:
      break;
    }
  }else if(runOpt.ascending){
    switch (runOpt.orderOpt)
    {
    case f: //crescente por nome do arquivo
      std::sort(std::begin(database), std::end(database), compare_filiname_crescente);
      break;
    case t: //crescente por tipo do arquivo
      std::sort(std::begin(database), std::end(database), compare_type_crescente);
      break;
    case c: //crescente por linha comentarios
      std::sort(std::begin(database), std::end(database), compare_l_comentarios_crescente);
      break;
    case b: //crescente por linha brancas
      std::sort(std::begin(database), std::end(database), compare_l_bracas_crescente);
      break;
    case s: //crescente por linha codigo
      std::sort(std::begin(database), std::end(database), compare_l_sloc_crescente);
      break;
    case a: //crescente por linha brancas
      std::sort(std::begin(database), std::end(database), compare_l_total_crescente);
      break;
    default:
      break;
    }
  }

  if(database.size() != 0) std::cout << "Total de Aquivos: " << database.size() << "\n";
    int branco{0}, comentario{0}, codigo{0}, total{0};
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------\n"
    << "Filename\t\t Type\t\t Comment\t\t Blank\t\t Code\t\t All\n";
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
    std::cout<< file.filename << "\t\t"
              << type_extenso(file.type) << "\t\t"
              << file.n_comments <<  " (" << std::fixed << p2 << "%)" << "\t\t"
              << file.n_blank << " (" << std::fixed << p3 << "%)" << "\t" 
              << file.n_loc << " (" << std::fixed << p1 << "%)" << "\t" 
              << file.n_lines
              << "\n--------------------------------------------------------------------------------------------------------------------------------\n";
  }
  std::cout << "Sum\t\t\t\t\t\t"
              << comentario << "\t\t\t"
              << branco  << "\t\t"
              << codigo << "\t\t\t"
              << total << "\n";
}

int main(int argc, char *argv[]){
  RunningOpt runop = parse_cmd_line(argc, argv);
  auto src_files = list_of_src_files(runop);
  auto database = count_lines(src_files);
  if(!database.empty()){
    show_info(database, runop);
  }
  return EXIT_SUCCESS;
}