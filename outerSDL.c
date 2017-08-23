#include "outerSDL.h"

#define checkSKUp currentKeyStates[SDL_SCANCODE_UP]
#define checkSKDown currentKeyStates[SDL_SCANCODE_DOWN]
#define checkSKLeft currentKeyStates[SDL_SCANCODE_LEFT]
#define checkSKRight currentKeyStates[SDL_SCANCODE_RIGHT]
#define checkSKSpace currentKeyStates[SDL_SCANCODE_SPACE]
#define checkSKEsc currentKeyStates[SDL_SCANCODE_ESCAPE]

#define TILE_ID_TREE 8
#define TILE_ID_LAVA 9
#define TILE_ID_BOULDER 10
#define TILE_ID_CURSOR 16
#define TILE_ID_PLAYER 17
#define TILE_ID_DOOR 22
#define TILE_ID_WINDOW 23
#define TILE_ID_LIT_WINDOW 24
#define TILE_ID_COUNTER_TOP 27
#define TILE_ID_HOUSE_BACK_WALL 28
#define TILE_ID_BAD_WOOD_FLOOR 29
#define TILE_ID_DARKNESS 127
#define ARRAY_OF_MAP_IDS {playerSprite->worldNum, 1.1, 1.2, 1.21, 1.22, 1.31, 1.32, 1.33, 2.1, 2.11, 2.12, 2.21, 2.22, 2.23, 2.33, 3.1, 3.2, 3.22, 3.3, 3.31, 3.32, 4.1, 4.11, 4.12, 4.21, 4.22, 4.31, 4.32, 5.1, 5.11, 5.12, 5.2, 5.21, 5.22, 5.32, 6.1, 6.11, 6.2, 6.21, 6.22, 6.31, 6.32, 7.1, 7.11, 7.12, 7.13, 7.2, 7.21, 7.22, 7.23, 7.24, 8.1, 8.11, 8.2, 8.21, 8.22, 8.32, 8.33}
#define SIZE_OF_MAP_ARRAY 58
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
                loadTTFont(FONT_FILE_NAME, &mainFont, 48);
                loadTTFont(FONT_FILE_NAME, &smallFont, 20);
                if (!mainFont || !smallFont)
                {
                    printf("%s could not be created! SDL Error: %s\n", !mainFont ? "mainFont" : "smallFont", TTF_GetError());
                    return -3;
                }
                else
                {
                    loadIMG(TILESET_FILE_NAME, &tilesetTexture);
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

bool startGame(player* playerSprite, bool newSave)
{
    loadPlayerData(playerSprite, SAVE_FILE_NAME, newSave ? 2 - aMenu("Are you sure you       want to start a     new save?", "YES", "NO", " ", " ", " ", 2, 2, MAIN_MENU_PALETTE, false, false) : newSave);
    double arrayOfMaps[] = ARRAY_OF_MAP_IDS;
    //printf("%f\n", player->worldNum + (double)(player->mapScreen / 10.0));
    int map = checkArrayForVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 10.0), arrayOfMaps, SIZE_OF_MAP_ARRAY);
    loadMapFile(MAP_FILE_NAME, tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
    if (tilemap[0][0] == -1 || map == -1)
    {
        printf("Tilemap could not load! Error code 5. Extra info: %d", map);
        return 5;
    }
    return 0;
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
        txtSurface = TTF_RenderText(smallFont, text, color, ((SDL_Color) {181, 182, 173}));
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

void initSprite(sprite* spr, int x, int y, int size, int tileIndex, entityType type)
{
    spr->x = x;
	spr->y = y;
	spr->w = size;
	spr->h = size;
	spr->tileIndex = tileIndex;
	spr->clipRect = &((SDL_Rect){.x = (tileIndex / 8) * size, .y = (tileIndex % 8) * size, .w = size, .h = size});
	spr->type = type;
}

void initPlayer(player* player, int x, int y, int size, int tileIndex)
{
    inputName(player);  //custom text input routine to get player->name
    initSprite(&(player->spr), x, y, size, tileIndex, (entityType) type_player);
	player->level = 1;
	player->experience = 0;
	player->HP = 50;
	player->maxHP = 50;
	player->attack = 1;
	player->speed = 1;
	player->statPts = 0;
	player->move1 = 40;
	player->move2 = 0;
	player->move3 = 0;
	player->move4 = 0;
	player->steps = 0;
	player->worldNum = 1;
	player->mapScreen = 1.0;
	player->lastScreen = 1.0;
	player->overworldX = x;
	player->overworldY = y;
	player->flip = SDL_FLIP_NONE;
	player->movementLocked = false;
    //name, x, y, w, level, HP, maxHP, attack, speed, statPts, move1 - move4, steps, worldNum, mapScreen, lastScreen, overworldX, overworldY
}

void loadPlayerData(player* player, char* filePath, bool forceNew)
{
    if (!checkFile(filePath) || forceNew)
	{
	    initPlayer(player, 4 * TILE_SIZE, 6 * TILE_SIZE, TILE_SIZE, TILE_ID_PLAYER);
	}
	else
	{
	    char* buffer;
        strcpy(player->name, readLine(filePath, 0, &buffer));
        strcpy(player->name, removeNewline(player->name, PLAYER_NAME_LIMIT + 1));
        player->spr.x = strtol(readLine(filePath, 1, &buffer), NULL, 10);
        player->spr.y = strtol(readLine(filePath, 2, &buffer), NULL, 10);
        player->spr.w = TILE_SIZE;
        player->spr.h = player->spr.w;
        player->spr.tileIndex = TILE_ID_PLAYER;
        player->level = strtol(readLine(filePath, 3, &buffer), NULL, 10);
        player->experience = strtol(readLine(filePath, 4, &buffer), NULL, 10);
        player->HP = strtol(readLine(filePath, 5, &buffer), NULL, 10);
        player->maxHP = strtol(readLine(filePath, 6, &buffer), NULL, 10);
        player->attack = strtol(readLine(filePath, 7, &buffer), NULL, 10);
        player->speed = strtol(readLine(filePath, 8, &buffer), NULL, 10);
        player->statPts = strtol(readLine(filePath, 9, &buffer), NULL, 10);
        player->move1 = strtol(readLine(filePath, 10, &buffer), NULL, 10);
        player->move2 = strtol(readLine(filePath, 11, &buffer), NULL, 10);
        player->move3 = strtol(readLine(filePath, 12, &buffer), NULL, 10);
        player->move4 = strtol(readLine(filePath, 13, &buffer), NULL, 10);
        player->steps = 0;
        player->spr.clipRect = &((SDL_Rect){.x = (player->spr.tileIndex / 8) * player->spr.w, .y = (player->spr.tileIndex % 8) * player->spr.w, .w = player->spr.w, .h = player->spr.w});
        player->worldNum = strtol(readLine(filePath, 14, &buffer), NULL, 10);
        player->mapScreen = (double) strtol(readLine(filePath, 15, &buffer), NULL, 10) / 10.0;
        player->lastScreen = (double) strtol(readLine(filePath, 16, &buffer), NULL, 10) / 10.0;
        player->overworldX = strtol(readLine(filePath, 17, &buffer), NULL, 10);
        player->overworldY = strtol(readLine(filePath, 18, &buffer), NULL, 10);
        player->flip = SDL_FLIP_NONE;
        player->movementLocked = false;
        //name, x, y, w, level, HP, maxHP, attack, speed, statPts, move1 - move4, steps, worldNum, mapScreen, lastScreen, overworldX, overworldY
	}
}

void inputName(player* player)
{
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    //background text (drawn first)
    drawText("NAME?", 21 * SCREEN_WIDTH / 128, 13 * SCREEN_HEIGHT / 128, 119 * SCREEN_HEIGHT / 128, (SDL_Color){24, 65, 214}, false);
    //foreground text
    drawText("NAME?", 10 * SCREEN_WIDTH / 64, 5 * SCREEN_WIDTH / 64, 59 * SCREEN_HEIGHT / 64, (SDL_Color){24, 162, 239}, true);

    SDL_Event e;
    bool quit = false, hasTyped = false;
    char playerName[PLAYER_NAME_LIMIT + 1] = "      \0";
    int i = 0;
    while(!quit)
    {
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if(e.type == SDL_QUIT)
                quit = true;
            //User presses a key
            else if(e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z && i < PLAYER_NAME_LIMIT)
                {
                    playerName[i++] = toupper(e.key.keysym.sym);
                    hasTyped = true;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE && i > 0)
                {
                    playerName[--i] = ' ';
                    if (i == 0)
                        hasTyped = false;
                }

                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 2 * TILE_SIZE, .y = 3.5 * TILE_SIZE, .w = 6 * TILE_SIZE, .h = TILE_SIZE}));
                drawText(playerName, 2 * TILE_SIZE, 3.5 * TILE_SIZE, (HEIGHT_IN_TILES - 2) * TILE_SIZE, (SDL_Color){255, 255, 255}, true);

                if (e.key.keysym.sym == SDLK_RETURN)
                    quit = true;

            }
        }
    }
    if (!hasTyped)
        strcpy(playerName, "STEVO\0");
    strcpy(player->name, playerName);
}

void loadMapFile(char* filePath, int* array[], const int lineNum, const int y, const int x)
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

void drawTilemap(int startX, int startY, int endX, int endY)
{
    for(int dy = startY; dy < endY; dy++)
    {
        for(int dx = startX; dx < endX; dx++)
            drawTile(tilemap[dy][dx], dx, dy, TILE_SIZE, SDL_FLIP_NONE);
        SDL_Delay(20);
    }
    SDL_RenderPresent(mainRenderer);
}

void drawTile(int id, int tileX, int tileY, int width, SDL_RendererFlip flip)
{
    //printf("%d , %d\n", id  / 8, (id % 8));
    SDL_RenderCopyEx(mainRenderer, tilesetTexture, &((SDL_Rect){.x = (id / 8) * width, .y = (id % 8) * width, .w = width, .h = width}),
                   &((SDL_Rect){.x = tileX * width, .y = tileY * width, .w = width, .h = width}), 0 , NULL, flip);

    //SDL_RenderPresent(mainRenderer);
}

void drawText(char* input, int x, int y, int maxH, SDL_Color color, bool render)
{
    SDL_Texture* txtTexture = NULL;
    int* wh;
    wh = loadTextTexture(input, &txtTexture, color, true);
    SDL_RenderCopy(mainRenderer, txtTexture, &((SDL_Rect){.w = *wh, .h = *(wh + 1) > maxH ? maxH : *(wh + 1)}),
                                             &((SDL_Rect){.x =  x, .y = y, .w = *wh, .h = *(wh + 1) > maxH ? maxH : *(wh + 1)}));
    if (render)
        SDL_RenderPresent(mainRenderer);
    SDL_DestroyTexture(txtTexture);
}

bool checkKeyPress(player* playerSprite)
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (!playerSprite->movementLocked && (checkSKUp || checkSKDown || checkSKLeft || checkSKRight))
    {
        drawTile(tilemap[playerSprite->spr.y / TILE_SIZE][playerSprite->spr.x / TILE_SIZE], playerSprite->spr.x / TILE_SIZE, playerSprite->spr.y / TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
        if (playerSprite->spr.y > 0 && checkSKUp)
            playerSprite->spr.y -= TILE_SIZE;
        if (playerSprite->spr.y < SCREEN_HEIGHT - playerSprite->spr.h && checkSKDown)
            playerSprite->spr.y += TILE_SIZE;
        if (playerSprite->spr.x > 0 && checkSKLeft)
            playerSprite->spr.x -= TILE_SIZE;
        if (playerSprite->spr.x < SCREEN_WIDTH - playerSprite->spr.w && checkSKRight)
            playerSprite->spr.x += TILE_SIZE;
        if (checkSKLeft)
            playerSprite->flip = SDL_FLIP_HORIZONTAL;
        if (checkSKRight)
            playerSprite->flip = SDL_FLIP_NONE;
        int thisTile = tilemap[playerSprite->spr.y / TILE_SIZE][playerSprite->spr.x / TILE_SIZE];
        checkCollision(playerSprite, thisTile, checkSKRight + -1 * checkSKLeft, checkSKDown + -1 * checkSKUp);
        if (thisTile == TILE_ID_DOOR || !playerSprite->spr.x || playerSprite->spr.y == TILE_SIZE || playerSprite->spr.x == SCREEN_WIDTH - TILE_SIZE || playerSprite->spr.y == SCREEN_HEIGHT - TILE_SIZE)
        {
            drawTile(playerSprite->spr.tileIndex, playerSprite->spr.x / playerSprite->spr.w, playerSprite->spr.y / playerSprite->spr.w, playerSprite->spr.w, playerSprite->flip);
            SDL_RenderPresent(mainRenderer);
            if (thisTile == TILE_ID_DOOR)
            {
                if (playerSprite->mapScreen)
                {
                    playerSprite->lastScreen = playerSprite->mapScreen;
                    playerSprite->mapScreen = 0;
                    playerSprite->overworldX = playerSprite->spr.x;
                    playerSprite->overworldY = playerSprite->spr.y;
                    playerSprite->spr.x = 9 * TILE_SIZE;
                    playerSprite->spr.y = 12 * TILE_SIZE;
                }
                else
                {
                    playerSprite->mapScreen = playerSprite->lastScreen;
                    playerSprite->spr.x = playerSprite->overworldX;
                    playerSprite->spr.y = playerSprite->overworldY + TILE_SIZE;
                    //map = (int) 10 * fPart(playerSprite->mapScreen) + iPart(playerSprite->mapScreen - 1) * 8;
                }
            }
            else
            {
                if (!playerSprite->spr.x)
                {
                    playerSprite->spr.x = SCREEN_WIDTH - (2 * TILE_SIZE);
                    if (fPart(playerSprite->mapScreen) > 0)
                        playerSprite->mapScreen = playerSprite->mapScreen - .1;

                }
                if (playerSprite->spr.y == TILE_SIZE)
                {
                    playerSprite->spr.y = SCREEN_HEIGHT - (2 * TILE_SIZE);
                    if (iPart(playerSprite->mapScreen) > 0)
                        playerSprite->mapScreen = playerSprite->mapScreen - 1;
                }
                if (playerSprite->spr.x == SCREEN_WIDTH - TILE_SIZE)
                {
                    playerSprite->spr.x = TILE_SIZE;
                    if (10 * fPart(playerSprite->mapScreen) < 9)
                        playerSprite->mapScreen = playerSprite->mapScreen + .1;

                }
                if (playerSprite->spr.y == SCREEN_HEIGHT - TILE_SIZE)
                {
                    playerSprite->spr.y = TILE_SIZE * 2;
                    if (iPart(playerSprite->mapScreen) < 9)
                        playerSprite->mapScreen = playerSprite->mapScreen + 1;
                }
            }
            double arrayOfMaps[] = ARRAY_OF_MAP_IDS;
            int map = checkArrayForVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 10.0), arrayOfMaps, SIZE_OF_MAP_ARRAY);
            loadMapFile(MAP_FILE_NAME, tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
            return KEYPRESS_RETURN_BREAK;
        }
    }

    if (checkSKSpace && fPart(playerSprite->mapScreen) == playerSprite->mapScreen && playerSprite->spr.x == 9 * TILE_SIZE && playerSprite->spr.y == 6 * TILE_SIZE)
    {
        textBoxOn = !textBoxOn;
        playerSprite->movementLocked = !playerSprite->movementLocked;
        if (!textBoxOn)
            drawTilemap(0, 8, WIDTH_IN_TILES, HEIGHT_IN_TILES);
        SDL_Delay(75);
    }

    if (checkSKEsc)
        return KEYPRESS_RETURN_MENU;

    if (checkSKUp || checkSKDown || checkSKLeft || checkSKRight || checkSKSpace || checkSKEsc)
            return true;
        return false;
}

bool checkCollision(player* player, int tileID, int moveX, int moveY)
{
    if (tileID == TILE_ID_DARKNESS || tileID == TILE_ID_TREE || tileID == TILE_ID_LAVA || tileID == TILE_ID_WINDOW || tileID == TILE_ID_LIT_WINDOW
        || tileID == TILE_ID_COUNTER_TOP || tileID == TILE_ID_HOUSE_BACK_WALL || tileID == TILE_ID_BAD_WOOD_FLOOR)
    {
        player->spr.x -= moveX * TILE_SIZE;
        player->spr.y -= moveY * TILE_SIZE;
        return true;
    }
    return false;
}

void savePlayerData(player* player, char* filePath)
{
    char* buffer = "";
    createFile(filePath);
    writeLine(filePath, player->name);
    writeLine(filePath, toString(player->spr.x, buffer));
    writeLine(filePath, toString(player->spr.y, buffer));
    writeLine(filePath, toString(player->level, buffer));
    writeLine(filePath, toString(player->experience, buffer));
    writeLine(filePath, toString(player->HP, buffer));
    writeLine(filePath, toString(player->maxHP, buffer));
    writeLine(filePath, toString(player->attack, buffer));
    writeLine(filePath, toString(player->speed, buffer));
    writeLine(filePath, toString(player->statPts, buffer));
    writeLine(filePath, toString(player->move1, buffer));
    writeLine(filePath, toString(player->move2, buffer));
    writeLine(filePath, toString(player->move3, buffer));
    writeLine(filePath, toString(player->move4, buffer));
    writeLine(filePath, toString(player->worldNum, buffer));
    writeLine(filePath, toString((int) (10 * player->mapScreen), buffer));
    writeLine(filePath, toString((int) (10 * player->lastScreen), buffer));
    writeLine(filePath, toString(player->overworldX, buffer));
    writeLine(filePath, toString(player->overworldY, buffer));
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    drawText("Save completed.", 9 * SCREEN_WIDTH / 64, 30 * SCREEN_HEIGHT / 64, 34 * SCREEN_HEIGHT / 64, (SDL_Color){24, 162, 239}, false);
    SDL_RenderPresent(mainRenderer);
    SDL_Delay(450);
}

void closeSDL()
{
    TTF_CloseFont(mainFont);
    TTF_CloseFont(smallFont);
	SDL_DestroyTexture(tilesetTexture);
	SDL_FreeSurface(mainScreen);
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

void readStringInput(char* str, int limit)
{
	printf("Enter a string (Limit of %d characters): ", limit);
	//get input using a getc() loop and terminate upon a newline
	int i = 0;
	char c;
	while (i < limit)
	{
		c = getc(stdin);
		if (c == '\n')
			break;
		str[i] = c;
		i++;

	}
	if (i >= limit)
		i = limit;
	str = realloc(str, sizeof(char[i + 1]));
	str[i] = '\0';
	//this works when i < limit because apparently you can just increase the size of arrays
	//by storing a new value at [dim + 1]
}
void freeThisMem(int ** x)
{
	free(*x);
	*x = NULL;
}

char* removeNewline(char input[], size_t length)
{
    static char sansNewline[20];
    int i;
    length = strlen(input);
    for(i = 0; i < length - 1; i++)
    {
        if (input[i] == '\n')
            break;
        sansNewline[i] = input[i];
        //printf("%c\n", sansNewline[i]);
    }
    sansNewline[i] = '\0';
    //printf("%s at %d\n", sansNewline, sansNewline);
    return sansNewline;
}

int checkArrayForVal(double value, double* array, size_t arraySize)
{
	bool found = -1;
	for(int i = 0; i < arraySize; i++)
	{
	    //printf("%i  -> %f == %f: %d\n", i, value, *(array + i), value == *(array + i));
		if (fabs(value - *(array + i)) < .001)
		{
			found = i;
			break;
		}
	}
	return found;
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

bool checkFile(char* filePath)
{
    FILE* filePtr = fopen(filePath,"r");
	if (!filePtr)
		return false;
    return true;
}

int writeLine(char* filePath, char* stuff)
{
	FILE* filePtr;
	filePtr = fopen(filePath,"a");
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
	fseek(filePtr, 0, SEEK_SET);
	for(int p = 0; p <= lineNum; p++)
		fgets(thisLine, 512, filePtr);
	//printf("%s @ %d\n", thisLine, thisLine);
	*output = thisLine;
	//printf("%s @ %d\n", output, output);
	fclose(filePtr);
	return thisLine;
	}
}
