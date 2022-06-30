/**
 * COSC 3250 - Project # 3
 * This is the method of our os that will allow us to read in characters as well as manage the flag register so as to print chars out when dr is not busy.
 * @Salvador Guel and Sierra Thomas
 * Dr. Dennis Brylow
 * TA-BOT:MAILTO salvador.guel@marquette.edu sierra.thomas@mu.edu 
 */

/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN 127

char* decoder(char *chars, char *dmessage, int length, int newkey) {

        int c, last = 0, key = newkey, cnew = 0, temp = 0, temp2 = 0, i;

        for (i = 0; i < length + 1; i++) {
                c = chars[i];

                if (temp2<0){
                 temp2 = 0;
                 last = temp2;
                }

                else {
                 last = temp2;
                }

                if (c == 32) {
                 c = 0;
                 cnew = 0;
                }

                else {
                 cnew = (toupper(c)-64);
                }

                if((cnew-last-key)<0) {
                 temp = 27 - abs(cnew-last-key);
                }

                else {
                 temp = (abs(cnew-last-key))%27;
                }

                cnew = temp + 64;
                temp2 = (toupper(c)-64);

                if (cnew < 65 || cnew > 90) {
                 dmessage[i] = 32;
                }

                else {
                 dmessage[i] = cnew;
                }
        } //End of For loop
dmessage[i] = '\0';
return dmessage;
} //End of Decoder Method

int counte (char *chars, int length) {
        int count = 0;
        int i;
//This for loop loops through the characters and increases a count as the char it is looking at is e. Thus the count variable returned as "count" is the number of es present in the "string" it received.
        for(i = 0; i < length; i++) {
         char c = tolower(chars[i]);

                if (c == 'e') {
                 count++;
                }
        } //End of For loop
return count;
} //End of counte method

/**
 * Main process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.
 */
process main(void)
{
   kprintf("Hello Xinu World!\n");

  kprintf("Enter your codebreaker text:\n");
    // TODO: Put your main program for codebreaker here.
    //       Replace any calls to printf() with kprintf().
    //       Replace any calls to getchar() with kgetc().

	char characters[MAXLEN + 1];
        char output[MAXLEN + 1];
        int ecount[MAXLEN + 1];
	
        int key = 0, index = 0, vowelcount, guessedkey = 0, max1 = 0, max2 = 0, i;
        char c = kgetc();

                while ((c != EOF) && (index <= MAXLEN)) {
                 characters[index] = c;
                 index++;
                 c = kgetc();
                }

                characters[index] = '\0';

                        for (i = 0; i <= 27; i++) {
                         decoder(characters, output, index, key);
                         vowelcount = counte(output, index);
                         ecount[i] = vowelcount;
                         	//kprintf("The key value is: %d \n", key);
				//kprintf("Number of e: %d \n", vowelcount);
				//kprintf("%s \n", output);

			 key++;
			}
//Loop through i = 0 to i = 27. While doing this, use the current i at the time to see if at that index i if the value of what is in ecount (this is the number of es present in the decode message) is greater than the current value of max 1, if it is then that value if set as max1.
	for (i = 0; i < 27; i++) {
		if (ecount[i] >= max1){
			max1 = ecount[i];
		}
	}

	for (i=0; i < 27; i++) {
		if ((ecount[i] >= max2) && (ecount[i] < max1)) {
			max2 = ecount[i];
		}
	}
	
	int firstTrue = 1;

	for (i = 0; i < 27; i++) {
		if (ecount[i] >= max2 && firstTrue == 1) {
				guessedkey = i;
				firstTrue = 0;
		}
	}

//print out the probable key which is saved in guessed key and we then decode the message with our guessed key and print it out.
        kprintf("Most probable key is %d \n", guessedkey);
        decoder(characters, output, index, guessedkey);
	kprintf("%s \n", output);
	kprintf("\n");
	kprintf("===TEST END===");

    while (1)
        ;

    return 0;
}
