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
#define MOD 11380005

typedef struct _char {
  char palavra[46];
  bool present;
  int qtdColisoes;
  struct _char *c[1];
} ch; ch b[MOD];


typedef struct _tripair {
  int qtdErros, qtdPalavras;
  double tempoCheck;
} tripair;


bool carregaDicio (char * filename);
double calcula_tempo (const struct rusage *b, const struct rusage *a);
unsigned int APHash (const char* str, unsigned int length);
void set () { int i; for (i = 0; i < MOD; ++i) b[i].present = false; b[i].qtdColisoes = 0;}
void descarrega ();
tripair contaErros (char * filename);
int contaDicio ();



int main (int argc, char * argv[]) {
  struct rusage tempo_inicial, tempo_final;
  char * dicionario, * arqTexto;
  int qtdePalavrasDic;
  tripair texto;
  double tempo_carga = 0.0, tempo_calc_tamanho_dic = 0.0, tempo_limpeza_memoria = 0.0;
  bool carga;

  set ();

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
      descarrega ();
      return ARQTEXTO_ERROABERTURA;
  }


  getrusage(RUSAGE_SELF, &tempo_inicial);
    qtdePalavrasDic = contaDicio ();
  getrusage(RUSAGE_SELF, &tempo_final);

  tempo_calc_tamanho_dic = calcula_tempo (&tempo_inicial, &tempo_final);




  getrusage (RUSAGE_SELF, &tempo_inicial);
    descarrega ();
  getrusage (RUSAGE_SELF, &tempo_final);

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



bool carregaDicio (char *filename) {
  FILE *fp = fopen(filename, "r");
  char c[46];
  unsigned int indiceHash, posiColisao;

  if (fp == NULL) return false;

  while (fscanf(fp, "%s", c) > 0) {
    indiceHash = APHash(c, strlen(c));
    posiColisao = b[indiceHash].qtdColisoes;


    if (b[indiceHash].present == false) {
      strcpy(b[indiceHash].palavra, c);
      b[indiceHash].present = true;
    } else {
      b[indiceHash].c[posiColisao] = malc(ch);
      strcpy(b[indiceHash].c[posiColisao]->palavra, c);

      b[indiceHash].qtdColisoes++;
    }
  }

  fclose(fp);

  return true;
}


void descarrega () {
  int i, j;
  for (i = 0; i < 11380003; ++i) 
    if (b[i].present) 
      if (b[i].qtdColisoes > 0)
        for (j = 0; j < b[i].qtdColisoes - 1; ++j)
          free(b[i].c[j]);
}



tripair contaErros (char * filename) {
  FILE *fp = fopen(filename, "r");
  int indice = 0, i;
  unsigned int indiceHash;
  char lowered[46], original[46], c;
  tripair ans, fail; 
  struct rusage tempo_inicial, tempo_final;
  bool equal = false;

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

          indiceHash = APHash(lowered, strlen(lowered));

          getrusage(RUSAGE_SELF, &tempo_inicial);
            if (strcmp(b[indiceHash].palavra, lowered) == 0) {
              equal = true;
            } else if (b[indiceHash].qtdColisoes > 0) {
              for (i = 0; i < b[indiceHash].qtdColisoes; ++i) 
                if (strcmp(b[indiceHash].c[i]->palavra, lowered) == 0) equal = true;
            } else {
              equal = false;
            }
          getrusage(RUSAGE_SELF, &tempo_final);
          
          ans.tempoCheck += calcula_tempo(&tempo_inicial, &tempo_final);

          if (equal == false) {
              ans.qtdErros++;
          } else equal = false;

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


unsigned int APHash (const char* str, unsigned int length) {
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   }

   return hash % MOD;
}


int contaDicio () {
  int i, qtd = 0;
  for (i = 0; i < MOD; ++i) if (b[i].present) qtd += 1 + b[i].qtdColisoes;
  return qtd;
}