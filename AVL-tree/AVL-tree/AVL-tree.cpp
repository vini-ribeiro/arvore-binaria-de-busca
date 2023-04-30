#include <iostream>
#include <string>

using namespace std;

struct Pessoa {
	long long int CPF;
	string nome;
	string profissao;
};

template <typename T>
struct No {
	Pessoa *pessoa;
	No<T> *esquerda;
	No<T> *direita;
};

template <typename T>
struct Arvore {
	No<T> *raiz;
};

template <typename T>
void inicializarArvore(Arvore<T> &arvore)
{
	arvore.raiz = NULL;
}

template <typename T>
int buscar_altura(No<T> *raiz)
{
	return (raiz == NULL) ? 0 : raiz->altura;
}

template <typename T>
int calcular_altura(No<T> *raiz)
{
	int alt_e = buscar_altura(raiz->esquerda);
	int alt_d = buscar_altura(raiz->direita);
	return (alt_e > alt_d) ? alt_e + 1 : alt_d + 1;
}

template <typename T>
void buscar_maior(No<T> *raiz, int &chave)
{
	if (raiz == NULL) return;
	if (raiz->direita != NULL)
		buscar_maior(raiz->direita, chave);
	else
		chave = raiz->chave;
}

/// ROTACIONA A ARVORE ABAIXO
template <typename T>
No<T>* rotacao_simples_direita(No<T> *k)
{
	No<T> *x = k->esquerda;
	No<T> *t2 = x->direita;

	x->direita = k;
	k->esquerda = t2;
	k->altura = calcular_altura(k);
	x->altura = calcular_altura(x);
	return x;
}

template <typename T>
No<T>* rotacao_dupla_direita(No<T> *k)
{
	No<T> *x = k->esquerda;
	No<T> *w = x->direita;
	No<T> *t2 = w->esquerda;
	No<T> *t3 = w->direita;

	w->direita = k;
	w->esquerda = x;
	x->direita = t2;
	k->esquerda = t3;

	k->altura = calcular_altura(k);
	x->altura = calcular_altura(x);
	w->altura = calcular_altura(w);

	return w;
}

template <typename T>
No<T>* rotacao_simples_esquerda(No<T> *k)
{
	No<T> *y = k->direita;
	No<T> *t2 = y->esquerda;

	y->esquerda = k;
	k->direita = t2;
	k->altura = calcular_altura(k);
	y->altura = calcular_altura(y);
	return y;
}

template <typename T>
No<T>* rotacao_dupla_esquerda(No<T> *k)
{
	No<T> *y = k->direita;
	No<T> *z = y->esquerda;
	No<T> *t2 = z->esquerda;
	No<T> *t3 = z->direita;

	z->direita = y;
	z->esquerda = k;
	y->esquerda = t3;
	k->direita = t2;

	k->altura = calcular_altura(k);
	y->altura = calcular_altura(y);
	z->altura = calcular_altura(z);

	return z;
}

template <typename T>
No<T>* rotacao_avl(No<T> * k)
{
	if (buscar_altura(k->esquerda) > buscar_altura(k->direita)) { // Rota  o a direita
		No<T> *x = k->esquerda;
		if (buscar_altura(x->direita) > buscar_altura(x->esquerda))
			return rotacao_dupla_direita(k);
		else
			return rotacao_simples_direita(k);
	} else { // Rota  o a esquerda
		No<T>* y = k->direita;
		if (buscar_altura(y->esquerda) > buscar_altura(y->direita))
			return rotacao_dupla_esquerda(k);
		else
			return rotacao_simples_esquerda(k);
	}
}
/// ROTACIONA A ARVORE ACIMA

/// INSERCAO E RETIRADA DA ARVORE ABAIXO
template <typename T>
No<T>* inserirArvore(No<T> *raiz, int chave, T dado)
{
	if (raiz == NULL) {
		No<T> *novo = new No<T>;
		novo->chave = chave;
		novo->dado = dado;
		novo->direita = NULL;
		novo->esquerda = NULL;
		novo->altura = calcular_altura(novo);
		return novo;
	}

	if (chave == raiz->chave) return raiz;

	if (chave < raiz->chave)
		raiz->esquerda = inserirArvore(raiz->esquerda, chave, dado);
	else
		raiz->direita = inserirArvore(raiz->direita, chave, dado);

	int alt_e = buscar_altura(raiz->esquerda);
	int alt_d = buscar_altura(raiz->direita);

	if (alt_e - alt_d == 2 || alt_e - alt_d == -2)
		raiz = rotacao_avl(raiz);
	else
		raiz->altura = (alt_e > alt_d) ? alt_e + 1 : alt_d + 1;

	return raiz;
}

template <typename T>
void inserirArvore(Arvore<T> &arvore, int chave, T dado)
{
	arvore.raiz = inserirArvore(arvore.raiz, chave, dado);
}

template <typename T>
No<T>* retirarArvore(No<T> *&raiz, int chave)
{
	if (raiz == NULL) return NULL;

	if (raiz->chave == chave) {
		if (raiz->esq == NULL && raiz->dir == NULL) {
			delete raiz;
			return NULL;
		}

		if (raiz->esq != NULL && raiz->dir == NULL) {
			No<T> *temp = raiz->esq;
			delete raiz;
			return temp;
		}

		if (raiz->esq == NULL && raiz->dir != NULL) {
			No<T> *temp = raiz->dir;
			delete raiz;
			return temp;
		}

		int ch_aux;
		buscar_maior(raiz->esq, ch_aux);
		raiz->chave = ch_aux;
		raiz->esq = retirarArvore(raiz->esq, ch_aux);
		return raiz;
	}

	if (chave < raiz->chave)
		raiz->esq = retirarArvore(raiz->esq, chave);
	if (chave > raiz->chave)
		raiz->dir = retirarArvore(raiz->dir, chave);

	int alt_e = buscar_altura(raiz->esq);
	int alt_d = buscar_altura(raiz->dir);
	if (alt_e - alt_d == 2 || alt_e - alt_d == -2) raiz = rotacao_avl(raiz);
	else raiz->altura = (alt_e > alt_d) ? alt_e + 1 : alt_d + 1;

	return raiz;
}

template <typename T>
void retirarArvore(Arvore<T> *arvore, int chave)
{
	retirarArvore(arvore->raiz, chave);
}
/// INSERCAO E RETIRADA DA ARVORE ACIMA

/// METODOS DE CAMINHAMENTO ABAIXO
template <typename T>
void prefixado(No<T> *raiz)
{
	if (raiz == NULL) return;
	cout << raiz->dado << " (" << raiz->chave << ") - ";
	prefixado(raiz->esquerda);
	prefixado(raiz->direita);
}

template <typename T>
void prefixado(Arvore<T> &arvore)
{
	prefixado(arvore.raiz);
}

template <typename T>
void infixado(No<T> *raiz)
{
	if (raiz == NULL) return;
	infixado(raiz->esquerda);
	cout << raiz->dado << " (" << raiz->chave << ") - ";
	infixado(raiz->direita);
}

template <typename T>
void infixado(Arvore<T> &arvore)
{
	infixado(arvore.raiz);
}

template <typename T>
void posfixado(No<T> *raiz)
{
	if (raiz == NULL) return;
	posfixado(raiz->esquerda);
	posfixado(raiz->direita);
	cout << raiz->dado << " (" << raiz->chave << ") - ";
}

template <typename T>
void posfixado(Arvore<T> &arvore)
{
	posfixado(arvore.raiz);
}
/// METODOS DE CAMINHAMENTO ACIMA

template <typename T>
void mostrar_arvore(No<T> *raiz, int tab)
{
	cout << endl;
	for (int i = 0; i < tab; i++) cout << " ";
	if (raiz == NULL) {
		cout << "*";
		return;
	}
	cout << raiz->chave;
	mostrar_arvore(raiz->esquerda, tab + 3);
	mostrar_arvore(raiz->direita, tab + 3);
}

template <typename T>
void mostrar_arvore(Arvore<T> &a)
{
	mostrar_arvore(a.raiz, 2);
}

int main()
{
	Arvore<void> ordenadaCPF;
	Arvore<void> ordenadaNome;

	inicializarArvore(ordenadaCPF);
	inicializarArvore(ordenadaNome);

	return 0;
}
