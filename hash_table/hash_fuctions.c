#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#define TAM_MAX 45

// Palavra.
struct words{
    char palavra[TAM_MAX];
};
typedef struct words Words;


unsigned int RSHash(const char* str, unsigned int length) {
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

unsigned int JSHash(const char* str, unsigned int length) {
   unsigned int hash = 1315423911;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash ^= ((hash << 5) + (*str) + (hash >> 2));
   }

   return hash;
}

unsigned int PJWHash(const char* str, unsigned int length) {
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
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

unsigned int ELFHash(const char* str, unsigned int length) {
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

unsigned int BKDRHash(const char* str, unsigned int length) {
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash * seed) + (*str);
   }

   return hash;
}

unsigned int SDBMHash(const char* str, unsigned int length) {
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (*str) + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}

unsigned int DJBHash(const char* str, unsigned int length) {
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = ((hash << 5) + hash) + (*str);
   }

   return hash;
}

unsigned int DEKHash(const char* str, unsigned int length) {
   unsigned int hash = length;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
   }

   return hash;
}

unsigned int APHash(const char* str, unsigned int length) {
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));
   }

   return hash;
}

int countlines(const char *filename){
  FILE *fp = fopen(filename, "r");
  int count = 0;
  char c;

  for (c = getc(fp); c != EOF; c = getc(fp))
    if (c == '\n') count++;

  fclose(fp);
  return count;
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
} /* fim-calcula_tempo */

int main(int argc, char *argv[]) {
   FILE *dic = NULL;
   int i=0, tam = countlines("dicioPadrao.txt"), j=0, alf[26];
   Words *dic_vet = (Words*) malloc(sizeof(Words) * tam);
   char letra;
   struct rusage tempo_inicial, tempo_final;
   unsigned int *rs_p, *js_p, *pjw_p, *elf_p, *bkdr_p, *sdbm_p, *djb_p, *dek_p, *ap_p;
   int rs_c, js_c, pjw_c, elf_c, bkdr_c, sdbm_c, djb_c, dek_c, ap_c;
   double rs, js, pjw, elf, bkdr, sdbm, djb, dek, ap;

   alf[0] = 0;
   dic = fopen("dicioPadrao.txt", "rt");
   //Carrega dicionário na memória.
   rewind(dic);
   fgets(dic_vet[i++].palavra, sizeof(Words), dic);
   letra = dic_vet[0].palavra[0];
   j++;

   while(fgets(dic_vet[i++].palavra, sizeof(Words), dic)!= NULL) {
      if(dic_vet[i-1].palavra[0] =! letra) {
         letra = dic_vet[i-1].palavra[0];
         alf[j] = i;
         ++j;
      }
   }
   fclose(dic);


   // Testando o RSHash para colisões e tempo de hash do dicionário.
   rs_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      rs_p[i] = RSHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   rs = calcula_tempo(&tempo_inicial, &tempo_final);
   free(rs_p);

   // Testando JSHash para colisões e tempo de hash do dicionário.
   js_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      js_p[i] = JSHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   js = calcula_tempo(&tempo_inicial, &tempo_final);
   free(js_p);

   // Testando PJWHash para colisões e tempo de hash do dicionário.
   pjw_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      pjw_p[i] = PJWHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   pjw = calcula_tempo(&tempo_inicial, &tempo_final);
   free(pjw_p);

   // Testando ELFHash para colisões e tempo de hash do dicionário.
   elf_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      elf_p[i] = ELFHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   elf = calcula_tempo(&tempo_inicial, &tempo_final);
   free(elf_p);

   // Testando BKDRHash para colisões e tempo de hash do dicionário.
   bkdr_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      bkdr_p[i] = BKDRHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   bkdr = calcula_tempo(&tempo_inicial, &tempo_final);
   free(bkdr_p);

   // Testando SDBMHash para colisões e tempo de hash do dicionário.
   sdbm_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      sdbm_p[i] = SDBMHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   sdbm = calcula_tempo(&tempo_inicial, &tempo_final);
   free(sdbm_p);

   // Testando DJBHash para colisões e tempo de hash do dicionário.
   djb_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      djb_p[i] = DJBHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   djb = calcula_tempo(&tempo_inicial, &tempo_final);
   free(djb_p);

   // Testando DEKHash para colisões e tempo de hash do dicionário.
   dek_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   getrusage(RUSAGE_SELF, &tempo_inicial);
   for(i=0; i<tam;++i){
      dek_p[i] = DEKHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_final);
   dek = calcula_tempo(&tempo_inicial, &tempo_final);
   free(dek_p);

   // Testando APHash para colisões e tempo de hash do dicionário.
   ap_p = (unsigned int *)malloc(sizeof(unsigned int) * tam);
   for(i=0; i<tam;++i){
      ap_p[i] = APHash(dic_vet[i].palavra, strlen(dic_vet[i].palavra));
   }
   getrusage(RUSAGE_SELF, &tempo_inicial);
   getrusage(RUSAGE_SELF, &tempo_final);
   ap = calcula_tempo(&tempo_inicial, &tempo_final);
   free(ap_p);

   printf(" RSHash = %lf\n JSHash =  %lf\n PJWHash = %lf\n ELFHash = %lf\n", rs, js, pjw, elf);
   printf(" BKDRHash = %lf\n SDBMHash = %lf\n DJBHash = %lf\n DEKHash = %lf\n APHash = %lf\n", bkdr, sdbm, djb, dek, ap);

   return 0;
}