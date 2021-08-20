#ifndef _GAME_H
#define _GAME_H
#include "Class.h"
#include "Framework\timer.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
struct SMouseEvent
{
    COORD mousePosition;
    DWORD buttonState;
    DWORD eventFlags;
};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE,
    K_COUNT,
    K_TAB
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_MENUSCREEN,
    S_GAMEPAUSE,
    S_GAME,
    S_COUNT,
    S_BATTLE,
    S_BATTLETARGET,
    S_INVENTORY,
    S_SHOP
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void splashScreenWait();    // waits for time to pass in splash screen
void updateGame();          // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void foundRandomEncounter(); //function that handles if there is na random enounter or not
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void updateBattle(); //Battle logic
void TurnStart(); //initializes a turn in battle
void BattleMove(); //Checks for movement keys in battle
void BattleSelect(); // checks selection for initial battle menu
void initEnemyGroup(int EnemyGroup); //initialize enemy groups
void updateBattle2();
void BattleAttack();
void SelectTarget(Class* TargetParty[]);
void SelectSpecialAction();
void SelectSkill();
void CheckTargetType(int type);
void ExecuteSkill(int SkillIndex);
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderShopScreen();    //renders the shopping screen
void renderInventoryScreen();//renders the screen for the player's inventory
void InventorySelect(Inventory* PlayerInventory, Class* Player1,
    Class* Player2, Class* Player3, Class* Player4);//function for selecting inventory items
void ShopSelect(Items* SelectedItem, Inventory* ShopInventory, Inventory* PlayerInventory);//shop selecting system
void renderShop();           //renders shop system
void renderInventory();      //renders inventory system
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events
void renderBattle();
void renderSelection();
void renderBattleScreen();
void renderSpecialSelect();
void renderSelectScreen();
void renderEnemyHealth();
void arrow();
void gamepause();

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 


#endif // _GAME_H