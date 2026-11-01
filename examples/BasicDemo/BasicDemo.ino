/*
 * ScanLabsCYDMatrixEffect
 *
 * Basic example
 *
 * Displays a Matrix-style animation on a CYD display
 * and periodically shows custom messages.
 */

#include <ScanLabsCYDMatrixEffect.h>

ScanLabsCYDMatrixEffect matrix;

uint32_t lastMessageTime = 0;
uint8_t messageIndex = 0;

const char* messages[] =
{
    "SCANLABS",
    "HELLO WORLD",
    "ESP32",
    "LVGL 9",
    "CYD MATRIX"
};

const uint8_t messageCount =
    sizeof(messages) /
    sizeof(messages[0]);

void setup()
{
    Serial.begin(115200);

    matrix.begin(
        USB_LEFT,  // Display rotation
        320,       // Screen width
        240,       // Screen height
        40,        // Matrix columns
        24,        // Matrix rows
        8,         // Column spacing
        25,        // Maximum brightness
        2000       // Boot screen duration (ms)
    );

    matrix.show(
        "SCANLABS",
        5000);
}

void loop()
{
    matrix.loop();

    if (millis() - lastMessageTime > 3000)
    {
        lastMessageTime = millis();

        matrix.show(
            messages[messageIndex],
            2000);

        messageIndex++;

        if (messageIndex >= messageCount)
        {
            messageIndex = 0;
        }
    }
}