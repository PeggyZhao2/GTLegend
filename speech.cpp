#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
#define PURPLE 0x9400d3
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{
    uLCD.filled_rectangle(3, 94, 123, 114, BLACK);
}

///?
void erase_speech_bubble()
{
    uLCD.filled_rectangle(0,100,127,127, BLACK);
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
    uLCD.line(0, 9, 127, 9, GREEN);
}

void draw_speech_line(const char* line, int which)
{
    //which == 0---top
    if (!which) {
        uLCD.locate(1, 12);
    }
    if (which) {
        uLCD.locate(1, 13);
    }
    if (line) {
        uLCD.printf("%s", line);
    }
}

void speech_bubble_wait()
{
    wait(1.5);
}

void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();//?
}


void long_speech(const char* lines[], int n)
{   
}
