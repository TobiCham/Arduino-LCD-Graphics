#ifndef ARD_LCD_RENDERER_H
#define ARD_LCD_RENDERER_H

#include <LiquidCrystal.h>

enum Orientation {
    HORIZONTAL, VERTICAL
};

class LCDRenderer {
public:

    /**
     * @param lcd LCD display to use
     * @param orientation Either horizontal or vertical
     * @param cellWidth The width in pixels of one display cell of the LCD, relative to your orientation
     * @param cellHeight The height in pixels of one display cell of the LCD, relative to your orientation
     * @param cellsX How many cells make up the width of the LCD, relative to your orientation
     * @param cellsY How many cells make up the height of the LCD, relative to your orientation
     * @param maxCustomChars How many custom characters the LCD can support
     */
    LCDRenderer(LiquidCrystal *lcd, Orientation orientation, unsigned int cellWidth, unsigned int cellHeight,
                unsigned int cellsX, unsigned int cellsY, unsigned int maxCustomChars);

    ~LCDRenderer();

    /**
     * Writes all pixel data to the display then clears the pixel buffer
     */
    void render();

    /**
     * Draws a filled rectangle onto the buffer
     */
    void fillRect(int x, int y, int width, int height);

    /**
     * Draws the outline of a rectangle onto the buffer
     */
    void drawRect(int x, int y, int width, int height);

    /**
     * Sets a pixel in the buffer to on or off
     */
    void setPixel(int x, int y, bool on);

    /**
     * @return whether the specified pixel is on or off at the coordinates in the pixel buffer
     */
    bool getPixel(int x, int y);

    inline LiquidCrystal *getLCD() { return lcd; }

    /**
     * @return width of the display, in pixels, relative to your orientation
     */
    inline unsigned int getWidth() { return orientation == Orientation::VERTICAL ? width : height; }

    /**
     * @return height of the display, in pixels, relative to your orientation
     */
    inline unsigned int getHeight() { return orientation == Orientation::VERTICAL ? height : width; }

    inline unsigned int getMaxCustomChars() { return maxCustomChars; }

    inline Orientation getOrientation() { return orientation; }

private:
    LiquidCrystal *lcd;
    Orientation orientation;

    const unsigned int width, height, cellWidth, cellHeight, cellsX, cellsY;

    const unsigned short pixelsLen;
    uint8_t *pixels;

    const unsigned int maxCustomChars;
    const unsigned int customCharsLen;
    short customCharCounter;
    uint8_t *customChars;

    bool shouldRender(int x, int y);

    bool charCellMatch(uint8_t *cell, int charIndex);

    short createCharacter(int cellX, int cellY);

    void renderCell(int cellX, int cellY);

    inline int getPixelIndex(int x, int y, int row) {
        return (x * cellsY * cellWidth) + (y * cellWidth) + row;
    }

    inline uint8_t *getCell(int x, int y) {
        return pixels + (x * cellsY * cellWidth) + (y * cellWidth);
    }

    inline int getCustomCharIndex(int customChar, int row) {
        return (customChar * cellWidth) + row;
    }
};

#endif
