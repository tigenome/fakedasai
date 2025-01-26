#include <SPI.h>
#include <Wire.h>
#include <BitBang_I2C.h>
#include <OneBitDisplay.h>
#include <AnimatedGIF.h>
#include "a.h"

#define RESET_PIN     -1
#define SDA_PIN       -1
#define SCL_PIN       -1
#define OLED_ADDR     -1
#define MY_OLED       OLED_128x64
#define USE_HW_I2C    1
#define FLIP180       1
#define INVERT        0
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define NUMBEROFANIMATION 50
#define HISTORY_SIZE      5

const int ANIMATION_NO_DELAY_1 = 42;
const int ANIMATION_NO_DELAY_2 = 39;
const int ANIMATION_NO_DELAY_3 = 30;
const int ANIMATION_30 = 30;

OBDISP obd;
AnimatedGIF gif;
static uint8_t ucOLED[4096];
static uint8_t ucPalette[256];

uint8_t last_animation = 0;
int counter = 99;
int debugRandom = 0;

int recentAnimations[HISTORY_SIZE] = {0};
int historyIndex = 0;

void DrawPixel(int x, int y, uint8_t ucColor);
void GIFDraw(GIFDRAW* pDraw);
void playWrapper(uint8_t* gifinput, int size, bool slowDown = false);
void selectAnimation(int animationIndex);
bool isRecentAnimation(int animationIndex);

void setup() {
    Serial.begin(115200);
    int rc = obdI2CInit(&obd, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 800000L);
    obdFill(&obd, 0, 1);
    gif.begin(LITTLE_ENDIAN_PIXELS);
    delay(500);
    playWrapper((uint8_t*)_31, sizeof(_31));
    delay(4000);
}

void loop() {
    int animationIndex;
    do {
        if (debugRandom == 0) {
            animationIndex = random(1, NUMBEROFANIMATION + 1);
        } else {
            counter++;
            if (counter > NUMBEROFANIMATION) {
                counter = 1;
            }
            animationIndex = counter;
        }
    } while (isRecentAnimation(animationIndex));
    
    selectAnimation(animationIndex);
    
    if (animationIndex == ANIMATION_30) {
        playWrapper((uint8_t*)intro, sizeof(intro));
    }
    
    recentAnimations[historyIndex] = animationIndex;
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    
    if (animationIndex != ANIMATION_NO_DELAY_1 && 
        animationIndex != ANIMATION_NO_DELAY_2 && 
        animationIndex != ANIMATION_NO_DELAY_3) {
        delay(0);
    }
    
    for (int i = 0; i < 3; i++) {
        playWrapper((uint8_t*)_7, sizeof(_7));
        int delayTime = random(5000, 10000);
        delay(delayTime);
    }
    
    last_animation = animationIndex;
}

void selectAnimation(int animationIndex) {
    switch (animationIndex) {
        case 1:  playWrapper((uint8_t*)_1,  sizeof(_1));  break;
        case 2:  playWrapper((uint8_t*)_2,  sizeof(_2));  break;
        case 3:  playWrapper((uint8_t*)_3,  sizeof(_3));  break;
        case 4:  playWrapper((uint8_t*)_4,  sizeof(_4));  break;
        case 5:  playWrapper((uint8_t*)_5,  sizeof(_5));  break;
        case 6:  playWrapper((uint8_t*)_6,  sizeof(_6));  break;
        case 7:  playWrapper((uint8_t*)_8,  sizeof(_8));  break;
        case 8:  playWrapper((uint8_t*)_9,  sizeof(_9));  break;
        case 9:  playWrapper((uint8_t*)_10, sizeof(_10)); break;
        case 10: playWrapper((uint8_t*)_11, sizeof(_11)); break;
        case 11: playWrapper((uint8_t*)_12, sizeof(_12)); break;
        case 12: playWrapper((uint8_t*)_13, sizeof(_13)); break;
        case 13: playWrapper((uint8_t*)_14, sizeof(_14)); break;
        case 14: playWrapper((uint8_t*)_16, sizeof(_16)); break;
        case 15: playWrapper((uint8_t*)_18, sizeof(_18)); break;
        case 16: playWrapper((uint8_t*)_19, sizeof(_19)); break;
        case 17: playWrapper((uint8_t*)_21, sizeof(_21)); break;
        case 18: playWrapper((uint8_t*)_22, sizeof(_22)); break;
        case 19: playWrapper((uint8_t*)_23, sizeof(_23)); break;
        case 20: playWrapper((uint8_t*)_24, sizeof(_24)); break;
        case 21: playWrapper((uint8_t*)_25, sizeof(_25)); break;
        case 22: playWrapper((uint8_t*)_28, sizeof(_28)); break;
        case 23: playWrapper((uint8_t*)_29, sizeof(_29)); break;
        case 24: playWrapper((uint8_t*)_37, sizeof(_37)); break;
        case 25: playWrapper((uint8_t*)_32, sizeof(_32)); break;
        case 26: playWrapper((uint8_t*)_33, sizeof(_33)); break;
        case 27: playWrapper((uint8_t*)_34, sizeof(_34)); break;
        case 28: playWrapper((uint8_t*)_35, sizeof(_35)); break;
        case 29: playWrapper((uint8_t*)_36, sizeof(_36)); break;
        case 30: playWrapper((uint8_t*)_30, sizeof(_30)); break;
        case 32: playWrapper((uint8_t*)_44, sizeof(_44)); break;
        case 33: playWrapper((uint8_t*)_43, sizeof(_43)); break;
        case 34: playWrapper((uint8_t*)_45, sizeof(_45)); break;
        case 35: playWrapper((uint8_t*)_42, sizeof(_42)); break;
        case 36: playWrapper((uint8_t*)_17, sizeof(_17)); break;
        case 37: playWrapper((uint8_t*)_15, sizeof(_15)); break;
        case 38: playWrapper((uint8_t*)_20, sizeof(_20)); break;
        case 39: playWrapper((uint8_t*)_39, sizeof(_39)); break;
        case 40: playWrapper((uint8_t*)_40, sizeof(_40)); break;
        case 41: playWrapper((uint8_t*)_41, sizeof(_41)); break;
        default: break;
    }
}

bool isRecentAnimation(int animationIndex) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (recentAnimations[i] == animationIndex) {
            return true;
        }
    }
    return false;
}

void DrawPixel(int x, int y, uint8_t ucColor) {
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
    uint8_t ucMask = 1 << (y & 7);
    int index = x + ((y >> 3) * DISPLAY_WIDTH);
    if (ucColor) {
        ucOLED[index] |= ucMask;
    } else {
        ucOLED[index] &= ~ucMask;
    }
}

void GIFDraw(GIFDRAW* pDraw) {
    if (pDraw->y == 0) {
        for (int x = 0; x < 256; x++) {
            uint16_t usColor = pDraw->pPalette[x];
            int gray = (usColor & 0xf800) >> 8;
            gray += ((usColor & 0x7e0) >> 2);
            gray += ((usColor & 0x1f) << 3);
            ucPalette[x] = (gray > 800) ? 1 : 0;
        }
    }

    int y = pDraw->iY + pDraw->y;
    int iWidth = pDraw->iWidth > DISPLAY_WIDTH ? DISPLAY_WIDTH : pDraw->iWidth;

    uint8_t* s = pDraw->pPixels;

    if (pDraw->ucDisposalMethod == 2) {
        for (int x = 0; x < iWidth; x++) {
            if (s[x] == pDraw->ucTransparent) {
                s[x] = pDraw->ucBackground;
            }
        }
        pDraw->ucHasTransparency = 0;
    }

    if (pDraw->ucHasTransparency) {
        uint8_t ucTransparent = pDraw->ucTransparent;
        for (int x = 0; x < iWidth; x++) {
            uint8_t c = *s++;
            if (c != ucTransparent) {
                DrawPixel(pDraw->iX + x, y, ucPalette[c]);
            }
        }
    } else {
        s = pDraw->pPixels;
        for (int x = 0; x < pDraw->iWidth; x++) {
            DrawPixel(pDraw->iX + x, y, ucPalette[*s++]);
        }
    }

    if (pDraw->y == pDraw->iHeight - 1) {
        obdDumpBuffer(&obd, ucOLED);
    }
}

void playWrapper(uint8_t* gifinput, int size, bool slowDown) {
    if (gif.open(gifinput, size, GIFDraw)) {
        while (gif.playFrame(true, NULL)) {
            if (slowDown) {
                delay(10);
            }
        }
    }
    gif.close();
}