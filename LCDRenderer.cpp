#include "LCDRenderer.h"
#include "Arduino.h"

LCDRenderer::LCDRenderer(LiquidCrystal *lcd, Orientation orientation, unsigned int cellWidth, unsigned int cellHeight,
                         unsigned int cellsX, unsigned int cellsY, unsigned int maxCustomChars)
        : cellWidth(orientation == Orientation::VERTICAL ? cellWidth : cellHeight),
          cellHeight(orientation == Orientation::VERTICAL ? cellHeight : cellWidth),
          cellsX(orientation == Orientation::VERTICAL ? cellsX : cellsY),
          cellsY(orientation == Orientation::VERTICAL ? cellsY : cellsX),
          maxCustomChars(maxCustomChars),
          width(orientation == Orientation::VERTICAL ? (cellWidth * cellsX) : (cellHeight * cellsY)),
          height(orientation == Orientation::VERTICAL ? (cellHeight * cellsY) : (cellWidth * cellsX)),
          orientation(orientation),
          pixelsLen(cellsX * cellsY * (orientation == Orientation::VERTICAL ? cellWidth : cellHeight)),
          customCharsLen(maxCustomChars * (orientation == Orientation::VERTICAL ? cellWidth : cellHeight)) {

    this->lcd = lcd;
    this->pixels = new uint8_t[pixelsLen];
    this->customChars = new uint8_t[customCharsLen];

    for (unsigned int i = 0; i < pixelsLen; i++) pixels[i] = 0;
}

LCDRenderer::~LCDRenderer() {
    delete pixels;
    delete customChars;
}

void LCDRenderer::fillRect(int x, int y, int width, int height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) setPixel(x + i, y + j, true);
    }
}

void LCDRenderer::drawRect(int x, int y, int width, int height) {
    for (int i = 0; i < width; i++) {
        setPixel(i + x, y, true);
        setPixel(i + x, y + height - 1, true);
    }
    for (int i = 1; i < height - 1; i++) {
        setPixel(x, i + y, true);
        setPixel(x + width - 1, i + y, true);
    }
}

void LCDRenderer::setPixel(int x, int y, bool on) {
    if (orientation == Orientation::HORIZONTAL) {
        int temp = x;
        x = y;
        y = temp;
    }

    if (x < 0 || y < 0 || x >= width || y >= height) return;
    int index = getPixelIndex(x / cellWidth, y / cellHeight, x % cellWidth);
    int mask = 1 << cellHeight - (y % cellHeight) - 1;

    if (on) pixels[index] |= mask;
    else pixels[index] &= ~mask;
}

bool LCDRenderer::getPixel(int x, int y) {
    if (orientation == Orientation::HORIZONTAL) {
        int temp = x;
        x = y;
        y = temp;
    }
    if (x < 0 || y < 0 || x >= width || y >= height) return false;

    int index = getPixelIndex(x / cellWidth, y / cellHeight, x % cellWidth);
    return (pixels[index] >> cellHeight - (y % cellHeight) - 1) & 1;
}

void LCDRenderer::render() {
    lcd->clear();
    for (int i = 0; i < customCharsLen; i++) customChars[i] = 0;
    customCharCounter = 0;

    for (int i = 0; i < cellsX; i++) {
        for (int j = 0; j < cellsY; j++) {
            if (shouldRender(i, j)) {
                renderCell(i, j);
                if (customCharCounter >= maxCustomChars) goto endLoop;
            }
        }
    }
    endLoop:
    for (int i = 0; i < pixelsLen; i++) pixels[i] = 0;
}

void LCDRenderer::renderCell(int cellX, int cellY) {
    short id = createCharacter(cellX, cellY);
    if (id < 0) return;
    lcd->setCursor(cellY, cellX);
    lcd->write((uint8_t) id);
}

short LCDRenderer::createCharacter(int cellX, int cellY) {
    uint8_t *cell = getCell(cellX, cellY);

    short cellID = -1;
    for (short i = 0; i < customCharCounter; i++) {
        if (charCellMatch(cell, i)) {
            cellID = i;
            break;
        }
    }
    if (cellID >= 0) return cellID;

    if (customCharCounter >= maxCustomChars) return -1;

    lcd->createChar((uint8_t) customCharCounter, cell);

    for (int i = 0; i < cellWidth; i++) {
        customChars[getCustomCharIndex(customCharCounter, i)] = cell[i];
    }

    customCharCounter++;
    return customCharCounter - 1;
}

bool LCDRenderer::shouldRender(int x, int y) {
    for (int i = 0; i < cellWidth; i++) {
        if (pixels[getPixelIndex(x, y, i)] != 0) {
            return true;
        }
    }
    return false;
}

bool LCDRenderer::charCellMatch(uint8_t *cell, int charIndex) {
    for (int i = 0; i < cellWidth; i++) {
        if (cell[i] != customChars[getCustomCharIndex(charIndex, i)]) return false;
    }
    return true;
}
