#include <Arduino.h>
#include "FCTUC.h"
#include "ListLib.h"

FCTUC bot;
struct Caminho
{
	int distancia;
	List<Vec2> sequencia;
};

const Vec2 vec_top = Vec2(0, -1);
const Vec2 vec_right = Vec2(1, 0);
const Vec2 vec_bottom = Vec2(0, 1);
const Vec2 vec_left = Vec2(-1, 0);

List<Caminho> lista_caminhos;

int *identificaParedes(Walls paredes) {
    int tab[5] = {0};
	if (paredes.top) tab[0] = 1;
	if (paredes.right) tab[1] = 1;
	if (paredes.bottom) tab[2] = 1;
	if (paredes.left) tab[3] = 1;
	for (int i = 0; i < 4; i++){
		if(tab[i] == 0) tab[4]++;
	}
    return tab;
}

bool existeNoCaminho(Vec2 vector, Caminho caminho) {
	for(int i = 0; i < caminho.sequencia.Count(); i++) {
		if (vector.x == caminho.sequencia[i].x && vector.y == caminho.sequencia[i].y) {
			return true;
		}
	}
	return false;
}

Vec2 mudaPosicao(Vec2 vector, int id) {
	Vec2 vector_novo;
	if(id == 0) {
		vector_novo = vector.operator+(vec_top);
	}
	if(id == 1) {
		vector_novo = vector.operator+=(vec_right);
	}
	if(id == 2) {
		vector_novo = vector.operator+=(vec_bottom);
	}
	if(id == 3) {
		vector_novo = vector.operator+=(vec_left);
	}

	return vector_novo;
}

void recursivaCaminhos(byte *labirinto, Vec2 pos_atual, Vec2 pos_destination, Caminho caminho_atual) {
	bool leave = false;
	int id_direcao;
	// Check Walls
	int *tabela_walls = identificaParedes(GetWallsAtPos(pos_atual, labirinto));
	
	while(!leave) {
		if(pos_atual == pos_destination) { // Se chegou ao destino
			lista_caminhos.Add(caminho_atual);
			leave = true;
			return ;
		}
		else if(pos_atual == caminho_atual.sequencia[0] && tabela_walls[4] == 1) {  // Se é a primeira posição
			for(int i = 0; i < 4; i++) {
				if(tabela_walls[i] == 0) {
					Vec2 pos_nova = mudaPosicao(pos_atual, i);
					if(!existeNoCaminho(pos_nova, caminho_atual)) {
						pos_atual = pos_nova;
						caminho_atual.sequencia.Add(pos_nova);
						caminho_atual.distancia++;
					}
					else{
						lista_caminhos.Add(caminho_atual);
						leave = true;
						return;
					}
					break;
				}
			}
		}	
		else if(pos_atual != caminho_atual.sequencia[0] && tabela_walls[4] == 1) { // Se só tem uma opção para onde ir mas veio dela anteriormente
			lista_caminhos.Add(caminho_atual);
			leave = true;
			return ;
		}
		else if(tabela_walls[4] > 1) { // Se tiver mais do que uma opção para onde ir 
			bool apareceu = false;
			for(int i = 0; i < 4; i++) {
				if(tabela_walls[i] == 0 && !apareceu) { // Se for o primeiro sem paredes
					// confirmar se já apareceu e dar return e leave = true
					Vec2 pos_nova = mudaPosicao(pos_atual, i);
					if(!existeNoCaminho(pos_nova, caminho_atual)) {
						pos_atual = pos_nova;
						caminho_atual.sequencia.Add(pos_nova);
						caminho_atual.distancia++;
					}
					else{
						lista_caminhos.Add(caminho_atual);
						leave = true;
						return;
					}
					break;
					apareceu = true;
				}
				else if(tabela_walls[i] == 0 && apareceu) {
					Vec2 pos_nova = mudaPosicao(pos_atual, i);
					if(!existeNoCaminho(pos_nova, caminho_atual)) {
						pos_atual = pos_nova;
						caminho_atual.sequencia.Add(pos_nova);
						caminho_atual.distancia++;
						recursivaCaminhos(labirinto, pos_atual, pos_destination, caminho_atual);
						leave = true;
						continue;
					}
					else{
						lista_caminhos.Add(caminho_atual);
						leave = true;
						return;
					}
					break;
				}
			}
			leave = true;
		}
	}
	// Por cada opção nova criar uma outra recursiva (se não for uma casa por onde já passou) (número de opções -1)
	// Se for 0 retorna
	// Se só houver uma opção para além da de onde viemos continua na mesma

	// Guarda o caminho
}

void printCaminhos(){
	for(int i = 0; i < lista_caminhos.Count(); i++){
		bot.print(lista_caminhos[i].distancia);
		for(int j = 0; j < lista_caminhos[j].distancia; j++){
			bot.print(lista_caminhos[i].sequencia[j].x);
			bot.print(lista_caminhos[i].sequencia[j].y);
		}
	}
}


Vec2 *caminhoEficaz(byte *labirinto, Vec2 pos_robot, Vec2 pos_destination){
	Caminho caminhoAux;
	Caminho caminhoPequeno;
	boolean leave = false;
	while (!leave){ // Enquanto o melhor caminho não for encontrado
		boolean destino_encontrado = false;
		Vec2 pos_atual = pos_robot;
		if (caminhoPequeno.distancia == 0){ // Primeira Iteração -> preenche caminhoPequeno
			while (!destino_encontrado){	
				while (1) { // Contar todas as opções possíveis
					
				}
				caminhoPequeno.distancia++;
				if (pos_atual == pos_destination){
					destino_encontrado = true;
				}
			}
		}
		else { // Resto das iterações -> preenche o caminhoAux e compara com caminhoPequeno
			while (!destino_encontrado || (caminhoAux.distancia > caminhoPequeno.distancia)){	
					caminhoAux.distancia++;
					// Correr 
					if (pos_atual == pos_destination){
						destino_encontrado = true;
						if (caminhoAux.distancia < caminhoPequeno.distancia){
							// Substituir o caminhoPequeno pelo novo caminhoAux
							caminhoPequeno = caminhoAux;
						}
					}
					if (caminhoAux.distancia > caminhoPequeno.distancia){
						caminhoAux.distancia = 0;
						//delete[] caminhoAux.sequencia;
						//caminhoAux.sequencia = nullptr;
					}
			}
		}
	}
} 

void seeMap() {
	byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
			0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
			0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
			0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
		};

	Vec2 pos_inicial = Vec2(0,6);
	Vec2 pos_final = Vec2(7,4);
	Caminho caminho_inicial;
	caminho_inicial.distancia = 0;
	caminho_inicial.sequencia.Add(pos_inicial);
	recursivaCaminhos(lab, pos_inicial, pos_final, caminho_inicial);
	printCaminhos;
}






void setup() {
	Serial.begin(115200);
	
	bot.begin();
	//bot.beginOTA("teste");

	bot.waitStart();
}

int contar = 0;

void loop() {
	//bot.println("prints");
	//bot.moveMotors(-1000, 1000);
	
  	delay(100);
	if (contar == 0)
	{
		seeMap();
		contar++;
	}
	delay(100);
}
