// This is the main file for the game logic and function

#include "game.h"
#include "Framework\console.h"
#include "Party.h"
#include "Class.h"
#include "Fighter.h"
#include "Wizard.h"
#include "Rogue.h"
#include "Cleric.h"
#include "Skeleton.h"
#include "Inventory.h"
#include "Items.h"
#include "HealingItems.h"
#include <iostream>
#include <iomanip>
#include <sstream>


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

//classes
Class* CurrentClass;
Class* Target;
Class* Classes[8];

//parties
Party* PlayerParty;
Party* EnemyParty;
Party* TargetParty;

//creating the player inventory, the shop inventory and the items in it
Inventory PlayerInventory;
Inventory ShopInventory;
Items* GoldApple = new HealingItems("Gold Apple", 4, 8);
Items* Bandage = new HealingItems("Bandage", 1, 2);
Items* NullItem = new Items;


//turn count for battles
int TurnCount;
int CurrentTurn;

//coordinates of player when battle is entered
int PlayerTempCoordX;
int PlayerTempCoordY;

//player action indicator
enum PlayerActions
{
    Attack,
    Skill,
    Item
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
    Target = nullptr;
    for (int i = 0; i < 8; i++)
    {
        Classes[i] = nullptr;
    }
    //Adds items and gold to the player and shop inventories
    ShopInventory.AddItem(GoldApple);
    ShopInventory.AddItem(Bandage);
    NullItem->SetName(" ");
    for (int i = 0; i < 9; i++)
    {
        ShopInventory.AddItem(NullItem);
    }
    for (int i = 0; i < 10; i++)
    {
        PlayerInventory.AddItem(NullItem);
    }

    //Initialize the parties for battles
    // Classes 0 - 3 are player classes
    //will change so that player selects class but for testing init as 4 classes
    Classes[0] = new Fighter;
    Classes[1] = new Wizard;
    Classes[2] = new Rogue;
    Classes[3] = new Cleric;
    PlayerParty = new Party(Classes[0], Classes[1], Classes[2], Classes[3]);

    // Classes 4 - 7 are player classes
    EnemyParty = new Party(nullptr, nullptr, nullptr, nullptr);

    //the targeted party
    TargetParty = new Party(nullptr, nullptr, nullptr, nullptr);

    //initialize turn count for battles
    TurnCount = 1;
    CurrentTurn = 1;

    //initialize player coord when entering battle
    PlayerTempCoordX = 0;
    PlayerTempCoordY = 0;

    //initialize player action indicator
    Action = Attack;

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
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_BATTLE: gameplayKBHandler(keyboardEvent); // handle gameplay mouse event
        break;
    case S_BATTLETARGET: gameplayKBHandler(keyboardEvent); // handle gameplay mouse event
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
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_BATTLE: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_BATTLETARGET: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
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
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
        case S_BATTLE: updateBattle(); // handle gameplay mouse event
            break;
        case S_BATTLETARGET: updateBattleTarget();
            break;
    }
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 10.0) // wait for 3 seconds to switch to game mode, else do nothing //CHNAGE TIMING FIX LTR!!!!!!!!!
        g_eGameState = S_GAME;
    if (g_skKeyEvent[K_DOWN].keyDown && cb.Y<16)
    {
        cb.Y += 2;
    }
    if (g_skKeyEvent[K_UP].keyDown && cb.Y > 12)
    {
        cb.Y -= 2;
    }
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
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


}

void foundRandomEncounter(void)
{
    if (((rand() % RandomRate) == 0) &&
        (--RandomDelay == 0))
    {
        RandomDelay = 3;
        initEnemyGroup(rand() % 1);
        EnemyParty->GetPartyClass(0);
        PlayerTempCoordX = g_sChar.m_cLocation.X;
        PlayerTempCoordY = g_sChar.m_cLocation.Y;
        g_eGameState = S_BATTLE;
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;
}

void updateBattle()
{
    TurnStart();
    BattleMove();
    BattleSelect();
}

void TurnStart()
{
    if (CurrentTurn == TurnCount)
    {
        //reset target pointers
        Target = nullptr;
        TargetParty = nullptr;

        TurnCount++;
        g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 8;

        CurrentClass = Classes[0];
        for (int i = 0; i < 8; i++)
        {
            if (Classes[i]->GetSpeed() > CurrentClass->GetSpeed())
            {
                if ((Classes[i] != nullptr) &&
                    (Classes[i]->GetTurn()))
                {
                    CurrentClass = Classes[i];
                }
            }
        }
        CurrentClass->SetTurn(false);
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
        g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 8;

        //set player action
        Action = Attack;

        //set the target party
        TargetParty = EnemyParty;

        //change game state
        g_eGameState = S_BATTLETARGET;
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

    }

    //select flee
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        TurnCount = 1;
        CurrentTurn = 1;
        g_sChar.m_cLocation.X = PlayerTempCoordX;
        g_sChar.m_cLocation.Y = PlayerTempCoordY;
        g_eGameState = S_GAME;
    }
}

void updateBattleTarget()
{
    BattleMove();
    CheckAction(Action);
    SelectTarget(TargetParty);
}

void CheckAction(int Action)
{
    if (Action == Attack)
    {
        if (TargetParty != nullptr)
        {
            TargetParty = EnemyParty;
        }
        if (Target != nullptr)
        {
            CurrentClass->Attack(Target);
            CurrentTurn++;
            g_eGameState = S_BATTLE;
        }
    }
}

void SelectTarget(Party* TargetParty)
{
    //select target 1
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == g_Console.getConsoleSize().X / 8)
    {
        if (TargetParty->GetPartyClass(0) != nullptr)
        {
            Target = TargetParty->GetPartyClass(0);
        }
    }

    //select target 2
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        if (TargetParty->GetPartyClass(1) != nullptr)
        {
            Target = TargetParty->GetPartyClass(1);
        }
    }

    //select target 3
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8))
    {
        if (TargetParty->GetPartyClass(2) != nullptr)
        {
            Target = TargetParty->GetPartyClass(2);
        }
    }

    //select target 4
    if (g_skKeyEvent[K_SPACE].keyReleased &&
        (g_sChar.m_cLocation.Y == g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8)) &&
        g_sChar.m_cLocation.X == (g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2))
    {
        if (TargetParty->GetPartyClass(3) != nullptr)
        {
            Target = TargetParty->GetPartyClass(3);
        }
    }

    //go back if escape
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        //reset cursor position
        g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 8;

        //go back to previous menu
        g_eGameState = S_BATTLE;
    }

    if (Target != nullptr)
    {
        CheckAction(Action);
    }

}


void initEnemyGroup(int EnemyGroup)
{
    //will add more as necessary
    if (EnemyGroup == 0)
    {
        for (int i = 4; i < 8; i++)
        {
            Classes[i] = new Skeleton;
        }
    }
    if (EnemyGroup == 1)
    {
        for (int i = 4; i < 7; i++)
        {
            Classes[i] = new Skeleton;
        }
    }
    for (int i = 4; i < 8; i++)
    {
        EnemyParty->SetPartyClass(i - 4, Classes[i]);
    }
    EnemyParty->GetPartyClass(0);
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
    case S_GAME: renderGame();
        break;
    case S_BATTLE: renderBattle();
        break;
    case S_BATTLETARGET: renderTargeting();
        break;
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
    g_Console.writeToBuffer(ca, "2. Load", 0x09);
    ca.Y += 2;
    ca.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(ca, "3. Quit", 0x09); // Main page
    arrow();

}
void arrow()
{
    g_Console.writeToBuffer(cb, "-->", 0x09);
}
//creates the shop screen
void renderShop()
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
    c.X = (g_Console.getConsoleSize().X / 10) * 2;
    ss.str("Exit");
    g_Console.writeToBuffer(c, ss.str(), 0x07);

    c.Y = (g_Console.getConsoleSize().Y / 10) * 9;
    c.X = (g_Console.getConsoleSize().X / 10) * 4;
    ss.str("Buy");
    g_Console.writeToBuffer(c, ss.str(), 0x07);

    c.Y = (g_Console.getConsoleSize().Y / 10) * 9;
    c.X = (g_Console.getConsoleSize().X / 10) * 6;
    ss.str("Item ability");
    g_Console.writeToBuffer(c, ss.str(), 0x07);
}

void renderInventory()
{
    COORD c;
    std::ostringstream ss;

    //SHOP TITLE
    c.Y = g_Console.getConsoleSize().Y / 10;
    c.X = (g_Console.getConsoleSize().X / 13)*6;
    ss.str(" YOUR BACKPACK");
    g_Console.writeToBuffer(c, ss.str(), 0x07);

    /*Player's items all displayed below*/
    for (int i = 0; i < 5; i++)
    {
        c.Y = (g_Console.getConsoleSize().Y / 10) * (i + 3);
        c.X = (g_Console.getConsoleSize().X / 10) * 2;
        ss.str(PlayerInventory.GetItem(i)->GetName());
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
    for (int i = 5; i < 10; i++)
    {
        c.Y = (g_Console.getConsoleSize().Y / 10) * (i - 2);
        c.X = (g_Console.getConsoleSize().X / 10) * 7;
        ss.str(PlayerInventory.GetItem(i)->GetName());
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
}

void renderMap()
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
    //    g_Console.writeToBuffer(c, " °±²Û", colors[i]);   // colour combi
    //}
    if (c.X = 91, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "²²²²²", colors[5]); //fountain
    }
    if (c.X = 65, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "²²²²²", colors[5]); //fountain
    }
    if (c.X = 10, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[3]); // BOSS
    }
    if (c.X = 70, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 75, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 78, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }

    if (c.X = 41, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 41, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 67, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 67, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 67, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 67, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 67, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 67, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 53, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°° ", colors[1]);
    }
    if (c.X = 50, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 50, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 55, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 50, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 50, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 50, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 50, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 50, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 50, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }

    if (c.X = 40, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }

    if (c.X = 125, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 125, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }

    if (c.X = 60, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }if (c.X = 5, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 10, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }
    //---------------------------------------------------
    if (c.X = 140, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }

    if (c.X = 78, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 82, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 86, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 21, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 31, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 29, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 91, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 96, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }

    if (c.X = 106, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 106, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 106, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 120, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 115, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 110, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 115, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 115, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 115, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 115, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 115, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 110, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°", colors[1]);
    }
    if (c.X = 95, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 101, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 140, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 0, c.Y = 28)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 128, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 134, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°", colors[1]);
    }
    if (c.X = 94, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 105, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 105, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 105, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 119, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 119, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 119, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 128, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 131, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 128, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 131, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 80, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 80, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 80, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 80, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 80, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 70, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 70, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 60, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 60, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 40, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 40, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 40, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 40, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 40, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 40, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 25, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 25, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }

    if (c.X = 30, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 5, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 15, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = 3, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 0, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 145, c.Y = 28)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°", colors[1]);
    }
    if (c.X = 0, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, "°°°°°°°°°°°°°°°", colors[1]);
    }
    if (c.X = -1, c.Y = 1)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 2)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 3)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 4)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 5)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 6)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 7)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 8)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 9)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 10)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 11)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 12)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 13)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 14)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 15)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 16)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 17)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 18)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 19)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 20)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 21)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 22)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 23)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 24)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 25)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 26)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 27)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }
    if (c.X = -1, c.Y = 28)
    {
        colour(colors[1]);
        g_Console.writeToBuffer(c, " °", colors[1]);
    }

}
//----------------------------------------------------------------------------------------------------------------------------------------------

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

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
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
    renderSelect();
    renderEnemyHealth();
}

void renderSelect()
{
    // Draw the location of the character
    WORD charColor = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;

    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)0, charColor);
}

void renderBattleScreen()
{
    COORD c;
    std::ostringstream ss;

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
//creates the shop screen



void renderTargeting()
{
    renderTargetingScreen();
    renderSelect();
    renderEnemyHealth();
}
void renderTargetingScreen()
{
    COORD c;
    std::ostringstream ss;

    //target 1
    if (TargetParty->GetPartyClass(0) != nullptr)
    {
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        c.X = (g_Console.getConsoleSize().X / 8);

        ss.str("");
        ss << " 1." << TargetParty->GetPartyClass(0)->GetName();
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }

    //target 2
    if (TargetParty->GetPartyClass(1) != nullptr)
    {
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 4);
        c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));

        ss.str("");
        ss << " 2." << TargetParty->GetPartyClass(1)->GetName();
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }

    //target 3
    if (TargetParty->GetPartyClass(2) != nullptr)
    {
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
        c.X = (g_Console.getConsoleSize().X / 8);

        ss.str("");
        ss << " 3." << TargetParty->GetPartyClass(2)->GetName();
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }

    //target 4
    if (TargetParty->GetPartyClass(3) != nullptr)
    {
        c.Y = g_Console.getConsoleSize().Y - (g_Console.getConsoleSize().Y / 8);
        c.X = ((g_Console.getConsoleSize().X / 8) + (g_Console.getConsoleSize().X / 2));


        ss.str("");
        ss << " 4." << TargetParty->GetPartyClass(3)->GetName();
        g_Console.writeToBuffer(c, ss.str(), 0x07);
    }
}

void renderEnemyHealth()
{
    //temporary
    COORD c;
    std::ostringstream ss;

    c.X = g_Console.getConsoleSize().X / 2;
    c.Y = g_Console.getConsoleSize().Y / 2;

    for (int i = 0; i < 4; i++)
    {
        if (EnemyParty->GetPartyClass(i + 4) != nullptr)
        {
            c.X = g_Console.getConsoleSize().X / 2;
            c.Y = g_Console.getConsoleSize().Y / 2 + i;
            ss.str("");
            ss << i + 1 << "." << EnemyParty->GetPartyClass(i)->GetName() << " HP:" <<
                EnemyParty->GetPartyClass(i)->GetHealth() << "/" << EnemyParty->GetPartyClass(i)->GetMaxHealth();
            g_Console.writeToBuffer(c, ss.str(), 0x07); \
        }
    }
}

