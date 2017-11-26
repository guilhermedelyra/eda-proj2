#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ncurses.h>
#include <sys/resource.h>
#include <sys/time.h>

#define malc(x) (x *) malloc(sizeof(x))
#define nl ; printf("\n")
#define max(a, b) ((a > b) ? (a) : (b))

#define NOME_DICIONARIO "dicioPadrao"
#define SUCESSO                 0
#define NUM_ARGS_INCORRETO      1
#define ARQDICIO_NAOCARREGADO   2
#define ARQTEXTO_ERROABERTURA   3
#define ARQTEXTO_ERROLEITURA    4
#define ERRO_DICIO_NAOCARREGADO 5
#define TAM_MAX                 45

typedef struct _link {
	char pal[65];
	bool present, collision;
	char *pal[]
} link; link a[1049939];





unsigned int PJWHash (const char* str, unsigned int length);
bool carregaDicio (char * filename);
double calcula_tempo (const struct rusage *b, const struct rusage *a);


int main () {
	FILE *fp = fopen("hashs.txt", "w");
	unsigned int i = 0, rs, hesh = 6381921;

	carregaDicio ("dicioPadrao");

	for(i = 0; i < 1049939; ++i) {
		rs = PJWHash (a[i].pal, strlen(a[i].pal));
		fprintf(fp, "%u\n", rs);
		if (rs == hesh) {
			printf ("[%s]\n", a[i].pal);
		}
	}

	fclose(fp);


	return 0;
}


double calcula_tempo (const struct rusage *b, const struct rusage *a) {
    if (b == NULL || a == NULL)
        return 0;
    else
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
}


bool carregaDicio (char * filename) {
	FILE *fp = fopen(filename, "r");
	long int i = 0;
	char c[65];

	if (fp == NULL) return false;

	while (fscanf(fp, "%s", c) > 0) {
		strcpy(a[i++].pal, c);
	}

	fclose(fp);

	return true;
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

