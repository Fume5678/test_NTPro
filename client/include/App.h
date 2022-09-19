#ifndef APP_H
#define APP_H

#include <string>

class App {
public:
    App();

    void run();
private:
    void print_help();
    void command_handler();

};

#endif // APP_H
