#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// defining all colours
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define VIOLET 0x5
#define TEAL 0x6
#define WHITE 0x7

// defining the colours for the selected buttons
#define LEFT 0x2
#define RIGHT 0x3
#define UP 0x4
#define DOWN 0x5

// the program has two states that adjust the modes
#define STORY 1
#define PRACTICE 2

// the program has two states that 
// adjust sequence length and one when arrow nunber
#define SEQUENCE 3
#define ARROWS 4

uint8_t mode = STORY;
uint8_t state = SEQUENCE;

char buffer[12];

// sequence starts from 1, max is 10
// the least possible number of arrows that will be shown is 2, max is 4
int sequence = 1, arrowsNum = 2;

String arrows[4] = {"LEFT", "RIGHT", "UP", "DOWN"}; // the commands (also referred to as arrows)
String userInput[10]; // array to store user input (buttons clicked)

int colour; // to be used for determining lcd color according to button
int arrowIndex; // to be used when generating game commands
String bton; // to be used when displaying the button clicked on the lcd screen
String randomArrow; // to be used when filling the array used for the game


void display(int, int);
void storyMode(int, String[]);

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.setBacklight(WHITE);
  loadingScreen();
  menu();
  state = SEQUENCE;
  mode = STORY;
}

// function containing the message shown
// when the user passes the level
void levelPassedMessage() {
  lcd.setBacklight(TEAL);
  lcd.print("Success!");
  delay(200);
  lcd.setCursor(0, 1);
  lcd.print("Level Passed!");
  delay(1000);
  lcd.clear();
  lcd.setBacklight(WHITE);

}

// function containing the message shown
// when the user makes a wrong move
void wrongMoveMessage() {
  lcd.clear();
  lcd.setBacklight(RED);
  lcd.print("Wrong move!");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Game Over!");
  delay(2000);
  lcd.clear();
  lcd.setBacklight(WHITE);

}

// function containing the message shown
// when the user's time is up
void timeIsUp() {
  lcd.clear();
  lcd.setBacklight(RED);
  lcd.print("Time is up!");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Game Over!");
  delay(2000);
  lcd.clear();
  lcd.setBacklight(WHITE);
}

// shows basic loading screen text
// when the arduino is booted
void loadingScreen() {

  lcd.print("Welcome to the Memory Game!"); //27
  delay(1000);

  for (int place = 0; place < 15; place++) {
    // scroll the display tex
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }

  delay(1000);
  lcd.clear();
  lcd.print("Entering main menu...");
  delay(600);

  for (int place = 0; place < 21; place++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }

  delay(1000);
  lcd.clear();

}

// function to be called when practice mode is chosen
// generates the specified sequence length
// of the specified number of arrows
void practiceMode() {
  lcd.clear();
  lcd.print("Starting game...");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Good luck!");
  delay(2000);
  lcd.clear();



  String arrows[4] = {"LEFT", "RIGHT", "UP", "DOWN"}; // the commands (also referred to as arrows)
  String gameArrows[arrowsNum]; // array to store the commands
  String sequenceOfArrows[sequence]; // array to store the sequence of commands for the game

  if (arrowsNum == 4) {
    for (int i = 0; i < 4; i++) {
      gameArrows[i] = arrows[i]; // all arrows are chosen to play with
    }
  }

  else {
    for (int i = 0; i < arrowsNum;) {
      arrowIndex = rand() % (4); // generates a random number to be associated with a character
      String newArrow = arrows[arrowIndex];
      // uses the random number as an index to
      // which arrow to take from the arrows array

      int x = 1;

      while (x == 1) {
        if (newArrow != gameArrows[i - 1]) {
          //checks whether the new arrow already exists in the array
          //if not it adds it to the gameArrows array
          gameArrows[i] = newArrow;
          i++;
          //only increment i by 1 if there is a new (command) arrow added
          //otherwise continue to loop until a new arrow is chosen
        }

        x = 0; // to exit the while loop
      }

      if (i == arrowsNum) {
        break;

      }
    }
  }


  for (int i = 0; i < sequence; i++) {

    arrowIndex =  rand() % (arrowsNum);
    // generates the index of a random arrow of the ones
    // that are chosen for the game
    randomArrow = gameArrows[arrowIndex];
    sequenceOfArrows[i] = randomArrow; // adds the arrow to the array used for the game

    // set the lcd colour according to button clicked
    if (randomArrow == "LEFT") {
      colour = 2;
    }
    else if (randomArrow == "RIGHT") {
      colour = 3;
    }
    else if (randomArrow == "UP") {
      colour = 4;
    }
    else if (randomArrow == "DOWN") {
      colour = 5;
    }

    lcd.setBacklight(colour);
    lcd.print(sequenceOfArrows[i]);
    delay(1000); // shows the command to the user for a second
    lcd.clear();
    lcd.setBacklight(0);
    delay(100);
  }



  lcd.setBacklight(WHITE);
  lcd.print("Your turn!");
  lcd.setCursor(0, 1);
  lcd.print("3...");
  delay(500);
  lcd.print("2...");
  delay(500);
  lcd.print("1...");
  delay(500);
  lcd.clear();
  lcd.setBacklight(TEAL);
  lcd.print("GO!!!");
  delay(300);
  lcd.clear();

  int i = 0;
  int x = 1;

  long roundStart = millis(); // current time
  long timeToPlay = 4000; // 4 seconds to enter a command

  // loop that works until the user doesn't have any time left
  // until the level is passed
  // until the level is failed
  while (roundStart + timeToPlay >= millis() && i < sequence && x == 1) {
    int buttons = lcd.readButtons();
    String bton; // stores the command the user has entered

    if (buttons & BUTTON_LEFT) {
      lcd.setBacklight(LEFT);
      bton = "LEFT";
      userInput[i] = bton; // adds the button clicked to the user array
      lcd.print(bton);
      delay(300);
      roundStart = millis(); // restarts the time to enter the next command

      // if the user input command is incorrect
      // compares the command entered by the user
      // and the one generated by the program
      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage(); // shows the corresponding message

      }


      i++; 
      // increment i by 1 after the button is clicked
      // and after the command is checked whether correct

    }

    if (buttons & BUTTON_RIGHT) {
      lcd.clear();
      lcd.setBacklight(RIGHT);
      bton = "RIGHT";
      userInput[i] = bton;
      lcd.print(bton);
      delay(300);
      roundStart = millis();

      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage();

      }

      i++;

    }

    if (buttons & BUTTON_UP) {
      lcd.setBacklight(UP);
      bton = "UP";
      userInput[i] = bton;
      lcd.print(bton);
      delay(300);
      roundStart = millis();

      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage();;
      }

      i++;

    }

    if (buttons & BUTTON_DOWN) {
      lcd.setBacklight(DOWN);
      bton = "DOWN";
      userInput[i] = bton;
      lcd.print(bton);
      delay(300);
      roundStart = millis();

      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage();
      }

      i++;

    }

    lcd.clear();
    lcd.setBacklight(7);

    // if the level is passed
    if (i == sequence && x == 1) { 
      x = 0;
      levelPassedMessage(); // shows corresponding message
    }

    // if the time is up
    if (x == 1 && roundStart + timeToPlay < millis()) {
      timeIsUp(); // shows corresponding message
      x = 0;

    }

  }

  // return to menu after game is finished
  displayMenu(); 
  menu();



}

// function to create the commands for story mode
// there are 10 levels and the difficulty is changed
// depending on which level the user has reached
void storyModeConfig() {

  lcd.setBacklight(WHITE);
  lcd.clear();

  String sequenceOfArrows[10]; 
  // initializes the array of commands for the game
  // once created, it is used for each level
  // each level in the game uses the corresponding number of commands for it
  // e.g. Level 3 - 3 commands, Level 5 - 5 commands, etc.

  // loop that generates the commands
  // works similarly to the loop that generates the commands in practice mode
  for (int i = 0; i < 10; i++) {

    int x = 1;

    while (i < 3 && x == 1) { // levels 1-3 difficulty: 2 possible arrows
      arrowIndex = rand() % 2;
      randomArrow = arrows[arrowIndex];
      sequenceOfArrows[i] = randomArrow;
      x = 0;

    }

    while (i >= 3 && i < 6 && x == 1) { // levels 4-6 difficulty: 3 possible arrows can be shown
      arrowIndex = rand() % 3;
      randomArrow = arrows[arrowIndex];
      sequenceOfArrows[i] = randomArrow;
      x = 0;
    }

    while (i >= 6 && i < 10 && x == 1) { // levels 7-10 difficulty: 4 possible arrows can be shown
      arrowIndex = rand() % 4;
      randomArrow = arrows[arrowIndex];
      sequenceOfArrows[i] = randomArrow;
      x = 0;
    }

  }

  storyMode(1, sequenceOfArrows);
  // enter story mode into the first round (sequence length of 1)
}

// function to start the story mode
// takes the array initialized in storyModeConfig() 
// and uses it until the game is finished
// works similarly to practice mode
void storyMode(int storyRound, String sequenceOfArrows[]) {

  for (int i = 0; i < storyRound; i++) {

    if (sequenceOfArrows[i] == "LEFT") {
      colour = 2;
    }
    else if (sequenceOfArrows[i] == "RIGHT") {
      colour = 3;
    }
    else if (sequenceOfArrows[i] == "UP") {
      colour = 4;
    }
    else if (sequenceOfArrows[i] == "DOWN") {
      colour = 5;
    }

    lcd.setBacklight(colour);
    lcd.print(sequenceOfArrows[i]);
    delay(1000);
    lcd.clear();
    lcd.setBacklight(0); // no light
    delay(100);
  }


  lcd.setBacklight(WHITE);
  lcd.print("Your turn!");
  lcd.setCursor(0, 1);
  lcd.print("3...");
  delay(300);
  lcd.print("2...");
  delay(300);
  lcd.print("1...");
  delay(300);
  lcd.clear();
  lcd.setBacklight(TEAL);
  lcd.print("GO!!!");
  delay(300);
  lcd.clear();
  lcd.setBacklight(WHITE);

  int i = 0;
  int x = 1;
  long roundStart = millis();
  long timeToPlay = 4000;


  while (roundStart + timeToPlay >= millis() & x == 1) {

    int buttons = lcd.readButtons();

    if (buttons & BUTTON_LEFT) {
      lcd.setBacklight(LEFT);
      bton = "LEFT";
      userInput[i] = bton;;
      lcd.print(bton);
      roundStart = millis();
      delay(300);


      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage();
      }


      i++;

    }

    if (buttons & BUTTON_RIGHT) {
      lcd.clear();
      lcd.setBacklight(RIGHT);
      bton = "RIGHT";
      userInput[i] = bton;
      lcd.print(bton);
      roundStart = millis();
      delay(300);

      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage();

      }

      i++;

    }

    if (buttons & BUTTON_UP) {
      lcd.setBacklight(UP);
      bton = "UP";
      userInput[i] = bton;
      lcd.print(bton);
      roundStart = millis();
      delay(300);

      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage();

      }

      i++;

    }

    if (buttons & BUTTON_DOWN) {
      lcd.setBacklight(DOWN);
      bton = "DOWN";
      userInput[i] = bton;
      lcd.print(bton);
      roundStart = millis();
      delay(300);

      if (userInput[i] != sequenceOfArrows[i]) {
        x = 0;
        wrongMoveMessage();

      }

      i++;

    }

    lcd.clear();
    lcd.setBacklight(7);

    // if the user has reached the last level - level 10
    if (storyRound == 10 & i == storyRound) {
      x = 0;
      lcd.clear();
      lcd.setBacklight(TEAL);
      lcd.print("You beat the");
      lcd.setCursor(0, 1);
      lcd.print("memory game!");
      delay(3000);
      lcd.setBacklight(WHITE);
    }

    

    if (i == storyRound & x == 1) {
      levelPassedMessage();
      roundStart = millis();
      int newRound = storyRound + 1;
      storyMode(newRound, sequenceOfArrows); // next level
      // function to create a story mode is called
      // with an increased number of rounds

    }

    if (i != storyRound && roundStart + timeToPlay < millis()) {
      x = 0;
      timeIsUp();
    }


  }

  // returns to menu after the game is finished

}

// a function to display the current values on the LCD
// when in main menu choosing between the modes
void displayMenu() {
  lcd.setCursor(0, 0);
  lcd.print("Select Mode:");
  sprintf(buffer, "%s", mode);
  lcd.setCursor(0, 1);
  lcd.print((mode == STORY ? "Story Mode      " : "Practice Mode   "));
}


uint8_t old_buttons = 0;

// function that shows menu screen to the display
// buttons up and down are used to change mode
// select is used to start the game in the desired mode
void menu() {

  uint8_t old_buttons = 0;
  lcd.setBacklight(WHITE);
  int x = 1;
  uint8_t buttons, changes;


  while (x == 1) {

    // clicking the button will have 1 in the button location
    buttons = lcd.readButtons();
    // button has changed from pressed to released
    changes = old_buttons & (~buttons);

    displayMenu();

    // FSM
    // waiting for user actions
    switch (mode) {

      case STORY:
        if (changes & BUTTON_UP) {
          mode = (mode == "Story Mode" ? "Practice Mode" : "Story Mode");
          mode = PRACTICE;
        } else if (changes & BUTTON_DOWN) {
          mode = (mode == "Story Mode" ? "Practice Mode" : "Story Mode");
          mode = PRACTICE;
        } else if (changes & BUTTON_SELECT) {
          lcd.clear();
          lcd.clear();
          lcd.print("Starting game...");
          delay(1000);
          lcd.setCursor(0, 1);
          lcd.print("Good luck!");
          delay(2000);
          storyModeConfig(); // enters story mode configurations

        }

        break;

      case PRACTICE:
        if (changes & BUTTON_UP) {
          mode = (mode == "Practice Mode" ? "Story Mode" : "Practice Mode");
          mode = STORY;
        } else if (changes & BUTTON_DOWN) {
          mode = (mode == "Practice Mode" ? "Story Mode" : "Practice Mode");
          mode = STORY;
        } else if (changes & BUTTON_SELECT) {
          lcd.clear();
          lcd.print("Loading game");
          lcd.setCursor(0, 1);
          lcd.print("configurations..");
          delay(2000);
          x = 0;
          display(1, 2);

        }
        break;
    }

    old_buttons = buttons;
  }

}

// a function to display the current values on the LCD
// when configuring practice mode
void display(int sequence, int arrowsNum) {
  lcd.setBacklight(WHITE);
  lcd.setCursor(0, 0);
  sprintf(buffer, "%01d             %01d ", sequence, arrowsNum);
  lcd.print(buffer);
  lcd.setCursor(0, 1);
  lcd.print((state == SEQUENCE ? "SEQUENCE        " : "         ARROWS"));
}



uint8_t oldButtons = 0;


// function to configure practice mode
// user can change the sequence length and number of arrows used
// works similarly to the main menu function
void loop() {

  lcd.setBacklight(WHITE);

  uint8_t buttons, changes;

  buttons = lcd.readButtons();
  changes = oldButtons & (~buttons);

  display(sequence, arrowsNum);

  // FSM
  // waiting for user actions
  switch (state) {

    case SEQUENCE:
      if (changes & BUTTON_UP) {
        sequence = (sequence == 10 ? 1 : sequence + 1); // sequence length possible is 1-10
      } else if (changes & BUTTON_DOWN) {
        sequence = (sequence == 1 ? 10 : sequence - 1);
      } else if (changes & BUTTON_RIGHT) {
        state = ARROWS;
      } else if (changes & BUTTON_LEFT) {
        state = ARROWS;
      } else if (changes & BUTTON_SELECT) {
        practiceMode();
      }
      break;

    case ARROWS:
      if (changes & BUTTON_UP) {
        arrowsNum = (arrowsNum == 4 ? 2 : arrowsNum + 1); // number of arrows used can be 2-4
      } else if (changes & BUTTON_DOWN) {
        arrowsNum = (arrowsNum == 2 ? 4 : arrowsNum - 1);
      } else if (changes & BUTTON_RIGHT) {
        state = SEQUENCE;
      } else if (changes & BUTTON_LEFT) {
        state = SEQUENCE;
      } else if (changes & BUTTON_SELECT) {
        practiceMode();
      }
      break;
  }

  oldButtons = buttons;

}
