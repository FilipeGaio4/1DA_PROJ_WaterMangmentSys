
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ManagementSystem.h"
#include <cmath>
#include "terminalColors.h"

using namespace std;
#ifndef DATASET_DIR
#define DATASET_DIR "."  // Default to current directory if OUTPUT_DIR is not defined
#endif


void ManagementSystem::readCities(string filename) {
    string city;
    int id;
    string tempId;
    string code;
    double demand;
    string tempDemand;
    int population = 0;
    string tempPopulation;
    ifstream citiesFile(string(DATASET_DIR) + "/" + filename);

    if (!citiesFile) {
        cerr << "Failed to open file\n";
        exit(EXIT_FAILURE);
        return;
    }

    string line;
    getline(citiesFile, line); //ignore the header

    while (getline(citiesFile, line)) {
        istringstream iss(line);
        getline(iss, city, ',');
        getline(iss, tempId, ',');
        getline(iss, code, ',');
        getline(iss, tempDemand, ',');
        getline(iss, tempPopulation, '\r');


        id = stoi(tempId);
        demand = stod(tempDemand);
        population = stoi(tempPopulation);


        network.addVertex(code);
        Vertex<string> *vertex = network.findVertex(code);
        vertex->setType(1);
        vertex->setCity(city);
        vertex->setId(id);
        vertex->setDemand(demand);
        vertex->setPopulation(population);
        cities.push_back(vertex);
    }


}

void ManagementSystem::readReservoirs(std::string filename) {
    ifstream in(string(DATASET_DIR) + "/" + filename);
    int count = 0;
    string s;
    if (!in) {
        cerr << "Failed to open file\n";
        exit(EXIT_FAILURE);
        return;
    }
    while (getline(in, s)) {
        if (count != 0) {
            istringstream iss(s);
            string reservoir, municipality, id, code;
            string maxDelivery;
            getline(iss, reservoir, ',');
            getline(iss, municipality, ',');
            getline(iss, id, ',');
            getline(iss, code, ',');
            getline(iss, maxDelivery, '\r');

            network.addVertex(code);
            Vertex<string> *vertex = network.findVertex(code);
            vertex->setType(0);
            vertex->setReservoir(reservoir);
            vertex->setId(stoi(id));
            vertex->setMaxDelivery(stoi(maxDelivery));
            reservoirs.push_back(vertex);
        }
        count++;
    }
}

void ManagementSystem::readStations(std::string filename) {
    ifstream in(string(DATASET_DIR) + "/" + filename);
    string line;
    string id;
    string code;
    if (!in) {
        cerr << "Failed to open file\n";
        exit(EXIT_FAILURE);
        return;
    }
    getline(in, line);
    while (getline(in, line)) {
        istringstream iss(line);
        getline(iss, id, ',');
        getline(iss, code, '\r');
        network.addVertex(code);
        Vertex<string> *vertex = network.findVertex(code);
        vertex->setType(2);
        vertex->setId(stoi(id));
        stations.push_back(vertex);
    }
}


void ManagementSystem::readPipes(std::string filename) {
    ifstream in("../Datasets/" + filename);
    string srcCode;
    string destCode;
    string scapacity;
    string sdirection;
    int capacity;
    int direction;
    string line;
    getline(in, line);
    while (getline(in, line)) {
        istringstream iss(line);
        getline(iss, srcCode, ',');
        getline(iss, destCode, ',');
        getline(iss, scapacity, ',');
        getline(iss, sdirection, '\r');
        capacity = stoi(scapacity);
        direction = stoi(sdirection);
        if (direction) {
            network.addEdge(srcCode, destCode, capacity);
        } else {
            network.addBidirectionalEdge(srcCode, destCode, capacity);
        }
    }
}


bool ManagementSystem::findAugmentingPath(Vertex<string> *src, Vertex<string> *dest) {
    for (Vertex<string> *vertex: network.getVertexSet()) {
        vertex->setVisited(false);
    }
    src->setVisited(true);
    queue<Vertex<string> *> q;
    q.push(src);

    while (!q.empty() && !dest->isVisited()) {
        Vertex<string> *vertex = q.front();
        q.pop();

        for (Edge<string> *edge: vertex->getAdj()) {
            Vertex<string> *travel = edge->getDest();
            if (!travel->isVisited() && (edge->getWeight() - edge->getFlow()) > 0 && !travel->isIgnore() &&
                !edge->isIgnore()) {
                travel->setVisited(true);
                travel->setPath(edge);
                q.push(travel);
            }
        }
        for (Edge<string> *edge: vertex->getIncoming()) {
            Vertex<string> *travel = edge->getOrig();
            if (!travel->isVisited() && edge->getFlow() > 0 && !travel->isIgnore() && !edge->isIgnore()) {
                travel->setVisited(true);
                travel->setPath(edge);
                q.push(travel);
            }
        }
    }

    return dest->isVisited();
}

bool ManagementSystem::findAugmentingPath(Graph<string> &network, Vertex<string> *src, Vertex<string> *dest) {
    for (Vertex<string> *vertex: network.getVertexSet()) {
        vertex->setVisited(false);
    }
    src->setVisited(true);
    queue<Vertex<string> *> q;
    q.push(src);

    while (!q.empty() && !dest->isVisited()) {
        Vertex<string> *vertex = q.front();
        q.pop();

        for (Edge<string> *edge: vertex->getAdj()) {
            Vertex<string> *travel = edge->getDest();
            if (!travel->isVisited() && (edge->getWeight() - edge->getFlow()) > 0) {
                travel->setVisited(true);
                travel->setPath(edge);
                q.push(travel);
            }
        }
        for (Edge<string> *edge: vertex->getIncoming()) {
            Vertex<string> *travel = edge->getOrig();
            if (!travel->isVisited() && edge->getFlow() > 0) {
                travel->setVisited(true);
                travel->setPath(edge);
                q.push(travel);
            }
        }
    }

    return dest->isVisited();
}

double ManagementSystem::minResidualPath(Vertex<std::string> *src, Vertex<std::string> *dest) {
    double minv = INF;


    for (Vertex<string> *vertex = dest; vertex != src;) {
        Edge<string> *edge = vertex->getPath();
        if (edge->getDest() == vertex) {
            minv = min(minv, edge->getWeight() - edge->getFlow());
            vertex = edge->getOrig();
        } else {
            minv = min(minv, edge->getFlow());
            vertex = edge->getDest();
        }
    }

    return minv;
}

void ManagementSystem::edmondsKarp(string srcCode, string destCode) {
    Vertex<string> *src = network.findVertex(srcCode);
    Vertex<string> *dest = network.findVertex(destCode);
    if (src == nullptr || dest == nullptr) {
        cout << "Given source and/or target don't exist." << endl;
        return;
    }
    for (Vertex<string> *vertex: network.getVertexSet()) {
        for (Edge<string> *edge: vertex->getAdj()) {
            edge->setFlow(0);
        }
    }
    while (findAugmentingPath(src, dest)) {
        double bottleneck = minResidualPath(src, dest);
        for (Vertex<string> *vertex = dest; vertex != src;) {
            Edge<string> *edge = vertex->getPath();
            if (vertex == edge->getOrig()) {
                edge->setFlow(edge->getFlow() - bottleneck);
                vertex = edge->getDest();
            } else {
                edge->setFlow(edge->getFlow() + bottleneck);
                vertex = edge->getOrig();

            }
        }
    }
    network.removeVertex("S");
    network.removeVertex("T");
}

void ManagementSystem::edmondsKarp(Graph<string> &network, string srcCode, string destCode) {
    Vertex<string> *src = network.findVertex(srcCode);
    Vertex<string> *dest = network.findVertex(destCode);
    if (src == nullptr || dest == nullptr) {
        cout << "Given source and/or target don't exist." << endl;
        return;
    }
    for (Vertex<string> *vertex: network.getVertexSet()) {
        for (Edge<string> *edge: vertex->getAdj()) {
            edge->setFlow(0);
        }
        for (Edge<string> *edge: vertex->getIncoming()) {
            edge->setFlow(0);
        }
    }
    while (findAugmentingPath(network, src, dest)) {
        double bottleneck = minResidualPath(src, dest);
        for (Vertex<string> *vertex = dest; vertex != src;) {
            Edge<string> *edge = vertex->getPath();
            if (vertex == edge->getOrig()) {
                edge->setFlow(edge->getFlow() - bottleneck);
                vertex = edge->getDest();
            } else {
                edge->setFlow(edge->getFlow() + bottleneck);
                vertex = edge->getOrig();

            }
        }
    }
}

const Graph<string> &ManagementSystem::getNetwork() const {
    return network;
}

void ManagementSystem::setNetwork(const Graph<string> &network) {
    ManagementSystem::network = network;
}

const vector<Vertex<string> *> &ManagementSystem::getCities() const {
    return cities;
}

void ManagementSystem::setCities(const vector<Vertex<string> *> &cities) {
    ManagementSystem::cities = cities;
}

const vector<Vertex<string> *> &ManagementSystem::getReservoirs() const {
    return reservoirs;
}

void ManagementSystem::setReservoirs(const vector<Vertex<string> *> &reservoirs) {
    ManagementSystem::reservoirs = reservoirs;
}

const vector<Vertex<string> *> &ManagementSystem::getStations() const {
    return stations;
}

void ManagementSystem::setStations(const vector<Vertex<string> *> &stations) {
    ManagementSystem::stations = stations;
}

void ManagementSystem::createSuperSource() {
    network.addVertex("S");
    for (Vertex<string> *reservoir: reservoirs) {
        if (!reservoir->isIgnore())
            network.addEdge("S", reservoir->getInfo(), reservoir->getMaxDelivery());
    }
}

void ManagementSystem::createSuperSink() {
    network.addVertex("T");
    for (Vertex<string> *city: cities) {
        network.addEdge(city->getInfo(), "T", city->getDemand());
    }
}

void ManagementSystem::createSuperSource(Graph<string> &network) {
    network.addVertex("S");
    for (Vertex<string> *v: network.getVertexSet()) {
        if (v->getInfo() != "S" && v->getType() == 0) network.addEdge("S", v->getInfo(), v->getMaxDelivery());
    }
}

void ManagementSystem::createSuperSink(Graph<string> &network) {
    network.addVertex("T");
    for (Vertex<string> *v: network.getVertexSet()) {
        if (v->getInfo() != "S" && v->getInfo() != "T" && v->getType() == 1)
            network.addEdge(v->getInfo(), "T", v->getDemand());
    }
}

void ManagementSystem::removeReservoir(string code) {
    Vertex<string> *vertex = network.findVertex(code);
    if (vertex == nullptr || vertex->getType() != 0) {
        cout << code << " is not a water reservoir" << endl;
        return;
    }

    createSuperSource();
    createSuperSink();
    edmondsKarp("S", "T");

    vector<pair<Vertex<string> *, double>> affected_cities;

    for (Vertex<string> *v: network.getVertexSet()) v->setVisited(false);

    queue<Vertex<string> *> q;
    q.push(vertex);
    vertex->setVisited(true);
    while (!q.empty()) {
        Vertex<string> *v = q.front();

        if (v->getType() == 1) {
            double flow = 0;
            for (Edge<string> *e: v->getIncoming()) flow += e->getFlow();
            affected_cities.push_back(make_pair(v, flow));
        } else {
            for (Edge<string> *e: v->getAdj()) {
                Vertex<string> *v_dest = e->getDest();
                if (!v_dest->isVisited()) {
                    v_dest->setVisited(true);
                    q.push(v_dest);
                }
            }
        }

        q.pop();
    }

    vector<Vertex<string> *> incoming_reservoirs;

    for (Vertex<string> *v: network.getVertexSet()) v->setVisited(false);

    for (const pair<Vertex<string> *, double> &pair: affected_cities) q.push(pair.first);
    while (!q.empty()) {
        Vertex<string> *v = q.front();

        if (v->getType() == 0 && v->getInfo() != code) {
            incoming_reservoirs.push_back(v);
        } else {
            for (Edge<string> *e: v->getIncoming()) {
                Vertex<string> *v_dest = e->getOrig();
                if (!v_dest->isVisited()) {
                    v_dest->setVisited(true);
                    q.push(v_dest);
                }
            }
        }

        q.pop();
    }

    bool finish = false;

    while (!finish) {

        int cities = affected_cities.size();
        int reservoirs = incoming_reservoirs.size();

        for (Vertex<string> *v: network.getVertexSet()) v->setVisited(false);
        for (Vertex<string> *v: incoming_reservoirs) q.push(v);

        while (!q.empty()) {
            Vertex<string> *v = q.front();

            if (v->getType() == 1) {
                bool is_alr_affected = false;
                for (const pair<Vertex<string> *, bool> &pair : affected_cities) {
                    if (v->getInfo() == pair.first->getInfo()) {
                        is_alr_affected = true;
                        break;
                    }
                }
                if (!is_alr_affected) {
                    double flow = 0;
                    for (Edge<string> *e: v->getIncoming()) flow += e->getFlow();
                    affected_cities.push_back(make_pair(v, flow));
                }
            } else {
                for (Edge<string> *e: v->getAdj()) {
                    Vertex<string> *v_dest = e->getDest();
                    if (!v_dest->isVisited()) {
                        v_dest->setVisited(true);
                        q.push(v_dest);
                    }
                }
            }

            q.pop();
        }

        if (affected_cities.size() == cities) finish = true;

        else {
            for (Vertex<string> *v: network.getVertexSet()) v->setVisited(false);

            for (const pair<Vertex<string> *, double> &pair : affected_cities) q.push(pair.first);
            while (!q.empty()) {
                Vertex<string> *v = q.front();

                if (v->getType() == 0 && v->getInfo() != code) {
                    incoming_reservoirs.push_back(v);
                } else {
                    for (Edge<string> *e: v->getIncoming()) {
                        Vertex<string> *v_dest = e->getOrig();
                        if (!v_dest->isVisited()) {
                            v_dest->setVisited(true);
                            q.push(v_dest);
                        }
                    }
                }

                q.pop();
            }
        }

        if (incoming_reservoirs.size() == reservoirs) finish = true;

    }

    cout << "Affected cities: " << endl;

    for (Vertex<string> *v: network.getVertexSet()) v->setVisited(false);

    Graph<string> copy;

    for (Vertex<string> *v: reservoirs) {
        if (find(incoming_reservoirs.begin(), incoming_reservoirs.end(), v) != incoming_reservoirs.end()) {
            if (copy.addVertex(v)) {
                queue<Vertex<string> *> q;
                q.push(v);
                while (!q.empty()) {
                    Vertex<string> *vq = q.front();
                    for (Edge<string> *e: vq->getAdj()) {
                        Vertex<string> *v_dest = e->getDest();
                        if (copy.addVertex(v_dest)) q.push(v_dest);
                    }
                    q.pop();
                }
            }
        }
    }

    createSuperSource(copy);
    createSuperSink(copy);
    edmondsKarp(copy, "S", "T");

    for (Vertex<string> *v: copy.getVertexSet()) {
        if (v->getType() == 1) {
            for (auto it = affected_cities.begin(); it != affected_cities.end(); it++) {
                if (v->getInfo() == it->first->getInfo()) {
                    double flow = 0;
                    for (Edge<string> *e: v->getIncoming()) flow += e->getFlow();
                    if (flow != it->second)
                        cout << '-' << v->getCity() << " (" << v->getInfo() << ") - " << flow << " less than "
                             << it->second << endl;
                    affected_cities.erase(it);
                    break;
                }
            }
        }
    }
    for (pair < Vertex<string> * , double > pair: affected_cities)
        cout << '-' << pair.first->getCity()
             << " (" << pair.first->getInfo() << ") - " << 0 << " less than " << pair.second << endl;
    copy.removeVertex("S");
    copy.removeVertex("T");
}

void ManagementSystem::resetIgnore() {
    for (Vertex<string> *vertex: network.getVertexSet()) {
        vertex->setIgnore(false);
    }
}

void ManagementSystem::setIgnore(vector<Vertex<std::string> *> vertexes) {
    for (Vertex<std::string> *vertex: vertexes) {
        vertex->setIgnore(true);
    }
}

vector<Vertex<std::string> *> ManagementSystem::getIgnoreVertex() const {
    return ignoreVertex;
}

void ManagementSystem::setIgnoreVertex(const vector<Vertex<string> *> &ignoreVertex) {
    ManagementSystem::ignoreVertex = ignoreVertex;
}


void ManagementSystem::printFilter() {
    vector<Vertex<string> *> reservoirs;
    vector<Vertex<string> *> stations;
    for (Vertex<string> *vertex: ignoreVertex) {
        if (vertex->getType() == 0) {
            reservoirs.push_back(vertex);
        } else if (vertex->getType() == 2) {
            stations.push_back(vertex);
        }
    }

    cout << "Reservoirs ignored:" << endl;
    int cnt = 0;
    for (Vertex<string> *vertex: reservoirs) {

        if (cnt == reservoirs.size() - 1) {
            cout << vertex->getInfo() << "." << endl;
        } else {
            cout << vertex->getInfo() << ", ";
        }
        cnt++;


    }
    cnt = 0;
    cout << endl << "Pumping Stations ignored:" << endl;
    for (Vertex<string> *vertex: stations) {
        if (cnt == stations.size() - 1) {
            cout << vertex->getInfo() << "." << endl;
        } else {
            cout << vertex->getInfo() << ", ";
        }
        cnt++;
    }
    cnt = 0;
    cout << endl << "Pipes ignored:" << endl;
    for (Edge<string> *edge: ignoreEdge) {
        if (cnt == ignoreEdge.size() - 1) {
            cout << "From " << edge->getOrig()->getInfo() << " To " << edge->getDest()->getInfo() << "." << endl;
        } else {
            cout << "From " << edge->getOrig()->getInfo() << " To " << edge->getDest()->getInfo() << ", " << endl;
        }
        cnt++;
    }
    cout << endl;

}

void ManagementSystem::addIgnoreVertex(Vertex<std::string> *vertex) {
    if (std::find(ignoreVertex.begin(), ignoreVertex.end(), vertex) == ignoreVertex.end()) {
        ignoreVertex.push_back(vertex);
        vertex->setIgnore(true);
        cout << "\nIgnored with success\n" << endl;
    }
}

void ManagementSystem::removeIgnoreVertex(Vertex<std::string> *vertex) {
    vector<Vertex<string> *> temp;
    bool found = false;
    for (auto vert: ignoreVertex) {
        if (vert != vertex) {
            temp.push_back(vert);
        } else {
            found = true;
        }
    }
    if (found) {
        vertex->setIgnore(false);
        ignoreVertex = temp;
        cout << "\nVertex removed from the filter\n" << endl;
    } else cout << "\nSelected vertex wasn't in the filter\n" << endl;
}

void ManagementSystem::cleanFilter() {
    for (auto vertex: ignoreVertex) {
        vertex->setIgnore(false);
    }
    for (auto edge: ignoreEdge) {
        edge->setIgnore(false);
        auto edgeR = edge->getReverse();
        if (edgeR != nullptr) {
            edgeR->setIgnore(false);
        }
    }
    ignoreVertex.clear();
    ignoreEdge.clear();
}

void ManagementSystem::addIgnoreEdge(Edge<string> *edge) {
    if (std::find(ignoreEdge.begin(), ignoreEdge.end(), edge) == ignoreEdge.end()) {
        ignoreEdge.push_back(edge);
        edge->setIgnore(true);
        if (edge->getReverse() != nullptr)
            edge->getReverse()->setIgnore(true);
    }
}

const vector<Edge<string> *> &ManagementSystem::getIgnoreEdge() const {
    return ignoreEdge;
}

void ManagementSystem::setIgnoreEdge(const vector<Edge<string> *> &ignoreEdge) {
    ManagementSystem::ignoreEdge = ignoreEdge;
}

void ManagementSystem::removeIgnoreEdge(Edge<string> *edge) {
    vector<Edge<string> *> temp;
    bool found = false;
    for (auto e: ignoreEdge) {
        if (e != edge) {
            temp.push_back(e);
        } else {
            found = true;
        }
    }
    if (found) {
        edge->setIgnore(false);
        auto edgeR = edge->getReverse();
        if (edgeR != nullptr) {
            edgeR->setIgnore(false);
        }
        ignoreEdge = temp;
    } else {
        cout << "\nSelected pipe wasn't in the filter\n" << endl;
    }
}


void ManagementSystem::saveVals(double avg, double min, double max) {
    network.setAvgPipeUsg(avg);
    network.setMinPipeUsg(min);
    network.setMaxPipeUsg(max);
    for (auto vertex: network.getVertexSet()) {
        if (vertex->getType() == 1) {
            double flow = 0;
            for (auto edge: vertex->getIncoming()) {
                flow += edge->getFlow();
            }
            vertex->setRecievedFlow(flow);
        }


        for (auto edge: vertex->getAdj()) {
            edge->setFlowUsage(edge->getFlow());
        }
    }
}

double ManagementSystem::avgPipeUsage(double *min, double *max, bool first) {
    int count = 0;
    double avg = 0;
    double atual;
    Edge<string> *emin;
    Edge<string> *emax;


    for (auto vertex: network.getVertexSet()) {
        for (auto edge: vertex->getAdj()) {
            count++;
            atual = (edge->getFlow() / edge->getWeight()) * 100;
            avg += atual;
            if (atual < *min) {
                *min = atual;
                emin = edge;
            }
            if (atual > *max) {
                *max = atual;
                emax = edge;
            }
        }
    }
    avg /= count;


    if (!first) {
        cout << "There are " << count << " pipes." << endl;
        cout << BC_RED << TC_WHITE << "Before balancing the average % of usage of pipes were : "
             << network.getAvgPipeUsg() << "% " << BC_RESET << TC_RESET << endl;
        cout << BC_RED << TC_WHITE << "Before balancing the biggest different of % of usage of two pipes were : "
             << network.getMaxPipeUsg() - network.getMinPipeUsg() << "% " << BC_RESET << TC_RESET << endl;
        cout << BC_GREEN << TC_WHITE << "After balancing the  average % of usage of pipes if : " << avg << "% "
             << BC_RESET << TC_RESET << endl;
        cout << BC_GREEN << TC_WHITE << "After balancing the biggest different of % of usage of two pipes were : "
             << *max - *min << "% " << BC_RESET << TC_RESET << endl;
        cout << "Min is : " << *min << endl;
        cout << "Min pipe goes from : " << emin->getOrig()->getInfo() << " to " << emin->getDest()->getInfo()
             << " with flow/cap " << emin->getFlow() << "/" << emin->getWeight() << endl;
        cout << "Max is : " << *max << endl;
        cout << "Max pipe goes from : " << emax->getOrig()->getInfo() << " to " << emax->getDest()->getInfo()
             << " with flow/cap " << emax->getFlow() << "/" << emax->getWeight() << endl;
    }

    return avg;
}


bool ManagementSystem::findAugmentingPath(Vertex<string> *src, Vertex<string> *dest, double avg) {
    for (Vertex<string> *vertex: network.getVertexSet()) {
        vertex->setVisited(false);
    }
    src->setVisited(true);
    queue<Vertex<string> *> q;
    q.push(src);

    while (!q.empty() && !dest->isVisited()) {
        Vertex<string> *vertex = q.front();
        q.pop();

        for (Edge<string> *edge: vertex->getAdj()) {
            Vertex<string> *travel = edge->getDest();
            if (!travel->isVisited() && (ceil(edge->getWeight() * (( avg + ((100-avg))/2)/100)) - edge->getFlow()) > 0) {
                travel->setVisited(true);
                travel->setPath(edge);
                q.push(travel);
            }
        }
        for (Edge<string> *edge: vertex->getIncoming()) {
            Vertex<string> *travel = edge->getOrig();
            if (!travel->isVisited() && edge->getFlow() > 0) {
                travel->setVisited(true);
                travel->setPath(edge);
                q.push(travel);
            }
        }
    }

    return dest->isVisited();
}

double ManagementSystem::minResidualPath(Vertex<std::string> *src, Vertex<std::string> *dest, double avg) {
    double minv = INF;


    for (Vertex<string> *vertex = dest; vertex != src;) {
        Edge<string> *edge = vertex->getPath();
        if (edge->getDest() == vertex) {
            minv = min(minv, ceil(edge->getWeight() * (( avg + ((100-avg))/2)/100)) - edge->getFlow());
            vertex = edge->getOrig();
        } else {
            minv = min(minv, edge->getFlow());
            vertex = edge->getDest();
        }
    }

    return minv;
}

void ManagementSystem::edmondsKarp(string srcCode, string destCode, double avg) {
    Vertex<string> *src = network.findVertex(srcCode);
    Vertex<string> *dest = network.findVertex(destCode);
    if (src == nullptr || dest == nullptr) {
        cout << "Given source and/or target don't exist." << endl;
        return;
    }
    for (Vertex<string> *vertex: network.getVertexSet()) {
        for (Edge<string> *edge: vertex->getAdj()) {
            edge->setFlow(0);
        }
    }
    while (findAugmentingPath(src, dest, avg)) {
        double bottleneck = minResidualPath(src, dest, avg);
        for (Vertex<string> *vertex = dest; vertex != src;) {
            Edge<string> *edge = vertex->getPath();
            if (vertex == edge->getOrig()) {
                edge->setFlow(edge->getFlow() - bottleneck);
                vertex = edge->getDest();
            } else {
                edge->setFlow(edge->getFlow() + bottleneck);
                vertex = edge->getOrig();

            }
        }
    }
    network.removeVertex("S");
    network.removeVertex("T");
}
