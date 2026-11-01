#include <ScanLabsCYDMatrixEffect.h>

ScanLabsCYDMatrixEffect matrix;

void setup()
{
    matrix.begin(
        USB_LEFT,
        320,
        240,
        40,
        24,
        8,
        25,
        0
    );
}

void loop()
{
    matrix.loop();
}