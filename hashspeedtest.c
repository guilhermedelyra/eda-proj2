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
} link; link a[14];


int compare (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

unsigned int RSHash (const char* str, unsigned int length);
unsigned int JSHash (const char* str, unsigned int length);
unsigned int PJWHash (const char* str, unsigned int length);
unsigned int ELFHash (const char* str, unsigned int length);
unsigned int BKDRHash (const char* str, unsigned int length);
unsigned int SDBMHash (const char* str, unsigned int length);
unsigned int DJBHash (const char* str, unsigned int length);
unsigned int DEKHash (const char* str, unsigned int length); 
unsigned int APHash (const char* str, unsigned int length);

bool carregaDicio (char * filename);
double calcula_tempo (const struct rusage *b, const struct rusage *a);


int main () {
	FILE *fp;
	struct rusage tempo_inicial, tempo_final;
	double RS = 0.0, JS = 0.0, PJW = 0.0, ELF = 0.0, BKDR = 0.0, SDBM = 0.0, DJBH = 0.0, DEKH = 0.0, APH = 0.0;
	unsigned int i = 0, rs[14];

	carregaDicio ("dicioLeve");

	getrusage(RUSAGE_SELF, &tempo_inicial);
	   for(i = 0; i < 13; ++i) {
	   		rs[i] = RSHash (a[i].pal, strlen(a[i].pal));
	   	}
	getrusage(RUSAGE_SELF, &tempo_final);
	RS = calcula_tempo(&tempo_inicial, &tempo_final);

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/rs.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	   for(i = 0; i < 13; ++i) {
	   		rs[i] = JSHash (a[i].pal, strlen(a[i].pal));
	   	}

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/js.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	   for(i = 0; i < 13; ++i) {
	   		rs[i] = PJWHash (a[i].pal, strlen(a[i].pal));
	   	}

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/pjw.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	getrusage(RUSAGE_SELF, &tempo_inicial);
	   for(i = 0; i < 13; ++i) {
	   		rs[i] = ELFHash (a[i].pal, strlen(a[i].pal));
	   	}
	getrusage(RUSAGE_SELF, &tempo_final);
	ELF = calcula_tempo(&tempo_inicial, &tempo_final);

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/elf.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	getrusage(RUSAGE_SELF, &tempo_inicial);
	   for(i = 0; i < 13; ++i) {
	   		rs[i] = BKDRHash (a[i].pal, strlen(a[i].pal));
	   	}
	getrusage(RUSAGE_SELF, &tempo_final);
	BKDR = calcula_tempo(&tempo_inicial, &tempo_final);

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/bkdr.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	getrusage(RUSAGE_SELF, &tempo_inicial);
	   for(i = 0; i < 13; ++i) {
	   		rs[i] = SDBMHash (a[i].pal, strlen(a[i].pal));
	   	}
	getrusage(RUSAGE_SELF, &tempo_final);
	SDBM = calcula_tempo(&tempo_inicial, &tempo_final);

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/sdbm.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	getrusage(RUSAGE_SELF, &tempo_inicial);
	   for(i = 0; i < 13; ++i) {
	   		rs[i] = DJBHash (a[i].pal, strlen(a[i].pal));
	   	}
	getrusage(RUSAGE_SELF, &tempo_final);
	DJBH = calcula_tempo(&tempo_inicial, &tempo_final);

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/djbh.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	getrusage(RUSAGE_SELF, &tempo_inicial);
	   for(i = 0; i < 13; ++i) {
	   		rs[i] = DEKHash (a[i].pal, strlen(a[i].pal));
	   	}
	getrusage(RUSAGE_SELF, &tempo_final);
	DEKH = calcula_tempo(&tempo_inicial, &tempo_final);

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/dekh.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	getrusage(RUSAGE_SELF, &tempo_inicial);
	   for(i = 0; i < 13; ++i) {
	   		rs[i] = APHash (a[i].pal, strlen(a[i].pal));
	   	}
	getrusage(RUSAGE_SELF, &tempo_final);
	APH = calcula_tempo(&tempo_inicial, &tempo_final);

	
	qsort(rs, 13, sizeof(unsigned int), compare);
	fp = fopen("hashresults/aph.txt", "w");


	for(i = 0; i < 13; ++i) fprintf(fp, "%u\n", rs[i]);
	fclose(fp);

	printf("RS %.5f\n", RS);
	printf("JS %.5f\n", JS);
	printf("PJW %.5f\n", PJW);
	printf("ELF %.5f\n", ELF);
	printf("BKDR %.5f\n", BKDR);
	printf("SDBM %.5f\n", SDBM);
	printf("DJBH %.5f\n", DJBH);
	printf("DEKH %.5f\n", DEKH);
	printf("APH %.5f\n", APH);

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


unsigned int RSHash (const char* str, unsigned int length) {
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = hash * a + (*str);
      a    = a * b;
   }

   return hash;
}

unsigned int JSHash (const char* str, unsigned int length) {
   unsigned int hash = 1315423911;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash ^= ((hash << 5) + (*str) + (hash >> 2));
   }

   return hash;
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

unsigned int ELFHash (const char* str, unsigned int length) {
   unsigned int hash = 0;
   unsigned int x    = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash << 4) + (*str);

      if ((x = hash & 0xF0000000L) != 0)
      {
         hash ^= (x >> 24);
      }

      hash &= ~x;
   }

   return hash;
}

unsigned int BKDRHash (const char* str, unsigned int length) {
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash * seed) + (*str);
   }

   return hash;
}

unsigned int SDBMHash (const char* str, unsigned int length) {
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (*str) + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}

unsigned int DJBHash (const char* str, unsigned int length) {
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = ((hash << 5) + hash) + (*str);
   }

   return hash;
}

unsigned int DEKHash (const char* str, unsigned int length) {
   unsigned int hash = length;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
   }

   return hash;
}

unsigned int APHash (const char* str, unsigned int length) {
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   }

   return hash;
}
