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
#include "SDL/SDL_ttf.h"   //This is included for textbox text stuff
#include <stdio.h>         //This is included because it's fundamental always. Even if it ain't needed
#include <string.h>        //This is included for strncat
#include <stdlib.h>        //This is included for calloc
#include <math.h>          //This is included for log10
#include "version.h"

#define bool char
#define false 0
#define true 1
#define windowName "Sorcery of SDL"
#define TILESET_NAME "SOUVUTU0.png"
#define MAP_DATA_NAME "map.txt"
#define SAVE_DATA_NAME "SAVUVUTU.txt"
#define FRAMERATE 60
#define SCREEN_WIDTH TILE_SIZE * 20
#define SCREEN_HEIGHT TILE_SIZE * 15
#define TILE_SIZE 48
#define WIDTH_IN_TILES SCREEN_WIDTH / TILE_SIZE
#define HEIGHT_IN_TILES SCREEN_HEIGHT / TILE_SIZE

#define printBool(x) x == true ? "true" : "false"
#define iPart(x) ((int) x)
#define fPart(x) (x - iPart(x))

typedef struct
{
    int x;  //
    int y;  //
    int w;  //
    int h;  //
    int tileIndex;  //
    int steps; //
    SDL_Rect* clipRect;  // 16? bytes (4 ints)
    int worldNum;  //
    double mapScreen;  //8 bytes
    double lastScreen;  // 8 bytes
    int overworldX;  //
    int overworldY;  //
    bool movementLocked;  // 1 byte
} sprite;

int mainMenu();
void drawGame();
void mainLoop();

int init();
bool loadIMG();
bool loadTTFont();
int* loadTextTexture();
void initSprite();
void loadSpriteData(sprite* spr, char* filePath, bool forceNew);
void loadMapFile();
void drawTilemap();
void drawTile();
void drawTextBox();
bool checkKeyPress();
bool checkCollision();
void savePlayerData(sprite* player, char* filePath);
void cleanSprites(sprite* sprites[], size_t elems);
void closeSDL();

char* toString(int value, char * result);
int digits(int num);
int pwrOf10(int power);
int checkArrayForVal(double value, double* array, size_t arraySize);
double absFloat(double val);
int createFile(char* filePath);
int writeLine(char* filePath, char* stuff);
char* readLine(char* filePath, int lineNum, char** output);


SDL_Window* mainWindow;
SDL_Surface* mainScreen;
SDL_Texture* tilesetTexture;
SDL_Renderer* mainRenderer;
bool textBoxOn;
TTF_Font* mainFont;
TTF_Font* smallFont;
int tilemap[HEIGHT_IN_TILES][WIDTH_IN_TILES];

#endif // OUTERSDL_H_INCLUDED
