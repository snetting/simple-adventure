/* 
 * A simple adventure game.
 * Refactored for safety and readability.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_COMMAND 64
#define MAX_INPUT 32
#define INVENTORY 9
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
    {1, "Bedroom", "The room is dimly lit. 1980s posters cover the walls.\nYou notice a phone jack above your desk.", 0, 0, 2, 0},
    {2, "Hallway", "A hallway complete with terrible wallpaper. Embarrassing childhood photos cover the walls.", 1, 5, 3, 4},
    {3, "Bathroom", "A bathroom. Dead flies cover the window sill.", 2, 0, 0, 0},
    {4, "Junk Room", "A second bedroom, although currently filled with junk.", 0, 2, 0, 0},
    {5, "Stairway", "A gloomy stairway with worn carpet and peeling wallpaper.", 0, 0, 6, 2},
    {6, "Lounge", "The living room, although currently covered in empty beer bottles and pizza boxes.", 5, 7, 0, 8},
    {7, "Diner", "A dining room. Old electronic equipment is stacked from floor to ceiling.", 0, 0, 0, 6},
    {8, "Kitchen", "Supposedly a kitchen but you're not sure you'd want to cook here!", 0, 6, 0, 0}
};

GameObject objects[] = {
    {1, "computer", "an Amstrad CPC 464.", 1},
    {2, "rs232c", "an Amstrad RS232 interface.", 4},
    {3, "monitor", "a green screen monitor.", 1},
    {4, "modem", "an RS232 modem.", 6},
    {5, "pizza", "a cold, half eaten pizza.", 6},
    {6, "teabag", "a mouldy teabag.", 8},
    {7, "magazine", "a copy of Amstrad Action.", 3}
};

const int num_rooms = sizeof(rooms) / sizeof(rooms[0]);
const int num_objects = sizeof(objects) / sizeof(objects[0]);

int current_room = 1;
int prev_room = 0;
int game_won = 0;

void print_room(int room_id) {
    if (room_id <= 0 || room_id >= num_rooms) return;
    
    Room *r = &rooms[room_id];
    printf("\nYou are in the %s\n", r->name);
    printf("%s\n", r->description);
    
    printf("Objects:\n");
    int found = 0;
    for (int i = 0; i < num_objects; i++) {
        if (objects[i].location == room_id) {
            printf("  %s\n", objects[i].name);
            found = 1;
        }
    }
    if (!found) printf("  (none)\n");
    
    printf("Exits:\n");
    if (r->north) printf("  North (%s)\n", rooms[r->north].name);
    if (r->east)  printf("  East (%s)\n", rooms[r->east].name);
    if (r->south) printf("  South (%s)\n", rooms[r->south].name);
    if (r->west)  printf("  West (%s)\n", rooms[r->west].name);
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
    
    Room *r = &rooms[current_room];
    
    if (strcmp(cmd, "n") == 0) {
        if (r->north) return r->north;
        printf("You can't go that way.\n");
    } else if (strcmp(cmd, "e") == 0) {
        if (r->east) return r->east;
        printf("You can't go that way.\n");
    } else if (strcmp(cmd, "s") == 0) {
        if (r->south) return r->south;
        printf("You can't go that way.\n");
    } else if (strcmp(cmd, "w") == 0) {
        if (r->west) return r->west;
        printf("You can't go that way.\n");
    } else if (strcmp(cmd, "look") == 0) {
        prev_room = 0; // Force redraw
    } else if (strcmp(cmd, "ex") == 0) {
        if (arg[0] == '\0') {
            printf("Examine what?\n");
        } else {
            int found = 0;
            for (int i = 0; i < num_objects; i++) {
                if (strcmp(arg, objects[i].name) == 0) {
                    if (objects[i].location == current_room || objects[i].location == INVENTORY) {
                        printf("\nYou examine the %s.\nIt's %s\n", objects[i].name, objects[i].description);
                        found = 1;
                        break;
                    }
                }
            }
            if (!found) printf("The %s is not here.\n", arg);
        }
    } else if (strcmp(cmd, "inv") == 0) {
        printf("Inventory:\n");
        int found = 0;
        for (int i = 0; i < num_objects; i++) {
            if (objects[i].location == INVENTORY) {
                printf("  %s: %s\n", objects[i].name, objects[i].description);
                found = 1;
            }
        }
        if (!found) printf("  (empty)\n");
    } else if (strcmp(cmd, "take") == 0) {
        if (arg[0] == '\0') {
            printf("Take what?\n");
        } else {
            int found = 0;
            for (int i = 0; i < num_objects; i++) {
                if (strcmp(arg, objects[i].name) == 0) {
                    if (objects[i].location == current_room) {
                        objects[i].location = INVENTORY;
                        printf("You take the %s.\n", objects[i].name);
                        found = 1;
                        break;
                    }
                }
            }
            if (!found) printf("The %s is not here.\n", arg);
        }
    } else if (strcmp(cmd, "drop") == 0) {
        if (arg[0] == '\0') {
            printf("Drop what?\n");
        } else {
            int found = 0;
            for (int i = 0; i < num_objects; i++) {
                if (strcmp(arg, objects[i].name) == 0) {
                    if (objects[i].location == INVENTORY) {
                        objects[i].location = current_room;
                        printf("You drop the %s.\n", objects[i].name);
                        found = 1;
                        break;
                    }
                }
            }
            if (!found) printf("You don't have that.\n");
        }
    } else if (strcmp(cmd, "use") == 0) {
        if (arg[0] == '\0') {
            printf("Use what?\n");
        } else if (strcmp(arg, "teabag") == 0) {
            printf("You poke the mouldy teabag.\n");
        } else if (strcmp(arg, "magazine") == 0) {
            printf("You read the magazine. Some time passes.\n");
        } else if (strcmp(arg, "computer") == 0) {
            if (current_room == 1) { // Bedroom
                int score = 0;
                printf("You connect the following: ");
                for (int i = 0; i < 4; i++) { // computer, rs232c, monitor, modem
                    if (objects[i].location == 1) {
                        printf("%s ", objects[i].name);
                        score++;
                    }
                }
                printf("\n");
                if (score == 4) {
                    printf("Win! You have set up your Amstrad and are now ready for some 8-bit fun!\n");
                    game_won = 1;
                } else {
                    printf("You try to use the computer but something appears to be missing.\n");
                }
            } else {
                printf("You need to be in your bedroom to set up the computer properly.\n");
            }
        } else {
            printf("You can't use that here.\n");
        }
    } else if (strcmp(cmd, "quit") == 0) {
        exit(0);
    } else {
        printf("I don't understand '%s'.\n", cmd);
    }
    
    return current_room;
}

int main() {
    char input[MAX_COMMAND];

    printf("\n\nWelcome to a simple adventure.\n\n");
    printf("Commands: n, e, s, w, ex [obj], take [obj], drop [obj], use [obj], look, inv, quit\n\n");

    while (!game_won) {
        if (prev_room != current_room) {
            print_room(current_room);
            prev_room = current_room;
        }
        
        printf("\nCommand: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        current_room = handle_command(input);
    }

    if (game_won) {
        printf("\nCongratulations! You've won the game!\n");
    }

    return 0;
}
