
#ifndef WATERMANAGEMENTDA_MANAGEMENTSYSTEM_H
#define WATERMANAGEMENTDA_MANAGEMENTSYSTEM_H

#include <string>
#include <algorithm>
#include "data_structures/Graph.h"

/** Classe que define um objeto do tipo ManagementSystem.
 *  Um objeto desta classe tem 6 membros privados.
 *  Um grafo do tipo string que representa a rede de água.
 *  4 vetores de Vertex e 1 de Edges.
 *  3 dos vetores de Vertex armazenam as cidades, reservatórios e estações.
 *  1 vetor de Vertex armazena os vértices ignorados.
 *  O vetor de Edges armazena os edges (pipes) ignorados.
 */
class ManagementSystem {
private:
    Graph<string> network; ///<Grafo que representa a rede de água.
    vector<Vertex<string>*> cities; ///<Vetor de cidades.
    vector<Vertex<string>*> reservoirs; ///<Vetor de reservatórios.
    vector<Vertex<string>*> stations; ///<Vetor de estações.
    vector<Vertex<string>*> ignoreVertex; ///<Vetor com os vértices ignorados.
    vector<Edge<string>*> ignoreEdge; ///<Vetor com os edges ignorados,
public:
    /** Função que retorna o vetor ignoreEdge.
     * @return vetor ignoreEdge.
    */
    const vector<Edge<string> *> &getIgnoreEdge() const;
    /** Funçao que permite alterar o vetor ignoreEdge.
     * @param ignoreEdge novo vetor ignoreEdge.
    */
    void setIgnoreEdge(const vector<Edge<string> *> &ignoreEdge);

public:
    /** Função que retorna o vetor ignoreVertex.
     * @return vetor ignoreVertex.
    */
    vector<Vertex<string> *> getIgnoreVertex() const;
    /** Funçao que permite alterar o vetor ignoreVertex.
     * @param ignoreVertex novo vetor ignoreVertex.
    */
    void setIgnoreVertex(const vector<Vertex<string> *> &ignoreVertex);
    /** Função que adiciona um vértice ao vetor ignoreVertex e coloca o seu atributo ignore a true.
     * @param vertex vértice a adicionar.
    */
    void addIgnoreVertex(Vertex<string> * vertex);
    /** Função que remove um vértice do vetor ignoreVertex e coloca o seu atributo ignore a false.
     * @param vertex vértice a ser removido.
    */
    void removeIgnoreVertex(Vertex<string> * vertex);


    /** Função que lê o ficheiro com as informações das cidades e adiciona à network.
     * @param filename nome do ficheiro.
    */
    void readCities(string filename);
    /** Função que lê o ficheiro com as informações dos pipes e adiciona à network.
     * @param filename nome do ficheiro.
    */
    void readPipes(string filename);
    /** Função que lê o ficheiro com as informações dos reservatórios e adiciona à network.
     * @param filename nome do ficheiro.
    */
    void readReservoirs(string filename);
    /** Função que lê o ficheiro com as informações das estações e adiciona à network.
     * @param filename nome do ficheiro.
    */
    void readStations(string filename);
    /** Função que corre o algoritmo edmondsKarp na network.
     * Complexidade temoporal: O(V E²).
     * @param srcCode código do vértice de origem.
     * @param destCode código do vértice de destino.
    */
    void edmondsKarp(string srcCode, string destCode);
    /** Função que corre o algoritmo edmondsKarp num grafo fornecido.
     * Complexidade temoporal: O(V E²).
     * @param network grafo em que vai ser utilizado o algoritmo.
     * @param srcCode código do vértice de origem.
     * @param destCode código do vértice de destino.
    */
    void edmondsKarp(Graph<string>& network, string srcCode, string destCode);
    /** Função que procura um augmenting path em network usando BFS.
     * Complexidade temporal: O(V + E).
     * @param src vértice de origem.
     * @param dest vértice de destino.
    */
    bool findAugmentingPath(Vertex<string>* src , Vertex<string>* dest );
    /** Função que procura um augmenting path num grafo fornecido usando BFS.
     * Complexidade temporal: O(V + E).
     * @param network grafo onde vai ser procurado o augmenting path.
     * @param src vértice de origem.
     * @param dest vértice de destino.
     * @return true se encontar um caminho ou falso se não encontar.
    */
    bool findAugmentingPath(Graph<string>& network, Vertex<string>* src , Vertex<string>* dest );
    /** Função que procura a capacidade residual mínima ao longo do augmenting path.
     * Complexidade temporal: O(V).
     * @param src vértice de origem.
     * @param dest vértice de destino.
     * @return capacidade residual mínima.
    */
    double minResidualPath(Vertex<string>* src , Vertex<string>* dest );
    /** Funçao que retorna o grafo network.
     * @return grafo network.
    */
    const Graph<string> &getNetwork() const;
    /** Funçao que permite alterar o grafo network.
     * @param network novo grafo network.
    */
    void setNetwork(const Graph<string> &network);
    /** Função que retorna o vetor cities.
     * @return vetor cities.
    */
    const vector<Vertex<string> *> &getCities() const;
    /** Funçao que permite alterar o vetor cities.
     * @param cities novo vetor cities.
    */
    void setCities(const vector<Vertex<string> *> &cities);
    /** Função que retorna o vetor reservoirs.
     * @return vetor reservoirs.
    */
    const vector<Vertex<string> *> &getReservoirs() const;
    /** Funçao que permite alterar o vetor ignoreVertex.
     * @param reservoirs novo vetor ignoreVertex.
    */
    void setReservoirs(const vector<Vertex<string> *> &reservoirs);
    /** Função que retorna o vetor stations.
     * @return vetor stations.
    */
    const vector<Vertex<string> *> &getStations() const;
    /** Funçao que permite alterar o vetor stations.
     * @param stations novo vetor stations.
    */
    void setStations(const vector<Vertex<string> *> &stations);
    /** Função que cria um vértice conectado a todos os resevatórios.
     * Cada edge tem como origem o novo vértice e como destino um reservatório.
     * Cada edge conectado a um reservatório tem como capacidade o maxDelivery do reservatório.
     * Complexidade temporal: O(V).
    */
    void createSuperSource();
    /** Função que cria um vértice conectado a todas as cidades.
     * Cada edge tem como origem uma cidade e como destino o novo vértice.
     * Cada edge que sai de uma cidade tem como capacidade o demand da cidade.
     * Complexidade temporal: O(V).
    */
    void createSuperSink();
    /** Função que cria um vértice conectado a todos os resevatórios de um grafo fornecido.
     * Cada edge tem como origem o novo vértice e como destino um reservatório.
     * Cada edge conectado a um reservatório tem como capacidade o maxDelivery do reservatório.
     * Complexidade temporal: O(V).
     * @param network grafo ao qual vai ser adicionado o vértice.
    */
    void createSuperSource(Graph<string>& network);
    /** Função que cria um vértice conectado a todas as cidades de um grafo fornecido.
     * Cada edge tem como origem uma cidade e como destino o novo vértice.
     * Cada edge que sai de uma cidade tem como capacidade o demand da cidade.
     * Complexidade temporal: O(V).
     * @param network grafo ao qual vai ser adicionado o vértice.
    */
    void createSuperSink(Graph<string>& network);
    /** Função que imprime que cidades são afetadas se for removido um reservatório e a quantidade de água que chega com e sem o reservatório.
     * Esta função analisa que cidades e reservatórios são afetados pela remoção do reservatório e corre o algoritmo edmondsKarp apenas por essas cidades e reservatórios.
     * Complexidade temporal: O(V E²)
     * @param code código do reservatório para ser removido.
    */
    void removeReservoir(string code);
    void resetIgnore();
    void setIgnore(vector<Vertex<string>*> vertexes);
    /** Função que imprime todos os reservatórios, estações e pipes ignorados.
     * Complexidade temporal: O(V + E).
    */
    void printFilter();
    /** Função que limpa os vetores ignoreVertex e ignoreEdge e coloca o atributo ignore desses vértices e edges a false.
     * Complexidade temporal: O(V + E).
     *
    */
    void cleanFilter();
    /** Função que adiciona um edge ao vetor ignoreEdge e coloca o seu atributo ignore a true.
     * @param edge edge a adicionar.
    */
    void addIgnoreEdge(Edge<string> *edge);
    /** Função que remove um edge do vetor ignoreEdge e coloca o seu atributo ignore a false.
     * @param edge edge a ser removido.
    */
    void removeIgnoreEdge(Edge<string> *edge);
    /** Função que guarda alguns atributos diretamente na network em questão.
     * @param avg media pipe % de utilizacao
     * @param min minimo pipe % de utilizacao
     * @param max maximo pipe % de utilizacao
    */
    void saveVals(double avg, double min, double max);
    /** Função que calcula a média de percentagem de utilização dos pipes de uma network em questão.
     * @param *min um apontador para um valor minimo de percentagem de utilizacao.
     * @param *max um apontador para um valor maximo de percentagem de utilizacao.
     * @param first um boolean que passa o valor true se for a primeira vez que a funcao esta a ser executada no programa.
    */
    double avgPipeUsage(double *min, double *max, bool first);
    /** Função que corre o algoritmo edmondsKarp num grafo fornecido com um valor extra para limitar a usage dos pipes
     * Complexidade temoporal: O(V E²).
     * @param network grafo em que vai ser utilizado o algoritmo.
     * @param srcCode código do vértice de origem.
     * @param destCode código do vértice de destino.
     * @param avg media da percentagem de usage dos pipes
    */
    void edmondsKarp(string srcCode, string destCode, double avg);
    /** Função que procura a capacidade residual mínima ao longo do augmenting path.
     * Complexidade temporal: O(V).
     * @param src vértice de origem.
     * @param dest vértice de destino.
     * @param avg media da percentagem de usage dos pipes
     * @return capacidade residual mínima.
    */
    double minResidualPath(Vertex<string> *src, Vertex<string> *dest, double avg);
    /** Função que procura um augmenting path num grafo fornecido usando BFS.
     * Complexidade temporal: O(V + E).
     * @param network grafo onde vai ser procurado o augmenting path.
     * @param src vértice de origem.
     * @param dest vértice de destino.
     * @param avg media da percentagem de usage dos pipes
     * @return true se encontar um caminho ou false se não encontar.
    */
    bool findAugmentingPath(Vertex<string> *src, Vertex<string> *dest, double avg);
};


#endif //WATERMANAGEMENTDA_MANAGEMENTSYSTEM_H
