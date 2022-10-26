#include "config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool ready_to_vote = false;
bool vote_done = false;
int votes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
String cand_names[] = {CANDIDATE_1, CANDIDATE_2, CANDIDATE_3, CANDIDATE_4, CANDIDATE_5, CANDIDATE_6, CANDIDATE_7, CANDIDATE_8, CANDIDATE_9, CANDIDATE_10};
File fileObj;

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

  if (!SD.begin(10)) {
    lcd.setCursor(0, 0);
    lcd.print(" SD CARD ERROR  ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    while (1);
  }

  if (SD.exists("tempRes.txt")) {
    fileObj = SD.open("tempRes.txt");
    if (fileObj) {
      String finalString = "";
      while (fileObj.available()) {
        finalString += (char)fileObj.read();
      }
      
      char oldIndex = 0;
      for(char i=0;i<10;i++) {
        char index = finalString.indexOf(';', oldIndex+1);
        int voteValue = finalString.substring(oldIndex + (i?1:0), index).toInt();
        oldIndex = index;
        votes[i] = voteValue;
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print(" ERROR OPENING  ");
      lcd.setCursor(0, 1);
      lcd.print("  tempRes.txt   ");
    }
    fileObj.close();
  } else {
    fileObj = SD.open("tempRes.txt", FILE_WRITE);
    for (int i = 0; i < 10; i++) {
      fileObj.print(votes[i]);
      fileObj.print(";");
    }
    fileObj.close();
  }

  if (!SD.exists("result.txt")) {
    fileObj = SD.open("result.txt", FILE_WRITE);
    fileObj.close();
  }
}

void loop() {
  digitalWrite(READY_LED, ready_to_vote);
  if(digitalRead(RESULT_KEY)) {
    saveResultFile();
    for(char pos=0; pos<10; pos+=2) {
      lcd.setCursor(0, 0);
      String line1 = cand_names[pos] + " : " + votes[pos];
      lcd.print(formatString(line1));
      lcd.setCursor(0, 1);
      String line2 = cand_names[pos+1] + " : " + votes[pos+1];
      lcd.print(formatString(line2));
      delay(RESULT_DELAY);
    }
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
  } else if(digitalRead(RESET_BTN)) {
    for(char i=0;i<10;i++) {
      votes[i] = 0;
    }
    saveTempFile();
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
  saveTempFile();
}

void saveTempFile() {
  SD.remove("tempRes.txt");
  fileObj = SD.open("tempRes.txt", FILE_WRITE);
  if (fileObj) {
    for (int i = 0; i < 10; i++) {
      fileObj.print(votes[i]);
      fileObj.print(";");
    }
    fileObj.close();
  } else {
    lcd.setCursor(0, 0);
    lcd.print(" SD CARD ERROR  ");
    lcd.setCursor(0, 1);
    lcd.print("  tempRes.txt   ");
  }
}

void saveResultFile() {
  SD.remove("result.txt");
  fileObj = SD.open("result.txt", FILE_WRITE);
  if (fileObj) {
    for (int i = 0; i < 10; i++) {
      fileObj.print(cand_names[i]);
      fileObj.print(" : ");
      fileObj.println(votes[i]);
    }
    fileObj.close();
  } else {
    lcd.setCursor(0, 0);
    lcd.print(" SD CARD ERROR  ");
    lcd.setCursor(0, 1);
    lcd.print("   result.txt   ");
  }      
}