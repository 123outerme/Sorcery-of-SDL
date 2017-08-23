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
#define TILE_ID_CUBED 20
#define TILE_ID_TILDA 21
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

#define OVERWORLD_RETURN 1
#define OVERWORLD_STATS 2
#define OVERWORLD_ITEMS 3
#define OVERWORLD_SAVE_RETURN 4
#define OVERWORLD_QUIT 5

#define OVERWORLD_MENU_PALETTE (SDL_Color){181, 182, 173}, (SDL_Color){181, 182, 173}, (SDL_Color){16, 32, 140}, (SDL_Color){16, 32, 140}

#define ALL_ATTACKS "SLICESLASHBURN ROASTCRACKBREAKCHILLICE  FLOW SWEEPWHACKBASH STAB GASH SMASH     THORNVINE FLAREBLAZEROCK STONEFROSTHAIL STORMVOLT SMELLSTINKDARK EVIL ALPHAARROW"

//Todo for 8/24:
//* Make world-sized tilemaps work (and smooth scrolling)?
//** Create world-sized tilemaps by stitching together the individual CSE map pngs, throw them in the xLIBC map generator, done
//** Make them work by doing nice scroll animations between map borders
//** Make sure you only render screen-sized chunks at a time

int main(int argc, char* args[])
{
	//Start up SDL and create window
	//sprite player;
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
                if (loopCode == LOOP_GOTO_BATTLE)
                    doBattle(&player);

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
    drawTile(player->spr.tileIndex, player->spr.x / player->spr.w, player->spr.y / player->spr.w, player->spr.w, player->flip);
    if (textBoxOn)
        drawTextBox(textInput, player);
    SDL_RenderPresent(mainRenderer);
}

int mainLoop(player* playerSprite)
{
    bool quit = false, press = false;
    SDL_Event e;
    int frame = 0, exitCode = LOOP_QUIT;
    char* textInput = "Did you know that this is placeholder text? Me neither.";
    drawTilemap(0, 0, WIDTH_IN_TILES, HEIGHT_IN_TILES);
    drawHUD(playerSprite);
    sprite entity;
    entityType type;
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
    initSprite(&entity, x, y, TILE_SIZE, index, type);
    drawTile(entity.tileIndex, entity.x / TILE_SIZE, entity.y / TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
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
    char* buffer = "";
    char* worldNames[8] = {"Plain Plains", "Dragon's Den", "Worry Quarry", "West Pole", "River Lake", "Under City", "Upper City", "Battleground"};
    drawText(worldNames[player->worldNum - 1], 0, 0, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    buffer = toString(player->HP, buffer);
    drawText(buffer, (16 - strlen(buffer)) * TILE_SIZE, 0, TILE_SIZE, ((10 * player->HP / player->maxHP) > 3 ? ((SDL_Color) {231, 223, 49}) : ((SDL_Color) {255, 0, 0})), false);
    drawText("/", 16 * TILE_SIZE, 0, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    drawText(toString(player->maxHP, buffer), 17 * TILE_SIZE, 0, TILE_SIZE, ((SDL_Color) {231, 223, 49}), true);
}

void drawTextBox(char* input, player* player)
{
    if (!player->mapScreen && player->overworldX == 192 && player->overworldY == 240)
        player->HP = player->maxHP;
    //19 letters per line/5 lines at 48pt font
    SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.y = 8.5 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 8.5) * TILE_SIZE}));
    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = 8.65 * TILE_SIZE, .w = SCREEN_WIDTH, .w = 126 * SCREEN_WIDTH / 128, .h = (HEIGHT_IN_TILES - 8.75) * TILE_SIZE}));
    drawText(input, SCREEN_WIDTH / 64, 8.75 * TILE_SIZE, (HEIGHT_IN_TILES - 8.25) * TILE_SIZE, (SDL_Color){0, 0, 0}, true);
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
    drawText(title, 2 * TILE_SIZE + 3 * TILE_SIZE / 8, 11 * SCREEN_HEIGHT / 128, 119 * SCREEN_HEIGHT / 128, titleColorUnder, false);
    //foreground text
    drawText(title, 2 * TILE_SIZE + TILE_SIZE / 4, 5 * SCREEN_HEIGHT / 64, 55 * SCREEN_HEIGHT / 64, titleColorOver, false);

    drawText(opt1, 2 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, (HEIGHT_IN_TILES - 5) * TILE_SIZE, textColor, false);
    drawText(opt2, 2 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, (HEIGHT_IN_TILES - 6) * TILE_SIZE, textColor, false);
    drawText(opt3, 2 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, (HEIGHT_IN_TILES - 7) * TILE_SIZE, textColor, false);
    drawText(opt4, 2 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, (HEIGHT_IN_TILES - 8) * TILE_SIZE, textColor, false);
    drawText(opt5, 2 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE, (HEIGHT_IN_TILES - 9) * TILE_SIZE, textColor, !showVersion);

    if (showVersion)
    {
        drawTile(TILE_ID_TILDA, 0, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawTile(TILE_ID_CUBED, 1, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawTile(TILE_ID_TILDA, 2, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawText(FULLVERSION_STRING, 2 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){24, 195, 247}, true);
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
                    case SDLK_UP:
                        if (cursor.y > 5 * TILE_SIZE)
                            cursor.y -= TILE_SIZE;
                    break;

                    case SDLK_DOWN:
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
        drawTile(cursor.tileIndex, cursor.x / TILE_SIZE, cursor.y / TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
        SDL_RenderPresent(mainRenderer);
    }
    return selection;
}

int aWallOfText(char* title, char* text, bool showHelpInfo)
{
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    //background text (drawn first)
    drawText(title, 21 * SCREEN_WIDTH / 128, 1 * SCREEN_HEIGHT / 128, 127 * SCREEN_HEIGHT / 128, (SDL_Color){24, 65, 214}, false);
    //foreground text
    drawText(title, 10 * SCREEN_WIDTH / 64, 0, SCREEN_HEIGHT, (SDL_Color){24, 162, 239}, false);

    drawText(text, 0, TILE_SIZE + TILE_SIZE / 2, (HEIGHT_IN_TILES - 2) * TILE_SIZE, (SDL_Color){24, 195, 247}, !showHelpInfo);
    if (showHelpInfo)
        drawText("Made by Stephen Policelli", 0, 13 * TILE_SIZE, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){24, 195, 247}, true);

    SDL_Event e;
    bool quit = false;
    //While application is running
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
                //Select surfaces based on key press
                switch(e.key.keysym.sym)
                {
                    default:
                        quit = true;
                    break;
                }
            }
        }
    }
    return MENU_HELP;
}

int showStats(player* player)
{
    int exitCode = OVERWORLD_STATS;
    char* buffer = "";
    SDL_RenderFillRect(mainRenderer, NULL);
    SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = 1 * SCREEN_HEIGHT / 128, .w = 126 * SCREEN_WIDTH / 128, .h = 126 * SCREEN_HEIGHT / 128}));
    //foreground text
    drawText("STATS", 2 * TILE_SIZE + TILE_SIZE / 4, 4 * SCREEN_HEIGHT / 64, 54 * SCREEN_HEIGHT / 64, (SDL_Color){16, 32, 140}, false);

    drawText("BACK", 2 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, (HEIGHT_IN_TILES - 3) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(player->name, 4 * TILE_SIZE + TILE_SIZE / 4, 4 * TILE_SIZE, (HEIGHT_IN_TILES - 4) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawTile(TILE_ID_CURSOR, 1, 3, TILE_SIZE, SDL_FLIP_NONE);
    drawTile(TILE_ID_PLAYER, 2, 4, TILE_SIZE, SDL_FLIP_NONE);

    drawText("LVL:    XP:", 4 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, (HEIGHT_IN_TILES - 5) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->level, buffer), 8 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, (HEIGHT_IN_TILES - 5) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(toString(player->experience, buffer), 15 * TILE_SIZE + TILE_SIZE / 8, 5 * TILE_SIZE, (HEIGHT_IN_TILES - 5) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    drawText("HP:    /   ", 4 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, (HEIGHT_IN_TILES - 6) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->HP, buffer), (10 - digits(player->HP)) * TILE_SIZE + 7 * TILE_SIZE / 8, 6 * TILE_SIZE, (HEIGHT_IN_TILES - 6) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(toString(player->maxHP, buffer), 12 * TILE_SIZE + 2 * TILE_SIZE / 4, 6 * TILE_SIZE, (HEIGHT_IN_TILES - 6) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    drawText("ATK:    SPD:", 4 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, (HEIGHT_IN_TILES - 7) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->attack, buffer), 8 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, (HEIGHT_IN_TILES - 7) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(toString(player->speed, buffer), 16 * TILE_SIZE + 5 * TILE_SIZE / 16, 7 * TILE_SIZE, (HEIGHT_IN_TILES - 7) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    drawText("STAT PTS=", 4 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, (HEIGHT_IN_TILES - 8) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
    drawText(toString(player->statPts, buffer), 13 * TILE_SIZE + TILE_SIZE / 8, 8 * TILE_SIZE, (HEIGHT_IN_TILES - 8) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);

    char* allAttacks = ALL_ATTACKS, thisAttack[6] = "     \0";
    if (player->move1)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move1 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 9 * TILE_SIZE, (HEIGHT_IN_TILES - 9) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move1, 12, 9, TILE_SIZE, SDL_FLIP_NONE);
        }
    if (player->move2)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move2 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 10 * TILE_SIZE, (HEIGHT_IN_TILES - 10) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move2, 12, 10, TILE_SIZE, SDL_FLIP_NONE);
        }
    if (player->move3)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move3 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 11 * TILE_SIZE, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move3, 12, 11, TILE_SIZE, SDL_FLIP_NONE);
        }
    if (player->move4)
        {
        drawText(strncpy(thisAttack, (allAttacks + (player->move4 - 40) * 5), 5), 6 * TILE_SIZE +  TILE_SIZE / 4, 12 * TILE_SIZE, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawTile(player->move4, 12, 12, TILE_SIZE, SDL_FLIP_NONE);
        }
    drawText("^:", 4 * TILE_SIZE +  TILE_SIZE / 4, 9 * TILE_SIZE, (HEIGHT_IN_TILES - 9) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(">:", 4 * TILE_SIZE +  TILE_SIZE / 4, 10 * TILE_SIZE, (HEIGHT_IN_TILES - 10) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText("V:", 4 * TILE_SIZE +  TILE_SIZE / 4, 11 * TILE_SIZE, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText("<:", 4 * TILE_SIZE +  TILE_SIZE / 4, 12 * TILE_SIZE, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){0, 0, 0}, true);

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
    int exitCode = OVERWORLD_ITEMS;
    return exitCode;
}

bool doBattle(player* player)
{
    int won = true;
    return won;
}
