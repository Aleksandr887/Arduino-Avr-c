/*			RULES
----------------------------
0-9 - play one note
# - enable edit mode
    press A-D for choose melody for edit
    * - finish editing
A-D - custom melodies
----------------------------
*/

#include <Keypad.h>

const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[rows] = {2, 3, 4, 5};
byte colPins[cols] = {14, 15, 16, 17};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

char keychars[] = "ABCD*#";

double notes[] = {
    261.63, 523.25, 1046.5,
    293.66, 587.32, 1174.6,
    329.63, 659.26, 1318.5,
    1668};

String music_create()
{
    String str = "";
    char c;

    do {
        c = keypad.waitForKey();
        if (!strchr(keychars, c)) {
            str += c;
            note_play(notes[int(c) - '0']);
        }
    } while (c != '*');

    return str;
}

void music_play(String melody)
{
    for (uint8_t i = 0; i < melody.length(); i++) {
        note_play(notes[int(melody[i]) - '0']);
    }
}

void note_play(double note)
{
    OCR1A = 16000000 / note;
    TCCR1A = 0b01000000;
    _delay_ms(150);
    TCCR1A = 0x00;
    _delay_ms(150);
}

void music_init()
{
    PORTB = 0xff;
    DDRB |= (1 << 1);
    TCCR1A = 0x00;
    TCCR1B = 0b00001001;
}

void setup()
{
    music_init();
    Serial.begin(9600);
}

String melodyA = "";
String melodyB = "";
String melodyC = "";
String melodyD = "";

void loop()
{
    char ch = keypad.waitForKey();
    Serial.print(ch);
    Serial.print('\n');

    if (ch == '#') {
        ch = keypad.waitForKey();
        switch (ch) {
        case 'A':
            melodyA = music_create();
            break;
        case 'B':
            melodyB = music_create();
            break;
        case 'C':
            melodyC = music_create();
            break;
        case 'D':
            melodyD = music_create();
            break;
        }
    } else {
        switch (ch) {
        case 'A':
            music_play(melodyA);
            break;
        case 'B':
            music_play(melodyB);
            break;
        case 'C':
            music_play(melodyC);
            break;
        case 'D':
            music_play(melodyD);
            break;
        default:
            if (!strchr(keychars, ch)) {
                note_play(notes[int(ch) - '0']);
            }
            break;
        }
    }
}