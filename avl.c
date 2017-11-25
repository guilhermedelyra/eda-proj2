#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define malc(x) (x *) malloc(sizeof(x))
#define nl ; printf("\n")
#define max(a, b) ((a > b) ? (a) : (b))

/* #### ESTRUTURAS DE DADOS ARVORE #### */

typedef struct _tree {
	char conteudo[46];
	int h;
	struct _tree *esq, *dir;
} arv; arv * raiz;


/* #### PSEUDO-FUNÇÕES PARA MANIPULAÇÃO DE ARVORE BINÁRIA DE BUSCA #### */

void carregaDicio (char * filename);
int contaErros (char * filename);
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
void set_height (arv * root, int height); 


/* #### MAIN ##### */

int main(){
	carregaDicio("test.txt");
	printf("%d\n", contaErros("test1.txt"));
	return 0;
}

/* #### FUNÇÕES PARA MANIPULAÇÃO DE AVL #### */


void carregaDicio (char * filename) {
	FILE *fp = fopen(filename, "r");
	char c[46];

	if (fp == NULL) return;

	while (fscanf(fp, "%s", c) > 0) {
		raiz = insert(raiz, c);
	}


	fclose(fp);
}


int contaErros (char * filename) {
	FILE *fp = fopen(filename, "r");
	arv * tmp;
	int ans = 0, i = 0;
	char c[46], d;

	if (fp == NULL) return 0;

	for(d = getc(fp); d != EOF; d = getc(fp)) {
		d = tolower(d);
		if ((d >= 97 && d <= 122) || d == 39) c[i++] = d;
		else {
			c[i++] = '\0';
			tmp = busca (raiz, c);
			if (tmp == NULL) ans++;
			memset(c, 0, sizeof(c)); i = 0;
		}
	}
	fclose(fp);

	return ans;
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


void set_height (arv * root, int height) {
	if (root != NULL) {
		root->h = height;
		set_height (root->esq, height+1);
		set_height (root->dir, height+1);
	}
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