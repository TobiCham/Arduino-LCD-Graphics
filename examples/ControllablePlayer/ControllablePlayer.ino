#include <LiquidCrystal.h>
#include "LCDRenderer.h"

/**
 * This uses a joystick input as a controller, however you could also use two variable resistors.
 * Moving the joystick allows you to move the player around the screen
 */

//Initialise LCD on pins 2, 3, 4, 5, 6, 7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//My LCD has a grid of 16x2 cells, with each cell being 5x8. It has 8 available custom characters
LCDRenderer graphics( & lcd, Orientation::HORIZONTAL, 5, 8, 16, 2, 8);

//--Player variables--
float playerX, playerY, playerSpeed = 2;
float playerWidth = 5, playerHeight = 5;

/**
 * @param analogValue Value read from analog read. Must be between 0 and 1024
 * @return a value between -1 and 1
 */
float getJoystickMovement(int analogValue) {
    analogValue -= 512;

    //Any value less than 35 should be treated as the joystick is still centred
    if (analogValue > -35 && analogValue < 35) return 0;
    return analogValue / 512.0;
}

void centrePlayer() {
    playerX = (graphics.getWidth() - playerWidth) / 2;
    playerY = (graphics.getHeight() - playerHeight) / 2;
}

void updatePlayer() {
    int dx = analogRead(A4);
    int dy = analogRead(A5);

    //Move based on joysitck input
    playerX += getJoystickMovement(dx) * playerSpeed;
    playerY += getJoystickMovement(dy) * playerSpeed;

    //Prevent going out of bounds
    if (playerX <= 0) playerX = 0;
    if (playerX + playerWidth > graphics.getWidth()) playerX = graphics.getWidth() - playerWidth;

    if (playerY <= 0) playerY = 0;
    if (playerY + playerHeight > graphics.getHeight()) playerY = graphics.getHeight() - playerHeight;
}

void setup() {
    lcd.begin(16, 2);
    centrePlayer();
}

void loop() {
    //Render graphics at a frequency of 4Hz
    updatePlayer();

    //Render the player as a rectangle outline
    graphics.drawRect((int) playerX, (int) playerY, (int) playerWidth, (int) playerHeight);

    //Render the graphics to the LCD
    graphics.render();
    delay(250);
}
