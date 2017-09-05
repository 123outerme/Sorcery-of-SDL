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
#define TILE_ID_CHEST 19
#define TILE_ID_CUBED 20
#define TILE_ID_TILDA 21
#define TILE_ID_SUPEREFFECTIVE 37
#define TILE_ID_RUN 38
#define TILE_ID_CRITICAL 39
#define TILE_ID_BLOCK 55
#define TILE_ID_APEMAN 74
#define TILE_ID_FEENIX 77
#define TILE_ID_TARANT 80
#define TILE_ID_POLARA 83
#define TILE_ID_HYDROA 86
#define TILE_ID_SWURM 89
#define TILE_ID_SENTRY 92
#define TILE_ID_DREGOH 95
#define TILE_ID_COIN 100
#define TILE_ID_INNKEEPER 104
#define TILE_ID_NPC 105
#define TILE_ID_UPGRADER 106
#define TILE_ID_MINER 107
#define TILE_ID_PARKA 108
#define TILE_ID_EARL 109
#define TILE_ID_GENERAL 111

#define ANYWHERE_QUIT -1

#define MENU_NEW_SAVE 1
#define MENU_LOAD_SAVE 2
#define MENU_HELP 3
#define MENU_QUIT 4

#define LOOP_QUIT 0
#define LOOP_GOTO_MENU 1
#define LOOP_GOTO_BATTLE 2
#define LOOP_RETURN 3
#define LOOP_GOTO_BATTLE_BOSS 4

#define OVERWORLD_RETURN 1
#define OVERWORLD_STATS 2
#define OVERWORLD_ITEMS 3
#define OVERWORLD_SAVE_RETURN 4
#define OVERWORLD_QUIT 5

#define OVERWORLD_MENU_PALETTE (SDL_Color){181, 182, 173}, (SDL_Color){181, 182, 173}, (SDL_Color){16, 32, 140}, (SDL_Color){16, 32, 140}

#define ALL_ATTACKS "SLICESLASHBURN ROASTCRACKBREAKCHILLICE  FLOW SWEEPWHACKBASH STAB GASH SMASH     THORNVINE FLAREBLAZEROCK STONEFROSTHAIL STORMVOLT SMELLSTINKDARK EVIL ALPHAARROW"
#define ALL_ENEMIES_ARRAY {"Ant", "Rat", "APEMAN", "Firant", "Pyre", "FEENIX", "Gemdog", "Golem", "TARANT", "Pengin", "Snoman", "POLARA", "C Gull", "C Star", "HYDROA", "Rodent", "Crow", "SWURM", "Midas", "Greed", "SENTRY", "Archer", "Knight", "- DREGOH -"}

#define ARRAY_OF_MAP_IDS {playerSprite->worldNum, 1.1, 1.2, 1.21, 1.22, 1.31, 1.32, 1.33, 2.1, 2.11, 2.12, 2.21, 2.22, 2.23, 2.33, 3.1, 3.2, 3.22, 3.3, 3.31, 3.32, 4.1, 4.11, 4.12, 4.21, 4.22, 4.31, 4.32, 5.1, 5.11, 5.12, 5.2, 5.21, 5.22, 5.32, 6.1, 6.11, 6.2, 6.21, 6.22, 6.31, 6.32, 7.1, 7.11, 7.12, 7.13, 7.2, 7.21, 7.22, 7.23, 7.24, 8.1, 8.11, 8.2, 8.21, 8.22, 8.32, 8.33}
#define SIZE_OF_MAP_ARRAY 58

#define ARRAY_OF_CHEST_IDS {1.2, 1.21, 2.11, 2.23, 3.2, 3.32, 4.11, 4.12, 5.1, 5.2, 5.12, 6.2, 6.22, 6.32, 7.1, 7.12, 7.2, 7.21, 7.24, 8.11, 8.2, 8.32}
#define SIZE_OF_CHEST_ARRAY 22

#define ARRAY_OF_BOSS_IDS {1.33, 2.33, 3.22, 4.31, 5.32, 6.31, 7.13, 7.23, 8.33}
#define SIZE_OF_BOSS_ARRAY 9

#define BATTLE_ACT_CODE_W 1
#define BATTLE_ACT_CODE_A 2
#define BATTLE_ACT_CODE_S 3
#define BATTLE_ACT_CODE_D 4
#define BATTLE_ACT_CODE_LSHIFT 5
#define ATTACK_CODE_RUN TILE_ID_RUN
#define ATTACK_CODE_BLOCK TILE_ID_BLOCK

//Todo for 9/5:
//* Make world-sized tilemaps work (and smooth scrolling)?
//** Create world-sized tilemaps by stitching together the individual CSE map pngs, throw them in the xLIBC map generator, done
//** Make them work by doing nice scroll animations between map borders
//** Make sure you only render screen-sized chunks at a time
//* Add item chests and bosses
//* Add items

int main(int argc, char* args[])
{
	player player;
	int succeeded = init();
	if (!succeeded)
    {

        int selection = 0;
        do
        {
            selection = aMenu("Sorcery of Uvutu", "NEW GAME", "LOAD GAME", "HELP", "QUIT", " ", 4, selection, MAIN_MENU_PALETTE, false, true);
            if (selection == MENU_HELP)
                selection = aWallOfText("Sorcery of Uvutu", "^<V> to move. Use ESC to enter menu. Spacebar to talk to people. Spacebar in the Menu to select an option. Critical Hits are marked by a RED X, and Weakness by a YELLOW +. See the README.", true);
        }
        while (selection == MENU_HELP);
        if (selection < MENU_HELP && selection != ANYWHERE_QUIT)
        {
            succeeded = startGame(&player, selection == MENU_NEW_SAVE);
            int loopCode = LOOP_GOTO_MENU;
            while (loopCode)
            {
                loopCode = mainLoop(&player);
                if (loopCode == LOOP_GOTO_BATTLE || loopCode == LOOP_GOTO_BATTLE_BOSS)
                    doBattle(&player, loopCode == LOOP_GOTO_BATTLE_BOSS);
                if (loopCode == LOOP_GOTO_MENU)
                {
                    loopCode = -2;
                    while (loopCode == OVERWORLD_STATS || loopCode == OVERWORLD_ITEMS || loopCode == -2)
                    {
                        loopCode = aMenu(player.name, "BACK", "STATS", "ITEMS", "SAVE + CONTINUE", "SAVE + QUIT", 5, loopCode, OVERWORLD_MENU_PALETTE, true, false);
                        if (loopCode == OVERWORLD_SAVE_RETURN)
                            savePlayerData(&player, SAVE_FILE_NAME);
                        if (loopCode == OVERWORLD_STATS)
                            loopCode = showStats(&player);
                        if (loopCode == OVERWORLD_ITEMS)
                            loopCode = showItems(&player);
                        if (loopCode == OVERWORLD_QUIT || loopCode == ANYWHERE_QUIT)
                            loopCode = LOOP_QUIT;
                    }
                }

            }
            printf("%s ended at %d, %d underneath a tile of index %d in map id %f\n", player.name, player.spr.x / player.spr.w, player.spr.y / player.spr.w, tilemap[player.spr.y / TILE_SIZE][player.spr.x / TILE_SIZE], player.worldNum + (player.mapScreen / 10));
            savePlayerData(&player, SAVE_FILE_NAME);
        }
        closeSDL();
	}
	return succeeded;
}

void drawGame(player* player, char* textInput)
{
    drawTile(player->spr.tileIndex, player->spr.x, player->spr.y, player->spr.w, player->flip);
    if (textBoxOn)
        drawTextBox(textInput, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 8.5 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 8.5) * TILE_SIZE});
    SDL_RenderPresent(mainRenderer);
}

int mainLoop(player* playerSprite)
{
    bool quit = false, press = false;
    SDL_Event e;
    int frame = 0, exitCode = LOOP_QUIT;
    char* textInput = "Did you know that this is placeholder text? Me neither.";
    {
        double arrayOfMaps[] = ARRAY_OF_MAP_IDS;
        int map = checkArrayForVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 10.0), arrayOfMaps, SIZE_OF_MAP_ARRAY);
        loadMapFile(MAP_FILE_NAME, tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
        //use the int "map" when testing if user is in a map with a boss/chest?
    }
    drawTilemap(0, 0, WIDTH_IN_TILES, HEIGHT_IN_TILES);
    drawHUD(playerSprite);
    sprite entity;
    entityType type = type_na;
    int x = -TILE_SIZE, y = -TILE_SIZE, index = 127;
    if (!playerSprite->mapScreen)
    {
        type = type_npc;
        x *= -9;
        y *= -4;
        index = TILE_ID_NPC;
        if (playerSprite->worldNum == 3)
            index = TILE_ID_MINER;
        if (playerSprite->worldNum == 4)
            index = TILE_ID_PARKA;
        if (playerSprite->worldNum == 7)
            index = TILE_ID_EARL;
        if (playerSprite->overworldX == 192 && playerSprite->overworldY == 240)
        {
            index = TILE_ID_INNKEEPER;
            textInput = "ARE YOU TIRED? REST HERE.       (HEALED)";
        }
    }
    else
    {
        int found;
        {
            double arrayOfMaps[SIZE_OF_CHEST_ARRAY] = ARRAY_OF_CHEST_IDS;
            found = checkArrayForVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 10.0), arrayOfMaps, SIZE_OF_CHEST_ARRAY);
        }
        if (found != -1)
        {
            /*If M=1.2
72.056
If M=1.21
136.088
If M=2.11
128.088
If M=2.23
64.088
If M=3.2
72.088
If M=3.32
24.088
If M=4.11
104.016
If M=4.12
24.04
If M=5.2
80.064
If M=5.1
112.016
If M=5.12
48.072
If M=6.2
16.04
If M=6.32
8.096
If M=7.1
40.032
If M=7.12
48.04
If M=7.2
32.08
If M=7.21
128.04
If M=7.24
40.056
If M=8.11
144.056
If M=8.2
24.08
If M=8.32
24.08
            */
            type = type_chest;
            index = TILE_ID_CHEST;
            if (found == 0)
            {
                x *= -9;
                y *= -7;
            }
            if (found == 1)
            {
                x *= -17;
                y *= -11;
            }
        }
        else
        {
            type = type_boss;
            {
                double arrayOfMaps[SIZE_OF_BOSS_ARRAY] = ARRAY_OF_BOSS_IDS;
                found = checkArrayForVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 10.0), arrayOfMaps, SIZE_OF_BOSS_ARRAY);
            }
            if (found != -1 && playerSprite->beatenBosses < playerSprite->worldNum)
            {
                if (found == 0)
                {
                    index = TILE_ID_APEMAN;
                    x *= -14;
                    y *= -7;
                }
                if (found == 1)
                {
                    index = TILE_ID_FEENIX;
                    x *= -11;
                    y *= -7;
                }
                if (found == 2)
                {
                    index = TILE_ID_TARANT;
                    x *= -7;
                    y *= -8;
                }
                if (found == 3)
                {
                    index = TILE_ID_POLARA;
                    x *= -10;
                    y *= -6;
                }
                if (found == 4)
                {
                    index = TILE_ID_HYDROA;
                    x *= -5;
                    y *= -5;
                }
                if (found == 5)
                {
                    index = TILE_ID_SWURM;
                    x *= -12;
                    y *= -7;
                }
                if (found == 6)
                {
                    index = TILE_ID_SENTRY;
                    x *= -14;
                    y *= -7;
                }
                if (found == 7)
                {
                    index = TILE_ID_DREGOH;
                    x *= -7;
                    y *= -3;
                }
                if (found == 8)
                {
                    index = TILE_ID_DREGOH;
                    x *= -11;
                    y *= -7;
                }
            }
        }
    }
    initSprite(&entity, x, y, TILE_SIZE, index, type);
    drawTile(entity.tileIndex, entity.x, entity.y, TILE_SIZE, SDL_FLIP_NONE);
    while(!quit)    //this is the literal main loop right here
    {
        drawGame(playerSprite, textInput);
        while(SDL_PollEvent(&e) != 0)  //while there are events in the queue
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        if (frame == 0)
            press = checkKeyPress(playerSprite);

        if (press == KEYPRESS_RETURN_BREAK)
        {
            exitCode = LOOP_RETURN;
            quit = true;
        }
        if (press == KEYPRESS_RETURN_MENU)
        {
            exitCode = LOOP_GOTO_MENU;
            quit = true;
        }
        if (press == KEYPRESS_RETURN_BATTLE)
        {
            exitCode = LOOP_GOTO_BATTLE;
            quit = true;
        }
        if (playerSprite->spr.x == entity.x && playerSprite->spr.y == entity.y)
        {
            if (entity.type == type_boss)
            {
                exitCode = LOOP_GOTO_BATTLE_BOSS;
                quit = true;
            }
            if (entity.type == type_chest)
            {
                //pick up chest here
            }
        }

        frame += press;

        if (frame % 6 == 0)
        {
            frame = 0;
            press = false;
        }

        SDL_Delay(1000 / FRAMERATE);
    }
    return exitCode;
}

void drawHUD(player* player)
{
    drawTilemap(0, 0, 19, 1);
    char* buffer = "";
    char* worldNames[8] = {"Plain Plains", "Dragon's Den", "Worry Quarry", "West Pole", "River Lake", "Under City", "Upper City", "Battleground"};
    drawText(worldNames[player->worldNum - 1], 0, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    buffer = toString(player->HP, buffer);
    drawText(buffer, (16 - strlen(buffer)) * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((10 * player->HP / player->maxHP) > 3 ? ((SDL_Color) {231, 223, 49}) : ((SDL_Color) {255, 0, 0})), false);
    drawText("/", 16 * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    drawText(toString(player->maxHP, buffer), 17 * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {231, 223, 49}), true);
}

void drawTextBox(char* input, player* player, SDL_Color outlineColor, SDL_Rect textBoxRect)
{
    Uint8 oldR, oldG, oldB, oldA;
    SDL_GetRenderDrawColor(mainRenderer, &oldR, &oldG, &oldB, &oldA);
    if (!player->mapScreen && player->overworldX == 192 && player->overworldY == 240)
        player->HP = player->maxHP;
    //19 letters per line/5 lines at 48pt font
    SDL_SetRenderDrawColor(mainRenderer, outlineColor.r, outlineColor.g, outlineColor.b, 0xFF);
    SDL_RenderFillRect(mainRenderer, &(textBoxRect));
    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = textBoxRect.x + TILE_SIZE / 8, .y = textBoxRect.y + TILE_SIZE / 8,
                                                  .w = textBoxRect.w -  2 * TILE_SIZE / 8, .h = textBoxRect.h - 2 * TILE_SIZE / 8}));
    drawText(input, textBoxRect.x + 2 * TILE_SIZE / 8, textBoxRect.y + 2 * TILE_SIZE / 8, textBoxRect.w -  3 * TILE_SIZE / 8, textBoxRect.h -  3 * TILE_SIZE / 8, (SDL_Color){0, 0, 0}, true);
    SDL_SetRenderDrawColor(mainRenderer, oldR, oldG, oldB, oldA);
}

int aMenu(char* title, char* opt1, char* opt2, char* opt3, char* opt4, char* opt5, const int options, int curSelect, SDL_Color bgColor, SDL_Color titleColorUnder, SDL_Color titleColorOver, SDL_Color textColor, bool border, bool showVersion)
{
    if (curSelect < 1)
        curSelect = 1;
    sprite cursor;
    initSprite(&cursor, TILE_SIZE, (curSelect + 4) * TILE_SIZE, TILE_SIZE, TILE_ID_CURSOR, (entityType) type_na);
    if (border)
        SDL_SetRenderDrawColor(mainRenderer, textColor.r, textColor.g, textColor.b, 0xFF);
    else
        SDL_SetRenderDrawColor(mainRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    SDL_SetRenderDrawColor(mainRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = SCREEN_HEIGHT / 128, .w = 126 * SCREEN_WIDTH / 128, .h = 126 * SCREEN_HEIGHT / 128}));
    //background text (drawn first)
    drawText(title, 2 * TILE_SIZE + 3 * TILE_SIZE / 8, 11 * SCREEN_HEIGHT / 128, SCREEN_WIDTH, 119 * SCREEN_HEIGHT / 128, titleColorUnder, false);
    //foreground text
    drawText(title, 2 * TILE_SIZE + TILE_SIZE / 4, 5 * SCREEN_HEIGHT / 64, SCREEN_WIDTH, 55 * SCREEN_HEIGHT / 64, titleColorOver, false);

    drawText(opt1, 2 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 5) * TILE_SIZE, textColor, false);
    drawText(opt2, 2 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 6) * TILE_SIZE, textColor, false);
    drawText(opt3, 2 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 7) * TILE_SIZE, textColor, false);
    drawText(opt4, 2 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 8) * TILE_SIZE, textColor, false);
    drawText(opt5, 2 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 9) * TILE_SIZE, textColor, !showVersion);

    if (showVersion)
    {
        char version[12];
        strcpy(version, FULLVERSION_STRING);
        strcat(version, STATUS_SHORT);
        strcat(version, "\0");
        drawTile(TILE_ID_TILDA, 0, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawTile(TILE_ID_CUBED, 1 * TILE_SIZE, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawTile(TILE_ID_TILDA, 2 * TILE_SIZE, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawText(version, 2 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){24, 195, 247}, true);
    }
    SDL_Event e;
    bool quit = false;
    int selection = -1;
    //While application is running
    while(!quit)
    {
        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = cursor.x, .y = cursor.y, .w = cursor.w, .h = cursor.w}));
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if(e.type == SDL_QUIT)
            {
                quit = true;
                selection = ANYWHERE_QUIT;
            }
            //User presses a key
            else if(e.type == SDL_KEYDOWN)
            {
                //Select surfaces based on key press
                switch(e.key.keysym.sym)
                {
                    case SDLK_w:
                        if (cursor.y > 5 * TILE_SIZE)
                            cursor.y -= TILE_SIZE;
                    break;

                    case SDLK_s:
                        if (cursor.y < (options + 4) * TILE_SIZE)
                            cursor.y += TILE_SIZE;
                    break;

                    case SDLK_SPACE:
                        selection = cursor.y / TILE_SIZE - 4;
                        quit = true;
                    break;
                    default:
                    break;
                }
            }
        }
        drawTile(cursor.tileIndex, cursor.x, cursor.y, TILE_SIZE, SDL_FLIP_NONE);
        SDL_RenderPresent(mainRenderer);
    }
    return selection;
}

int aWallOfText(char* title, char* text, bool showHelpInfo)
{
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    //background text (drawn first)
    drawText(title, 21 * SCREEN_WIDTH / 128, 1 * SCREEN_HEIGHT / 128, SCREEN_WIDTH, 127 * SCREEN_HEIGHT / 128, (SDL_Color){24, 65, 214}, false);
    //foreground text
    drawText(title, 10 * SCREEN_WIDTH / 64, 0, 54 * SCREEN_WIDTH / 64, SCREEN_HEIGHT, (SDL_Color){24, 162, 239}, false);

    drawText(text, 0, TILE_SIZE + TILE_SIZE / 2, SCREEN_WIDTH, (HEIGHT_IN_TILES - 2) * TILE_SIZE, (SDL_Color){24, 195, 247}, !showHelpInfo);
    if (showHelpInfo)
        drawText("By Stephen Policelli", 0, 13 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){24, 195, 247}, true);

    SDL_Event e;
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
                quit = true;
            else
                if(e.type == SDL_KEYDOWN)
                    quit = true;
        }
    }
    return MENU_HELP;
}

int showStats(player* player)
{
    int exitCode = OVERWORLD_STATS;
    char* buffer = "";
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = 1 * SCREEN_HEIGHT / 128, .w = 126 * SCREEN_WIDTH / 128, .h = 126 * SCREEN_HEIGHT / 128}));
    //foreground text
    drawText("STATS", 2 * TILE_SIZE + TILE_SIZE / 4, 4 * SCREEN_HEIGHT / 64, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, 54 * SCREEN_HEIGHT / 64, (SDL_Color){16, 32, 140}, false);

    drawText("BACK", 2 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 3) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(player->name, 4 * TILE_SIZE + TILE_SIZE / 4, 4 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 4) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawTile(TILE_ID_CURSOR, 1 * TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
    drawTile(TILE_ID_PLAYER, 2 * TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);

    drawText("LVL:    XP:", 4 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 5) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->level, buffer), 8 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, (WIDTH_IN_TILES - 8) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 5) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(toString(player->experience, buffer), 15 * TILE_SIZE + TILE_SIZE / 8, 5 * TILE_SIZE, (WIDTH_IN_TILES - 15) * TILE_SIZE - TILE_SIZE / 8, (HEIGHT_IN_TILES - 5) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    drawText("HP:    /   ", 4 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 6) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->HP, buffer), (10 - digits(player->HP)) * TILE_SIZE + 7 * TILE_SIZE / 8, 6 * TILE_SIZE, 3 * TILE_SIZE, (HEIGHT_IN_TILES - 6) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(toString(player->maxHP, buffer), 12 * TILE_SIZE + 2 * TILE_SIZE / 4, 6 * TILE_SIZE, 3 * TILE_SIZE, (HEIGHT_IN_TILES - 6) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    drawText("ATK:    SPD:", 4 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 7) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->attack, buffer), 8 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, 2 * TILE_SIZE, (HEIGHT_IN_TILES - 7) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(toString(player->speed, buffer), 16 * TILE_SIZE + 5 * TILE_SIZE / 16, 7 * TILE_SIZE, 2 * TILE_SIZE, (HEIGHT_IN_TILES - 7) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    drawText("STAT PTS=", 4 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 8) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->statPts, buffer), 13 * TILE_SIZE + TILE_SIZE / 8, 8 * TILE_SIZE, 2 * TILE_SIZE, (HEIGHT_IN_TILES - 8) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    char* allAttacks = ALL_ATTACKS, thisAttack[6] = "     \0";
    if (player->move1)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move1 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 9 * TILE_SIZE, (WIDTH_IN_TILES - 6) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 9) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move1, 12 * TILE_SIZE, 9 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
        }
    if (player->move2)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move2 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 10 * TILE_SIZE, (WIDTH_IN_TILES - 6) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 10) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move2, 12 * TILE_SIZE, 10 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
        }
    if (player->move3)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move3 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 11 * TILE_SIZE, (WIDTH_IN_TILES - 6) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move3, 12 * TILE_SIZE, 11 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
        }
    if (player->move4)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move4 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 12 * TILE_SIZE, (WIDTH_IN_TILES - 6) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move4, 12 * TILE_SIZE, 12 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
        }
    drawText("W:", 4 * TILE_SIZE +  TILE_SIZE / 4, 9 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 9) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText("A:", 4 * TILE_SIZE +  TILE_SIZE / 4, 10 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 10) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText("S:", 4 * TILE_SIZE +  TILE_SIZE / 4, 11 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText("D:", 4 * TILE_SIZE +  TILE_SIZE / 4, 12 * TILE_SIZE, (WIDTH_IN_TILES - 4) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){0, 0, 0}, true);

    SDL_Event e;
    bool quit = false;

    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                exitCode = ANYWHERE_QUIT;
                quit = true;
            }
            //User presses a key
            else if(e.type == SDL_KEYDOWN)
            {
                //Select surfaces based on key press
                switch(e.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        quit = true;
                    break;
                    default:
                    break;
                }
            }
        }
    }
    return exitCode;
}

int showItems(player* player)
{
    char* buffer = "";
    sprite cursor;
    initSprite(&cursor, TILE_SIZE, 1 * TILE_SIZE, TILE_SIZE, TILE_ID_CURSOR, (entityType) type_na);
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = 1 * SCREEN_HEIGHT / 128, .w = 126 * SCREEN_WIDTH / 128, .h = 126 * SCREEN_HEIGHT / 128}));

    drawText("BACK", 2 * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 1) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(" ", 2 * TILE_SIZE + TILE_SIZE / 4, 2 * TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 2) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(" ", 2 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 3) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(" ", 2 * TILE_SIZE + TILE_SIZE / 4, 4 * TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 4) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(" ", 2 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4,  (HEIGHT_IN_TILES - 5) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawTile(TILE_ID_COIN, 2 * TILE_SIZE, 13 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
    drawText("x", 3 * TILE_SIZE, 13 * TILE_SIZE, (WIDTH_IN_TILES - 3) * TILE_SIZE, (HEIGHT_IN_TILES - 14) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->money, buffer), 4 * TILE_SIZE, 13 * TILE_SIZE, 3 * TILE_SIZE, (HEIGHT_IN_TILES - 14) * TILE_SIZE, (SDL_Color){16, 32, 140}, true);

    int exitCode = OVERWORLD_ITEMS;
    SDL_Event e;
    bool quit = false;
    while(!quit)
    {
        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = cursor.x, .y = cursor.y, .w = cursor.w, .h = cursor.w}));
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if(e.type == SDL_QUIT)
            {
                quit = true;
                exitCode = ANYWHERE_QUIT;
            }
            //User presses a key
            else if(e.type == SDL_KEYDOWN)
            {
                //Select surfaces based on key press
                switch(e.key.keysym.sym)
                {
                    case SDLK_w:
                        if (cursor.y > 1 * TILE_SIZE)
                            cursor.y -= TILE_SIZE;
                    break;

                    case SDLK_s:
                        if (cursor.y < 12 * TILE_SIZE)
                            cursor.y += TILE_SIZE;
                    break;

                    case SDLK_SPACE:
                        quit = true;
                    break;
                    default:
                    break;
                }
            }
        }
        drawTile(cursor.tileIndex, cursor.x, cursor.y, TILE_SIZE, SDL_FLIP_NONE);
        SDL_RenderPresent(mainRenderer);
    }
    return exitCode;
}

bool doBattle(player* player, bool isBoss)
{
    bool won = false, run = false;
    int menuLevel = 1;
    printf("Did battle after %d steps.\n", player->steps);
    player->steps = 0;
    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0xFF);
    for(int i = 0; i < SCREEN_WIDTH; i += 2)
    {
        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 0, .y = 0, .w = i, .h = SCREEN_HEIGHT}));
        SDL_RenderPresent(mainRenderer);
    }
    SDL_Delay(350);
    //W1 = 123, 142, 231 / 7B8EE7 / xLIBC palette color 124
    //W2 = 198, 121, 24 / C67918 / xLIBC palette color 195
    //W3 = 132, 113, 24 / 847118 / xLIBC palette color 131
    //W4 = 16, 227, 189 / 10E3BD / xLIBC palette color 23
    //W5 = 16, 97, 156 / 10619C / xLIBC palette color 19
    //W6 = 8, 97, 90 / 08615A / xLIBC palette color 13
    //W7 = 231, 158, 33 / E79E21 / xLIBC palette color 228
    //W8 = 66, 105, 24 / 426918 / xLIBC palette color 67
    Uint8 rBG = 0, gBG = 0, bBG = 0;
    if (player->worldNum == 1)
    {
        rBG = 0x7B;
        gBG = 0x8E;
        bBG = 0xE7;
    }
    if (player->worldNum == 2)
    {
        rBG = 0xC6;
        gBG = 0x79;
        bBG = 0x18;
    }
    if (player->worldNum == 3)
    {
        rBG = 0x84;
        gBG = 0x71;
        bBG = 0x18;
    }
    if (player->worldNum == 4)
    {
        rBG = 0x10;
        gBG = 0xE3;
        bBG = 0xBD;
    }
    if (player->worldNum == 5)
    {
        rBG = 0x10;
        gBG = 0x61;
        bBG = 0x9C;
    }
    if (player->worldNum == 6)
    {
        rBG = 0x08;
        gBG = 0x61;
        bBG = 0x5A;
    }
    if (player->worldNum == 7)
    {
        rBG = 0xE7;
        gBG = 0x9E;
        bBG = 0x21;
    }
    if (player->worldNum == 8)
    {
        rBG = 0x42;
        gBG = 0x69;
        bBG = 0x18;
    }
    SDL_SetRenderDrawColor(mainRenderer, rBG, gBG, bBG, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    for(int i = 0; i < 20; i++)
        drawTile(player->worldNum - 1 + 8 * (i == 0 || i == 19), i * TILE_SIZE, 8 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
    sprite enemy;
    int enemyIndex;
    if (!isBoss)
        enemyIndex = (rand() % 2) + 1 + 3 * (player->worldNum - 1);
    else
        enemyIndex = 3 * player->worldNum + 3 * (player->worldNum == 7 && player->mapScreen == 2.3);
    int enemyHP = 14 + iPart(pow((double) enemyIndex, 1.22)) + (int) pow((5 + 1), player->worldNum) * (!enemyIndex % 3);
    //14+int(U^1.22)+40(M=8.33 and U/3=int(U/3))+(5+1^int(M))(U/3=int(U/3
    initSprite(&enemy, 15 * TILE_SIZE, 7 * TILE_SIZE, TILE_SIZE, enemyIndex + 71, type_na);
    char* enemyName;
    {
        char* enemyNames[] = ALL_ENEMIES_ARRAY;
        enemyName = enemyNames[enemyIndex - 1];
    }
    char* allAttacks = ALL_ATTACKS;
    bool doneFlag = false;
    bool blockTurns = 0;
    while (!doneFlag)
    {
    bool actFlag = false;
    int exitCode = 0;
        while(!actFlag)
        {
            char* textBoxText = "error\0";
            if (menuLevel == 1)
            {
                char* buffer = "";
                textBoxText = "W: ATTACK          S: BLOCK       LSHIFT: RUN";
                drawTile(player->spr.tileIndex, 4 * TILE_SIZE, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                drawTile(enemy.tileIndex, enemy.x, enemy.y, enemy.w, SDL_FLIP_NONE);
                drawTextBox(player->name, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 2 * TILE_SIZE, .w = 9 * TILE_SIZE, .h = 4 * TILE_SIZE});
                drawTextBox(enemyName, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.x = 11 * TILE_SIZE, .y = 2 * TILE_SIZE, .w = 9 * TILE_SIZE, .h = 4 * TILE_SIZE});
                drawText("Lv", 2 * TILE_SIZE / 8, 7 * TILE_SIZE / 2, 9 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText(toString(player->level, buffer), 26 * TILE_SIZE / 8, 7 * TILE_SIZE / 2, 9 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText("HP", 2 * TILE_SIZE / 8, 39 * TILE_SIZE / 8, 9 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText(toString(player->HP, buffer), 13 * TILE_SIZE / 4, 39 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText("HP", 11 * TILE_SIZE + TILE_SIZE / 4, 39 * TILE_SIZE / 8, 9 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText(toString(enemyHP, buffer), 14 * TILE_SIZE + TILE_SIZE / 4, 39 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
            }
            if (menuLevel == 2)
            {
                char thisAttack[6] = "     \0";
                char input[89];
                strcpy(input, "W: ");
                strcat(input, strncpy(thisAttack, (allAttacks + (player->move1 - 40) * 5), 5));
                strcat(input, "           A: ");
                strcat(input, player->move2 ? strncpy(thisAttack, (allAttacks + (player->move2 - 40) * 5), 5) : "     ");
                strcat(input, "           S: ");
                strcat(input, player->move3 ? strncpy(thisAttack, (allAttacks + (player->move3 - 40) * 5), 5) : "     ");
                strcat(input, "           D: ");
                strcat(input, player->move4 ? strncpy(thisAttack, (allAttacks + (player->move4 - 40) * 5), 5) : "     ");
                strcat(input, "           LSHIFT: BACK\0");
                //3 + 5 chars + 11 spaces --> 88 total chars
                //don't forget move1 -> w, move -> a, move3 ->s, move4 -> d
                textBoxText = input;
            }
            drawTextBox(textBoxText, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});

            SDL_Event e;
            bool quit = false;
            SDL_Delay(500);
            while(!quit)
            {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                        exitCode = ANYWHERE_QUIT;
                    }
                    //User presses a key
                    else if(e.type == SDL_KEYDOWN)
                    {
                        //Select surfaces based on key press
                        switch(e.key.keysym.sym)
                        {
                            case SDLK_w:
                                exitCode = BATTLE_ACT_CODE_W;
                                quit = true;
                            break;

                            case SDLK_a:
                                if (menuLevel == 2)
                                {
                                    exitCode = BATTLE_ACT_CODE_A;
                                    quit = true;
                                }
                            break;

                            case SDLK_s:
                                exitCode = BATTLE_ACT_CODE_S;
                                quit = true;
                            break;

                            case SDLK_d:
                                if (menuLevel == 2)
                                {
                                    exitCode = BATTLE_ACT_CODE_D;
                                    quit = true;
                                }
                            break;

                            case SDLK_LSHIFT:
                                exitCode = BATTLE_ACT_CODE_LSHIFT;
                                quit = true;
                            break;

                            break;
                            default:
                            break;
                        }
                    }
                }
            }
            if (exitCode == BATTLE_ACT_CODE_W && menuLevel == 1)
            {
                menuLevel = 2;
                exitCode = 0;
            }
            if ((exitCode == BATTLE_ACT_CODE_LSHIFT || exitCode == ANYWHERE_QUIT) && menuLevel == 2)
            {
                menuLevel = 1;
                exitCode = 0;
            }
            if ((exitCode > 0 && ((exitCode == BATTLE_ACT_CODE_W && player->move1) || (exitCode == BATTLE_ACT_CODE_A && player->move2) ||
                                  (exitCode == BATTLE_ACT_CODE_S && player->move3) || (exitCode == BATTLE_ACT_CODE_D && player->move4)) &&
                                   menuLevel == 2) || (exitCode != 0 && exitCode != BATTLE_ACT_CODE_W && menuLevel == 1))
                actFlag = true;
        }
        //proceed with attacks here
        int attackCode = 0;
        if (menuLevel == 2)
        {
            if (exitCode == BATTLE_ACT_CODE_W)
                attackCode = player->move1;
            if (exitCode == BATTLE_ACT_CODE_A)
                attackCode = player->move2;
            if (exitCode == BATTLE_ACT_CODE_S)
                attackCode = player->move3;
            if (exitCode == BATTLE_ACT_CODE_D)
                attackCode = player->move4;
        }
        if (menuLevel == 1)
        {
            if (exitCode == BATTLE_ACT_CODE_LSHIFT || exitCode == ANYWHERE_QUIT)
            {
                attackCode = ATTACK_CODE_RUN;
                //not(boss) and 3>randInt(1,U-100fPart(A
                if (3 > (rand() % abs(enemyIndex)) - player->speed)
                {
                    won = false;
                    run = true;
                }
            }
            if (exitCode == BATTLE_ACT_CODE_S)
                attackCode = ATTACK_CODE_BLOCK;

        }
        //If max(U={4,5})max(V={48,49,64,65}) or max(U={7,8,9})max(V={56,57}) or max(U={10,11,12})max(V={42,43,58,59}) or max(U={13,14,15})max(V={56,57}) or max(U={16,17,18})max(V={48,49,64,65}) or max(U={19,20,21})max(V={50,51,66,67}) or not(U=21)max(V={54,70
        int playerDMG = 0, enemyDMG = 0;
        bool crit = false, superEffective = false, playerTurn = false;
        int speedMinusIndex = player->speed - enemyIndex;
        int speedDiff = rand() % (abs(speedMinusIndex) + 2) * -1 * (speedMinusIndex < 0);
        //randInt(1,100fPart(A)-U)=K
        //if K >= 0 you go first
        //random number between 1 (floor) and speed - enemy index
        char textBoxText[22];
        char* buffer = "";
        for(int s = 0; s < 2; s++)
        {
            int redrawIndex = 0, redrawX = -TILE_SIZE, redrawY = -TILE_SIZE;
            if (((s == 0 && speedDiff >= -1) || (s == 1 && speedDiff < -1)))
            {
                playerTurn = true;
                if (player->HP > 0)
                {
                    crit = 0 < ((rand() % (11 + 2 * (attackCode > 55) + 5 * (attackCode == ATTACK_CODE_BLOCK || attackCode == ATTACK_CODE_RUN))) - 9);
                    if (crit && attackCode != ATTACK_CODE_BLOCK && attackCode != ATTACK_CODE_RUN)
                    drawTile(TILE_ID_CRITICAL, enemy.x, enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                    drawTile(attackCode, attackCode == ATTACK_CODE_BLOCK || attackCode == ATTACK_CODE_RUN ? 4 * TILE_SIZE : enemy.x, attackCode == ATTACK_CODE_BLOCK || attackCode == ATTACK_CODE_RUN ? 7 * TILE_SIZE : enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                    strcpy(textBoxText, player->name);
                    strcat(textBoxText, " USED ");
                    char input[6];
                    if (attackCode != ATTACK_CODE_RUN && attackCode != ATTACK_CODE_BLOCK)
                    {
                        strncpy(input, (allAttacks + (attackCode - 40) * 5), 5);
                        input[5] = '\0';

                        double rawDMG;
                        if (attackCode < 55)
                            rawDMG = iPart( 7 + pow(((player->attack - 1.0) * 1.2), 1.3));
                        if (attackCode > 55)
                            rawDMG = iPart(6 + pow(((player->attack - 1) * 1.2), 1.25));
                        if (attackCode % 2 == 1)
                        {
                            if (attackCode < 54)
                                rawDMG += iPart(pow(player->attack, 1.45));
                            if (attackCode > 56)
                                rawDMG += iPart(pow(player->attack, 1.5));
                        }
                        if (crit)
                            rawDMG *= 1.5;
                        if (superEffective)
                            rawDMG *= 1.35;
                        if (blockTurns)
                            rawDMG *= .8;
                        playerDMG = (int) rawDMG;
                        enemyHP -= playerDMG;
                        if (enemyHP < 1)
                        {
                            won = true;
                            enemyHP = 0;
                        }
                        SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
                        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 14 * TILE_SIZE + TILE_SIZE / 4, .y = 39 * TILE_SIZE / 8, .w = 3 * TILE_SIZE, .h = TILE_SIZE}));
                        drawText(toString(enemyHP, buffer), 14 * TILE_SIZE + TILE_SIZE / 4, 39 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
                        SDL_RenderPresent(mainRenderer);
                        SDL_SetRenderDrawColor(mainRenderer, rBG, gBG, bBG, 0xFF);
                        redrawIndex = enemy.tileIndex;
                        redrawX = enemy.x;
                        redrawY = enemy.y;
                    }
                    else
                    {
                        strcpy(input, (attackCode == ATTACK_CODE_RUN ? "RUN! \0" : "BLOCK\0"));
                        redrawIndex = player->spr.tileIndex;
                        redrawX = 4 * TILE_SIZE;
                        redrawY = 7 * TILE_SIZE;
                        if (attackCode == ATTACK_CODE_BLOCK)
                            blockTurns = 3;
                    }
                    strcat(textBoxText, input);
                }
            }
            else
            {
                playerTurn = false;
                if (enemyHP > 0 && !run)
                {
                    drawTile(39 + enemyIndex - (enemyIndex % 3 == 0), 4 * TILE_SIZE, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                    strcpy(textBoxText, enemyName);
                    strcat(textBoxText, " USED ");
                    char input[6];
                    strncpy(input, (allAttacks + (enemyIndex - (enemyIndex % 3 == 0) - 1) * 5), 5);
                    input[5] = '\0';
                    strcat(textBoxText, input);
                    enemyDMG = (int) (6 + (pow((double) enemyIndex, 1.13 + .05 * (enemyIndex % 3 == 0))));
                    if (blockTurns)
                    {
                        enemyDMG *= .5;
                        blockTurns--;
                    }
                    player->HP -= enemyDMG;
                    if (player->HP < 1)
                    {
                        player->HP = 0;
                    }
                    char* buffer = "";
                    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
                    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 26 * TILE_SIZE / 8, .y = 39 * TILE_SIZE / 8, .w = 3 * TILE_SIZE, .h = TILE_SIZE}));
                    drawText(toString(player->HP, buffer), 26 * TILE_SIZE / 8, 39 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                    SDL_RenderPresent(mainRenderer);
                    SDL_SetRenderDrawColor(mainRenderer, rBG, gBG, bBG, 0xFF);
                    //redraw player HP here
                }
                redrawIndex = player->spr.tileIndex;
                redrawX = 4 * TILE_SIZE;
                redrawY = 7 * TILE_SIZE;
            }
            if ((playerTurn && player->HP > 0) || (!playerTurn && enemyHP > 0 && !run))
            {
                bool dispDamage = false;
                drawTextBox(textBoxText, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
                if (((attackCode !=  ATTACK_CODE_BLOCK && attackCode != ATTACK_CODE_RUN) && playerTurn) || !playerTurn)
                {
                    drawText("DAMAGE:", TILE_SIZE / 4, 10 * TILE_SIZE + 2 * TILE_SIZE / 8 , SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                    drawText(toString(playerTurn ? playerDMG : enemyDMG, buffer), 8 * TILE_SIZE + 2 * TILE_SIZE / 8, 10 * TILE_SIZE + 2 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
                    dispDamage = true;
                }
                if (playerTurn && blockTurns && !run)
                {
                    drawText("BLOCK TURNS:", TILE_SIZE / 4, (10 + dispDamage) * TILE_SIZE + 2 * TILE_SIZE / 8, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                    drawText(toString(blockTurns, buffer), 13 * TILE_SIZE + 2 * TILE_SIZE / 8, (10 + dispDamage) * TILE_SIZE + 2 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
                }
                SDL_Delay(750);
                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = redrawX, .y = redrawY, .w = TILE_SIZE, .h = TILE_SIZE}));
                drawTile(redrawIndex, redrawX, redrawY, TILE_SIZE, SDL_FLIP_NONE);
                SDL_RenderPresent(mainRenderer);
                SDL_Event e;
                bool quit = false;
                while(!quit)
                {
                    while(SDL_PollEvent(&e) != 0)
                    {
                        if(e.type == SDL_QUIT)
                            quit = true;
                        else
                            if(e.type == SDL_KEYDOWN)
                                quit = true;
                    }
                }
            }
        }
        if (enemyHP < 1)
            won = true;
        if (won || run || player->HP < 1)
            doneFlag = true;

        menuLevel = 1;
    }
    if (!won && !run)
    {
        player->HP = player->maxHP;
        player->mapScreen = 1.0;
        player->spr.x = 4 * TILE_SIZE;
        player->spr.y = 6 * TILE_SIZE;
        SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0xFF);
        for(int i = 0; i <= TILE_SIZE * 9; i += 2)
        {
            SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.w = SCREEN_WIDTH, .h = i}));
            SDL_RenderPresent(mainRenderer);
            SDL_Delay(2);
        }
    SDL_Delay(650);
    }
    drawTextBox(won ? "You won!" : run ? "You fled!" : "You lost!", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
    SDL_Event e;
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
                quit = true;
            else
                if(e.type == SDL_KEYDOWN)
                    quit = true;
        }
    }
    if (won)
    {
        int acquiredGold = 4 - (player->level - enemyIndex);
        int acquiredExp = 23 + 3 * enemyIndex + (3 * player->worldNum) * (enemyIndex % 3 == 0) - 4 * (player->level - enemyIndex);
        if (acquiredGold < 0)
            acquiredGold = 0;
        if (acquiredExp < 0)
            acquiredExp = 0;
        player->money += acquiredGold;
        player->experience += acquiredExp;
        char* buffer = "";
        drawTextBox("EXP:", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
        drawText(toString(acquiredExp, buffer), 4 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);

        drawText("GOLD:", TILE_SIZE / 4, 10 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawText(toString(acquiredGold, buffer), 5 * TILE_SIZE + TILE_SIZE / 4, 10 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);

        {
            int remainingExp = (int)(50 + pow((player->level - 1), 1.75)) - player->experience;
            if (remainingExp < 0)
                remainingExp = 0;
            drawText("EXP TO LV UP:", TILE_SIZE / 4, 11 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
            drawText(toString(remainingExp, buffer), 13 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        }
        drawText("TOTAL GOLD:", TILE_SIZE / 4, 12 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawText(toString(player->money, buffer), 11 * TILE_SIZE + TILE_SIZE / 4, 12 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
        SDL_PollEvent(&e);
        quit = false;
        SDL_Delay(400);
        while(!quit)
        {
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_QUIT)
                    quit = true;
                else
                    if(e.type == SDL_KEYDOWN)
                        quit = true;
            }
        }
        if (player->level < 99 && player->experience >= (int) 50 + pow((player->level - 1), 1.75))
        {
            player->level++;
            player->experience -= (int) 50 + pow((player->level - 2), 1.75);
            player->maxHP += (rand() % 3) + 5 + 2 * (enemyIndex % 3 != 1);
            if (player->maxHP > 999)
                player->maxHP = 999;
            player->HP = player->maxHP;
            player->statPts++;
            if (rand() % 5 == 0)
                player->statPts++; //random chance for an extra stat pt
            if (player->statPts > 99)
                player->statPts = 99;
            drawTextBox("Level up!          Lv", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
            drawText(toString(player->level, buffer), 3 * TILE_SIZE + TILE_SIZE / 4, 10 * TILE_SIZE + TILE_SIZE / 4, 2 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
            SDL_PollEvent(&e);
            quit = false;
            SDL_Delay(400);
            while(!quit)
            {
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                        quit = true;
                    else
                        if(e.type == SDL_KEYDOWN)
                            quit = true;
                }
            }
            //doing stat increases
            SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
            quit = false;
            SDL_Delay(400);
            sprite cursor;
            initSprite(&cursor, TILE_SIZE / 4, 9 * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE, TILE_ID_CURSOR, type_na);
            int breakCode = 0;
            while (!quit)
            {
                drawTextBox(" ATK:", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
                drawText(toString(player->attack, buffer), 5 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE + TILE_SIZE / 4, 2 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText("SPD:", 5 * TILE_SIZE / 4, 10 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText(toString(player->speed, buffer), 5 * TILE_SIZE + TILE_SIZE / 4, 10 * TILE_SIZE + TILE_SIZE / 4, 2 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText("STAT PTS=", 5 * TILE_SIZE / 4, 11 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText(toString(player->statPts, buffer), 10 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE + TILE_SIZE / 4, 2 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText("DONE", 5 * TILE_SIZE / 4, 12 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
                while(!quit)
                {
                    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = cursor.x, .y = cursor.y, .w = cursor.w, .h = cursor.w}));
                    while(SDL_PollEvent(&e) != 0)
                    {
                        if(e.type == SDL_QUIT)
                        {
                            quit = true;
                            breakCode = ANYWHERE_QUIT;
                        }
                        else
                            if(e.type == SDL_KEYDOWN)
                            {
                                switch (e.key.keysym.sym)
                                {
                                    case SDLK_w:
                                        if (cursor.y > 9 * TILE_SIZE + TILE_SIZE / 4)
                                            cursor.y -= TILE_SIZE;
                                    break;

                                    case SDLK_s:
                                        if (cursor.y < 12 * TILE_SIZE + TILE_SIZE / 4)
                                            cursor.y += TILE_SIZE;
                                    break;
                                    case SDLK_SPACE:
                                        if (cursor.y == 12 * TILE_SIZE + TILE_SIZE / 4)
                                            quit = true;
                                        else
                                        {
                                            if (player->statPts)
                                            {
                                                if (cursor.y == 9 * TILE_SIZE + TILE_SIZE / 4)
                                                    player->attack++;
                                                if (cursor.y == 10 * TILE_SIZE + TILE_SIZE / 4)
                                                    player->speed++;
                                                if (cursor.y != 11 * TILE_SIZE + TILE_SIZE / 4)
                                                {
                                                    player->statPts--;
                                                    quit = true;
                                                }
                                            }
                                        }
                                    break;

                                    default:
                                    break;
                                }
                            }
                    }
                    drawTile(cursor.tileIndex, cursor.x, cursor.y, TILE_SIZE, SDL_FLIP_NONE);
                    SDL_RenderPresent(mainRenderer);
                }
                //check if user really wants to quit
                if (cursor.y != 12 * TILE_SIZE + TILE_SIZE / 4 || breakCode == ANYWHERE_QUIT)
                    quit = false;
                else
                {
                    if (player->statPts)
                    {
                        drawTextBox("QUIT WITHOUT USING ALL PTS?        SPACE = YES      ANYTHING ELSE = NO", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
                        bool unpause = false;
                        SDL_Delay(400);
                        while(!unpause)
                        {
                            while(SDL_PollEvent(&e) != 0)
                            {
                                if(e.type == SDL_QUIT)
                                    unpause = true;
                                else
                                    if(e.type == SDL_KEYDOWN)
                                    {
                                        switch (e.key.keysym.sym)
                                        {
                                            case SDLK_SPACE:
                                            break;

                                            default:
                                                quit = false;
                                            break;
                                        }
                                        unpause = true;
                                    }
                            }
                        }
                    }
                }
            }
        }
    }
    return won || run;
}
