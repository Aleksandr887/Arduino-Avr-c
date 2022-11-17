#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

Servo servo_9;
int angle_rotate = 180;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 14, 15, 16, 17 };
byte colPins[COLS] = { 2, 3, 4, 5 };

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char key, num[4];
int number;
int n, flag = 0;
int code = 1111;

boolean result = false;
 
void setup() 
{
  Serial.begin(9600); 
  lcd.begin(16, 2); 
  lcd.print("Electronic lock"); 
  lcd.setCursor(0, 1);   
  lcd.print("by KlimAss");
  delay(500); 
  LcdInputCode();
  servo_9.attach(7);
}

void updateCursor() {
  if (millis() / 250 % 2 == 0 ) {
    lcd.cursor();
  } else {
    lcd.noCursor();
  }
}

void loop() 
{
  updateCursor();
  key = kpd.getKey(); 
  if (key)
    DetectButtons();
}

void DetectButtons()
{ 
  if (strlen(num) < 4)
  {
    if ((key == '1') || (key == '2') || (key == '3') || (key == '4') ||
        (key == '5') || (key == '6') || (key == '7') || (key == '8') ||
        (key == '9') || (key == '0')) {
      
      num[n] = key;
      number = (number * 10) + (key - '0');
      n += 1;
      lcd.print(key);
    }
  }
  
  if (key == 'D')
  {
    if (number != code)
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Error code");
      lcd.setCursor(3, 1);
      lcd.print("try again!");
      delay(1000);
      LcdInputCode();
    } else {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Lock is open!");
        lcd.setCursor(0, 1);
        lcd.print("Press C to close");
        servo_9.write(angle_rotate);
        flag = 1;
    }
  }
  
  if (key == 'C' && flag == 1)
  {
    LcdInputCode();
    servo_9.write(angle_rotate - 90);
  }
}

void LcdInputCode()
{
  memset(num, 0, sizeof num);
  n = 0;
  number = 0;
  flag = 0;
  lcd.clear();
  lcd.cursor();
  lcd.print("Enter code: ");
  lcd.setCursor(0, 1);
}