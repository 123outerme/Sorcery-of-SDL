#include "outerSDL.h"

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
  error code 6: Media could not load/optimize
*/

#define TILE_ID_CURSOR 16
#define TILE_ID_PLAYER 17

#define MENU_QUIT 0
#define MENU_NEW_SAVE 1
#define MENU_LOAD_SAVE 2
#define MENU_HELP 3

//10208C (hex) / 16, 32, 140 (integers) -> dark blue color

int main(int argc, char* args[])
{
	//Start up SDL and create window
	sprite player;
	int succeeded = init(&player);
	if (!succeeded)
    {
        mainMenu();
        mainLoop(&player);
        printf("Ended at %d, %d underneath a tile of index %d in map id %f\n", player.x, player.y, tilemap[player.y / TILE_SIZE][player.x / TILE_SIZE], player.mapScreen);
        savePlayerData(&player, SAVE_DATA_NAME);
        //printf("Ended at %d, %d underneath a tile of index %d in map id %f\n", player.x, player.y, tilemap[player.y / TILE_SIZE][player.x / TILE_SIZE], player.mapScreen);
        closeSDL();
	}
	return succeeded;
}

int mainMenu()
{
    sprite cursor;
    initSprite(&cursor, TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_ID_CURSOR);
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    //background text (drawn first)
    drawText("Sorcery of Uvutu", 19 * SCREEN_WIDTH / 128, 9 * SCREEN_HEIGHT / 128, 119 * SCREEN_HEIGHT / 128, (SDL_Color){24, 65, 214}, false);
    //foreground text
    drawText("Sorcery of Uvutu", 9 * SCREEN_WIDTH / 64, 4 * SCREEN_HEIGHT / 64, 55 * SCREEN_HEIGHT / 64, (SDL_Color){24, 162, 239}, false);

    drawText("NEW GAME", 2 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, (TILE_SIZE - 5) * TILE_SIZE, (SDL_Color){24, 195, 247}, false);
    drawText("LOAD GAME", 2 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, (TILE_SIZE - 6) * TILE_SIZE, (SDL_Color){24, 195, 247}, false);
    drawText("HELP", 2 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, (TILE_SIZE - 7) * TILE_SIZE, (SDL_Color){24, 195, 247}, false);
    drawText("QUIT", 2 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, (TILE_SIZE - 8) * TILE_SIZE, (SDL_Color){24, 195, 247}, false);
    drawText(FULLVERSION_STRING, 2 * TILE_SIZE + TILE_SIZE / 4, 10 * TILE_SIZE, (TILE_SIZE - 10) * TILE_SIZE, (SDL_Color){24, 195, 247}, true);

    int i = 1;
    while(i)
    {
        i = 0;
        drawTile(cursor.tileIndex, cursor.x / TILE_SIZE, cursor.y / TILE_SIZE, cursor.w);
        SDL_RenderPresent(mainRenderer);
    }

    SDL_Delay(2000);
    return MENU_LOAD_SAVE;
}

void drawGame(sprite* player)
{
    drawTile(player->tileIndex, player->x / player->w, player->y / player->w, player->w);
    if (textBoxOn)
        drawTextBox("Hello world. I would like to tell you a story today. Once there was a programmer who hated things, so he made me.");
    SDL_RenderPresent(mainRenderer);
}

void mainLoop(sprite* playerSprite)
{
    bool quit = false;
    SDL_Event e;
    int frame = 0;
    bool press = false;
    drawTilemap(0, 0, WIDTH_IN_TILES, HEIGHT_IN_TILES);

    while(!quit)    //this is the literal main loop right here
    {
        drawGame(playerSprite);
        while(SDL_PollEvent(&e) != 0)  //while there are events in the queue
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        if (frame == 0)
            press = checkKeyPress(playerSprite);

        frame+= press;

        if (frame % 7 == 0)
        {
            frame = 0;
            press = false;
        }

        SDL_Delay((1000 / FRAMERATE));
    }
}
