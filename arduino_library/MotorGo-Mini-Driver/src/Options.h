// Options is a class to allow a user to select options for the program on boot
// It makes use of the onboard BOOT button and the onboard LED to allow the user to select options

#ifndef OPTIONS_H
#define OPTIONS_H

#include <Arduino.h>
#include <vector>

class Options {
public:
    Options(int num_options);

    // add an option to the list of options
    int addOption(String name);

    // run the options menu
    void run();

    // get the selected option
    int getSelectedOption();
    String getSelectedOptionName();


private:
    // the number of options
    size_t numOptions;

    // the list of options
    std::vector<String> options;

    // the selected option
    int selectedOption;

    // Built in LED pin number
    static const int LED_PIN = 2;

    // Built in BOOT button pin number
    static const int BOOT_PIN = 0;
};

#endif