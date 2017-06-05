/*
 * GFX.h
 *
 *  Created on: 23 gen 2017
 *      Author: paolo
 */

#ifndef GFX_H_
#define GFX_H_

#include <stdlib.h>
#include <algorithm>
#include <stdint.h>
#include <memory>
#include "BaseAcccess.h"

enum class LcdID {
    ID_932X, ID_7575, ID_9341, ID_HX8357B, ID_HX8357C, ID_HX8357D, ID_SPFD5408, ID_S6D0129, ID_HX8367, ID_UNKNOWN
};

enum class DriverQuality {
    OFF, SMALL, MEDIUM_LOW, MEDIUM, MEDIUM_HIGH, HIGHT
};

struct Font {
    uint8_t xSize;
    uint8_t ySize;
    uint8_t offset;
    uint16_t numchars;
    bool row;
    const uint8_t * data;
};

extern const Font bigFont;
extern const Font smallFont;

class Color16Bit {
public:
    constexpr Color16Bit(uint8_t red, uint8_t green, uint8_t blue) :
            color((((uint16_t) red & 0xF8) << 8) | (((uint16_t) green & 0xFC) << 3) | (((uint16_t) blue & 0xF8) >> 3)) {
    }

    bool operator==(Color16Bit & a) {
        return color == a.color;
    }

    bool operator!=(Color16Bit & a) {
        return color != a.color;
    }
    uint16_t color;

};

constexpr Color16Bit BLACK(0, 0, 0);
constexpr Color16Bit WHITE(255, 255, 255);

class Color6Bit {
private:
    uint8_t components[3];
public:
    constexpr Color6Bit(uint8_t red, uint8_t blue, uint8_t green) :
            components { red, blue, green } {
    }

    bool operator==(Color6Bit & a) {
        return components[0] == a.components[0] && components[1] == a.components[1] && components[2] == a.components[2];
    }

    bool operator!=(Color6Bit & a) {
        return components[0] != a.components[0] || components[1] != a.components[1] || components[2] != a.components[2];
    }

    uint8_t getRed() {
        return components[0];
    }
    uint8_t getGreen() {
        return components[1];
    }
    uint8_t getBlue() {
        return components[2];
    }

    uint16_t get565() {
        return (((uint16_t) components[0] & 0xF8) << 8) | (((uint16_t) components[1] & 0xFC) << 3) | (((uint16_t) components[2] & 0xF8) >> 3);
    }
    void setRGB(uint8_t r, uint8_t g, uint8_t b) {
        components[0] = r << 2;
        components[1] = g << 2;
        components[2] = b << 2;
    }
};

class Point {

public:
    Point(Point && point) :
            x(point.x), y(point.y) {
    }

    Point(const Point & point) :
            x(point.x), y(point.y) {
    }

    constexpr Point(int x, int y) :
            x(x), y(y) {
    }
    constexpr Point() :
            x(0xFFFF), y(0xFFFF) {
    }

    int16_t x;
    int16_t y;
};

template<typename Color>
class GFX {
public:
    GFX() :  foreground(255, 255, 255), background(0, 0, 0) {
        driver = LcdID::ID_UNKNOWN;
        width = 0xFFFF;
        height = 0xFFFF;
        font = &bigFont;
    }
    virtual ~GFX() = default;
    static void initIO() {
        initIOPort();
    }

    void drawPixel(Point && point, Color color) {
        if ((point.x < 0) || (point.y < 0) || (point.x >= width) || (point.y >= height))
            return;
        activeCS();
        drawPixelInternal(std::move(point), std::move(color));
        idleCS();
    }
    virtual void drawPixelInternal(Point && point, Color && color)=0;
    virtual void setAddrWindow(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) =0;
    virtual void flood(Color color, uint32_t len)=0;

    void drawFastHLine(Point p, uint16_t length, Color color) {
        int16_t x2;

        // Initial off-screen clipping
        if ((p.y < 0) || (p.y >= height) || (p.x >= width) || ((x2 = (p.x + length - 1)) < 0))
            return;

        if (p.x < 0) {        // Clip left
            length += p.x;
            p.x = 0;
        }
        if (x2 >= width) { // Clip right
            x2 = width - 1;
            length = x2 - p.x + 1;
        }

        activeCS();
        setAddrWindow(p.x, p.y, x2, p.y);
        flood(color, length);
        idleCS();
    }
    void drawFastVLine(Point p, uint16_t length, Color color) {
        int16_t y2;

        // Initial off-screen clipping
        if ((p.x < 0) || (p.x >= width) || (p.y >= height) || ((y2 = (p.y + length - 1)) < 0))
            return;
        if (p.y < 0) {
            length += p.y;
            p.y = 0;
        }
        if (y2 >= height) {
            y2 = height - 1;
            length = y2 - p.y + 1;
        }

        p.x = width - p.x;
        activeCS();
        setAddrWindow(p.x, p.y, p.x, y2);
        flood(color, length);
        idleCS();
    }
    void drawLine(Point start, Point end, Color color) {
        int16_t steep = abs(end.y - start.y) > abs(end.x - start.x);
        if (steep) {
            std::swap(start.x, start.y);
            std::swap(end.x, end.y);
        }

        if (start.x > end.x) {
            std::swap(start.x, end.x);
            std::swap(start.y, end.y);
        }

        int16_t dx, dy;
        dx = end.x - start.x;
        dy = abs(end.y - start.y);

        int16_t err = dx / 2;
        int16_t ystep;

        if (start.y < end.y) {
            ystep = 1;
        } else {
            ystep = -1;
        }

        for (; start.x <= end.x; start.x++) {
            if (steep) {
                drawPixel(Point { start }, color);
            } else {
                drawPixel(Point { start }, color);
            }
            err -= dy;
            if (err < 0) {
                start.y += ystep;
                err += dx;
            }
        }
    }
    void drawCircle(Point center, int16_t r, Color color) {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        Point p { 0, r };

        drawPixel(Point(center.x, center.y + r), color);
        drawPixel(Point(center.x, center.y - r), color);
        drawPixel(Point(center.x + r, center.y), color);
        drawPixel(Point(center.x - r, center.y), color);

        while (p.x < p.y) {
            if (f >= 0) {
                p.y--;
                ddF_y += 2;
                f += ddF_y;
            }
            p.x++;
            ddF_x += 2;
            f += ddF_x;

            drawPixel(Point { center.x + p.x, center.y + p.y }, color);
            drawPixel(Point { center.x - p.x, center.y + p.y }, color);
            drawPixel(Point { center.x + p.x, center.y - p.y }, color);
            drawPixel(Point { center.x - p.x, center.y - p.y }, color);
            drawPixel(Point { center.x + p.y, center.y + p.x }, color);
            drawPixel(Point { center.x - p.y, center.y + p.x }, color);
            drawPixel(Point { center.x + p.y, center.y - p.x }, color);
            drawPixel(Point { center.x - p.y, center.y - p.x }, color);

        }
    }
    void drawCircleHelper(Point center, int16_t r, uint8_t cornername, Color color) {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;
            if (cornername & 0x4) {
                drawPixel(Point { center.x + x, center.y + y }, color);
                drawPixel(Point { center.x + y, center.y + x }, color);
            }
            if (cornername & 0x2) {
                drawPixel(Point { center.x + x, center.y - y }, color);
                drawPixel(Point { center.x + y, center.y - x }, color);
            }
            if (cornername & 0x8) {
                drawPixel(Point { center.x - y, center.y + x }, color);
                drawPixel(Point { center.x - x, center.y + y }, color);
            }
            if (cornername & 0x1) {
                drawPixel(Point { center.x - y, center.y - x }, color);
                drawPixel(Point { center.x - x, center.y - y }, color);
            }
        }
    }
    void fillCircle(Point c, int16_t r, Color color) {
        drawFastVLine(Point { c.x, c.y - r }, 2 * r + 1, color);

        fillCircleHelper(c, r, 3, 0, color);
    }
    void drawRect(Point leftTop, int16_t w, int16_t h, Color color) {
        drawFastHLine(leftTop, w, color);
        drawFastHLine(Point { leftTop.x, leftTop.y + h - 1 }, w, color);
        drawFastVLine(leftTop, h, color);
        drawFastVLine(Point { leftTop.x + w - 1, leftTop.y }, h, color);
    }
    void fillRect(Point && leftTop, int16_t w, int16_t h, Color color) {
        int16_t x2, y2;

        if ((w <= 0) || (h <= 0) || (leftTop.x >= width) || (leftTop.y >= height) || ((x2 = leftTop.x + w - 1) < 0) || ((y2 = leftTop.y + h - 1) < 0))
            return;
        if (leftTop.x < 0) {
            w += leftTop.x;
            leftTop.x = 0;
        }
        if (leftTop.y < 0) {
            h += leftTop.y;
            leftTop.y = 0;
        }
        if (x2 >= width) {
            x2 = width - 1;
            w = x2 - leftTop.x + 1;
        }
        if (y2 >= height) {
            y2 = height - 1;
            h = y2 - leftTop.y + 1;
        }

        //    x2 = width - x2;

        activeCS();
        setAddrWindow(leftTop.x, leftTop.y, x2, y2);
        flood(color, w * h);
        setAddrWindow(0, 0, width - 1, height - 1);
        idleCS();
    }
    void drawRoundRect(Point leftTop, int16_t w, int16_t h, int16_t r, Color color) {
        // smarter version
        drawFastHLine(Point { leftTop.x + r, leftTop.y }, w - 2 * r, color); // Top
        drawFastHLine(Point { leftTop.x + r, leftTop.y + h - 1 }, w - 2 * r, color); // Bottom
        drawFastVLine(Point { leftTop.x, leftTop.y + r }, h - 2 * r, color); // Left
        drawFastVLine(Point { leftTop.x + w - 1, leftTop.y + r }, h - 2 * r, color); // Right
        // draw four corners
        drawCircleHelper(Point { leftTop.x + r, leftTop.y + r }, r, 1, color);
        drawCircleHelper(Point { leftTop.x + w - r - 1, leftTop.y + r }, r, 2, color);
        drawCircleHelper(Point { leftTop.x + w - r - 1, leftTop.y + h - r - 1 }, r, 4, color);
        drawCircleHelper(Point { leftTop.x + r, leftTop.y + h - r - 1 }, r, 8, color);
    }
    void fillRoundRect(Point leftTop, int16_t w, int16_t h, int16_t r, Color color) {
        fillRect(Point(leftTop.x + r, leftTop.y), w - 2 * r, h, color);
        fillCircleHelper(Point { leftTop.x + w - r - 1, leftTop.y + r }, r, 1, h - 2 * r - 1, color);
        fillCircleHelper(Point { leftTop.x + r, leftTop.y + r }, r, 2, h - 2 * r - 1, color);
    }
    void drawChar(Point p, unsigned char c, uint8_t size) {
        if ((p.x >= width) || (p.y >= height) || ((p.x + font->xSize * size - 1) < 0) || ((p.y + font->ySize * size - 1) < 0))
            return;

        if (c < font->offset)
            return;
        c -= font->offset;
        if (c > font->numchars)
            return;

        uint8_t dataLen = (font->xSize * font->ySize) / 8;
        const uint8_t * data = font->data + dataLen * c;
        Point pixelPos = p;
        int8_t x;
        int8_t y;
        if (font->row) {
            for (pixelPos.x = p.x, x = 0; x < font->xSize; x++, pixelPos.x += size) {
                uint8_t bit = 1;
                uint8_t row = *data;
                for (pixelPos.y = p.y, y = 0; y < font->xSize; y++, pixelPos.y += size) {
                    putCharPixel(pixelPos, row, size);
                    row >>= 1;
                    bit++;
                    if (bit == 8) {
                        data++;
                        row = *data;
                        bit = 0;
                    }
                }
            }
        } else {
            for (pixelPos.y = p.y, y = 0; y < font->ySize; y++, pixelPos.y += size) {
                uint8_t bit = 1;
                uint8_t col = *data;
                for (pixelPos.x = p.x, x = 0; x < font->xSize; x++, pixelPos.x += size) {
                    putCharPixel(pixelPos, col, size);
                    col <<= 1;
                    bit++;
                    if (bit == 8) {
                        data++;
                        col = *data;
                        bit = 0;
                    }
                }
            }
        }
    }
    void drawChar(Point p, unsigned char c) {
        if ((p.x >= width) || (p.y >= height) || ((p.x + font->xSize - 1) < 0) || ((p.y + font->ySize - 1) < 0))
            return;

        if (c < font->offset)
            return;
        c -= font->offset;
        if (c > font->numchars)
            return;

        uint8_t dataLen = (font->xSize * font->ySize) / 8;
        const uint8_t * data = font->data + dataLen * c;
        Point pixelPos = p;
        int8_t x;
        int8_t y;
        if (font->row) {
            for (pixelPos.x = p.x, x = 0; x < font->xSize; x++, pixelPos.x++) {
                uint8_t bit = 1;
                uint8_t row = *data;
                for (pixelPos.y = p.y, y = 0; y < font->xSize; y++, pixelPos.y++) {
                    putCharPixel(pixelPos, row, 1);
                    row >>= 1;
                    bit++;
                    if (bit == 8) {
                        data++;
                        row = *data;
                        bit = 0;
                    }
                }
            }
        } else {
            for (pixelPos.y = p.y, y = 0; y < font->ySize; y++, pixelPos.y++) {
                uint8_t bit = 1;
                uint8_t col = *data;
                for (pixelPos.x = p.x, x = 0; x < font->xSize; x++, pixelPos.x++) {
                    putCharPixel(pixelPos, col, 1);
                    col <<= 1;
                    bit++;
                    if (bit == 8) {
                        data++;
                        col = *data;
                        bit = 0;
                    }
                }
            }
        }
    }
    void drawString(Point && p, const char * s) {
        while (*s != 0) {
            drawChar(p, *s);
            p.x += font->xSize;
            s++;
        }
    }
    void setFont(const Font * font) {
        this->font = font;
    }
    void setForeground(Color color) {
        foreground = color;
    }
    void setBackground(Color color) {
        background = color;
    }

    uint16_t width;
    uint16_t height;
private:
    void fillCircleHelper(Point c, int16_t r, uint8_t cornername, int16_t delta, Color6Bit color) {
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;

            if (cornername & 0x1) {
                drawFastVLine(Point { c.x + x, c.y - y }, 2 * y + 1 + delta, color);
                drawFastVLine(Point { c.x + y, c.y - x }, 2 * x + 1 + delta, color);
            }
            if (cornername & 0x2) {
                drawFastVLine(Point { c.x - x, c.y - y }, 2 * y + 1 + delta, color);
                drawFastVLine(Point { c.x - y, c.y - x }, 2 * x + 1 + delta, color);
            }
        }
    }
    void putCharPixel(Point p, uint8_t bits, uint8_t size) {
        if (bits & 0x80) {
            if (size == 1)
                drawPixel(Point { p }, foreground);
            else {
                fillRect(Point { p }, size, size, foreground);
            }
        } else if (background != foreground) {
            if (size == 1)
                drawPixel(Point { p }, background);
            else {
                fillRect(Point { p }, size, size, background);
            }
        }
    }
protected:
    LcdID driver;

    const Font * font;
    Color foreground;
    Color background;
}
;

#endif /* GFX_H_ */
