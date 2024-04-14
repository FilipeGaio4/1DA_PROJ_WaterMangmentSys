
#include <fstream>
#include "data_structures/Graph.h"
#include "Menu.h"
#include "terminalColors.h"

using namespace std;


#ifndef OUTPUT_DIR
#define OUTPUT_DIR "."  // Default to current directory if OUTPUT_DIR is not defined
#endif


Menu::Menu(ManagementSystem system) : system(std::move(system)) {};


void Menu::start() {
    system.createSuperSource();
    system.createSuperSink();
    system.edmondsKarp("S", "T");
    double min = 101;
    double max = -1;
    double avg = system.avgPipeUsage(&min, &max, true);

    system.saveVals(avg, min, max);


    while (true) {
        string temp;
        cout << "Welcome to our Water Management system." << endl << "What are you looking for \n" << endl;
        cout << "1) Max flow (4.1)" << endl;
        cout << "2) Filter Menu (4.2)" << endl;
        cout << "3) Remove water reservoir without recalculation entire graph's max flow (3.1)" << endl;
        cout << "4) Balancing Algorithm (2.3)" << endl;
        cout << "0) Exit the program" << endl;
        cin >> temp;
        if (temp == "0") {
            cout << endl << TC_GREEN << "Thank you for using the Water Management System!" << endl << TC_RESET;
            break;
        }
        if (temp == "1") {

            printMenu4_1();
        } else if (temp == "2") {
            filterMenu4_2();
        } else if (temp == "3") {
            cout << "Insert water reservoir code: ";
            string code;
            cin >> code;
            system.removeReservoir(code);
            enterToContinue();
        } else if (temp == "4") {
            system.createSuperSource();
            system.createSuperSink();
            system.edmondsKarp("S", "T", system.getNetwork().getAvgPipeUsg());
            min = 101;
            max = -1;
            double avg = system.avgPipeUsage(&min, &max, false);

            enterToContinue();
        }
        else{
            cout << BC_RED << TC_WHITE << "Enter a valid option!" << TC_RESET << BC_RESET << endl;
            enterToContinue();
        }
    }
}

void Menu::enterToContinue() {
    cout << TC_WHITE << BC_RED << "PRESS ENTER TO CONTINUE" << TC_RESET << BC_RESET << endl;
    string z;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, z);
}


void Menu::filterMenu4_2() {
    while (true) {
        Graph<string> tempNetwork = system.getNetwork();
        string temp;
        cout << "Filter of ignored/nonfunctional stops." << endl;
        system.printFilter();
        cout << "1) Add Reservoir/Station" << endl;
        cout << "2) Remove Reservoir/Station" << endl;
        cout << "3) Add Pipe" << endl;
        cout << "4) Remove pipe" << endl;
        cout << "5) Empty the filter" << endl;
        cout << "6) Check affected Cities" << endl;
        cout << "0) Return" << endl;
        cin >> temp;

        if (temp == "0") {
            system.cleanFilter();
            break;
        }
        if (temp == "1") {
            Vertex<string> *vertex;
            string code;
            cout << "What reservoir/station do you want to add to the filter? (Code only)" << endl;
            cin >> code;
            vertex = tempNetwork.findVertex(code);
            if (vertex) {
                if (vertex->getType() != 1) system.addIgnoreVertex(vertex);
                else {
                    cout << endl << TC_WHITE << BC_RED
                         << "The code that you typed represents a city! Enter the code of a reservoir or station"
                         << TC_RESET << BC_RESET << endl << endl;
                    enterToContinue();
                }
            } else {
                cout << endl << TC_WHITE << BC_RED << "Invalid code" << TC_RESET << BC_RESET << endl << endl;
                enterToContinue();
            }
        }
        if (temp == "2") {
            Vertex<string> *vertex;

            string code;
            cout << "What reservoir/station do you want to remove from the filter? (Code only)" << endl;
            cin >> code;
            vertex = tempNetwork.findVertex(code);
            if (vertex) {
                if (vertex->getType() != 1) {
                    system.removeIgnoreVertex(vertex);
                    enterToContinue();
                } else {
                    cout << endl << TC_WHITE << BC_RED
                         << "The code that you typed represents a city! Enter the code of a reservoir or station"
                         << TC_RESET << BC_RESET << endl << endl;
                    enterToContinue();
                }
            } else {
                cout << endl << TC_WHITE << BC_RED << "Invalid code" << TC_RESET << BC_RESET << endl << endl;
                enterToContinue();
            }
        }
        if (temp == "3") {
            Edge<string> *pipe = nullptr;
            string codeOrig, codeDest;
            cout << "What is the pipe's source and target? (Code only)" << endl;
            cout << "Source:" << endl;
            cin >> codeOrig;
            bool found = false;
            auto orig = tempNetwork.findVertex(codeOrig);
            if (orig != nullptr) {
                cout << "Dest:" << endl;
                cin >> codeDest;
                for (auto edge: orig->getAdj()) {
                    if (edge->getDest()->getInfo() == codeDest) {
                        pipe = edge;
                        found = true;
                    }
                }
                if (!found) {
                    cout << endl << TC_WHITE << BC_RED
                         << "Invalid code or there's no connection between source and dest!" << TC_RESET << BC_RESET
                         << endl << endl;
                    enterToContinue();
                } else system.addIgnoreEdge(pipe);
            } else {
                cout << endl << TC_WHITE << BC_RED << "Invalid code" << TC_RESET << BC_RESET << endl << endl;
                enterToContinue();
            }
        }
        if (temp == "4") {
            Edge<string> *pipe = nullptr;
            string codeOrig, codeDest;
            cout << "What is the pipe's source and target? (Code only)" << endl;
            cout << "Source:" << endl;
            cin >> codeOrig;
            bool found = false;
            auto orig = tempNetwork.findVertex(codeOrig);
            if (orig != nullptr) {
                cout << "Dest:" << endl;
                cin >> codeDest;
                for (auto edge: orig->getAdj()) {
                    if (edge->getDest()->getInfo() == codeDest) {
                        pipe = edge;
                        found = true;
                    }
                }
                if (!found) {
                    cout << endl << TC_WHITE << BC_RED
                         << "Invalid code or there's no connection between source and dest!" << TC_RESET << BC_RESET
                         << endl << endl;
                    enterToContinue();
                } else system.removeIgnoreEdge(pipe);
            } else {
                cout << endl << TC_WHITE << BC_RED << "Invalid code" << TC_RESET << BC_RESET << endl << endl;
                enterToContinue();
            }
        }
        if (temp == "5") {
            system.cleanFilter();
        }
        if (temp == "6") {
            system.createSuperSource();
            system.createSuperSink();
            system.edmondsKarp("S", "T");
            vector<pair<Vertex<string> *, double>> metDemand, missedDemand;

            int countNoChange = 0, countDecrease = 0, countIncrease = 0;

            for (auto city: system.getCities()) {
                double recieved = 0;
                for (auto edge: city->getIncoming()) {
                    recieved += edge->getFlow();
                }
                if (recieved == city->getRecievedFlow()) {
                    countNoChange++;
                } else if (recieved > city->getRecievedFlow()) {
                    countIncrease++;
                    cout << endl << TC_GREEN;
                    cout << city->getCity() << " - " << city->getInfo() << " - recieved more water." << endl;
                    cout << "Flow went from " << city->getRecievedFlow() << " to " << recieved << endl;
                    cout << "Defitic went from " << city->getDemand() - city->getRecievedFlow() << " to "
                         << city->getDemand() - recieved << TC_RESET << endl;

                } else {
                    countDecrease++;
                    cout << endl << TC_RED;
                    cout << city->getCity() << " - " << city->getInfo() << " - recieved less water." << endl;
                    cout << "Flow went from " << city->getRecievedFlow() << " to " << recieved << endl;
                    cout << "Defitic went from " << city->getDemand() - city->getRecievedFlow() << " to "
                         << city->getDemand() - recieved << TC_RESET << endl;

                }

            }

            if (countNoChange == system.getCities().size()) {
                cout << endl << "There were no changes in the flow." << endl;
            } else {
                if (countIncrease != 0) {
                    cout << endl << countIncrease << " cities got more water than before." << endl;
                }
                if (countDecrease != 0) {
                    cout << endl << countDecrease << " cities got less water than before." << endl;
                }
            }
            enterToContinue();
        }
        else{
            cout << BC_RED << TC_WHITE << "Enter a valid option!" << TC_RESET << BC_RESET << endl;
        }
    }
}


void Menu::printMenu4_1() {
    int flow = 0;
    vector<pair<Vertex<string> *, double>> metDemand, missedDemand;
    for (auto city: system.getCities()) {
        if (city->getDemand() <= city->getRecievedFlow()) {
            metDemand.emplace_back(city, city->getRecievedFlow());
        } else {
            missedDemand.emplace_back(city, city->getRecievedFlow());
        }
        flow += city->getRecievedFlow();
    }

    while (true) {
        cout << "What do you want to see?" << endl;
        cout << "1) Max flow" << endl;
        cout << "2) Cities that met the demand" << endl;
        cout << "3) Cities that didn't meet the demand" << endl;
        cout << "4) Specific city (search by code)" << endl;
        cout << "0) Go Back" << endl;

        string temp;
        cin >> temp;
        if (temp == "0") return;
        if (temp == "1") {
            ofstream out(string(OUTPUT_DIR) + "/maxFlow.csv");
            out << "Max Flow\r";
            out << flow << "\r";
            cout << TC_YELLOW << to_string(flow) << " is the max flow of the Water System." << TC_RESET << endl;
            cout << TC_MAGENTA << "Result saved on file: \"output/maxFlow.csv\"" << TC_RESET << endl;
        }
        if (temp == "2") {
            ofstream out(string(OUTPUT_DIR) + "/metDemand.csv");
            out << "City,Code,Value/Demand\r";
            cout << TC_YELLOW << "City - Code - Value" << TC_RESET << endl << endl;

            for (auto print: metDemand) {
                out << print.first->getCity() << "," << print.first->getInfo() << "," << print.second << "\n";
                cout << TC_GREEN << print.first->getCity() << " - " << print.first->getInfo() << " - " << print.second
                     << TC_RESET << endl << endl;

            }
            cout << TC_BLUE << metDemand.size() << "/" << system.getCities().size() << " cities met the demand."
                 << TC_RESET << std::endl;
            cout << TC_MAGENTA << "Result saved on file: \"output/metDemand.csv\"" << TC_RESET << endl;

        }
        if (temp == "3") {
            ofstream out(string(OUTPUT_DIR) + "/missedDemand.csv");
            out << "City,Code,Deficit,Value,Demand\r";
            cout << TC_YELLOW << "City - Code - Value/Demand" << endl << "Deficit:" << TC_RESET << "\n\n";
            for (auto print: missedDemand) {
                out << print.first->getCity() << "," << print.first->getInfo() << ","
                    << print.first->getDemand() - print.second << "," << print.second << "," << print.first->getDemand()
                    << "\n";
                cout << TC_RED << print.first->getCity() << " - " << print.first->getInfo() << " - " << print.second
                     << "/" << print.first->getDemand() << endl << "Deficit: "
                     << print.first->getDemand() - print.second << TC_RESET << endl << endl;
            }
            cout << TC_BLUE << missedDemand.size() << "/" << system.getCities().size()
                 << " cities didn't meet the demand." << TC_RESET << std::endl;
            cout << TC_MAGENTA << "Result saved on file: \"output/missedDemand.csv\"" << TC_RESET << endl;

        }
        if (temp == "4"){
            Vertex<string> *vertex;
            string code;
            cout << "What city do you want to check? (Code only)" << endl;
            cin >> code;
            vertex = system.getNetwork().findVertex(code);
            if (vertex) {
                if (vertex->getType() == 1) {
                    if (vertex->getRecievedFlow() >= vertex->getDemand()){
                        cout << TC_GREEN << vertex->getCity() << " - " << vertex->getInfo() << " reached the demand " << vertex->getRecievedFlow() << "/" << vertex->getDemand() << TC_RESET << endl;
                    }
                    else{
                        cout << TC_RED << vertex->getCity() << " - " << vertex->getInfo() << " didn't reach the demand " << vertex->getRecievedFlow() << "/" << vertex->getDemand() << TC_RESET << endl;
                        cout << TC_RED  << "It has a deficit of " << vertex->getDemand() - vertex->getRecievedFlow() << "." << TC_RESET<<endl;
                    }
                }
                else {
                    cout << endl << TC_WHITE << BC_RED
                         << "The code that you typed doesn't represent a city! Enter the code of a city"
                         << TC_RESET << BC_RESET << endl << endl;
                }
            } else {
                cout << endl << TC_WHITE << BC_RED << "Invalid code" << TC_RESET << BC_RESET << endl << endl;
            }
        }
        else{
            cout << BC_RED << TC_WHITE << "Enter a valid option!" << TC_RESET << BC_RESET << endl;
        }
        enterToContinue();

    }
}
