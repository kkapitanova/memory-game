# memory-game

# INS: Instructions

ON STARTUP
As the game starts loading screen text will appear.
The game will enter the menu from which you can select either practice mode or story mode using the up and down buttons. Click select when the desired mode is on the screen.


PRACTICE MODE
When in practice mode, use the left and right buttons to select which part of the game you want to edit - the sequence length, or the number of arrows you want to play with, and the up and down buttons to change the value to the corresponding parameter you want to change. Press select when ready to play game. The screen will flash the commands and tell you when to enter them after a countdown. 
After the round is over, the game will return to main menu.


STORY MODE
When in story mode, the screen will flash a number of commands to the screen depending on the round you reach, starting from 1. After flashing the commands, there will be a countdown and then you can enter the commands. As the game progresses the difficulty increases. Level 10 is the last level. 
After the mode is over - the user either failed or succeeded, the game will return to main menu.


# MFS: Features of the minimal feature set

* Sequences are displayed on the screen as specified: *YES*
* Backlight flashes (only when correct): *YES*
* Incorrect answer leads to game over: *YES*
* Game supports *N* up to 10: *YES*
* Game supports *M* of size 4: *YES*
* A menu is present: *YES*
* Size of *M* can be changed: *YES*
* *N* can be changed: *YES*
* Message when wrong printed: *YES*



# OAF: Optional and additional features
* Backlight flashes a specific color for each command: green for LEFT, yellow for RIGHT, blue for UP, violet for DOWN
* Backlight flashes RED when wrong
* Story mode included



# TPL: Test plan

1. When the Arduino is booted loading screen text should be shown
2. After that, the menu is shown
3. Press up or down to change between Story Mode and Practice Mode
4. Click select when the desired mode is shown on the screen
5. If Practice Mode was chosen the display will show loading screen and enter configuration mode
6. In the upper left corner is shown the sequence number and in the upper left corner is shown the symbols (arrows) number
7. Pressing the left and right buttons will change which of the two above is selected to be changed
8. Pressing the up and down buttons will change the corresponding value
9. Press select to enter the game
10. A sequence of certain symbols (arrow commmands) is shown
11. A countdown is shown after which the user should press the correct buttons
12. The screen should flash in the specified color according to the button displayed/pressed
13. If the user enters the sequnce correctly the screen flashes in teal color and prints the corresponding success message
14. If the user fails (enters the wrong command or does not respond in time) the screen flashes in red and shows the corresponding message
15. The program returns to main menu after practice mode is finished (failed or succeeded)
16. If Story Mode is selected the display will show a countdown and the game will begin
17. The screen should flash in the specified color according to the button flashed/pressed
18. The game starts with a sequence of 1 and then will increase if the user enters it correctly. The size of commands used will also changed - Levels 1-3 has 2 possible arrows, Levels 4-6 have 3, and Levels 7-10 have 4
19. If the user fails (enters the wrong command or does not respond in time) the screen flashes in red and shows the corresponding message
20. After the user has either beaten or lost the mode the game will return to menu
