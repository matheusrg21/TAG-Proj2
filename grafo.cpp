/*
    Universidade de Brasília
    Instituto de Ciências Exatas
    Departamento de Ciência da Computação¹

    Teoria e Aplicação de Grafos 2018/2

    Gabriel Nunes Rodrigues Fonseca - 16/0006597¹
    Matheus Rodrigues Guimarães     - 15/0141661¹
*/


#include <iostream>   // funcoes de entrada e saida de dados
#include <fstream>    // funcoes de manipulacao de arquivos
#include <string>     // funcoes de manipulacao de cadeia de caracteres
#include <vector>     // funcoes de vetor dinamico
#include <algorithm>  // funcoes de sort
#include <cstdlib>    // funcoes de conversao de tipos de arquivo

using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::string;
using namespace std;

typedef struct vertex{  // Estrutura de elo
  int number;           // Numero do vertice (Equivalente a matricula)
  string name;          // Nome do aluno
  int credits;          // creditos de cada materia
  int difficulty;       // ou weight: peso da materia
  int nofrequirements;  // numeros de requirements
  vector<string> requirements;     // Lista de arestas conectadas a ele
} vertex;

typedef struct graph{ // Estrutura de grafo
  vector<vertex> digraph; // Conjunto de elos (digrafo pq tem direcao)
} graph;

typedef struct caminho{
  vector<int> meuCaminho;
  int allPesos;
} caminho;

int loadfile (graph &g){

  unsigned int i;
  ifstream myfile("materias_cic.txt"); // nome do arquivo
  string line; // auxiliador para pegar linhas do arquivo
  string aux; // auxiliador para converter os valores das linhas do arquivo
  vertex aux_vertex;

  if(myfile.is_open()){ // Se abrir o arquivo, exercuta
    while(getline(myfile, line)){ //Enquanto for lido linha faz


      //------------------------------------NUMERO
      i = 0;
      while (line[i] != '|') { // Le valores do line ate o pipe
        aux.push_back(line[i]); // Adiciona no auxiliar para futura conversao
        i++;
      }

      aux_vertex.number = atoi(aux.c_str()); // Converte auxiliar para int
      aux.clear(); // Limpa vetor para reaproveitamento
      //------------------------------------NOME
      i += 1; // Pula o pipe
      while (line[i] != '|') { // Le valores do line ate o pipe
        aux.push_back(line[i]); // Adiciona no auxiliar para futura conversao
        i++;
      }

      aux_vertex.name = aux.c_str(); // Converte auxiliar para int
      aux.clear(); // Limpa vetor para reaproveitamento
      //------------------------------------CREDITOS
      i += 1; // Pula o pipe
      while (line[i] != '|') { // Le valores do line ate o pipe
        aux.push_back(line[i]); // Adiciona no auxiliar para futura conversao
        i++;
      }

      aux_vertex.credits = atoi(aux.c_str()); // Converte auxiliar para int
      aux.clear(); // Limpa vetor para reaproveitamento
      //------------------------------------PESO
      i += 1; // Pula o pipe
      while (line[i] != '|') { // Le valores do line ate o pipe
        aux.push_back(line[i]); // Adiciona no auxiliar para futura conversao
        i++;
      }
      aux_vertex.difficulty = atoi(aux.c_str()) * aux_vertex.credits; // Converte auxiliar para int
      aux.clear(); // Limpa vetor para reaproveitamento
      //------------------------------------PRE REQUISITOS
      i += 1; // Pula o pipe
	  aux_vertex.nofrequirements = 0;
      while(i < line.size()) { // Le ate o final da linha
        while (line[i] != '-' && line[i] != '\0') { // Le os numeros entre espacos
          aux.push_back(line[i]); // Adiciona ao vetor auxiliar
          i++;
        }

        aux_vertex.requirements.push_back(aux); // Joga valor convertido no vetor de arestas
        aux.clear(); // Limpa vetor para reutilizacao
        i++; // Verifica se era espaco ou pipe
        aux_vertex.nofrequirements++;
      }

      g.digraph.push_back(aux_vertex); // Adiciona o vertice auxiliar no vetor do grafo
      aux_vertex.requirements.clear(); // Limpa dados do vetor de arestas do vertice auxiliar
    }
    myfile.close(); // Fecha o arquivo apos finalizacao da leitura
  }

  else cout << "Unable to open file"; // RETURN -1;

  return 0;
}

int printg (int mode, graph g){

  unsigned int i, j;

  if (mode == 0) {
    cout << "\n\t\t ### Curso ### \n" << "\n\t\t Ciencia da Computacao \n";
    cout << "\n Universidade de Brasília" << endl; 
    cout << "\n Instituto de Ciências Exatas" << endl;
    cout << "\n Departamento de Ciência da Computação¹" << endl;
    cout << "\n Teoria e Aplicação de Grafos 2018/2" << endl;
    cout << "\n Gabriel Nunes Rodrigues Fonseca - 16/0006597¹" << endl;
    cout << "\n\tMatheus Rodrigues Guimarães     - 15/0141661¹" << endl;

  } else if (mode == 1){
    cout << "\n\t\t ### DAG Respectiva  ###\n";
    for (i = 0; i < g.digraph.size(); i++){ //Imprime uma caixa com os valores de cada vertice do grafo
      cout << "==================================================================================================================" << endl;
      cout << "\r| Numero: " << g.digraph[i].number << endl;
      cout << "\r| Creditos: " <<  g.digraph[i].credits << endl;
      cout << "\r| Dificuldade: " << g.digraph[i].difficulty << endl;
      cout << "\r| Name: " << g.digraph[i].name << endl;
      cout << "\r| Numero de pre requisitos: " << g.digraph[i].nofrequirements << endl;
      if(g.digraph[i].requirements.size() > 0){
        cout << "\r| Numero dos pre requirements: ";
      }
      for (j = 0; j < g.digraph[i].requirements.size(); j++){
        cout << "   " << g.digraph[i].requirements[j];
        if(j+1 < g.digraph[i].requirements.size())
        cout << "   E";
      }
      cout << endl;
      cout << "==================================================================================================================" << endl;

    }
  } else if (mode == 2){
    cout << "\n\t\t ### Ordenacao Topologica  ###\n Inicio";
    for (i = 0; i < g.digraph.size(); i++){ //Imprime uma caixa com os valores de cada vertice do grafo
       cout << " -> " << g.digraph[i].name;
    }
    cout << " -> Fim \n\n\n";
  } else {
    // CAMINHO CRITICO AQUI
    cout << "\n\t\t ### Caminho Critico ###\n Inicio";
    for (i = 0; i < g.digraph.size(); i++){ //Imprime uma caixa com os valores de cada vertice do grafo
       cout << " -> " << g.digraph[i].number;
    }
    cout << " -> Fim \n\n\n";
  }
  return 0;
}

void csort(graph &g){

  int i, j, graph_size = g.digraph.size();
  int bigger;
  vertex aux;

  for(i = 0;i < graph_size; i++){ //Procura sempre o maior e joga no inicio
    bigger = i;
    for (j = i; j < graph_size;j++){
      if (g.digraph[j].requirements.size() < g.digraph[bigger].requirements.size()) bigger = j;
    }
    aux = g.digraph[i];
    g.digraph[i] = g.digraph[bigger];
    g.digraph[bigger] = aux;
  }
}

graph kahn(graph g){

    graph copy_g = g, aux_g, out_g;
    int size_g;
    unsigned i, j, k, visited = 0;

    for (i = 0; i < copy_g.digraph.size(); i++){
        if (copy_g.digraph[i].nofrequirements == 0){
            aux_g.digraph.push_back(copy_g.digraph[i]);
            copy_g.digraph.erase(copy_g.digraph.begin()+i);
            i--;
        }
    }
    size_g = aux_g.digraph.size();
    while (size_g != 0){
        out_g.digraph.push_back(aux_g.digraph[0]);
        visited++;
        for(j = 0; j < copy_g.digraph.size(); j++){
            for (k = 0; k < copy_g.digraph[j].requirements.size(); k++){
                if (aux_g.digraph[0].name == copy_g.digraph[j].requirements[k]){
                    copy_g.digraph[j].requirements.erase(copy_g.digraph[j].requirements.begin()+k);
                    copy_g.digraph[j].nofrequirements--;
                    if (copy_g.digraph[j].nofrequirements == 0) {
                      aux_g.digraph.push_back(copy_g.digraph[j]);
                      size_g++;
                    }
                }
            }
        }
        aux_g.digraph.erase(aux_g.digraph.begin());
        size_g--;

    }

    if (visited != g.digraph.size()) cout << "\n Error!";
    printg(2, out_g);

    return aux_g;

}

void caminhoCritico(graph g){
  unsigned int v;
  unsigned int w = 0;
  caminho cam;
  caminho maior;
  maior.allPesos = -1;
  graph toReturn = g;
  cam.allPesos = 0;
  vertex aux;
  graph print;
  
  int maiorD = 0;
  for(v = 0; v < toReturn.digraph.size(); v++){
    for(w = 0; w < toReturn.digraph[v].nofrequirements; w++){
      maiorD += toReturn.digraph[v].difficulty;
      
      cam.allPesos += toReturn.digraph[v].difficulty;

      if(cam.allPesos > maior.allPesos){
        aux = toReturn.digraph[v];
        print.digraph.push_back(aux);
        maior = cam;
      }
    }
  }
  
}


int main () {

  graph g;


  loadfile(g);
  csort(g);
  printg(0, g);
  printg(1, g);
  kahn(g);


  return 0;
}
