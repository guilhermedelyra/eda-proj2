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

/* #### ESTRUTURAS DE DADOS ARVORE #### */


typedef struct _tree {
	char conteudo[TAM_MAX+1];
	int h;
	struct _tree *esq, *dir;
} arv; arv * raiz;


typedef struct _tripair {
	int qtdErros, qtdPalavras;
	double tempoCheck;
} tripair;

/* #### PSEUDO-FUNÇÕES PARA MANIPULAÇÃO DE ARVORE BINÁRIA DE BUSCA #### */


bool carregaDicio (char * filename);
tripair contaErros (char * filename);
int contaDicio (arv * root);
double calcula_tempo(const struct rusage *b, const struct rusage *a);
arv * busca (arv * raiz, char * el);
void liberaraiz (arv * ra);
int height (arv * temp);
int diff (arv * temp);
arv * rr (arv * parent);
arv * ll (arv * parent);
arv * lr (arv * parent);
arv * rl (arv * parent);
arv * balance (arv * temp);
arv * insert (arv * root, char * conteudo);


/* #### MAIN ##### */


int main (int argc, char * argv[]) {
	struct rusage tempo_inicial, tempo_final;
	char * dicionario, * arqTexto;
	int qtdePalavrasDic;
	tripair texto;
	double tempo_carga = 0.0, tempo_calc_tamanho_dic = 0.0, tempo_limpeza_memoria = 0.0;
	bool carga;


	dicionario = (argc == 3) ? argv[1] : NOME_DICIONARIO;

	if (argc != 2 && argc != 3) {
	    printf("Uso: %s [nomeArquivoDicionario] nomeArquivoTexto\n", argv[0]);
	    return NUM_ARGS_INCORRETO;
	}

	getrusage(RUSAGE_SELF, &tempo_inicial);
		carga = carregaDicio(dicionario);
	getrusage(RUSAGE_SELF, &tempo_final);

	if (!carga) {
		printf("Dicionario nao carregado!\n");
		return ARQDICIO_NAOCARREGADO;
	}

	tempo_carga = calcula_tempo (&tempo_inicial, &tempo_final);


	arqTexto = (argc == 3) ? argv[2] : argv[1];	
	texto = contaErros(arqTexto);

	if (texto.qtdErros == -1 && texto.qtdPalavras == -1) {
	    printf ("Nao foi possivel abrir o arquivo de texto %s.\n", arqTexto);
	    liberaraiz (raiz);
	    return ARQTEXTO_ERROABERTURA;
	}


	getrusage(RUSAGE_SELF, &tempo_inicial);
		qtdePalavrasDic = contaDicio (raiz);
	getrusage(RUSAGE_SELF, &tempo_final);

	tempo_calc_tamanho_dic = calcula_tempo (&tempo_inicial, &tempo_final);


	getrusage(RUSAGE_SELF, &tempo_inicial);
		liberaraiz(raiz);
	getrusage(RUSAGE_SELF, &tempo_final);

	tempo_limpeza_memoria = calcula_tempo (&tempo_inicial, &tempo_final);


	printf("\nTOTAL DE PALAVRAS ERRADAS NO TEXTO      : %d\n",   texto.qtdErros);
    printf("TOTAL DE PALAVRAS DO DICIONARIO         : %d\n",   qtdePalavrasDic);
    printf("TOTAL DE PALAVRAS DO TEXTO              : %d\n",   texto.qtdPalavras);
    printf("TEMPO GASTO COM CARGA DO DICIONARIO     : %.2f\n", tempo_carga);
    printf("TEMPO GASTO COM CHECK DO ARQUIVO        : %.2f\n", texto.tempoCheck);
    printf("TEMPO GASTO P CALCULO TAMANHO DICIONARIO: %.2f\n", tempo_calc_tamanho_dic);
    printf("TEMPO GASTO COM LIMPEZA DA MEMORIA      : %.2f\n", tempo_limpeza_memoria);
    printf("------------------------------------------------------\n");
    printf("T E M P O   T O T A L                   : %.2f\n\n",
     tempo_carga + texto.tempoCheck + tempo_calc_tamanho_dic + tempo_limpeza_memoria);
    printf("------------------------------------------------------\n");

	return 0;
}


/* #### FUNÇÕES PARA MANIPULAÇÃO DE AVL #### */


bool carregaDicio (char * filename) {
	FILE *fp = fopen(filename, "r");
	char c[46];

	if (fp == NULL) return false;

	while (fscanf(fp, "%s", c) > 0) {
		raiz = insert(raiz, c);
	}


	fclose(fp);

	return true;
}


tripair contaErros (char * filename) {
	FILE *fp = fopen(filename, "r");
	arv * tmp;
	int indice = 0;
	char lowered[46], original[46], c;
	tripair ans, fail; 
	struct rusage tempo_inicial, tempo_final;

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

	        getrusage(RUSAGE_SELF, &tempo_inicial);
	        	tmp = busca (raiz, lowered);
	        getrusage(RUSAGE_SELF, &tempo_final);
	        
	        ans.tempoCheck += calcula_tempo(&tempo_inicial, &tempo_final);

	        if (tmp == NULL) {
	            printf("%s\n", original);
	            ans.qtdErros++;
	        }

	        memset(original, 0, sizeof(original));
	        memset(lowered, 0, sizeof(lowered));
	        indice = 0;
	    }
	}

	fclose(fp);

	return ans;
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


arv * busca (arv * raiz, char * el) {
	if (raiz != NULL) {
		if (strcmp(raiz->conteudo, el) > 0) return busca (raiz->esq, el);
		if (strcmp(raiz->conteudo, el) < 0) return busca (raiz->dir, el);
		return raiz;
	} else return NULL;
}


void liberaraiz (arv * ra) {
	if (ra == NULL) return;
	liberaraiz (ra->esq);
	liberaraiz (ra->dir);
	free (ra);	
	return;
}


int height (arv * temp) {
	int h = 0;
	if (temp != NULL) {
		int l_height = height(temp->esq), r_height = height(temp->dir);
		int max_height = max(l_height, r_height);
		h = max_height + 1;
	}
	return h;
}


int diff (arv * temp) {
	if (temp == NULL) return 0;
	return height (temp->esq) - height (temp->dir);
}


arv * rr (arv * parent) {
	arv * temp = parent->dir;
	parent->dir = temp->esq;
	temp->esq = parent;
	return temp;
}


arv * ll (arv * parent) {
	arv * temp = parent->esq;
	parent->esq = temp->dir;
	temp->dir = parent;
	return temp;
}


arv * lr (arv * parent){
	arv * temp = parent->esq;
	parent->esq = rr (temp);
	return ll (parent);
}


arv * rl (arv *parent) {
	arv * temp;
	temp = parent->dir;
	parent->dir = ll (temp);
	return rr (parent);
}


arv * balance (arv *temp) {
	int bal_factor = diff (temp);
	if (bal_factor > 1) {
		if (diff(temp->esq) > 0) temp = ll (temp);
		else temp = lr (temp);
	} else if (bal_factor < -1) {
		if (diff(temp->dir) > 0) temp = rl (temp);
		else temp = rr (temp);
	}
	return temp;
}


arv * insert (arv * root, char * conteudo) {
	if (root == NULL) {
		root = malc(arv);
		strcpy(root->conteudo, conteudo);
		root->esq = NULL;
		root->dir = NULL;
		return root;
	} else if (strcmp(conteudo, root->conteudo) < 0) {
		root->esq = insert (root->esq, conteudo);
		root = balance (root);
	} else if (strcmp(conteudo, root->conteudo) > 0) {
		root->dir = insert(root->dir, conteudo);
		root = balance (root);
	}
	return root;
}


int contaDicio (arv * root) {
	int res = 0;
	if (root) {
		res++;
		res += contaDicio (root->esq);
		res += contaDicio (root->dir);
	}
	return res;
}