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

#include "ScanLabsCYDMatrixEffect.h"

ScanLabsCYDMatrixEffect*
ScanLabsCYDMatrixEffect::instance = nullptr;

/* -------------------------------------------------------------------------- */
/* Constructor                                                                 */
/* -------------------------------------------------------------------------- */

ScanLabsCYDMatrixEffect::ScanLabsCYDMatrixEffect()
{
    matrixCols       = nullptr;

    initScreen       = nullptr;
    matrixScreen     = nullptr;

    secretActive     = false;

    secretRow        = 0;
    secretCol        = 0;

    secretStartTime  = 0;
    secretDuration   = 0;
}

/* -------------------------------------------------------------------------- */
/* Initialize library                                                          */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::begin(
    lv_display_rotation_t rotation,
    uint16_t screenWidth,
    uint16_t screenHeight,
    uint8_t matrixColsCount,
    uint8_t matrixRowsCount,
    uint8_t columnSpacing,
    uint8_t maxBrightness,
    uint32_t bootScreenDurationMs)
{
    instance = this;

    this->screenWidth      = screenWidth;
    this->screenHeight     = screenHeight;

    this->matrixColsCount  = matrixColsCount;
    this->matrixRowsCount  = matrixRowsCount;

    this->columnSpacing    = columnSpacing;
    this->maxBrightness    = maxBrightness;

    this->bootDuration     = bootScreenDurationMs;

    LVGL_CYD::begin(rotation);

    createInitScreen();
    createMatrixScreen();

    lv_screen_load(initScreen);

    lv_timer_t* bootTimer =
        lv_timer_create(
            bootTimerCallback,
            bootDuration,
            nullptr);

    lv_timer_set_repeat_count(
        bootTimer,
        1);

    Serial.println(
        "ScanLabsCYDMatrixEffect started");
}

/* -------------------------------------------------------------------------- */
/* Main loop                                                                    */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::loop()
{
    lv_timer_handler();
    delay(5);
}

/* -------------------------------------------------------------------------- */
/* Display secret message                                                      */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::show(
    const char* text,
    uint32_t durationMs)
{
    if (text == nullptr)
    {
        return;
    }

    secretText = text;
    secretDuration = durationMs;

    secretActive = false;
}

/* -------------------------------------------------------------------------- */
/* Generate printable ASCII character                                           */
/* -------------------------------------------------------------------------- */

char ScanLabsCYDMatrixEffect::getRandomMatrixChar()
{
    return (char)(33 + (esp_random() % 94));
}

/* -------------------------------------------------------------------------- */
/* Create boot screen                                                           */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::createInitScreen()
{
    initScreen = lv_obj_create(NULL);

    lv_obj_set_size(
        initScreen,
        screenWidth,
        screenHeight);

    lv_obj_set_style_bg_color(
        initScreen,
        lv_color_black(),
        0);

    lv_obj_set_scrollbar_mode(
        initScreen,
        LV_SCROLLBAR_MODE_OFF);

    lv_obj_t* label =
        lv_label_create(
            initScreen);

    lv_label_set_text(
        label,
        "CYD SYSTEM BOOT...\nINITIALIZING MODULES");

    lv_obj_set_style_text_color(
        label,
        lv_color_hex(0x00FF33),
        0);

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_14,
        0);

    lv_obj_center(label);
}

/* -------------------------------------------------------------------------- */
/* Create matrix screen                                                         */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::createMatrixScreen()
{
    matrixScreen =
        lv_obj_create(NULL);

    lv_obj_set_size(
        matrixScreen,
        screenWidth,
        screenHeight);

    lv_obj_set_style_bg_color(
        matrixScreen,
        lv_color_black(),
        0);

    lv_obj_set_scrollbar_mode(
        matrixScreen,
        LV_SCROLLBAR_MODE_OFF);

    matrixCols =
        new MatrixCol[matrixColsCount];

    for (uint8_t i = 0;
         i < matrixColsCount;
         i++)
    {
        MatrixCol* col =
            &matrixCols[i];
            
        col->chars      = nullptr;
        col->brightness = nullptr;
        col->textBuffer = nullptr;

        col->chars =
            new char[matrixRowsCount];

        col->brightness =
            new uint8_t[matrixRowsCount];

        col->textBufferSize =
            (matrixRowsCount * 16) + 32;

        col->textBuffer =
            new char[col->textBufferSize];

        col->textBuffer[0] = '\0';


        //crash FIX
        col->label =
            lv_label_create(
                matrixScreen);

        lv_obj_set_style_text_font(
            col->label,
            &lv_font_montserrat_10,
            0);

        lv_obj_set_style_text_color(
            col->label,
            lv_color_hex(0x00FF33),
            0);

        lv_label_set_recolor(
            col->label,
            true);

        lv_obj_set_style_text_line_space(
            col->label,
            0,
            0);

        int randomOffset =
            (int)(esp_random() % 3) - 2;

        lv_obj_set_pos(
            col->label,
            (i * columnSpacing) + randomOffset,
            0);

        col->drop_row =
            -(esp_random() %
            matrixRowsCount);

        for (uint8_t r = 0;
             r < matrixRowsCount;
             r++)
        {
            col->chars[r] =
                getRandomMatrixChar();

            col->brightness[r] =
                (esp_random() % 100 < 10)
                ? (1 + esp_random() % 5)
                : 0;
        }

        lv_label_set_text(
            col->label,
            "");
    }
}

/* -------------------------------------------------------------------------- */
/* LVGL boot callback                                                           */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::bootTimerCallback(
    lv_timer_t* timer)
{
    if (instance)
    {
        instance->switchToMatrixScreen();
    }

    lv_timer_del(timer);
}

/* -------------------------------------------------------------------------- */
/* Switch to matrix screen                                                      */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::switchToMatrixScreen()
{
    lv_screen_load(
        matrixScreen);

    lv_timer_create(
        matrixTimerCallback,
        50,
        nullptr);

    Serial.println(
        "Matrix screen loaded");
}

/* -------------------------------------------------------------------------- */
/* LVGL matrix callback                                                        */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::matrixTimerCallback(
    lv_timer_t* timer)
{
    LV_UNUSED(timer);

    if (instance)
    {
        instance->processMatrix();
    }
}

/* -------------------------------------------------------------------------- */
/* Detect secret message position                                              */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::checkSecretTrigger()
{
    if (secretActive)
    {
        return;
    }

    uint32_t secretLen =
        secretText.length();

    if (secretLen == 0)
    {
        return;
    }

    if (secretLen >= matrixColsCount)
    {
        return;
    }

    uint8_t startRow =
        esp_random() % matrixRowsCount;

    for (uint8_t row = startRow;
         row < matrixRowsCount;
         row++)
    {
        int consecutive = 0;
        int startCol = 0;

        uint8_t firstCol =
            esp_random() % matrixColsCount;

        for (uint8_t col = firstCol;
             col < matrixColsCount;
             col++)
        {
            if (matrixCols[col].brightness[row] > 0)
            {
                if (consecutive == 0)
                {
                    startCol = col;
                }

                consecutive++;

                if (consecutive >= secretLen)
                {
                    secretActive = true;

                    secretRow = row;
                    secretCol = startCol;

                    secretStartTime =
                        millis();

                    Serial.printf(
                        "Secret row=%d col=%d\n",
                        row,
                        startCol);

                    return;
                }
            }
            else
            {
                consecutive = 0;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Update matrix animation                                                     */
/* -------------------------------------------------------------------------- */

void ScanLabsCYDMatrixEffect::processMatrix()
{
    checkSecretTrigger();

    float secretFade = 0.0f;

    uint32_t secretLen =
        secretText.length();

    if (secretActive)
    {
        uint32_t elapsed =
            millis() - secretStartTime;

        if (elapsed >= secretDuration)
        {
            secretActive = false;
        }
        else
        {
            secretFade =
                1.0f -
                ((float)elapsed /
                 (float)secretDuration);
        }
    }

    const int thresholdWhite =
        (maxBrightness * 90) / 100;

    const int thresholdLightGreen =
        (maxBrightness * 70) / 100;

    const int thresholdGreen =
        (maxBrightness * 50) / 100;

    const int thresholdMidGreen =
        (maxBrightness * 30) / 100;

    for (uint8_t colIndex = 0;
         colIndex < matrixColsCount;
         colIndex++)
    {
        MatrixCol* col =
            &matrixCols[colIndex];

        if(col->textBuffer == nullptr)
        {
            Serial.printf(
                "[ERROR] textBuffer NULL col=%d\n",
                colIndex);
            continue;
        }

        /* Fade trail */

        for (uint8_t row = 0;
             row < matrixRowsCount;
             row++)
        {
            if (col->brightness[row] > 0)
            {
                bool secretCell =
                    secretActive &&
                    (row == secretRow) &&
                    (colIndex >= secretCol) &&
                    (colIndex < (secretCol + secretLen));

                if (!secretCell)
                {
                    col->brightness[row]--;
                }
            }
        }

        /* Advance head */

        col->drop_row++;

        if (col->drop_row >
            (matrixRowsCount + 8))
        {
            col->drop_row =
                -(esp_random() % 10);
        }

        int head =
            col->drop_row;

        if ((head >= 0) &&
            (head < matrixRowsCount))
        {
            bool secretCell =
                secretActive &&
                (head == secretRow) &&
                (colIndex >= secretCol) &&
                (colIndex < (secretCol + secretLen));

            if (!secretCell)
            {
                col->chars[head] =
                    getRandomMatrixChar();

                col->brightness[head] =
                    maxBrightness;
            }
        }

        char* ptr =
            col->textBuffer;

        size_t remaining =
            col->textBufferSize;

        *ptr = 0;

        for (uint8_t row = 0;
             row < matrixRowsCount;
             row++)
        {
            int written = 0;

            uint8_t brightness =
                col->brightness[row];

            bool secretCell =
                secretActive &&
                (row == secretRow) &&
                (colIndex >= secretCol) &&
                (colIndex < (secretCol + secretLen));

            if (secretCell)
            {
                char secretChar =
                    secretText[
                        colIndex - secretCol];

                uint8_t intensity =
                    (uint8_t)
                    (255.0f * secretFade);

                if (intensity < 8)
                {
                    intensity = 8;
                }

                char color[16];

                snprintf(
                    color,
                    sizeof(color),
                    "#%02X%02X00",
                    intensity,
                    intensity);

                written =
                    snprintf(
                        ptr,
                        remaining,
                        "%s %c#\n",
                        color,
                        secretChar);
            }
            else if (
                brightness >= thresholdWhite)
            {
                written =
                    snprintf(
                        ptr,
                        remaining,
                        "#FFFFFF %c#\n",
                        col->chars[row]);
            }
            else if (
                brightness >= thresholdLightGreen)
            {
                written =
                    snprintf(
                        ptr,
                        remaining,
                        "#55FF55 %c#\n",
                        col->chars[row]);
            }
            else if (
                brightness >= thresholdGreen)
            {
                written =
                    snprintf(
                        ptr,
                        remaining,
                        "#00FF33 %c#\n",
                        col->chars[row]);
            }
            else if (
                brightness >= thresholdMidGreen)
            {
                written =
                    snprintf(
                        ptr,
                        remaining,
                        "#00AA22 %c#\n",
                        col->chars[row]);
            }
            else if (
                brightness > 0)
            {
                written =
                    snprintf(
                        ptr,
                        remaining,
                        "#003305 %c#\n",
                        col->chars[row]);
            }
            else
            {
                written =
                    snprintf(
                        ptr,
                        remaining,
                        "\n");
            }

            if ((written <= 0) ||
                (written >= remaining))
            {
                break;
            }

            ptr += written;
            remaining -= written;
        }

        *ptr = 0;

        lv_label_set_text(
            col->label,
            col->textBuffer);
    }
}