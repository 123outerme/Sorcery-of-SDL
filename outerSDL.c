#include "outerSDL.h"

#define checkSKUp currentKeyStates[SDL_SCANCODE_UP]
#define checkSKDown currentKeyStates[SDL_SCANCODE_DOWN]
#define checkSKLeft currentKeyStates[SDL_SCANCODE_LEFT]
#define checkSKRight currentKeyStates[SDL_SCANCODE_RIGHT]
#define checkSKSpace currentKeyStates[SDL_SCANCODE_SPACE]

#define TILE_ID_DOOR 22

void initSprite(sprite* spr, int x, int y, int size, int tileIndex)
{
    spr->x = x;
	spr->y = y;
	spr->w = size;
	spr->h = size;
	spr->mapScreen = 1.1;
	spr->lastScreen = 1.1;
	spr->overworldX = x;
	spr->overworldY = y;
	spr->tileIndex = tileIndex;
	spr->clipRect = &((SDL_Rect){.x = (tileIndex / 8) * size, .y = (tileIndex % 8) * size, .w = size, .h = size});
	spr->movementLocked = false;
}

int init()
{
    int done = 0;
    SDL_Init(SDL_INIT_VIDEO);
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
         done = -1;
    }
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        done = -2;
    }
    mainWindow = NULL;
    mainScreen = NULL;
    tilesetTexture = NULL;
    mainRenderer = NULL;
    mainFont = NULL;
    textBoxOn = false;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return 1;
    }
    else
    {
        mainWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!mainWindow)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 2;
        }
        else
        {
            mainScreen = SDL_GetWindowSurface(mainWindow);
            mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
            if(!mainRenderer)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                return 3;
            }
            else
            {
                SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderSetLogicalSize(mainRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
                SDL_RenderClear(mainRenderer);
                loadTTFont("Texas-Instruments-TI-84_X.ttf", &mainFont, 64);
                loadTTFont("Texas-Instruments-TI-84_X.ttf", &smallFont, 32);
                if (!mainFont || !smallFont)
                {
                    printf("%s could not be created! SDL Error: %s\n", !mainFont ? "mainFont" : "smallFont", TTF_GetError());
                    return -3;
                }
                else
                    loadMapFile("maps.txt", tilemap, 1, HEIGHT_IN_TILES, WIDTH_IN_TILES);
                    if (tilemap[0][0] == -1)
                    {
                        printf("Tilemap could not load! Error code 5.");
                        return 5;
                    }
                    else
                    {
                        loadIMG(TILESET_NAME, &tilesetTexture);
                        if (!tilesetTexture)
                        {
                            printf("Tileset could not load! SDL Error: %s\n", SDL_GetError());
                            return 6;
                        }
                        else;
                    }
            }
        }
    }
    return done;
}

bool loadIMG(char* imgPath, SDL_Texture** dest)
{
    SDL_Surface* surf = IMG_Load(imgPath);
    SDL_SetColorKey(surf, 1, SDL_MapRGB(surf->format, 255, 28, 198));
    *dest = SDL_CreateTextureFromSurface(mainRenderer, surf);
    if (!*dest)
    {
        printf("Unable to load image/create texture for %s! SDL_Error: %s\n", imgPath, SDL_GetError());
        return false;
    }
    SDL_FreeSurface(surf);
    return true;
}

bool loadTTFont(char* filePath, TTF_Font** dest, int sizeInPts)
{
    *dest = TTF_OpenFont(filePath, sizeInPts);
    if (!*dest)
    {
        printf( "Font could not be loaded! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    return true;
}

int* loadTextTexture(char* text, SDL_Texture** dest, SDL_Color color, int isBlended)
{
    static int wh[] = {0, 0};
    SDL_Surface* txtSurface;
    if (isBlended)
        txtSurface = TTF_RenderText_Blended_Wrapped(mainFont, text, color, 63 * SCREEN_WIDTH / 64);
    else
    {
        SDL_Color color2 = {81, 81, 81};
        txtSurface = TTF_RenderText(smallFont, text, color, color2);
    }
    *dest = SDL_CreateTextureFromSurface(mainRenderer, txtSurface);
    if (!*dest)
    {
        printf("Text texture could not be loaded! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        wh[0] = txtSurface->w;
        wh[1] = txtSurface->h;
    }
    SDL_FreeSurface(txtSurface);
    return wh;
}

void loadMapFile(char* filePath, int* array, const int lineNum, const int y, const int x)
{
	FILE* filePtr;
	filePtr = fopen(filePath,"r");
	if (!filePtr)
	{
		printf("Error opening file!\n");
		*array = -1;
	}
	else
	{
		int numsC = 0, numsR = 0,  i = 0, flag = 0;
		char c;
		char thisLine[1024];
		for(int p = 0; p <= lineNum; p++)
			fgets(thisLine, 1024, filePtr);
		//printf("%s\n\n", thisLine);
		while (i != 1023)
			{
				if (thisLine[i] != '[')
					break;
				i++;
				//printf("%d\n", i);
			}
			i--;
			c = thisLine[i];
			//printf("thisLine[%d] = %c\n", i, c);
		while (flag < 3)
		{
			if (flag > 0)
				flag--;
			c = thisLine[i++];
			//printf("thisLine[%d] = %c\n", i, c);
			if (c == ',' || c == '[')
			{
				//fseek(filePtr, i++, SEEK_SET);
				sscanf(thisLine + i++, "%d", array + numsR++ + numsC * x);
				//printf("^");
				flag = 0;
			}
			if (c == ']')
			{
				numsC++;
				numsR = 0;
				flag += 2;

			}
			//printf("nums[%d][%d]\n", numsC, numsR);
		}
		//for(int dy = 0; dy < y; dy++)
		//{
		//	for(int dx = 0; dx < x; dx++)
		//	{
		//		printf("%d,", *(array + dx + dy * x));
		//	}
		//	printf("\n");
		//}
		//printf("nums[%d][%d]\n", numsC, numsR);
		fclose(filePtr);
	}
}

void drawGame(sprite* player)
{
    //SDL_RenderClear(mainRenderer);
    drawTile(player->tileIndex, player->x / player->w, player->y / player->w, player->w);
    //SDL_BlitScaled(player->sprSurface, NULL, mainScreen, playerRect);
    if (textBoxOn)
        drawTextBox("Hello world. I would like to tell you a story today. Once there was a programmer who hated things, so he made me.");
    SDL_RenderPresent(mainRenderer);
}

void drawTilemap(int startX, int startY, int endX, int endY)
{
    //printf("");
    for(int dy = startY; dy < endY; dy++)
    {
        for(int dx = startX; dx < endX; dx++)
            drawTile(tilemap[dy][dx], dx, dy, TILE_SIZE);
        SDL_Delay(20);
    }

}

void drawTile(int id, int tileX, int tileY, int width)
{
    //printf("%d , %d\n", id  / 8, (id % 8));
    SDL_RenderCopy(mainRenderer, tilesetTexture, &((SDL_Rect){.x = (id / 8) * width, .y = (id % 8) * width, .w = width, .h = width}), &((SDL_Rect){.x = tileX * width, .y = tileY * width, .w = width, .h = width}));

    //SDL_RenderPresent(mainRenderer);
}

void drawTextBox(char* input)
{
    //36 letters per line/5 lines at 64pt font
    SDL_SetRenderDrawColor(mainRenderer, 0x51, 0x51, 0x51, 0xFF);
    SDL_Rect rect = {.y = 17 * SCREEN_HEIGHT / 32, .w = SCREEN_WIDTH, .h = 15 * SCREEN_HEIGHT / 32};
    SDL_RenderFillRect(mainRenderer, &rect);
    SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Texture* txtTexture = NULL;
    int* wh;
    wh = loadTextTexture(input, &txtTexture, (SDL_Color){0, 0, 0}, true);
    //printf("%d * %d \n", *wh, *(wh + 1));
    const SDL_Rect* txtRect = &((SDL_Rect){.x =  SCREEN_WIDTH / 64, .y = 9 * SCREEN_HEIGHT / 16, .w = *wh,
                                           .h = *(wh + 1) > 7 * SCREEN_HEIGHT / 16 ? 320 : *(wh + 1)});
    const SDL_Rect* boundRect = &((SDL_Rect) {.w = *wh, .h = *(wh + 1) > 7 * SCREEN_HEIGHT / 16 ? 320 : *(wh + 1)});
    SDL_RenderCopy(mainRenderer, txtTexture, boundRect, txtRect);
    SDL_free(&rect);
    SDL_free(&txtRect);
    SDL_DestroyTexture(txtTexture);
    SDL_free(&boundRect);
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

        if (press)
            frame++;

        if (frame % 7 == 0)
        {
            frame = 0;
            press = false;
        }

        SDL_Delay((1000 / FRAMERATE));
    }
}

bool checkKeyPress(sprite* playerSprite)
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (checkSKUp || checkSKDown || checkSKLeft || checkSKRight)
    {
        drawTile(tilemap[playerSprite->y / TILE_SIZE][playerSprite->x / TILE_SIZE],
                 playerSprite->x / TILE_SIZE, playerSprite->y / TILE_SIZE, TILE_SIZE);
        if (playerSprite->y > 0 && checkSKUp)
            playerSprite->y-= TILE_SIZE;
        if (playerSprite->y < SCREEN_HEIGHT - playerSprite->h && checkSKDown)
            playerSprite->y+= TILE_SIZE;
        if (playerSprite->x > 0 && checkSKLeft)
            playerSprite->x-= TILE_SIZE;
        if (playerSprite->x < SCREEN_WIDTH - playerSprite->w && checkSKRight)
            playerSprite->x+= TILE_SIZE;
        if (tilemap[playerSprite->y / TILE_SIZE][playerSprite->x / TILE_SIZE] == TILE_ID_DOOR)
        {
            int map = 0;
            if (fPart(playerSprite->mapScreen))
            {
                playerSprite->mapScreen = iPart(playerSprite->mapScreen);
                playerSprite->overworldX = playerSprite->x;
                playerSprite->overworldY = playerSprite->y;
                playerSprite->x = 576;
                playerSprite->y = 576;
            }
            else
            {
                playerSprite->mapScreen = playerSprite->lastScreen;
                playerSprite->x = playerSprite->overworldX;
                playerSprite->y = playerSprite->overworldY + TILE_SIZE;
                map = (int) 10 * fPart(playerSprite->mapScreen) + iPart(playerSprite->mapScreen - 1) * 8;
                //map = 1;
            }
            loadMapFile("maps.txt", tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
            drawTilemap(0, 0, WIDTH_IN_TILES, HEIGHT_IN_TILES);
        }
        if (!playerSprite->x|| !playerSprite->y || playerSprite->x == SCREEN_WIDTH - TILE_SIZE || playerSprite->y == SCREEN_HEIGHT - TILE_SIZE)
        {
            printf("wee!");
        }
    }

    if (checkSKSpace)
    {
            textBoxOn = !textBoxOn;
        if (!textBoxOn)
            drawTilemap(0, 6, WIDTH_IN_TILES, HEIGHT_IN_TILES);
    }

    if (checkSKUp || checkSKDown || checkSKLeft || checkSKRight || checkSKSpace)
            return true;
        return false;
}

void cleanSprites(sprite* sprites[], size_t elems)
{
    for(int i = 0; i < elems; i++)
    {
        sprites[i] = &((sprite){.x = 0, .y = 0, .w = 0, .h = 0, .tileIndex = 0, .mapScreen = 0, .lastScreen = 0,
                       .overworldX = 0, .overworldY = 0, .movementLocked = false});
    }
}

void closeSDL()
{
    TTF_CloseFont(mainFont);
    TTF_CloseFont(smallFont);
	SDL_DestroyTexture(tilesetTexture);
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainRenderer);
    SDL_Quit();
}

char * toString(int value, char * result)
{
    if (value == 0)
        return 0;
	int digit = digits(value);
	//printf("digit = %d\n", digit);
	result = calloc(digit + 1, sizeof(char));
	result[digit] = '\0';
	int usedVal = 0;
	for (int i = digit; i > 0; i--)
	{
		int x = (value - usedVal) / pwrOf10(i - 1);
		result[digit - i] = (char) x + '0';
		//printf("result[%d] = (%d) / %d = %d = character %c\n", digit - i, value - usedVal, pwrOf10(i - 1), x, result[digit - i] - '0');
		usedVal = usedVal + (result[digit - i] - '0') * pwrOf10(i - 1);
		//printf("usedVal = itself + %d * %d =  %d\n", (int) result[digit - i] - '0', pwrOf10(i - 1), usedVal);
	}
	//printf("%s\n",result);
	return result;
}
int digits(int num)
{
	return 1 + log10(num);
}
int pwrOf10(int power)
{
	int val = 1;
	int i = power;
	while (i > 0)
	{
		val *= 10;
		i--;
	}
	return val;
}
