# Introduction

<!-- TODO -->

Esse projeto é um contador de linhas de arquivo. Ele suporta arquivos das linguagens C e C++. Ao final da contagem uma tabela é impressa com os dados dos arquivos, esses dados são dividos nas colunas de "nome do arquivo", "linguagem", "número de linhas comentadas", "número de linhas em branco", "numero de linhas de código" e "total".  

# Author(s)

<!-- TODO -->

Indique o nome do(s) autor(es) e email(s).
Nome: Igor Rauan Soares Da Silva  Email: igorrauans18@gmail.com 
Nome: Pedro Henrique Sales dos Santos  Email: pedrosales2806@gmail.com 


# Problems found or limitations

<!-- TODO -->

Lista aqui os problemas encontrados (se houve algum) e as possíveis limitações do programa.

Caso o usuário indique incorretamente uma opção do tipo "-x" o programa apenas finaliza sem mensagem de erro.
A tabela não possuí a formatação adequada.

Imprime apenas uma das cinco mensagens de erro.

# Grading

<!-- TODO -->

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                                | Valor máximo | Valor esperado |
| --------------------------------------------------- | :----------: | :------------: |
| Read a single source file                           |      10      |       10       |
| Read a list of files from a directory               |      15      |       15       |
| Show help as requested `-h`                         |      4       |       4        |
| Treat error situations                              |      8       |       6        |
| Show correct number of blank lines                  |      5       |       5        |
| Show correct number of comment lines                |      10      |       9        |
| Show correct number of code lines                   |      10      |       10       |
| Show correct number of total lines                  |      5       |       5        |
| Show the type of source file                        |      5       |       5        |
| Show percentages as requested                       |      10      |       10       |
| Show consolidation at the bottom of a table         |      5       |       5        |
| Table is formatted as requested                     |      8       |       4        |
| The project contains at least one struct or classes |      5       |       5        |

# Compiling and Runnig

<!-- TODO -->

Indique aqui como compilar e executar seu programa, caso você não esteja usando `cmake`.

compilar cmake: mkdir build -- cd build -- cmake .. -- make (dentro da pasta src/)

compilar no manualmente no terminal: g++ -Wall -std=c++17 main.cpp conta_linha.cpp -o sloc (dentro da pasta src/)

executar: ./sloc <arquivo/diretorio> [opções de ordenação]
