#ifndef CLASSES_STRUCTS_H
#define CLASSES_STRUCTS_H

#define uint unsigned int

// == Enumerações

// Essa enumeração lista todas as linguagens suportadas.
enum lang_type_e : uint{
  C = 0,  //!< Linguagem C
  CPP,    //!< Linguagem C++
  H,      //!< Cabeçalho C/C++
  HPP,    //!< Cabeçalho C++
  UNDEF,  //!< Tipo indefinido.
};

// Essa enumeração lista todas as possibilidades de ordenação.
enum order_possibility : uint{
  f = 0, //!< Ordena por nome de arquivo.
  t,     //!< Ordena por tipo de arquivo.
  c,     //!< Ordena por linhas de comentarios.
  b,     //!< Ordena por linhas em branco.
  s,     //!< Ordena por numero de linhas de codigo.
  a,     //!< Ordena por numero total de linhas.
  undef, //!< Ordenação indefinida.
};

//== Declaração de Classe/Struct 

// Tipo para os contadores de linha.
using count_t = unsigned long;
// Armazena as informação do arquivo que estamos coletando.
class FileInfo {
 public:
  std::string filename;     //!< Nome do arquivo.
  lang_type_e type;         //!< Linguagem.
  count_t n_blank;          //!< # de linhas brancas.
  count_t n_comments;       //!< # de linhas de cometários.
  count_t n_loc;            //!< # de linhas de código.
  count_t n_lines;          //!< # de linhas.

 public:
  // Construtor.
  FileInfo(const std::string &fn = "",
           lang_type_e t = UNDEF,
           count_t nb = 0,
           count_t nc = 0,
           count_t nl = 0,
           count_t ni = 0)
      : filename{ fn }, type{ t }, n_blank{ nb }, n_comments{ nc }, n_loc{ nl }, n_lines{ ni } { /* empty */ }
};

// Opções de execução fornecidas via linha de comando. 
struct RunningOpt {
  std::string input_name = "";    //!< Isso deve ser o nome de um arquivo ou de um diretório.
  bool recursive = false;         //!< Indica se deve buscar com recurção em subdiretórios. 
  bool ascending = false;         //!< Indica se está em ordem ascendente.
  bool descending = false;        //!< Indica se está em ordem descendente.
  order_possibility orderOpt = undef;
};


#endif