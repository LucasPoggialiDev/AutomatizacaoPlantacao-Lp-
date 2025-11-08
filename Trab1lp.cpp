#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// estrutura dos dados
struct DadosSolo {
    float temperatura;
    float umidade;
    float salinidade;
    DadosSolo* prox;
};

// insere no final
void inserir(DadosSolo*& inicio, float temp, float umid, float sal) {
    DadosSolo* novo = new DadosSolo;
    novo->temperatura = temp;
    novo->umidade = umid;
    novo->salinidade = sal;
    novo->prox = nullptr;
    
    if (inicio == nullptr) {
        inicio = novo;
        return;
    }
    
    DadosSolo* aux = inicio;
    while (aux->prox != nullptr) {
        aux = aux->prox;
    }
    aux->prox = novo;
}

void remover(DadosSolo*& inicio, DadosSolo* alvo) {
    if (inicio == nullptr || alvo == nullptr) return;
    
    if (inicio == alvo) {
        inicio = inicio->prox;
        delete alvo;
        return;
    }
    
    DadosSolo* aux = inicio;
    while (aux->prox != nullptr && aux->prox != alvo) {
        aux = aux->prox;
    }
    
    if (aux->prox == alvo) {
        aux->prox = alvo->prox;
        delete alvo;
    }
}

// calcula media tirando os extremos
void calcularMedias(DadosSolo* inicio, float& mediaTemp, float& mediaUmid, float& mediaSal) {
    if (inicio == nullptr) return;
    
    DadosSolo* menorTemp = inicio;
    DadosSolo* maiorTemp = inicio;
    DadosSolo* menorUmid = inicio;
    DadosSolo* maiorUmid = inicio;
    DadosSolo* menorSal = inicio;
    DadosSolo* maiorSal = inicio;
    
    int cont = 0;
    float somaTemp = 0, somaUmid = 0, somaSal = 0;
    
    DadosSolo* aux = inicio;
    while (aux != nullptr) {
        if (aux->temperatura < menorTemp->temperatura) menorTemp = aux;
        if (aux->temperatura > maiorTemp->temperatura) maiorTemp = aux;
        if (aux->umidade < menorUmid->umidade) menorUmid = aux;
        if (aux->umidade > maiorUmid->umidade) maiorUmid = aux;
        if (aux->salinidade < menorSal->salinidade) menorSal = aux;
        if (aux->salinidade > maiorSal->salinidade) maiorSal = aux;
        
        somaTemp += aux->temperatura;
        somaUmid += aux->umidade;
        somaSal += aux->salinidade;
        cont++;
        aux = aux->prox;
    }
    
    somaTemp -= (menorTemp->temperatura + maiorTemp->temperatura);
    somaUmid -= (menorUmid->umidade + maiorUmid->umidade);
    somaSal -= (menorSal->salinidade + maiorSal->salinidade);
    
    mediaTemp = somaTemp / (cont - 2);
    mediaUmid = somaUmid / (cont - 2);
    mediaSal = somaSal / (cont - 2);
}

// logica pra decidir se irriga ou nao
bool precisaIrrigar(float temp, float umid, float sal) {
    if (umid < 40.0) return true;
    if (temp > 30.0 && umid < 60.0) return true;
    if (sal > 2.5 && umid < 50.0) return true;
    return false;
}

void limparLista(DadosSolo*& inicio) {
    while (inicio != nullptr) {
        DadosSolo* temp = inicio;
        inicio = inicio->prox;
        delete temp;
    }
}

int main() {
    srand(time(0));
    
    DadosSolo* dados = nullptr;
    ofstream arquivo("relatorio_irrigacao.txt");
    
    cout << "Sistema de monitoramento do solo" << endl;
    cout << "Coletando dados a cada 15 minutos por 4 horas..." << endl << endl;
    
    arquivo << "Relatorio de irrigacao do solo" << endl << endl;
    
    for (int i = 1; i <= 16; i++) {
        float temp = 15.0 + (rand() % 250) / 10.0;
        float umid = 20.0 + (rand() % 600) / 10.0;
        float sal = 0.5 + (rand() % 300) / 100.0;
        
        inserir(dados, temp, umid, sal);
        
        cout << "Coleta " << i << " (t=" << (i*15) << "min): ";
        cout << fixed << setprecision(1);
        cout << "temp=" << temp << "C, ";
        cout << "umid=" << umid << "%, ";
        cout << "sal=" << sal << "dS/m" << endl;
        
        arquivo << "Coleta " << i << " - tempo: " << (i*15) << " min" << endl;
        arquivo << "  Temperatura: " << temp << " C" << endl;
        arquivo << "  Umidade: " << umid << " %" << endl;
        arquivo << "  Salinidade: " << sal << " dS/m" << endl << endl;
    }
    
    float mediaTemp, mediaUmid, mediaSal;
    calcularMedias(dados, mediaTemp, mediaUmid, mediaSal);
    
    cout << endl << "Medias (sem maior e menor):" << endl;
    cout << "Temperatura media: " << mediaTemp << " C" << endl;
    cout << "Umidade media: " << mediaUmid << " %" << endl;
    cout << "Salinidade media: " << mediaSal << " dS/m" << endl << endl;
    
    arquivo << "Medias calculadas (removendo maior e menor valor):" << endl;
    arquivo << "Temperatura media: " << mediaTemp << " C" << endl;
    arquivo << "Umidade media: " << mediaUmid << " %" << endl;
    arquivo << "Salinidade media: " << mediaSal << " dS/m" << endl << endl;
    
    bool irrigar = precisaIrrigar(mediaTemp, mediaUmid, mediaSal);
    
    cout << "Decisao: ";
    arquivo << "Decisao de irrigacao: ";
    
    if (irrigar) {
        cout << "irrigar" << endl;
        arquivo << "irrigar" << endl;
        arquivo << "Motivo: condicoes do solo indicam necessidade de agua" << endl;
    } else {
        cout << "nao irrigar" << endl;
        arquivo << "nao irrigar" << endl;
        arquivo << "Motivo: condicoes do solo estao adequadas" << endl;
    }
    
    limparLista(dados);
    arquivo.close();
    
    cout << endl << "Relatorio salvo em 'relatorio_irrigacao.txt'" << endl;
    
    return 0;
}
