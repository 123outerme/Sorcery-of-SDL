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
//#include "version.h"

#define bool char
#define false 0
#define true 1
#define windowName "Sorcery of SDL"
#define TILESET_NAME "SOUVUTU0.png"
#define FRAMERATE 60
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 768
#define TILE_SIZE 64
#define WIDTH_IN_TILES SCREEN_WIDTH / TILE_SIZE
#define HEIGHT_IN_TILES SCREEN_HEIGHT / TILE_SIZE

#define printBool(x) x == true ? "true" : "false"
#define iPart(x) ((int) x)
#define fPart(x) (x - iPart(x))

typedef struct
{
    int x;
    int y;
    int w;
    int h;
    int tileIndex;
    SDL_Rect* clipRect;
    double mapScreen;
    double lastScreen;
    int overworldX;
    int overworldY;
    bool movementLocked;
} sprite;

void initSprite();
int init();
bool loadIMG();
bool loadTTFont();
int* loadTextTexture();
void loadMapFile();
void drawGame();
void drawPlayer();
void drawTilemap();
void drawTile();
void drawTextBox();
void mainLoop();
bool checkKeyPress();
void cleanSprites();
void closeSDL();

char* toString();
int digits();
int pwrOf10();

SDL_Window* mainWindow;
SDL_Surface* mainScreen;
SDL_Texture* tilesetTexture;
SDL_Renderer* mainRenderer;
bool textBoxOn;
TTF_Font* mainFont;
TTF_Font* smallFont;
int tilemap[HEIGHT_IN_TILES][WIDTH_IN_TILES];

#endif // OUTERSDL_H_INCLUDED
