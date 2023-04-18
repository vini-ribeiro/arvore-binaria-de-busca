#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <stdio.h>
using std::srand;
using std::rand;

#include <time.h>
using std::time;

template <typename T>
struct No {
    int chave;
    T dado;
    No<T> *esquerda, *direita;
};

template <typename T>
struct Arvore {
    No<T> *raiz;
};

template <typename T>
void inicializarArvore(Arvore<T> *arvore)
{
    arvore->raiz = NULL;
}

/// se eu botar apenas o * atras de raiz, ela comecara a apontar para o local aonde a variavel que esta sendo passada por parametro esta sendo apontada
template <typename T>
void inserirNaArvore(No<T> *&raiz, int chave, T dado) /// **raiz 
{
    if (raiz == NULL) { /// *raiz
        No<T> *novo = new No<T>;
        novo->chave = chave;
        novo->dado = dado;
        novo->direita = NULL;
        novo->esquerda = NULL;
        raiz = novo; /// *raiz

        return;
    }

    if (raiz->chave >= chave) { /// (*raiz)->chave
        inserirNaArvore(raiz->esquerda, chave, dado); /// &((*raiz)->esquerda)
    }

    if (raiz->chave < chave) { /// (*raiz)->chave
        inserirNaArvore(raiz->direita, chave, dado); /// &((*raiz)->direita)
    }
}

template <typename T>
void inserirNaArvore(Arvore<T> *arvore, int chave, T dado)
{
    inserirNaArvore(arvore->raiz, chave, dado); /// &(arvore->raiz)
}

template <typename T>
void prefixado(No<T> *raiz)
{
    if (raiz == NULL) return;
    cout << raiz->dado << " - ";
    prefixado(raiz->esquerda);
    prefixado(raiz->direita);
}

template <typename T>
void prefixado(Arvore<T> *arvore)
{
    prefixado(arvore->raiz);
}

template <typename T>
void infixado(No<T> *raiz)
{
    if (raiz == NULL) return;
    infixado(raiz->esquerda);
    cout << raiz->dado << " - ";
    infixado(raiz->direita);
}

template <typename T>
void infixado(Arvore<T> *arvore)
{
    infixado(arvore->raiz);
}

template <typename T>
void posfixado(No<T> *raiz)
{
    if (raiz == NULL) return;
    posfixado(raiz->esquerda);
    posfixado(raiz->direita);
    cout << raiz->dado << " - ";
}

template <typename T>
void posfixado(Arvore<T> *arvore)
{
    posfixado(arvore->raiz);
}

template <typename T>
bool buscarNaArvore(No<T> *raiz, int chave)
{
    if (raiz == NULL) return false;

    if (raiz->chave == chave) return true;

    if (raiz->chave >= chave)
        return buscarNaArvore(raiz->esquerda, chave);

    if (raiz->chave < chave)
        return buscarNaArvore(raiz->direita, chave);
}

template <typename T>
bool buscarNaArvore(Arvore<T> *arvore, int chave)
{
    return buscarNaArvore(arvore->raiz, chave);
}

template <typename T>
void alturaDaArvore(No<T> *raiz, int &contador, int &altura)
{
    if (raiz == NULL) {
        if (contador > altura)
            altura = contador;
        return;
    }
    contador++;
    alturaDaArvore(raiz->esquerda, contador, altura);
    alturaDaArvore(raiz->direita, contador, altura);
    contador--;
}

template <typename T>
int alturaDaArvore(Arvore<T> *arvore)
{
    int contador = 0, altura = 0;
    alturaDaArvore(arvore->raiz, contador, altura);

    return altura;
}

template <typename T>
No<T>* procuraRaiz(No<T> *raiz)
{
    if (raiz->direita == NULL) {
        return raiz;
    }

    if (raiz->direita->direita == NULL) {
        No<T> *aux = raiz->direita;
        raiz->direita = NULL;
        return aux;
    }

    return procuraRaiz(raiz->direita);
}

template <typename T>
void removerDaArvore(No<T> *raiz, int chave, bool &removido)
{
    if (raiz == NULL) return;

    if (raiz->chave == chave) {
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            delete raiz;
            removido = true;
            return;
        }

        if (raiz->esquerda != NULL && raiz->direita == NULL) {
            raiz->chave = raiz->esquerda->chave;
            raiz->dado = raiz->esquerda->dado;

            No<T> *temporario = raiz->esquerda;

            raiz->esquerda = temporario->esquerda;
            raiz->direita = temporario->direita;

            delete temporario;

            return;
        }

        if (raiz->esquerda == NULL && raiz->direita != NULL) {
            raiz->chave = raiz->direita->chave;
            raiz->dado = raiz->direita->dado;

            No<T> *temporario = raiz->direita;

            raiz->direita = temporario->direita;
            raiz->esquerda = temporario->esquerda;

            delete temporario;

            return;
        }

        No<T> *temp = procuraRaiz(raiz->esquerda);

        raiz->chave = temp->chave;
        raiz->dado = temp->dado;

        if (raiz->esquerda->chave == temp->chave) {
            raiz = temp->esquerda;
        }

        delete temp;
    }

    if (raiz->chave >= chave)
        removerDaArvore(raiz->esquerda, chave, removido);

    if (removido) {
        raiz->esquerda = NULL;
        removido = false;
    }

    if (raiz->chave < chave)
        removerDaArvore(raiz->direita, chave, removido);

    if (removido) {
        raiz->direita = NULL;
        removido = false;
    }
}

template <typename T>
void removerDaArvore(Arvore<T> *arvore, int chave)
{
    if (!buscarNaArvore(arvore, chave)) return;

    bool removido = false;

    removerDaArvore(arvore->raiz, chave, removido);

    if (removido)
        arvore->raiz = NULL;
}

void embaralhaAlfabeto(char *alfabeto, int vezes)
{
    srand(time(NULL));
    int letra1, letra2;
    char aux;
    for (int i = 0; i < vezes; i++) {
        letra1 = rand() % 26;
        letra2 = rand() % 26;
        aux = alfabeto[letra1];
        alfabeto[letra1] = alfabeto[letra2];
        alfabeto[letra2] = aux;
    }
}

int main()
{
    Arvore<char> *arvore = new Arvore<char>;

    inicializarArvore(arvore);

    char alfabeto[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    int vezes = 0;
    embaralhaAlfabeto(alfabeto, 100);

    do {
        inserirNaArvore(arvore, alfabeto[vezes], alfabeto[vezes]);
        vezes++;
    } while (vezes < 26);

    if (buscarNaArvore(arvore, 'C')) {
        cout << "Esta na arvore" << endl;
    } else {
        cout << "Nao esta na arvore" << endl;
    }

    cout << "Prefixado: ";
    prefixado(arvore);
    cout << endl;
    cout << "Infixado: ";
    infixado(arvore);
    cout << endl;
    cout << "Posfixado: ";
    posfixado(arvore);
    cout << endl;
    cout << "Altura: ";
    int altura = alturaDaArvore(arvore);
    cout << altura << endl;

    for (int i = 65; i < 91; i++) {
        cout << endl;
        removerDaArvore(arvore, i);
        char letra = i;
        cout << letra << " Removido: ";
        infixado(arvore);
        cout << endl;
    }

    return 0;
}
