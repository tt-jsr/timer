#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include "timer.h"

// G2C1   G2R1      1
// G2C2   G2R1      2
// G1C3   G2R1      3
// G2C1   G2R2      4
// G2C2   G2R2      5
// G2C3   G2R2      6
// G2C1   G2R3      7
// G1C2   G2R3      8
// G1C3   G2R3      9digi
// G1C2   G1R4      0    error: dup with # 
// G1C1   G1R4      *
// G1C2   G1R4      #    error: dup with 0
// G1C4   G1R4     Redial and Pause
// unknown           Flash

namespace keypad_ns
{
    int scanMap[COL_2C3][ROW_4];
    int keymap[15];

    void setCol(int col)
    {
        digitalWrite(SRCLR, LOW);
        digitalWrite(SRCLR, HIGH);

        int b = 1 << col;
        shiftOut(SER_DATA, SRCLK, MSBFIRST, b);
        
        digitalWrite(RCLK, HIGH);
        digitalWrite(RCLK, LOW);
    }

    char keycode2char(int k)
    {
        if (k < 0 || k > KEY_REDIAL)
            return keymap[KEY_NONE];
        return keymap[k];
    }

    int getKey()
    {
        for (int col = COL_1C1; col <= COL_2C3; ++col)
        {
            setCol(col);
            for(int row = ROW_1; row <= ROW_4; ++row)
            {
                if (HIGH == digitalRead(row))
                    return scanMap[col][row];
            }
        }
        return KEY_NONE;
    }

    void setup()
    {
        memset(scanMap, 0, sizeof(int)*COL_2C3*ROW_4);
        scanMap[COL_2C1][ROW_1] = KEY_1;
        scanMap[COL_2C2][ROW_1] = KEY_2;
        scanMap[COL_1C3][ROW_1] = KEY_3;
        scanMap[COL_2C1][ROW_2] = KEY_4;
        scanMap[COL_2C2][ROW_2] = KEY_5;
        scanMap[COL_2C3][ROW_2] = KEY_6;
        scanMap[COL_2C1][ROW_3] = KEY_7;
        scanMap[COL_1C2][ROW_3] = KEY_8;
        scanMap[COL_1C3][ROW_3] = KEY_9;
        scanMap[COL_1C2][ROW_4] = KEY_0;
        scanMap[COL_1C1][ROW_4] = KEY_STAR;
        scanMap[COL_1C2][ROW_4] = KEY_HASH;
        scanMap[COL_1C4][ROW_4] = KEY_REDIAL;

        keymap[KEY_NONE] = 'N';
        keymap[KEY_0] = '0';
        keymap[KEY_1] = '1';
        keymap[KEY_2] = '2';
        keymap[KEY_3] = '3';
        keymap[KEY_4] = '4';
        keymap[KEY_5] = '5';
        keymap[KEY_6] = '6';
        keymap[KEY_7] = '7';
        keymap[KEY_8] = '8';
        keymap[KEY_9] = '9';
        keymap[KEY_HASH] = '#';
        keymap[KEY_STAR] = '*';
        keymap[KEY_REDIAL] = 'R';
    }
}
