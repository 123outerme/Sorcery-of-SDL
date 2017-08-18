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
	initSprite(&player, 0, 0, 64, 17);

	int succeeded = init();
	if (succeeded == 0)
    {
        mainLoop(&player);
        printf("Ended at %d, %d", player.x, player.y);
        sprite sprites[1] = {&player};
        cleanSprites(sprites, 1);
        closeSDL();
	}
	return succeeded;
}
