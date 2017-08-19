#include "outerSDL.h"

#define checkSKUp currentKeyStates[SDL_SCANCODE_UP]
#define checkSKDown currentKeyStates[SDL_SCANCODE_DOWN]
#define checkSKLeft currentKeyStates[SDL_SCANCODE_LEFT]
#define checkSKRight currentKeyStates[SDL_SCANCODE_RIGHT]
#define checkSKSpace currentKeyStates[SDL_SCANCODE_SPACE]

#define TILE_ID_TREE 8
#define TILE_ID_LAVA 9
#define TILE_ID_BOULDER 10
#define TILE_ID_DOOR 22
#define TILE_ID_WINDOW 23
#define TILE_ID_LIT_WINDOW 24
#define TILE_ID_COUNTER_TOP 27
#define TILE_ID_HOUSE_BACK_WALL 28
#define TILE_ID_BAD_WOOD_FLOOR 29
#define TILE_ID_DARKNESS 127
#define ARRAY_OF_MAP_IDS_ALT  {iPart(player->mapScreen), 1.1, 1.2, 1.21, 1.22, 1.31, 1.32, 1.33, 2.1, 2.11, 2.12, 2.21, 2.22, 2.23, 2.33, 3.1, 3.2, 3.22, 3.3, 3.31, 3.32, 4.1, 4.12, 4.21, 4.22, 4.31, 4.32, 5.1, 5.11, 5.12, 5.2, 5.21, 5.22, 5.32, 6.1, 6.11, 6.2, 6.21, 6.22, 6.31, 6.32, 7.1, 7.11, 7.12, 7.13, 7.2, 7.21, 7.22, 7.23, 7.24, 8.1, 8.11, 8.2, 8.21, 8.22, 8.32, 8.33}
#define ARRAY_OF_MAP_IDS {iPart(playerSprite->mapScreen), 1.1, 1.2, 1.21, 1.22, 1.31, 1.32, 1.33, 2.1, 2.11, 2.12, 2.21, 2.22, 2.23, 2.33, 3.1, 3.2, 3.22, 3.3, 3.31, 3.32, 4.1, 4.12, 4.21, 4.22, 4.31, 4.32, 5.1, 5.11, 5.12, 5.2, 5.21, 5.22, 5.32, 6.1, 6.11, 6.2, 6.21, 6.22, 6.31, 6.32, 7.1, 7.11, 7.12, 7.13, 7.2, 7.21, 7.22, 7.23, 7.24, 8.1, 8.11, 8.2, 8.21, 8.22, 8.32, 8.33}
#define SIZE_OF_MAP_ARRAY 57

//10208C (hex) / 16, 32, 140 (integers) -> dark blue color

int init(sprite* player)
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
                loadTTFont("Texas-Instruments-TI-84_X.ttf", &mainFont, 52);
                loadTTFont("Texas-Instruments-TI-84_X.ttf", &smallFont, 24);
                if (!mainFont || !smallFont)
                {
                    printf("%s could not be created! SDL Error: %s\n", !mainFont ? "mainFont" : "smallFont", TTF_GetError());
                    return -3;
                }
                else
                {
                    double arrayOfMaps[] = ARRAY_OF_MAP_IDS_ALT;
                    int map = checkArrayForVal(player->mapScreen, arrayOfMaps, SIZE_OF_MAP_ARRAY);
                    loadMapFile(MAP_DATA_NAME, tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
                    if (tilemap[0][0] == -1 || map == -1)
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
                        else
                            loadSpriteData(player, SAVE_DATA_NAME);
                    }
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
        SDL_Color color2 = {181, 182, 173};
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

void initSprite(sprite* spr, int x, int y, int size, int tileIndex)
{
    spr->x = x;
	spr->y = y;
	spr->w = size;
	spr->h = size;
	spr->tileIndex = tileIndex;
	spr->steps = 0;
	spr->clipRect = &((SDL_Rect){.x = (tileIndex / 8) * size, .y = (tileIndex % 8) * size, .w = size, .h = size});
	spr->mapScreen = 1.1;
	spr->lastScreen = 1.1;
	spr->overworldX = x;
	spr->overworldY = y;
	spr->movementLocked = false;
}

void loadSpriteData(sprite* spr, char* filePath)
{
    char* buffer;
    readLine(filePath, 0, &buffer);
    if (!strncmp(buffer, "0", 1))
	{
	    initSprite(spr, 4 * TILE_SIZE, 6 * TILE_SIZE, TILE_SIZE, 17);
	}
	else
	{
        //initSprite(spr, 0, 0, TILE_SIZE, 17);
        spr->x = strtol(readLine(filePath, 0, &buffer), NULL, 10);
        spr->y = strtol(readLine(filePath, 1, &buffer), NULL, 10);
        spr->w = strtol(readLine(filePath, 2, &buffer), NULL, 10);
        spr->h = spr->w;
        spr->tileIndex = 17;
        spr->steps = strtol(readLine(filePath, 3, &buffer), NULL, 10);
        spr->clipRect = &((SDL_Rect){.x = (spr->tileIndex / 8) * spr->w, .y = (spr->tileIndex % 8) * spr->w, .w = spr->w, .h = spr->w});
        spr->overworldX = strtol(readLine(filePath, 4, &buffer), NULL, 10);
        spr->overworldY = strtol(readLine(filePath, 5, &buffer), NULL, 10);
        spr->movementLocked = (bool) strtol(readLine(filePath, 6, &buffer), NULL, 10);
        spr->mapScreen = strtol(readLine(filePath, 7, &buffer), NULL, 10) / 100.0;
        spr->lastScreen = strtol(readLine(filePath, 8, &buffer), NULL, 10) / 100.0;
	}
}

void loadMapFile(char* filePath, int** array, const int lineNum, const int y, const int x)
{
	FILE* filePtr;
	filePtr = fopen(filePath,"r");
	if (!filePtr)
	{
		printf("Error opening file!\n");
		*array = (int*) 0;
	}
	else
	{
		int numsC = 0, numsR = 0,  i, num;
		int sameArray[y][x];
		char thisLine[601], substring[3];
		for(int p = 0; p < lineNum + 1; p++)
			fgets(thisLine, 602, filePtr);
		//printf("%s\n", thisLine);
		for(i = 0; i < 600; i += 2)
		{
			sprintf(substring, "%.*s", 2, thisLine + i);
			//*(array + numsR++ + numsC * x)
			num = (int)strtol(substring, NULL, 16);
			sameArray[numsC][numsR++] = num;
			//printf("nums[%d][%d] = %d = %d (%s)\n", numsC, numsR - 1, num, sameArray[numsC][numsR - 1], substring);
			if (numsR > x - 1)
			{
				numsC++;
				numsR = 0;
			}
			//printf("%d\n", num);
		}
		for(int dy = 0; dy < y; dy++)
		{
			for(int dx = 0; dx < x; dx++)
				*(array + dx + dy * x) = sameArray[dy][dx];
		}
	}
	fclose(filePtr);
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
    SDL_RenderPresent(mainRenderer);
}

void drawTile(int id, int tileX, int tileY, int width)
{
    //printf("%d , %d\n", id  / 8, (id % 8));
    SDL_RenderCopy(mainRenderer, tilesetTexture, &((SDL_Rect){.x = (id / 8) * width, .y = (id % 8) * width, .w = width, .h = width}),
                   &((SDL_Rect){.x = tileX * width, .y = tileY * width, .w = width, .h = width}));

    //SDL_RenderPresent(mainRenderer);
}

void drawTextBox(char* input)
{
    //36 letters per line/5 lines at 36pt font
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.y = 17 * SCREEN_HEIGHT / 32, .w = SCREEN_WIDTH, .h = 15 * SCREEN_HEIGHT / 32}));
    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = 69 * SCREEN_HEIGHT / 128, .w = SCREEN_WIDTH, .w = 126 * SCREEN_WIDTH / 128, .h = 58 * SCREEN_HEIGHT / 128}));
    SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Texture* txtTexture = NULL;
    int* wh;
    wh = loadTextTexture(input, &txtTexture, (SDL_Color){0, 0, 0}, true);
    //printf("%d * %d \n", *wh, *(wh + 1));
    SDL_RenderCopy(mainRenderer, txtTexture, &((SDL_Rect){.w = *wh, .h = *(wh + 1) > 27 * SCREEN_HEIGHT / 64 ? 27 * SCREEN_HEIGHT / 64 : *(wh + 1)}),
                   &((SDL_Rect){.x =  SCREEN_WIDTH / 64, .y = 37 * SCREEN_HEIGHT / 64, .w = *wh, .h = *(wh + 1) > 27 * SCREEN_HEIGHT / 64 ? 27 * SCREEN_HEIGHT / 64 : *(wh + 1)}));
    SDL_DestroyTexture(txtTexture);
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

bool checkKeyPress(sprite* playerSprite)
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (!playerSprite->movementLocked && (checkSKUp || checkSKDown || checkSKLeft || checkSKRight))
    {
        drawTile(tilemap[playerSprite->y / TILE_SIZE][playerSprite->x / TILE_SIZE], playerSprite->x / TILE_SIZE, playerSprite->y / TILE_SIZE, TILE_SIZE);
        if (playerSprite->y > 0 && checkSKUp)
            playerSprite->y-= TILE_SIZE;
        if (playerSprite->y < SCREEN_HEIGHT - playerSprite->h && checkSKDown)
            playerSprite->y+= TILE_SIZE;
        if (playerSprite->x > 0 && checkSKLeft)
            playerSprite->x-= TILE_SIZE;
        if (playerSprite->x < SCREEN_WIDTH - playerSprite->w && checkSKRight)
            playerSprite->x+= TILE_SIZE;
        int thisTile = tilemap[playerSprite->y / TILE_SIZE][playerSprite->x / TILE_SIZE];
        checkCollision(playerSprite, thisTile, checkSKRight + -1 * checkSKLeft, checkSKDown + -1 * checkSKUp);
        if (thisTile == TILE_ID_DOOR || !playerSprite->x || playerSprite->y == TILE_SIZE || playerSprite->x == SCREEN_WIDTH - TILE_SIZE || playerSprite->y == SCREEN_HEIGHT - TILE_SIZE)
        {
            drawTile(playerSprite->tileIndex, playerSprite->x / playerSprite->w, playerSprite->y / playerSprite->w, playerSprite->w);
            SDL_RenderPresent(mainRenderer);
            if (thisTile == TILE_ID_DOOR)
            {
                int map;
                if (fPart(playerSprite->mapScreen))
                {
                    playerSprite->lastScreen = playerSprite->mapScreen;
                    playerSprite->mapScreen = iPart(playerSprite->mapScreen);
                    playerSprite->overworldX = playerSprite->x;
                    playerSprite->overworldY = playerSprite->y;
                    playerSprite->x = 9 * TILE_SIZE;
                    playerSprite->y = 12 * TILE_SIZE;
                }
                else
                {
                    playerSprite->mapScreen = playerSprite->lastScreen;
                    playerSprite->x = playerSprite->overworldX;
                    playerSprite->y = playerSprite->overworldY + TILE_SIZE;
                    //map = (int) 10 * fPart(playerSprite->mapScreen) + iPart(playerSprite->mapScreen - 1) * 8;
                }
                double arrayOfMaps[] = ARRAY_OF_MAP_IDS;
                map = checkArrayForVal(playerSprite->mapScreen, arrayOfMaps, SIZE_OF_MAP_ARRAY);
                loadMapFile(MAP_DATA_NAME, tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
                drawTilemap(0, 0, WIDTH_IN_TILES, HEIGHT_IN_TILES);
            }
            else
            {
                if (!playerSprite->x)
                {
                    playerSprite->x = SCREEN_WIDTH - (2 * TILE_SIZE);
                    if (fPart((10 * fPart(playerSprite->mapScreen))) > 0)
                        playerSprite->mapScreen = playerSprite->mapScreen - .01;

                }
                if (playerSprite->y == TILE_SIZE)
                {
                    playerSprite->y = SCREEN_HEIGHT - (2 * TILE_SIZE);
                    if(iPart(10 * fPart(playerSprite->mapScreen)) > 0)
                        playerSprite->mapScreen = playerSprite->mapScreen - .1;
                }
                if (playerSprite->x == SCREEN_WIDTH - TILE_SIZE)
                {
                    playerSprite->x = (2 * TILE_SIZE);
                    if (10 * fPart( 10 * fPart(playerSprite->mapScreen)) < 9)
                        playerSprite->mapScreen = playerSprite->mapScreen + .01;

                }
                if (playerSprite->y == SCREEN_HEIGHT - TILE_SIZE)
                {
                    playerSprite->y = TILE_SIZE * 2;
                    if (iPart(10 * fPart(playerSprite->mapScreen)) < 9)
                        playerSprite->mapScreen = playerSprite->mapScreen + .1;
                }
                double arrayOfMaps[] = ARRAY_OF_MAP_IDS;
                int map = checkArrayForVal(playerSprite->mapScreen, arrayOfMaps, SIZE_OF_MAP_ARRAY);
                //(int) 10 * fPart(playerSprite->mapScreen) + iPart(playerSprite->mapScreen - 1) * 8
                loadMapFile(MAP_DATA_NAME, tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
                drawTilemap(0, 0, WIDTH_IN_TILES, HEIGHT_IN_TILES);
            }
        }
    }

    if (checkSKSpace)
    {
        textBoxOn = !textBoxOn;
        playerSprite->movementLocked = !playerSprite->movementLocked;
        if (!textBoxOn)
            drawTilemap(0, 6, WIDTH_IN_TILES, HEIGHT_IN_TILES);
    }

    if (checkSKUp || checkSKDown || checkSKLeft || checkSKRight || checkSKSpace)
            return true;
        return false;
}

bool checkCollision(sprite* player, int tileID, int moveX, int moveY)
{
    if (tileID == TILE_ID_DARKNESS || tileID == TILE_ID_TREE || tileID == TILE_ID_WINDOW || tileID == TILE_ID_LIT_WINDOW
        || tileID == TILE_ID_COUNTER_TOP || tileID == TILE_ID_HOUSE_BACK_WALL || tileID == TILE_ID_BAD_WOOD_FLOOR)
    {
        player->x = player->x - moveX * TILE_SIZE;
        player->y = player->y - moveY * TILE_SIZE;
        return true;
    }
    return false;
}

void savePlayerData(sprite* player, char* filePath)
{
    char* buffer;
    createFile(filePath);
    writeLine(filePath, toString(player->x, buffer));
    writeLine(filePath, toString(player->y, buffer));
    writeLine(filePath, toString(player->w, buffer));
    writeLine(filePath, toString(player->steps, buffer));
    writeLine(filePath, toString(player->overworldX, buffer));
    writeLine(filePath, toString(player->overworldY, buffer));
    writeLine(filePath, toString((int) player->movementLocked, buffer));
    writeLine(filePath, toString(100 * player->mapScreen, buffer));
    writeLine(filePath, toString(100 * player->lastScreen, buffer));
}

void cleanSprites(sprite* sprites[], size_t elems)
{
    //not working
    for(int i = 0; i < elems; i++)
        sprites[i] = &((sprite){.x = 0, .y = 0, .w = 0, .h = 0, .tileIndex = 0, .steps = 0, .clipRect = NULL, .mapScreen = 0, .lastScreen = 0,
                       .overworldX = 0, .overworldY = 0, .movementLocked = false});
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

char* toString(int value, char * result)
{
    if (value == 0)
        return "0";
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

int checkArrayForVal(double value, double* array, size_t arraySize)
{
	bool found = -1;
	for(int i = 0; i < arraySize; i++)
	{
	    //printf("%i  -> %f == %f: %d\n", i, value, *(array + i), value == *(array + i));
		if (absFloat(value - *(array + i)) < .01)
		{
			found = i;
			break;
		}
	}
	return found;
}

double absFloat(double val)
{
    if (val < 0)
        val *= -1;
    return val;
}

int createFile(char* filePath)
{
	FILE* filePtr;
	filePtr = fopen(filePath,"w");
	if (!filePtr)
	{
		printf("Error opening file!\n");
		return -1;
	}
	else
		return 0;
}

int writeLine(char* filePath, char* stuff)
{
	FILE* filePtr;
	filePtr = fopen(filePath,"a+");
	if (!filePtr)
	{
		printf("Error opening file!\n");
		return -1;
	}
	else
	{
		fprintf(filePtr, "%s\n", stuff);
		return 0;
	}
}

char* readLine(char* filePath, int lineNum, char** output)
{
	FILE* filePtr = fopen(filePath,"r");
	if (!filePtr)
		return 0;
	else
	{
	static char thisLine[512];
	for(int p = 0; p <= lineNum; p++)
		fgets(thisLine, 512, filePtr);
	//printf("%s @ %d\n", thisLine, thisLine);
	*output = thisLine;
	//printf("%s @ %d\n", output, output);
	fclose(filePtr);
	return thisLine;
	}
}
