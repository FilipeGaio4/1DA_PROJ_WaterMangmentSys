
#ifndef WATERMANAGEMENTDA_MENU_H
#define WATERMANAGEMENTDA_MENU_H

#include "ManagementSystem.h"

using namespace std;


class Menu {
private:
    ManagementSystem system;
public:
    explicit Menu(ManagementSystem system);

    void start();

    void enterToContinue();

    void printMenu4_1();

    void filterMenu4_2();
};


#endif //WATERMANAGEMENTDA_MENU_H
