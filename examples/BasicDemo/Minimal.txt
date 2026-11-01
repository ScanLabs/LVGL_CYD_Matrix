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
        2000
    );

    matrix.show(
        "SCANLABS",
        5000
    );
}

void loop()
{
    matrix.loop();
}
