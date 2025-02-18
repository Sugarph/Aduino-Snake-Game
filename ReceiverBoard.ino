int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};
int buzzerPin = 9; 
int highestScore = 0;

int digits[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    int receivedNumber = Serial.parseInt();
    
    if (receivedNumber == 100) {  // Signal for start
      startBeep();
    } else if (receivedNumber == 200) {  // Signal for end
      endBeep();
      displayDigit(highestScore);  // Show highest score at the end
    } else if (receivedNumber >= 0 && receivedNumber <= 9) {
      displayDigit(receivedNumber);
      if (receivedNumber > highestScore) {
        highestScore = receivedNumber;  // Update highest score
      }
    }
  }
}

void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[num][i]);
  }
}

void startBeep() {
  digitalWrite(buzzerPin, HIGH);  
  delay(500);                      
  digitalWrite(buzzerPin, LOW);  
}

void endBeep() {
  digitalWrite(buzzerPin, HIGH);  
  delay(300);                      
  digitalWrite(buzzerPin, LOW);  
  delay(400);                     
  digitalWrite(buzzerPin, HIGH);  
  delay(300);                    
  digitalWrite(buzzerPin, LOW);   
}
