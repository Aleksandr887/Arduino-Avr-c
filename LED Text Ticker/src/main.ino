#include <Adafruit_NeoPixel.h>
#define PIN0 13
#define PIN1 12
#define PIN2 11
#define PIN3 10
#define PIN4 9
#define PIN5 8
#define PIN6 7

#define NUMPIXELS 12
#define NUMLINES 7

int len = NUMPIXELS;
int ch_n = NUMPIXELS - 1;
int speed_str = 25;

String c;
int flag = 0;
int count = 0;

int save_pos[5][NUMLINES];
int counter = 1;

byte char_pos[] = {
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
    46, 47, 83, 49, 50, 51, 52, 53, 54, 55, 56, 57};

Adafruit_NeoPixel pixels[NUMLINES] = {
    Adafruit_NeoPixel(NUMPIXELS, PIN0, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUMPIXELS, PIN3, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUMPIXELS, PIN4, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUMPIXELS, PIN5, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUMPIXELS, PIN6, NEO_GRB + NEO_KHZ800)};

byte mas[][5] = {
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, //__А (0xC0).
    {0x7F, 0x49, 0x49, 0x49, 0x33}, //__Б (0xC1).
    {0x7F, 0x49, 0x49, 0x49, 0x36}, //__В (0xC2).
    {0x7F, 0x01, 0x01, 0x01, 0x03}, //__Г (0xC3).
    {0xE0, 0x51, 0x4F, 0x41, 0xFF}, //__Д (0xC4).
    {0x7F, 0x49, 0x49, 0x49, 0x41}, //__Е (0xC5).
    {0x77, 0x08, 0x7F, 0x08, 0x77}, //__Ж (0xC6).
    {0x41, 0x49, 0x49, 0x49, 0x36}, //__З (0xC7).
    {0x7F, 0x10, 0x08, 0x04, 0x7F}, //__И (0xC8).
    {0x7C, 0x21, 0x12, 0x09, 0x7C}, //__Й (0xC9).
    {0x7F, 0x08, 0x14, 0x22, 0x41}, //__К (0xCA).
    {0x20, 0x41, 0x3F, 0x01, 0x7F}, //__Л (0xCB).
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, //__М (0xCC).
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, //__Н (0xCD).
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, //__О (0xCE).
    {0x7F, 0x01, 0x01, 0x01, 0x7F}, //__П (0xCF).
    {0x7F, 0x09, 0x09, 0x09, 0x06}, //__Р (0xD0).
    {0x3E, 0x41, 0x41, 0x41, 0x22}, //__С (0xD1).
    {0x01, 0x01, 0x7F, 0x01, 0x01}, //__Т (0xD2).
    {0x47, 0x28, 0x10, 0x08, 0x07}, //__У (0xD3).
    {0x1C, 0x22, 0x7F, 0x22, 0x1C}, //__Ф (0xD4).
    {0x63, 0x14, 0x08, 0x14, 0x63}, //__Х (0xD5).
    {0x7F, 0x40, 0x40, 0x40, 0xFF}, //__Ц (0xD6).
    {0x07, 0x08, 0x08, 0x08, 0x7F}, //__Ч (0xD7).
    {0x7F, 0x40, 0x7F, 0x40, 0x7F}, //__Ш (0xD8).
    {0x7F, 0x40, 0x7F, 0x40, 0xFF}, //__Щ (0xD9).
    {0x01, 0x7F, 0x48, 0x48, 0x30}, //__Ъ (0xDA).
    {0x7F, 0x48, 0x30, 0x00, 0x7F}, //__Ы (0xDB).
    {0x00, 0x7F, 0x48, 0x48, 0x30}, //__Ь (0xDC).
    {0x22, 0x41, 0x49, 0x49, 0x3E}, //__Э (0xDD).
    {0x7F, 0x08, 0x3E, 0x41, 0x3E}, //__Ю (0xDE).
    {0x46, 0x29, 0x19, 0x09, 0x7F}, //__Я (0xDF).
    {0x00, 0x00, 0x00, 0x00, 0x00}, //   0x20  32

    {0x00, 0x42, 0x7f, 0x40, 0x00}, // 1 0x31 49
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 0x32 50
    {0x21, 0x41, 0x45, 0x4b, 0x31}, // 3 0x33 51
    {0x18, 0x14, 0x12, 0x7f, 0x10}, // 4 0x34 52
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 0x35 53
    {0x3c, 0x4a, 0x49, 0x49, 0x30}, // 6 0x36 54
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 0x37 55
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 0x38 56
    {0x06, 0x49, 0x49, 0x29, 0x1e}, // 9 0x39 57

};

void setup()
{
    for (int j = 0; j < NUMLINES; j++) {
        pixels[j].begin();
    }
    Serial.begin(9600);
}

void loop()
{
    int i = 0;
    int r_m = 0;
    int n = 0;

    if (Serial.available() > 0) {
        c = Serial.readString();
        for (int j = 0; j < c.length(); j++) {
            if (c[j] == ' ')
                c[j] = 'S';
        }
        flag = 1;
    } else {
        if (flag != 0) {
            if (ch_n != -(6 * c.length())) {
                count = 0;
                for (int j = 0; j < NUMLINES; j++) {
                    pixels[j].clear();
                }

                while (i < c.length()) {
                    int m = 0;
                    while (m < sizeof(char_pos)) {
                        if ((byte)c[i] == char_pos[m]) {
                            print_char(m, ch_n + r_m);
                            if ((byte)c[i] == 83) {
                                r_m = r_m + 2;
                            } else {
                                r_m = r_m + 6;
                            }
                            count += 1;
                            break;
                        }
                        m = m + 1;
                    }
                    i = i + 1;
                }

                ch_n = ch_n - 1;
                delay(speed_str);

                for (int j = 0; j < NUMLINES; j++) {
                    pixels[j].show();
                }
            } else {
                flag = 0;
                ch_n = NUMPIXELS - 1;
            }
        }
    }
}

void print_char(byte code, byte pos)
{
    int p;
    int for_i = 5;

    if (pos == 11) {
        for_i = 1;
    } else if (pos == 10) {
        for_i = 2;
    } else if (pos == 9) {
        for_i = 3;
    } else if (pos == 8) {
        for_i = 4;
    }

    if (pos < 12) {
        for (int x = 0; x < for_i; x++) {
            for (int y = 0; y < NUMLINES; y++) {
                byte bit = bitRead(mas[code][x], y);
                if (bit) {
                    p = pos + x;
                    pixels[y].setPixelColor(p, pixels[y].Color(255, 0, 0));
                }

                if (pos == 0) {
                    if (bit) {
                        save_pos[x][y] = p;
                    } else {
                        save_pos[x][y] = NULL;
                    }
                    counter = 1;
                }
            }
        }
    }

    if (pos > 250) {
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < NUMLINES; y++) {
                if (save_pos[x][y] != NULL) {
                    p = save_pos[x][y] - counter;
                    pixels[y].setPixelColor(p, pixels[y].Color(255, 0, 0));
                }
            }
        }
        counter += 1;
    }
}