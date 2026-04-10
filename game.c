/* 
 * A simple adventure game.
 * Now with branching endings and corrected inventory logic!
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_COMMAND 64
#define MAX_INPUT 32
#define INVENTORY 100
#define INVALID_EXIT 0

typedef struct {
    int id;
    const char *name;
    const char *description;
    int north, east, south, west;
} Room;

typedef struct {
    int id;
    const char *name;
    const char *description;
    int location;
} GameObject;

Room rooms[] = {
    {0, "None", "Nothing here.", 0, 0, 0, 0},
    {1, "Bedroom", "Dimly lit by a lava lamp. 1980s posters of neon cars cover the walls.\nA phone jack sits above your desk, waiting for a connection.", 0, 0, 2, 0},
    {2, "Hallway", "Brown wood-paneling and terrible floral wallpaper. \nEmbarrassing childhood photos in brass frames line the walls.", 1, 5, 3, 4},
    {3, "Bathroom", "The avocado-green suite is a design crime. \nDead flies litter the window sill next to a bottle of Brut.", 2, 0, 0, 0},
    {4, "Junk Room", "Piles of old magazines and tangled cables. \nIt's a graveyard for obsolete tech and mismatched socks.", 0, 2, 0, 0},
    {5, "Stairway", "Gloomy stairs with worn orange carpet and peeling wallpaper. \nIt smells faintly of old cigarettes and floor wax.", 0, 0, 6, 2},
    {6, "Lounge", "A sagging brown sofa faces a chunky CRT television. \nEmpty beer bottles and pizza boxes are scattered like 8bit debris.", 5, 7, 9, 8},
    {7, "Diner", "A heavy oak table sits under a dusty chandelier. \nOld electronic equipment is stacked from floor to ceiling.", 0, 0, 0, 6},
    {8, "Kitchen", "Lino flooring and formica cupboards. \nA kettle whistles on the stove, or maybe that's just your tinnitus.", 0, 6, 0, 0},
    {9, "Backyard", "Overgrown grass and a rusty swing set. \nA wooden shed stands in the corner under a grey sky.", 6, 0, 0, 10},
    {10, "Shed", "It's dark and smells of creosote and lawnmower oil. \nSpiders have claimed most of the corners here.", 0, 9, 0, 0}
};

GameObject objects[] = {
    {1, "computer", "an Amstrad CPC 464 with built-in tape deck.", 1},
    {2, "rs232c-interface", "an Amstrad RS232 interface. It plugs into the back of the CPC.", 4},
    {3, "monitor", "a green screen monitor. It's heavy and boxy.", 1},
    {4, "modem", "a chunky RS232 modem with big red lights and a phone cable.", 6},
    {5, "pizza", "a cold, half eaten Hawaiian pizza. Classic.", 6},
    {6, "teabag", "a mouldy teabag. It looks like a science experiment.", 8},
    {7, "magazine", "a copy of Amstrad Action. Issue 1!", 3},
    {8, "walkman", "a yellow Sony Sports Walkman. It's waterproof!", 10},
    {9, "floppy", "a 3-inch floppy disk labeled 'Slipstream BBS'.", 4},
    {10, "cube", "a scrambled Rubik's Cube. It's frustrating.", 2},
    {11, "drive", "an Amstrad DDI-1 external floppy disk drive.", 10},
    {12, "cassette", "a cassette tape labeled 'Bridge-It'.", 4}
};

const int num_rooms = sizeof(rooms) / sizeof(rooms[0]);
const int num_objects = sizeof(objects) / sizeof(objects[0]);

int current_room = 1;
int prev_room = 0;
int game_won = 0;
int pizza_eaten = 0;

void print_help() {
    printf("\nAvailable Commands:\n");
    printf("  north (n), east (e), south (s), west (w)  - Move in a direction\n");
    printf("  examine (ex) [object]                    - Examine an object\n");
    printf("  take (t) [object]                       - Pick up an object\n");
    printf("  drop (d) [object]                       - Put down an object\n");
    printf("  use (u) [object]                        - Use an object\n");
    printf("  inventory (inv, i)                      - Show what you are carrying\n");
    printf("  look (l)                                - Look around the current room\n");
    printf("  help (h)                                - Show this help list\n");
    printf("  quit (q)                                - Exit the game\n");
}

int get_object_index(const char *name) {
    for (int i = 0; i < num_objects; i++) {
        if (strcmp(name, objects[i].name) == 0) return i;
    }
    return -1;
}

void print_room(int room_id) {
    if (room_id <= 0 || room_id >= num_rooms) return;
    
    Room *r = &rooms[room_id];
    printf("\n--- %s ---\n", r->name);
    printf("%s\n", r->description);
    
    printf("\nObjects here:\n");
    int found = 0;
    for (int i = 0; i < num_objects; i++) {
        if (objects[i].location == room_id) {
            printf("  - %s\n", objects[i].name);
            found = 1;
        }
    }
    if (!found) printf("  (nothing of interest)\n");
    
    printf("\nExits:\n");
    if (r->north) printf("  North: %s\n", rooms[r->north].name);
    if (r->east)  printf("  East:  %s\n", rooms[r->east].name);
    if (r->south) printf("  South: %s\n", rooms[r->south].name);
    if (r->west)  printf("  West:  %s\n", rooms[r->west].name);
}

void to_lowercase(char *str) {
    for (; *str; ++str) *str = tolower((unsigned char)*str);
}

int handle_command(char *input) {
    char cmd[MAX_INPUT] = "";
    char arg[MAX_INPUT] = "";
    
    char *token = strtok(input, " \n");
    if (!token) return current_room;
    strncpy(cmd, token, MAX_INPUT - 1);
    cmd[MAX_INPUT - 1] = '\0';
    to_lowercase(cmd);
    
    token = strtok(NULL, " \n");
    if (token) {
        strncpy(arg, token, MAX_INPUT - 1);
        arg[MAX_INPUT - 1] = '\0';
        to_lowercase(arg);
    }
    
    // Easter Eggs
    if (strcmp(cmd, "xyzzy") == 0) {
        printf("A hollow voice says 'Fool'.\n");
        return current_room;
    }
    if (strcmp(cmd, "jump") == 0) {
        printf("You jump up and down. You look ridiculous.\n");
        return current_room;
    }
    
    Room *r = &rooms[current_room];
    
    if (strcmp(cmd, "n") == 0 || strcmp(cmd, "north") == 0) {
        if (r->north) return r->north;
        printf("A wall prevents your progress.\n");
    } else if (strcmp(cmd, "e") == 0 || strcmp(cmd, "east") == 0) {
        if (r->east) return r->east;
        printf("You can't go that way.\n");
    } else if (strcmp(cmd, "s") == 0 || strcmp(cmd, "south") == 0) {
        if (r->south) return r->south;
        printf("The path is blocked.\n");
    } else if (strcmp(cmd, "w") == 0 || strcmp(cmd, "west") == 0) {
        if (r->west) return r->west;
        printf("You bump into the wallpapered wall.\n");
    } else if (strcmp(cmd, "look") == 0 || strcmp(cmd, "l") == 0) {
        prev_room = 0; // Force redraw
    } else if (strcmp(cmd, "ex") == 0 || strcmp(cmd, "examine") == 0) {
        if (arg[0] == '\0') {
            printf("Examine what?\n");
        } else {
            int idx = get_object_index(arg);
            if (idx != -1 && (objects[idx].location == current_room || objects[idx].location == INVENTORY)) {
                printf("\n%s\nIt's %s\n", objects[idx].name, objects[idx].description);
            } else {
                printf("You don't see that here.\n");
            }
        }
    } else if (strcmp(cmd, "inv") == 0 || strcmp(cmd, "inventory") == 0 || strcmp(cmd, "i") == 0) {
        printf("\nYou are carrying:\n");
        int found = 0;
        for (int i = 0; i < num_objects; i++) {
            if (objects[i].location == INVENTORY) {
                printf("  - %s\n", objects[i].name);
                found = 1;
            }
        }
        if (!found) printf("  Nothing. Your pockets are empty.\n");
    } else if (strcmp(cmd, "take") == 0 || strcmp(cmd, "t") == 0) {
        if (arg[0] == '\0') {
            printf("Take what?\n");
        } else {
            int idx = get_object_index(arg);
            if (idx != -1 && objects[idx].location == current_room) {
                objects[idx].location = INVENTORY;
                printf("You pick up the %s.\n", objects[idx].name);
            } else {
                printf("The %s isn't here to take.\n", arg);
            }
        }
    } else if (strcmp(cmd, "drop") == 0 || strcmp(cmd, "d") == 0) {
        if (arg[0] == '\0') {
            printf("Drop what?\n");
        } else {
            int idx = get_object_index(arg);
            if (idx != -1 && objects[idx].location == INVENTORY) {
                objects[idx].location = current_room;
                printf("You dropped the %s.\n", objects[idx].name);
            } else {
                printf("You aren't carrying that.\n");
            }
        }
    } else if (strcmp(cmd, "use") == 0 || strcmp(cmd, "u") == 0) {
        if (arg[0] == '\0') {
            printf("Use what?\n");
        } else {
            int idx = get_object_index(arg);
            if (idx == -1 || (objects[idx].location != current_room && objects[idx].location != INVENTORY)) {
                printf("You don't have the %s.\n", arg);
            } else if (strcmp(arg, "teabag") == 0) {
                printf("You poke the mouldy teabag. It squishes unpleasantly. Gross.\n");
            } else if (strcmp(arg, "magazine") == 0) {
                printf("You flip through the pages. The tips and tricks for 'Manic Miner' look useful.\n");
            } else if (strcmp(arg, "pizza") == 0) {
                if (!pizza_eaten) {
                    printf("You eat the cold pizza. It's rubbery but strangely nostalgic. You feel energized!\n");
                    pizza_eaten = 1;
                    objects[idx].location = 0; // Remove from game
                } else {
                    printf("The pizza is already gone.\n");
                }
            } else if (strcmp(arg, "walkman") == 0) {
                printf("You put on the headphones. Synth-pop fills your ears. Radical!\n");
            } else if (strcmp(arg, "cube") == 0) {
                printf("You twist the Rubik's Cube. You manage to get one side blue. Good enough.\n");
            } else if (strcmp(arg, "computer") == 0) {
                if (current_room == 1) { // Bedroom
                    // BBS Ending items: interface (idx 1), modem (3), floppy (8), drive (10)
                    int has_bbs = (objects[1].location == 1 || objects[1].location == INVENTORY) &&
                                  (objects[3].location == 1 || objects[3].location == INVENTORY) &&
                                  (objects[8].location == 1 || objects[8].location == INVENTORY) &&
                                  (objects[10].location == 1 || objects[10].location == INVENTORY);
                    
                    // BASIC Ending items: cassette (idx 11)
                    int has_basic = (objects[11].location == 1 || objects[11].location == INVENTORY);

                    if (has_bbs) {
                        printf("WIN! You connect the RS232 interface, modem, and disk drive.\n");
                        printf("The green screen flickers to life as you load the Slipstream BBS software from the floppy.\n");
                        printf("BEEP... HISSS... You are now connected to the world!\n");
                        game_won = 1;
                    } 
                    else if (has_basic) {
                        printf("WIN! You load the cassette into the built-in deck.\n");
                        printf("The green screen flickers to life. READY.\n");
                        printf("You spend the rest of the night coding in BASIC.\n");
                        game_won = 1;
                    } else {
                        printf("The computer is here, but you have no software to load.\n");
                        printf("You need either the cassette or the disk drive setup (interface, modem, drive, and floppy).\n");
                    }
                } else {
                    printf("You should probably set this up on your desk in the bedroom.\n");
                }
            } else {
                printf("You're not sure how to use the %s right now.\n", arg);
            }
        }
    } else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "h") == 0) {
        print_help();
    } else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) {
        printf("Goodbye! Thanks for playing.\n");
        exit(0);
    } else {
        printf("I don't understand '%s'. Try 'help'.\n", cmd);
    }
    
    return current_room;
}

int main() {
    char input[MAX_COMMAND];

    printf("\n========================================\n");
    printf("   RETRO ADVENTURE: THE AMSTRAD QUEST   \n");
    printf("========================================\n\n");
    printf("It's 1986. You need to get your computer set up.\n");
    print_help();

    while (!game_won) {
        if (prev_room != current_room) {
            print_room(current_room);
            prev_room = current_room;
        }
        
        printf("\n> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        current_room = handle_command(input);
    }

    if (game_won) {
        printf("\nCongratulations! You've conquered the 80s!\n");
    }

    return 0;
}
