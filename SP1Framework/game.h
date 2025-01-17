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

enum EKEYS
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE,
    K_TAB,
    K_COUNT
};


// Enumeration for the different screen states
enum EGAMESTATES
{
    S_HOWTOPLAY,
    S_MENUSCREEN,
    S_GAMEPAUSE,
    S_MAP1,
    S_MAP2,
    S_COUNT,
    S_BATTLE,
    S_INVENTORY,
    S_SHOP,
    S_BATTLESPLASH
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

void init(void);      // initialize your variables, allocate memory, etc
void getInput(void);      // get input from player
void update(double dt); // update the game and the state of the game
void render(void);      // renders the current state of the game to the console
void shutdown(void);      // do clean up, free memory
void CheckBoss();
void Collision();
void Colision2();
void splashScreenWait();    // waits for time to pass in splash screen
void updateGame();          // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void foundRandomEncounter(); //function that handles if there is na random enounter or not
void BattleSplash();
void renderBattleSplash();
void initEnemyGroup(int EnemyGroup); //initialize enemy groups
/*void processUserInput(); */   // checks if you should change states or do something else with the game, e.g. pause, exit
void updateBattle(); //Battle logic
void TurnStart(); //initializes a turn in battle
void BattleMove(); //Checks for movement keys in battle
void BattleSelect(); // checks selection for initial battle menu
void BattleAttack();
void SelectTarget(Class* TargetParty[]);
void SelectSpecialAction();
void SelectSkill();
void CheckTargetType(int type);
void ExecuteSkill(int SkillIndex);
void ResetCursorPosition();
void CheckStatuses();
void TurnEnd();
void RoundEnd();
void VictoryCondition();
void VictorySplash();
void EndBattle();
void EnemyAI(int EnemyPartyType);
void GameOver();
void GameOverSplash();
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void updateInventory();
void inventoryOpened();
void renderInventoryScreen();
void InventorySelection();
void renderInventory();
void renderShop();
void renderShopScreen();
void shopOpened();
void ShopSelect();
void updateShop();
void renderGame();          // renders the game stuff
void renderMap1();           // renders the map to the buffer first
void renderMap2();
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderBattle();
void renderSelection();
void renderBattleScreen();
void renderStatuses();
void arrow();
void rendergamepause();
void renderhowtoplay();
void howtoplaybutton();
void changelevel();
void renderGame2();

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 


#endif // _GAME_H