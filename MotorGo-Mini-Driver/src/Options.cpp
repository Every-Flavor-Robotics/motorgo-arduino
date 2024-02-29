#include "Options.h"

void blinkFastMilliseconds(int pin, size_t ms)
{
    int delayTime = 25; // ms
    for (int i = 0; i < ms / delayTime / 2; i++)
    {
        digitalWrite(pin, HIGH);
        delay(delayTime);
        digitalWrite(pin, LOW);
        delay(delayTime);
    }
}

void blinkNTimes(int pin, int n, int delayMs)
{
    for (int i = 0; i < n; i++)
    {
        digitalWrite(pin, HIGH);
        delay(delayMs);
        digitalWrite(pin, LOW);
        delay(delayMs);
    }
}

void blinkNTimesSlow(int pin, int n)
{
    blinkNTimes(pin, n, 250);
}

void blinkNTimesQuick(int pin, int n)
{
    blinkNTimes(pin, n, 50);
}

void blinkOffbeat(int pin, int n)
{
    for (int i = 0; i < n; i++)
    {
        digitalWrite(pin, HIGH);
        delay(50);
        digitalWrite(pin, LOW);
        delay(50);
        digitalWrite(pin, HIGH);
        delay(50);
        digitalWrite(pin, LOW);
        delay(250);
    }
}

Options::Options(int numOptions) : numOptions(numOptions)
{
    // Create options array
    options.reserve(numOptions);
    selectedOption = -1;

    // Configure LED pin
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Configure BOOT pin
    pinMode(BOOT_PIN, INPUT_PULLUP);
}

int Options::addOption(String name)
{
    if (options.size() < numOptions)
    {
        options.push_back(name);
        return options.size() - 1;
    }
    else
    {
        return -1;
    }
}

int Options::getSelectedOption()
{
    return selectedOption;
}

String Options::getSelectedOptionName()
{
    return options[selectedOption];
}

void Options::run()
{
    // Blink LED fast to indicate options menu
    blinkFastMilliseconds(LED_PIN, 1500);
    delay(300);
    blinkFastMilliseconds(LED_PIN, 500);

    // Wait 4 seconds for user to select option
    // User selects option by pressing BOOT button n times

    bool selectionValid = false;
    while(!selectionValid)
    {
        int numPresses = 0;
        // start time
        unsigned long startTime = millis();
        while (millis() - startTime < 3000)
        {
            // check if BOOT button is pressed
            if (digitalRead(BOOT_PIN) == LOW)
            {
                // wait for button to be released
                while (digitalRead(BOOT_PIN) == LOW)
                {
                    delay(10);
                    blinkNTimesQuick(LED_PIN, 1);
                }
                // Reset start time
                startTime = millis();

                // increment number of presses
                numPresses++;

            }
        }

        // check if number of presses is valid - option selected
        if (numPresses <= numOptions && numPresses > 0)
        {
            // Confirm selection
            blinkNTimesSlow(LED_PIN, numPresses);
            delay(500);
            // Blink fast to ask for confirmation
            blinkFastMilliseconds(LED_PIN, 500);

            bool selectionConfirmed = false;
            // Wait 2 seconds for user to confirm selection
            // User confirms selection by pressing BOOT button
            unsigned long startTime = millis();
            while (millis() - startTime < 2000)
            {
                // check if BOOT button is pressed
                if (digitalRead(BOOT_PIN) == LOW)
                {
                    // wait for button to be released
                    while (digitalRead(BOOT_PIN) == LOW)
                    {
                        delay(10);
                        // blink LED to indicate press
                        blinkNTimesQuick(LED_PIN, 1);
                    }

                    // selection confirmed
                    selectionConfirmed = true;
                    break;
                }
            }

            if(selectionConfirmed)
            {
                delay(500);
                // Blink LED to indicate selection confirmed
                blinkNTimesSlow(LED_PIN, numPresses);

                // set selected option
                selectedOption = numPresses - 1;
                selectionValid = true;
            }

        }
        // No selection, default option
        else if(numPresses == 0)
        {
            selectionValid = true;
            blinkOffbeat(LED_PIN, 3);
        }

        if(!selectionValid)
        {
            // Error
            blinkOffbeat(LED_PIN, 3);

            delay(300);

            // Return to options menu
            blinkFastMilliseconds(LED_PIN, 3000);
            delay(300);
            blinkFastMilliseconds(LED_PIN, 500);
        }
    }
}
