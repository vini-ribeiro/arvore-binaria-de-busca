#include <iostream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include "windows.h"

using namespace std;

struct Pessoa {
	long long int CPF;
	string nome;
	string profissao;
};

template <typename T>
struct No {
	Pessoa* pessoa;
	int altura;
	No<T>* esquerda;
	No<T>* direita;
};

template <typename T>
struct Arvore {
	No<T>* raiz;
};

template <typename T>
void inicializarArvore(Arvore<T>& arvore)
{
	arvore.raiz = NULL;
}

template <typename T>
int buscar_altura(No<T>* raiz)
{
	return (raiz == NULL) ? 0 : raiz->altura;
}

template <typename T>
int calcular_altura(No<T>* raiz)
{
	int altura_esquerda = buscar_altura(raiz->esquerda);
	int altura_direita = buscar_altura(raiz->direita);
	return (altura_esquerda > altura_direita) ? altura_esquerda + 1 : altura_direita + 1;
}

template <typename T>
No<T>* buscar_maior(No<T>* raiz)
{
	if (raiz == NULL) return NULL;

	if (raiz->direita != NULL)
		buscar_maior(raiz->direita);

	return raiz;
}

template <typename T>
No<T>* pesquisarRegistroArvore(No<T>* raiz, string& nome)
{
	if (raiz == NULL) return NULL;

	if (raiz->pessoa->nome == nome) {
		return raiz;
	}

	if (raiz->pessoa->nome > nome) {
		return pesquisarRegistroArvore(raiz->esquerda, nome);
	}

	return pesquisarRegistroArvore(raiz->direita, nome);
}

template <typename T>
No<T>* pesquisarRegistroArvore(Arvore<T>* arvore, string nome)
{
	return pesquisarRegistroArvore(arvore->raiz, nome);
}

template <typename T>
No<T>* pesquisarRegistroArvore(No<T>* raiz, long long int& CPF)
{
	if (raiz == NULL) return NULL;

	if (raiz->pessoa->CPF == CPF) {
		return raiz;
	}

	if (raiz->pessoa->CPF > CPF) {
		return pesquisarRegistroArvore(raiz->esquerda, CPF);
	}

	return pesquisarRegistroArvore(raiz->direita, CPF);
}

template <typename T>
No<T>* pesquisarRegistroArvore(Arvore<T>* arvore, long long int CPF)
{
	return pesquisarRegistroArvore(arvore->raiz, CPF);
}

/// ROTACIONA A ARVORE ABAIXO
template <typename T>
No<T>* rotacao_simples_direita(No<T>* k)
{
	No<T>* x = k->esquerda;
	No<T>* t2 = x->direita;

	x->direita = k;
	k->esquerda = t2;
	k->altura = calcular_altura(k);
	x->altura = calcular_altura(x);
	return x;
}

template <typename T>
No<T>* rotacao_dupla_direita(No<T>* k)
{
	No<T>* x = k->esquerda;
	No<T>* w = x->direita;
	No<T>* t2 = w->esquerda;
	No<T>* t3 = w->direita;

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
No<T>* rotacao_simples_esquerda(No<T>* k)
{
	No<T>* y = k->direita;
	No<T>* t2 = y->esquerda;

	y->esquerda = k;
	k->direita = t2;
	k->altura = calcular_altura(k);
	y->altura = calcular_altura(y);
	return y;
}

template <typename T>
No<T>* rotacao_dupla_esquerda(No<T>* k)
{
	No<T>* y = k->direita;
	No<T>* z = y->esquerda;
	No<T>* t2 = z->esquerda;
	No<T>* t3 = z->direita;

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
No<T>* rotacao_avl(No<T>* k)
{
	if (buscar_altura(k->esquerda) > buscar_altura(k->direita)) { // Rota  o a direita
		No<T>* x = k->esquerda;
		if (buscar_altura(x->direita) > buscar_altura(x->esquerda))
			return rotacao_dupla_direita(k);
		else
			return rotacao_simples_direita(k);
	}
	else { // Rota  o a esquerda
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
No<T>* inserirArvoreOrdenadaCPF(No<T>* raiz, Pessoa* pessoa)
{
	if (raiz == NULL) {
		No<T>* novo = new No<T>;
		novo->pessoa = pessoa;
		novo->direita = NULL;
		novo->esquerda = NULL;
		novo->altura = calcular_altura(novo);
		return novo;
	}

	if (pessoa->CPF < raiz->pessoa->CPF)
		raiz->esquerda = inserirArvoreOrdenadaCPF(raiz->esquerda, pessoa);

	if (pessoa->CPF > raiz->pessoa->CPF)
		raiz->direita = inserirArvoreOrdenadaCPF(raiz->direita, pessoa);

	int alt_e = buscar_altura(raiz->esquerda);
	int alt_d = buscar_altura(raiz->direita);

	if (alt_e - alt_d == 2 || alt_e - alt_d == -2)
		raiz = rotacao_avl(raiz);
	else
		raiz->altura = (alt_e > alt_d) ? alt_e + 1 : alt_d + 1;

	return raiz;
}

template <typename T>
No<T>* inserirArvoreOrdenadaNome(No<T>* raiz, Pessoa* pessoa)
{
	if (raiz == NULL) {
		No<T>* novo = new No<T>;
		novo->pessoa = pessoa;
		novo->direita = NULL;
		novo->esquerda = NULL;
		novo->altura = calcular_altura(novo);
		return novo;
	}

	if (pessoa->nome < raiz->pessoa->nome)
		raiz->esquerda = inserirArvoreOrdenadaNome(raiz->esquerda, pessoa);

	if (pessoa->nome > raiz->pessoa->nome)
		raiz->direita = inserirArvoreOrdenadaNome(raiz->direita, pessoa);

	int alt_e = buscar_altura(raiz->esquerda);
	int alt_d = buscar_altura(raiz->direita);

	if (alt_e - alt_d == 2 || alt_e - alt_d == -2)
		raiz = rotacao_avl(raiz);
	else
		raiz->altura = (alt_e > alt_d) ? alt_e + 1 : alt_d + 1;

	return raiz;
}

template <typename T>
void inserirArvore(Arvore<T>& arvoreCPF, Arvore<T>& arvoreNome, long long int CPF, string nome, string profissao)
{
	if (pesquisarRegistroArvore(&arvoreCPF, CPF) != NULL) return;

	Pessoa* pessoa = new Pessoa;

	pessoa->CPF = CPF;
	pessoa->nome = nome;
	pessoa->profissao = profissao;

	arvoreCPF.raiz = inserirArvoreOrdenadaCPF(arvoreCPF.raiz, pessoa);
	arvoreNome.raiz = inserirArvoreOrdenadaNome(arvoreNome.raiz, pessoa);
}

template <typename T>
No<T>* retirarArvore(No<T>*& raiz, long long int& CPF)
{
	if (raiz == NULL) return NULL;

	if (raiz->pessoa->CPF == CPF) {
		if (raiz->esquerda == NULL && raiz->direita == NULL) {
			delete raiz;
			return NULL;
		}

		if (raiz->esquerda != NULL && raiz->direita == NULL) {
			No<T>* temp = raiz->esquerda;
			delete raiz;
			return temp;
		}

		if (raiz->esquerda == NULL && raiz->direita != NULL) {
			No<T>* temp = raiz->direita;
			delete raiz;
			return temp;
		}

		No<T>* maiorNoEsquerda = buscar_maior(raiz->esquerda);
		raiz->pessoa = maiorNoEsquerda->pessoa;

		CPF = raiz->pessoa->CPF;

		raiz->esquerda = retirarArvore(raiz->esquerda, CPF);

		int altura_esquerda = buscar_altura(raiz->esquerda);
		int altura_direita = buscar_altura(raiz->direita);

		if (altura_esquerda - altura_direita == 2 || altura_esquerda - altura_direita == -2)
			raiz = rotacao_avl(raiz);
		else
			raiz->altura = (altura_esquerda > altura_direita) ? altura_esquerda + 1 : altura_direita + 1;

		return raiz;
	}

	if (CPF < raiz->pessoa->CPF)
		raiz->esquerda = retirarArvore(raiz->esquerda, CPF);
	if (CPF > raiz->pessoa->CPF)
		raiz->direita = retirarArvore(raiz->direita, CPF);

	int altura_esquerda = buscar_altura(raiz->esquerda);
	int altura_direita = buscar_altura(raiz->direita);

	if (altura_esquerda - altura_direita == 2 || altura_esquerda - altura_direita == -2)
		raiz = rotacao_avl(raiz);
	else
		raiz->altura = (altura_esquerda > altura_direita) ? altura_esquerda + 1 : altura_direita + 1;

	return raiz;
}

template <typename T>
No<T>* retirarArvore(No<T>*& raiz, string& nome)
{
	if (raiz == NULL) return NULL;

	if (raiz->pessoa->nome == nome) {
		if (raiz->esquerda == NULL && raiz->direita == NULL) {
			delete raiz;
			return NULL;
		}

		if (raiz->esquerda != NULL && raiz->direita == NULL) {
			No<T>* temp = raiz->esquerda;
			delete raiz;
			return temp;
		}

		if (raiz->esquerda == NULL && raiz->direita != NULL) {
			No<T>* temp = raiz->direita;
			delete raiz;
			return temp;
		}

		No<T>* maiorNoEsquerda = buscar_maior(raiz->esquerda);
		raiz->pessoa = maiorNoEsquerda->pessoa;

		nome = raiz->pessoa->nome;

		raiz->esquerda = retirarArvore(raiz->esquerda, nome);

		int altura_esquerda = buscar_altura(raiz->esquerda);
		int altura_direita = buscar_altura(raiz->direita);

		if (altura_esquerda - altura_direita == 2 || altura_esquerda - altura_direita == -2)
			raiz = rotacao_avl(raiz);
		else
			raiz->altura = (altura_esquerda > altura_direita) ? altura_esquerda + 1 : altura_direita + 1;

		return raiz;
	}

	if (nome < raiz->pessoa->nome)
		raiz->esquerda = retirarArvore(raiz->esquerda, nome);
	if (nome > raiz->pessoa->nome)
		raiz->direita = retirarArvore(raiz->direita, nome);

	int altura_esquerda = buscar_altura(raiz->esquerda);
	int altura_direita = buscar_altura(raiz->direita);

	if (altura_esquerda - altura_direita == 2 || altura_esquerda - altura_direita == -2)
		raiz = rotacao_avl(raiz);
	else
		raiz->altura = (altura_esquerda > altura_direita) ? altura_esquerda + 1 : altura_direita + 1;

	return raiz;
}

template <typename T>
void retirarArvore(Arvore<T>& arvoreCPF, Arvore<T>& arvoreNome, long long int CPF)
{
	No<T>* temp = pesquisarRegistroArvore(&arvoreCPF, CPF);

	if (temp == NULL) return;

	Pessoa* pessoaExcluida = temp->pessoa;

	string nome = pessoaExcluida->nome;

	arvoreCPF.raiz = retirarArvore(arvoreCPF.raiz, CPF);
	arvoreNome.raiz = retirarArvore(arvoreNome.raiz, nome);

	delete pessoaExcluida;
}

template <typename T>
void retirarArvore(Arvore<T>& arvoreCPF, Arvore<T>& arvoreNome, string nome)
{
	No<T>* temp = pesquisarRegistroArvore(&arvoreNome, nome);

	if (temp == NULL) return;

	Pessoa* pessoaExcluida = temp->pessoa;

	long long int CPF = temp->pessoa->CPF;

	arvoreCPF.raiz = retirarArvore(arvoreCPF.raiz, CPF);
	arvoreNome.raiz = retirarArvore(arvoreNome.raiz, nome);

	delete pessoaExcluida;
}
/// INSERCAO E RETIRADA DA ARVORE ACIMA

/// METODOS DE CAMINHAMENTO ABAIXO
template <typename T>
void prefixado(No<T>* raiz)
{
	if (raiz == NULL) return;
	cout << setw(12) << left << raiz->pessoa->nome << " | " << setw(5) << left << raiz->pessoa->CPF << " | " << raiz->pessoa->profissao << endl;
	prefixado(raiz->esquerda);
	prefixado(raiz->direita);
}

template <typename T>
void prefixado(Arvore<T>& arvore)
{
	prefixado(arvore.raiz);
}

template <typename T>
void infixado(No<T>* raiz)
{
	if (raiz == NULL) return;
	infixado(raiz->esquerda);
	cout << setw(15) << left << raiz->pessoa->nome << " | " << setw(13) << left << raiz->pessoa->CPF << " | " << raiz->pessoa->profissao << endl;
	infixado(raiz->direita);
}

template <typename T>
void infixado(Arvore<T>& arvore)
{
	infixado(arvore.raiz);
}

template <typename T>
void posfixado(No<T>* raiz)
{
	if (raiz == NULL) return;
	posfixado(raiz->esquerda);
	posfixado(raiz->direita);
	cout << setw(12) << left << raiz->pessoa->nome << " | " << setw(5) << left << raiz->pessoa->CPF << " | " << raiz->pessoa->profissao << endl;
}

template <typename T>
void posfixado(Arvore<T>& arvore)
{
	posfixado(arvore.raiz);
}
/// METODOS DE CAMINHAMENTO ACIMA

void menu()
{
	cout << endl << "Insira o numero da operacao desejada (1 - 8)" << endl;
	cout << "1 - Inserir um novo registro" << endl;
	cout << "2 - Remover um registro por CPF" << endl;
	cout << "3 - Remover um registro por nome" << endl;
	cout << "4 - Pesquisar um registro por CPF" << endl;
	cout << "5 - Pesquisar um registro por nome" << endl;
	cout << "6 - Listar todas as informacoes cadastradas ordenadas por CPF" << endl;
	cout << "7 - Listar todas as informacoes cadastradas ordenadas por nome" << endl;
	cout << "8 - Limpar tela" << endl;
	cout << "9 - Sair" << endl;
}

int main()
{
	Arvore<void> ordenadaCPF;
	Arvore<void> ordenadaNome;

	inicializarArvore(ordenadaCPF);
	inicializarArvore(ordenadaNome);

	No<void>* aux = NULL;
	long long int CPF = 0;
	string nome = "", profissao = "";
	int opcao = 0;

	do {
		menu();
		cin >> opcao;

		switch (opcao) {
		case 1:

			system("cls");

			cout << endl << "Insira os dados da pessoa:" << endl << endl;
			cout << "CPF:" << endl;
			cin >> CPF;
			getchar();
			cout << "Nome:" << endl;
			getline(cin, nome);
			cout << "Profissao:" << endl;
			getline(cin, profissao);

			aux = pesquisarRegistroArvore(&ordenadaCPF, CPF);

			if (aux == NULL)
				inserirArvore(ordenadaCPF, ordenadaNome, CPF, nome, profissao);
			else
				cout << endl << "CPF ja cadastrado!" << endl << endl;

			aux = NULL;

			break;

		case 2:

			system("cls");

			getchar();
			cout << endl << "Insira o CPF para remover: ";
			cin >> CPF;
			retirarArvore(ordenadaCPF, ordenadaNome, CPF);

			break;

		case 3:

			system("cls");

			getchar();
			cout << endl << "Insira o nome completo para remover: ";
			getline(cin, nome);
			retirarArvore(ordenadaCPF, ordenadaNome, nome);

			break;

		case 4:

			system("cls");

			getchar();
			cout << endl << "Insira o CPF para pesquisar: ";
			cin >> CPF;

			aux = pesquisarRegistroArvore(&ordenadaCPF, CPF);

			if (aux != NULL)
				cout << setw(15) << left << aux->pessoa->nome << " | " << setw(13) << left << aux->pessoa->CPF << " | " << aux->pessoa->profissao << endl;

			aux = NULL;

			break;

		case 5:

			system("cls");

			getchar();
			cout << endl << "Insira o nome completo para pesquisar: ";
			getline(cin, nome);

			aux = pesquisarRegistroArvore(&ordenadaCPF, nome);

			if (aux != NULL)
				cout << setw(15) << left << aux->pessoa->nome << " | " << setw(13) << left << aux->pessoa->CPF << " | " << aux->pessoa->profissao << endl;

			aux = NULL;

			break;

		case 6:

			system("cls");

			cout << endl << "Tabela com todas as informacoes por ordem crescente de CPF:" << endl << endl;
			infixado(ordenadaCPF);

			break;

		case 7:

			system("cls");

			cout << endl << "Tabela com todas as informacoes por ordem crescente de nome:" << endl << endl;
			infixado(ordenadaNome);

			break;

		case 8:

			system("cls");

			break;

		default:
			break;
		}

	} while (opcao > 0 && opcao < 9);

	return 0;
}
