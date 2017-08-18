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

int main(int argc, char* args[])
{
	//Start up SDL and create window
	sprite player;
	initSprite(&player, 4 * TILE_SIZE, 6 * TILE_SIZE, TILE_SIZE, 17);

	int succeeded = init(&player);
	if (!succeeded)
    {
        mainLoop(&player);
        printf("Ended at %d, %d underneath a tile of index %d in map id %f", player.x, player.y, tilemap[player.y / TILE_SIZE][player.x / TILE_SIZE], player.mapScreen);
        sprite sprites[1] = {&player};
        cleanSprites(sprites, 1);
        closeSDL();
	}
	return succeeded;
}
