#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(){
	char a[] = "abcd";
	printf("%s\n", a);
	memset(a, 0, sizeof(a));
	a[0] = 'o';
	a[1] = 'o';
	a[2] = 'o';
	a[3] = 'o';
	printf("%s\n", a);
}