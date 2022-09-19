#ifndef APP_H
#define APP_H

#include <Models.h>

class App {
    void print_help();
    void command_handler();

    // Commands 
    bool add_order();
    bool get_orders();
    // bool get_orders();
public:
    App();

    void run();
private:
};

#endif // APP_H
