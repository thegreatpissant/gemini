/**
 * Sample for a very simple curses window.
 * Can be used to determine getch() integer values for keys.
 */

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string>

int main(int argc, char** argv, char** envp){
   // The curses window
   WINDOW* win;

   // Needed integers
   int key = 0;
   bool go = true;

   // String
   std::string temp = "0";

   // Set up some curses defaults
   win = initscr();  // Init the screen
   cbreak();         // Break from tty mode
   noecho();         // Don't echo the user input
   curs_set(0);      // Make the cursor invisible (if supported)
 
   /* Simulation loop */
   while(go){
     erase();              // Clear the screen
     temp[0] = key;
//     snprintf(temp, 14, "%d", key);
     mvaddstr(2, 2, temp.c_str());

     key = getch();  // Function will block until a key is hit!

     if(key != -1){  // Valid input
       if(key == 127){  // Backspace was hit!
         go = false;   // End the loop
       }
     }
   }  /* End simulation loop */
 
   /* End the curses window */
   erase();
   endwin();

   return 0;
}
