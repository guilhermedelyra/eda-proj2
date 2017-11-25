#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define malc(x) (x *) malloc(sizeof(x))
#define nl ; printf("\n")
#define max(a, b) ((a > b) ? (a) : (b))

/* #### ESTRUTURAS DE DADOS FILA, PILHA E ARVORE #### */


typedef struct _tree {
	int conteudo, h;
	struct _tree *esq, *dir;
} arv; arv * raiz;


typedef struct _stack {
   arv *endereco; 
   struct _stack *prox;
} pilha; pilha * top = NULL;


typedef struct _queue {
    arv * endereco;
    struct _queue *next;
} fila; fila * head = NULL;


/* #### PSEUDO-FUNÇÕES PARA MANIPULAÇÃO DE PILHA #### */


void empilha (arv *no);
arv * desempilha ();
void liberapilha ();


/* #### PSEUDO-FUNÇÕES PARA MANIPULAÇÃO DE FILA #### */

void enfileira (arv * endereco);
arv * desenfileira ();
void limpa_fila (fila * c);


/* #### PSEUDO-FUNÇÕES PARA MANIPULAÇÃO DE ARVORE BINÁRIA DE BUSCA #### */


void pre_ordem (arv * raiz);
void em_ordem (arv * raiz);
void pos_ordem (arv * raiz);
void dfs ();
void bfs ();
arv * busca (arv * raiz, int el);
void liberaraiz (arv * ra);
int height (arv * temp);
int diff (arv * temp);
arv * rr (arv * parent);
arv * ll (arv * parent);
arv * lr (arv * parent);
arv * rl (arv * parent);
arv * balance (arv * temp);
arv * insert (arv * root, int conteudo);
void set_height (arv * root, int height); 
arv * delete (arv * root, int conteudo);
arv * min_node (arv * node);

/* ############################################################# */

int main(){
	raiz = insert(raiz, 9);
	raiz = insert(raiz, 5);
	raiz = insert(raiz, 10);
	raiz = insert(raiz, 0);
	raiz = insert(raiz, 6);
	raiz = insert(raiz, 11);
	raiz = insert(raiz, -1);
	raiz = insert(raiz, 1);
	raiz = insert(raiz, 2);

	set_height (raiz, 0);
	printf("pre_ordem") nl nl;
	pre_ordem (raiz) nl nl;
	printf("em_ordem") nl nl;
	em_ordem (raiz) nl nl;
	printf("pos_ordem") nl nl;
	pos_ordem (raiz) nl nl;

	printf("dfs") nl nl;
	dfs () nl nl;
	printf("bfs") nl nl;
	bfs () nl nl;

	raiz = delete (raiz, 10) nl nl nl;

	set_height (raiz, 0);
	printf("pre_ordem") nl nl;
	pre_ordem (raiz) nl nl;
	printf("em_ordem") nl nl;
	em_ordem (raiz) nl nl;
	printf("pos_ordem") nl nl;
	pos_ordem (raiz) nl nl;

	printf("dfs") nl nl;
	dfs () nl nl;
	printf("bfs") nl nl;
	bfs () nl nl;

	liberaraiz(raiz);

	return 0;
}

/* #### FUNÇÕES PARA MANIPULAÇÃO DE PILHA #### */


void empilha (arv *no) {
	pilha *new = malc (pilha);
	new->endereco = no;
	if(top == NULL) new->prox = NULL;
	else new->prox = top;
	top = new;
}

arv * desempilha () {
	if(top != NULL) {
		pilha *temp = top;
		arv *x = top->endereco;
		top = temp->prox;
		free(temp);
		return x;
	} else return NULL;
}


void liberapilha () {
	while (top != NULL) {
		desempilha ();
	}
}


/* #### FUNÇÕES PARA MANIPULAÇÃO DE FILA #### */


void enfileira (arv * endereco) {
	fila *new = malc(fila);
	if (!new) return;

	new->endereco = endereco;
	new->next = head;

	head = new;
}

arv * desenfileira () {
	fila *current, *prev = NULL;
	arv * tmp;

	if (head == NULL) return NULL;

	current = head;

	while (current->next != NULL) {
		prev = current;
		current = current->next;
	}

	tmp = current->endereco;
	free(current);

	if (prev) prev->next = NULL;
	else head = NULL;

	return tmp;
}


void limpa_fila (fila * c) {
	while (head != NULL) {
		limpa_fila (c->next);
		free(c);
	}
}


/* #### FUNÇÕES PARA MANIPULAÇÃO DE AVL #### */


void pre_ordem (arv * raiz) {
	if(raiz != NULL){
		printf ("%d [%d] - ", raiz->conteudo, raiz->h);
		pre_ordem (raiz->esq);
		pre_ordem (raiz->dir);
	}
}


void em_ordem (arv * raiz) {
	if(raiz != NULL){
		pre_ordem (raiz->esq);
		printf ("%d [%d] - ", raiz->conteudo, raiz->h);
		pre_ordem (raiz->dir);
	}
}


void pos_ordem (arv * raiz) {
	if(raiz != NULL){
		pre_ordem (raiz->esq);
		pre_ordem (raiz->dir);
		printf ("%d [%d] - ", raiz->conteudo, raiz->h);
	}
}


void dfs () {
	if (raiz == NULL) return;
	arv *tmp = raiz;
	empilha (tmp);
	while (top) {
		printf ("%d [%d] - ", tmp->conteudo, tmp->h);
		if (tmp->esq != NULL) empilha (tmp->esq);
		if (tmp->dir != NULL) empilha (tmp->dir);
		tmp = desempilha ();
	}
	liberapilha (); 
}


void bfs () {
	if (raiz == NULL) return;
	arv * tmp = raiz;
	enfileira (raiz);
	while(head != NULL){
		tmp = desenfileira ();
		printf ("%d [%d] - ", tmp->conteudo, tmp->h);
		if (tmp->esq != NULL) enfileira (tmp->esq);
		if (tmp->dir != NULL) enfileira (tmp->dir);
	}
	limpa_fila (head);
}

arv * busca (arv * raiz, int el) {
	if (raiz != NULL) {
		if (raiz->conteudo > el) return busca (raiz->esq, el);
		if (raiz->conteudo < el) return busca (raiz->dir, el);
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


arv * insert (arv * root, int conteudo) {
	if (root == NULL) {
		root = malc(arv);
		root->conteudo = conteudo;
		root->esq = NULL;
		root->dir = NULL;
		return root;
	} else if (conteudo < root->conteudo) {
		root->esq = insert (root->esq, conteudo);
		root = balance (root);
	} else if (conteudo > root->conteudo) {
		root->dir = insert(root->dir, conteudo);
		root = balance (root);
	}
	return root;
}


arv * delete (arv * root, int conteudo) {
	if (root == NULL) return root;

	if (conteudo < root->conteudo) root->esq = delete (root->esq, conteudo);
	else if (conteudo > root->conteudo) root->dir = delete (root->dir, conteudo);
	else {
		if ((root->esq == NULL) || (root->dir == NULL)) {
			arv * temp = root->esq ? root->esq : root->dir;
		if (temp == NULL) {
			temp = root;
			root = NULL;
		} else *root = *temp;
			free(temp);
		} else {
			arv * temp = min_node (root->dir);
			root->conteudo = temp->conteudo;
			root->dir = delete (root->dir, temp->conteudo);
		}
	}

	if (root == NULL) return root;

	root->h = 1 + max(height (root->esq), height (root->dir));

	if (diff (root) > 1 && diff (root->esq) >= 0) return ll (root);
	if (diff (root) > 1 && diff (root->esq) < 0) return lr (root);
	if (diff (root) < -1 && diff (root->dir) <= 0) return rr (root);
	if (diff (root) < -1 && diff (root->dir) > 0) return rl (root);

	return root;
}

arv * min_node (arv * node) {
	arv * current = node;
	while (current->esq != NULL) current = current->esq;
	return current;
}