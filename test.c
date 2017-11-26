#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ncurses.h>
#include <sys/resource.h>
#include <sys/time.h>

typedef struct _char {
	char a[46];
	bool present, collision;
	struct _char *c[1];
} ch; ch *b[27][1] = {NULL};


unsigned int PJWHash (const char* str, unsigned int length);
bool carregaDicio (char * filename);
double calcula_tempo (const struct rusage *b, const struct rusage *a);


int main(){
	bool temp;
	b['a' - 97][0] = (ch *) malloc(sizeof(ch));
	b[0][0]->present = true;
	strcpy(b['a' - 97][0]->a, "testetestetestetestetestetestetestetesteteste");
	printf("%s\n", b['a' - 97][0]->a);

	b['a' - 97][0]->c[0] = (ch *) malloc(sizeof(ch));

	strcpy(b['a' - 97][0]->c[0]->a, "cocoacocoacocoacocoacocoacocoacocoacocoacocoa");
	printf("%s\n", b['a' - 97][0]->c[0]->a);

	temp = b[0][123123]->present;
	return 0;
}


bool carregaDicio (char *filename) {
	FILE *fp = fopen(filename, "rt");
	char c[46];

	if (fp == NULL) return false;

	while (fscanf(fp, "%s", c) > 0) {
		b[c[0]-97][PJWHash(c, strlen(c))]->present = true;
	}

	fclose(fp);

	return true;
}


unsigned int PJWHash (const char* str, unsigned int length) {
   
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(long int) * 8);
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   
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
