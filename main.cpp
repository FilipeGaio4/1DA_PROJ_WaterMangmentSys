#include <iostream>
#include "src/ManagementSystem.h"
#include "src/Menu.h"

using namespace std;

int main() {
    ManagementSystem system;
    string size, extra, choice;

    do {
        cout << "What dataset do you want to use?" << endl;
        cout << "1. Small Dataset" << endl;
        cout << "2. Large Dataset" << endl;
        cin >> choice;
    } while (choice != "1" && choice != "2" );
    if(choice == "1"){
        size = "Project1DataSetSmall/";
        extra = "_Madeira.csv";
    }else{
        size = "Project1LargeDataSet/";
        extra = ".csv";
    }
    system.readCities( size + "Cities" + extra);
    cout << "Done cities"<<endl;

    system.readReservoirs(size + "Reservoir" + extra);
    cout << "Done reservoirs"<<endl;

    system.readStations(size + "Stations" + extra);
    cout << "Done stations"<<endl;

    system.readPipes(size + "Pipes" + extra);
    cout << "Done pipes"<<endl;
    Menu menu(system);
    menu.start();

    return 0;
}
