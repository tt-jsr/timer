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

int table[COL_2C3][ROW_4];

void setCol(int col)
{
    digitalWrite(SRCLR, LOW);
    digitalWrite(SRCLR, HIGH);

    int b = 1 << col;
    shiftOut(SER_DATA, SRCLK, MSBFIRST, b);
    
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
}


int getKey()
{
    for (int col = COL_1C1; col <= COL_2C3; ++col)
    {
        setCol(col);
        for(int row = ROW_1; row <= ROW_4; ++row)
        {
            if (HIGH == digitalRead(row))
                return table[col][row];
        }
    }
    return KEY_NONE;
}

void setupKeypad()
{
    memset(table, 0, sizeof(int)*COL_2C3*ROW_4);
    table[COL_2C1][ROW_1] = KEY_1;
    table[COL_2C2][ROW_1] = KEY_2;
    table[COL_1C3][ROW_1] = KEY_3;
    table[COL_2C1][ROW_2] = KEY_4;
    table[COL_2C2][ROW_2] = KEY_5;
    table[COL_2C3][ROW_2] = KEY_6;
    table[COL_2C1][ROW_3] = KEY_7;
    table[COL_1C2][ROW_3] = KEY_8;
    table[COL_1C3][ROW_3] = KEY_9;
    table[COL_1C2][ROW_4] = KEY_0;
    table[COL_1C1][ROW_4] = KEY_STAR;
    table[COL_1C2][ROW_4] = KEY_HASH;
    table[COL_1C4][ROW_4] = KEY_REDIAL;
}
