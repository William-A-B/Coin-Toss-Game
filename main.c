///* This codeblocks project is currently working on windows 10 in codeblocks version 20.03 *///

///* For the purpose of marking and to save time, when you select the options in the game menu, the password is "password" *///


/*  This line allows the compiler to understand the
 *	graphics functions
 */
#include "graphics_lib.h"
#include <math.h>
#include <time.h>
#include <string.h>

///* Structure Definitions *///

/* Struct to contain any set of x,y coordinates */
typedef struct coordinate_struct
{
    int x;
    int y;
}coordinate_t;

/* Struct to contain all the different scores calculated */
typedef struct scores
{
    /* Score variables */
    double total_score;
    double mean_score;
    double max_score;
    double min_score;
    double total_square_scores;
    double mean_square_scores;
    double standard_deviation;
}scores_t;

/* Struct to contain all the variable information needed when playing the game */
typedef struct play_screen_struct
{
    /* Declare a structure for two x and y coordinates from the user */
    coordinate_t user_input_coords;
    coordinate_t target_coords;
    /* Coordinate for holding the xy position for starting the drawing/projectile */
    coordinate_t draw_init_coords;
    /* Declare a structure for two x and y positions to mark the initial projectile position */
    coordinate_t projectile_init_coords;

    char stickman_colour_letter;
    int wind_velocity;

    char wind_text[50];
    char projectile_velocity_text[50];
    char projectile_angle_text[50];

    int projectile_final_x_coord;


    double projectile_angle;
    double projectile_velocity;

}play_screen_t;

/* Constant ints to define the number of rounds in the game
and the number of throws per round */
const int max_throws_round = 3;
const int max_rounds = 2;

/* Number of places displayed on the leaderboard */
const int leaderboard_places = 5;

///* Function header definitions *///

/* Function which shows main menu and allows interaction with the buttons */
void gameMenu(play_screen_t play_screen);

/* Main function which contains the logic to play the game */
void playGame(play_screen_t play_screen, int target_score[max_rounds][max_throws_round]);

/* return the mouse coordinates when the user left clicks */
void getMouseCoordinates(coordinate_t *mouse_click);

/* Causes an action depending on which menu button is clicked on */
bool mouseClickAction(coordinate_t mouse_click, bool *play_clicked);

/* Draw text and wait for click to return to menu */
void menuReturnInstruction(int x_pos, int y_pos);

/* Draw the menu screen to the graphics window */
void drawGameMenu(char stickman_colour_letter);

/* Instructions displayed when the help button is clicked */
void gameInstructions();

/* Function to calculate the coordinate positions from the user input coords
either for drawing the stickman or throwing a projectile */
void calculateCoordinatePositions(coordinate_t input, coordinate_t *output, int draw_or_throw);

/* Function to draw the stickman */
void draw_stick_man(coordinate_t xy_pos);

/* Draw a target to aim at */
void drawTarget(coordinate_t target_pos, char stickman_colour_letter);

/* Check if the stickman is fully displayed when drawn with current coordinates */
int isStickManFullyDisplayed(int x_coordinate, int y_coordinate);

/* Changes the pen colour depending on the colour the user picked */
int stickManColour(unsigned stickman_colour_letter);

/* Calculates a random wind speed between -20 and +20 m/s */
int calculateWindVelocity();

/* Display the wind speed in the graphics window */
void displayWind(int wind_vel, char stickman_colour_letter);

///Extra feature (Not currently in use)
/* Display the velocity (and angle tba) on the play screen based on the mouse pos */
void displayVelocityAngle(double velocity, double angle);

/* Draw a short line to show where the player is currently aiming */
void showTrajectory(coordinate_t mouse_pos, play_screen_t play_screen);

/* Waits for the user to left click
Then calculates a velocity and angle for the projectile based on click coordinates */
void mouseProjectileVelocityAngle(coordinate_t *mouse_click, play_screen_t *play_screen);

/* Draw the coin at x and y positions */
void coinBitmap(int x_pos, int y_pos);

/* Carries out the calculations to throw the projectile and also draws the trajectory */
int throw_projectile(int initial_x_pos, int initial_y_pos, double projectile_velocity, double projectile_angle, int wind_vel);

/* Calculates the statistics at the end of each round */
void calculateRoundStatistics(int round, int total_scores[max_rounds][max_throws_round]);

/* Calculates the statistics for the end of all the rounds and the end of the game
Also displays the scores to the graphics window
And updates the scoreboard with the new highscore */
void calculateTotalStatistics(int total_scores[max_rounds][max_throws_round]);

/* Calculate the score from where the coin landed on the target */
int calculateTotalTargetScore(coordinate_t target_pos, int final_x_pos);

/* Definitions of functions in FileIO.c */
void fileClear();
char *fileReadPassword();


/*Clears the scoreboard if the user requests it */
void resetScoreboard();

/* Called when the user selects options
Requires a password to access options */
void options();

/* Displays the scoreboard on the screen when the user clicks on the score button */
void displayScoreboard();

///* Main Function - Start of running code *///

/* Main function contains the loop for the game to run
And sets up initial conditions */
int main()
{
    /* Struct to contain all variables used when playing the game */
    play_screen_t play_screen;

    /* Variable which is used as a temporary variable to check if the stickman is fully displayed */
    int coordinatesOk;
    coordinatesOk = 0;

    /* Array which contains the scores for each throw and round */
    int target_score[max_rounds][max_throws_round];

    /* Temp variable to store the users input on initial conditions */
    char change_pos;

    /* Loop which initialises the stickman starting coordinates and base colour
    user is asked if they are happy with the initial coordinates
    If not they can select new coordinates
    Then checks if when the stickman is drawn it will be fully shown on the canvas
    If it is, the program continues
    If it is not, the user is asked to enter new coordinates */
    while (coordinatesOk == 0)
    {
        /* Ask the user if they want to change the starting coords */
        printf("Would you like to change the start position of the stickman? (y/n)\n");
        printf("Default (x, y) pos is (100, 850)\n");
        scanf("%c", &change_pos);

        if (change_pos == 'y')
        {
            /* Ask user for the x and y starting positions to draw the stick man
            and store the values in the corresponding variables */
            printf("X coordinate: ");
            scanf("%d", &play_screen.user_input_coords.x);

            printf("Y coordinate: ");
            scanf("%d", &play_screen.user_input_coords.y);

            play_screen.target_coords.y = play_screen.user_input_coords.y;

            /* Checks if the stickman is drawn fully on the canvas by calling the function isStickManFullyDisplayed
            If its not the program asks the user for new coordinates
            Else the temp variable is updated and the while loop ends */
            if (isStickManFullyDisplayed(play_screen.user_input_coords.x, play_screen.user_input_coords.y) == 0)
                printf("Please enter new coordinates, as currently the stick man doesn't fit on the canvas\n\n");
            else
                coordinatesOk = 1;
        }
        /* If the user is happy with the default coords then initialise them */
        else
        {
            play_screen.user_input_coords.x = 100;
            play_screen.user_input_coords.y = 850;
            play_screen.target_coords.y = play_screen.user_input_coords.y;
            coordinatesOk = 1;
        }

    }

    /*Ask user for the first letter of a colour
    and store the value */
    printf("Type the first letter of the colour you would like the Stick Man to be: ");
    scanf(" %c", &play_screen.stickman_colour_letter);

    fflush(stdin);

    /*Open the graphics window with the following pixel dimensions */
    GFX_InitWindow(1720, 968);

    /* Initialises an event queue to detect events */
    GFX_CreateEventQueue();

    /* Loop indefinitely, program is stopped by the game menu */
    while (1)
    {
        /* Main function for the game menu, which allows interaction with the menu */
        gameMenu(play_screen);

        /* Contains the logic and mouse and keyboard events to play the game */
        playGame(play_screen, target_score);

        /*Calculate the final statistics to print at the end */
        calculateTotalStatistics(target_score);

        /* Clear the window so the user can be returned to the main menu */
        GFX_ClearWindow();
    }
    return 0;
}

/* Function which shows main menu and allows interaction with the buttons */
void gameMenu(play_screen_t play_screen)
{
    bool play_clicked = false;

    /* Struct to contain the mouse coordinates from the users click */
    coordinate_t mouse_click;

    /* Draws the game menu onto the graphics display */
    drawGameMenu(play_screen.stickman_colour_letter);

    /* Pauses the event queue to stop new events being added */
    GFX_PauseEventQueue(false);

    while (play_clicked == false)
    {
        /* Gets coordinates of the mouse click */
        getMouseCoordinates(&mouse_click);

        /* Stop mouse clicks being added to event queue */
        GFX_PauseEventQueue(true);

        /* Execute action which was clicked on in the menu */
        /* Redraw game menu after player has left chosen screen */
        if (mouseClickAction(mouse_click, &play_clicked) == true)
        {
            if (play_clicked == false)
                drawGameMenu(play_screen.stickman_colour_letter);
        }
        /* Resume mouse clicks being added to event queue */
        GFX_PauseEventQueue(false);
    }
}

/* Main function which contains the logic to play the game */
void playGame(play_screen_t play_screen, int target_score[max_rounds][max_throws_round])
{
    coordinate_t mouse_click;

    int target_attempt = 0;
    int throw_attempt = 0;

    /* Used for seeding the random number generator */
    time_t t;

    int wind_vel = 0;

    GFX_InitKeyboard();
    GFX_RegisterKeyboardEvents();

    /* Call the function stickManColour to work out the corresponding colour from the character entered by the user
    Then update the pen colour to match the chosen colour */
    GFX_SetColour(stickManColour(play_screen.stickman_colour_letter));


    /* Calculate coordinates for drawing the stickman*/
    calculateCoordinatePositions(play_screen.user_input_coords, &play_screen.draw_init_coords, 0);


    /* Call draw stick man function
    This draws the stickman on the canvas */
    draw_stick_man(play_screen.draw_init_coords);
    GFX_SetColour(stickManColour(play_screen.stickman_colour_letter));

    /* Sets the seed for the random number based off the time */
    srand((unsigned) time(&t));

    /* Gets a random number between 400 and 1520 */
    play_screen.target_coords.x = GFX_RandNumber(400, 1520);

    drawTarget(play_screen.target_coords, play_screen.stickman_colour_letter);

    /*Get a random wind speed for the round */
    play_screen.wind_velocity = calculateWindVelocity();
    displayWind(play_screen.wind_velocity, play_screen.stickman_colour_letter);

    /* Update the canvas to make sure all the lines are drawn and shown */
    GFX_UpdateDisplay();

    /* Do - While loop to loop the playing of the game until the number of target attempts reaches the number of rounds set
    Contains logic which allows the user to choose one of three options:
    Move the stickman left or right or throw a projectile
    Carries out logic after throwing a coin to calculate a score and check conditions:
    Which throw is the user on, which round is the user on, and increments them correspondingly */
    do
    {
        GFX_SetColour(stickManColour(play_screen.stickman_colour_letter));

        int key_press;
        int Fired = false;

        /* Loops until the user presses 'f' to throw a coin */
        while (Fired == false)
        {
            GFX_WaitForEvent();

            if (GFX_IsEventKeyDown())
            {
                GFX_GetKeyPress(&key_press);

                if (key_press == ALLEGRO_KEY_A)
                {
                    /* Clear the window
                    A new stickman position is then set 50 pixels to the left */
                    GFX_ClearWindow();
                    play_screen.user_input_coords.x = play_screen.user_input_coords.x - 50;

                    /* Calculate coordinates for drawing the stickman*/
                    calculateCoordinatePositions(play_screen.user_input_coords, &play_screen.draw_init_coords, 0);

                    /* Call draw stick man function
                    This draws the stickman on the canvas */
                    draw_stick_man(play_screen.draw_init_coords);
                    drawTarget(play_screen.target_coords, play_screen.stickman_colour_letter);
                    displayWind(play_screen.wind_velocity, play_screen.stickman_colour_letter);

                    GFX_UpdateDisplay();
                }


                else if (key_press == ALLEGRO_KEY_D)
                {
                    /* Clear the window
                    A new stickman position is then set 50 pixels to the left */
                    GFX_ClearWindow();
                    play_screen.user_input_coords.x = play_screen.user_input_coords.x + 50;

                    /* Calculate coordinates for drawing the stickman*/
                    calculateCoordinatePositions(play_screen.user_input_coords, &play_screen.draw_init_coords, 0);

                    /* Call draw stick man function
                    This draws the stickman on the canvas */
                    draw_stick_man(play_screen.draw_init_coords);
                    drawTarget(play_screen.target_coords, play_screen.stickman_colour_letter);
                    displayWind(play_screen.wind_velocity, play_screen.stickman_colour_letter);

                    GFX_UpdateDisplay();
                }


                else if (key_press == ALLEGRO_KEY_F)
                {
                    /* End the current while loop to throw a coin */
                    Fired = true;
                }

            }
        }


        /* The initial positions for the projectile are calculated */
        play_screen.projectile_init_coords.x = play_screen.projectile_init_coords.y = 0;
        calculateCoordinatePositions(play_screen.user_input_coords, &play_screen.projectile_init_coords, 1);

        /* Waits for the user to left click
        Then calculates a velocity and angle for the projectile based on click coordinates */
        mouseProjectileVelocityAngle(&mouse_click, &play_screen);;

        /* Iterates over the parabola of the projectile and draws the coin onto the graphics window at set spaces along the trajectory
        Returns the final x coordinate of where the coin landed */
        play_screen.projectile_final_x_coord = throw_projectile(play_screen.projectile_init_coords.x, play_screen.projectile_init_coords.y,
                                                                play_screen.projectile_velocity, play_screen.projectile_angle, wind_vel);


        /* Add scores to the array for each round and check if the scores should be added to the next round
        Also calculate the scores and display them on the graphics window at the end of all the rounds */
        if (target_attempt < max_rounds)
        {

            target_score[target_attempt][throw_attempt] = calculateTotalTargetScore(play_screen.target_coords, play_screen.projectile_final_x_coord);

            throw_attempt++;

            if (throw_attempt == max_throws_round)
            {
                /* Calculates round statistics based on the scores from the user for the last played round
                Currently commented out so uncomment to add this additional feature */
                calculateRoundStatistics(target_attempt, target_score);

                /* Increments the round number i.e. target attempt and resets the throw counter */
                target_attempt++;
                throw_attempt = 0;

                GFX_ClearWindow();

                /* Calculate coordinates for drawing the stickman*/
                calculateCoordinatePositions(play_screen.user_input_coords, &play_screen.draw_init_coords, 0);

                /* Call draw stick man function
                This draws the stickman on the canvas */
                draw_stick_man(play_screen.draw_init_coords);

                /* Calculates a new wind speed for the start of each round */
                play_screen.wind_velocity = calculateWindVelocity();
                displayWind(play_screen.wind_velocity, play_screen.stickman_colour_letter);

                /* Calculate a random x position to draw the target */
                play_screen.target_coords.x = GFX_RandNumber(350, 1720);
                drawTarget(play_screen.target_coords, play_screen.stickman_colour_letter);
            }
        }

        /* Else set the target attempt equal to the max rounds so that the loop ends and finishes*/
        else
            target_attempt = max_rounds;

    } while (target_attempt < max_rounds);
}

/* return the mouse coordinates when the user left clicks */
void getMouseCoordinates(coordinate_t *mouse_click)
{
    int click = false;
    int mouse_button;

    GFX_InitMouse();
    GFX_RegisterMouseEvents();

    /* Loop until left mouse button is clicked */
    while (click == false)
    {
        GFX_WaitForEvent();

        if (GFX_IsEventMouseButton())
        {
            GFX_GetMouseButton(&mouse_button);

            if (mouse_button == MOUSE_BUTTON_LEFT)
            {
                /* Store mouse coordinates in the struct mouse_click */
                GFX_GetMouseCoordinates(&mouse_click->x, &mouse_click->y);
                click = true;
            }
        }
    }
}

/* Causes an action depending on which menu button is clicked on */
bool mouseClickAction(coordinate_t mouse_click, bool *play_clicked)
{
    /* Play button clicked */
    if (mouse_click.x > 767 && mouse_click.x < 954 && mouse_click.y > 279 && mouse_click.y < 351)
    {
        *play_clicked = true;
        GFX_ClearWindow();
    }
    /* Options button clicked */
    else if (mouse_click.x > 767 && mouse_click.x < 954 && mouse_click.y > 371 && mouse_click.y < 445)
    {
        GFX_ClearWindow();
        options();
    }
    /* Score button clicked */
    else if (mouse_click.x > 767 && mouse_click.x < 954 && mouse_click.y > 467 && mouse_click.y < 538)
    {
        displayScoreboard();
    }
    /* Help button clicked */
    else if (mouse_click.x > 767 && mouse_click.x < 954 && mouse_click.y > 560 && mouse_click.y < 634)
    {
        GFX_ClearWindow();
        gameInstructions();
    }
    /* Quit button clicked */
    else if (mouse_click.x > 767 && mouse_click.x < 954 && mouse_click.y > 655 && mouse_click.y < 727)
    {
        GFX_CloseWindow();
        exit(0);
    }
    else
    {
        /* False, no button was clicked */
        return false;
    }

    /* True, a button was clicked */
    return true;
}

/* Draw text and wait for click to return to menu */
void menuReturnInstruction(int x_pos, int y_pos)
{
    char return_instruct[50];
    bool click = false;
    int mouse_button;

    /* Text which will be displayed on the graphics window */
    sprintf(return_instruct, "Click anywhere on the screen to return");
    GFX_DrawText(x_pos, y_pos, return_instruct);
    GFX_UpdateDisplay();

    /* Resume mouse clicks being added to event queue */
    GFX_PauseEventQueue(false);

    GFX_InitMouse();
    GFX_RegisterMouseEvents();

    /* Loop while waiting for left mouse button click */
    while (click == false)
    {
        GFX_WaitForEvent();

        if (GFX_IsEventMouseButton())
        {
            GFX_GetMouseButton(&mouse_button);

            if (mouse_button == MOUSE_BUTTON_LEFT)
            {
                /* Left click is true, so returning from current screen to menu screen now occurs */
                click = true;
                /* Stop mouse clicks being added to event queue */
                GFX_PauseEventQueue(true);
            }
        }
    }
}

/* Draw the menu screen to the graphics window */
void drawGameMenu(char stickman_colour_letter)
{
    /* Game Title */
    char title[30] = "Coin Toss Savings Game";

    GFX_InitFont(36);
    GFX_SetColour(WHITE);
    GFX_DrawText(685, 150, title);

    /* Initialise Bitmap Images, this must go after GFX_InitWindow but before any other BITMAP functions */
    GFX_InitBitmap();

    /* Load the image into a variable of type BITMAP. The image should be stored in the same location as the .cbp project file. */
    BITMAP image = GFX_LoadBitmap("images/menu_buttons.png");

    /* Draw the image to the window, the x,y position is the centre of the image. */
    GFX_DrawBitmap(image, 860, 500);
    GFX_UpdateDisplay();

    /* Free the memory used by the image once finished with it */
    GFX_FreeBitmap(image);

    GFX_ClearWindow();

    /* Reset colour to be the initial stickman colour */
    GFX_SetColour(stickManColour(stickman_colour_letter));
}

/* Instructions displayed when the help button is clicked */
void gameInstructions()
{
    /* Stores text in variables to draw later */
    GFX_InitFont(24);
    GFX_SetColour(WHITE);
    char line1[100] = "Welcome to the stickman digital coin toss savings game!";
    char line2[100];
    sprintf(line2, "This game has %d rounds with %d attempts per round.", max_rounds, max_throws_round);
    char line3[100] = "The aim of the game:";
    char line4[100] = "Land your coin within the target area";
    char line5[100] = "The closer to the centre of the target you are, the higher the money amount gained";
    char line6[100] = "'a' and 'd' will move the stickman left and right";
    char line7[100] = "'f' will allow you to control the angle and velocity of the coin toss by moving your mouse";
    char line8[100] = "Left click to throw the coin when happy with your position";

    /* Draws the text onto the graphics window */
    GFX_DrawText(520, 150, line1);
    GFX_DrawText(520, 180, line2);
    GFX_DrawText(520, 210, line3);
    GFX_DrawText(520, 240, line4);
    GFX_DrawText(520, 270, line5);
    GFX_DrawText(520, 300, line6);
    GFX_DrawText(520, 330, line7);
    GFX_DrawText(520, 360, line8);

    GFX_UpdateDisplay();

    /* Allows the user to return to the main menu */
    menuReturnInstruction(580, 700);
    GFX_ClearWindow();
}

/* Called when the user selects options
Requires a password to access options */
void options()
{
    /* Initialise variables */
    char entered_password[100];
    int pw_correct;
    char *password;

    /* Tell the user to view the terminal to access options */
    GFX_InitFont(24);
    GFX_SetColour(WHITE);
    char message[50] = "Please view the terminal to continue";

    GFX_DrawText(520, 150, message);
    GFX_UpdateDisplay();

    /* Set the password var to be the password stored in the file */
    password = fileReadPassword();

    /* Ask the user to enter the password
    Checks if it is correct, if not, try again
    If correct go to function to reset the scoreboard */
    do
    {
        printf("\nEnter password to continue:\n");
        scanf("%s", entered_password);
        pw_correct = strcmp(entered_password, password);

        if (pw_correct == 0)
        {
            resetScoreboard();
        }
        else
        {
            printf("\nThe password you entered is incorrect\n");
        }


    }while(pw_correct != 0);
    GFX_ClearWindow();
}


/*Clears the scoreboard if the user requests it */
void resetScoreboard()
{
    char scoreboardReset;
    printf("\nWould you like to clear the scoreboard? (y/n)\n");
    scanf(" %c", &scoreboardReset);
    if (scoreboardReset == 'y')
    {
        fileClear();
    }
}

/* Function to calculate the coordinate positions from the user input coords
either for drawing the stickman or throwing a projectile */
void calculateCoordinatePositions(coordinate_t input, coordinate_t *output, int draw_or_throw)
{
    /* If drawing the stickman */
    if (draw_or_throw == 0)
    {
        output->x = input.x + 70;
        output->y = input.y - 310;
    }
    /* If throwing the projectile */
    else if (draw_or_throw == 1)
    {
        output->x = input.x + 140;
        output->y = input.y - 230;
    }
}

/* Function to draw the stick man and ground */
void draw_stick_man(coordinate_t xy_pos)
{
    GFX_DrawCircle(xy_pos.x, xy_pos.y, 40, 4);                        /* Draw head */
    GFX_DrawLine(xy_pos.x, xy_pos.y+40, xy_pos.x, xy_pos.y+200, 4);         /* Draw body */
    GFX_DrawLine(xy_pos.x-70, xy_pos.y+80, xy_pos.x+70, xy_pos.y+80, 4);    /* Draw arms */
    GFX_DrawLine(xy_pos.x, xy_pos.y+200, xy_pos.x-70, xy_pos.y+310, 4);     /* Draw left leg */
    GFX_DrawLine(xy_pos.x, xy_pos.y+200, xy_pos.x+70, xy_pos.y+310, 4);     /* Draw right leg */
    GFX_SetColour(GREEN);
    GFX_DrawLine(0, xy_pos.y+310, 1720, xy_pos.y+310, 8);             /* Draw ground */

    GFX_UpdateDisplay();
}

/* Draw a target to aim at */
void drawTarget(coordinate_t target_pos, char stickman_colour_letter)
{
    /* Allegro functions used here so that the colour can be specified for each section of the target more easily
    5 rectangles layered on top of each other to recreate the look of an archery target along the ground */
    al_draw_filled_rectangle(target_pos.x-100, target_pos.y-6, target_pos.x+100, target_pos.y+6, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(target_pos.x-80, target_pos.y-6, target_pos.x+80, target_pos.y+6, al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(target_pos.x-60, target_pos.y-6, target_pos.x+60, target_pos.y+6, al_map_rgb(3, 242, 255));
    al_draw_filled_rectangle(target_pos.x-40, target_pos.y-6, target_pos.x+40, target_pos.y+6, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(target_pos.x-20, target_pos.y-6, target_pos.x+20, target_pos.y+6, al_map_rgb(255, 247, 0));

    GFX_UpdateDisplay();
}

/* Check if the stickman is fully displayed when drawn with current coordinates */
int isStickManFullyDisplayed(int x_coordinate, int y_coordinate)
{
     /* Declare two variables for the x and y positions */
    int x_pos;
    int y_pos;

    /* Calculate x and y positions of the centre of the head from the x and y coordinates entered */
    x_pos = x_coordinate + 70;
    y_pos = y_coordinate - 310;

    /* Check the stickman doesn't draw outside the graphics window */
    if (y_pos - 40 <= 0)
    {
        return 0;
    }
    else if (x_pos - 70 <= 0)
    {
        return 0;
    }
    else if (x_pos + 70 > 300)
    {
        return 0;
    }
    else if (y_pos + 310 > 968)
    {
        return 0;
    }
    return 1;
}

/* Changes the pen colour depending on the colour the user picked */
int stickManColour(unsigned stickman_colour_letter)
{
    /* Defaults to white if no selection of an existing colour is made */
    int stickman_colour = WHITE;

    if (stickman_colour_letter == 'r')
        stickman_colour = RED;
    else if (stickman_colour_letter == 'g')
        stickman_colour = GREEN;
    else if (stickman_colour_letter == 'b')
        stickman_colour = BLUE;
    else if (stickman_colour_letter == 'y')
        stickman_colour = YELLOW;
    else if (stickman_colour_letter == 'm')
        stickman_colour = MAGENTA;
    else if (stickman_colour_letter == 'c')
        stickman_colour = CYAN;
    else
        stickman_colour = WHITE;

    return stickman_colour;
}

/* Calculates a random wind speed between -20 and +20 m/s */
int calculateWindVelocity()
{
    time_t t;
    int wind_vel = 0;

    /* Sets the seed for the random number based off the time */
    srand((unsigned) time(&t));

    /* Calculate a random wind speed based off the range -10 to +10 */
    wind_vel = GFX_RandNumber(-20, 20);

    return wind_vel;
}

/* Display the wind speed in the graphics window */
void displayWind(int wind_vel, char stickman_colour_letter)
{
    char text[26];
    sprintf(text, "Wind Speed = %d m/s", wind_vel);


    GFX_InitFont(24);
    GFX_SetColour(WHITE);
    GFX_DrawText(100, 100, text);
    GFX_UpdateDisplay();
    GFX_SetColour(stickManColour(stickman_colour_letter));
}


///Extra function that can be added for the velocity to be displayed on the playing screen
//void displayVelocityAngle(double velocity, double angle)
//{
//    char text[35];
//    sprintf(text, "Velocity = %lf m/s", velocity);
//
//
//    GFX_InitFont(24);
//    al_map_rgb(255, 255, 255);
//    GFX_DrawText(400, 100, text);
//    GFX_UpdateDisplay();
//}

/* Draw a short line to show where the player is currently aiming */
void showTrajectory(coordinate_t mouse_pos, play_screen_t play_screen)
{
    /* Initialise variables */
    coordinate_t end_of_trajectory;
    int trajectory_len = 0;
    int x_dist = 0;
    int y_dist = 0;

    /* Clear window from last throw
    And redraw stickman target and wind speed */
    GFX_ClearWindow();
    draw_stick_man(play_screen.draw_init_coords);
    drawTarget(play_screen.target_coords, play_screen.stickman_colour_letter);
    displayWind(play_screen.wind_velocity, play_screen.stickman_colour_letter);

    /* Check mouse position against the starting projectile position
    Makes sure the trajectory line doesn't draw in the wrong place */
    if (mouse_pos.x < play_screen.projectile_init_coords.x)
        mouse_pos.x = play_screen.projectile_init_coords.x;
    if (mouse_pos.y > play_screen.projectile_init_coords.y)
        mouse_pos.y = play_screen.projectile_init_coords.y;

    /* Calculate distance to mouse pos */
    x_dist = mouse_pos.x - play_screen.projectile_init_coords.x;
    y_dist = mouse_pos.y - play_screen.projectile_init_coords.y;

    /* Calculates the length of the trajectory line */
    trajectory_len = (int)sqrt((double)(x_dist*x_dist+y_dist*y_dist));

    if (trajectory_len > 130)
    {
        /* Scale distances */
        x_dist = x_dist*130/trajectory_len;
        y_dist = y_dist*130/trajectory_len;
    }

    /* Modify end of trajectory position */
    end_of_trajectory.x = play_screen.projectile_init_coords.x + x_dist;
    end_of_trajectory.y = play_screen.projectile_init_coords.y + y_dist;

    /* Draw line to graphics window */
    GFX_DrawLine(play_screen.projectile_init_coords.x, play_screen.projectile_init_coords.y, end_of_trajectory.x, end_of_trajectory.y, 2);
    GFX_UpdateDisplay();
    GFX_PauseFor(50);
}

/* Waits for the user to left click
Then calculates a velocity and angle for the projectile based on click coordinates */
void mouseProjectileVelocityAngle(coordinate_t *mouse_click, play_screen_t *play_screen)
{
    coordinate_t mouse_pos;
    mouse_pos.x = 0;
    mouse_pos.y = 0;

    unsigned int click = false;
    int mouse_button;

    GFX_InitMouse();
    GFX_RegisterMouseEvents();

    while(click == false)
    {
        /* Pause the program until an event occurs */
        GFX_WaitForEvent();

        /* Once an event has occurred, check if it is
           someone moving the mouse */
        if (GFX_IsEventMouseMoved())
        {
            /* Stop mouse movements being added to event queue */
            GFX_PauseEventQueue(true);

            /* Get the position of the mouse, store the
               values in mouseX and mouseY */
            GFX_GetMouseCoordinates(&mouse_pos.x, &mouse_pos.y);
            showTrajectory(mouse_pos, *play_screen);

            /* Resume mouse movements being added to event queue */
            GFX_PauseEventQueue(false);
        }
        else if (GFX_IsEventMouseButton())
        {
            GFX_GetMouseButton(&mouse_button);

            if (mouse_button == MOUSE_BUTTON_LEFT)
            {
                GFX_GetMouseCoordinates(&mouse_click->x, &mouse_click->y);

                ///Debug
                /* Prints the mouse position when you click to fire the projectile */
                //printf("\nx:%d", mouse_click->x);
                //printf("\ny:%d\n", mouse_click->y);

                /* Only calculate projectile values if mouse coords are valid */
                if (mouse_click->x > play_screen->projectile_init_coords.x &&
                    mouse_click->y < play_screen->projectile_init_coords.y)
                {
                    //*projectile_angle = atan((projectile_init_pos.y-mouse_click->y)/(mouse_pos.x-projectile_init_pos.x));
                    play_screen->projectile_angle = atan2((play_screen->projectile_init_coords.y - mouse_pos.y),(mouse_pos.x - play_screen->projectile_init_coords.x));
                    play_screen->projectile_velocity = sqrt((pow((mouse_pos.x - play_screen->projectile_init_coords.x), 2)) + (pow((play_screen->projectile_init_coords.y - mouse_pos.y), 2)))/5;

                    /* Test line to draw from hand to mouse click */
                    //GFX_DrawLine(play_screen->projectile_init_coords.x, play_screen->projectile_init_coords.y, mouse_click->x, mouse_click->y, 1);
                    GFX_UpdateDisplay();
                    click = true;
                }
            }
        }
    }
}

/* Draw the coin at x and y positions */
void coinBitmap(int x_pos, int y_pos)
{
    /* Initialise Bitmap Images, this must go after GFX_InitWindow but before any other BITMAP functions */
    GFX_InitBitmap();

    /* Load the image into a variable of type BITMAP. The image should be stored in the same location as the .cbp project file. */
    BITMAP coin = GFX_LoadBitmap("images/coin.png");

    /* Draw the coin to the window, the x,y position is the centre of the image. */
    GFX_DrawBitmap(coin, x_pos, y_pos);
    GFX_UpdateDisplay();

    /* You need to manually free the memory used by the image once you are done with it */
    GFX_FreeBitmap(coin);
}

/* Carries out the calculations to throw the projectile and also draws the trajectory */
int throw_projectile(int initial_x_pos, int initial_y_pos, double projectile_velocity, double projectile_angle, int wind_vel)
{
    /* Define variables used */
    double time;
    float gravity = 9.81;
    int x_position;
    int y_position;

    /* Variables which store the velocity of the projectile in the x and y direction */
    double vel_x;
    double vel_y;

    int iteration = 0;

    /* Move to the initial throwing position */
    GFX_MoveTo(initial_x_pos, initial_y_pos);

    /* Set drawing x_postion equal to the initial position */
    x_position = initial_x_pos;

    /* Calculate the velocity in the x and y direction from the velocity and angle */
    vel_x = projectile_velocity * cos((projectile_angle));
    vel_y = projectile_velocity * sin((projectile_angle));

    vel_x = vel_x + wind_vel;

    /* While the y position is above the ground level calculate a parabola to draw the projectile */
    while (y_position <= initial_y_pos + 230)
    {
        time = (x_position - initial_x_pos) / vel_x;
        y_position = initial_y_pos - (vel_y * time) + (gravity * time * time)/2;

        /* Draw projectile line
        Iterates over the parabola calculated based on newtons equations of motion */
        //GFX_DrawLineTo(x_position, y_position, 3);
        if (iteration % 50 == 1 || y_position == initial_y_pos + 229)
        {
            coinBitmap(x_position, y_position);
        }
        x_position = x_position + 1;
        iteration++;
        GFX_UpdateDisplay();
    }

    /* Update display and return x position */
    GFX_UpdateDisplay();

    return x_position;
}

/* Calculate the statistics for the end of each round */
void calculateRoundStatistics(int round, int total_scores[max_rounds][max_throws_round])
{
///Uncomment to enable round statistics to be calculated
//    /* Create a struct to contain the round scores */
//    scores_t rScores;
//
//    rScores.total_score = 0;
//    rScores.mean_score = 0;
//    rScores.max_score = 0;
//    rScores.min_score = 0;
//    rScores.total_square_scores = 0;
//    rScores.mean_square_scores = 0;
//    rScores.standard_deviation = 0;
//
//    /* For loop which calculates the total score at the end of the round */
//    for (int i = 0; i < max_throws_round; i++)
//    {
//        rScores.total_score = rScores.total_score + total_scores[round][i];
//    }
//
//    /* calculate the mean score from the total score and max throws per round */
//    rScores.mean_score = rScores.total_score/max_throws_round;
//
//    /* Initialize maximum element */
//    int max = total_scores[round][0];
//
//    /* Traverse array elements from second and
//    compare every element with current max */
//    for (int i = 1; i < max_throws_round; i++)
//    {
//        if (total_scores[round][i] > max)
//        {
//            max = total_scores[round][i];
//        }
//    }
//
//    /* The max distance thrown away from the target */
//    rScores.max_score = max;
//
//    /* Initialize minimum element */
//    int min = total_scores[round][0];
//
//    /* Traverse array elements from second and
//    compare every element with current max */
//    for (int i = 1; i < max_throws_round; i++)
//    {
//        if (total_scores[round][i] < min)
//        {
//            min = total_scores[round][i];
//        }
//    }
//
//    /* The min distance thrown away from the target */
//    rScores.min_score = min;
//
//    /* For loop to calculate the square of each score in the round */
//    for (int i = 0; i < max_throws_round; i++)
//    {
//        rScores.total_square_scores = rScores.total_square_scores + pow(total_scores[round][i], 2);
//    }
//
//    /* Calculations to work out the standard deviation */
//    rScores.mean_square_scores = rScores.total_square_scores/max_throws_round;
//
//    rScores.standard_deviation = sqrt(rScores.mean_square_scores - pow(rScores.mean_score, 2));
//
//    /* Print the statistics for the round */
//    printf("\nYour total score for this round is: %lf\n\n", rScores.total_score);
//    printf("Your mean distance of your throws is: %lf\n\n", rScores.mean_score);
//    printf("Your maximum distance thrown is: %lf\n\n", rScores.max_score);
//    printf("Your minimum distance thrown is: %lf\n\n", rScores.min_score);
//    printf("The standard deviation of your scores is: %lf\n\n", rScores.standard_deviation);


    ///Debug
    /* Writes the round scores to a CSV file which is then printed to the terminal.
    Can be used to check the scores you got each round if the total score doesn't make sense*/
    //fileWriteScore(rScores.total_score, rScores.mean_score, rScores.max_score, rScores.min_score, rScores.standard_deviation);
    //fileReadScore();
}

/* Calculates the statistics for the end of all the rounds and the end of the game
Also displays the scores to the graphics window
And updates the scoreboard with the new highscore */
void calculateTotalStatistics(int total_scores[max_rounds][max_throws_round])
{
    /* Create a struct to contain the round scores */
    scores_t tScores;

    tScores.total_score = 0;
    tScores.mean_score = 0;
    tScores.max_score = 0;
    tScores.min_score = 0;
    tScores.total_square_scores = 0;
    tScores.mean_square_scores = 0;
    tScores.standard_deviation = 0;

    char final_score[30];
    char total_score[100];
    char mean_dis[100];
    char max_dis[100];
    char min_dis[100];
    char stand_dev[100];

    /* For loop which calculates the total score at the end of the game */
    for (int i = 0; i < max_rounds; i++)
    {
        for (int j = 0; j < max_throws_round; j++)
        {
            tScores.total_score = tScores.total_score + total_scores[i][j];
        }
    }

    /* calculate the mean score from the total score and max throws over the whole game */
    tScores.mean_score = tScores.total_score/(max_rounds*max_throws_round);

    /* Initialize maximum element */
    int max = total_scores[0][0];

    /* Traverse array elements from second and
    compare every element with current max */
    for (int i = 0; i < max_rounds; i++)
    {
        for (int j = 0; j < max_throws_round; j++)
        {
            if (total_scores[i][j] > max)
            {
                max = total_scores[i][j];
            }
        }
    }

    /* The max distance thrown away from the target */
    tScores.max_score = max;

    /* Initialize minimum element */
    int min = total_scores[0][0];

    /* Traverse array elements from second and
    compare every element with current max */
    for (int i = 0; i < max_rounds; i++)
    {
        for (int j = 0; j < max_throws_round; j++)
        {
            if (total_scores[i][j] < min)
            {
                min = total_scores[i][j];
            }
        }
    }

    /* The min distance thrown away from the target */
    tScores.min_score = min;

    /* For loop to calculate the square of each score in the whole game */
    for (int i = 0; i < max_rounds; i++)
    {
        for (int j = 0; j < max_throws_round; j++)
        {
            tScores.total_square_scores = tScores.total_square_scores + pow(total_scores[i][j], 2);
        }
    }

    /* Calculations to work out the standard deviation */
    tScores.mean_square_scores = tScores.total_square_scores/(max_rounds*max_throws_round);

    tScores.standard_deviation = sqrt(tScores.mean_square_scores - pow(tScores.mean_score, 2));

    /* Print the statistics for the round */
    sprintf(final_score, "Final game scores!");
    sprintf(total_score, "Your total score for the whole game is: %.0lf GBP", tScores.total_score);
    sprintf(mean_dis, "Your mean score of all your throws is: %.2lf GBP", tScores.mean_score);
    sprintf(max_dis, "Your maximum score hit is: %.0lf GBP", tScores.max_score);
    sprintf(min_dis, "Your minimum score hit is: %.0lf GBP", tScores.min_score);
    sprintf(stand_dev, "The standard deviation of all your scores is: %.2lf GBP", tScores.standard_deviation);


    GFX_InitFont(24);
    GFX_SetColour(WHITE);

    /* Draw to graphics window */
    GFX_DrawText(1000, 200, final_score);
    GFX_DrawText(1000, 230, total_score);
    GFX_DrawText(1000, 260, mean_dis);
    GFX_DrawText(1000, 290, max_dis);
    GFX_DrawText(1000, 320, min_dis);
    GFX_DrawText(1000, 350, stand_dev);
    GFX_UpdateDisplay();



    /* Logic to check compare new scores with the current leaderboard
    Goes down through the leaderboard, compares each value
    If its bigger, it goes in that place and shifts the rest down by one
    If its smaller, it moves to the next value and repeats */
    char ldbd_position[100];

    char score[100];

    int ldbd_pos;
    double ldbd_score;

    double leaderboard[leaderboard_places];
    double new_leaderboard[leaderboard_places];

    int i = 0;
    int j = 0;
    int p = 0;
    int k = 0;

    for (int i = 0; i < leaderboard_places; i++)
    {
        leaderboard[i] = 0;
        new_leaderboard[i] = 0;
    }
    i = 0;

    /* Get values from leaderboard file */
    FILE *fp;
    fp = fopen("files/leaderboard.txt", "r");

    if (fp != NULL)
    {
        fscanf(fp, "%s,%s\n", ldbd_position, score);

        while(!feof(fp))
        {
            fscanf(fp, "%d,%lf\n", &ldbd_pos, &ldbd_score);
            leaderboard[i] = ldbd_score;
            i++;
        }

        fclose(fp);
    }

    for (int j = 0; j < leaderboard_places; j++)
    {
        p++;
        if (leaderboard[j] > tScores.total_score)
        {
            new_leaderboard[j] = leaderboard[j];
        }
        else if (leaderboard[j] <= tScores.total_score)
        {
            new_leaderboard[j] = tScores.total_score;

            break;
        }
    }

    for (int j = p; j < leaderboard_places; j++)
    {
        new_leaderboard[j] = leaderboard[j-1];
    }


    /* Write new values to leaderboard file */
    FILE *fp1;

	fp1 = fopen("files/leaderboard.txt", "w");
	fprintf(fp1, "position,score\n");

	for (int k = 0; k < leaderboard_places; k++)
    {
        fprintf(fp1, "%d,%lf\n", k+1, new_leaderboard[k]);
    }

	fclose(fp1);

	/* Allows the user to return to the menu after the game has finished */
    menuReturnInstruction(1000, 400);
}

/* Calculate the score from where the coin landed on the target */
int calculateTotalTargetScore(coordinate_t target_pos, int final_x_pos)
{
    int distance_centre = abs(target_pos.x - final_x_pos);

    int target_score = 0;

    distance_centre = abs((distance_centre-1)/10);

    target_score = 10-distance_centre;

    /* Ensure target score is never less than 0 */
    if (target_score < 0)
        target_score = 0;

    return target_score;
}

/* Displays the scoreboard on the screen when the user clicks on the score button */
void displayScoreboard()
{
    char ldbd_position[100];
    char total_score[100];
    int ldbd_pos;
    double ldbd_score;
    char positions[30];
    int y_pos_draw = 150;


    GFX_InitFont(24);
    GFX_SetColour(WHITE);

    /* Reads the leaderboard scores and stores in a variable to be displayed */
    FILE *fp2;
    fp2 = fopen("files/leaderboard.txt", "r");

    /* Conditional to make sure the text file exists */
    if (fp2 != NULL)
    {
        fscanf(fp2, "%s,%s\n", ldbd_position, total_score);

        while(!feof(fp2))
        {
            fscanf(fp2, "%d,%lf\n", &ldbd_pos, &ldbd_score);
            sprintf(positions, "%d -- %.0lf GBP", ldbd_pos, ldbd_score);
            GFX_DrawText(520, y_pos_draw, positions);
            y_pos_draw = y_pos_draw + 30;
        }
        fclose(fp2);
    }
    else
    {
        sprintf(positions, "Leaderboard is empty");
        GFX_DrawText(580, 150, positions);
    }


    GFX_UpdateDisplay();

    /* Allows the user to return to the menu by left clicking */
    menuReturnInstruction(580, 700);

    GFX_ClearWindow();
}
