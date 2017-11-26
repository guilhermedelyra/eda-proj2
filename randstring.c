#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ncurses.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>

#define malc(x) (x *) malloc(sizeof(x))

char *randstring(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyz'";        
    char *randomString = NULL;
    unsigned int n;
    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (n = 0; n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

unsigned int PJWHash (const char* str, unsigned int length) {
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(long int) * 8);
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   const unsigned int HighBits          =
                      (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash = 0;
   unsigned int test = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash << OneEighth) + (*str);

      if ((test = hash & HighBits) != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash;
}


int main(){
	FILE *fp = fopen("randstring.txt", "w");
	int i;
	srand(time(NULL) * 45 + 25011984);
	for(i = 0; i < 15; ++i) {
		fprintf(fp, "%u\n", PJWHash(randstring(45), 45));
	}
	fclose(fp);
	return 0;
}