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
    bool  keyDown(false);
    KeyCode scanMap[KS_COL_2C3+1][KS_ROW_4+1];
    int keymap[15];

    void setCol(int col)
    {
        digitalWrite(PIN_SRCLR, LOW);
        digitalWrite(PIN_SRCLR, HIGH);

        digitalWrite(PIN_RCLK, LOW);
        shiftOut(PIN_SER_DATA, PIN_SRCLK, MSBFIRST, 1<< col);
        
        digitalWrite(PIN_RCLK, HIGH);
        digitalWrite(PIN_RCLK, LOW);
    }

    char keycode2char(KeyCode k)
    {
        if (k < 0 || k > KEY_REDIAL)
            return keymap[KEY_NONE];
        return keymap[k];
    }

    KeyCode getKeyCode()
    {
        for (int col = KS_COL_1C1; col <= KS_COL_2C3; ++col)
        {
            setCol(col);
            for(int row = KS_ROW_1; row <= KS_ROW_4; ++row)
            {
                int pin = 0;
                switch (row)
                {
                case KS_ROW_1:
                    pin = PIN_ROW_1;
                    break;
                case KS_ROW_2:
                    pin = PIN_ROW_2;
                    break;
                case KS_ROW_3:
                    pin = PIN_ROW_3;
                    break;
                case KS_ROW_4:
                    pin = PIN_ROW_4;
                    break;
                }
                if (HIGH == digitalRead(pin))
                {
                    if (keyDown == true)
                        return KEY_NONE;  // don't repeat
                    keyDown = true;
                    //char buf[64];
                    //sprintf(buf, "Pin: %d was high, row: %d col:%d", pin, row, col);
                    //Serial.println(buf);
                    return scanMap[col][row];
                }
            }
        }
        keyDown = false;
        return KEY_NONE;
    }

    char getKeyPress()
    {
        KeyCode k = getKeyCode();
        if (k == KEY_NONE)
            return 0;
        return keycode2char(k);
    }

    char getKey()
    {
        KeyCode k = KEY_NONE;
        while (k == KEY_NONE)
        {
            k = getKeyCode();
        }
        return keycode2char(k);
    }

    void setup()
    {
        memset(scanMap, 0, sizeof(int)*KS_COL_2C3*KS_ROW_4);
        scanMap[KS_COL_2C1][KS_ROW_1] = KEY_1;
        scanMap[KS_COL_2C2][KS_ROW_1] = KEY_2;
        scanMap[KS_COL_1C3][KS_ROW_1] = KEY_3;
        scanMap[KS_COL_2C1][KS_ROW_2] = KEY_4;
        scanMap[KS_COL_2C2][KS_ROW_2] = KEY_5;
        scanMap[KS_COL_2C3][KS_ROW_2] = KEY_6;
        scanMap[KS_COL_2C1][KS_ROW_3] = KEY_7;
        scanMap[KS_COL_1C2][KS_ROW_3] = KEY_8;
        scanMap[KS_COL_1C3][KS_ROW_3] = KEY_9;
        scanMap[KS_COL_1C3][KS_ROW_4] = KEY_HASH;
        scanMap[KS_COL_1C1][KS_ROW_4] = KEY_STAR;
        scanMap[KS_COL_1C2][KS_ROW_4] = KEY_0;
        scanMap[KS_COL_1C4][KS_ROW_4] = KEY_REDIAL;

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

        digitalWrite(PIN_SRCLR, LOW);
    }
}
