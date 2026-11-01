# LVGL\_CYD\_Matrix

A lightweight Matrix digital rain effect library for ESP32 Cheap Yellow Display (CYD) boards using LVGL 9 and LVGL\_CYD.

The library reproduces the classic "Matrix" falling character effect and allows secret messages to appear dynamically within the animation.

\---

# Features

* Matrix-style digital rain animation
* Fully configurable at runtime
* Dynamic secret message injection
* Built on LVGL
* Compatible with Cheap Yellow Display (CYD)
* Lightweight and easy to integrate
* No global variables required in user sketches
* Suitable for splash screens, demo applications, kiosks and screensavers

\---

# Requirements

## Hardware

* ESP32 Cheap Yellow Display (CYD)

## Software

* Arduino IDE
* ESP32 Arduino Core
* LVGL 9.x
* LVGL\_CYD

\---

# Installation

Copy the library folder into:

```text
Documents/Arduino/libraries/
```

Final structure:

```text
Arduino
└── libraries
    └── LVGL\_CYD\_Matrix
        ├── src
        ├── examples
        ├── LICENSE
        ├── README.md
        └── library.properties
```

Restart Arduino IDE after installation.

\---

# Quick Start

```cpp
#include <ScanLabsCYDMatrixEffect.h>

ScanLabsCYDMatrixEffect matrix;

void setup()
{
    matrix.begin(
        USB\\\_LEFT,
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
```

\---

# Basic Example

```cpp
#include <ScanLabsCYDMatrixEffect.h>

ScanLabsCYDMatrixEffect matrix;

uint32\\\_t lastMessageTime = 0;
uint8\\\_t messageIndex = 0;

const char\\\* messages\\\[] =
{
    "SCANLABS",
    "HELLO WORLD",
    "ESP32",
    "LVGL",
    "CYD"
};

void setup()
{
    Serial.begin(115200);

    matrix.begin(
        USB\\\_LEFT,
        320,
        240,
        40,
        24,
        8,
        25,
        2000
    );
}

void loop()
{
    matrix.loop();

    if (millis() - lastMessageTime > 10000)
    {
        lastMessageTime = millis();

        matrix.show(
            messages\\\[messageIndex],
            5000
        );

        messageIndex++;

        if (messageIndex >= 5)
        {
            messageIndex = 0;
        }
    }
}
```

\---

# API Reference

## Constructor

```cpp
ScanLabsCYDMatrixEffect matrix;
```

Creates a new Matrix effect object.

\---

## begin()

Initializes the display and Matrix engine.

```cpp
matrix.begin(
    rotation,
    screenWidth,
    screenHeight,
    matrixCols,
    matrixRows,
    columnSpacing,
    maxBrightness,
    bootScreenDurationMs
);
```

### Parameters

|Parameter|Description|
|-|-|
|rotation|Display orientation (e.g. USB\_LEFT)|
|screenWidth|Screen width in pixels|
|screenHeight|Screen height in pixels|
|matrixCols|Number of Matrix columns|
|matrixRows|Number of Matrix rows|
|columnSpacing|Horizontal spacing between columns|
|maxBrightness|Maximum trail brightness|
|bootScreenDurationMs|Boot screen duration in milliseconds|

### Example

```cpp
matrix.begin(
    USB\\\_LEFT,
    320,
    240,
    40,
    24,
    8,
    25,
    2000
);
```

\---

## show()

Displays a secret message inside the Matrix effect.

```cpp
matrix.show(
    "HELLO",
    5000
);
```

### Parameters

|Parameter|Description|
|-|-|
|text|Message to display|
|durationMs|Visibility duration|

### Example

```cpp
matrix.show(
    "SCANLABS",
    5000
);
```

\---

## loop()

Processes LVGL and Matrix animation.

Must be called continuously from Arduino loop.

```cpp
matrix.loop();
```

### Example

```cpp
void loop()
{
    matrix.loop();
}
```

\---

# Configuration Guide

## Typical 320x240 CYD Configuration

```cpp
matrix.begin(
    USB\_LEFT,
    320,
    240,
    40,
    24,
    8,
    25,
    2000
);
```

\---

## Slow Animation

```cpp
matrix.begin(
    USB\_LEFT,
    320,
    240,
    30,
    20,
    10,
    15,
    3000
);
```

\---

## Dense Animation

```cpp
matrix.begin(
    USB\_LEFT,
    320,
    240,
    50,
    28,
    6,
    30,
    2000
);
```

\---

# Internal Behavior

The library continuously generates:

* Random printable ASCII characters
* Falling character streams
* Fading trails
* Dynamic secret messages

The message is automatically injected when a suitable active region is detected.

No user interaction is required.

\---

# Performance Notes

Default settings:

```cpp
40 columns
24 rows
50ms refresh
```

provide smooth animation on ESP32-based CYD boards.

Increasing:

```cpp
matrixCols
matrixRows
```

will increase RAM usage and CPU load.

\---

# Attribution

This library may be used in personal, educational and commercial projects.

The following attribution must always be preserved:

```text
Powered by ScanLabsCYDMatrixEffect
Copyright (c) ScanLabs
```

Removal of original ownership attribution is not permitted.

\---

# License

Copyright (c) 2026 ScanLabs

Permission is granted to use, modify and distribute this software for commercial and non-commercial purposes provided that attribution to ScanLabs is retained in all copies and derivative works.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.

\---

# Author

ScanLabs

Created for ESP32 Cheap Yellow Display projects using LVGL.

