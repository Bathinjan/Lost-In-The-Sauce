// Lost in the Sauce
// A Stirring Text Adventure 
// Written by Bathinjan

// NOTES
// This awful, clunky code is based off an assignment I had in C for Spring 2021
// My grade was A+ and the prof said "longest assignment I've ever graded" (nearly 1k lines jfc sorry dude)
// I listened to Chili Con Carne on repeat for nearly 5 weeks while I coded this
// So please excuse the very candid tone in the comments, I had fun if you couldn't tell
// I heavily copied the parser straight from some dude's Geocities website but hey it works
// This was literally My First Text Adventure and also my first fully fledged program so apologies in advance

// TODO
// add checks to prevent writing multiple of the same lines to recipe.txt (screwed this up when I implemented line breaks instead of @ to divide items in txt file, opted for easier readability)
// write a better help menu probably
// make game EXIT_SUCCESS actually work u dingus

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

// Function Prototypes
void helpMenu(void); 
void conTinue(void); // haha WHOOPS reserved word alert
int userInput(void); 
int parser1(FILE *writePointer, FILE *readPointer, bool *clearParserPointer); 
int parser2(FILE *writePointer, FILE *readPointer, bool inventory[7]); 
void printInventory(bool inventory[7]);
void printLocations(int noteCount, const char* const locations[10]); 

// Global Variables
char uInput[20];
const char* const locations[11] = {"stove", "fridge", "freezer", "sink", "cabinets", "microwave"}; 
bool inventory[10] = {false /*[0]tomato*/,false /*[1]onions*/,false /*[2]meat*/,false /*[3]beans*/,false /*[4]spices*/,false /*[5]tortillas*/,false /*[6]GameEnd*/, false /*[7]Can Opener*/, false /*[8]CO used*/, false /*[9]5found*/};
bool clearParser = false; // used before I implemented bool array, kept in b/c lazy

int main(int argc, char *argv[])
{
  // Variables
	char name[256];

  // absolutely unnecessary but required by the assignment
  char userE;
  char uYes[5] = "yes";
  char uNo[4] = "no";

  // set Pointers
  bool *clearParserPointer = &clearParser;
  FILE *writePointer = fopen ("recipe.txt", "w"); // write pointer to file recipe.txt, write mode - should reset txt on each run
  FILE *readPointer = fopen("recipe.txt", "r"); // read pointer to receipt.txt
	srand(time(NULL)); // RNG Seed
	printf("Please enter your name: ");
	scanf("%s",name);
      
  // inventory[6] game clear condition    
  while(inventory[6] == false)
  {
    // Opening Narration
    system("clear");

    puts("--------------------------------------------------------------------------------------------");
    puts("You're probably not sleepwalking.\n\nIf you were, someone would have *certainly* woken you up at this point to tell you how stupid you look ambling around your house in your ratty old PJ's.\n\n");
    puts("However, you're convinced you couldn't be awake either - blinking your eyes several times, nothing but darkness stretches out in front of you. You try to examine your hand and find it's not even visible an inch from your face.\n");
    puts("You decide to press forward - where else could you go, anyway? As you carefully place one foot in front of another, after a small eternity, your toes finally hit the hard edge of some sort of wall.\n\n");
    conTinue();
    system("clear");
    puts("You splay your fingers across the smooth surface, searching for a way through. The edge of your hand collides dully with something circular - you place your hand on what feels like a doorknob. Turning it in your fingers, it yeilds to the movement - it's not locked!");
    puts("\nTwisting the knob and bracing your shoulder, you push against the wall with a strained grunt. It's much less heavy than you anticipated, leading you to throw off your own weight and clumsily step through trying to keep your balance.\n");
    puts("It slams shut just as you catch your footing. The door is now sealed tightly behind you.\n");
    puts("A familiar grumble roils from deep within your gut.");
    puts("Before you conquer that hefty door again, you'll have to vanquish the hunger beast.\n");
    printf("Do you want to take a look around? (Type y for yes or n for no) :");
    scanf(" %c", &userE);

    switch(userE)
    {
      case 'y' :
      {
        puts("\nPatting your stomach in agreement, you decide that going on incredible adventures through a very large mysterious void can wait.\n");
        puts("For now, you have to look around for some food. Eat first, think later.\n");
        break;
      }

      case 'n' :
      {
        puts("\n...no?");
        puts("\nNO?????");
        printf("\noOOOooo OOO oo lookit me, I'm a silly little adventurer named %s! I dOnT nEeD To eAt!! i'M sO StROnG aN d ClEvEr AnD I'm nEvEr HuNgRYyyyYYH haAHAa heeHEEhoO\n\n", name);
        puts("You're about as dumb as a bowl of oats. Do you think you can open a heavily sealed door like this?\n");
        puts("When you're so hungry, you couldn't push one little door back open?!\n");
        puts("Well, too bad - that door isn't budging, and you're looking around for some sustenance, whether you like it or not! Your stomach isn't giving you any other options here!\n");
        break;
      }

      case 'm' :
      {
        puts("\n...what do you mean by 'm'? Maybe??\n\nListen, you gotta be more decisive in life. Don't 'maybe' your way through things anymore. Today's the day you're gonna seize destiny, and you're gonna seize it right here in this kitchen!\n");
        break;
      }

      default :
      {
        puts("\nWell, whatever you chose, the universe pretended not to hear it. Your fate is dictated by your stomach, so you're being thrown headfirst into this adventure whether you like it or not!\n");
        break;
      }
    }

    conTinue();

    system("clear");
    puts("---------------------------------------------------------------------------------");
    puts("LOST IN THE SAUCE\n\nYOUR GOALS : \nFind the 5 Notes, Complete the Recipe \nCook and eat the food to escape the Kitchen!");
    helpMenu();
    puts("--------------------------------------------------------------------------------");
    conTinue();
    puts("\nYou've stumbled through the door, but found no additional light source. The room is still completely dark.\n");
    puts("Fishing your phone out of your pocket, you swipe a few times to activate its flashlight.\n");
    puts("Sweeping the room, you notice the stainless steel glint of a FRIDGE and FREEZER wedged into the right corner.\n");
    puts("You can make out the red glow of a STOVE clock, blinking 00:00 steadily against the darkness.\n");
    
    // Parser Loop 1
    while (userInput() && parser1(writePointer, readPointer, clearParserPointer));

  }

  fclose(writePointer);

	return EXIT_SUCCESS;
}

void helpMenu(void)
{
	puts("--------------------------------------------------------------------------------");
	puts("COMMANDS:\n look + WORD\n use + WORD\n locations, inv, help, exit\n\nType 'look' or 'use' followed by the OBJECT you want to interact with (objects with CAPITAL letters can be interacted with)\nExample: use KEY, look TABLE\n\nType 'help' to view a list of commands.\nType 'locations' to view the places you can go.\nType 'inv' to see your inventory.\nYou can also type 'use' to put some items in your inventory.");
	puts("\nType 'exit' to leave the parser.");
	puts("--------------------------------------------------------------------------------");
}

void printInventory(bool inventory[10]) // printInventory begins in 2nd parser
{
  puts("You have the following items in your inventory: \n");
  int idx;
  if(inventory[0] == true)
  {
    puts("Tomato\n");
  }

  if(inventory[1] == true)
  {
    puts("Onions\n");
  }

  if(inventory[2] == true)
  {
    puts("Meat\n");
  }

  if(inventory[3] == true)
  {
    puts("Beans\n");
  }

  if(inventory[4] == true)
  {
    puts("Spices\n");
  }

  if(inventory[5] == true)
  {
    puts("Tortillas\n");
  }

  if(inventory[6] == true)
  {
    puts("Beer\n");
  }

  if(inventory[7] == true)
  {
    puts("Can Opener");
  }

}

void printLocations(int noteCount, const char* const locations[10])
{
  int i = 0;
  puts("You can go to the following locations: \n");
  if (noteCount < 3)
  {
    for (i = 0; i < 3; i++)
    {
      printf("%s\n", locations[i]);
    }
  }

  else
  {
    for (i = 0; i < 6; i++)
    {
      printf("%s\n", locations[i]);
    }
  }

}

// implemented because c dislikes getchar() by itself to pause for user input
// a bit fucky but it mostly works
void conTinue(void) 
{
	printf("(Press 0 and Enter to continue)");
	fflush(stdout); // do I need to flush stdout?? Ah fuck it whatever
	while(getchar() != '0'){};
}

// I need to get rid of this but I broke things when I did it before
// it is frankly low on my list of concerns
// pointers amirite lads
void changeboolean(bool *x)
{
  *x = true;
}

int userInput(void) 
{
	//fflush(stdout); // still not really convinced I need this but someone on StackOverflow insisted
	printf("");
	printf("\n------> ");
  return fgets ( uInput, sizeof(uInput), stdin ) != NULL;
}

int parser1(FILE *writePointer, FILE *readPointer, bool *clearParserPointer) 
{
  // parser uses 2 tokenized strings (return 0 if true) for classic 2-string input
	char *word1 = strtok(uInput, " \n");
	char *word2 = strtok(NULL, "\n");

	char string[20];
	int noteCounter = 0;
	rewind(readPointer);

  // check Parser 2 clear
  if ( inventory[6]== true )
  {
    return 0;
  }

  if (word1 != NULL)
  {
    // command "exit"
    if (strcmp(word1, "exit") == 0)
    {
      return 0;
    }

    if (strcmp(word1, "perish") == 0)
    {
      puts("You are dead.\nWhy did you do that??? Why do you want to die in a text adventure game about making chili??? Well I mean I can't stop you so, I guess have a great...rest of your day??? Cool?? See you later???");
      puts("\n\nPress 0 and Enter to actually Perish");
      conTinue();
    }

    // command "exit1" - progress to parser 2
    if (strcmp(word1, "exit1") == 0)
    {
      system("clear");            
      puts("A flash of light cuts across your vision - shielding your eyes with your arm, you squint against the brightness until your eyes adjust.\n\nThe power is back on! Now, you can take a clearer look at your surroundings.\n");
      puts("--------------------------------------------------------------------------------------------");
      puts("Not only have more locations opened up to you, but you're now able to add ingredients to your inventory with the 'use' command!\n\nReference 'recipe.txt' in your files, using the clues you collect, to find 5 ingredients.");
      puts("--------------------------------------------------------------------------------------------");
      puts("The red glow of a STOVE clock steadily blinks 00:00.\n\nYou notice the stainless steel glint of a FRIDGE and FREEZER wedged into the right corner.\n\nA SINK is filled with dishes, and the doors of the CABINETS lay half-open.\n\nA lone MICROWAVE sits in a solitary corner.\n\nYou feel like you might not only be constrained to the commands 'look' and 'use' in some instances...");
      while (userInput() && parser2(writePointer, readPointer, inventory));
    }

    // command "help"
    else if (strcmp(word1, "help") == 0)
    {
      helpMenu();
    }

    // command "locations"
    else if (strcmp(word1, "locations") == 0)
    {
      printLocations(noteCounter, locations);
    }

    // command "inv"
    else if (strcmp(word1, "inv") == 0)
    {
      puts("The user only has 'updog' in their inventory.\n");
    }

    // command "look"
    else if (strcmp(word1, "look") == 0) 
    {
      // look stove 
      if (strcmp(word2, "STOVE") == 0 || strcmp(word2, "stove") == 0)
      {
        puts("\nThe oven light is blinking 00:00 in red letters against the darkness, suggesting that the power went out recently, the appliance is broken, or both.");
      }

      // look fridge
      else if (strcmp(word2, "FRIDGE") == 0 || strcmp(word2, "fridge") == 0)
      {
        puts("\nThe fridge's stainless steel surface is littered with post it notes. You pick one up and angle your cell phone to read it.");
        puts("\n\'If one more person eats my leftovers without permission, I will *commit a felony*. I mean it!!!!!'");
        puts("\nFrowning, you turn it over to discover there's more written on the back.");
        puts("\n'Actually, you know what? Eat them. Go on. I dare you. Eat all of it. I hope you choke on it and die in agony right here on my kitchen floor. You should have never been born!! - J'");
        puts("\nYou find yourself wondering who the hell this 'J' is, and what made them upset enough to cover their refridgerator with passive aggressive post-its.");
      }

      // look beer
      else if (strcmp(word2, "BEER") == 0 || strcmp(word2, "beer") == 0)
      {
        puts("There's a post-it note curving around the can. You turn the can to read the note: \n\n'Good luck getting this Natty Lite open - I found it in the basement and I'm pretty sure it's been in there since the 90's. Why would you want to drink this anyway? You might as well drink your own piss. -J");
      }

      // look leftovers
      else if (strcmp(word2, "LEFTOVERS") == 0 || strcmp(word2, "leftovers") == 0)
      {
        puts("A post-it note sitting on the top reads : 'J's Leftovers - Please, help yourself.' You wonder why this person bothered to personally label their leftovers only to follow up with 'help yourself'...");
      }

      // look sauce
      else if (strcmp(word2, "SAUCE") == 0 || strcmp(word2, "sauce") == 0)
      {
        puts("Picking up the bottle and turning it in your hand, you notice it's nearly empty; a post-it note on the side hangs by a corner. It reads: 'Stop hitting the sauce'. You turn it over to read: 'Get it?? Because booze is also called 'sauce' but it's also bbq sauce! heh...'-J\n\nAnother post-it note tacked onto the end says: 'Jim, did you seriously just write 'heh' on a post it note? Explaining your own joke?! *Stop* wasting my office supplies!! - H.'\n\nYou find some irony in the fact that 'H' decided to reply with an equally pointless post-it note.");
      }

      // look freezer
      else if (strcmp(word2, "FREEZER") == 0 || strcmp(word2, "freezer") == 0)
      {
        puts("\nThe freezer's stainless steel surface is littered with post it notes. You pick one up and angle your cell phone to read it.");
        puts("\n'Jim, I swear to God, if you move the eggs again to put more post-it notes inside, I will buy the dollar store brand of BBQ sauce from now on. Do not try me, young man. - H'");
        puts("\nAnother post-it note is attached to the end of this one. It reads: ");
        puts("\n'I do not fear death or dry chicken nuggets, woman! I will *not* be manipulated by the sauce! \nI have an emergency stash of Sweet Baby Rays for just this occasion, anyway. -J'");
        puts("\nYou are baffled by just how much writing can fit on a post-it note. Your eyes are starting to hurt from squinting at all the tiny writing.\n");
      }

      // look door
      else if (strcmp(word2, "DOOR") == 0 || strcmp(word2, "door") == 0)
      {
        puts("The door is tightly sealed behind you. In order to open it, you're going to need something to eat first.");
      }

      // look freezer note (note #1)
      else if (strcmp(word2, "NOTE#1") == 0 || strcmp(word2, "note#1") == 0 || strcmp(word2, "note1") == 0 || strcmp(word2, "NOTE1") == 0)
      {
        puts("You pluck the freezer NOTE from its spot and hold it gingerly between your thumb and forefinger; it must have been here for some time, because it's quite cold. It reads:\n\n'TOMATO - MICROWAVE'\n\nYou're not sure what that means at all, but you place the note in your pocket anyway.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "TOMATO | MICROWAVE\n");
        fflush(writePointer);
        noteCounter++;
      }

      // look fridge note (note #2)
      else if (strcmp(word2, "NOTE#TWO") == 0 || strcmp(word2, "note#two") == 0 || strcmp(word2, "note # two") == 0 || strcmp(word2, "NOTE # TWO") == 0)
      {
        puts("Doing your best not to get more sauce underneath your nails, you unfold the saucy note and find it reads: \n\nBEANS - SINK\n\nNot wanting to ruin your pants pocket, you throw the gross note into the nearby trash can, glad to be done away with it.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "BEANS | SINK\n");
        fflush(writePointer);
      }

      // look stove note (note #3)
      else if (strcmp(word2, "NOTE3") == 0 || strcmp(word2, "note3") == 0)
      {
        puts("Grabbing for the note, you accidentally kick up a little dust; coughing, you wave a hand in front of your face. You brush off the note and read: \n\nONION - CABINETS\n\nYou're not excited to prep onions for a meal, but you're also hungry enough not to care at this point, and press on.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "ONION | CABINETS\n");
        fflush(writePointer);
      }

    }

    // command "use"
    else if (strcmp(word1, "use") == 0) 
    {
      // use stove
      if (strcmp(word2, "STOVE") == 0 || strcmp(word2, "stove") == 0)
      {
        // originally planned to have a broken / fixed stove flag
        puts("\nYou turn a burner knob tentatively, but hear a 'click click click' of a burner failing to light. You smell a faint tinge of gas. Crouching on your heels and sweeping your cell phone light, you can't find anything else wrong with the oven. Looks like you might need matches to get a burner working.\n\nNOTE3, covered in a fine layer of dust and a few hairs, catches the light.");
      }

      // use fridge
      else if (strcmp(word2, "FRIDGE") == 0 || strcmp(word2, "fridge") == 0)
      {
        puts("\nYou pull gently on the fridge handle, and are greeted with its soft inner glow and a faint whiff of eggs and...old onions?? Ugh.");
        puts("\nYou spot a container of LEFTOVERS, a bottle of bbq SAUCE, and a can of BEER. There are post-it notes on each.");
      }

      // use beer
      else if (strcmp(word2, "BEER") == 0 || strcmp(word2, "beer") == 0)
      {
        puts("You go to pull the metal tab of the BEER to find that it's been removed. Undeterred, you press your thumb into the aluminum divot instead, willing to go the extra mile for the bastion of alcohol.\n\nUnfortunately, the aluminum won't budge. Leveraging the can against your leg, you push in with your thumb with all the force you can muster, but it doesn't yeild.\n\nDefeated, your thumb smarting, you shove the can back into the fridge. You'll have to find a tool later to try to open it.");
      }

      // use leftovers
      else if (strcmp(word2, "LEFTOVERS") == 0 || strcmp(word2, "leftovers") == 0)
      {
        puts("Holding the container still with one hand, you hook your fingers on the lip of the tupperware and pull eagerly. The lid pries open easily to reveal the contents inside.\n\nWhatever is inside is concealed by a piece of paper towel. Lifting it to get a better look, you find it's suprisingly heavy, and you struggle to steady it. Peeling away the paper towel layer and poking the contents underneath, you find that you're looking at a brick.\n\nThat's...that's definitely a *brick* in a tupperware container.\n\nYou're now absolutely convinced this 'J' person is a definitely psycho for putting a brick in the fridge to bait anyone taking their leftovers. Who does that!?");
      }

      // use sauce
      else if (strcmp(word2, "SAUCE") == 0 || strcmp(word2, "sauce") == 0)
      {
        puts("You pick up the bottle of bbq SAUCE. You've decided to 'use' it, but what does that mean exactly? Are you just gonna squeeze this thing? Right here??\n\nNot one to question your own motives, you twist off the cap, aim the bottle at the floor, and squeeze with reckless abandon.\n\nYour chaotic choice is rewarded, however, when something not-liquid slips out of the bottle partway, coated in a thick layer of sauce. Undaunted, and with a few more squeezes, you manage to get it out and into the palm of your hand.\n\nYou're now covered in the scent of Sweet Baby Ray's bbq SAUCE, but don't seem to mind as you prod at the item in your hand, letters beneath the smudge of sauce reading NOTE#TWO. (To read it, type 'use note#two') ");
      }

      // use freezer
      else if (strcmp(word2, "FREEZER") == 0 || strcmp(word2, "freezer") == 0)
      {
        puts("\nYou swiftly against the hermetic seal of the freezer, and are met with a small puff of chilled air.");
        puts("\n...followed closely by a much stronger, more decidedly *pungent* whiff of egg.");
        puts("\nBlinking rapidly to ensure you're not seeing things, you realize the entire fridge is full of decorated frozen easter eggs. A small post-it note on eye-level with the user reads: 'Little Jimmy's Easter Eggs, 1987. Do ***not*** throw away!!!'");
        puts("\nHorrified, you sincerely wish you had found a dead body instead.");
        puts("\nThere is an additional post it labelled NOTE#1 wedged in between the freezer wall and the awful, awful Egg Pile.");
      }

      // use freezer note (note #1)
      else if (strcmp(word2, "NOTE#1") == 0 || strcmp(word2, "note#1") == 0 || strcmp(word2, "note1") == 0 || strcmp(word2, "NOTE1") == 0)
      {
        puts("You pluck the freezer NOTE from its spot and hold it gingerly between your thumb and forefinger; it must have been here for some time, because it's quite cold. It reads:\n\n'TOMATO - MICROWAVE'\n\nYou're not sure what that means at all, but you place the note in your pocket anyway.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "TOMATO | MICROWAVE\n");
        fflush(writePointer);
        noteCounter++;
      }

      // use fridge note (note #2)
      else if (strcmp(word2, "NOTE#TWO") == 0 || strcmp(word2, "note#two") == 0 || strcmp(word2, "note # two") == 0 || strcmp(word2, "NOTE # TWO") == 0)
      {
        puts("Doing your best not to get more sauce underneath your nails, you unfold the saucy note and find it reads: \n\nBEANS - SINK\n\nNot wanting to ruin your pants pocket, you throw the gross note into the nearby trash can, glad to be done away with it.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "BEANS | SINK\n");
        fflush(writePointer);
      }

      // use stove note (note #3)
      else if (strcmp(word2, "NOTE3") == 0 || strcmp(word2, "note3") == 0)
      {
        puts("Grabbing for the note, you accidentally kick up a little dust; coughing, you wave a hand in front of your face. You brush off the note and read: \n\nONION - CABINETS\n\nYou're not excited to prep onions for a meal, but you're also hungry enough not to care at this point, and press on.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "ONION | CABINETS\n");
        fflush(writePointer);
      }

      // funny joke
      else if (strcmp(word2, "updog") == 0)
      {
        // inventory becomes populated in parser 2
        puts("\nNothin, what's up with you, dude?\n");
        puts(" ( ͡° ͜ʖ ͡°) ");
      }

    }
    
    // dumb parser
    else
    {
      printf("\nI have no idea how to %s, but knock yourself out.\n", word1);
    }

    // pretty sure I broke this at some point? counters had to be taken out at the last minute. can't remember why
    while ( fgets (string, 20, readPointer) != NULL )
    {
      noteCounter++;
      // for some reason it spits out to here when exiting, so I'm trying this EXIT_SUCCESS method on game end = true
      if(inventory[6] == true)
      {
        EXIT_SUCCESS;
      }

      // if I put = does that fix the problem
      if(noteCounter >= 3)
      {
        puts("------------------------------------------------------------------------");
        puts("\nYou've found 3 notes! Check recipe.txt to see if you have 3 entries.");
        puts("If you don't, something probably went wrong. But don't worry : it's a feature, not a bug.");
        puts("\nYou'll be able to solve the next set of puzzles if you've found 3 different notes so far.");
        puts("If you'd like to advance, type 'exit1'. If not, feel free to continue using this parser until you find all three.");
        break;
      }
    }
  
    // user input validation; null strings (thanks Mythra <3)
    if (word2 == NULL)
    {
      word2 = "(empty)";
    }
    if (word1 == NULL)
    {
      word1 = "(empty)";
    }

  }
	return 1;
}

// parser 2
int parser2(FILE *writePointer, FILE *readPointer, bool inventory[10])
{
  char *word1 = strtok(uInput, " \n");
	char *word2 = strtok(NULL, "\n");
  char string[20];
  char userI[20];
  int noteCounter = 3;
  int average = 0; // utterly useless but required by the assignment so ¯\_(ツ)_/¯
  rewind(readPointer);
  int extraCredit, result = 0;
  double score = 0.0;

  // opening narration
  puts("");

  //user input validation - null strings
  if (word2 == NULL)
  {
    word2 = "(empty)";
  }
  
  if (word1 == NULL)
  {
    word1 = "(empty)";
  }

	if (word1 != NULL)
	{
    // command "exit"
		if (strcmp(word1, "exit") == 0)
		{
			return 0;
		}

		// command "help"
		else if (strcmp(word1, "help") == 0)
		{
			helpMenu();
		}

    // command "locations"
    else if (strcmp(word1, "locations") == 0)
    {
      printLocations(noteCounter, locations);
    }

		// command "inv"
		else if (strcmp(word1, "inv") == 0)
		{
      //put inventory things here
			printInventory(inventory);
      puts("------------------------------------------------------------------------");
		}

		// command "look"
		else if (strcmp(word1, "look") == 0) 
		{
      //look cabinets
      if (strcmp(word2, "cabinets") == 0 || strcmp(word2, "CABINETS") == 0)
      {
        puts("\nSweet jesus, finally - finally! The place where snacks are kept! You're absolutely convinced there's something edible in there, but knowing these two knobs J and H, there's also a high possibility you'll find something bizarre and unpleasant, too.\n");
      }

      //look sink
      else if (strcmp(word2, "sink") == 0 || strcmp(word2, "SINK") == 0)
      {
        puts("A post-it hanging off the lip of the sink catches your eye - the letters are big enough to read from afar:\n\nTAKEOUT TRASH\n\nYou step a bit closer to read the post-it note attached below it: 'Helen, for christ's sake, We are out of trash bags, and I'm not going near the neighbor's fence again! You know their goat *hates* me!! -J\n\nYou wonder who's weirder : the 2 post-it goofs living in this house, or the neighbor who has an (allegedly) rabid goat.");
      }

      //look stove
      else if (strcmp(word2, "stove") == 0 || strcmp(word2, "STOVE") == 0)
      {
        puts("Perhaps the burners will work, now that the lights are on?");
      }

      //look fridge
      else if (strcmp(word2, "fridge") == 0 || strcmp(word2, "FRIDGE") == 0)
      {
        puts("\nYou look longingly at the fridge. The distance...it's...still too much. You must get closer...\n\nYou press your cheek against the fridge, straining to wrap your arms around as far as they can reach.\n\nThe fridge does not hug you back, but you somehow feel a brief reprieve from your loneliness, nonetheless.");
      }

      //look freezer
      else if (strcmp(word2, "freezer") == 0 || strcmp(word2, "FREEZER") == 0)
      {
        puts("\nYou look over the freezer. You think of a few cold-related puns, but you quickly banish them from your mind. Puns are the lowest form of comedy, of course, and you'd be a shameless wreck before you'd ever entertain one, even in your head.\n\n\n...If thought to yourself just now, 'Now, that's just cold...', you, in fact, did not. You didn't think anything at all. You don't think. Your head is empty.\n\nGo shove your idiot head full of idiot puns in the freezer for idiots.");
      }

      //look microwave
      else if (strcmp(word2, "microwave") == 0 || strcmp(word2, "MICROWAVE") == 0)
      {
        puts("The microwave has seen better days, but anything that will heat up food for you right now is an appliance sent from heaven as far as you're concerned.");
      }

      //look note # 4 : sink note
      else if (strcmp(word2, "noteFORE") == 0 || strcmp(word2, "notefore") == 0)
      { 
        puts("The note is soaked with water, suds, and bits of mushy food, but miraculously, the blurred writing is still somewhat legible.\nIt reads:\n\n'SPICES - STOVE'\n\nYou drop the drenched post-it into the sink disposal, and run it for good measure. You wash your hands several times, but the icky feeling lingers, and you're not sure it'll go away anytime soon.\n\n");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "SPICES | STOVE\n");
        fflush(writePointer);
        noteCounter++;
      }

      //look note # 5 : cabinets note
      else if (strcmp(word2, "n5") == 0 || strcmp(word2, "N5") == 0)
      { 
        puts("The note reads:\n\n'MEAT - FREEZER'\n\nYou came up empty in the cupboards, but at least you gained another portion of this recipe.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "MEAT | FREEZER\n");
        fflush(writePointer);
        noteCounter++; 
      }
    }

    // command "use"
    else if (strcmp(word1, "use") == 0)
    {
      //use cabinets
      if (strcmp(word2, "cabinets") == 0 || strcmp(word2, "CABINETS") == 0)
      {
        puts("Salivating, you nudge open the cupbaoard doors, praying that you find something. Your body slowly deflates from hope to quiet despair when you're met with the rustiest CANOPENER you've ever seen, and - of course - a post it note.\n\nSighing loudly, you pluck it to read the contents : 'In case of BeerMergencies'\n\nYou're not sure you know what a BeerMergency is, but if it's anything like your current crisis, you're probably pretty close to one right now.\n\nExpecting something snarky, you prod at the post-it note to see that it's actually another portion of the recipe, N5.");
      }

      //use sink
      else if (strcmp(word2, "sink") == 0 || strcmp(word2, "SINK") == 0)
      {
        puts("Before you can even approach, you can tell by looking that the stench is going to be awful. The nauseating wave that hits you as you step closer to the sink confirms your suspicions.\n\nYou really *really* don't want to put your hands anywhere near the filthy pile (especially if it does any favors for this 'J' idiot), but notice a sponge and some detergent on the counter. Perhaps if someone WASHed the DISHES, you could at least get yourself a glass of water or something?");
      }

      //use stove
      else if (strcmp(word2, "stove") == 0 || strcmp(word2, "STOVE") == 0)
      {
        // originally planned to direct user to stove to progress to final stage
        puts("You try the burner knob again, but get a sad repetitive 'click click click', signaling the stove is still not working.");
      }

      //use fridge
      else if (strcmp(word2, "fridge") == 0 || strcmp(word2, "FRIDGE") == 0)
      {
        puts("You open the fridge to spot a lone BEER on the middle tray. You won't be able to get it open by yourself.");
      }

      //use freezer
      else if (strcmp(word2, "freezer") == 0 || strcmp(word2, "FREEZER") == 0)
      {
        puts("You carefully tug on the handle of the freezer, but nothing you can do prepares you from the queasy wave of stench from the Awful Egg Pile.\n\nIt's subtle enough to stay contained in the freezer and fridge, but not so subtle that your eyes don't water every time you encounter it. Good Lord that's bad.");
      }

      // use microwave
      else if (strcmp(word2, "microwave") == 0 || strcmp(word2, "MICROWAVE") == 0)
      {
        // planned to use microwave for more than just a funny
        puts("You prod at the buttons on the microwave, but nothing happens. Angling your body to look behind it, you find that someone has snipped the cord clean in half.\nThe worst part is they just left the cord plugged into the wall, so it still *looks* like it's plugged in.");
      }

      //use note # 4 : sink note
      else if (strcmp(word2, "NOTEFORE") == 0 || strcmp(word2, "notefore") == 0)
      { 
        puts("The note is soaked with water, suds, and bits of mushy food, but miraculously, the blurred writing is still somewhat legible.\nIt reads:\n\n'Spices - Stove'\n\nYou drop the drenched post-it into the sink disposal, and run it for good measure. You wash your hands several times, but the icky feeling lingers, and you're not sure it'll go away anytime soon.\n\n");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "SPICES | STOVE\n");
        fflush(writePointer);
        noteCounter++;
      }

      //use note # 5 : cabinets note
      else if (strcmp(word2, "n5") == 0 || strcmp(word2, "N5") == 0)
      { 
        puts("The note reads:\n\n'Meat - Freezer'\n\nYou came up empty in the cupboards, but at least you gained another portion of this recipe.");
        puts("**RECIPE HAS BEEN UPDATED**");
        fprintf(writePointer, "MEAT | FREEZER \n");
        fflush(writePointer);
        noteCounter++;      
      }

      // use Can Opener
      else if (strcmp(word2, "canopener") == 0 || strcmp(word2, "CANOPENER") == 0)
      {
        // check : was the can opener already used?
        if (inventory[8] == false)
        {
          puts("If you can't find a good use for this, it'll at least make a terrible souvenir for the time you *didn't* go on an adventure, and picked the worst text adventure in the world to play.\n\n");
          inventory[7] = true;
          puts("***CAN OPENER - added to inventory***");   
          puts("\n\nNOTE: To use the can opener, type CANOPENER followed by the item you want to use it on. If nothing happens...it simply wasn't meant to be.");
        }  

        // can opener was already used
        else
        {
          puts("The can opener was absolutely obliterated in your crazy attempt to skewer and shotgun a beer.\n\nIt had a good life, and died in glory.");
        } 
      }

      // use tortillas (hidden item)
      else if (strcmp(word2, "tortillas") == 0 || strcmp(word2, "TORTILLAS") == 0)
      {
        puts("You consider cracking open the tortillas and eating them here and now, but know that it wouldn't do the trick to cure your adventurer's appetite by itself.\n\nYou decide to put it into your inventory, and wait until you assemble the full recipe to eat it.\n\n");
        inventory[5] = true;
        puts("***TORTILLAS - added to inventory***");
        extraCredit++;         
      }

    }

    // wash
    else if (strcmp(word1, "wash") == 0 || strcmp(word1, "WASH") == 0)
    {
      // wash dishes
      if (strcmp(word2, "dishes") == 0 || strcmp(word2, "DISHES") == 0)
      {
        puts("Rolling up your sleeves and grabbing the moist little blob of sponge from the faucet, you get to work on the mound of dishes in the sink. Hot water on blast, you squeeze a healthy dollop of dish detergent onto the dishes to start; it smells like old oranges on vacation in Florida.\n\nMinutes pass; after dealing with the pungent globs of whatever food was left on the plates, you sincerely wish you'd looked for gloves first.\n\nYou go to wash the last cup left in the sink when you notice there's a familiar little crumpled note in it: NOTEFORE");
      }
    }

    // Takeout
    else if (strcmp(word1, "takeout") == 0 || strcmp(word1, "TAKEOUT") == 0)
    {
      // take out trash
      if (strcmp(word2, "trash") == 0 || strcmp(word2, "TRASH") == 0)
      {
        puts("Out of the goodness of your heart - and, perhaps, the sensitivity of your nose - you tug on the edges of the wastebin to pull the trashbag around the stinking contents. Before you tie the top knot, however, you notice a perfectly good package of TORTILLAS sitting on top; you check the expiration date, and in a stroke of luck, they're fresh!.\n\nSetting the tortillas aside, you finish tying the trash and toss it into a corner.\n\nYou haven't quite 'taken it out', but at least you've covered up the smell for now.");
      }
    }

    // Canopener
    else if (strcmp(word1, "canopener") == 0 || strcmp(word1, "CANOPENER") == 0)
    {
      if (inventory[7] == true)
      {
        if (strcmp(word2, "beer") == 0 || strcmp(word2, "BEER") == 0)
        {
          puts("With all the grace and dexterity of a starving animal, you harpoon the side of the beer can with the can opener and steady the can against your mouth, as the most foul stream of alcohol you've ever tasted slams down your gullet.\n\nIt's still beer, though, so you shotgun it down expertly, determined to put the calories and buzz to good use.\n\nExhaling triumphantly, you indulge the urge to crush the can against your adventurous skull and lob it in an upwards arc towards the trash can:\n\n");
          int r = rand() % 2;
          
          // shot misses
          if (r == 0)
          {
            puts("The crushed can hits the wall and unceremoniously bounces off the lip of the trash can. There were no stakes and no one watching, but you feel disappointed in yourself regardless. This kills your buzz off before it could even take hold, and the shotgunned beer now sits souring in your stomach, offering no solace from your hunger.\n");
            inventory[7] = false;
            inventory[8] = true;
          }
          
          // shot makes it
          else if(r == 1)
          {
            puts("As if in slow motion, the can travels in a perfect parabola into the trash can, landing gently into the pile as if being carried by angels.\n\nThere were no stakes and no one watching, but you can hear ecstatic cheers ringing through your ears.\n\nThat, or the buzz is already starting to take effect from all that shotgunned beer. Either way, you're definitely not complaining about the boost in your mood.\n\nYou get the feeling that this event has affected more than just your self-esteem.");
            inventory[7] = false;
            inventory[8] = true;
            extraCredit++;
          }

          else
          {
            puts("Something broke in the RNG machine ¯|_(ツ)_/¯ Try again?");
          }
        }
      }
    }

    // TOMATO - MICROWAVE
    else if (strcmp(word1, "tomato") == 0 || strcmp(word1, "TOMATO") == 0)
    {
      if (strcmp(word2, "microwave") == 0 || strcmp(word2, "MICROWAVE") == 0)
      {
        puts("Tomato was added to your inventory! It's a fruit in all the ways that don't count.");
        inventory[0] = true;
      }
    }

    // BEANS - SINK
    else if (strcmp(word1, "beans") == 0 || strcmp(word1, "BEANS") == 0)
    {
      if (strcmp(word2, "sink") == 0 || strcmp(word2, "SINK") == 0)
      {
        puts("Beans were added to your inventory! They got jalapenos in em...mmm");
        inventory[3] = true;        
      }
    }

    // ONION - CABINETS
    else if (strcmp(word1, "onion") == 0 || strcmp(word1, "ONION") == 0)
    {
      if (strcmp(word2, "cabinets") == 0 || strcmp(word2, "CABINETS") == 0)
      {
        puts("Onions were added to your inventory! Hey, who put all these tears in your eyes??");
        inventory[1] = true;       
      }
    }

    // SPICES - STOVE
    else if (strcmp(word1, "spices") == 0 || strcmp(word1, "SPICES") == 0)
    {
      if (strcmp(word2, "stove") == 0 || strcmp(word2, "STOVE") == 0)
      {
        puts("Spices were added to your inventory! What kind?? The best kind!! All your favorites are here!");
        inventory[4] = true;        
      }
    }

    // MEAT - FREEZER
    else if (strcmp(word1, "meat") == 0 || strcmp(word1, "MEAT") == 0)
    {
      if (strcmp(word2, "freezer") == 0 || strcmp(word2, "FREEZER") == 0)
      {
        puts("Meat was added to your inventory - unless you're a vegetarian/vegan, then it's plant-based protein, I promise.");
        inventory[2] = true;        
      }
    }

    // Final Challenge
    else if (strcmp(word1, "IM") == 0 || strcmp(word1, "im") == 0)
    {
      if (strcmp(word2, "READY") == 0 || strcmp(word2, "ready") == 0)
      {
        // conditions met - array check indices 0-4 True
        if(inventory[0] && inventory[1] && inventory[2] && inventory[3] && inventory[4])
        {
          system("clear");
          puts("It's time. You are more than eager to eat this thing, even if the recipe admittedly doesn't look like a very good one.\n\nYou're about to enter an RNG cooking event, so here's a chance at some extra points:\n\n");
          printf("QUESTION 1: What is 'J's actual name?\n");
          scanf(" %s", userI);
          if (strcmp(userI, "Jim") == 0 || strcmp(userI, "JIM") == 0 || strcmp(userI, "jim") == 0)
          {
            puts("***CORRECT***\n\nThe fact that you remembered means that you read nearly every post-it note, or you have an amazing memory. 1 point added!\n");
            extraCredit++;
          }
          else
          {
            puts("***INCORRECT***\n\nHis name is 'Jim', but that's ok. I wouldn't want to read any more of his post-it notes than I have to, either.\n");
          }

          printf("QUESTION 2: What is 'H's actual name?\n");
          scanf(" %s", userI);
          if (strcmp(userI, "HELEN") == 0 || strcmp(userI, "Helen") == 0 || strcmp(userI, "helen") == 0)
          {
            puts("***CORRECT***\n\nThat was on *1* post-it in the entire game! Excellent work! You're a true sleuther, aren't ya? 3 points added!\n");
            extraCredit = extraCredit + 3;
          }
          else
          {
            puts("***INCORRECT***\n\nThis one was a toughie. Her name is 'Helen', and it was only on one post-it in the entire game. No worries.\n");
          }

          printf("QUESTION 3: What's the name of the person who wrote this game?\n");
          scanf(" %s", userI);
          if (strcmp(userI, "BATHINJAN") == 0 || strcmp(userI, "bathinjan") == 0 || strcmp(userI, "Bathinjan") == 0 || strcmp(userI, "Bath") == 0 || strcmp(userI, "BATH") == 0|| strcmp(userI, "bath") == 0)
          {
            puts("***CORRECT***\n\nYou cheated and looked this up, huh? ( ͡° ͜ʖ ͡°) That's ok...you won't forget my name again, right? (≖‿≖)\n\nUnless you're my friends playtesting this game. Hey. What's up? We playing Phasmo later? Sweet.\n");
            extraCredit++;
          }
          else
          {
            puts("***INCORRECT***\n\nDid you mispell my name? That's alright, you won't mispell it again, will you? (≖‿≖)\n\nMaybe if you followed me on twitter, this wouldn't have happened, hmm??? Go on now, don't be shy : www.twitter.com/bathinjan_ - don't forget the underscore!\n");
          }

          conTinue();
          system("clear");
          // Rewrite this part - or don't. Preserve the memory of you scrambling to finish this literally an hour before it was due lol
          puts("----------------------------------------------");
          puts("In a fit of Deus Ex Machina, you find some matches in your pocket. You can light the burners on the stove now!\n\nAlright, no more fooling around. Time to cook!\n\nThe RNG machine will add a number from 1-10 of your ingredients, then take the average. After that, it will add your extra credit points to your total score.\n\nWhy??? Because it was required for the stupid assignment this game was based off of. I'll change it later (never), so just shut up and get cooking, ok??");
          puts("----------------------------------------------\n");
          result = rand() % 10;
          score  = score + result;
          printf("You take %d tomato(s) and put them in a bowl with oil.\n", result);
          result = rand() % 10;
          score  = score + result;
          printf("Next, you dice %d onion(s), strong and tasty. You throw the mixture into a pot and boil on the stove.\n", result);
          // debug - printf("Score so far: %lf", score);
          result = rand() % 10;
          score  = score + result;
          printf("Whatever kind of meat this is, it doesn't matter - you add %d to the mixture.\n", result);
          result = rand() % 10;
          score  = score + result;
          printf("The beans have jalapeno peppers in 'em - mmmm - very necessary for this recipe. You add %d to the pot.\n", result);
          result = rand() % 10;
          score  = score + result;
          printf("Lastly, you can't forget the spices!! It's the heart of the art of the cooking, after all. You add %d to the mixture and stir vigorously.\n\n", result);

          // tortilla check
          if (inventory[5] == true)
          {
            puts("\n\nYou warm the tortillas to a crisp on the burner next to the pot, then transfer to a clean plate you procured from the dish rack. They sound really nice when they break.");
          }

          // beer check 
          if (inventory[8] == true)
          {
            puts("You wish you had one, or two, or three, or four, or seven beers to wash this down with, but you're realizing now it's probably a godsend you got to shotgun 1 beer, given your circumstances.");
          }

          puts("--------------------------------------------------------------------");
          printf("\nYour RNG score : %lf\n", score);
          average = (double)score / 5;
          printf("Your RNG score average : %d\n\n", average);
          average = average + extraCredit;
          printf("FINAL SCORE : %d\n\n", average);
          conTinue();
          // endings (not based on score, but based on extra credit ( ͡° ͜ʖ ͡°) spooked ya)
          if (extraCredit <= 2)
          {
            puts("--------------------------------------------------------------------");
            puts("You eat the chili con carne with reckless abandon, not even bothering to fish a utensil out of the clean dishes. Your appetite is quickly sated, but you keep eating, feeling a keen sense of spite replacing the space your hunger occupied.\n\nAs you scoop the remnants of the Chili Con Carne out of the pan with your hands, you feel a stomachache set in. This is not a hindrance to your adventurous spirit, however, as you're absolutely determined to pick a better way to spend your time from now on.\n\nYou always hated cooking anyway.\n\n\nLOST IN THE SAUCE : ENDING 1 (Worst Ending)");
            conTinue();
            inventory[6] = true;
            EXIT_SUCCESS;
            return 0;
          }

          else if (extraCredit == 3)
          {
            puts("--------------------------------------------------------------------");
            puts("You fish a spoon out of the clean dishes pile, and get to work on the Chili Con Carne fresh out of the pan. It's not exactly what you expected, but given these crazy circumstances, you'd say you ended up cooking a pretty decent meal for yourself. Your mom would be so proud.\n\nWell, she might not be proud that you chose such a lousy text adventure game to play in your spare time, but at least you can get back to...whatever other things you have to do...after this.\nYou avoid a stomachache, but feel as if your recipe was lacking somehow...couldn't be the spices, you definitely didn't forget those.\n\nMaybe you'll add more salt next time, you think, as you push through the door you came through with a renewed vigor and a sated stomach.");
            puts("\n\n\nLOST IN THE SAUCE : ENDING 2 (Neutral Ending)");
            conTinue();
            inventory[6] = true;
            EXIT_SUCCESS;
            return 0;
          }

          else if (extraCredit >= 4)
          {
            puts("--------------------------------------------------------------------");
            puts("Your palette having been refined by this stressful experience, you take a moment to set the mood. You tuck a napkin into your shirt collar, and procure a bowl, spoon, and tablemat from the clean dish rack. You light the solitary candle on the nearby table with your Deus-Ex-Machina matches.\n\nNo need to rush this exquisite experience. You're a post-it reading, clue-sleuthing champion in your prime, and so is this Chili Con Carne you've whipped up - and you know you deserve it. Using your crisp tortillas and spoon in tandem, you relish eat bite, taking moments here and there to kiss your fingers like an Italian chef to your cooking expertise.\n\nYou may get nothing tangible out of this cooking escapade, but in your heart, you know that this is a meal fit for true winners.\n\nThat's right. You're the greatest text-adventurer there ever was. You can conquer any door, any wall, now. You'd conquer ALL of them if you wanted to.\n\nHaving devoured this Chili Con Carne of Heroes, you're certainly ready for it. Striding through the exit with perfect poise, you decide it deosn't matter whether or not this was all a dream - the spirit of the chili will always be with you. Always.");
            puts("\n\n\nLOST IN THE SAUCE : ENDING 3 (Best Ending)\n");
            conTinue();
            inventory[6] = true;
            EXIT_SUCCESS;
            return 0;           
          }

        } //end game clear

        //conditions not met
        else
        {
          puts("No, you're not. {ಠ ʖ ಠ}\n\nGo find the 5 ingredients first!");
        }
      }
    }

    // debug - canopener 'true'
    else if (strcmp(word1, "debug") == 0)
    {
      inventory[9] = true;
      puts("inventory[9] has been set to true.");
    }

    // debug1 - fill inventory
    else if (strcmp(word1, "debug1") == 0)
    {
      int i;
      for(i = 0; i<5; i++)
      {
        inventory[i] = true;
      }
      puts("All items added to inventory.");
    }

    // dumb parser
    else
    {
      printf("\nI have no idea how to %s, but knock yourself out.\n", word1);
    }

    // if inventory indeces 0-4 true, 5Found condition flagged true
    if(inventory[0] && inventory[1] && inventory[2] && inventory[3] && inventory[4])
    {
      inventory[9] = true;
    }

    // final phase of game
    if (inventory[9] == true)
    {
      rewind(writePointer);
      fprintf(writePointer, "CHILI CON CARNE\n\nType 'IM READY' when you're ready to cook\n\n");
      fflush(writePointer);
      puts("You found all 5 notes, completing the recipe!! Great job!\n\nIf you haven't already - start collecting your ingredients.\nOnce you've collected all 5 ingredients, type:\n\nIM READY\n\nto complete your final challenge!");
      inventory[9] = false; // to avoid repeat triggers but I think this just resets lol oh well
    }

    //user input validation - null strings
    if (word2 == NULL)
    {
      word2 = "(empty)";
    }
    
    if (word1 == NULL)
    {
      word1 = "(empty)";
    }

  }
  return 1;
}