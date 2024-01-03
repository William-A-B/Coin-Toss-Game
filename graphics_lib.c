/* graphics_lib.c defines the implementations of the
   wrapper functions which hide
   complexities of allegro and allow
   the C programming labscripts to be used
   almost as is. This is done so that many of the complexities
   of C are hidden from the beginner.

   Author: Julian Miller, Department of Electronics
   University of York, Jan 2013.

   Updates:
        -Bitmap handling added on 29/4/2013
        -Function names updated to add consistency 05/12/2016 jdf513@york.ac.uk

   Notes:
   Originally, these function implementations were part of
   graphic_lib.h. This was done so that only one file needed
   in programs to allow graphics commands. However this caused
   problems when #including the file multiple times (redefinition
    of functions!
*/

#include "graphics_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


/* globals */
COLOUR        RGB, BACKGROUND_RGB;
ALLEGRO_DISPLAY     *DISPLAY;
int                 GLOBAL_X, GLOBAL_Y;
ALLEGRO_FONT        *GLOBAL_FONT; /*global font pointer */
ALLEGRO_PATH        *path;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_EVENT       event;
ALLEGRO_TIMER       *timer;

/* provide a pseudo random integer in range
   [lower_range, upper_range]
*/
int GFX_RandNumber(int lower_range, int upper_range)
{
    int x;

    if (upper_range < lower_range)
    {
        x = upper_range;
        upper_range = lower_range;
        lower_range = x;
    }
    return lower_range + rand() %(upper_range -lower_range +1);

}

/* wait for some milliconds (given as argument) */
void GFX_PauseFor(int time_in_milliseconds)
{

    al_rest(time_in_milliseconds/1000.0);

}

void GFX_PauseEventQueue(bool pause)
{
    al_pause_event_queue(event_queue, pause);
}


/* wrapper function defining initwindow in terms of
   allegro routines */
void GFX_InitWindow(int x, int y)
{
    /* These variables are globals defined
       in graphics_lib.h  */
    GLOBAL_X = 0;
    GLOBAL_Y = 0;
    event_queue= NULL;
    DISPLAY = NULL;

    if (!al_init())
    {
        printf("\nIn GFX_InitWindow: al_init failed\n");
        exit(1);
    }


    al_init_image_addon();

    /* allow the use of allegro drawing primitives */
    al_init_primitives_addon();

    DISPLAY = al_create_display(x, y);

    if (!DISPLAY)
       exit(1);

    GFX_SetColour(WHITE);  /* make initial pen colour white */

    GFX_SetBackgroundColour(BLACK); /* make initial background colour black */

}

/* load a font type for writing on graphics window. This is ugly
as (a) there are not many fonts available and (b) it needs a path to
the font definitions. Investigate a better way.  */
void GFX_InitFont(int text_size)
{
    /* fonts will only work from Code::Blocks IDE unless
      the data folder has been copied to the folder where
      the executable is */
    char pathtofont[300]="data/times.ttf";

    /* allow the use fo allegro fonts */
    al_init_font_addon();
    al_init_ttf_addon();

   /* load a font. This uses windows paths, so would have to
    be changed in Unix and possibly MacOS */


    GLOBAL_FONT = al_load_font(pathtofont, text_size, 0);

    if (!GLOBAL_FONT)
    {
        printf("\nFailed to load fixed_font.tga\n");
        printf("\nYou must be running programs with text outside Code::Blocks IDE");
        printf("\nGFX_DrawText requires predefined fonts. You need to copy the data folder");
        printf("\ninto the folder where your executable file is\n");
        exit(0);
    }
}

/* wraper for allegro close of display window */
void GFX_CloseWindow(void)
{
    al_destroy_display(DISPLAY);
}

/* given an integer colour value returns a RGB structure colour */
COLOUR GFX_MakeRGB(unsigned colour)
{
    COLOUR result;

    switch(colour)
    {
        case BLACK: result.r = 0; result.g = 0; result.b = 0;
        break;
        case BLUE: result.r = 0; result.g = 0; result.b = 255;
        break;
        case GREEN: result.r = 0; result.g = 255; result.b = 0;
        break;
        case CYAN: result.r = 0; result.g = 255; result.b = 255;
        break;
        case RED: result.r = 255; result.g = 0; result.b = 0;
        break;
        case MAGENTA: result.r = 255; result.g = 0; result.b = 255;
        break;
        case BROWN: result.r = 139; result.g = 69; result.b = 19;
        break;
        case LIGHTGRAY: result.r = 200; result.g = 200; result.b = 200;
        break;
        case DARKGRAY: result.r = 100; result.g = 100; result.b = 100;
        break;
        case LIGHTBLUE: result.r = 200; result.g = 200; result.b = 255;
        break;
        case LIGHTGREEN: result.r = 200; result.g = 255; result.b = 200;
        break;
        case LIGHTCYAN: result.r = 200; result.g = 255; result.b = 255;
        break;
        case LIGHTRED: result.r = 255; result.g = 200; result.b = 200;
        break;
        case LIGHTMAGENTA: result.r = 255; result.g = 200; result.b = 200;
        break;
        case YELLOW: result.r = 255; result.g = 255; result.b = 0;
        break;
        case WHITE: result.r = 255; result.g = 255; result.b = 255;
    }
    return result;
}

/* It writes the RGB value to a global that can be used by
   graphics drawing wrapper functions */
void GFX_SetColour(unsigned x)
{
    RGB = GFX_MakeRGB(x);
}

/* this moves the screen buffer to the display */
void GFX_UpdateDisplay(void)
{
    /* allegro routines write to a screen buffer
    and this has to be moved to the screen buffer
    to actual display so we can see it */
    al_flip_display();
}

/* wrapper for a filled circle function */
void GFX_DrawFilledCircle(int x, int y, int radius, unsigned fillcolour)
{
    COLOUR fill;

    GLOBAL_X = x;
    GLOBAL_Y = y;


    fill = GFX_MakeRGB(fillcolour);
    al_draw_filled_circle(x, y , radius, al_map_rgb(fill.r,fill.g,fill.b));
}

/* wrapper for the circle function */
void GFX_DrawCircle(int x, int y, int radius, int thickness)
{
    GLOBAL_X = x;
    GLOBAL_Y = y;

    al_draw_circle(x, y, radius, al_map_rgb(RGB.r,RGB.g,RGB.b),thickness);

}

/* wrapper for the rectangle function */
void GFX_DrawFilledRectangle(int upper_left_x, int upper_left_y,
               int lower_right_x, int lower_right_y,
               unsigned fillcolour)
{
    COLOUR fill;

    GLOBAL_X = lower_right_x;
    GLOBAL_Y = lower_right_y;


    fill = GFX_MakeRGB(fillcolour);

    al_draw_filled_rectangle(upper_left_x, upper_left_y,
                          lower_right_x, lower_right_y,
                          al_map_rgb(fill.r,fill.g,fill.b));
}

/* wrapper for the rectangle function */
void GFX_DrawRectangle(int upper_left_x, int upper_left_y,
               int lower_right_x, int lower_right_y,
               int thickness)
{

    GLOBAL_X = lower_right_x;
    GLOBAL_Y = lower_right_y;


    al_draw_rectangle(upper_left_x, upper_left_y,
                    lower_right_x, lower_right_y,
                    al_map_rgb(RGB.r,RGB.g,RGB.b),thickness);
}

/* wrapper for allegro filled triangle */
void GFX_DrawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned colour)
{
    GLOBAL_X = x3;
    GLOBAL_Y = y3;

    COLOUR fill;
    fill = GFX_MakeRGB(colour);

    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, al_map_rgb(fill.r,fill.g,fill.b));
}

/* wrapper for the ellipse  function */
void GFX_DrawFilledEllipse(int centre_x, int centre_y,
               int radius_x, int radius_y,
               unsigned fillcolour)
{
    COLOUR fill;

    GLOBAL_X = centre_x;
    GLOBAL_Y = centre_y;

    fill = GFX_MakeRGB(fillcolour);

    al_draw_filled_ellipse(centre_x, centre_y,
                          radius_x, radius_y,
                          al_map_rgb(fill.r,fill.g,fill.b));
}

/* wrapper for the ellipse  function */
void GFX_DrawEllipse(int centre_x, int centre_y,
               int radius_x, int radius_y,
               int thickness)
{

    GLOBAL_X = centre_x;
    GLOBAL_Y = centre_y;

    al_draw_ellipse(centre_x, centre_y,
                    radius_x, radius_y,
                    al_map_rgb(RGB.r,RGB.g,RGB.b),thickness);
}

/* wrapper for allegro arc -  function
 angles in degrees*/
void GFX_DrawArc(int centre_x, int centre_y, int radius,
         double angle_start, double angle_end, int thickness)
{
    GLOBAL_X = centre_x;
    GLOBAL_Y = centre_y;

    /* convert to degrees */
    angle_start = 2.0*PI*angle_start/360.0;
    angle_end   = 2.0*PI*angle_end/360.0;

    al_draw_arc(centre_x, centre_y, radius, angle_start, angle_end,
                al_map_rgb(RGB.r,RGB.g,RGB.b),thickness);
}



/* wrapper for setbkcolor */
/* sets the global background colour to the chosen colour */
void GFX_SetBackgroundColour(unsigned colour)
{
    BACKGROUND_RGB = GFX_MakeRGB(colour);
}

/* Clears everything */
/* fills current graphics window with the current
background colour and sets the global drawing coordinates
to 0 , 0 */
void GFX_ClearWindow(void)
{
    al_clear_to_color(al_map_rgb(BACKGROUND_RGB.r,BACKGROUND_RGB.g,BACKGROUND_RGB.b));
    GLOBAL_X = 0;
    GLOBAL_Y = 0;
}

/* wrapper for the line function: draw line in current
  global colour (RGB) from one point to another  */
void GFX_DrawLine(int xstart, int ystart, int xend, int yend, int thickness)
{

    GLOBAL_X = xend;
    GLOBAL_Y = yend;

    al_draw_line(xstart, ystart, xend, yend, al_map_rgb(RGB.r,RGB.g,RGB.b),thickness);
}

/* wrapper for lineto function
   same as line except it works from global start position
*/
void GFX_DrawLineTo(int x, int y, int thickness)
{

    al_draw_line(GLOBAL_X, GLOBAL_Y, x, y, al_map_rgb(RGB.r,RGB.g,RGB.b),thickness);

    GLOBAL_X = x;
    GLOBAL_Y = y;
}

/* wrapper for moveto.
   Moves the global drawing position to the user
   choice */
void GFX_MoveTo(int x, int y)
{
     GLOBAL_X = x;
     GLOBAL_Y = y;
}

/* wrapper for outtextxy that writes a string
  in current pen colour to position x, y */
  /* To use this GFX_InitFont() has to be called first */
void GFX_DrawText(int x, int y, const char * text)
{
    al_draw_text(GLOBAL_FONT, al_map_rgb(RGB.r,RGB.g,RGB.b), x, y, ALLEGRO_ALIGN_LEFT, text);
}

/* allows the detection of events */
void GFX_CreateEventQueue(void)
{
    timer = al_create_timer(1/60.0);
    event_queue = al_create_event_queue();
}

void GFX_RegisterEventSource(void)
{
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
}

bool GFX_EventTimer(void)
{
    if (event.type == ALLEGRO_EVENT_TIMER)
        return true;
    else
        return false;
}

/* register a display event source */
void GFX_RegisterDisplayEvents(void)
{
    if (event_queue != NULL)
        al_register_event_source(event_queue, al_get_display_event_source(DISPLAY));
    else
    {
        printf("\nYou need to create an event queue with function: GFX_CreateEventQueue\n");
        exit(0);
    }
}

/* register a mouse event source */
void GFX_RegisterMouseEvents(void)
{
    if (event_queue != NULL)
        al_register_event_source(event_queue, al_get_mouse_event_source());
    else
    {
        printf("\nYou need to create an event queue with function: GFX_CreateEventQueue\n");
        exit(0);
    }
}

void GFX_RegisterKeyboardEvents(void)
{
    if (event_queue != NULL)
        al_register_event_source(event_queue, al_get_keyboard_event_source());
    else
    {
        printf("\nYou need to create an event queue with function: GFX_CreateEventQueue\n");
        exit(0);
    }
}

/* hide the mouse cursor */
void GFX_HideCursor(void)
{
    al_hide_mouse_cursor(DISPLAY);
}

/* if an event return 1 else return 0 */
int GFX_IsEventWaiting(void)
{
    if (al_is_event_queue_empty(event_queue))
        return 0;
    else
        return 1;
}

/* wait until the event queue is non-empty. In other words wait
   until the user makes an event (moves mouse, clicks a mouse button
or close the display */
void GFX_WaitForEvent(void)
{
    al_wait_for_event(event_queue, &event);
}

/* close the display if the user initiates it */
int GFX_IsEventCloseDisplay(void)
{
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	   return 1;
    else
        return 0;
}

/* detect if the mosue position has changed */
int GFX_IsEventMouseMoved(void)
{
    if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* get the mouse coordinates */
int GFX_GetMouseCoordinates(int * x_ptr, int * y_ptr)
{
    if (GFX_IsEventMouseMoved() || GFX_IsEventMouseButton())
    {
        *x_ptr = event.mouse.x;
        *y_ptr = event.mouse.y;
        return 1;
    }
    else
    {
        return 0;
    }

}

/* sends back the mouse button that was pressed */
int GFX_GetMouseButton(int * button_ptr)
{
    if (GFX_IsEventMouseButton())
    {
        *button_ptr = event.mouse.button;
        return 1;
    }
    else
    {
        return 0;
    }
}

/* detect whether a mouse button is down */
int GFX_IsEventMouseButton(void)
{
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* detect if a key on the keyboard is pressed */
int GFX_IsEventKeyDown(void)
{
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* detect if no key on the keyboard is pressed */
int GFX_IsEventKeyUp(void)
{
    if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int GFX_GetKeyPress(int * keypress_ptr)
{
    if ((event.type == ALLEGRO_EVENT_KEY_UP) || (event.type == ALLEGRO_EVENT_KEY_DOWN))
    {
        *keypress_ptr = event.keyboard.keycode;
        return 1;
    }
    else
    {
        return 0;
    }
}

/* install the mouse */
void GFX_InitMouse(void)
{

    if (!al_install_mouse())
    {
        printf("\nIn GFX_InitMouse: al_install_mouse failed\n");
        exit(1);
    }

}

/* uninstall the mouse */
void GFX_CloseMouse(void)
{
    al_uninstall_mouse();
}

/* initialize the keyboard */
void GFX_InitKeyboard(void)
{
    if (!al_install_keyboard())
    {
        printf("\nIn GFX_InitKeyboard: al_install_keyboard failed\n");
        exit(1);
    }
}

void GFX_CloseKeyboard(void)
{
    al_uninstall_keyboard();
}

/* initialize the bitmap variable pointer */
BITMAP GFX_InitBitmap(void)
{
    BITMAP image = NULL;

    return image;
}

/* load the bit map. The name of the file
as a character string is stored in name */
BITMAP GFX_LoadBitmap(char* name)
{
    return al_load_bitmap(name);
}

/* make the colour background of the bitmap
    transparent - note the image background
    must be one colour - i.e. the red green and blue
    pixels must all be the same for the background.
*/
void GFX_MakeImageBGTransparent(BITMAP image, int red, int green, int blue)
{
 	al_convert_mask_to_alpha(image, al_map_rgb(red, green, blue));
}

/* draw the bitmap so its centre is at x, y in the screen */
void GFX_DrawBitmap(BITMAP image, int x, int y)
{
    int imageWidth = 0;
	int imageHeight = 0;

	imageWidth =  al_get_bitmap_width(image);
	imageHeight = al_get_bitmap_height(image);

    al_draw_bitmap(image, x - imageWidth/2, y- imageHeight/2, 0);
}


/* always call to free up memory for bitmaps after use */
void GFX_FreeBitmap(BITMAP image)
{
    al_destroy_bitmap(image);
}


