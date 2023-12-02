#include <iostream>

using namespace std;

class Pessoa {
private:
    string nome;
    int idade;
    string cpf;
public:
    Pessoa() {
        nome = "";
        idade = 0;
        cpf = "";
    }

    Pessoa(string nome, int idade, string cpf) {
        this->set_pessoa(nome, idade, cpf);
    }
    
    void set_pessoa(string nome, int idade, string cpf) {
        this->nome = nome;
        this->idade = idade;
        this->cpf = cpf;
    }
    void set_nome(string nome) { this->nome = nome; }
    void set_idade(int idade) { this->idade = idade; }
    void set_cpf(string cpf) { this->cpf = cpf; }

    string get_nome() { return this->nome; }
    int get_idade() { return this->idade; }
    string get_cpf() { return this->cpf; }
};

class No {
private:
    Pessoa *pessoa;
    No *esquerda;
    No *direita;
    int altura;
public:
    No() {
        this->pessoa = new Pessoa();
        this->esquerda = nullptr;
        this->direita = nullptr;
        this->altura = 0;
    }

    No(Pessoa &pessoa) : No() {
        this->pessoa->set_nome(pessoa.get_nome());
        this->pessoa->set_idade(pessoa.get_idade());
        this->pessoa->set_cpf(pessoa.get_cpf());
    }

    void set_pessoa(Pessoa *pessoa) { this->pessoa = pessoa; }
    void set_esquerda(No *esquerda) { this->esquerda = esquerda; }
    void set_direita(No *direita) { this->direita = direita; }
    void set_altura(int altura) { this->altura = altura; }

    Pessoa *get_pessoa() { return this->pessoa; }
    No *get_esquerda() { return this->esquerda; }
    No *get_direita() { return this->direita; }
    int get_altura() { return this->altura; }

    bool eh_folha() { return this->esquerda == nullptr && this->direita == nullptr; }
    int grau() { return (this->esquerda != nullptr) + (this->direita != nullptr); }
};

class Arvore {
private:
    No *raiz;
public:
    Arvore() { this->raiz = nullptr; }

    No *get_raiz() { return this->raiz; }

    void set_raiz(No *raiz) { this->raiz = raiz; }

    No *inserir(No *raiz, Pessoa &pessoa) {
        if(raiz == nullptr)
            return new No(pessoa);
        else if(pessoa.get_cpf() < raiz->get_pessoa()->get_cpf())
            raiz->set_esquerda(inserir(raiz->get_esquerda(), pessoa));
        else if(pessoa.get_cpf() > raiz->get_pessoa()->get_cpf())
            raiz->set_direita(inserir(raiz->get_direita(), pessoa));

        raiz->set_altura(maior(altura_no(raiz->get_esquerda()), altura_no(raiz->get_direita())) + 1);
        return balancear(raiz);
    }

    No *balancear(No *raiz) {
        int fb = fator_de_balanceamento(raiz);

        if(fb == 2) {
            if(fator_de_balanceamento(raiz->get_esquerda()) >= 0)
                raiz = rotacao_direita(raiz);
            else
                raiz = rotacao_esquerda_direita(raiz);
        }
        else if(fb == -2) {
            if(fator_de_balanceamento(raiz->get_direita()) <= 0)
                raiz = rotacao_esquerda(raiz);
            else
                raiz = rotacao_direita_esquerda(raiz);
        }

        return raiz;
    }

    int fator_de_balanceamento(No *raiz) {
        if(raiz == nullptr)
            return 0;
        return altura_no(raiz->get_esquerda()) - altura_no(raiz->get_direita());
    }

    No *rotacao_esquerda(No *r) {
        No *y = r->get_direita();
        No *f = y->get_esquerda();

        y->set_esquerda(r);
        r->set_direita(f);

        r->set_altura(maior(altura_no(r->get_esquerda()), altura_no(r->get_direita())) + 1);
        y->set_altura(maior(altura_no(y->get_esquerda()), altura_no(y->get_direita())) + 1);

        return y;
    }

    No *rotacao_direita(No *r) {
        No *y = r->get_esquerda();
        No *f = y->get_direita();

        y->set_direita(r);
        r->set_esquerda(f);

        r->set_altura(maior(altura_no(r->get_esquerda()), altura_no(r->get_direita())) + 1);
        y->set_altura(maior(altura_no(y->get_esquerda()), altura_no(y->get_direita())) + 1);

        return y;
    }

    No *rotacao_esquerda_direita(No *r) {
        r->set_esquerda(rotacao_esquerda(r->get_esquerda()));
        return rotacao_direita(r);
    }

    No *rotacao_direita_esquerda(No *r) {
        r->set_direita(rotacao_direita(r->get_direita()));
        return rotacao_esquerda(r);
    }

    int maior(int a, int b) {
        return (a > b) ? a : b;
    }

    int altura_no(No *raiz) {
        if(raiz == nullptr)
            return -1;
        return raiz->get_altura();
    }

    No *minimo(No *raiz) {
        if(raiz->get_esquerda() != nullptr)
            return minimo(raiz->get_esquerda());
        else
            return raiz;
    }

    No *remover(No *raiz, string cpf) {
        if(raiz == nullptr)
            return nullptr;
        if(cpf < raiz->get_pessoa()->get_cpf()) {
            raiz->set_esquerda(remover(raiz->get_esquerda(), cpf));
        }
        else if(cpf > raiz->get_pessoa()->get_cpf()) {
            raiz->set_direita(remover(raiz->get_direita(), cpf));
        }
        else {
            if(raiz->get_direita() == nullptr || raiz->get_esquerda() == nullptr) {
                No *aux = (raiz->get_direita() == nullptr) ? raiz->get_esquerda() : raiz->get_direita();
                cout << "vou deletar a pessoa de cpf: " << raiz->get_pessoa()->get_cpf() << endl;
                delete raiz->get_pessoa();
                delete raiz;
                return aux;
            }
            else {
                No *sucessor = minimo(raiz->get_direita());
                Pessoa *aux = raiz->get_pessoa();
                raiz->set_pessoa(sucessor->get_pessoa());
                sucessor->set_pessoa(aux);
                raiz->set_direita(remover(raiz->get_direita(), sucessor->get_pessoa()->get_cpf()));
            }
        }

        raiz->set_altura(maior(altura_no(raiz->get_esquerda()), altura_no(raiz->get_direita()))+1);
        return balancear(raiz);
    }

    void imprimir_pessoa(Pessoa *pessoa) {
        cout << "Nome: " << pessoa->get_nome() << ", Idade: " << pessoa->get_idade() << ", CPF: " << pessoa->get_cpf();
    }

    void imprimir(No *raiz, int nivel) {
        if(raiz != nullptr) {
            imprimir(raiz->get_direita(), nivel+1);
            cout << "\n\n";

            for(int i = 0; i < nivel; i++)
                cout << "\t";
            
            this->imprimir_pessoa(raiz->get_pessoa());
            imprimir(raiz->get_esquerda(), nivel+1);
        }
    }
};

int main(void) {

    Arvore arvore;
    Pessoa pessoa;
    int opcao, idade;
    string nome, cpf;

    do {
        printf("\n\n0 - Sair\n1 - Inserir\n2 - Remover\n3 - Imprimir\n\n");
        cin >> opcao;

        switch(opcao) {
        case 1:
            printf("\nDigite o nome da pessoa: ");
            getchar();
            getline(cin, nome);
            printf("\nDigite a idade da pessoa: ");
            cin >> idade;
            printf("\nDigite o cpf da pessoa: ");
            cin >> cpf;
            pessoa.set_pessoa(nome, idade, cpf);
            arvore.set_raiz(arvore.inserir(arvore.get_raiz(), pessoa));
            break;
        case 2:
            printf("\nDigite o cpf da pessoa que deseja remover: ");
            cin >> cpf;
            arvore.set_raiz(arvore.remover(arvore.get_raiz(), cpf));
            break;
        case 3:
            arvore.imprimir(arvore.get_raiz(), 1);
            break;
        default:
            break;
        }
    } while(opcao != 0);

    return 0;
}
