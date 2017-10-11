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
#define TILE_ID_SMASH 54
#define TILE_ID_BLOCK 55
#define TILE_ID_ALPHA 70
#define TILE_ID_APEMAN 74
#define TILE_ID_FEENIX 77
#define TILE_ID_TARANT 80
#define TILE_ID_POLARA 83
#define TILE_ID_HYDROA 86
#define TILE_ID_SWURM 89
#define TILE_ID_SENTRY 92
#define TILE_ID_ARCHER 93
#define TILE_ID_SOLDIER 94
#define TILE_ID_DREGOH 95
#define TILE_ID_SWORD 96
#define TILE_ID_TOME 97
#define TILE_ID_ARMOR 98
#define TILE_ID_POTION 99
#define TILE_ID_COIN 100
#define TILE_ID_STONE 102
#define TILE_ID_INNKEEPER 104
#define TILE_ID_NPC 105
#define TILE_ID_BESERKERJ 106
#define TILE_ID_MINER 107
#define TILE_ID_PARKA 108
#define TILE_ID_EARL 109
#define TILE_ID_GENERAL 111

#define ANYWHERE_QUIT -1

#define MENU_NEW_SAVE 1
#define MENU_LOAD_SAVE 2
#define MENU_SETTINGS 3
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

#define ALL_ATTACKS "SLICESLASHBURN ROASTCRACKBREAKCHILLICE  FLOW SWEEPWHACKBASH STAB GASH SMASH     THORNVINE FLAREBLAZEROCK STONEFROSTHAIL STORMVOLT SMELLSTINKDARK EVIL ALPHAARROW"
#define ALL_ENEMIES_ARRAY {"Ant", "Rat", "APEMAN", "Firant", "Pyre", "FEENIX", "Gemdog", "Golem", "TARANT", "Pengin", "Snoman", "POLARA", "C Gull", "C Star", "HYDROA", "Rodent", "Crow", "SWURM", "Midas", "Greed", "SENTRY", "Archer", "Knight", "-DREGOH-"}

#define ARRAY_OF_MAP_IDS {playerSprite->worldNum, 1.1, 1.2, 1.21, 1.22, 1.31, 1.32, 1.33, 2.1, 2.11, 2.12, 2.21, 2.22, 2.23, 2.33, 3.1, 3.2, 3.22, 3.3, 3.31, 3.32, 4.1, 4.11, 4.12, 4.21, 4.22, 4.31, 4.32, 5.1, 5.11, 5.12, 5.2, 5.21, 5.22, 5.32, 6.1, 6.11, 6.2, 6.21, 6.22, 6.31, 6.32, 7.1, 7.11, 7.12, 7.13, 7.2, 7.21, 7.22, 7.23, 7.24, 8.1, 8.11, 8.2, 8.21, 8.22, 8.32, 8.33}
#define SIZE_OF_MAP_ARRAY 58

#define ARRAY_OF_BOSS_IDS {1.33, 2.33, 3.22, 4.31, 5.32, 6.31, 7.13, 7.23, 8.33}
#define SIZE_OF_BOSS_ARRAY 9

#define ARRAY_OF_BOSS_XS {14, 11, 7, 10, 5, 12, 14, 7, 11}
#define ARRAY_OF_BOSS_YS {7, 7, 8, 6, 5, 7, 7, 3, 7}
#define ARRAY_OF_BOSS_TILEIDS {TILE_ID_APEMAN, TILE_ID_FEENIX, TILE_ID_TARANT, TILE_ID_POLARA, TILE_ID_HYDROA, TILE_ID_SWURM, TILE_ID_SENTRY, TILE_ID_DREGOH, TILE_ID_DREGOH}
#define SIZE_OF_BOSSDATA_ARRAY 9

#define ARRAY_OF_UPGRADER_IDS {1.22, 3.1, 4.22, 5.1, 6.32, 7.1, 8.1}
#define ARRAY_OF_UPGRADER_OVERWORLDXS {playerSprite->overworldX / TILE_SIZE, 13, playerSprite->overworldX / TILE_SIZE, 10, playerSprite->overworldX / TILE_SIZE, 13, 11}
#define ARRAY_OF_UPGRADER_OVERWORLDYS {playerSprite->overworldY / TILE_SIZE, 6, playerSprite->overworldY / TILE_SIZE, 11, playerSprite->overworldY / TILE_SIZE, 6, 6}
#define ALL_UPGRADER_MOVES_ARRAY {"SLICE or THORN. ", "BURN or FLARE. ", "CRACK or ROCK. ", "CHILL or FROST. ", "FLOW or STORM. ", "WHACK or SMELL. ", "STAB or DARK."}
#define SIZE_OF_UPGRADER_ARRAY 7

#define ALL_NPC_TEXT_ARRAY {"BLOCK REDUCES YOUR DMG A BIT, BUT HALVES ENEMY DMG.", "A CRITICAL IS SHOWN WITH RED, AND A WEAKNESS WITH YELLOW!", "I SAW AN APEMAN DEEP IN THE WOODS.", "I FOUND THE APEMAN FIRST. HE'S MINE! BUT HE'S ANGRY.", "THESE CREATURES ARE WEAK TO WATER!", "SOME SAY THESE MONSTERS FIGHT US BECAUSE THEY SERVE THE DRAGONS KING.", "IF YOU GROW SOME WEEDS, THEY'LL BREAK THROUGH THE MONSTERS.", "MAN, IF I HAD A FIRE, I COULD MELT THE SNOW HERE!", "I KNEW I SHOULD'VE MOVED TO THE EAST POLE. THEY HAVE NO MONSTERS.", "WE SHOULD PUT SOME PLANTS AROUND HERE TO HOLD THE SAND IN PLACE.", "THE MONSTER OF THE SEA HAS TURNED THE FISH AGAINST US! BUT WHO WOKE HIM?", "THIS IS MY MANSION. SINCE IT'S BY THE WATER, IT WAS VERY EXPENSIVE. BUT NOW IF I GO OUTSIDE, I'LL GET HURT.", "WE HAVE TO LIVE HERE. IT'S BECAUSE OF THAT DARN WORM! HE SAYS HE SERVES THE GREEDY DRAGONS KING.", "MAYBE IF WE COULD GET RUNNING WATER, WE COULD SCARE THESE DIRTY MONSTERS.", "THE GENERAL HAS A MESSAGE: EITHER FIGHT AND THE KING WILL KILL YOUR PEOPLE, or JOIN HIM IN HIS ARMORY.", "HERE MONSTERS DON'T ATTACK US. ONLY OUR THOUGHTS OF GREED DO. THEY HATE DIRTY ATTACKS.", "THIS IS A REALLY NICE PLACE TO LIVE. EXCEPT FOR THE STRICT LAWS.", "AAH! WE'RE UNDER ATTACK! THE DRAGONS KING IS HERE! HELP US, "}
#define SIZE_OF_NPC_TEXT_ARRAY 17

#define ARRAY_OF_NPC_WORLDNUMS {1, 1, 1, 1, 2, 2, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8}
#define ARRAY_OF_NPC_LASTSCREENS {10, 10, 10, 32, 10, 10, 10, 10, 10, 10, 10, 21, 10, 10, 12, 10, 10, 10}
#define ARRAY_OF_NPC_OVERWORLDXS {6, 12, 16, playerSprite->overworldX / TILE_SIZE, 12, 12, 8, 14, 15, 11, 17, playerSprite->overworldX / TILE_SIZE, 11, 6, playerSprite->overworldX / TILE_SIZE, 5, 13, 15}
#define ARRAY_OF_NPC_OVERWORLDYS {9, 3, 7, playerSprite->overworldY / TILE_SIZE, 6, 10, 5, 6, 3, 6, 6, playerSprite->overworldY / TILE_SIZE, 7, 10, playerSprite->overworldY / TILE_SIZE, 11, 11, 7}
#define SIZE_OF_NPCDATA_ARRAY 18

#define ALL_BOSS_QUIP_ARRAY {"LEAF ME ALONE! I WOOD-N'T DO THAT!", "I'M ON FIRE TODAY! DON'T BE SO HOT-HEADED!", "DON'T TRY TO BE STONEFACED. I KNOW YOU WANT TO RUN!", "I'M A COOL, COLD-HEARTED CREATURE. ICY YOUR FEAR!" , "GO WITH THE FLOW DUDE. YOU ARE IN THE WAKE OF RUIN!", "WRIGGLE ALL YOU WANT, YOU CAN'T SQUISH ME!", "So this is your choice? PREPARE TO BE CRUSHED!", "Here, I will crush you and your people!"}
#define SIZE_OF_BOSS_QUIP_ARRAY 8

#define ARRAY_OF_CHEST_IDS {1.2, 1.21, 2.11, 2.23, 3.2, 3.32, 4.11, 4.12, 5.1, 5.2, 5.12, 6.2, 6.22, 6.32, 7.1, 7.12, 7.2, 7.21, 7.24, 8.11, 8.2, 8.32}
#define SIZE_OF_CHEST_ARRAY SIZE_OF_CHESTID_ARRAY

#define ARRAY_OF_CHEST_XS {9, 17, 16, 8, 9, 3, 13, 3, 14, 10, 6, 2, 5, 1, 5, 6, 4, 16, 5, 18, 3, 3}
#define ARRAY_OF_CHEST_YS {7, 11, 11, 11, 11, 11, 2, 5, 2, 8, 9, 5, 4, 12, 4, 5, 10, 5, 7, 7, 10, 10}
#define ARRAY_OF_CHEST_ITEMS {991, 981, 971, 961, 972, 992, 973, 962, 963, 974, 964, 993, 975, 965, 976, 994, 966, 968, 994, 978, 977, 978}
#define SIZE_OF_CHESTDATA_ARRAY SIZE_OF_CHESTID_ARRAY

#define ARRAY_OF_SWORD_NAMES {"FLAME SWORD", "ROCK SWORD", "CHILL SWORD", "WATER SWORD", "DUAL KNIFE", "GOLD SWORD", "SMASH SWORD", "MAGIC SWORD"}
#define SIZE_OF_SWORD_ARRAY 8
#define ARRAY_OF_TOME_NAMES {"WOOD TOME", "BURNT TOME", "STONE TOME", "COLD TOME", "STORM TOME", "SMELLY TOME", "DARK TOME", "ALPHA TOME", "POWER TOME"}
#define SIZE_OF_TOME_ARRAY 9
#define ARRAY_OF_ARMOR_NAMES {"WEAK ARMOR", "BASIC ARMOR", "GOOD ARMOR", "GREAT ARMOR", "FINE ARMOR", "THICK ARMOR", "HEAVY ARMOR", "METAL ARMOR", "SOLID ARMOR"}
#define SIZE_OF_ARMOR_ARRAY 9
#define ARRAY_OF_POTION_NAMES {"FLAT POTION", "FIZZ POTION", "GOOD POTION", "FULL POTION"}
#define SIZE_OF_POTION_ARRAY 4
#define ARRAY_OF_STONE_NAMES {"LAVA STONE", "STONE STONE", "ICE STONE", "WATER STONE", "BRICK STONE", "GOLD STONE", "MUD STONE"}
#define SIZE_OF_STONE_ARRAY 7

#define BATTLE_ACT_CODE_UP 1
#define BATTLE_ACT_CODE_LEFT 2
#define BATTLE_ACT_CODE_DOWN 3
#define BATTLE_ACT_CODE_RIGHT 4
#define BATTLE_ACT_CODE_MENU 5
#define ATTACK_CODE_RUN TILE_ID_RUN
#define ATTACK_CODE_BLOCK TILE_ID_BLOCK

#define UPGRADE_COST 15

//Todo for 10/2:
//* Make world-sized tilemaps work (and smooth scrolling)?
//** Create world-sized tilemaps by stitching together the individual CSE map pngs, throw them in the xLIBC map generator, done
//** Make them work by doing nice scroll animations between map borders
//** Make sure you only render screen-sized chunks at a time

int main(int argc, char* argv[])
{
	player player;
	int succeeded = init();
	if (!succeeded)
    {
        int selection = 0;
        while(selection != MENU_QUIT && selection != ANYWHERE_QUIT)
        {
            SDL_RenderClear(mainRenderer);
            do
            {
                selection = aMenu("Sorcery of Uvutu", "NEW GAME", "LOAD GAME", "SETTINGS", "QUIT", " ", 4, selection, MAIN_MENU_PALETTE, false, true);
                if (selection == MENU_SETTINGS)
                    selection = showSettings();
            }
            while (selection == MENU_SETTINGS);
            if (selection < MENU_SETTINGS && selection != ANYWHERE_QUIT)
            {
                succeeded = startGame(&player, selection == MENU_NEW_SAVE);
                int loopCode = LOOP_GOTO_MENU;
                while (loopCode > 0)
                {
                    loopCode = mainLoop(&player);
                    if (loopCode == LOOP_GOTO_BATTLE || loopCode == LOOP_GOTO_BATTLE_BOSS)
                        doBattle(&player, loopCode == LOOP_GOTO_BATTLE_BOSS);
                    if (loopCode == ANYWHERE_QUIT)
                        selection = MENU_QUIT;
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
                            if (loopCode == OVERWORLD_QUIT)
                                loopCode = LOOP_QUIT;
                            if (loopCode == ANYWHERE_QUIT)
                                selection = loopCode;
                        }
                    }
                }
                //printf("%s ended at %d, %d underneath a tile of index %d in map id %f\n", player.name, player.spr.x / player.spr.w, player.spr.y / player.spr.w, tilemap[player.spr.y / TILE_SIZE][player.spr.x / TILE_SIZE], player.worldNum + (player.mapScreen / 100.0));
                savePlayerData(&player, SAVE_FILE_NAME);
                saveConfig(CONFIG_FILE_NAME);
            }
        }
            closeSDL();
    }
	return succeeded;
}

int showSettings()
{
    int selection = 1;
    while(selection != 3 && selection != -1)
    {
        selection = aMenu("SETTINGS", "CONFIGURE KEYS", "HELP", "BACK", " ", " ", 3, 1, MAIN_MENU_PALETTE, false, false);
        if (selection == 2)
            aWallOfText("Sorcery of Uvutu", "Default keys are ^<V> to move, ESC to enter menu, Spacebar to talk to people & in the Menu to select an option. Critical Hits are marked by a RED X, and Weakness by a YELLOW +. See the README for more.", true, false);
        if (selection == 1)
            configureKeys();
        if (selection > 0)
            selection = MENU_SETTINGS;
    }
    return selection;
}

void configureKeys()
{
    sprite cursor;
    SDL_Color textColor = (SDL_Color){24, 195, 247};
    SDL_Color bgColor = (SDL_Color){16, 32, 140};
    initSprite(&cursor, TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_ID_CURSOR, (entityType) type_na);
    int selection = -1;
    while(selection != 7)
    {
        SDL_SetRenderDrawColor(mainRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
        SDL_RenderFillRect(mainRenderer, NULL);
        //foreground text
        drawText("CONFIGURE KEYS", 2 * TILE_SIZE + TILE_SIZE / 4, 5 * SCREEN_HEIGHT / 64, SCREEN_WIDTH, 55 * SCREEN_HEIGHT / 64, textColor, false);

        char keyText[99];
        strcpy(keyText, "UP: ");
        drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_UP))), 2 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 5) * TILE_SIZE, textColor, false);
        //we don't use SDL_GetScancodeName() because the SDL documentation warns that it isn't a stable way to get key names

        strcpy(keyText, "DOWN: ");
        drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_DOWN))), 2 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 6) * TILE_SIZE, textColor, false);

        strcpy(keyText, "LEFT: ");
        drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_LEFT))), 2 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 7) * TILE_SIZE, textColor, false);

        strcpy(keyText, "RIGHT: ");
        drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_RIGHT))), 2 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 8) * TILE_SIZE, textColor, false);

        strcpy(keyText, "CONFIRM: ");
        drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_INTERACT))), 2 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 9) * TILE_SIZE, textColor, false);

        strcpy(keyText, "MENU: ");
        drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_MENU))), 2 * TILE_SIZE + TILE_SIZE / 4, 10 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 10) * TILE_SIZE, textColor, false);
        drawText("BACK", 2 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 11) * TILE_SIZE, textColor, true);
        drawText("Default is W/S/A/D, Space, Esc", 3 * TILE_SIZE / 4, 13 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 13) * TILE_SIZE, textColor, true);
        SDL_Event e;
        bool quit = false;
        //While application is running
        while(!quit)
        {
            SDL_RenderClear(mainRenderer);
            //SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = cursor.x, .y = cursor.y, .w = cursor.w, .h = cursor.w}));
            //Handle events on queue
            while(SDL_PollEvent(&e) != 0)
            {
                //User requests quit
                if(e.type == SDL_QUIT)
                {
                    quit = true;
                    selection = 7;
                }
                //User presses a key
                else if(e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_UP))
                    {
                        if (cursor.y > 5 * TILE_SIZE)
                            cursor.y -= TILE_SIZE;
                    }

                    if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_DOWN))
                    {
                        if (cursor.y < 11 * TILE_SIZE)
                            cursor.y += TILE_SIZE;
                    }

                    if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_INTERACT))
                    {
                        selection = cursor.y / TILE_SIZE - 4;
                        quit = true;
                    }
                    SDL_RenderClear(mainRenderer);
                    drawText("CONFIGURE KEYS", 2 * TILE_SIZE + TILE_SIZE / 4, 5 * SCREEN_HEIGHT / 64, SCREEN_WIDTH, 55 * SCREEN_HEIGHT / 64, textColor, false);

                    char keyText[99];
                    strcpy(keyText, "UP: ");
                    drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_UP))), 2 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 5) * TILE_SIZE, textColor, false);
                    //we don't use SDL_GetScancodeName() because the SDL documentation warns that it isn't a stable way to get key names

                    strcpy(keyText, "DOWN: ");
                    drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_DOWN))), 2 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 6) * TILE_SIZE, textColor, false);

                    strcpy(keyText, "LEFT: ");
                    drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_LEFT))), 2 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 7) * TILE_SIZE, textColor, false);

                    strcpy(keyText, "RIGHT: ");
                    drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_RIGHT))), 2 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 8) * TILE_SIZE, textColor, false);

                    strcpy(keyText, "CONFIRM: ");
                    drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_INTERACT))), 2 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 9) * TILE_SIZE, textColor, false);

                    strcpy(keyText, "MENU: ");
                    drawText(strcat(keyText, SDL_GetKeyName(SDL_GetKeyFromScancode(SC_MENU))), 2 * TILE_SIZE + TILE_SIZE / 4, 10 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 10) * TILE_SIZE, textColor, false);
                    drawText("BACK", 2 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 11) * TILE_SIZE, textColor, true);
                    drawText("Default is W/S/A/D, Space, Esc", 3 * TILE_SIZE / 4, 13 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 13) * TILE_SIZE, textColor, true);
                }
            }
            drawTile(cursor.tileIndex, cursor.x, cursor.y, TILE_SIZE, SDL_FLIP_NONE);
            SDL_RenderPresent(mainRenderer);
        }
        if (selection != 7)
        {
            char* keyName[6] = {"UP", "DOWN", "LEFT", "RIGHT", "CONFIRM", "MENU"};
            char titleText[] = "PRESS A KEY FOR ";
            strcat(titleText, keyName[selection - 1]);
            getNewKey(titleText, bgColor, textColor, selection - 1);
        }
        else
            saveConfig(CONFIG_FILE_NAME);
    }
}

void getNewKey(char* titleText, SDL_Color bgColor, SDL_Color textColor, int selection)
{
    SDL_SetRenderDrawColor(mainRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
    SDL_RenderFillRect(mainRenderer, NULL);
    drawText(titleText, .5 * TILE_SIZE, 5 * SCREEN_HEIGHT / 64, (WIDTH_IN_TILES - .5) * TILE_SIZE, (HEIGHT_IN_TILES - 4) * TILE_SIZE, textColor, true);
    SDL_Keycode kc = waitForKey();
    bool conflict= false;
    for(int i = 0; i < SIZE_OF_SCANCODE_ARRAY; i++)
    {
        if (CUSTOM_SCANCODES[i] == SDL_GetScancodeFromKey(kc))
            conflict = true;
    }
    if (!conflict)
        CUSTOM_SCANCODES[selection] = SDL_GetScancodeFromKey(kc);
}

void drawGame(player* player, char* textInput)
{
    drawTile(player->spr.tileIndex, player->spr.x, player->spr.y, player->spr.w, player->flip);
    if (textBoxOn)
        drawTextBox(textInput, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 8.5 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 8.5) * TILE_SIZE});
    //SDL_RenderPresent(mainRenderer);
}

int mainLoop(player* playerSprite)
{
    bool quit = false, press = false, upgradeMoves = false;
    SDL_Event e;
    int frame = 0, exitCode = LOOP_QUIT;
    static int pickedUp = 1;
    char* textInput = "Did you know that this is placeholder text? Me neither.";
    {
        double arrayOfMaps[SIZE_OF_MAP_ARRAY] = ARRAY_OF_MAP_IDS;
        int map = checkArrayForDVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 100.0), arrayOfMaps, SIZE_OF_MAP_ARRAY);
        loadMapFile(MAP_FILE_NAME, tilemap, map, HEIGHT_IN_TILES, WIDTH_IN_TILES);
    }
    //on Linux, seg fault after calling drawTile/map
    drawTilemap(0, 0, WIDTH_IN_TILES, HEIGHT_IN_TILES, false);
    drawHUD(playerSprite);
    sprite entity;
    entityType type = type_na;
    int x = -TILE_SIZE, y = -TILE_SIZE, index = 127, found = -1, textLocation = 0, foundPhys = -1, foundMag = -1;
    bool drawEntityFlag = true, pickFromLocation = true;
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
        double arrayOfMaps[SIZE_OF_UPGRADER_ARRAY] = ARRAY_OF_UPGRADER_IDS;
        int arrayOfXs[SIZE_OF_UPGRADER_ARRAY] = ARRAY_OF_UPGRADER_OVERWORLDXS;
        int arrayOfYs[SIZE_OF_UPGRADER_ARRAY] = ARRAY_OF_UPGRADER_OVERWORLDYS;
        int isUpgraderMap = checkArrayForDVal(playerSprite->worldNum + (double)(playerSprite->lastScreen / 100.0), arrayOfMaps, SIZE_OF_UPGRADER_ARRAY);
        int isUpgraderX = playerSprite->overworldX / TILE_SIZE == arrayOfXs[isUpgraderMap];
        int isUpgraderY = playerSprite->overworldY / TILE_SIZE == arrayOfYs[isUpgraderMap];
        //printf("isUpgraderMap = %d, isUpgraderX = %d, isUpgraderY = %d\n", isUpgraderMap, isUpgraderX, isUpgraderY);
        if (isUpgraderMap != -1 && isUpgraderX && isUpgraderY)
        {
            index = TILE_ID_BESERKERJ;
            int moveArray[4] = {playerSprite->move1, playerSprite->move2, playerSprite->move3, playerSprite->move4};
            char* nameArray[SIZE_OF_UPGRADER_ARRAY] = ALL_UPGRADER_MOVES_ARRAY;
            for(int i = 0; i < 4; i++)
            {
                if (moveArray[i] == 38 + 2 * (playerSprite->worldNum - (playerSprite->worldNum > 2)))
                    foundPhys = i;
                if (moveArray[i] == 54 + 2 * (playerSprite->worldNum - (playerSprite->worldNum > 2)))
                    foundMag = i;
            }
            textInput = "PRESS 2nd TO UPGRADE A MOVE. PLEASE PROGRAM BETTER DIALOGUE FOR ME.";
            char temp[99] = "";
            char* moves = nameArray[playerSprite->worldNum - (playerSprite->worldNum > 2) - 1];
            if ((foundPhys > -1 || foundMag > -1) && playerSprite->money > UPGRADE_COST - 1)
            {
                strcpy(temp, "FOR 15 COINS EACH I CAN UPGRADE YOUR ");
                strcat(temp, moves);
                strcat(temp, "Confirm - YES.     Any other key - NO\0");
                upgradeMoves = true;
            }
            else
            {
                strcpy(temp, "YOU NEED 15 COINS TO UPGRADE YOUR ");
                strcat(temp, moves);
                strcat(temp, "\0");
            }
            if (foundPhys == -1 && foundMag == -1)
            {
                strcpy(temp, "YOU DON'T SEEM TO BE ABLE TO USE ");
                strcat(temp, moves);
                strcat(temp, "\0");
            }
            if (strlen(temp))
                textInput = temp;
            pickFromLocation = false;
        }
        if (playerSprite->overworldX == 192 && playerSprite->overworldY == 240)
        {
            index = TILE_ID_INNKEEPER;
            textInput = "ARE YOU TIRED? REST HERE.       (HEALED)";
            pickFromLocation = false;
        }
        if (pickFromLocation)
        {
            int npcWorldNums[SIZE_OF_NPCDATA_ARRAY] = ARRAY_OF_NPC_WORLDNUMS;
            int npcLastScreens[SIZE_OF_NPCDATA_ARRAY] = ARRAY_OF_NPC_LASTSCREENS;
            int npcOverworldXs[SIZE_OF_NPCDATA_ARRAY] = ARRAY_OF_NPC_OVERWORLDXS;
            int npcOverworldYs[SIZE_OF_NPCDATA_ARRAY] = ARRAY_OF_NPC_OVERWORLDYS;
            for(int i = 0; i < SIZE_OF_NPCDATA_ARRAY; i++)
            {
                if (playerSprite->worldNum == npcWorldNums[i] &&
                    playerSprite->lastScreen == npcLastScreens[i] &&
                    playerSprite->overworldX / TILE_SIZE == npcOverworldXs[i] &&
                    playerSprite->overworldY / TILE_SIZE == npcOverworldYs[i])
                {
                    textLocation = i;
                    break;
                }
            }
            //all NPC text
            char* stringArray[] = ALL_NPC_TEXT_ARRAY;
            textInput = stringArray[textLocation];

            if (textLocation == SIZE_OF_NPCDATA_ARRAY - 1)
            {
                char tempString[35] = "";
                strcpy(tempString, textInput);
                strcat(tempString, playerSprite->name);
                strcat(tempString, "!");
                textInput = tempString;
            }
        }
    }
    else
    {
        {
            //printf("finding if chest exists: ");
            double arrayOfMaps[SIZE_OF_CHEST_ARRAY] = ARRAY_OF_CHEST_IDS;
            found = checkArrayForDVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 100.0), arrayOfMaps, SIZE_OF_CHEST_ARRAY);
            //printf("found\n");
        }
        if (found != -1)
        {
            //printf("finding chest: ");
            type = type_chest;
            index = TILE_ID_CHEST;
            int chestX[SIZE_OF_CHESTDATA_ARRAY] = ARRAY_OF_CHEST_XS;
            int chestY[SIZE_OF_CHESTDATA_ARRAY] = ARRAY_OF_CHEST_YS;
            x *= -1 * chestX[found];
            y *= -1 * chestY[found];
            //printf("found\n");
            if (playerSprite->pickedUpChests[found])
                drawEntityFlag = false;
        }
        else
        {
            {
                //printf("finding if boss exists: ");
                double arrayOfMaps[SIZE_OF_BOSS_ARRAY] = ARRAY_OF_BOSS_IDS;
                found = checkArrayForDVal(playerSprite->worldNum + (double)(playerSprite->mapScreen / 100.0), arrayOfMaps, SIZE_OF_BOSS_ARRAY);
                //printf("found\n");
            }
            if (found != -1 && ((playerSprite->beatenBosses / 10 < playerSprite->worldNum && found != 7) || (playerSprite->beatenBosses % 10 == 0 && found == 7)))
            {
                //printf("finding boss ID %d: ", found);
                type = type_boss;
                int bossX[SIZE_OF_BOSSDATA_ARRAY] = ARRAY_OF_BOSS_XS;
                int bossY[SIZE_OF_BOSSDATA_ARRAY] = ARRAY_OF_BOSS_YS;
                int bossIndex[SIZE_OF_BOSSDATA_ARRAY] = ARRAY_OF_BOSS_TILEIDS;
                if ((found == 7 && playerSprite->beatenBosses % 10 == 0) || found != 7)
                {
                    index = bossIndex[found];
                    x *= -1 * bossX[found];
                    y *= -1 * bossY[found];
                }
                //printf("found\n");
            }
        }
    }
    //printf("loaded sprite\n");
    initSprite(&entity, x, y, TILE_SIZE, index, type);
    drawEntity(entity, drawEntityFlag, true, false);
    drawTile(playerSprite->spr.tileIndex, playerSprite->spr.x, playerSprite->spr.y, TILE_SIZE, playerSprite->flip);
    SDL_RenderPresent(mainRenderer);
    SDL_Delay(220);
    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0xFF);
    while(!quit)    //this is the literal main loop right here
    {
        SDL_RenderClear(mainRenderer);
        drawTilemap(0, 0, 20, 15, false);
        drawGame(playerSprite, textInput);
        drawEntity(entity, drawEntityFlag, true, false);
        drawHUD(playerSprite);
        SDL_RenderPresent(mainRenderer);
        while(SDL_PollEvent(&e) != 0)  //while there are events in the queue
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                exitCode = ANYWHERE_QUIT;
            }
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
        if (press == KEYPRESS_RETURN_TEXTACTION && entity.tileIndex == TILE_ID_BESERKERJ && playerSprite->money > UPGRADE_COST - 1 && upgradeMoves)
        {
            int movesToUpgrade = 0;
            //figure out a way to only upgrade one if they have only enough for one upgrade
            if (foundPhys == 0 || foundMag == 0)
            {
                playerSprite->move1++;
                movesToUpgrade++;
            }
            if ((foundPhys == 1 || foundMag == 1) && playerSprite->money > (UPGRADE_COST * (1 + movesToUpgrade)) - 1)
            {
                playerSprite->move2++;
                movesToUpgrade++;
            }
            if ((foundPhys == 2 || foundMag == 2) && playerSprite->money > (UPGRADE_COST * (1 + movesToUpgrade)) - 1)
            {
                playerSprite->move3++;
                movesToUpgrade++;
            }
            if ((foundPhys == 3 || foundMag == 3) && playerSprite->money > (UPGRADE_COST * (1 + movesToUpgrade)) - 1)
            {
                playerSprite->move4++;
                movesToUpgrade++;
            }
            playerSprite->money -= UPGRADE_COST * movesToUpgrade;
            press = 0;
            upgradeMoves = false;
            //refactoring move upgrader text
            foundPhys = -1, foundMag = -1;
            int moveArray[4] = {playerSprite->move1, playerSprite->move2, playerSprite->move3, playerSprite->move4};
            char* nameArray[SIZE_OF_UPGRADER_ARRAY] = ALL_UPGRADER_MOVES_ARRAY;
            for(int i = 0; i < 4; i++)
            {
                if (moveArray[i] == 38 + 2 * (playerSprite->worldNum - (playerSprite->worldNum > 2)))
                    foundPhys = i;
                if (moveArray[i] == 54 + 2 * (playerSprite->worldNum - (playerSprite->worldNum > 2)))
                    foundMag = i;
            }
            textInput = "PRESS 2nd TO UPGRADE A MOVE. PLEASE PROGRAM BETTER DIALOGUE FOR ME.";
            char temp[99] = "";
            char* moves = nameArray[playerSprite->worldNum - (playerSprite->worldNum > 2) - 1];
            if ((foundPhys > -1 || foundMag > -1) && playerSprite->money > UPGRADE_COST - 1)
            {
                strcpy(temp, "FOR 15 COINS EACH I CAN UPGRADE YOUR ");
                strcat(temp, moves);
                strcat(temp, "Confirm - YES.     Any other key - NO\0");
            }
            else
            {
                strcpy(temp, "YOU NEED 15 COINS TO UPGRADE YOUR ");
                strcat(temp, moves);
                strcat(temp, "\0");
            }
            if (foundPhys == -1 && foundMag == -1)
            {
                strcpy(temp, "YOU DON'T SEEM TO BE ABLE TO USE ");
                strcat(temp, moves);
                strcat(temp, "\0");
            }
            if (strlen(temp))
                textInput = temp;
        }

        if (playerSprite->spr.x == entity.x && playerSprite->spr.y == entity.y)
        {
            if (entity.type == type_boss)
            {
                drawEntity(entity, false, true, false);
                exitCode = LOOP_GOTO_BATTLE_BOSS;
                quit = true;
            }
            if (entity.type == type_chest)
            {
                int itemArray[SIZE_OF_CHESTDATA_ARRAY] = ARRAY_OF_CHEST_ITEMS;
                if (!playerSprite->pickedUpChests[found] && pickedUp)
                {
                    pickedUp = pickupItem(playerSprite, itemArray[found], found, true);
                    drawEntityFlag = false;
                    drawEntity(entity, drawEntityFlag, true, false);
                    //pick up chest here
                }
            }
        }

        frame += press;

        if (frame % 7 == 0)
        {
            frame = 0;
            press = false;
        }
        SDL_Delay(1000 / FRAMERATE);
    }
    return exitCode;
}

void drawEntity(sprite entity, bool doDraw, bool saveEntity, bool loadEntity)
{
    static sprite entitySaved;
    static bool drawIt;
    if (saveEntity)
    {
        initSprite(&entitySaved, entity.x, entity.y, TILE_SIZE, entity.tileIndex, entity.type);
        drawIt = doDraw;
    }
    if (saveEntity || loadEntity)
    {
        if (drawIt)
            drawTile(entitySaved.tileIndex, entitySaved.x, entitySaved.y, TILE_SIZE, SDL_FLIP_NONE);
    }
    else
    {
        if (doDraw)
            drawTile(entity.tileIndex, entity.x, entity.y, TILE_SIZE, SDL_FLIP_NONE);
    }
}

void drawHUD(player* player)
{
    drawTilemap(0, 0, 19, 1, false);
    char* buffer = "";
    char* worldNames[8] = {"Plain Plains", "Dragon's Den", "Worry Quarry", "West Pole", "River Lake", "Under City", "Upper City", "Battleground"};
    drawText(worldNames[player->worldNum - 1], 0, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    buffer = toString(player->HP, buffer);
    drawText(buffer, (16 - strlen(buffer)) * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((10 * player->HP / player->maxHP) > 3 ? ((SDL_Color) {231, 223, 49}) : ((SDL_Color) {255, 0, 0})), false);
    drawText("/", 16 * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {255, 255, 255}), false);
    drawText(toString(player->maxHP, buffer), 17 * TILE_SIZE, 0, SCREEN_WIDTH, TILE_SIZE, ((SDL_Color) {231, 223, 49}), false);
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

int aMenu(char* title, char* opt1, char* opt2, char* opt3, char* opt4, char* opt5, const int options, int curSelect, SDL_Color bgColor, SDL_Color titleColorUnder, SDL_Color titleColorOver, SDL_Color textColor, bool border, bool isMain)
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
    drawText(opt5, 2 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 9) * TILE_SIZE, textColor, !isMain);

    if (isMain)
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
        //SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = cursor.x, .y = cursor.y, .w = cursor.w, .h = cursor.w}));
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
                const Uint8* keyStates = SDL_GetKeyboardState(NULL);
                if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_UP))
                {
                    if (cursor.y > 5 * TILE_SIZE)
                        cursor.y -= TILE_SIZE;
                }

                if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_DOWN))
                {
                    if (cursor.y < (options + 4) * TILE_SIZE)
                        cursor.y += TILE_SIZE;
                }

                if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_INTERACT))
                {
                    selection = cursor.y / TILE_SIZE - 4;
                    quit = true;
                }
                SDL_RenderClear(mainRenderer);
                if (isMain && (keyStates[SDL_SCANCODE_LCTRL] || keyStates[SDL_SCANCODE_RCTRL]) && keyStates[SDL_SCANCODE_R])
                {
                    SC_UP = SDL_SCANCODE_W;
                    SC_DOWN = SDL_SCANCODE_S;
                    SC_LEFT = SDL_SCANCODE_A;
                    SC_RIGHT = SDL_SCANCODE_D;
                    SC_INTERACT = SDL_SCANCODE_SPACE;
                    SC_MENU = SDL_SCANCODE_ESCAPE;
                    saveConfig(CONFIG_FILE_NAME);
                }
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
                drawText(opt5, 2 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 9) * TILE_SIZE, textColor, !isMain);

                if (isMain)
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
            }
        }
        drawTile(cursor.tileIndex, cursor.x, cursor.y, TILE_SIZE, SDL_FLIP_NONE);
        SDL_RenderPresent(mainRenderer);
    }
    return selection;
}

int aWallOfText(char* title, char* text, bool showHelpInfo, bool fadeIn)
{
    if (fadeIn)
        SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
    for(int i = 255; i > (fadeIn ? 0 : 254); i--)
    {
        SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
        SDL_RenderFillRect(mainRenderer, NULL);
        //background text (drawn first)
        drawText(title, 21 * SCREEN_WIDTH / 128, 1 * SCREEN_HEIGHT / 128, SCREEN_WIDTH, 127 * SCREEN_HEIGHT / 128, (SDL_Color){24, 65, 214}, false);
        //foreground text
        drawText(title, 10 * SCREEN_WIDTH / 64, 0, 54 * SCREEN_WIDTH / 64, SCREEN_HEIGHT, (SDL_Color){24, 162, 239}, false);

        drawText(text, 0, TILE_SIZE + TILE_SIZE / 2, SCREEN_WIDTH, (HEIGHT_IN_TILES - 2) * TILE_SIZE, (SDL_Color){24, 195, 247}, false);
        if (showHelpInfo)
            drawText("By Stephen Policelli", 0, 13 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){24, 195, 247}, false);
        if (fadeIn)
        {
            SDL_SetRenderDrawColor(mainRenderer, 0x00, 0x00, 0x00, (Uint8) i);
            SDL_RenderFillRect(mainRenderer, NULL);
            SDL_Delay(2);
        }
        SDL_RenderPresent(mainRenderer);
    }
    if (fadeIn)
        SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_NONE);
    waitForKey();
    return MENU_SETTINGS;
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
    drawText("^:", 3 * TILE_SIZE +  TILE_SIZE / 4, 9 * TILE_SIZE, (WIDTH_IN_TILES - 3) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 9) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText("<:", 3 * TILE_SIZE +  TILE_SIZE / 4, 10 * TILE_SIZE, (WIDTH_IN_TILES - 3) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 10) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText("V:", 3 * TILE_SIZE +  TILE_SIZE / 4, 11 * TILE_SIZE, (WIDTH_IN_TILES - 3) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 11) * TILE_SIZE, (SDL_Color){0, 0, 0}, false);
    drawText(">:", 3 * TILE_SIZE +  TILE_SIZE / 4, 12 * TILE_SIZE, (WIDTH_IN_TILES - 3) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 12) * TILE_SIZE, (SDL_Color){0, 0, 0}, true);

    if (waitForKey() == KCMenu)
        exitCode = ANYWHERE_QUIT;
    return exitCode;
}

int showItems(player* player)
{
    char* buffer = "";
    sprite cursor;
    initSprite(&cursor, TILE_SIZE, 1 * TILE_SIZE, TILE_SIZE, TILE_ID_CURSOR, (entityType) type_na);
    int exitCode = OVERWORLD_ITEMS;
    bool quit = false;
    while (!quit)
    {
        SDL_RenderClear(mainRenderer);
        SDL_SetRenderDrawColor(mainRenderer, 0x10, 0x20, 0x8C, 0xFF);
        SDL_RenderFillRect(mainRenderer, NULL);
        SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = 1 * SCREEN_HEIGHT / 128, .w = 126 * SCREEN_WIDTH / 128, .h = 126 * SCREEN_HEIGHT / 128}));
        drawText("BACK", 2 * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 1) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
        for(int i = 0; i < PLAYER_ITEMS_LIMIT; i++)
        {
            if (player->items[i] > 0)
            {
                char* itemName = "";
                {
                    if (player->items[i] / 10 == TILE_ID_POTION)
                    {
                        char* arrayOfItems[SIZE_OF_POTION_ARRAY] = ARRAY_OF_POTION_NAMES;
                        int nameIndex = player->items[i] - (10 * iPart((player->items[i] / 10.0)));
                        itemName = arrayOfItems[nameIndex - 1];
                    }
                    if (player->items[i] / 10 == TILE_ID_SWORD)
                    {
                        char* arrayOfItems[SIZE_OF_SWORD_ARRAY] = ARRAY_OF_SWORD_NAMES;
                        int nameIndex = player->items[i] - (10 * iPart((player->items[i] / 10.0)));
                        itemName = arrayOfItems[nameIndex - 1];
                    }
                    if (player->items[i] / 10 == TILE_ID_TOME)
                    {
                        char* arrayOfItems[SIZE_OF_TOME_ARRAY] = ARRAY_OF_TOME_NAMES;
                        int nameIndex = player->items[i] - (10 * iPart((player->items[i] / 10.0)));
                        itemName = arrayOfItems[nameIndex - 1];
                    }
                    if (player->items[i] / 10 == TILE_ID_ARMOR)
                    {
                        char* arrayOfItems[SIZE_OF_ARMOR_ARRAY] = ARRAY_OF_ARMOR_NAMES;
                        int nameIndex = player->items[i] - (10 * iPart((player->items[i] / 10.0)));
                        itemName = arrayOfItems[nameIndex - 1];
                    }
                    if (player->items[i] / 10 == TILE_ID_STONE)
                    {
                        char* arrayOfItems[SIZE_OF_STONE_ARRAY] = ARRAY_OF_STONE_NAMES;
                        int nameIndex = player->items[i] - (10 * iPart((player->items[i] / 10.0)));
                        itemName = arrayOfItems[nameIndex - 1];
                    }
                }
                drawTile(player->items[i] / 10, 2 * TILE_SIZE, (2 + i) * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                drawText(itemName, 3 * TILE_SIZE + TILE_SIZE / 4, (2 + i) * TILE_SIZE, (WIDTH_IN_TILES - 2) * TILE_SIZE - TILE_SIZE / 4, (HEIGHT_IN_TILES - 2 - i) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
            }
        }
        drawTile(TILE_ID_COIN, 2 * TILE_SIZE, 13 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
        drawText("x", 3 * TILE_SIZE, 13 * TILE_SIZE, (WIDTH_IN_TILES - 3) * TILE_SIZE, (HEIGHT_IN_TILES - 14) * TILE_SIZE, (SDL_Color){16, 32, 140}, false);
        drawText(toString(player->money, buffer), 4 * TILE_SIZE, 13 * TILE_SIZE, 3 * TILE_SIZE, (HEIGHT_IN_TILES - 14) * TILE_SIZE, (SDL_Color){16, 32, 140}, true);

        SDL_Event e;
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
                    if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_UP))
                    {
                        if (cursor.y > 1 * TILE_SIZE)
                            cursor.y -= TILE_SIZE;
                    }

                    if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_DOWN))
                    {
                        if (cursor.y < 12 * TILE_SIZE)
                            cursor.y += TILE_SIZE;
                    }

                    if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_INTERACT))
                        quit = true;
                }
            }
            drawTile(cursor.tileIndex, cursor.x, cursor.y, TILE_SIZE, SDL_FLIP_NONE);
            SDL_RenderPresent(mainRenderer);
        }
        int itemIndex = cursor.y / TILE_SIZE - 2;
        if (itemIndex >= 0)
        {
            int itemCode = player->items[itemIndex];
            if (itemCode / 10 != TILE_ID_STONE)
            {
                quit = false;
                player->items[itemIndex] = 0;
            }
            else
            {
                sprite cursor;
                SDL_Color textColor = (SDL_Color){16, 32, 140};
                SDL_Color bgColor = (SDL_Color){181, 182, 173};
                initSprite(&cursor, TILE_SIZE, 4 * TILE_SIZE, TILE_SIZE, TILE_ID_CURSOR, (entityType) type_na);
                SDL_SetRenderDrawColor(mainRenderer, textColor.r, textColor.g, textColor.b, 0xFF);
                SDL_RenderFillRect(mainRenderer, NULL);
                SDL_SetRenderDrawColor(mainRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = SCREEN_WIDTH / 128, .y = SCREEN_HEIGHT / 128, .w = 126 * SCREEN_WIDTH / 128, .h = 126 * SCREEN_HEIGHT / 128}));
                //foreground text
                drawText("TELEPORT TO WHERE?", 1 * TILE_SIZE + TILE_SIZE / 4, 5 * SCREEN_HEIGHT / 64, SCREEN_WIDTH, 51 * SCREEN_HEIGHT / 64, textColor, false);

                drawText("BACK", 2 * TILE_SIZE + TILE_SIZE / 4, 4 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 4) * TILE_SIZE, textColor, false);
                drawText("PLAIN PLAINS", 2 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 5) * TILE_SIZE, textColor, false);
                if (player->beatenBosses > 0)
                    drawText("DRAGON'S DEN", 2 * TILE_SIZE + TILE_SIZE / 4, 6 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 6) * TILE_SIZE, textColor, false);
                if (player->beatenBosses > 10)
                    drawText("WORRY QUARRY", 2 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 7) * TILE_SIZE, textColor, false);
                if (player->beatenBosses > 20)
                    drawText("WEST POLE", 2 * TILE_SIZE + TILE_SIZE / 4, 8 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 8) * TILE_SIZE, textColor, false);
                if (player->beatenBosses > 30)
                    drawText("RIVER LAKE", 2 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 9) * TILE_SIZE, textColor, false);
                if (player->beatenBosses > 40)
                    drawText("UNDER CITY", 2 * TILE_SIZE + TILE_SIZE / 4, 10 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 10) * TILE_SIZE, textColor, false);
                if (player->beatenBosses > 50)
                    drawText("UPPER CITY", 2 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 11) * TILE_SIZE, textColor, false);
                if (player->beatenBosses > 61)
                    drawText("BATTLEGROUND", 2 * TILE_SIZE + TILE_SIZE / 4, 12 * TILE_SIZE, SCREEN_WIDTH, (HEIGHT_IN_TILES - 12) * TILE_SIZE, textColor, false);

                SDL_Event e;
                bool quit = false;
                int tpSelection = -1;
                while(!quit)
                {
                    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = cursor.x, .y = cursor.y, .w = cursor.w, .h = cursor.w}));
                    while(SDL_PollEvent(&e) != 0)
                    {
                        if(e.type == SDL_QUIT)
                        {
                            quit = true;
                            tpSelection = 4;
                        }
                        else if(e.type == SDL_KEYDOWN)
                        {
                            if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_UP))
                            {
                                if (cursor.y > 4 * TILE_SIZE)
                                    cursor.y -= TILE_SIZE;
                            }

                            if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_DOWN))
                            {
                                if (cursor.y < (player->beatenBosses / 10 + 5) * TILE_SIZE)
                                    cursor.y += TILE_SIZE;
                            }

                            if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_INTERACT))
                            {
                                tpSelection = cursor.y / TILE_SIZE - 3;
                                quit = true;
                            }
                        }
                    }
                    drawTile(cursor.tileIndex, cursor.x, cursor.y, TILE_SIZE, SDL_FLIP_NONE);
                    SDL_RenderPresent(mainRenderer);
                }
                if (tpSelection > 1)
                {
                    player->worldNum = tpSelection - 1;
                    player->mapScreen = 10;
                    player->spr.x = 4 * TILE_SIZE;
                    player->spr.y = 6 * TILE_SIZE;
                }
            }
            if (itemCode / 10 == TILE_ID_POTION)
            {
                if (player->HP == player->maxHP)
                    player->items[itemIndex] = itemCode;
                player->HP += 40 + 60 * ((itemCode - 10 * (itemCode / 10)) - 1);
                if (player->HP > player->maxHP)
                    player->HP = player->maxHP;
            }
            if (itemCode / 10 == TILE_ID_ARMOR)
            {
                int increasedHP = 2 + 10 * (itemCode - 10 * (itemCode / 10));
                player->maxHP += increasedHP;
                player->HP += increasedHP;
                if (player->maxHP > 999)
                    player->maxHP = 999;
                if (player->HP > player->maxHP)
                    player->HP = player->maxHP;
            }
            if (itemCode / 10 == TILE_ID_SWORD || itemCode / 10 == TILE_ID_TOME)
            {
                char* allAttacks = ALL_ATTACKS;
                char thisAttack[6] = "     \0";
                char wArray[9] = "^: ";
                strcat(wArray, strncpy(thisAttack, (allAttacks + (player->move1 - 40) * 5), 5));
                char aArray[9] = "<: ";
                strcat(aArray, player->move2 ? strncpy(thisAttack, (allAttacks + (player->move2 - 40) * 5), 5) : "     ");
                char sArray[9] = "V: ";
                strcat(sArray, player->move3 ? strncpy(thisAttack, (allAttacks + (player->move3 - 40) * 5), 5) : "     ");
                char dArray[9] = ">: ";
                strcat(dArray, player->move4 ? strncpy(thisAttack, (allAttacks + (player->move4 - 40) * 5), 5) : "     ");
                int retCode = aMenu("Which Slot?", wArray, aArray, sArray, dArray, "BACK", 5, 0, OVERWORLD_MENU_PALETTE, true, false);
                //use aMenu() to do the move replacement menu
                int newMove = 40 + 14 * (itemCode / 10 == 97) + 2 * (itemCode - 10 * (itemCode / 10));
                if (retCode == 1)
                    player->move1 = newMove;
                if (retCode == 2)
                    player->move2 = newMove;
                if (retCode == 3)
                    player->move3 = newMove;
                if (retCode == 4)
                    player->move4 = newMove;
                if (retCode == 5)
                    player->items[itemIndex] = itemCode;  //restoring item back to its place
            }
        }
    }
    return exitCode;
}

bool doBattle(player* player, bool isBoss)
{
    bool won = false, run = false;
    int menuLevel = 1;
    //printf("Did battle after %d steps.\n", player->steps);
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
        enemyIndex = 3 * player->worldNum + 3 * (player->worldNum == 7 && player->mapScreen == 23);
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
    char* buffer = "";
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
    if (isBoss)
    {
        char* quipArray[SIZE_OF_BOSS_QUIP_ARRAY] = ALL_BOSS_QUIP_ARRAY;
        drawTextBox(quipArray[player->worldNum - 1], player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
        waitForKey();
    }
    while (!doneFlag)
    {
    bool actFlag = false;
    int exitCode = 0;
        while(!actFlag)
        {
            char* textBoxText = "error\0";
            if (menuLevel == 1)
                textBoxText = "Up:   ATTACK\nDown: BLOCK\nMenu: RUN";
            if (menuLevel == 2)
            {
                char thisAttack[6] = "     \0";
                char input[89];
                strcpy(input, "Up:    ");
                strcat(input, strncpy(thisAttack, (allAttacks + (player->move1 - 40) * 5), 5));
                strcat(input, "\nLeft:  ");
                strcat(input, player->move2 ? strncpy(thisAttack, (allAttacks + (player->move2 - 40) * 5), 5) : "     ");
                strcat(input, "\nDown:  ");
                strcat(input, player->move3 ? strncpy(thisAttack, (allAttacks + (player->move3 - 40) * 5), 5) : "     ");
                strcat(input, "\nRight: ");
                strcat(input, player->move4 ? strncpy(thisAttack, (allAttacks + (player->move4 - 40) * 5), 5) : "     ");
                strcat(input, "\nMenu:  BACK\0");
                //3 + 5 chars + 11 spaces --> 88 total chars
                textBoxText = input;
            }
            drawTextBox(textBoxText, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
            SDL_Event e;
            bool quit = false;
            SDL_Delay(500);
            while(!quit)
            {
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                        exitCode = ANYWHERE_QUIT;
                    }
                    else if(e.type == SDL_KEYDOWN)
                    {
                        if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_UP))
                        {
                            exitCode = BATTLE_ACT_CODE_UP;
                            quit = true;
                        }

                        if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_LEFT))
                        {
                            if (menuLevel == 2)
                            {
                                exitCode = BATTLE_ACT_CODE_LEFT;
                                quit = true;
                            }
                        }

                        if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_DOWN))
                        {
                            exitCode = BATTLE_ACT_CODE_DOWN;
                            quit = true;
                        }

                        if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_RIGHT))
                        {
                            if (menuLevel == 2)
                            {
                                exitCode = BATTLE_ACT_CODE_RIGHT;
                                quit = true;
                            }
                        }

                        if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_MENU))
                        {
                            exitCode = BATTLE_ACT_CODE_MENU;
                            quit = true;
                        }
                    }
                }
            }
            if (exitCode == BATTLE_ACT_CODE_UP && menuLevel == 1)
            {
                menuLevel = 2;
                exitCode = 0;
            }
            if ((exitCode == BATTLE_ACT_CODE_MENU || exitCode == ANYWHERE_QUIT) && menuLevel == 2)
            {
                menuLevel = 1;
                exitCode = 0;
            }
            if ((exitCode > 0 && ((exitCode == BATTLE_ACT_CODE_UP && player->move1) || (exitCode == BATTLE_ACT_CODE_LEFT && player->move2) ||
                                  (exitCode == BATTLE_ACT_CODE_DOWN && player->move3) || (exitCode == BATTLE_ACT_CODE_RIGHT && player->move4)) &&
                                   menuLevel == 2) || (exitCode != 0 && exitCode != BATTLE_ACT_CODE_UP && menuLevel == 1))
                actFlag = true;
        }
        //proceed with attacks here
        int attackCode = 0;
        if (menuLevel == 2)
        {
            if (exitCode == BATTLE_ACT_CODE_UP)
                attackCode = player->move1;
            if (exitCode == BATTLE_ACT_CODE_LEFT)
                attackCode = player->move2;
            if (exitCode == BATTLE_ACT_CODE_DOWN)
                attackCode = player->move3;
            if (exitCode == BATTLE_ACT_CODE_RIGHT)
                attackCode = player->move4;
        }
        if (menuLevel == 1)
        {
            if (exitCode == BATTLE_ACT_CODE_MENU || exitCode == ANYWHERE_QUIT)
            {
                attackCode = ATTACK_CODE_RUN;
                //not(boss) and 3>randInt(1,U-100fPart(A
                if (3 > (rand() % abs(enemyIndex)) - player->speed && enemyIndex % 3 != 0)
                {
                    won = false;
                    run = true;
                }
            }
            if (exitCode == BATTLE_ACT_CODE_DOWN)
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
                    if (attackCode != ATTACK_CODE_RUN && attackCode != ATTACK_CODE_BLOCK)
                    {
                        if (attackCode > ATTACK_CODE_BLOCK)
                        {
                            //if it's a magic attack
                            for(int x = 1; x < TILE_SIZE + 1; x++)
                            {
                                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 4 * TILE_SIZE, .y = 7 * TILE_SIZE, .w = 2 * TILE_SIZE, .h = TILE_SIZE}));
                                drawTile(TILE_ID_TOME, 4 * TILE_SIZE + x, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                                drawTile(player->spr.tileIndex, 4 * TILE_SIZE, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                                SDL_RenderPresent(mainRenderer);
                                SDL_Delay(1);
                            }
                            //quickly presenting tome follow by spell cast
                            SDL_Delay(200);
                        }
                        else
                        {
                            //if it's a physical attack
                            for(int x = 0; x > -6; x--)
                            {
                                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 4 * TILE_SIZE + x + 1, .y = 7 * TILE_SIZE, .w = TILE_SIZE, .h = TILE_SIZE}));
                                drawTile(player->spr.tileIndex, 4 * TILE_SIZE + x, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                                SDL_RenderPresent(mainRenderer);
                                SDL_Delay(15);
                            }
                            SDL_Delay(15);
                            //quick backstep animation
                            for(int x = -6; x < TILE_SIZE; x++)
                            {
                                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 4 * TILE_SIZE + x - 1, .y = 7 * TILE_SIZE, .w = TILE_SIZE, .h = TILE_SIZE}));
                                drawTile(player->spr.tileIndex, 4 * TILE_SIZE + x, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                                SDL_RenderPresent(mainRenderer);
                                SDL_Delay(1);
                            }
                            //from backstep, pushing foward
                            for (int x = TILE_SIZE; x > -1; x--)
                            {
                                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 4 * TILE_SIZE + x + 1, .y = 7 * TILE_SIZE, .w = TILE_SIZE, .h = TILE_SIZE}));
                                drawTile(player->spr.tileIndex, 4 * TILE_SIZE + x, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                                SDL_RenderPresent(mainRenderer);
                                SDL_Delay(1);
                            }
                            //retreating to neutral after attack
                            //the reason for x > -1 is because it'll display at x == 0, fully resetting the position.
                        }
                    }
                    drawTile(attackCode, attackCode == ATTACK_CODE_BLOCK || attackCode == ATTACK_CODE_RUN ? 4 * TILE_SIZE : enemy.x, attackCode == ATTACK_CODE_BLOCK || attackCode == ATTACK_CODE_RUN ? 7 * TILE_SIZE : enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                    strcpy(textBoxText, player->name);
                    strcat(textBoxText, " USED ");
                    char input[6];
                    if (attackCode != ATTACK_CODE_RUN && attackCode != ATTACK_CODE_BLOCK)
                    {
                        crit = 0 < ((rand() % (11 + 2 * (attackCode > ATTACK_CODE_BLOCK) + 5 * (attackCode == ATTACK_CODE_BLOCK || attackCode == ATTACK_CODE_RUN))) - 9);
                        if (crit && attackCode != ATTACK_CODE_BLOCK && attackCode != ATTACK_CODE_RUN)
                            drawTile(TILE_ID_CRITICAL, enemy.x, enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                        {
                            int enemy1[5] = {4, 5, 16, 17, 18};
                            int weakness1[4] = {48, 49, 64, 65};
                            int enemy2[6] = {7, 8, 9, 13, 14, 15};
                            int weakness2[4] = {42, 43, 58, 59};
                            int weakness3[4] = {50, 51, 66, 67};
                            superEffective = (-1 != checkArrayForIVal(enemyIndex, enemy1, 5) && -1 != checkArrayForIVal(attackCode, weakness1, 4))
                            || (-1 != checkArrayForIVal(enemyIndex, enemy2, 6) && (attackCode == 56 || attackCode == 57))
                            || ((enemyIndex == 10 || enemyIndex == 11 || enemyIndex == 12) && -1 != checkArrayForIVal(attackCode, weakness2, 4))
                            || ((enemyIndex == 19 || enemyIndex == 20 || enemyIndex == 21) && -1 != checkArrayForIVal(attackCode, weakness3, 4))
                            || (enemyIndex != 21 && (attackCode == 54 || attackCode == 70));
                            //if (superEffective)
                                //printf("Attack ID %d is super effective against enemy ID %d.\n", attackCode, enemyIndex);
                        }
                        if (superEffective && (attackCode != TILE_ID_ALPHA || attackCode != TILE_ID_SMASH))
                            drawTile(TILE_ID_SUPEREFFECTIVE, enemy.x, enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                        input[5] = '\0';
                        strncpy(input, (allAttacks + (attackCode - 40) * 5), 5);

                        double rawDMG;
                        if (attackCode < 55)
                            rawDMG = iPart(7 + pow(((player->attack - 1.0) * 1.2), 1.3));
                        if (attackCode > 55)
                            rawDMG = iPart(6 + pow(((player->attack - 1.0) * 1.2), 1.25));
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
                    for(int x = 0; x < 6; x++)
                    {
                        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = enemy.x + x - 1, .y = enemy.y, .w = TILE_SIZE, .h = TILE_SIZE}));
                        drawTile(enemy.tileIndex, enemy.x + x, enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                        SDL_RenderPresent(mainRenderer);
                        SDL_Delay(15);
                    }
                    SDL_Delay(40);
                    //heavier backstep
                    for(int x = 6; x > -TILE_SIZE; x--)
                    {
                        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = enemy.x + x + 1, .y = enemy.y, .w = TILE_SIZE, .h = TILE_SIZE}));
                        drawTile(enemy.tileIndex, enemy.x + x, enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                        SDL_RenderPresent(mainRenderer);
                        SDL_Delay(2);
                    }
                    //from backstep, pushing forward
                    for (int x = -TILE_SIZE; x < 1; x++)
                    {
                        SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = enemy.x + x - 1, .y = enemy.y, .w = TILE_SIZE, .h = TILE_SIZE}));
                        drawTile(enemy.tileIndex, enemy.x + x, enemy.y, TILE_SIZE, SDL_FLIP_NONE);
                        SDL_RenderPresent(mainRenderer);
                        SDL_Delay(1);
                    }
                    //going back to neutral
                    //the reason for x < 1 is because it'll update for x == 0, fully resetting the position.
                    drawTile(39 + enemyIndex - (enemyIndex / 3) - (enemy.tileIndex == TILE_ID_DREGOH) + 17 * (enemy.tileIndex == TILE_ID_ARCHER) - 2 * (enemy.tileIndex == TILE_ID_SOLDIER), 4 * TILE_SIZE, 7 * TILE_SIZE, TILE_SIZE, SDL_FLIP_NONE);
                    strcpy(textBoxText, enemyName);
                    strcat(textBoxText, " USED ");
                    char input[6];
                    strncpy(input, (allAttacks + (enemyIndex - (enemyIndex / 3) - 1 - (enemy.tileIndex == TILE_ID_DREGOH) + 17 * (enemy.tileIndex == TILE_ID_ARCHER) - 2 * (enemy.tileIndex == TILE_ID_SOLDIER)) * 5), 5);
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
                        player->HP = 0;
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
                SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = redrawX, .y = redrawY, .w =  TILE_SIZE, .h = TILE_SIZE}));
                if (attackCode > ATTACK_CODE_BLOCK && playerTurn)
                    SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.x = 5 * TILE_SIZE, .y = 7 * TILE_SIZE, .w = TILE_SIZE, .h = TILE_SIZE}));
                drawTile(redrawIndex, redrawX, redrawY, TILE_SIZE, SDL_FLIP_NONE);
                SDL_RenderPresent(mainRenderer);
                waitForKey();
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
        //if you lost
        player->HP = player->maxHP;
        player->mapScreen = 10;
        player->spr.x = 4 * TILE_SIZE;
        player->spr.y = 6 * TILE_SIZE;
        SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
        for(int i = 0; i <= 255; i += 2)
        {
            SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, i);
            SDL_RenderFillRect(mainRenderer, &((SDL_Rect){.w = SCREEN_WIDTH, .h = 9 * TILE_SIZE}));
            SDL_RenderPresent(mainRenderer);
            SDL_Delay(18 - 16 * (i > 90));
        }
        SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_NONE);
    }
    drawTextBox(won ? "You won!" : run ? "You fled!" : "You lost!", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
    waitForKey();
    if (won)
    {
        //if you won
        int acquiredGold = 4 - (player->level - enemyIndex);
        int acquiredExp = 23 + 3 * enemyIndex + 3 * (player->worldNum * (enemyIndex % 3 == 0)) - 4 * (player->level - enemyIndex);
        // 23+3U+3int(M)(U/3=int(U/3))-4(L-U->B
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

        drawText("EXP TO LV UP:", TILE_SIZE / 4, 11 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        {
            int remainingExp = (int)(50 + pow((player->level - 1), 1.75)) - player->experience;
            if (remainingExp < 0)
                remainingExp = 0;
            drawText(toString(remainingExp, buffer), 13 * TILE_SIZE + TILE_SIZE / 4, 11 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        }
        drawText("TOTAL GOLD:", TILE_SIZE / 4, 12 * TILE_SIZE + TILE_SIZE / 4, SCREEN_WIDTH, TILE_SIZE, (SDL_Color){0, 0, 0}, false);
        drawText(toString(player->money, buffer), 11 * TILE_SIZE + TILE_SIZE / 4, 12 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE, TILE_SIZE, (SDL_Color){0, 0, 0}, true);
        SDL_Delay(400);
        waitForKey();
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
            SDL_Delay(400);
            waitForKey();
            //doing stat increases
            SDL_SetRenderDrawColor(mainRenderer, 0xB5, 0xB6, 0xAD, 0xFF);
            SDL_Delay(400);
            sprite cursor;
            initSprite(&cursor, TILE_SIZE / 4, 9 * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE, TILE_ID_CURSOR, type_na);
            int breakCode = 0;
            bool quit = false;
            SDL_Event e;
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
                                if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_UP))
                                {
                                    if (cursor.y > 9 * TILE_SIZE + TILE_SIZE / 4)
                                        cursor.y -= TILE_SIZE;
                                }

                                if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_DOWN))
                                {
                                    if (cursor.y < 12 * TILE_SIZE + TILE_SIZE / 4)
                                        cursor.y += TILE_SIZE;
                                }
                                if (e.key.keysym.sym == SDL_GetKeyFromScancode(SC_INTERACT))
                                {
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
                        drawTextBox("QUIT WITHOUT USING ALL PTS?\nConfirm = YES\nANYTHING ELSE = NO", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
                        SDL_Delay(400);
                        quit = waitForKey() == KCInteract;
                    }
                }
            }
        }
        if (isBoss)
        {
            int itemLocation = findItem(player, TILE_ID_STONE * 10 + player->worldNum - 1);
            if (itemLocation == -1)
                pickupItem(player, TILE_ID_STONE * 10 + player->worldNum, -1, false);
            else
            {
                if (enemy.tileIndex != TILE_ID_DREGOH)
                {
                    player->items[itemLocation] = TILE_ID_STONE * 10 + player->worldNum;
                    drawTextBox("Your fight seemed to change your teleport stone!", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
                    waitForKey();
                }
            }
            player->beatenBosses += 10 - 9 * (player->worldNum == 7 && player->mapScreen == 23); // <-reg boss beaten = +10, world 7 alt boss = +1
        }
        if (isBoss && player->worldNum == 8)
        {
            aWallOfText("And...", ENDING_TEXT, false, true);
            savePlayerData(player, SAVE_FILE_NAME);
        }
    }
    return won || run;
}
bool pickupItem(player* player, int itemCode, int chestID, bool redraw)
{
    int firstOpenSlot = findItem(player, 0);
    if (redraw)
    {
        drawTilemap(player->spr.x / TILE_SIZE, player->spr.y, 1 + player->spr.x / TILE_SIZE, 1 + player->spr.y / TILE_SIZE, false);
        drawTile(player->spr.tileIndex, player->spr.x, player->spr.y, player->spr.w, player->flip);
        SDL_RenderPresent(mainRenderer);
    }
    if (firstOpenSlot > -1)
    {
        player->items[firstOpenSlot] = itemCode;
        char text[24] = "Found ";
        if (itemCode / 10 == TILE_ID_SWORD)
            strcat(text, "a Sword!\0");
        if (itemCode / 10 == TILE_ID_TOME)
            strcat(text, "a Tome!\0");
        if (itemCode / 10 == TILE_ID_ARMOR)
            strcat(text, "some Armor!\0");
        if (itemCode / 10 == TILE_ID_POTION)
            strcat(text, "a Potion!\0");
        if (itemCode / 10 == TILE_ID_STONE)
            strcat(text, "a Teleport Stone!\0");
        if (chestID >= 0)
            player->pickedUpChests[chestID] = 1;
        if (!(itemCode / 10 == TILE_ID_STONE) && redraw)
        {
            drawTilemap(player->spr.x / TILE_SIZE, player->spr.y / TILE_SIZE, player->spr.x / TILE_SIZE + 1, player->spr.y / TILE_SIZE + 1, true);
            drawTile(player->spr.tileIndex, player->spr.x, player->spr.y, player->spr.w, player->flip);
            SDL_RenderPresent(mainRenderer);
        }
        drawTextBox(text, player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
    }
    else
    {
        firstOpenSlot = -1;
        drawTextBox("My inventory's too full!", player, (SDL_Color){0, 0, 0}, (SDL_Rect){.y = 9 * TILE_SIZE, .w = SCREEN_WIDTH, .h = (HEIGHT_IN_TILES - 9) * TILE_SIZE});
    }
    SDL_Delay(400);
    waitForKey();
    if (!(itemCode / 10 == TILE_ID_STONE) && redraw)
        drawTilemap(0, 8, WIDTH_IN_TILES, HEIGHT_IN_TILES, true);
    return firstOpenSlot > -1;
}

int findItem(player* player, int itemToFind)
{
    int firstOpenSlot = -1;
    for(int i = PLAYER_ITEMS_LIMIT; i >= 0; i--)
    {
        if (player->items[i] == itemToFind)
            firstOpenSlot = i;
    }
    return firstOpenSlot;
}
