/* A very simple, badly written adventure.
   There's also an exciting lack of comments.
   steve@netting.org.uk 
*/

#include <stdio.h>
#include <string.h>

int totalrooms;
int i, room;
int won;
int prevroom = 0;
char command[20];

/* room id 9 doesn't exist but marks invalid exits */
char *rooms[] = { "0", "dummy", "dummy", "9", "9", "9", "9", 
                  "1", "Bedroom", "The room is dimly lit. 1980s posters cover the walls.\n You notice a phone jack above your desk.", "9", "9", "2", "9",
                  "2", "Hallway", "A hallway complete with terrible wallpaper. Embarassing childhood photos cover the walls.", "1", "5", "3", "4",
                  "3", "Bathroom", "A piss stained bathroom. Dead flies cover the window sill.", "2", "9", "9", "9",
                  "4", "Bedroom", "A second bedroom, alhough currently filled with junk.", "9", "2", "9", "9",
                  "5", "Stairway", "A gloomy stairway with worn carpet and peeling wallpaper.", "9", "9", "6", "2",
                  "6", "Lounge", "The living room, although currently covered in empty beer bottles and pizza boxes.", "5", "7", "9", "8",
                  "7", "Diner", "A dining room, although you can't remember when you last ate here.\n Old electronic equipment is stacked from floor to celing..", "9", "9", "9", "6",
                  "8", "Kitchen", "Supposedly a kitchen but I'm not sure you'd want to cook here!", "9", "6", "9", "9",
                };

/* Use location 9 as 'in inventory' */
int totalobjects = 8;
char *objects[] = { "0", "dummy", "dummy", "0",
                    "1", "computer", "an Amstrad CPC 464.", "1",
                    "2", "rs232c", "an Amstrad RS232 interface.", "4",
                    "3", "monitor", "a green screen monitor.", "1",
                    "4", "modem", "an RS232 modem.", "6",
                    "5", "pizza", "a cold, half eaten pizza.", "6",
		    "6", "teabag", "a mouldy teabag.", "8",
		    "7", "magazine", "a copy of Amstrad Action.", "3",
                    };

void printroom(int room);
int userinput(char command[], int room);
void printobjects(int room);

int main()
{
  room = 1;
  won = 0;

  printf("\n\nWelcome to a very simple adveture.\n\nCommands: n/e/s/w, ex (examine), take, drop, use, look and inv (inventory).\n\n");

  while ( won == 0 )
  {
    if ( prevroom != room ) {
      printroom(room);
    }
    printf("\nCommand: ");
    fgets(command,20,stdin);
    strtok(command, "\n");
    /* printf("DEBUG CAUGHT: %s\n", command); */
    prevroom = room;
    room = userinput(command, room);
  }

  return 0;

}

void printroom(int room_id)
{
  int exit_lkup, roomp;
  roomp = room_id*7; 
  /*printf("Passed Room: %i\n", room_id);
  printf("Array Position: %i\n", room);
  printf("\nID: %s\n", rooms[roomp]);*/
  printf("\nYou are in the %s\n", rooms[roomp+1]);
  printf(" %s\n", rooms[roomp+2]);
  printobjects(room_id);
  printf("Exits:\n");
  if (strcmp(rooms[roomp+3], "9") != 0) {
    exit_lkup = atoi(rooms[roomp+3]);
    printf(" North (%s)\n", rooms[exit_lkup*7+1]);
  }
  if (strcmp(rooms[roomp+4], "9") != 0) {
    exit_lkup = atoi(rooms[roomp+4]);
    printf(" East (%s)\n", rooms[exit_lkup*7+1]);
  }
  if (strcmp(rooms[roomp+5], "9") != 0) {
    exit_lkup = atoi(rooms[roomp+5]);
    printf(" South (%s)\n", rooms[exit_lkup*7+1]);
  }
  if (strcmp(rooms[roomp+6], "9") != 0) {
    exit_lkup = atoi(rooms[roomp+6]);
    printf(" West (%s)\n", rooms[exit_lkup*7+1]);
  }
}

void printobjects(int room_id)
{
  int obj_id;
  printf("Objects:\n");
  for (obj_id=0;obj_id<totalobjects;obj_id++) {
  /*printf("Checking object %i...\n", obj_id);
  printf("Name: %s\n", objects[obj_id*4+1]);
  printf("object check room id %i\n", room_id);
  printf("Compare Object %i, location %s and current room %s\n", obj_id, objects[obj_id*4+3], rooms[room_id*7]); 
  */

  if (!strcmp(objects[obj_id*4+3], rooms[room_id*7])) {
  printf(" %s\n", objects[obj_id*4+1]); 
  }
  /*printf("objects[obj_id*4+3]=%i", obj_id*4+3);
    if (strcmp(objects[obj_id*4+3], room_id)) {
      printf("%s\n", objects[obj_id*4]); }
    
  */
  }
}

int userinput(char command[], int room)
{
  /*printf("\nPassed: %s in Room: %i\n", command, room);*/
  char *pch;
  char in_a[10] = "";
  char in_b[10] = "";
  int exit_lkup = room;
  int obj_id, score, i;

  pch = strtok (command, " ");

  strcpy(in_a, pch);
 
  /* Convert to lower case */
  for(i = 0; in_a[i]; i++){
    in_a[i] = tolower(in_a[i]);
  }

  
  pch = strtok (NULL, " ");
  if ( pch != NULL) {
    strcpy(in_b, pch); 
    /* Convert to lower case */
    for(i = 0; in_b[i]; i++){
      in_b[i] = tolower(in_b[i]);
    }
  }


  /*printf("in_a = %s\n", in_a);
  printf("in_b = %s\n", in_b);
  */

  if (strcmp(in_a, "n") == 0) {
    if (strcmp(rooms[room*7+3], "9") != 0) {
      exit_lkup = atoi(rooms[room*7+3]);
      return exit_lkup;
    }
    else { printf("You can't exit in that direction.\n");
    }
  }

  if (strcmp(in_a, "e") == 0) {
    if (strcmp(rooms[room*7+4], "9") != 0) {
      exit_lkup = atoi(rooms[room*7+4]);
      return exit_lkup;
    }
    else { printf("You can't exit in that direction.\n");
    }
  }
  if (strcmp(in_a, "s") == 0) {
    if (strcmp(rooms[room*7+5], "9") != 0) {
      exit_lkup = atoi(rooms[room*7+5]);
      return exit_lkup;
    }
      else { printf("You can't exit in that direction.\n");
    }
}
  if (strcmp(in_a, "w") == 0) {
    if (strcmp(rooms[room*7+6], "9") != 0) {
      exit_lkup = atoi(rooms[room*7+6]);
      return exit_lkup;
    }
      else { printf("You can't exit in that direction.\n");
    }
  }
  if (strcmp(in_a, "ex") == 0) {
    for (obj_id=0;obj_id<totalobjects;obj_id++) {
      if (strcmp(in_b, objects[obj_id*4+1]) == 0) {
        if (strcmp(rooms[room*7], objects[obj_id*4+3]) == 0 || strcmp("9", objects[obj_id*4+3]) == 0) { 
          printf("\nYou examine the %s.\n It's %s\n", in_b, objects[obj_id*4+2]); 
        }
        else {
          printf("The %s is not here.", in_b);
        }
      }
    }
    return room;
  }
  if (strcmp(in_a, "inv") == 0) {
    printf("Inventory:\n");
    for (obj_id=0;obj_id<totalobjects;obj_id++) {
      if (strcmp("9", objects[obj_id*4+3]) == 0) {
        printf(" %s, %s\n", objects[obj_id*4+1], objects[obj_id*4+2]);
      }
    }
    return room;
  }

  if (strcmp(in_a, "take") == 0) {
    for (obj_id=0;obj_id<totalobjects;obj_id++) {
      if (strcmp(in_b, objects[obj_id*4+1]) == 0) {
        if (strcmp(rooms[room*7], objects[obj_id*4+3]) == 0) {
          printf("\nYou take the %s.\n", in_b);
          objects[obj_id*4+3]="9";
        }
        else {
          printf("The %s is not here.\n", in_b);
          return room;
        }
      }
    }
    return room;
  } 
    
  if (strcmp(in_a, "drop") == 0) {
    for (obj_id=0;obj_id<totalobjects;obj_id++) {
      if (strcmp(in_b, objects[obj_id*4+1]) == 0) {
        if (strcmp("9", objects[obj_id*4+3]) == 0) {
          printf("\nYou drop the %s.\n", in_b, room); 
          objects[obj_id*4+3]=rooms[room*7];
        }
      }
    }
    return room;
  }


  if (strcmp(in_a, "debug") == 0) {
   for (obj_id=0;obj_id<totalobjects;obj_id++) {
    printf("Object  : %i...\n", obj_id);
    printf("%i      ID: %s\n", obj_id*4+0, objects[obj_id*4+0]);
    printf("%i      NM: %s\n", obj_id*4+1,objects[obj_id*4+1]);
    printf("%i      DS: %s\n", obj_id*4+2,objects[obj_id*4+2]);
    printf("%i      RM: %s\n", obj_id*4+3,objects[obj_id*4+3]);
   }
  return room;
 }
  
 
  if (strcmp(in_a, "use") == 0) {
    /*score=0;*/
    if (strcmp(in_b, "teabag") == 0) { printf("You poke the mouldy teabag.\n"); return room; }
    if (strcmp(in_b, "magazine") == 0) { printf("You read the magazine. Some time passes.\n"); return room; }

    for (obj_id=0;obj_id<totalobjects;obj_id++) {
      if (strcmp(in_b, objects[obj_id*4+1]) == 0) {
        if (strcmp(rooms[room*7], objects[obj_id*4+3]) == 0) {
        /*printf("WIN DEBUG: room %i, Objects in %s %s %s %s.\n", room, objects[4+3], objects[8+3], objects[12+3], objects[16+3]); */
          if (room==1) {printf("You connect the following: ");
          score=0;
          /* printf("(wanted): %s %s %s %s\n", objects[5], objects[9], objects[13], objects[17]); */
          if (strcmp("1", objects[7]) ==0) { printf("%s ", objects[5]); score++; }
          if (strcmp("1", objects[11]) ==0) { printf("%s ", objects[9]); score++; }
          if (strcmp("1", objects[15]) ==0) { printf("%s ", objects[13]); score++; }
          if (strcmp("1", objects[19])==0) { printf("%s ", objects[17]); score++; }
/*          for (score=0;score<20;score++) {
 *           *                      printf("objects[%i]=%s\n", score, objects[score]);
 *            *                                }
 *             *                                */
          printf("\n");
          /*printf("Score= %i", score);*/
          }
          if (score==4) { printf ("Win!"); won=1; }
          else {
            printf("You try to use the %s but something appears to be missing.\n", in_b);
          }
        }
      }
    }
    return room;
  }
  if (strcmp(in_a, "look") == 0) {
    prevroom=0;
    return room;
  }   
      
  printf("I don't understand.\n");
  return room;
}
