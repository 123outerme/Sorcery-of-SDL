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

#define OVERWORLD_RETURN 1
#define OVERWORLD_STATS 2
#define OVERWORLD_ITEMS 3
#define OVERWORLD_SAVE_RETURN 4
#define OVERWORLD_QUIT 5

#define OVERWORLD_MENU_PALETTE (SDL_Color){181, 182, 173}, (SDL_Color){181, 182, 173}, (SDL_Color){16, 32, 140}, (SDL_Color){16, 32, 140}

#define ALL_ATTACKS "SLICESLASHBURN ROASTCRACKBREAKCHILLICE  FLOW SWEEPWHACKBASH STAB GASH SMASH     THORNVINE FLAREBLAZEROCK STONEFROSTHAIL STORMVOLT SMELLSTINKDARK EVIL ALPHAARROW"
#define ALL_ENEMIES_ARRAY {"Ant", "Rat", "APEMAN", "Firant", "Pyre", "FEENIX", "Gemdog", "Golem", "TARANT", "Pengin", "Snoman", "POLARA", "C Gull", "C Star", "HYDROA", "Rodent", "Crow", "SWURM", "Midas", "Greed", "SENTRY", "Archer", "Knight", "- DREGOH -"}

//Todo for 8/24:
//* Make world-sized tilemaps work (and smooth scrolling)?
//** Create world-sized tilemaps by stitching together the individual CSE map pngs, throw them in the xLIBC map generator, done
//** Make them work by doing nice scroll animations between map borders
//** Make sure you only render screen-sized chunks at a time

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
                if (loopCode == LOOP_GOTO_BATTLE)
                    doBattle(&player);
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
    drawText(worldNames[player->worldNum - 1], 0, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    buffer = toString(player->HP, buffer);
    drawText(buffer, (16 - strlen(buffer)) * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((10 * player->HP / player->maxHP) > 3 ? ((SDL_Color) {231, 223, 49}) : ((SDL_Color) {255, 0, 0})), false);
    drawText("/", 16 * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    drawText(toString(player->maxHP, buffer), 17 * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {231, 223, 49}), true);
}

void drawTextBox(char* input, player* player, SDL_Color outlineColor, SDL_Rect textBoxRect)
{
    if (!player->mapScreen && player->overworldX == 192 && player->overworldY == 240)
        player->HP = player->maxHP;
    //19 letters per line/5 lines at 48pt font
    SDL_SetRenderDrawColor(mainRenderer, outlineColor.r, outlineColor.g, outlineColor.b, 0xFF);
    SDL_RenderFillRect(mainRenderer, &(textBoxRect));
    SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = textBoxRect.x + TILE_SIZE / 8, .y = textBoxRect.y + TILE_SIZE / 8, .w = textBoxRect.w -  2 * TILE_SIZE / 8, .h = textBoxRect.h - 2 * TILE_SIZE / 8}));

    drawText(input, textBoxRect.x + 2 * TILE_SIZE / 8, textBoxRect.y + 2 * TILE_SIZE / 8, textBoxRect.w -  3 * TILE_SIZE / 8, textBoxRect.h -  3 * TILE_SIZE / 8, (SDL_Color){0, 0, 0}, true);
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
        drawTile(TILE_ID_TILDA, 0, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawTile(TILE_ID_CUBED, 1 * TILE_SIZE, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawTile(TILE_ID_TILDA, 2 * TILE_SIZE, 0, TILE_SIZE, SDL_FLIP_NONE);
        drawText(FULLVERSION_STRING, 2 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){24, 195, 247}, true);
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

bool doBattle(player* player)
{
    bool won = true, run = false;
    int menuLevel = 1;
    printf("Did battle after %d steps.\n", player->steps);
    player->steps = 0;
    SDL_SetRenderDrawColor(mainRenderer, 0x7B, 0x8E, 0xE7, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    //W1 = 123, 142, 231 / 7B8EE7 / xLIBC palette color 124
    //W2 = 198, 121, 24 / C67918 / xLIBC palette color 195
    //W3 = 132, 113, 24 / 847118 / xLIBC palette color 131
    //W4 = 16, 227, 189 / 10E3BD / xLIBC palette color 23
    //W5 = 16, 97, 156 / 10619C / xLIBC palette color 19
    //W6 = 8, 97, 90 / 08615A / xLIBC palette color 13
    //W7 = 231, 158, 33 / E79E21 / xLIBC palette color 228
    //W8 = 66, 105, 24 / 426918 / xLIBC palette color 67
    for(int i = 0; i < 20; i++)
        drawTile(player->worldNum - 1 + 8 * (i == 0 || i == 19), i * TILE_SIZE, 8 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
    sprite enemy;
    //enemy = randInt(1+3int(M-1),2+3int(M-1
    //bosses = 3(M=7.23)+3int(M
    int enemyIndex = (rand() % 2) + 1 + 3 * (player->worldNum - 1);
    int enemyHP = 14 + iPart(pow((double) enemyIndex, 1.22)) + (5 + 1) * (!enemyIndex % 3);
    //14+int(U^1.22)+40(M=8.33 and U/3=int(U/3))+(5+1^int(M))(U/3=int(U/3
    initSprite(&enemy, 15 * TILE_SIZE, 7 * TILE_SIZE, TILE_SIZE, enemyIndex + 71, type_na);
    char* enemyName;
    {
        char* enemyNames[] = ALL_ENEMIES_ARRAY;
        enemyName = enemyNames[enemyIndex - 1];
    }
    bool doneFlag = false;
    while (!doneFlag)
    {
    bool actFlag = false;
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
                drawText(toString(player->HP, buffer), 26 * TILE_SIZE / 8, 39 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText("HP", 11 * TILE_SIZE + 2 * TILE_SIZE / 8, 39 * TILE_SIZE / 8, 9 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
                drawText(toString(enemyHP, buffer), 14 * TILE_SIZE + 2 * TILE_SIZE / 8, 39 * TILE_SIZE / 8, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
            }
            if (menuLevel == 2)
            {
                char* allAttacks = ALL_ATTACKS;
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

            int exitCode = 0;
            SDL_Event e;
            bool quit = false;
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
                                exitCode = 1;
                                quit = true;
                            break;

                            case SDLK_a:
                                if (menuLevel == 2)
                                    exitCode = 2;
                                quit = true;
                            break;

                            case SDLK_s:
                                exitCode = 3;
                                quit = true;
                            break;

                            case SDLK_d:
                                if (menuLevel == 2)
                                    exitCode = 4;
                                quit = true;
                            break;

                            case SDLK_LSHIFT:
                                exitCode = 5;
                                quit = true;
                            break;

                            break;
                            default:
                            break;
                        }
                    }
                }
            }
            if (exitCode == 1 && menuLevel == 1)
            {
                menuLevel = 2;
                exitCode = 0;
            }
            if (exitCode == 5 && menuLevel == 2)
            {
                menuLevel = 1;
                exitCode = 0;
            }
            if (exitCode == 5 && menuLevel == 1)
            {
                won = false;
                run = true;
            }
            if ((exitCode > 0 && exitCode < 5 && menuLevel == 2) || (exitCode > 0 && exitCode != 1 && menuLevel == 1))
                actFlag = true;
        }
        //proceed with attacks here
        //do the below only when a battle is completed
        doneFlag = true;
    }
    drawTextBox(won ? "You won!" : !run ? "You lost!" : "You fled!", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
    SDL_Delay(1500);

    return won;
}
