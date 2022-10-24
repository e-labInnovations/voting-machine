#include "config.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool ready_to_vote = false;
bool vote_done = false;
bool show_result = true;
int votes[] = {100, 90, 55, 0, 0, 44, 0, 0, 0, 0};
String cand_names[] = {CANDIDATE_1, CANDIDATE_2, CANDIDATE_3, CANDIDATE_4, CANDIDATE_5, CANDIDATE_6, CANDIDATE_7, CANDIDATE_8, CANDIDATE_9, CANDIDATE_10};

void setup() {
  for(char btn_pin = 0; btn_pin<10; btn_pin++) {
    pinMode(btn_pin, INPUT_PULLUP);
  }
  pinMode(13, OUTPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   ELECTRONIC   ");
  lcd.setCursor(0, 1);
  lcd.print(" VOTING MACHINE ");
  delay(2000);
  ready_to_vote = true;
}

void loop() {
  if(show_result) {
    for(char pos=0; pos<10; pos+=2) {
      lcd.setCursor(0, 0);
      String line1 = cand_names[pos] + " : " + votes[pos];
      lcd.print(formatString(line1));
      lcd.setCursor(0, 1);
      String line2 = cand_names[pos+1] + " : " + votes[pos+1];
      lcd.print(formatString(line2));
      delay(1000);
    }
  } else if(ready_to_vote) {
    lcd.setCursor(0, 0);
    lcd.print("    READY TO    ");
    lcd.setCursor(0, 1);
    lcd.print("      VOTE      ");
    
    if (digitalRead(0) &&
    digitalRead(1) &&
    digitalRead(2) &&
    digitalRead(3) &&
    digitalRead(4) &&
    digitalRead(5) &&
    digitalRead(6) &&
    digitalRead(7) &&
    digitalRead(8) &&
    digitalRead(9)) {
      digitalWrite(13, LOW);
    } else {
      digitalWrite(13, HIGH);
    }
  } else {
    if(vote_done) {
      lcd.setCursor(0, 0);
      lcd.print("     VOTING     ");
      lcd.setCursor(0, 1);
      lcd.print("   SUCCESSFUL   ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print(" VOTING MACHINE ");
      lcd.setCursor(0, 1);
      lcd.print("  IS NOT READY  ");
    }
  }
}

String formatString(String text) {
  char zeros_count = 16 - text.length();
  for(char count=0; count<zeros_count; count++) {
    text += ' ';
  }
  return text;
}
