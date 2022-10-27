#include "config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool ready_to_vote = false;
bool vote_done = false;
int votes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
String cand_names[] = {CANDIDATE_1, CANDIDATE_2, CANDIDATE_3, CANDIDATE_4, CANDIDATE_5, CANDIDATE_6, CANDIDATE_7, CANDIDATE_8, CANDIDATE_9, CANDIDATE_10};

void setup() {
  for(char btn_pin = 0; btn_pin<10; btn_pin++) {
    pinMode(btn_pin, INPUT_PULLUP);
  }
  pinMode(RESULT_KEY, INPUT);
  pinMode(RESET_BTN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(READY_LED, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(READY_LED, LOW);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   ELECTRONIC   ");
  lcd.setCursor(0, 1);
  lcd.print(" VOTING MACHINE ");
  delay(2000);
  ready_to_vote = true;

  int isVoteExist = getVoteFromEEPROM(0);
  if (isVoteExist>=0) {
    for (char i = 0; i < 10; i++) {
      saveVoteToEEPROM(i, votes[i]);
    }
  } else {
    for(char i=0;i<10;i++) {
      int vote = getVoteFromEEPROM(i);
      votes[i] = vote;
    }
  }
}

void loop() {
  digitalWrite(READY_LED, ready_to_vote);
  if(digitalRead(RESULT_KEY)) {
    for(char pos=0; pos<10; pos+=2) {
      lcd.setCursor(0, 0);
      String line1 = cand_names[pos] + " : " + votes[pos];
      lcd.print(formatString(line1));
      lcd.setCursor(0, 1);
      String line2 = cand_names[pos+1] + " : " + votes[pos+1];
      lcd.print(formatString(line2));
      delay(RESULT_DELAY);
    }
  } else if(digitalRead(RESET_BTN)) {
    for(char i=0;i<10;i++) {
      votes[i] = 0;
      saveVoteToEEPROM(i, 0);
    }
    lcd.setCursor(0, 0);
    lcd.print(" RESULT CLEARED ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(2000);
  } else if(ready_to_vote) {
    lcd.setCursor(0, 0);
    lcd.print("    READY TO    ");
    lcd.setCursor(0, 1);
    lcd.print("      VOTE      ");

    if(!digitalRead(0)) {
      votingDone(0);
    } else if(!digitalRead(1)) {
      votingDone(1);
    } else if(!digitalRead(2)) {
      votingDone(2);
    } else if(!digitalRead(3)) {
      votingDone(3);
    } else if(!digitalRead(4)) {
      votingDone(4);
    } else if(!digitalRead(5)) {
      votingDone(5);
    } else if(!digitalRead(6)) {
      votingDone(6);
    } else if(!digitalRead(7)) {
      votingDone(7);
    } else if(!digitalRead(8)) {
      votingDone(8);
    } else if(!digitalRead(9)) {
      votingDone(9);
    }
  } else {
    if(vote_done) {
      lcd.setCursor(0, 0);
      lcd.print("     VOTING     ");
      lcd.setCursor(0, 1);
      lcd.print("   SUCCESSFUL   ");
      delay(SUCCESS_SHOW_TIME);
      vote_done = false;
      ready_to_vote = true;
      digitalWrite(BUZZER_PIN, LOW);
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

void votingDone(char pos) {
  digitalWrite(BUZZER_PIN, HIGH);
  votes[pos] = votes[pos]+1;
  vote_done = true;
  ready_to_vote = false;
  saveVoteToEEPROM(pos, votes[pos]);
}

void saveVoteToEEPROM(int pos, int vote) {
  int address = pos * 2;
  EEPROM.write(address, vote >> 8);
  EEPROM.write(address + 1, vote & 0xFF);  
}

int getVoteFromEEPROM(int pos) {
  int address = pos * 2;
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}