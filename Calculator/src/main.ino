#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {14, 15, 16, 17};
byte colPins[COLS] = {2, 3, 4, 5};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

double Num1 = 0;
double Num2 = -1;
double Number = 0;
char key, action;
char numbers[] = "0123456789";
char symbols[] = "ABCD";
boolean result = false;
boolean flag = false;

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.print("Calculator by");
    lcd.setCursor(0, 1);
    lcd.print("CodeRatny");
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("0");
}

void loop()
{
    key = kpd.getKey();
    if (key != NO_KEY) {
        DetectButtons();
    }
}

void DetectButtons()
{
    if (strchr(numbers, key) != NULL) {
        PrintNumber(key);
    }
    if (strchr(symbols, key) != NULL) {
        PrintSymbol(key);
    }

    if (key == '*') {
        ResetLcd();
        lcd.setCursor(0, 1);
        lcd.print(0);
    }

    if (key == '#') {
        if (Num1 == 0 && Num2 == -1 && action != '-') {
            return;
        }

        if (result == false) {
            Num2 = Number;
        }

        if (Num1 != 0 && Num2 == -1) {
            Num2 = Num1;
        }

        lcd.clear();
        lcd.setCursor(0, 0);

        if ((Num1 - (int)Num1) == 0.0) {
            lcd.print(int(Num1));
        } else {
            lcd.print(Num1);
        }

        lcd.print(" ");
        lcd.print(action);
        lcd.print(" ");

        if ((Num2 - (int)Num2) == 0.0) {
            lcd.print(int(Num2));
        } else {
            lcd.print(Num2);
        }

        lcd.print(" = ");

        if (action == '+') {
            Number = Num1 + Num2;
        } else if (action == '-') {
            Number = Num1 - Num2;
        } else if (action == '*') {
            Number = Num1 * Num2;
        } else if (action == '/') {
            Number = Num1 / Num2;
        }

        lcd.setCursor(0, 1);
        if ((Number - (int)Number) == 0.0) {
            lcd.print(int(Number));
        } else {
            lcd.print(Number);
        }
        Num1 = Number;
        result = true;
        flag = false;
    }
}

void ClearLine(int Line)
{
    lcd.setCursor(0, Line);
    for (int n = 0; n < 16; n++) {
        lcd.print(" ");
    }
}

void PrintNumber(char key)
{
    if (result == true && flag == false) {
        ResetLcd();
    }

    if (Num1 != 0) {
        ClearLine(1);
    }

    lcd.print(" ");
    lcd.setCursor(0, 1);
    Number = (Number * 10) + (key - '0');
    lcd.print(int(Number));

    if (result == true && flag == true) {
        Num2 = Number;
    }
}

void PrintSymbol(char key)
{
    flag = true;
    switch (key)
    {
    case 'A':
        action = '+';
        break;
    case 'B':
        action = '-';
        break;
    case 'C':
        action = '*';
        break;
    case 'D':
        action = '/';
        break;
    }

    char result[] = "   ";
    result[1] = action;

    if (Num1 == 0) {
        Num1 = Number;
        lcd.setCursor(0, 0);

        if ((Num1 - (int)Num1) == 0.0) {
            lcd.print(int(Num1));
        } else {
            lcd.print(Num1);
        }

        lcd.print(result);
    } else {
        ClearLine(0);
        lcd.setCursor(0, 0);
        if ((Num1 - (int)Num1) == 0.0) {
            lcd.print(int(Num1));
        } else {
            lcd.print(Num1);
        }
        lcd.print(result);
    }
    Number = 0;
}

void ResetLcd()
{
    lcd.clear();
    Num1 = 0;
    Num2 = 0;
    Number = 0;
    result = false;
}