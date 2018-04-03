## Overview

This library attempts to create a Graphics style library to render simple graphics to an [Arduino LCD display](https://www.arduino.cc/en/Tutorial/HelloWorld).

The standard LCD display that is used with Arduinos is primarily a text based display, and does not support rendering custom graphics. Fortunately, most displays do have custom characters which can be rendered as characters. This library makes use of these to dynamically render graphics.

<img src="/examples/StaticDraw.png" width="500"/>

## Limitations
The LCD display was not designed with this use in mind. The complexity of graphics which can be rendered is limited by the number of custom characters which can be rendered. The display I have used to test this has 8. This means that only 8 unique cells can be rendered on the display at once. Therefore, this library should only primarily be used to draw simple graphics.

## Initialisation
Include the two files in your project, and include the header file
```c
#include "LCDRenderer.h"
```

Then, create a renerer object for your orientation.

Horizontal:
```c
//My LCD has a grid of 16x2 cells, with each cell being 5x8. It has 8 available custom characters
LCDRenderer graphics(&lcd, Orientation::HORIZONTAL, 5, 8, 16, 2, 8);
```

Vertical:
```c
//My LCD has a grid of 16x2 cells, with each cell being 5x8. It has 8 available custom characters.
//Due to being viewed vertically however, we view it as being 2x16 and each cell as 8x5
LCDRenderer graphics(&lcd, Orientation::VERTICAL, 8, 5, 2, 16, 8);
```

## Usage
All draw calls are cached until the render method is called. Remember this! You must call render() in order for graphics to be displayed! Here is a simple example which draws a filled rectangle inside a rectangle outline
```c
#include <LiquidCrystal.h>
#include "LCDRenderer.h"

//Initialise LCD on pins 2, 3, 4, 5, 6, 7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//My LCD has a grid of 16x2 cells, with each cell being 5x8. It has 8 available custom characters
LCDRenderer graphics(&lcd, Orientation::HORIZONTAL, 5, 8, 16, 2, 8);

void setup() {
  lcd.begin(16, 2);

  graphics.drawRect(2, 3, 10, 10);
  graphics.fillRect(5, 6, 4, 4);

  graphics.render();
}

void loop() {}
```

Other examples are included in the [examples](examples/) directory.
Pull requests for additional draw methods are welcome
