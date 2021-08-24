// This is the main file for the game logic and function

#include "game.h"
#include "Framework\console.h"
#include "Fighter.h"
#include "Wizard.h"
#include "Rogue.h"
#include "Cleric.h"
#include "Skeleton.h"
#include "Inventory.h"
#include "Items.h"
#include "HealingItems.h"
#include "EmptyClass.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <windows.ui.composition.scenes.h>


double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_MENUSCREEN; // initial state

//random encounter variables
int RandomRate;
float RandomDelay;

//pause menu variable
bool timescale = true;

//classes
Class* PreviousClass; //used to denote the end of the round
Class* CurrentClass;
Class* Target[4];
Class* PlayerParty[4];
Class* EnemyParty[4];

//creating the player inventory, the shop inventory and the items in it
Inventory PlayerInventory;
Inventory ShopInventory;
Items* GoldApple = new HealingItems("Gold Apple", 4, 8);
Items* Bandage = new HealingItems("Bandage", 1, 2);


//turn count for battles
int TurnCount;
int CurrentTurn;

//Selection for battles
int EffectSelect;
int TargetIndex;

//coordinates of player when battle is entered
int PlayerTempCoordX;
int PlayerTempCoordY;

//player action indicator
enum PlayerActions
{
    Main,
    Attack,
    Special,
    Skill,
    Item,
    Select,
    FSelect
};
int Action;

// Console object
Console g_Console(150, 30, "SP1 Framework");
COORD cb;
//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;    

    // sets the initial state for the game
    g_eGameState = S_MENUSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);

    //init random encounter variables
    RandomRate = 4; //how to get: denominator of rate - 1 (e.g. 1/5 = 20% so rate is 4 since 5-1 = 4)
    RandomDelay = 1;
    
    //Initialize the Classes
    CurrentClass = nullptr;
    for (int i = 0; i < 4; i++)
    {
        Target[i] = nullptr;
    }
    for (int i = 0; i < 4; i++)
    {
        EnemyParty[i] = nullptr;
    }

    for (int i = 0; i < 4; i++)
    {
        PlayerParty[i] = nullptr;
    }
    //is temporary for testing purposes
    PlayerParty[0] = new Fighter;
    PlayerParty[1] = new Rogue;
    PlayerParty[2] = new Cleric;
    PlayerParty[3] = new Wizard;

    //Adds items and gold to the player and shop inventories
    GoldApple->SetDescription(": Heals 8 hp");
    ShopInventory.AddItem(GoldApple);
    ShopInventory.AddItem(Bandage);
    for (int i = 0; i < 10; i++)
    {
        PlayerInventory.AddItem(GoldApple);
    }

    //initialize turn count for battles
    TurnCount = 1;
    CurrentTurn = 1;

    //initialize player coord when entering battle
    PlayerTempCoordX = 0;
    PlayerTempCoordY = 0;

    //Selection for battles
    EffectSelect = 0;
    TargetIndex = 0;

    //initialize player action indicator
    Action = Main;

    cb = g_Console.getConsoleSize();
    cb.Y = 12;
    cb.X = g_Console.getConsoleSize().X / 2-14;
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();    
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{    
    switch (g_eGameState)
    {
    case S_MENUSCREEN: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event
        break;
    case S_HOWTOPLAY: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event
        break;
    case S_MAP1: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_GAMEPAUSE: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event
        break;
    case S_BATTLE: gameplayKBHandler(keyboardEvent); // handle gameplay mouse event
        break;
    case S_INVENTORY: gameplayKBHandler(keyboardEvent);
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{    
    switch (g_eGameState)
    {
    case S_MENUSCREEN: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_HOWTOPLAY: gameplayMouseHandler(mouseEvent); // handle gameplay keyboard event
        break;
    case S_MAP1: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_GAMEPAUSE: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_BATTLE: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_INVENTORY: gameplayMouseHandler(mouseEvent);
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case 0x57: key = K_UP; break;
    case 0x53: key = K_DOWN; break;
    case 0x41: key = K_LEFT; break;
    case 0x44: key = K_RIGHT; break;
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case VK_TAB: key = K_TAB; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
   

}   


//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_MENUSCREEN: splashScreenWait(); // game logic for the splash screen
            break;
        case S_HOWTOPLAY: howtoplaybutton();
            break;
        case S_MAP1: updateGame(); // gameplay logic when we are in the game
            break;
        case S_GAMEPAUSE: splashScreenWait(); // game logic for the splash screen
            break;
        case S_BATTLE: updateBattle(); // handle gameplay mouse event
            break;
        case S_INVENTORY: updateInventory();
            break;
    }
}

void splashScreenWait()    // choose options in menu
{
    if (g_skKeyEvent[K_DOWN].keyDown && cb.Y < 16)
    {
        cb.Y += 2;
    }
    if (g_skKeyEvent[K_UP].keyDown && cb.Y > 12)
    {
        cb.Y -= 2;
    }

    if (g_skKeyEvent[K_SPACE].keyDown)
    {
        if (cb.Y == 12)
        {
            if (g_eGameState == S_MENUSCREEN || g_eGameState == S_GAMEPAUSE)
            {
                g_eGameState = S_MAP1;

            }
        }
        if (cb.Y == 14)
        {
            if (g_eGameState == S_GAMEPAUSE)
            {
                g_eGameState = S_MENUSCREEN;
            }
            else
            {
                g_eGameState = S_HOWTOPLAY;
            }
        }
        if (cb.Y == 16)
        {
            if (g_eGameState == S_MENUSCREEN || g_eGameState == S_GAMEPAUSE)
            {
                g_bQuitGame = true;

            }
        }
    }

}
void howtoplaybutton()
{
    if (g_skKeyEvent[K_ESCAPE].keyDown)
    {
        g_eGameState = S_MENUSCREEN;
    }
}

void renderhowtoplay()
{
    COORD ca = g_Console.getConsoleSize();
    ca.Y = 10;
    ca.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(ca, "Explore the map and encouter random enemies", 0x05);
    ca.Y += 3;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "Use WASD for movement", 0x06);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(ca, "Press esc in game to pause", 0x06);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(ca, "Spacebar to enter", 0x06);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 17;
    g_Console.writeToBuffer(ca, "Press esc to go back to main menu", 0x09);
}

void updateGame()       // gameplay logic
{
    if (g_skKeyEvent[K_ESCAPE].keyDown)
    {
        timescale = false;
        g_eGameState = S_GAMEPAUSE;
    }
    else
    {
        Collision();
     moveCharacter();    // moves the character, collision detection, physics, etc
                 // sound can be played here too.

    }
     //processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
}
void rendergamepause()
{
    COORD ca = g_Console.getConsoleSize();
    ca.Y = 12;
    //ca.X = g_Console.getConsoleSize().X / 2 - 10;
    //g_Console.writeToBuffer(ca, "Resume", 0x09);
    //ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "Resume", 0x09);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "Main menu", 0x09);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "Quit", 0x09); // Main page
    arrow();
}

void moveCharacter()
{  
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y--;   

        //setup seed for random encounter every move
        srand(static_cast<unsigned int>(time(0)));

        //random encounter
        foundRandomEncounter();
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X--;    

        //setup seed for random encounter every move
        srand(static_cast<unsigned int>(time(0)));

        //random encounter
        foundRandomEncounter();
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y++;

        //setup seed for random endcounter every move
        srand(static_cast<unsigned int>(time(0)));

        //random encounter
        foundRandomEncounter();
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X++; 

        //setup seed for random endcounter every move
        srand(static_cast<unsigned int>(time(0)));

        //random encounter
        foundRandomEncounter();
    }
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
    g_sChar.m_bActive = !g_sChar.m_bActive;
    }
    if (g_skKeyEvent[K_TAB].keyReleased)
    {
        inventoryOpened();

    }
  
    
}

//map2 collision(DELETE IT)
//========================================================================================================
void Colision2()
{
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 75 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.Y -= 1;
    }
}
//==========================================================================================================
void Collision()
{
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 75 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 76 && g_sChar.m_cLocation.Y == 15) //starting
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 74 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 73 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 78 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 14)
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 13)
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 12)
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 11)
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 15)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 14)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 13)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 12) //starting right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 11)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 10)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 9)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 8)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 7)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 76 && g_sChar.m_cLocation.Y == 6)  //starting above wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 75 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 74 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 73 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 71 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 70 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 69 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 68 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 67 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 54 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 53 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 52 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 51 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 50 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 48 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 47 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 6)
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 71 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 70 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 69 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 68 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 67 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 54 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 53 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 52 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 51 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 50 && g_sChar.m_cLocation.Y == 10) //above hole wall
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 11) // hole entrance
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 50 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 51 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 52 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 53 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 54 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 12) // hole up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 50 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 51 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 52 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 53 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 54 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 12) // hole down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 12) // inside hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 6) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 7) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 8) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 9) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 10) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 11) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 12) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 13) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 14) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 15) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 16) // beside hole
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 13) // below big block hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 14) // below big block hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 15) // below big block hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 16) // below big block hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 17) // below big block hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 18) // below big block hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 19) // below big block hole
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 11 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 12 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 13 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 14 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 15 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 16 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 17 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 18 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 19 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 21 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 22 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 23 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 24 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 25 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 27 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 29 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 30 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 31 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 32 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 33 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 35 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 36 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 37 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 38 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 39 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 41 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 42 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 43 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 44 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 45 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 47 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 48 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 19) // area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 21 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 22 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 23 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 24 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 25 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 27 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 29 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 30 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 31 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 32 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 33 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 35 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 36 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 37 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 38 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 39 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 41 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 42 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 43 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 44 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 45 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 17) // area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 19) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 18) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 17) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 16) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 15) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 14) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 13) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 12) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 11) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 10) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 9) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 8) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 7) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 6) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 5) // area 1 left wall
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 7) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 8) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 9) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 10) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 11) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 12) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 13) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 14) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 15) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 16) // area 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 10 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 11 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 12 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 13 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 14 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 15 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 16 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 17 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 18 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 19 && g_sChar.m_cLocation.Y == 5) // area 1 up tall wall
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 6) // maze tail
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 21 && g_sChar.m_cLocation.Y == 6) // maze tail
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 22 && g_sChar.m_cLocation.Y == 6) // maze tail
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 23 && g_sChar.m_cLocation.Y == 6) // maze tail
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 24 && g_sChar.m_cLocation.Y == 6) // maze tail
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 25 && g_sChar.m_cLocation.Y == 6) // maze tail
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 7) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 8) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 9) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 10) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 11) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 12) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 13) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 14) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 15) // in start maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 27 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 29 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 30 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 31 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 32 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 33 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 35 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 36 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 37 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 38 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 39 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 15) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 14) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 13) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 12) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 11) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 10) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 9) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 14) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 13) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 12) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 11) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 10) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 9) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 35 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 36 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 37 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 38 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 39 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 8) // inside maze 1
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 14) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 13) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 12) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 11) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 10) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 9) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 8) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 7) //  maze 1 right wall
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 4) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 3) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 2) // inside maze 1
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 29 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 30 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 31 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 32 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 33 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 35 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 36 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 37 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 38 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 39 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 6) // on top of maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 5) // on top of maze 1
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 20 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 21 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 22 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 23 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 24 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 25 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 26 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 27 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 28 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 29 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 30 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 31 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 32 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 33 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 34 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 35 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 36 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 37 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 38 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 39 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 40 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 41 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 42 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 43 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 44 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 45 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 47 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 48 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 50 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 51 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 52 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 53 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 54 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 67 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 68 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 69 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 70 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 71 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 73 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 74 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 75 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 76 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 78 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 79 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 80 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 81 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 82 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 84 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 2) // upmost map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 41 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 42 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 43 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 44 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 45 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 46 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 47 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 48 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 49 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 50 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 51 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 52 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 53 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 54 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 67 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 68 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 69 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 70 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 71 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 73 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 74 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 75 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 76 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 78 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 79 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 80 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 81 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 82 && g_sChar.m_cLocation.Y == 4) // on top of maze map
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 2) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 3) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 4) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 5) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 6) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 7) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 8) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 9) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 10) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 11) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 12) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 13) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 14) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 15) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 16) // right of area 2
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 5) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 6) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 7) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 8) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 9) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 10) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 11) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 12) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 13) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 14) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 15) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 16) // right of area 2
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 67 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 68 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 69 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 70 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 71 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 73 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 74 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 75 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 76 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 78 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 79 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 80 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 81 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 82 && g_sChar.m_cLocation.Y == 17) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 14) // area 2 up map right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 15) // area 2 up map right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 16) // area 2 up map right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 14) // area 2 up map
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 14) // area 2 up map left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 15) // area 2 up map left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 16) // area 2 up map left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 17) // area 2 up map left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 18) // area 2 up map left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 19) // area 2 up map left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 55 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 56 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 57 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 58 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 59 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 61 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 63 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 64 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 65 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 66 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 67 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 68 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 69 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 70 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 71 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 72 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 73 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 74 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 75 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 76 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 77 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 78 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 79 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 80 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 81 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 82 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 83 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 84 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 86 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 87 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 88 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 89 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 90 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 91 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 92 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 93 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 94 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 95 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 96 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 97 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 98 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 99 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 100 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 101 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 102 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 103 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 104 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 105 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 107 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 108 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 109 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 110 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 112 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 113 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 115 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 116 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 117 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 118 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 119 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 121 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 122 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 123 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 19) // mid platform
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 86 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 87 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 88 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 89 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 90 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 91 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 92 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 93 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 94 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 95 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 96 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 97 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 98 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 99 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 100 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 101 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 102 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 103 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 104 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 105 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 107 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 108 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 109 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 110 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 17) // mid right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 18) // mid right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 19) // mid right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 115 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 116 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 117 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 118 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 119 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 121 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 122 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 123 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 17) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 16) // entrance 
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 14) // entrance 
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 15) // entrance 
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 16) // entrance 
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 112 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 113 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 115 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 116 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 117 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 118 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 119 && g_sChar.m_cLocation.Y == 14) // mid up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 115 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 116 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 117 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 118 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 119 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 121 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 122 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 123 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 6) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 7) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 8) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 9) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 10) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 11) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 12) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 13) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 14) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 15) // mid down
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 8) // mid upwards left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 9) // mid upwards left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 10) // mid upwards left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 11) // mid upwards left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 12) // mid upwards left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 13) // mid upwards left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 107 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 108 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 109 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 110 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 112 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 113 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 115 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 116 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 117 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 118 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 119 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 120 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 121 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 122 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 123 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 124 && g_sChar.m_cLocation.Y == 6) // mid upwards up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 110 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 112 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 113 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 115 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 116 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 117 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 118 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 119 && g_sChar.m_cLocation.Y == 7) // mid upwards down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 109 && g_sChar.m_cLocation.Y == 8) // mid upwards small side
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 6) // mid upwards left side
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 7) // mid upwards left side
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 8) // mid upwards left side
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 9) // mid upwards left side
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 9) // box right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 10) // box right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 11) // box right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 12) // box right
    {
        g_sChar.m_cLocation.X -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 110 && g_sChar.m_cLocation.Y == 9) // box up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 9) // box up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 112 && g_sChar.m_cLocation.Y == 9) // box up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 113 && g_sChar.m_cLocation.Y == 9) // box up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 9) // box up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 107 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 108 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 109 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 110 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 111 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 112 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 113 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 114 && g_sChar.m_cLocation.Y == 12) // box down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 11) // box hole left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_LEFT].keyDown && g_sChar.m_cLocation.X == 106 && g_sChar.m_cLocation.Y == 12) // box hole left
    {
        g_sChar.m_cLocation.X += 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 101 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 102 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 103 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 104 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_DOWN].keyDown && g_sChar.m_cLocation.X == 105 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 down
    {
        g_sChar.m_cLocation.Y -= 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 101 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 102 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 103 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 104 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }
    if (g_skKeyEvent[K_UP].keyDown && g_sChar.m_cLocation.X == 105 && g_sChar.m_cLocation.Y == 10) // entrance to area 1 up
    {
        g_sChar.m_cLocation.Y += 1;
    }

}

void foundRandomEncounter(void)
{
    if (((rand() % RandomRate) == 0) &&
        (--RandomDelay == 0))
    {
        RandomDelay = 3;
        initEnemyGroup(rand() % 1);
        PlayerTempCoordX = g_sChar.m_cLocation.X;
        PlayerTempCoordY = g_sChar.m_cLocation.Y;
        g_eGameState = S_BATTLE;
    }
}

void initEnemyGroup(int EnemyGroup)
{
    //will add more as necessary
    if (EnemyGroup == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            EnemyParty[i] = new Skeleton;
        }
    }
    if (EnemyGroup == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            EnemyParty[i] = new Skeleton;
        }
    }
}

//void processUserInput()
//{
//    // quits the game if player hits the escape key
//    if (g_skKeyEvent[K_ESCAPE].keyReleased)
//        g_bQuitGame = true;
//}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void updateBattle()
{
    if ((CurrentClass == PlayerParty[0])||
        (CurrentClass == PlayerParty[1]) ||
        (CurrentClass == PlayerParty[2]) ||
        (CurrentClass == PlayerParty[3]))

    {
        BattleMove();
    }

    switch (Action)
    {
    case Main:
        TurnStart();
        BattleSelect();
        break;
        if ((CurrentClass == PlayerParty[0]) ||
            (CurrentClass == PlayerParty[1]) ||
            (CurrentClass == PlayerParty[2]) ||
            (CurrentClass == PlayerParty[3]))

        {
    case Attack:
        SelectTarget(EnemyParty);
        if (Target[0] != nullptr)
        {
            BattleAttack();
        }
        break;
    case Special:
        SelectSpecialAction();
        break;
    case Skill:
        SelectSkill();
        break;
    case Select:

        SelectTarget(EnemyParty);
        if (Target[0] != nullptr)
        {
            ExecuteSkill(EffectSelect);
        }
        break;
        }
    }

    VictoryCondition();
}

void TurnStart()
{
    if (CurrentTurn == TurnCount)
    {
        //set previous class
        if ((CurrentClass != nullptr) &&
            (CurrentClass->GetTurn() == false))
        {
            PreviousClass = CurrentClass;
        }
        //reset target pointers
        for (int i = 0; i < 4; i++)
        {
            Target[i] = nullptr;
        }

        TurnCount++;
        ResetCursorPosition();

        CurrentClass = new EmptyClass;
        for (int i = 0; i < 4; i++)
        {
            //check player party first so that if a enemy and a player has the same speed the player would go first
            if (PlayerParty[i] != nullptr)
            {
                if (PlayerParty[i]->GetTurn() == true)
                {
                    if (PlayerParty[i]->GetSpeed() >= CurrentClass->GetSpeed())
                    {
                        if (i == 0)
                        {
                            delete CurrentClass;
                            CurrentClass = nullptr;
                        }
                        CurrentClass = PlayerParty[i];
                    }
                }
            }
            if (EnemyParty[i] != nullptr)
            {
                if (EnemyParty[i]->GetTurn() == true)
                {
                    if (EnemyParty[i]->GetSpeed() > CurrentClass->GetSpeed())
                    {
                        if (i == 0)
                        {
                            delete CurrentClass;
                            CurrentClass = nullptr;
                        }
                        CurrentClass = EnemyParty[i];
                    }
                }
            }
        }

        if (CurrentClass == PreviousClass)
        {
            RoundEnd();
        }
    }
}

void BattleMove()
{
    if (g_skKeyEvent[K_UP].keyReleased && g_sChar.m_cLocation.Y > (g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)))
    {
        //move up
        g_sChar.m_cLocation.Y -= ((g_Console.getConsoleSize().Y / 4) / 2) + 1;

    }
    if (g_skKeyEvent[K_LEFT].keyReleased && g_sChar.m_cLocation.X > g_Console.getConsoleSize().X / 8)
    {
        //move left
        g_sChar.m_cLocation.X -= g_Console.getConsoleSize().X / 2;

    }
    if (g_skKeyEvent[K_DOWN].keyReleased && g_sChar.m_cLocation.Y < (g_Console.getConsoleSize().Y - g_Console.getConsoleSize().Y / 8))
    {
        //move down
        g_sChar.m_cLocation.Y += ((g_Console.getConsoleSize().Y / 4) / 2) + 1;
    }
    if (g_skKeyEvent[K_RIGHT].keyReleased && g_sChar.m_cLocation.X < (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        //move right
        g_sChar.m_cLocation.X += g_Console.getConsoleSize().X / 2;
    }

    
}

void BattleSelect()
{
    //select button attack
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == g_Console.getConsoleSize().X / 8)
    {
        //reset curosr position
        ResetCursorPosition();

        //set player action
        Action = Attack;
    }

    //select defend
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        CurrentClass->Defend();
    }

    //select special
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8))
    {
        //reset cursor position
        ResetCursorPosition();

        //set player action
        Action = Special;
    }

    //select flee
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        EndBattle();
    }
}

void BattleAttack()
{
    if (Target != nullptr)
    {
        CurrentClass->Attack(Target[0]);
        TurnEnd();
        Action = Main;
    }
}

void SelectTarget(Class* TargetParty[])
 {
    //select target 1
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == g_Console.getConsoleSize().X / 8 &&
        (TargetParty[0] != nullptr)) 
    {
        Target[0] = TargetParty[0];
    }

    //select target 2
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2) &&
        (TargetParty[1] != nullptr))
    {
        Target[0] = TargetParty[1];
    }

    //select target 3
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) &&
        (TargetParty[2] != nullptr))
    {
        Target[0] = TargetParty[2];
    }

    //select target 4
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2) &&
        (TargetParty[3] != nullptr))
    {
        Target[0] = TargetParty[3];
    }

    //go back if escape
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        //reset cursor position
        g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 8;

        //go back to previous menu
        Action = Main;
    }
}

void SelectSpecialAction()
{
    //select Skill
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == g_Console.getConsoleSize().X / 8)
    {
        Action = Skill;
    }

    //select Item
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        ResetCursorPosition();
        Action = Item;
    }

    //return to previous menu
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        //reset cursor position
        ResetCursorPosition();

        //go back to previous menu
        Action = Main;
    }
}

void SelectSkill()
{
    //select skill 1
    if (g_skKeyEvent[K_SPACE].keyReleased &&
         (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == g_Console.getConsoleSize().X / 8 )
    {
        if (CurrentClass->SkillNameList(0) != "")
        {
            EffectSelect = 0;
            CheckTargetType(CurrentClass->SkillTargetType(0));
        }
    }

    //select skill 2
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        if (CurrentClass->SkillNameList(1) != "")
        {
            EffectSelect = 1;
            CheckTargetType(CurrentClass->SkillTargetType(1));
        }
    }

    //select skill 3
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8))
    {
        if (CurrentClass->SkillNameList(2) != "")
        {
            EffectSelect = 2;
            CheckTargetType(CurrentClass->SkillTargetType(2));
        }
    }

    //select skill 4
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        if (CurrentClass->SkillNameList(4) != "")
        {
            EffectSelect = 3;
            CheckTargetType(CurrentClass->SkillTargetType(4));
        }
    }
}
void CheckTargetType(int type)
{
    if (type == Class::Single)
    {
        ResetCursorPosition();
        Action = Select;
    }
    else if (type == Class::AOE)
    {
        for (int i = 0; i < 4; i++)
        {
            Target[i] = EnemyParty[i];
        }
        ExecuteSkill(EffectSelect);
    }
    else if (type == Class::Self)
    {
        Target[0] = PlayerParty[0];
        ExecuteSkill(EffectSelect);
    }
    else if (type == Class::FSingle)
    {
        ResetCursorPosition();
        Action = FSelect;
    }
    else if (type == Class::FAOE)
    {
        for (int i = 0; i < 4; i++)
        {
            Target[i] = PlayerParty[i];
        }
        ExecuteSkill(EffectSelect);
    }
}

void ExecuteSkill(int SkillIndex)
{
    CurrentClass->SkillList(SkillIndex, TargetIndex, Target);
    TurnEnd();
    Action = Main;
}

void ResetCursorPosition(void)
{
    //reset cursor position
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 8;
}

void TurnEnd(void)
{
    //check if an enemy is dead
    for (int i = 0; i < 4; i++)
    {
        if (EnemyParty[i] != nullptr)
        {
            if (EnemyParty[i]->GetHealth() <= 0)
            {
                delete EnemyParty[i];
                EnemyParty[i] = nullptr;
            }
        }
    }
    CurrentClass->SetTurn(false);
    CurrentTurn++;
}

void RoundEnd(void)
{
    for (int i = 0; i < 4; i++)
    {
        if (EnemyParty[i] != nullptr)
        {
            EnemyParty[i]->SetTurn(true);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if ((PlayerParty[i] != nullptr) &&
            !(PlayerParty[i]->GetHealth() <= 0))
        {
            PlayerParty[i]->SetTurn(true);
        }
    }
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_MENUSCREEN: renderSplashScreen();
        break;
    case S_HOWTOPLAY: renderhowtoplay();
        break;
    case S_GAMEPAUSE: rendergamepause();
        break;
    case S_MAP1: renderGame();
        break;
    case S_BATTLE: renderBattle();
        break;
    case S_INVENTORY: renderInventory();
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x07);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void VictoryCondition()
{
    if ((EnemyParty[0] == nullptr) &&
        (EnemyParty[1] == nullptr) &&
        (EnemyParty[2] == nullptr) &&
        (EnemyParty[3] == nullptr))
    {
        EndBattle();
    }
}

void EndBattle()
{
    RoundEnd();
    TurnCount = 1;
    CurrentTurn = 1;
    g_sChar.m_cLocation.X = PlayerTempCoordX;
    g_sChar.m_cLocation.Y = PlayerTempCoordY;
    g_eGameState = S_MAP1;
}



//----------------------------------------------------------------------------------
//Inventory function is coded here
Items* SelectedItem = nullptr;
Class* SelectedPlayer = nullptr;
//these store the player's original position on the map so they can return to it if after leaving inventories/shops
int initialX = 0;
int initialY = 0;
int InventoryPage = 1;

void inventoryOpened()
{
    initialX = g_sChar.m_cLocation.X;
    initialY = g_sChar.m_cLocation.Y;
    g_sChar.m_cLocation.X = 29;
    g_sChar.m_cLocation.Y = 9;
    g_eGameState = S_INVENTORY;
}
void inventoryClosed()
{
    g_sChar.m_cLocation.X = initialX;
    g_sChar.m_cLocation.Y = initialY;
    SelectedItem = nullptr;
    SelectedPlayer = nullptr;
}

void renderShop()
{
    renderShopScreen();
}
//Moving system for inventories and shops
void InventoryMove()
{
    if (InventoryPage == 1)
    {
        if (g_skKeyEvent[K_UP].keyReleased && (g_sChar.m_cLocation.Y != 9))
        {
            //move up
            if (g_sChar.m_cLocation.Y == 27)
            {
                g_sChar.m_cLocation.Y = 9;
                g_sChar.m_cLocation.X = 29;
            }
            else
            {
                g_sChar.m_cLocation.Y -= 3;
            }
        }
        else if (g_skKeyEvent[K_LEFT].keyReleased && (g_sChar.m_cLocation.X != 32)
            && (g_sChar.m_cLocation.X != 29))
        {
            //move left
            if (g_sChar.m_cLocation.Y == 27)
            {
                g_sChar.m_cLocation.X -= 68;
            }
            else
            {
                g_sChar.m_cLocation.X -= 75;
            }
        }
        else if (g_skKeyEvent[K_DOWN].keyReleased && (g_sChar.m_cLocation.Y != 27))
        {
            //move down
            if (g_sChar.m_cLocation.Y == 21)
            {
                g_sChar.m_cLocation.Y = 27;
                g_sChar.m_cLocation.X = 32;
            }
            else
            {
                g_sChar.m_cLocation.Y += 3;
            }
        }
        else if (g_skKeyEvent[K_RIGHT].keyReleased && (g_sChar.m_cLocation.X != 104)
            && (g_sChar.m_cLocation.X != 99))
        {
            //move right
            if (g_sChar.m_cLocation.Y == 27)
            {
                g_sChar.m_cLocation.X += 68;
            }
            else
            {
                g_sChar.m_cLocation.X += 75;
            }
        }
    }
    else if (InventoryPage == 2)
    {
        if (g_skKeyEvent[K_UP].keyReleased && (g_sChar.m_cLocation.Y != 8))
        {
            g_sChar.m_cLocation.Y -= 3;
        }
        else if (g_skKeyEvent[K_DOWN].keyReleased && (g_sChar.m_cLocation.Y != 20))
        {
            g_sChar.m_cLocation.Y += 3;
        }
    }
}
void updateInventory()
{
    InventoryMove();
    InventorySelection();
}
//selecting system for shops
void ShopSelect()
{
    //select the item
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y <= (g_Console.getConsoleSize().Y / 10) * 8))
    {

    }

    //select exit
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == (g_Console.getConsoleSize().Y / 10) * 9) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 13) * 3)
    {
    }
    //select use
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == (g_Console.getConsoleSize().Y / 10) * 9) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 13) * 9)
    {

    }
}

void renderInventory()
{
    renderInventoryScreen();
    renderSelection();
}
void renderShopScreen()
{
    COORD c;
    std::ostringstream ss;

    //SHOP TITLE
    c.Y = g_Console.getConsoleSize().Y / 10;
    c.X = g_Console.getConsoleSize().X / 2;
    ss.str(" SHOP");
    g_Console.writeToBuffer(c, ss.str(), 0x07);

    /*Shop Items all displayed below*/
    for (int i = 0; i < 5; i++)
    {
        c.Y = (g_Console.getConsoleSize().Y / 10) * (i + 3);
        c.X = (g_Console.getConsoleSize().X / 10) * 2;
        ss.str(ShopInventory.GetItem(i)->GetName());
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
    for (int i = 5; i < 10; i++)
    {
        c.Y = (g_Console.getConsoleSize().Y / 10) * (i - 2);
        c.X = (g_Console.getConsoleSize().X / 10) * 7;
        ss.str(ShopInventory.GetItem(i)->GetName());
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
    c.Y = (g_Console.getConsoleSize().Y / 10) * 9;
    c.X = (g_Console.getConsoleSize().X / 13) * 3;
    ss.str("Exit");
    g_Console.writeToBuffer(c, ss.str(), 0x07);

    c.Y = (g_Console.getConsoleSize().Y / 10) * 9;
    c.X = (g_Console.getConsoleSize().X / 13) * 9;
    ss.str("Buy");
    g_Console.writeToBuffer(c, ss.str(), 0x07);
}

void renderInventoryScreen()
{
    COORD c;
    std::ostringstream ss;
    if (InventoryPage == 1)
    {
        c.Y = 3;
        c.X = 67;
        ss.str(" YOUR BACKPACK");
        g_Console.writeToBuffer(c, ss.str(), 0x07);

        /*Player's items all displayed below*/
        for (int i = 0; i < 5; i++)
        {
            c.Y = 9 + (3 * i);
            c.X = 30;
            if (PlayerInventory.GetItem(i) == nullptr)
            {
                ss.str("-");
            }
            else
            {
                ss.str(PlayerInventory.GetItem(i)->GetName());
            }
            g_Console.writeToBuffer(c, ss.str(), 0x07);
            if (SelectedItem == PlayerInventory.GetItem(i) &&
                SelectedItem != nullptr)
            {
                g_Console.writeToBuffer(c, ss.str() + PlayerInventory.GetItem(i)->GetDescription(), 0x5E);

            }
            else
            {
                g_Console.writeToBuffer(c, ss.str(), 0x07);
            }
        }
        for (int i = 5; i < 10; i++)
        {
            c.Y = (3 * i) - 6;
            c.X = 105;
            if (PlayerInventory.GetItem(i) == nullptr)
            {
                ss.str("-");
            }
            else
            {
                ss.str(PlayerInventory.GetItem(i)->GetName());
            }
            g_Console.writeToBuffer(c, ss.str(), 0x07);
            if (SelectedItem == PlayerInventory.GetItem(i) &&
                SelectedItem != nullptr)
            {
                g_Console.writeToBuffer(c, ss.str() + PlayerInventory.GetItem(i)->GetDescription(), 0x5E);
            }
            else
            {
                g_Console.writeToBuffer(c, ss.str(), 0x07);
            }
        }
        c.Y = 27;
        c.X = 33;
        ss.str("Exit");
        g_Console.writeToBuffer(c, ss.str(), 0x07);
        c.Y = 27;
        c.X = 100;
        ss.str(" Use");
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
    else if (InventoryPage == 2)
    {
        c.Y = 3;
        c.X = 54;
        g_Console.writeToBuffer(c, "Who would you like to give the item to?", 0x07);
        for (int i = 0; i < 4; i++)
        {
            c.X = 63;
            c.Y = 8 + (i * 3);
            if (SelectedPlayer == PlayerParty[i])
            {
                ss << PlayerParty[i]->GetName() << " - HP :  " << PlayerParty[i]->GetHealth() << "/" << PlayerParty[i]->GetMaxHealth();
                g_Console.writeToBuffer(c, ss.str(), 0x5E);
            }
            else
            {
                ss << PlayerParty[i]->GetName() << " - HP :  " << PlayerParty[i]->GetHealth() << "/" << PlayerParty[i]->GetMaxHealth();
                g_Console.writeToBuffer(c, ss.str(), 0x07);
            }
            ss.str(" ");
        }
        c.Y = 20;
        c.X = 63;
        g_Console.writeToBuffer(c, "Use", 0x07);
    }
}

void InventorySelection()
{
    if (InventoryPage == 1)
    {
        //select item
        for (int i = 0; i < 5; i++)
        {
            if (g_skKeyEvent[K_SPACE].keyReleased &&
                g_sChar.m_cLocation.X == 29 &&
                g_sChar.m_cLocation.Y == 9 + (3 * i))
            {
                SelectedItem = PlayerInventory.GetItem(i);
            }
        }
        for (int i = 5; i < 10; i++)
        {
            if (g_skKeyEvent[K_SPACE].keyReleased &&
                g_sChar.m_cLocation.X == 104 &&
                g_sChar.m_cLocation.Y == (3 * i) - 6)
            {
                SelectedItem = PlayerInventory.GetItem(i);
            }
        }
        //select exit
        if (g_skKeyEvent[K_SPACE].keyReleased &&
            (g_sChar.m_cLocation.Y == 27) &&
            g_sChar.m_cLocation.X == 32)
        {
            inventoryClosed();
        }
        //select use
        else if (g_skKeyEvent[K_SPACE].keyReleased &&
            g_sChar.m_cLocation.Y == 27 &&
            g_sChar.m_cLocation.X == 100 &&
            SelectedItem != nullptr)
        {
            InventoryPage = 2;
            g_sChar.m_cLocation.X = 62;
            g_sChar.m_cLocation.Y = 8;
        }
    }
    else if (InventoryPage == 2)//the player selects the player to use the item on
    {
        for (int i = 0; i < 4; i++)
        {
            if (g_skKeyEvent[K_SPACE].keyReleased &&
                g_sChar.m_cLocation.X == 62 &&
                g_sChar.m_cLocation.Y == 8 + (3 * i))
            {
                SelectedPlayer = PlayerParty[i];
            }
        }
        if (g_sChar.m_cLocation.X == 62 && g_sChar.m_cLocation.Y == 20 && g_skKeyEvent[K_SPACE].keyReleased &&
            SelectedPlayer != nullptr)
        {
            //item is used on the player and removed from the inventory
            SelectedItem->ItemEffect(SelectedPlayer);
            PlayerInventory.DiscardItem(SelectedItem);

            SelectedPlayer = nullptr;
            SelectedItem = nullptr;
            g_sChar.m_cLocation.X = 29;
            g_sChar.m_cLocation.Y = 9;
            InventoryPage = 1;
        }
    }
}

//----------------------------------------------------------------------------



void renderSplashScreen()  // renders the splash screen
{
    COORD ca=g_Console.getConsoleSize();
    ca.Y = 10;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "Welcome To :THE 'RPG'!", 0x03);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "1. Start", 0x09);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "2. How to play", 0x09);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "3. Quit", 0x09); // Main page
    arrow();
}


void arrow()
{
    g_Console.writeToBuffer(cb, "-->", 0x09);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void renderGame()
{
    renderMap1();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
}

void renderMap1()
//---------------------------------------------------------------------------------------------------------------------------------------------
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };


    COORD c;
    //if (c.X = 5 * i, c.Y = i + 1)
    //{
    //    colour(colors[i]);
    //    g_Console.writeToBuffer(c, " ∞±≤€", colors[i]);   // colour combi
    //}
    if (c.X = 91, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "≤≤≤≤≤", colors[5]); //fountain
    }
    if (c.X = 65, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "≤≤≤≤≤", colors[5]); //fountain
    }
    if (c.X = 10, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[3]); // BOSS
    }
    if (c.X = 70, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 75, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 78, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }

    if (c.X = 41, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 41, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 67, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 67, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 67, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 67, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 67, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 67, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 53, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞ ", colors[1]);
    }
    if (c.X = 50, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 50, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 55, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 50, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 50, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 50, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 50, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 50, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 50, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }

    if (c.X = 40, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }

    if (c.X = 125, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 125, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }

    if (c.X = 60, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }if (c.X = 5, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 10, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    //---------------------------------------------------
    if (c.X = 140, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }

    if (c.X = 78, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 82, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 86, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 21, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 31, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 29, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 91, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 96, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }

    if (c.X = 106, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 106, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 106, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 120, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 115, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 110, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 115, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 115, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 115, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 115, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 115, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 110, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 95, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 101, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 140, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 0, c.Y = 28)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 128, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 134, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞", colors[1]);
    }
    if (c.X = 94, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 105, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 105, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 105, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 119, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 119, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 119, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 128, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 131, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 128, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 131, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 80, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 80, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 80, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 80, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 80, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 70, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 70, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 60, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 60, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 40, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 40, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 40, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 40, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 40, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 40, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 25, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 25, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }

    if (c.X = 30, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 5, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 15, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 3, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 0, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 145, c.Y = 28)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞", colors[1]);
    }
    if (c.X = 0, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞", colors[1]);
    }
    if (c.X = -1, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }
    if (c.X = -1, c.Y = 28)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " ∞", colors[1]);
    }

}
//----------------------------------------------------------------------------------------------------------------------------------------------


//===============================================================================================================================================
//===============================================================================================================================================
//2nd map
//void renderMap1()
//{
//    
//    const WORD colors[] = {
//       0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
//       0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
//    };
//    COORD c;
//
//    if (c.X = 0, c.Y = 1)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, " €", colors[6]);
//    }
//    if (c.X = 0, c.Y = 2)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, " €", colors[6]);
//    }
//    if (c.X = 0, c.Y = 3)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 4)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 5)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 6)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 7)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 8)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 9)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 10)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 11)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 12)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 13)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 14)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 15)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 16)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 17)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 18)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 19)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 20)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 21)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 22)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 23)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 24)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 25)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 26)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 27)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 28)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 2)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 2, c.Y = 1)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 1)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 2)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 3)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 4)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 5)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 6)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 7)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 8)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 9)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 10)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 11)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 12)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 13)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 14)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 15)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 16)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 17)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 18)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 19)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 20)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 21)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 22)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 23)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 24)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 25)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 26)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 27)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 148, c.Y = 28)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "€€", colors[6]);
//    }
//    if (c.X = 0, c.Y = 28)
//    {
//        colour(colors[6]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 1, c.Y = 4)
//    {
//        colour(colors[6]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 25, c.Y = 8)
//    {
//        colour(colors[6]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 2, c.Y = 13)
//    {
//        colour(colors[6]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 25, c.Y = 17)
//    {
//        colour(colors[6]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 1, c.Y = 21)
//    {
//        colour(colors[6]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 25, c.Y = 24)
//    {
//        colour(colors[6]);
//        g_Console.writeToBuffer(c, "€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€", colors[6]);
//    }
//    if (c.X = 142, c.Y = 27)
//    {
//        colour(colors[3]);
//        g_Console.writeToBuffer(c, "      ", colors[3]); //boss
//    }
//    if (c.X = 2, c.Y = 17)
//            {
//                colour(colors[1]);
//                g_Console.writeToBuffer(c, "≤≤≤≤≤", colors[5]); //fountain
//            }
//}
//===============================================================================================================================================
//===============================================================================================================================================
void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    //// displays the elapsed time
    //ss.str("");
    //ss << g_dElapsedTime << "secs";
    //c.X = 0;
    //c.Y = 0;
    //g_Console.writeToBuffer(c, ss.str(), 0x59);
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    COORD startPos = {50, 2};
    std::ostringstream ss;
    std::string key;
   /* for (int i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_UP: key = "W";
            break;
        case K_DOWN: key = "S";
            break;
        case K_LEFT: key = "A";
            break;
        case K_RIGHT: key = "D";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }*/

    // mouse events    
    ss.str("");
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;        
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:        
        break;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void renderBattle()
{
        renderBattleScreen();
        renderStatuses();
        renderSelection();
}

void renderSelection()
{
    // Draw the location of the character
    WORD charColor = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;

    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)0, charColor);
}

void renderBattleScreen()
{
    COORD c;
    std::ostringstream ss;
    if (Action == Main)
    {
        //fight button
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        c.X = (g_Console.getConsoleSize().X / 8);

        ss.str(" Attack");
        g_Console.writeToBuffer(c, ss.str(), 0x07);

        //defend button
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));

        ss.str(" Defend");
        g_Console.writeToBuffer(c, ss.str(), 0x07);

        //special button
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
        c.X = (g_Console.getConsoleSize().X / 8);

        ss.str(" Special");
        g_Console.writeToBuffer(c, ss.str(), 0x07);

        //flee button
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
        c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));

        ss.str(" Flee");
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
    else if ((Action == Attack) ||
        (Action == Select))
    {
        //target 1
        if (EnemyParty[0] != nullptr)
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
            c.X = (g_Console.getConsoleSize().X / 8);

            ss.str("");
            ss << " 1." << EnemyParty[0]->GetName();
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }

        //target 2
        if (EnemyParty[1] != nullptr)
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
            c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));

            ss.str("");
            ss << " 2." << EnemyParty[1]->GetName();
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }

        //target 3
        if (EnemyParty[2] != nullptr)
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
            c.X = (g_Console.getConsoleSize().X / 8);

            ss.str("");
            ss << " 3." << EnemyParty[2]->GetName();
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }

        //target 4
        if (EnemyParty[3] != nullptr)
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
            c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));


            ss.str("");
            ss << " 4." << EnemyParty[3]->GetName();
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }
    }
    else if (Action == Special)
    {
        //render skill button
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        c.X = (g_Console.getConsoleSize().X / 8);

        ss.str(" Skill");
        g_Console.writeToBuffer(c, ss.str(), 0x07);

        //render item button
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));

        ss.str(" Item");
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
    else if (Action == Skill)
    {
        //skill 1
        if (CurrentClass->SkillNameList(0) != "")
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
            c.X = (g_Console.getConsoleSize().X / 8);

            ss.str("");
            ss << " " << CurrentClass->SkillNameList(0) << " MP(" << CurrentClass->ManaCost(0) << ")";
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }

        //skill 2
        if (CurrentClass->SkillNameList(1) != "")
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
            c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));

            ss.str("");
            ss << " " << CurrentClass->SkillNameList(1) << " MP(" << CurrentClass->ManaCost(1) << ")";
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }

        //skill 3 
        if (CurrentClass->SkillNameList(2) != "")
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
            c.X = (g_Console.getConsoleSize().X / 8);

            ss.str("");
            ss << " " << CurrentClass->SkillNameList(2) << " MP(" << CurrentClass->ManaCost(2) << ")";
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }

        //skill 4
        if (CurrentClass->SkillNameList(3) != "")
        {
            c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
            c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));


            ss.str("");
            ss << " " << CurrentClass->SkillNameList(3) << " MP(" << CurrentClass->ManaCost(3) << ")";
            g_Console.writeToBuffer(c, ss.str(), 0x07);
        }
    }
}

void renderSelectScreen()
{
    COORD c;
    std::ostringstream ss;
    
}

void renderStatuses()
{
    //temporary
    COORD c;
    std::ostringstream ss;

    c.X = g_Console.getConsoleSize().X / 2;
    c.Y = g_Console.getConsoleSize().Y / 2;

    for (int i = 0; i < 4; i++)
    {
        if (EnemyParty[i] != nullptr)
        {
            c.X = g_Console.getConsoleSize().X / 2;
            c.Y = g_Console.getConsoleSize().Y / 2 + i;
            ss.str("");
            ss << i + 1 << "." << EnemyParty[i]->GetName() << " HP:" <<
                EnemyParty[i]->GetHealth() << "/" << EnemyParty[i]->GetMaxHealth();
            g_Console.writeToBuffer(c, ss.str(), 0x07); 
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (PlayerParty[i] != nullptr)
        {
            c.X = 0;
            c.Y = 0 + i;
            
            if (CurrentClass == PlayerParty[i])
            {
                for (int i = 0; i < 34; i++)
                {
                    ss.str(" ");
                    g_Console.writeToBuffer(c, ss.str(), 0x70);
                    c.X++;
                }
                c.X = 0;
                ss.str("");
                ss << "> " << i + 1 << "." << PlayerParty[i]->GetName() << " HP:" <<
                    PlayerParty[i]->GetHealth() << "/" << PlayerParty[i]->GetMaxHealth() << "   MP:"
                    << PlayerParty[i]->GetMana() << "/" << PlayerParty[i]->GetMaxMana();
                g_Console.writeToBuffer(c, ss.str(), 0x70);
            }
            else
            {
                ss.str("");
                ss << "  " << i + 1 << "." << PlayerParty[i]->GetName() << " HP:" <<
                    PlayerParty[i]->GetHealth() << "/" << PlayerParty[i]->GetMaxHealth() << "   MP:"
                    << PlayerParty[i]->GetMana() << "/" << PlayerParty[i]->GetMaxMana();
                g_Console.writeToBuffer(c, ss.str(), 0x07);
            }
        }
    }
}

