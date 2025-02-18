int R[] = {2, 7, A5, 5, 13, A4, 12, A2};  // Row pins
int C[] = {6, 11, 10, 3, A3, 4, 8, 9};    // Column pins

int pixelState[8][8] = {0}; 

int snakeLength = 2;
int snakeX[64];
int snakeY[64]; 

int direction = 1;
long lastMoveTime = 0;
int moveInterval = 325;
int buttonLeftState = 0;
int buttonRightState = 0; 
long lastButtonTime = 0;  
int buttonDelay = 325;  
int foodX;
int foodY;
int score = 0;
int gameStartTime = 0;
bool gameRunning;

void setup()  
{  
  Serial.begin(9600);  

  // Set row and column pins as outputs
  for (int i = 0; i < 8; i++) {  
    pinMode(R[i], OUTPUT);  
    pinMode(C[i], OUTPUT);  
  }


  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

  // Clear the display
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      pixelState[row][col] = 0;  
    }
  }
  randomSeed(millis());  
  resetGame();
}


void loop()  
{
  if (!gameRunning) {
    if (digitalRead(A0) == LOW || digitalRead(A1) == LOW) {
      startGame();
    }
  } else {
    if (millis() - gameStartTime > 30000) {  //Stop game after 30 sec
      endGame();
      delay(5000);
    }

    if (millis() - lastButtonTime > buttonDelay) {
      buttonLeftState = digitalRead(A0); 
      buttonRightState = digitalRead(A1);

      if (buttonLeftState == LOW) {  
        direction = (direction - 1 + 4) % 4;  
        lastButtonTime = millis();  
      }

      if (buttonRightState == LOW) {  
        direction = (direction + 1) % 4;  
        lastButtonTime = millis();  
      }
    }

    if (millis() - lastMoveTime > moveInterval) {
      lastMoveTime = millis();
      moveSnake();
    }

    Display(); 
    Serial.println(score);
  }
}

void startGame() {
  gameStartTime = 0;
  gameRunning = true;
  gameStartTime = millis();
  Serial.println(100);  
  resetGame();
}

void endGame() {
  gameRunning = false;
  Serial.println(200);  
}

void moveSnake() {
  pixelState[snakeY[snakeLength - 1]][snakeX[snakeLength - 1]] = 0;  

  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  if (direction == 0) snakeY[0]--; // Up
  if (direction == 1) snakeX[0]++; // Right
  if (direction == 2) snakeY[0]++; // Down
  if (direction == 3) snakeX[0]--; // Left

  if (snakeX[0] < 0 || snakeX[0] >= 8 || snakeY[0] < 0 || snakeY[0] >= 8) {
    resetGame();
  }

  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    score++;
    snakeLength++;
    generateFood(); 
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      resetGame();
    }
  }

  pixelState[snakeY[0]][snakeX[0]] = 1;
}

void resetGame() {
  
  // Clear the display
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      pixelState[row][col] = 0;
    }
  }

  snakeLength = 2;
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = 3 - i; 
    snakeY[i] = 3;     
  }
  generateFood();

  direction = 2;
  score = 0;
  delay(500);
}

void generateFood() {
  bool validFoodPosition = false;
  while (!validFoodPosition) {
    foodX = random(0, 8);
    foodY = random(0, 8);
    validFoodPosition = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        validFoodPosition = false;
        break;
      }
    }
  }

  pixelState[foodY][foodX] = 2; 
}

void Display()  
{  
  for (int i = 0; i < 8; i++) {
    digitalWrite(R[i], LOW);
  }

  for (int col = 0; col < 8; col++) {
    digitalWrite(C[col], LOW);

    for (int row = 0; row < 8; row++) {
      if (pixelState[row][col] == 1) {
        digitalWrite(R[row], HIGH); 
      } else if (pixelState[row][col] == 2) { 
        digitalWrite(R[row], HIGH); 
      } else {
        digitalWrite(R[row], LOW);  
      }
    }
    //delayMicroseconds(1000);
    digitalWrite(C[col], HIGH);

    for (int i = 0; i < 8; i++) {
      digitalWrite(R[i], LOW);
    }
  }
}

void printPixelState() {
  Serial.println("Matrix State:");
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      Serial.print(pixelState[row][col]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}
