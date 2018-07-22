#include <LiquidCrystal.h>
#include "LCDRenderer.h"

//Initialise LCD on pins 2, 3, 4, 5, 6, 7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//My LCD has a grid of 16x2 cells, with each cell being 5x8. It has 8 available custom characters
LCDRenderer graphics( & lcd, Orientation::HORIZONTAL, 5, 8, 16, 2, 8);

void setup() {
    lcd.begin(16, 2);

    graphics.drawRect(2, 3, 10, 10);

    graphics.fillRect(5, 6, 4, 4);

    graphics.render();
}

void loop() {}
