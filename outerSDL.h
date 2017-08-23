#ifndef OUTERSDL_H_INCLUDED
#define OUTERSDL_H_INCLUDED

/*
 error code -3; TTFs failed to load
 error code -2: SDL_ttf failed to initialize
 error code -1: SDL_img failed to initialize
  error code 0: No error
  error code 1: SDL system failed to initialize
  error code 2: Window could not be created
  error code 3: Renderer failed to initialize
  error code 4: TTF font failed to load
  error code 5: tilemap failed to load
*/

#include "SDL/SDL.h"       //This is included because it's an SDL2 program... duh
#include "SDL/SDL_image.h" //This is included so we can use PNGs.
#include "SDL/SDL_ttf.h"   //This is included for text stuff
#include <stdio.h>         //This is included because it's fundamental always. Even if it ain't needed
#include <string.h>        //This is included for strncat and other string functions
#include <stdlib.h>        //This is included for calloc and a lot of other stuff
#include <math.h>          //This is included for log10
#include <ctype.h>         //This is included for toupper
#include "version.h"       //This is included for version display at main menu

#define bool char
#define false 0
#define true 1
#define windowName "Sorcery of SDL"
#define TILESET_FILE_NAME "SOUVUTU0.png"
#define MAP_FILE_NAME "maps.bin"
#define SAVE_FILE_NAME "SAVUVUTU.bin"
#define FONT_FILE_NAME "Px437_ITT_BIOS_X.ttf"
#define FRAMERATE 60
#define SCREEN_WIDTH TILE_SIZE * 20
#define SCREEN_HEIGHT TILE_SIZE * 15
#define TILE_SIZE 48
#define WIDTH_IN_TILES SCREEN_WIDTH / TILE_SIZE
#define HEIGHT_IN_TILES SCREEN_HEIGHT / TILE_SIZE

#define printBool(x) x == true ? "true" : "false"
#define iPart(x) ((int) x)
#define fPart(x) (x - iPart(x))

#define PLAYER_NAME_LIMIT 6

#define KEYPRESS_RETURN_MENU 2
#define KEYPRESS_RETURN_BATTLE 3
#define KEYPRESS_RETURN_BREAK 4
#define MAIN_MENU_PALETTE (SDL_Color){16, 32, 140}, (SDL_Color){24, 65, 214}, (SDL_Color){24, 162, 239}, (SDL_Color){24, 195, 247}

typedef enum
{
    type_na,  //0
    type_chest,  //1
    type_npc,  //2
    type_boss,  //3
    type_player,  //4
} entityType;

typedef struct
{
    int x;  //
    int y;  //
    int w;  //
    int h;  //
    int tileIndex;  //
    SDL_Rect* clipRect;  // 16? bytes (4 ints)
    entityType type;
} sprite;

typedef struct {
    sprite spr;  //?
    char name[PLAYER_NAME_LIMIT + 1];  //7 bytes
    int level;  //
    int experience;
    int HP;  //
    int maxHP;  //
    int attack;  //
    int speed;  //
    int statPts;  //
    int move1;  //
    int move2;  //
    int move3;  //
    int move4;  //
    int steps;  //
    int worldNum;  //
    double mapScreen;  //8 bytes
    double lastScreen;  //8 bytes
    int overworldX;  //
    int overworldY;  //
    SDL_RendererFlip flip;  //
    bool movementLocked;  // 1 byte
} player;

void drawGame(player* player, char* textInput);  //draws overworld stuff
int mainLoop(player* playerSprite);  //does main overworld loop
void drawHUD(player* player);  //draws HUD in overworld
void drawTextBox(char* input, player* player);  //draws the NPC text box
int aMenu(char* title, char* opt1, char* opt2, char* opt3, char* opt4, char* opt5, const int options, int curSelect, SDL_Color bgColor, SDL_Color titleColorUnder, SDL_Color titleColorOver, SDL_Color textColor, bool border, bool showVersion);  //menu
int aWallOfText(char* title, char* text, bool showHelpInfo);  //draws a wall of text for the player to read & dismiss
int showStats(player* player);  //opens stats display
int showItems(player* player);  //shows list of player items
bool doBattle(player* player);  //does a battle

int init();  //inits SDL and necessary game systems
bool startGame(player* playerSprite, bool newSave);  //inits player sprite, tilemap
bool loadIMG(char* imgPath, SDL_Texture** dest);  //loads an image from a file into a texture
bool loadTTFont(char* filePath, TTF_Font** dest, int sizeInPts);  //loads a .ttf file into an SDL font
int* loadTextTexture();  //loads a texture from inputted text
void initSprite(sprite* spr, int x, int y, int size, int tileIndex, entityType type);  //initializes a new sprite
void initPlayer(player* player, int x, int y, int size, int tileIndex);  //initializes a new player
void loadPlayerData(player* player, char* filePath, bool forceNew);  //loads data from filePath. If not, or forceNew = true, inits new sprite.
void inputName(player* player);  //gets the name of the sprite by prompting player
void loadMapFile();  //loads a map from a file
void drawTilemap(int startX, int startY, int endX, int endY);  //draws a tilemap to the screen
void drawTile(int id, int tileX, int tileY, int width, SDL_RendererFlip flip);  //draws a tile to the screen
void drawText(char* input, int x, int y, int maxH, SDL_Color color, bool render);  //draws text to the screen
bool checkKeyPress();  //checks if a key was pressed and acts accordingly if so
bool checkCollision();  //checks if player has collided with a solid tile
void savePlayerData(player* player, char* filePath);  //saves sprite data to a file
void closeSDL();  //closes out SDL and necessary game systems

char* toString(int value, char * result);  //turns inputted int into a string
int digits(int num);  //gets the number of digits an int has
int pwrOf10(int power);  //gets input ^ 10
void readStringInput(char* str, int limit);  //gets string input from the console, letter-by-letter
void freeThisMem(int** x);  //frees memory of any type/object/whatever and nulls its pointer.
char* removeNewline(char input[], size_t length);  //removes the \n character from a string
int checkArrayForVal(double value, double* array, size_t arraySize);  //checks a 1D array of floats for a single float
int createFile(char* filePath);  //creates a file if it doesn't exist; if it does, clears it out
bool checkFile(char* filePath);  //checks if a file exists
int writeLine(char* filePath, char* stuff);  //appends a line to a file
char* readLine(char* filePath, int lineNum, char** output);  //reads a certain line from a file


SDL_Window* mainWindow;
SDL_Surface* mainScreen;
SDL_Texture* tilesetTexture;
SDL_Renderer* mainRenderer;
bool textBoxOn;
TTF_Font* mainFont;
TTF_Font* smallFont;
int tilemap[HEIGHT_IN_TILES][WIDTH_IN_TILES];

#endif // OUTERSDL_H_INCLUDED
