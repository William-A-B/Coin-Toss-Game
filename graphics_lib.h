/* graphics_lib.h defines functions which hide
   complexities of allegro and allow
   the C programming labscripts to be used
   almost as is. This is done so that many of the complexities
   of C are hidden from the beginner.
   graphics_lib.h does not update the display after each drawing
   function, instead it expects the programmer to do that
   (using update_display).
   This will allow the ability to simulate multiple events.
   Author: Julian Miller, Department of Electronics
   University of York, Jan 2013.

   Updates: Bitmap handling added on 29/4/2013
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#ifndef __GRAPHICS_LIB__
#define __GRAPHICS_LIB__


typedef ALLEGRO_BITMAP* BITMAP;

typedef struct colour_info
{
  unsigned r;
  unsigned g;
  unsigned b;
} COLOUR;

#define  PI  3.141592653589793

#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGHTCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15

/* Mouse button definitions - these are from the allegro API */
#define MOUSE_BUTTON_LEFT (1)
#define MOUSE_BUTTON_RIGHT (2)
#define MOUSE_BUTTON_SCROLL (4)


/**
    Returns a pseudo random integer in range
    [lower_range, upper_range]

    @param lower_range Lowest number the random number will be
    @param upper_range Highest number the random number will be
    @return random number between or equal to lower_range and
    upper_range

*/
int GFX_RandNumber(int lower_range, int upper_range);

/**
    Waits for some milliconds
    @param time_in_milliseconds Time to wait for in ms.
*/
void GFX_PauseFor(int time_in_milliseconds);

void GFX_PauseEventQueue(bool pause);
/**
    Initialises a graphics window
    @param x Width of window in pixels
    @param y Height of window in pixels
*/
void GFX_InitWindow(int x, int y);

/**
    Initialises the graphics font system
*/
void GFX_InitFont(int text_size);

/**
    Closes the graphics window
*/
void GFX_CloseWindow(void);

/**
    Creates an RGB colour from a colour index (see colour defines)
    @param colour Colour to create
    @return Colour structure with correct values for the given colour
*/
COLOUR GFX_MakeRGB(unsigned colour);

/**
    Sets the current drawing colour
    @param colour Colour to use
*/
void GFX_SetColour(unsigned colour);

/* this moves the screen buffer to the display */
/**
    Writes to the screen any changes made since the last update
*/
void GFX_UpdateDisplay(void);

/**
    Draws a filled circle to the screen. The outline will be drawn
    in the current colour (change by using GFX_SetColour().

    @param x X coordinate of the centre of the circle
    @param y Y coordinate of the centre of the circle
    @param radius Radius of the circle
    @param fillcolour Colour to fill the circle with (see colour defines)
*/
void GFX_DrawFilledCircle(int x, int y, int radius, unsigned fillcolour);

/**
    Draws a circle
    @param x X coordinate of the centre of the circle
    @param y Y coordinate of the centre of the circle
    @param radius Radius of the circle
    @param thickness Thickness of line in pixels
*/
void GFX_DrawCircle(int x, int y, int radius, int thickness);

/**
    Draws a filled rectange

    @param upper_left_x X value of coordinate of upper left corner
    @param int upper_left_y Y value of coordinate of upper left corner
    @param int lower_right_x X value of coordinate of lower right corner
    @param lower_right_y Y value of coordinate of lower right corner
    @param fillcolour Colour to fill the circle with (see colour defines)
*/
void GFX_DrawFilledRectangle(int upper_left_x, int upper_left_y,
               int lower_right_x, int lower_right_y,
               unsigned fillcolour);

/**
    Draws a rectangle
    @param upper_left_x X value of coordinate of upper left corner
    @param int upper_left_y Y value of coordinate of upper left corner
    @param int lower_right_x X value of coordinate of lower right corner
    @param lower_right_y Y value of coordinate of lower right corner
    @param thickness Thickness of line in pixels

*/
void GFX_DrawRectangle(int upper_left_x, int upper_left_y,
               int lower_right_x, int lower_right_y,
               int thickness);

/**
    Draws a filled triangle given by three points
    @param x1 X value of first coordinate
    @param y1 Y value of first coordinate
    @param x1 X value of second coordinate
    @param y1 Y value of second coordinate
    @param x1 X value of third coordinate
    @param y1 Y value of third coordinate
    @param fillcolour Colour to fill the circle with (see colour defines)
*/
void GFX_DrawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned fillcolour);

/**
    Draws a filled ellipse. Ellipses' axis are always vertical and
    horizontal and cannot be rotated

    @param centre_x X value of coordinate of centre of ellipse
    @param centre_y Y value of coordinate of centre of ellipse
    @param radius_x Radius of the ellipse in the x direction
    @param radius_y Radius of the ellipse in the y direction
    @param fillcolour Colour to fill the circle with (see colour defines)

*/
void GFX_DrawFilledEllipse(int centre_x, int centre_y,
               int radius_x, int radius_y,
               unsigned fillcolour);

/**
    Draws a filled ellipse. Ellipses' axis are always vertical and
    horizontal and cannot be rotated

    @param centre_x X value of coordinate of centre of ellipse
    @param centre_y Y value of coordinate of centre of ellipse
    @param radius_x Radius of the ellipse in the x direction
    @param radius_y Radius of the ellipse in the y direction
    @param thickness Thickness of line in pixels

*/
void GFX_DrawEllipse(int centre_x, int centre_y,
               int radius_x, int radius_y,
               int thickness);

/**
    Draws an arc (section of a circle)
    @param centre_x X value of coordinate of centre of circle
    from which the arc is derived
    @param centre_y Y value of coordinate of centre of circle
    from which the arc is derived
    @param angle_start Angle in degrees at which the arc should start
    @param angle_end Angle in degrees at which the arc should end
    @param thickness Thickness of line in pixels
*/
void GFX_DrawArc(int centre_x, int centre_y, int radius,
         double angle_start, double angle_end, int thickness);

/**
    Sets the background colour of the graphics window

    @param colour Colour of background (see colour defines)
*/
void GFX_SetBackgroundColour(unsigned colour);

/**
    Resets canvas to background colour (set using
    GFX_SetBackgroundColour)
*/
void GFX_ClearWindow(void);

/**
    Draws a line between two points

    @param xstart X value of start coordinate
    @param ystart Y value of start coordinate
    @param xend X value of end coordinate
    @param yend Y value of end coordinate
    @param thickness Thickness of line in pixels
*/
void GFX_DrawLine(int xstart, int ystart, int xend, int yend, int thickness);

/**
    Draws a line from the current pen point to the specified coordinate
    @param x X component of coordinate to draw line to
    @param y Y component of coordinate to draw line to
    @param thickness Thickness of line in pixels

*/
void GFX_DrawLineTo(int x, int y, int thickness);

/**
    Moves the current pen point to the specified coordinate
    @param x X component of coordinate to move to
    @param y Y component of coordinate to move to
*/
void GFX_MoveTo(int x, int y);

/**
    Writes text to screen

    @param x X component of start coordinate
    @param y Y component of start coordinate
    @param text Null-terminated string to write to the screen
*/
void GFX_DrawText(int x, int y, const char * text);

/**
    Creates an event queue. Must be called before registering any events
*/
void GFX_CreateEventQueue(void);

/**
    Registers display events (e.g. quitting the window) with the
    event queue. Must be called after GFX_CreateEventQueue
*/
void GFX_RegisterEventSource(void);

/**
    Registers a timer with the event queue
*/

bool GFX_EventTimer(void);

/**
    Checks if an event timer has been added to
    the event queue
*/

void GFX_RegisterDisplayEvents(void);

/**
    Registers mouse events with the event queue.
    Must be called after GFX_CreateEventQueue
*/
void GFX_RegisterMouseEvents(void);

/**
    Registers keyboard events with the event queue.
    Must be called after GFX_CreateEventQueue

*/
void GFX_RegisterKeyboardEvents(void);

/**
    Hides the mouse cursor
*/
void GFX_HideCursor(void);

/**
    Checks whether an event is waiting

    @return Whether an event is waiting
    @retval 1 if there is an event waiting
    @retval 0 if there are no events waiting
*/
int GFX_IsEventWaiting(void);

/* wait until the event queue is non-empty. In other words wait
   until the user makes an event (moves mouse, clicks a mouse button
or close the display */
/**
    Waits (does not return) until an event occurs. If two events occur at once,
    the first one will cause this function to return and the second one will
    cause this function to return as soon as it is called the next time to
    allow that event to be processed.
*/
void GFX_WaitForEvent(void);

/**
    Checks whether the event is that the user closed the display

    @return Whether the event is the user closing the display
    @retval 1 if the event is the user closing the display
    @retval 0 if the event is not the user closing the display
*/
int GFX_IsEventCloseDisplay(void);

/**
    Checks whether the event is that the user moved the mouse

    @return Whether the event is the user moved the mouse
    @retval 1 if the event is the user moved the mouse
    @retval 0 if the event is not the user moved the mouse
*/
int GFX_IsEventMouseMoved(void);

/**
    If the most recent event was a mouse event, this function
    allows the caller to get the coordinates of the mouse at the
    point when the event occurred.

    @param x_ptr Pointer to integer where the x coordinate will
    be stored
    @param y_ptr Pointer to integer where the y coordinate will
    be stored
    @return Whether the function was successful or not
    @retval 1 if function was successful (event was a mouse event)
    @retval 0 if function was unsuccessful (event was not a mouse event)
*/
int GFX_GetMouseCoordinates(int * x_ptr, int * y_ptr);

/**
    If the most recent event was a mouse button event, this function
    can be used to identify which button was used.

    @param button_ptr Pointer to integer where the button id will be
    placed. This could be one of the following:
    MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_SCROLL
    @return Whether the function was successful or not
    @retval 1 if function was successful (event was a mouse press)
    @retval 0 if function was unsuccessful (event was not a mouse press)

*/
int GFX_GetMouseButton(int * button_ptr);

/**
    Checks whether the event is a mouse button press

    @return Whether the event is a mouse button press
    @retval 1 if the event is a mouse button press
    @retval 0 if the event is not a mouse button press
*/
int GFX_IsEventMouseButton(void);

/**
    Checks whether the event is a keyboard key being pressed

    @return Whether the event is a keyboard key being pressed
    @retval 1 if the event is a keyboard key being pressed
    @retval 0 if the event is not a keyboard key being pressed
*/
int GFX_IsEventKeyDown(void);

/**
    Checks whether the event is a keyboard key being released

    @return Whether the event is a keyboard key being released
    @retval 1 if the event is a keyboard key being released
    @retval 0 if the event is not a keyboard key being released
*/
int GFX_IsEventKeyUp(void);

/**
    Used to get which key has been pressed/released
    @param keypress_ptr Pointer to integer to store the index of
    the key that has been pressed. Use names to identify keys. The
    names are in the format ALLEGRO_KEY_[key]. For example, the
    escape key is ALLEGRO_KEY_ESCAPE, and the q key is
    ALLEGRO_KEY_Q.
    Full list: https://sourceforge.net/p/alleg/allegro/ci/31f83ea2a1742b4f5c79f64839e30b3fa665d150/tree/include/allegro/keycodes.h


    @return Whether the function was successful or not
    @retval 1 if function was successful (event was a key press/release)
    @retval 0 if function was unsuccessful (event was not a key press/release)

*/
int GFX_GetKeyPress(int * keypress_ptr);

/**
    Initialise the mouse
*/
void GFX_InitMouse(void);

/**
    Close the mouse
*/
void GFX_CloseMouse(void);

/**
    Initialise the keyboard
*/
void GFX_InitKeyboard(void);

/**
    Close the keyboard
*/
void GFX_CloseKeyboard(void);

/**
    Initialise a picture structure

    @return initialised bitmap structure
*/
BITMAP GFX_InitBitmap(void);

/**
    Load a picture from a file

    @param name Filename (including path) of image file
    @return Image structure containing image
*/
BITMAP GFX_LoadBitmap(char* name);

/* make the colour background of the bitmap
    transparent - note the image background
    must be one colour - i.e. the red green and blue
    pixels must all be the same for the background.
*/
/**
    Make the colour background of a picture trasparent. Note the
    background must be a single colour
    @param image Image to affect
    @param red Red colour component of background colour to
    make transparent (0-255)
    @param green Green colour component of background colour
    to make transparent (0-255)
    @param blue Blue colour component of background colour
    to make transparent (0-255)
*/
void GFX_MakeImageBGTransparent(BITMAP image, int red, int green, int blue);

/* draw the bitmap so its centre is at x, y in the screen */
/**
    Draw image so the centre is at the specified point.
    @param image Image to draw
    @param x X value of coordinate of the centre of the image
    @param y Y value of coordinate of the centre of the image
*/
void GFX_DrawBitmap(BITMAP image, int x, int y);

/**
    Delete an image from program memory that's no longer in
    use (needed for saving memory)

    @param image Image to delete
*/
void GFX_FreeBitmap(BITMAP image);


#endif /* __GRAPHICS_LIB__ */

