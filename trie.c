#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <ncurses.h>
#include <sys/resource.h>
#include <sys/time.h>
 
#define NOME_DICIONARIO "dicioPadrao"
#define SUCESSO                 0
#define NUM_ARGS_INCORRETO      1
#define ARQDICIO_NAOCARREGADO   2
#define ARQTEXTO_ERROABERTURA   3
#define ARQTEXTO_ERROLEITURA    4
#define ERRO_DICIO_NAOCARREGADO 5
#define TAM_MAX                 45

#define malc(x) (x*) malloc(sizeof (x))

int qtdePalavrasDic = 0;


typedef struct _arv {
    struct _arv *children[27];
    bool end;
} arv; arv *root = NULL;
 

typedef struct _tripair {
	int qtdErros, qtdPalavras;
	double tempoCheck;
} tripair;


arv *createNode ( );
void insert (const char *key);
bool search (const char *key);
bool carregaDicio (char * filename);
tripair contaErros (char * filename);
double calcula_tempo(const struct rusage *b, const struct rusage *a);
void contaDicio (arv * node);
void destroy (arv * node);


int main (int argc, char * argv[]) {
	struct rusage tempo_inicial, tempo_final;
	char * dicionario, * arqTexto;
	tripair texto;
	double tempo_carga = 0.0, tempo_calc_tamanho_dic = 0.0, tempo_limpeza_memoria = 0.0;
	bool carga;

	root = createNode(); 


	dicionario = (argc == 3) ? argv[1] : NOME_DICIONARIO;

	if (argc != 2 && argc != 3) {
		printf ("Uso: %s [nomeArquivoDicionario] nomeArquivoTexto\n", argv[0]);
		return NUM_ARGS_INCORRETO;
	}

	getrusage (RUSAGE_SELF, &tempo_inicial);
		carga = carregaDicio(dicionario);
	getrusage (RUSAGE_SELF, &tempo_final);

	if (!carga) {
		printf ("Dicionario nao carregado!\n");
		return ARQDICIO_NAOCARREGADO;
	}

	tempo_carga = calcula_tempo (&tempo_inicial, &tempo_final);


	arqTexto = (argc == 3) ? argv[2] : argv[1]; 
	texto = contaErros(arqTexto);

	if (texto.qtdErros == -1 && texto.qtdPalavras == -1) {
		printf ("Nao foi possivel abrir o arquivo de texto %s.\n", arqTexto);
		return ARQTEXTO_ERROABERTURA;
	}


	getrusage (RUSAGE_SELF, &tempo_inicial);
		contaDicio (root);
	getrusage (RUSAGE_SELF, &tempo_final);

	tempo_calc_tamanho_dic = calcula_tempo (&tempo_inicial, &tempo_final);


	getrusage (RUSAGE_SELF, &tempo_inicial);
		destroy (root);
	getrusage (RUSAGE_SELF, &tempo_final);

	tempo_limpeza_memoria = calcula_tempo (&tempo_inicial, &tempo_final);


	printf ("\nTOTAL DE PALAVRAS ERRADAS NO TEXTO      : %d\n",           texto.qtdErros);
	printf ("TOTAL DE PALAVRAS DO DICIONARIO         : %d\n",            qtdePalavrasDic);
	printf ("TOTAL DE PALAVRAS DO TEXTO              : %d\n",          texto.qtdPalavras);
	printf ("TEMPO GASTO COM CARGA DO DICIONARIO     : %.2f\n",              tempo_carga);
	printf ("TEMPO GASTO COM CHECK DO ARQUIVO        : %.2f\n",         texto.tempoCheck);
	printf ("TEMPO GASTO P CALCULO TAMANHO DICIONARIO: %.2f\n",   tempo_calc_tamanho_dic);
	printf ("TEMPO GASTO COM LIMPEZA DA MEMORIA      : %.2f\n",    tempo_limpeza_memoria);
	printf ("------------------------------------------------------\n");
	printf ("T E M P O   T O T A L                   : %.2f\n\n",
		tempo_carga + texto.tempoCheck + tempo_calc_tamanho_dic + tempo_limpeza_memoria);
	printf ("------------------------------------------------------\n");

	return 0;
}



double calcula_tempo(const struct rusage *b, const struct rusage *a) {
    if (b == NULL || a == NULL)
        return 0;
    else
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
}


tripair contaErros (char * filename) {
	FILE *fp = fopen(filename, "r");
	int indice = 0;
	char lowered[46], original[46], c;
	tripair ans, fail; 
	struct rusage tempo_inicial, tempo_final;
	bool found = false;

	ans.qtdPalavras = ans.qtdErros = ans.tempoCheck = 0;

	if (fp == NULL) {
		fail.qtdPalavras = fail.qtdErros = -1;
		return fail;
	}

	for (c = fgetc(fp); c != EOF; c = fgetc(fp)) {
		if (isalpha(c) || (c == '\'' && indice > 0)) {
			original[indice] = c;
			lowered[indice++] = tolower(c);
			if (indice > TAM_MAX) {
				while ((c = fgetc(fp)) != EOF && isalpha(c));
				indice = 0;
			}
		} else if (isdigit(c)) {
			while ((c = fgetc(fp)) != EOF && isalnum(c));
			indice = 0;
		} else if (indice > 0) {
			original[indice] = lowered[indice] = '\0';
			ans.qtdPalavras++;

			getrusage (RUSAGE_SELF, &tempo_inicial);
				found = search (lowered);
			getrusage (RUSAGE_SELF, &tempo_final);

			ans.tempoCheck += calcula_tempo(&tempo_inicial, &tempo_final);

			if (found == false) {
				printf ("%s\n", original);
				ans.qtdErros++;
			} else found = false;

			memset(original, 0, sizeof (original));
			memset(lowered, 0, sizeof (lowered));
			indice = 0;
		}
	}

	fclose (fp);

	return ans;
}


bool carregaDicio (char * filename) {
	FILE *fp = fopen(filename, "r");
	char c[46];
	if (fp == NULL) return false;
	while (fscanf (fp, "%s", c) > 0) insert(c);
	fclose (fp);
	return true;
}


bool search (const char *key) {
	int level, length = strlen(key), index;
	arv *tmp = root;

	for (level = 0; level < length; level++) {
		index = key[level] == '\'' ? 26 : key[level] - 97;
		if (!tmp->children[index]) return false;
		tmp = tmp->children[index];
	}

	return (tmp != NULL && tmp->end);
}


void insert (const char *key) {
	int level, length = strlen(key), index;
	arv *tmp = root;

	for (level = 0; level < length; level++) {
		index = key[level] == '\'' ? 26 : key[level] - 97;
		if (!tmp->children[index]) tmp->children[index] = createNode();
		tmp = tmp->children[index];
	}

	tmp->end = true;
}


arv *createNode ( ) {
	arv *tmp = malc(arv);
	int i;

	if (tmp != NULL) { 
		tmp->end = false;
		for (i = 0; i < 27; i++) tmp->children[i] = NULL;
	}

	return tmp;
}


void contaDicio (arv * node) {
	int i;
	if (node->end == true) qtdePalavrasDic++;
	for (i = 0; i < 27; ++i) if (node->children[i]) contaDicio (node->children[i]);
}


void destroy (arv * node) {
	int i;

	if (node==NULL) return;

	for(i=0; i<27; i++) destroy (node->children[i]);

	free(node);

	return;
}