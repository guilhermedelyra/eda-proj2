#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define malc(x) (x *) malloc(sizeof(x))

typedef struct _vecstring {
	char *palavra;
} vs; vs *b[1];


int qtIndice = 0;

char* concat(char *s1, char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
void carregaDicio (char * filename) {
	FILE *fp = fopen(filename, "r");
	char c[46];
	b[0]->palavra = "";
	while (fscanf(fp, "%s", c) > 0) {
		b[0]->palavra = concat(b[0]->palavra, c);
	}

	fclose (fp);
}


int main(){
	b[0] = malc(vs);
	carregaDicio ("dicioPadrao");
	printf("%s\n", b[0]->palavra);
	return 0;
}

