#ifndef SCANLABS_CYD_MATRIX_EFFECT_H
#define SCANLABS_CYD_MATRIX_EFFECT_H

/*
 * ScanLabsCYDMatrixEffect
 *
 * Copyright (c) 2026 ScanLabs
 *
 * Permission is granted to use, modify and distribute this library
 * provided that attribution to ScanLabs is retained.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 *
 * Author: ScanLabs
 */

#include <Arduino.h>
#include <new>
#include <LVGL_CYD.h>

#define SCANLABS_CYD_MATRIX_EFFECT_VERSION "1.0.0"

class ScanLabsCYDMatrixEffect
{
public:

    ScanLabsCYDMatrixEffect();
    //~ScanLabsCYDMatrixEffect();

    void begin(
        lv_display_rotation_t rotation,
        uint16_t screenWidth,
        uint16_t screenHeight,
        uint8_t matrixCols,
        uint8_t matrixRows,
        uint8_t columnSpacing,
        uint8_t maxBrightness,
        uint32_t bootScreenDurationMs);

    void loop();

    void show(
        const char* text,
        uint32_t durationMs);

private:

    struct MatrixCol
    {
        lv_obj_t* label;
        int drop_row;

        char* chars;
        uint8_t* brightness;

        char* textBuffer;
        size_t textBufferSize;
    };

    static ScanLabsCYDMatrixEffect* instance;

    MatrixCol* matrixCols;

    lv_obj_t* initScreen;
    lv_obj_t* matrixScreen;

    bool secretActive;

    int secretRow;
    int secretCol;

    uint32_t secretStartTime;

    String secretText;
    uint32_t secretDuration;

    uint16_t screenWidth;
    uint16_t screenHeight;

    uint8_t matrixColsCount;
    uint8_t matrixRowsCount;

    uint8_t columnSpacing;
    uint8_t maxBrightness;

    uint32_t bootDuration;

    static void bootTimerCallback(
        lv_timer_t* timer);

    static void matrixTimerCallback(
        lv_timer_t* timer);

    void switchToMatrixScreen();

    void processMatrix();

    void checkSecretTrigger();

    char getRandomMatrixChar();

    void createInitScreen();

    void createMatrixScreen();
};

#endif