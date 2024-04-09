#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef int TIPOCHAVE;

struct NO {
  TIPOCHAVE chave;
  struct NO *esq = NULL;
  struct NO *dir = NULL;
  int h = 0;

  NO(TIPOCHAVE ch) { chave = ch; }
};

typedef NO *PONT;

PONT inicializa() { return (NULL); }

PONT criaNo(TIPOCHAVE ch) { return new NO(ch); }

int max(int a, int b) { return (a > b) ? a : b; }

int altura(PONT r) {
  if (!r) return -1;
  return r->h;
}

PONT direita(PONT r) {
  PONT aux;
  aux = r->esq;
  r->esq = aux->dir;
  aux->dir = r;
  r->h = max(altura(r->dir), altura(r->esq)) + 1;
  aux->h = max(altura(aux->esq), r->h) + 1;
  return (aux);
}

PONT esquerda(PONT r) {
  PONT aux;
  aux = r->dir;
  r->dir = aux->esq;
  aux->esq = r;
  r->h = max(altura(r->dir), altura(r->esq)) + 1;
  aux->h = max(altura(aux->dir), r->h) + 1;
  return (aux);
}

PONT esquerdaDireita(PONT r) {
  r->esq = esquerda(r->esq);
  return direita(r);
}

PONT direitaEsquerda(PONT r) {
  r->dir = esquerda(r->dir);
  return esquerda(r);
}

PONT insere(PONT raiz, TIPOCHAVE ch) {
  if (!raiz) return criaNo(ch);
  if (ch < raiz->chave) {
    raiz->esq = insere(raiz->esq, ch);
    if ((altura(raiz->esq) - altura(raiz->dir)) == 2)
      if (ch < raiz->esq->chave)
        raiz = direita(raiz);
      else
        raiz = esquerdaDireita(raiz);
  } else if (ch > raiz->chave) {
    raiz->dir = insere(raiz->dir, ch);
    if ((altura(raiz->dir) - altura(raiz->esq)) == 2)
      if (ch > raiz->dir->chave)
        raiz = esquerda(raiz);
      else
        raiz = direitaEsquerda(raiz);
  }
  raiz->h = max(altura(raiz->esq), altura(raiz->dir)) + 1;
  return raiz;
}

PONT contem(TIPOCHAVE ch, PONT raiz) {
  if (raiz == NULL) return (NULL);
  if (raiz->chave == ch) return (raiz);
  if (raiz->chave > ch) return (contem(ch, raiz->esq));
  return (contem(ch, raiz->dir));
}

int numeroNos(PONT raiz) {
  if (!raiz) return 0;
  return (numeroNos(raiz->esq) + 1 + numeroNos(raiz->dir));
}

void exibirArvore(PONT raiz) {
  if (raiz != NULL) {
    printf("%i", raiz->chave);
    printf("(");
    exibirArvore(raiz->esq);
    exibirArvore(raiz->dir);
    printf(")");
  }
}

PONT buscaNo(PONT raiz, TIPOCHAVE ch, PONT *pai) {
  PONT atual = raiz;
  *pai = NULL;
  while (atual) {
    if (atual->chave == ch) return atual;
    *pai = atual;
    if (ch < atual->chave)
      atual = atual->esq;
    else
      atual = atual->dir;
  }
  return (NULL);
}

PONT removeNo(PONT raiz, TIPOCHAVE ch) {
  PONT pai, no, p, q;
  no = buscaNo(raiz, ch, &pai);
  if (no == raiz) {
    printf("\n\nPara esta aplicacao nao eh possivel excluir a raiz\n");
    return raiz;
  }
  if (no == NULL) return raiz;
  if (!no->esq || !no->dir) {
    if (!no->esq)
      q = no->dir;
    else
      q = no->esq;
  } else {
    p = no;
    q = no->esq;
    while (q->dir) {
      p = q;
      q = q->dir;
    }
    if (p != no) {
      p->dir = q->dir;
      q->esq = no->esq;
    }
    q->dir = no->dir;
  }
  if (!pai) {
    free(no);
    return q;
  }
  if (ch < pai->chave)
    pai->esq = q;
  else
    pai->dir = q;
  free(no);
  return (raiz);
}

int main() {
  PONT r, r1;
  r = inicializa();
  r1 = inicializa();
  PONT p;
  r1 = insere(r, 2);
  r1 = insere(r, 8);
  r1 = insere(r, 12);
  printf("r ====>\n");
  exibirArvore(r);
  printf("r1 ====>\n");
  exibirArvore(r1);
  printf("\n");
  r = insere(r, 15);
  exibirArvore(r1);
  printf("\n");
  r = insere(r, 23);
  exibirArvore(r);
  printf("\n");
  r = insere(r, 25);
  exibirArvore(r);
  printf("\n");
  printf("\n");

  p = contem(12, r);
  if (p != NULL)
    printf("ok\n");
  else
    printf("not ok\n");
  p = contem(18, r);
  if (p != NULL)
    printf("ok\n");
  else
    printf("not ok\n");
  printf("\nNumero de nos %d\n", numeroNos(r));
  exibirArvore(r);
  printf("\nTentativa de remover 8: ");
  removeNo(r, 8);
  printf("\n");
  exibirArvore(r);

  printf("\nTentativa de remover 15: ");
  removeNo(r, 15);
  printf("\n");
  exibirArvore(r);

  printf("\nTentativa de remover 23\n");
  removeNo(r, 23);
  printf("\n");
  exibirArvore(r);

  return 0;
}