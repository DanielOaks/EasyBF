/********************************************************
****  #######                        #######   RC 1  ****
**** ##                             ##     ##        ****
**** ##         ####  #####  #   #  ##     ##  ##### ****
**** ######    ## ## ##     ##  ##  ########  ##     ****
**** ##       ######  ####   ####   ##     ## #####  ****
**** ##       ##  ##     ##   ##    ##     ## ##     ****
**** ######## ##  ## #####    ##    ########  ##     ****
*********************************************************
**** EasyBf **** An EasyBrainfuck Interpreter ***********
*******************************************************************
** TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION  **
**************** EasyBf Copyright Danneh 2009-2010 ****************
* Redistribution  and  use  in  source and  binary forms, with or *
* without modification, are permitted provided that the following *
* conditions are met:                                             *
*                                                                 *
* * Redistributions may not be sold, nor may they be used in a    *
*    commercial product or activity.                              *
* * Redistributions in source code and/or in binary form must     *
*    reproduce the above copyright notice, this list of           *
*    conditions and the following disclaimer in the documentation *
*    and/or other materials provided with the distribution.       *
*                                                                 *
* This program is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY,  EXPRESS OR IMPLIED; it is limited to *
* PRIVATE USE  and NOT SUBJECT TO  ANY FORM OF BINDING AGREEMENTS *
*******************************************************************

**********************
**** EasyBf Notes ****

***** Testing Builds ****
The program test.ebf is used to test builds of EasyBf,
and can also be used to test other interpreters

***** Compiling ****
EasyBf must be compiled in C99 mode:
 gcc main.c -std=c99 -o easybf

***** Program Length ****
EasyBF programs can only be 65534 operators long
 This may be adusted changing the  #define MAXARRAY,
  though the unsigned int file_data.i may need to be
  changed to alternate datatype (long, short int) in
  order to handle  indexing the new array dimentions

***** Errors, Modifications, etc ****
If you want to drop me a line about any errors,
 modifications, or things you've coded up, then just
 send me an email at <dan11999922@hotmail.com>
 
Hope you have fun with this language!

***************************************************/
#include <stdio.h>
#include <stdlib.h> //atoi
#include <string.h> //strcmp
#include <ctype.h>  //isdigit, isalpha, isspace

//> Functions
enum helpSection {
   help_help, help_specs, help_license, help_programerror, help_specialerror
};
void help(int section);

//> Variables/Structures
#define MAXARRAY 65534
int flagDebug = 0, skipGet = 0;

struct {
   unsigned int i; //Index
   char chars[MAXARRAY];
   int ints[MAXARRAY];
} file_data;

struct data {
   int i; //Index
   char chars[MAXARRAY];
   int ints[MAXARRAY];
} prog_data;

int main(int argc, char *argv[])
{
   ////////////////////////
   //> Initialization <<//
   printf("\nEasyBf - RC1");

   //> Flags
   // Loops through Argument, checking
   // to see if they match any recognised
   // flag identifiers
   for(int i = 1; i < argc; i++) {

      //  -d  Debug
      if(!strcmp(argv[i], "-d")) {
         argc--; //for later Args Check
         flagDebug = 1;
      }

      //  -h  Help
      else if(!strcmp(argv[i], "-h")) {
         printf(" - Help\n");
         help(help_help);
         return 0;
      }

      //  -l  Licence
      else if(!strcmp(argv[i], "-l")) {
         printf(" - Licensing\n");
         help(help_license);
         return 0;
      }

      //  -s  Specifications
      else if(!strcmp(argv[i], "-s")) {
         printf(" - Language Specs\n");
         help(help_specs);
         return 0;
      }
   }

   //> Args Check
   //Checks number of args only
   //Invalid args will be handeled
   // at the file-handler
   if(argc < 2) {
      printf("\n");
      help(help_help);
      return 1;
   }

   if(flagDebug) {
      printf(" - Debug\n");
   } else {
      printf("\n");
   }

    ///////////////////////
   //> File Operations <//

   //> Initialising File/Program Arrays
   // Clears data arrays and
   // initialises Index vars
   for(int i = 0; i < MAXARRAY; i++) {
      file_data.chars[i] = '\0';
      file_data.ints[i] = 0;
      prog_data.chars[i] = ' ';
      prog_data.ints[i] = 0;
   }
   file_data.i = 0;
   prog_data.i = 0;
   
   //-------------------//---------------------------------------------//
   // NEGATIVE INDEXING // PROG_DATA INDEX MANAGMENT                   //
   //-------------------//---------------------------------------------//
   // INDEX (user view) // -0.5*MAXARRAY -       0      - 0.5*MAXARRAY //
   // ARRAY (data view) //        0      - 0.5*MAXARRAY -   MAXARRAY   //
   //-------------------//---------------------------------------------//
   
   #define PUTS_I (prog_data.i + (int) (MAXARRAY / 2))
   #define ADDS_I ((int) (MAXARRAY / 2))
   //PUTS_I : Value for current prog_data index
   //ADDS_I : Offset Value to add for prog_data index

   //> File Opening
   FILE *fp;
   fp = fopen(argv[1], "r");
   if(fp == NULL) {
      help(help_programerror);
      return 1 ;
   }

   //> File Parsing
   // Read single chars into file_data
   // until a commenter (#) is reached,
   // then skip ahead to the next line
   char fileChar;                // input/temp char
   char fileCharArray[MAXARRAY]; // input/temp array
   while(1) {
      if(skipGet) {
         skipGet = 0;
      } else {
         fileChar = fgetc(fp);
      }

      if(fileChar == EOF)
         break;

      //  #  Commenter
      if(fileChar == '#') {
         do {
            fileChar = fgetc(fp);
         } while(fileChar != '\n');
      }

      //  @() !()  Special Modifiers
      else if((fileChar == '@') || (fileChar == '!')) {
         file_data.chars[file_data.i] = fileChar;
         
         //Position at start of numbers
         do {
            fileChar = fgetc(fp);
            if(isspace(fileChar)) {
               //continue, to get to brace
            } else if(fileChar == '(') {
               break;
            } else {
               help(help_specialerror);
               return 0;
            }
         } while(1);

         for(int i = 0; ; i++) {
            fileChar = fgetc(fp);
            fileCharArray[i] = fileChar;
            if(fileChar == ')')
               break;
         }

         file_data.ints[file_data.i] = atoi(fileCharArray);
         file_data.i++;
         for(int i = 0; i < MAXARRAY; i++) {
            fileCharArray[i] = '\0';
         }
      }

      //  0-9  Numbers
      else if(isdigit(fileChar)) {

         for(int i = 0;  ; i++) {
            fileCharArray[i] = fileChar;
            fileChar = fgetc(fp);
            if(!isdigit(fileChar)) {
               break;
            }
         }
         skipGet = 1;
         file_data.ints[file_data.i] = atoi(fileCharArray);
         file_data.chars[file_data.i] = ' ';
         file_data.i++;
         for(int i = 0; i < MAXARRAY; i++) {
            fileCharArray[i] = '\0';
         }
      }

      //  ( )  Whitespace
      else if(isspace(fileChar)) {
      }

      //  >A.  Program Data, Letters
      else {
         file_data.chars[file_data.i++] = fileChar;
      }
   }
   fclose(fp);

    //////////////////////////
   //> Program Operations <//

   //> Printing
   // Print current program to screen
   printf("Program:");
   for(file_data.i = 0; file_data.chars[file_data.i] != '\0'; file_data.i++) {
      printf(" ");
      if(file_data.chars[file_data.i] != ' ') {

         //  @()  Special Modifier
         if(file_data.chars[file_data.i] == '@') {
            printf("@(%i)", file_data.ints[file_data.i]);

            //  !()  Special Modifier
         } else if(file_data.chars[file_data.i] == '!') {
            printf("!(%i)", file_data.ints[file_data.i]);

            //  (>)  Program Data, Letters, Numbers
         } else {
            printf("%c", file_data.chars[file_data.i]);
         }

      } else {
         printf("%i", file_data.ints[file_data.i]);
      }
   }
   printf("\n");
   file_data.i = 0; //Start parsing at first character

   //> Program Parsing
   //01-GET statement FROM ebf_program
   //02-EXECUTE statement
   //03-GOTO 01
   if(flagDebug) { // DebugStart
      printf(" || prog_data.i(%i)\n", prog_data.i);
   }               // DebugEnd

   do {

      //  >  Memory Index Pointer Up
      if(file_data.chars[file_data.i] == '>') {
         prog_data.i++;
         if(flagDebug) { // DebugStart
            printf(">|| prog_data.i(%i)\n", prog_data.i);
         }               // DebugEnd
      }
      //  <  Memory Index Pointer Down
      else if(file_data.chars[file_data.i] == '<') {
         prog_data.i--;
         if(flagDebug) { // DebugStart
            printf("<|| prog_data.i(%i)\n", prog_data.i);
         }               // DebugEnd
      }


      //  +  Arithmetic Add
      else if(file_data.chars[file_data.i] == '+') {
         if(prog_data.chars[PUTS_I] == ' ') {
            prog_data.ints[PUTS_I] += prog_data.ints[PUTS_I+1];
         } else {
            prog_data.chars[PUTS_I] += prog_data.ints[PUTS_I+1];
         }
         if(flagDebug) { // DebugStart
            printf("+|| prog_data.i(%i) ", prog_data.i);
            if(prog_data.chars[PUTS_I] == ' ') {
               printf("is now %i\n", prog_data.ints[PUTS_I]);
            } else {
               printf("is now %c\n", prog_data.chars[PUTS_I]);
            }
         }               // DebugEnd
      }
      //  -  Arithmetic Subtract
      else if(file_data.chars[file_data.i] == '-') {
         if(prog_data.chars[PUTS_I] == ' ') {
            prog_data.ints[PUTS_I] -= prog_data.ints[PUTS_I+1];
         } else {
            prog_data.chars[PUTS_I] -= prog_data.ints[PUTS_I+1];
         }
         if(flagDebug) { // DebugStart
            printf("-|| prog_data.i(%i) ", prog_data.i);
            if(prog_data.chars[PUTS_I] == ' ') {
               printf("is now %i\n", prog_data.ints[PUTS_I]);
            } else {
               printf("is now %c\n", prog_data.chars[PUTS_I]);
            }
         }               // DebugEnd
      }
      //  *  Arithmetic Multiply
      else if(file_data.chars[file_data.i] == '*') {
         if(prog_data.chars[PUTS_I] == ' ') {
            prog_data.ints[PUTS_I] *= prog_data.ints[PUTS_I+1];
         } else {
            prog_data.chars[PUTS_I] *= prog_data.ints[PUTS_I+1];
         }
         if(flagDebug) { // DebugStart
            printf("*|| prog_data.i(%i) ", prog_data.i);
            if(prog_data.chars[PUTS_I] == ' ') {
               printf("is now %i\n", prog_data.ints[PUTS_I]);
            } else {
               printf("is now %c\n", prog_data.chars[PUTS_I]);
            }
         }               // DebugEnd
      }
      //  /  Arithmetic Divide
      else if(file_data.chars[file_data.i] == '/') {
         if(prog_data.chars[PUTS_I] == ' ') {
            prog_data.ints[PUTS_I] /= prog_data.ints[PUTS_I+1];
         } else {
            prog_data.chars[PUTS_I] /= prog_data.ints[PUTS_I+1];
         }
         if(flagDebug) { // DebugStart
            printf("/|| prog_data.i(%i) ", prog_data.i);
            if(prog_data.chars[PUTS_I] == ' ') {
               printf("is now %i\n", prog_data.ints[PUTS_I]);
            } else {
               printf("is now %c\n", prog_data.chars[PUTS_I]);
            }
         }               // DebugEnd
      }


      //  .  Memory Print
      else if(file_data.chars[file_data.i] == '.') {
         if(flagDebug) { // DebugStart
            printf(".||  ");
         }               // DebugEnd
         if(prog_data.chars[PUTS_I] == ' ') {
            printf("%i ", prog_data.ints[PUTS_I]);
         } else {
            printf("%c", prog_data.chars[PUTS_I]);
         }
         if(flagDebug) { // DebugStart
            printf("\n");
         }               // DebugEnd
      }
      
      
      //  ,  Memory Input
      else if(file_data.chars[file_data.i] == ',') {
         while(1) {
            if(flagDebug) { // DebugStart
               printf(",||  ");
            }               // DebugEnd
            char input[100];
            int inputnum = 0;
            fgets(input, sizeof(input), stdin);
            if(isalpha(input[0])) {
               prog_data.ints[PUTS_I]  = 0;
               prog_data.chars[PUTS_I] = input[0];
               break;
            } else if(sscanf(input, "%d", &inputnum)) { //is number
               prog_data.ints[PUTS_I]  = inputnum;
               prog_data.chars[PUTS_I] = ' ';
               break;
            } else {
               printf("  Unknown Value :");
            }
         }
      }


      //  [  Skip Entry
      else if(file_data.chars[file_data.i] == '[') {

         //Current Index is 0
         if((prog_data.chars[PUTS_I] == ' ') && (prog_data.ints[PUTS_I] == 0)) {
            if(flagDebug) { // DebugStart
               printf("[|| Skipped\n");
               printf("]|| Skipped\n");
            }               // DebugEnd

            //> Matching Bracket Search
            // Searches back to find matching ']'
            int skipIndex = 0; //for nested brackets
            while(1) {
               if(file_data.chars[file_data.i] == '[') {
                  skipIndex++;
               } else if(file_data.chars[file_data.i] == ']') {
                  skipIndex--;
                  if(skipIndex == 0) {
                     break;
                  }
               }
               file_data.i++;
            }       

         //Current Index is not 0, or is not a number
         } else {
            if(flagDebug) { // DebugStart
               printf("[|| Skip Entered\n");
            }               // DebugEnd
         }
      }
      //  ]  Skip Exit
      else if(file_data.chars[file_data.i] == ']') {
         if(flagDebug) { // DebugStart
            printf("]|| Skip Exited\n");
         }               // DebugEnd
      }


      //  {  Loop Entry
      else if(file_data.chars[file_data.i] == '{') {
         if(flagDebug) { // DebugStart
            printf("{|| Loop Entered\n");
         }               // DebugEnd
      }
      //  }  Loop Exit
      else if(file_data.chars[file_data.i] == '}') {
         if((prog_data.ints[PUTS_I] != 0) && (prog_data.chars[PUTS_I] == ' ')) {
            if(flagDebug) { // DebugStart
               printf("}|| Loop Reentered\n");
            }               // DebugEnd

            //> Matching Loop Search
            // Searches back to find matching '{'
            int loopIndex = 0; //for nested loops
            while(1) {
               if(file_data.chars[file_data.i] == '}') {
                  loopIndex++;
               } else if(file_data.chars[file_data.i] == '{') {
                  loopIndex--;
                  if(loopIndex == 0) {
                     file_data.i--;
                     break;
                  }
               }
               file_data.i--;
            }

         } else {
            if(flagDebug) { // DebugStart
               printf("}|| Loop Exited\n");
            }               // DebugEnd
         }
      }


      //  a-Z  Entry-Characters
      else if(isalpha(file_data.chars[file_data.i])) {
         prog_data.ints[PUTS_I] = 0;
         prog_data.chars[PUTS_I] = file_data.chars[file_data.i];

         if(flagDebug) { // DebugStart
            printf("C|| prog_data.i(%i) ", prog_data.i);
            printf("is now \'%c\'\n", file_data.chars[file_data.i]);
         }               // DebugEnd
      }
      //  0-9  Entry-Numbers
      else if(file_data.chars[file_data.i] == ' ') {
         prog_data.ints[PUTS_I] = file_data.ints[file_data.i];
         prog_data.chars[PUTS_I] = ' ';

         if(flagDebug) { // DebugStart
            printf("N|| prog_data.i(%i) ", prog_data.i);
            printf("is now %i\n", file_data.ints[file_data.i]);
         }               // DebugEnd
      }


      //  @()  Special Modifier
      else if(file_data.chars[file_data.i] == '@') {
         if(prog_data.chars[PUTS_I] == ' ') {
            prog_data.ints[file_data.ints[file_data.i] + ADDS_I] = prog_data.ints[PUTS_I];
            prog_data.chars[file_data.ints[file_data.i] + ADDS_I] = ' ';
         } else {
            prog_data.ints[file_data.ints[file_data.i] + ADDS_I] = 0;
            prog_data.chars[file_data.ints[file_data.i] + ADDS_I] = prog_data.chars[PUTS_I];
         }

         if(flagDebug) { // DebugStart
            printf("@|| prog_data.i(%i) ", file_data.ints[file_data.i]);
            printf("is now %i\n", prog_data.ints[PUTS_I]);
         }               // DebugEnd
      }
      //  !()  Special Modifier
      else if(file_data.chars[file_data.i] == '!') {
         prog_data.i = file_data.ints[file_data.i];

         if(flagDebug) { // DebugStart
            printf("!|| prog_data.i(%i)\n", prog_data.i);
         }               // DebugEnd
      }


      //  ???  Invalid
      else {
         if(flagDebug) { // DebugStart
            printf("%c|| INVALID OPERAND\n", file_data.chars[file_data.i]);
         }               // DebugEnd
      }

      file_data.i++; //Get the next statement
   } while(file_data.chars[file_data.i] != '\0');

    ///////////////////
   //> Program End <//
   if(!flagDebug) {
      printf("\n");
   }
   printf("\n");

   return 0 ;
}

 /////////////////////
//> Help Function <//
// Tells the user information from a variety of sections
//
//> Variables
// name     description
// section  Section to Display
void help(int section)
{
   //Default Help
   if(section == help_help) {
      printf("SYNOPSIS: easybf [FLAGS] <EasyBrainfuck File>\n");
      printf("\n");
      printf("FLAGS:\n");
      printf("   -h\n");
      printf("      Help, displays this message\n");
      printf("   -d\n");
      printf("      Debug Mode, verbose, outputs all important debug information\n");
      printf("   -l\n");
      printf("      License, shows licensing and author info\n");
      printf("   -s\n");
      printf("      Specs, shows EasyBrainfuck Language Specifications\n");
      printf("\n");
      printf("NOTES:\n");
      printf("   Programs can only be %d characters long\n", MAXARRAY);
      printf("   Check source code for more information\n");
      printf("\n");
      printf("REPORTING BUGS:\n");
      printf("   Report bugs to <dan11999922@hotmail.com>\n");
      printf("   Switch on the debug (-d) flag when showing program outputs\n");
   }

   //EasyBrainfuck Specifications
   else if(section == help_specs) {
      printf("\n");
      printf("EASYBRAINFUCK SPECIFICATIONS:\n");
      printf("   >    | Move the memory index pointer plus one higher\n");
      printf("   <    | Move the memory index pointer minus one lower\n");
      printf("         \n");
      printf("   .    | Print data from the Current Index to the screen\n");
      printf("        |  prints a single space after numbers\n");
      printf("   ,    | Input data from the user to the Current Index\n");
      printf("   \"    | Strings within double-quotes are printed directly\n");
      printf("           to the screen, newlines represented as actual line breaks\n");
      printf("         \n");
      printf("   0-9  | Add integers to the memory at the Current Index\n");
      printf("        |  (Can go from 0 to max cell value of the implimentation)\n");
      printf("   a-Z  | Add characters to the memory at the Current Index\n");
      printf("         \n");
      printf("   +    | Add Index+1 to the Current Index, store in Current Index\n");
      printf("   -    | Minus Index+1 from the Current Index, store in Current Index\n");
      printf("   *    | Multiply Index+1 with the Current Index, store in Current Index\n");
      printf("   /    | Divide Index+1 from the Current Index, store in Current Index\n");
      printf("         \n");
      printf("   @(#) | Copy the Current Index to index of #\n");
      printf("        |  (@(33) copies to the thirty-third memory index)\n");
      printf("   !(#) | Change memory index pointer to the one specified in #\n");
      printf("        |  (!(33) moves the memory index pointer to the 33rd place)\n");
      printf("         \n");
      printf("   {}   | Loops, if Current Index is 0, break loop\n");
      printf("   []   | If Current Index is 0, skip between the brackets\n");
      printf("   #    | Comment out the current line to the end\n");
      printf("   \n");
      printf("   ** Operators, cont ****\n");
      printf("    / -Divides are to round as accuratly as possible to integer values\n");
      printf("   \n");
      printf("   ** Cells ****\n");
      printf("    The number of cells is implementation-defined, though should\n");
      printf("     have at least from -900 to 900\n");
      printf("    \n");
      printf("    Cells are able to contain the values a-z, A-Z, and Integers\n");
      printf("    Integer values should range from at least -128 to 128,\n");
      printf("     higher values are recommended\n");
      printf("   \n");
      printf("   ** Brackets ****\n");
      printf("    If a program contains unbalanced brackets, the program's behavior\n");
      printf("     is to be implementation-defined, printing an error message,\n");
      printf("     attempting to run the program anyways, or other actions\n");
      printf("   \n");
      printf("   ** Language Examples ****\n");
      printf("    :Hello World:\n");
      printf("    Program: H>e>l>o>W>r>d<<<<<<.>.>..>.>.<.>>.<<<.>>>>.\n");
      printf("    Output : HelloWorld\n");
      printf("   \n");
      printf("    :Countdown:\n");
      printf("    Program: 9>1<{.-}B.l.a.s.t.o.f..\n");
      printf("    Output : 9 8 7 6 5 4 3 2 1 Blastoff\n");
      printf("   \n");
      printf("    :Fibonacci Sequence:\n");
      printf("    Program: 11>1>0>0>1.!(0){>>>+@(2)>@(3)<<.@(4)<<-}\n");
      printf("    Output : 1 1 2 3 5 8 13 21 34 55 89 144 \n");
   }

   //Licensing and Author info
   else if(section == help_license) {
      printf("LICENCE:\n");      printf("   *******************************************************************\n");      printf("   ** TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION  **\n");      printf("   **************** EasyBf Copyright Danneh 2009-2010 ****************\n");      printf("   * Redistribution and use in source and binary forms, with or      *\n");      printf("   * without modification, are permitted provided that the following *\n");      printf("   * conditions are met:                                             *\n");      printf("   *                                                                 *\n");      printf("   * * Redistributions may not be sold, nor may they be used in a    *\n");      printf("   *    commercial product or activity.                              *\n");      printf("   * * Redistributions in source code and/or in binary form must     *\n");      printf("   *    reproduce the above copyright notice, this list of           *\n");      printf("   *    conditions and the following disclaimer in the documentation *\n");      printf("   *    and/or other materials provided with the distribution.       *\n");      printf("   *                                                                 *\n");      printf("   * This program is distributed in the hope that it will be useful, *\n");      printf("   * but WITHOUT ANY WARRANTY,  EXPRESS OR IMPLIED; it is limited to *\n");      printf("   * PRIVATE USE  and NOT SUBJECT TO  ANY FORM OF BINDING AGREEMENTS *\n");      printf("   *******************************************************************\n");
      printf("\n");
      printf("AUTHOR:\n");
      printf("   Coded by Danneh <dan11999922@hotmail.com>\n");
      printf("   \n");
      printf("   Thanks to the following people:\n");
      printf("      Urban Müller for the original Brainfuck\n");
      printf("      JiveMasta for inspiring this project,\n");
      printf("         and helping spec the language\n");
      printf("      ZeekyHBomb for helping fix the screwy code,\n");
      printf("         and advice speccing this\n");
      printf("      t0rento for writing up an interpreter,\n");
      printf("         and for inspiting the help layout\n");
      printf("      Eudoxia for helping test this and being awesome\n");
      printf("      and everyone else who's helped write this language,\n");
      printf("      or shown interest in the project\n");
   }

   //Couldn't open file
   else if(section == help_programerror) {
      printf("Could not open program\n");
   }

   //Special Character (! @) Reading Error
   else if(section == help_specialerror) {
      printf("Error with ! or @ attributes\n");
   }

   //Invalid section
   else {
      printf("Invalid Help Section: %i\n", section);
      printf("Report bugs to <dan11999922@hotmail.com>\n");
      printf("Please switch on the debug (-d) flag when showing error outputs\n");
   }

   printf("\n");
}
