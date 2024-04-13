#include <Arduino.h>
#include "FCTUC.h"
#include "ListLib.h"

FCTUC bot;
struct Caminho
{
	int distancia;
	Vec2 *sequencia;
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
		if(tab[i] == 1) tab[5]++;
	}
    return tab;
}

bool existeNoCaminho(Vec2 vector, Caminho caminho) {
	for(int i = 0; i < caminho.distancia; i++) {
		if (vector.x == caminho.sequencia[i].x && vector.y == caminho.sequencia[i].y) {
			return true;
		}
	}
	return false;
}

Caminho recursivaCaminhos(byte *labirinto, Vec2 pos_atual, Vec2 pos_destination, Caminho caminho_atual) {
	// Check Walls
	int *tabela_walls = identificaParedes(GetWallsAtPos(pos_atual, labirinto));

	// Confirmar se as paredes são true
	/*for(int i = 0; i < 4; i++){
		
	}
	if(!walls.top && !existeNoCaminho(pos_atual.operator+ (vec_top), caminho_atual)) {
		
	}
	
	if(!walls.right && !existeNoCaminho(pos_atual.operator+ (vec_right), caminho_atual)) {
		
	}

	if(!walls.bottom && !existeNoCaminho(pos_atual.operator+ (vec_bottom), caminho_atual)) {
		
	}

	if(!walls.left && !existeNoCaminho(pos_atual.operator+ (vec_left), caminho_atual)) {

	}*/
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
						delete[] caminhoAux.sequencia;
						caminhoAux.sequencia = nullptr;
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

	//Walls walls = GetWallsAtPos(1, 1, lab); 
	//bot.println(walls.top);
	//bot.println(walls.bottom);
	//bot.println(walls.left);
	//bot.println(walls.right);
	//Vec2(const short, const short);

	Walls walls; 
	Vec2 pos;
	pos = bot.getRobotPosition();
	walls = GetWallsAtPos(pos, lab);
	bot.println("\n\nposicoes, x e y:");
	bot.println(pos.x);
	bot.println(pos.y);
	bot.println("posicoes: top, bottom, left, right, ");
	bot.println(walls.top);
	bot.println(walls.bottom);
	bot.println(walls.left);
	bot.println(walls.right);
}






void setup() {
	Serial.begin(115200);
	
	bot.begin();
	//bot.beginOTA("teste");

	bot.waitStart();

}

void loop() {
	//bot.println("prints");
	//bot.moveMotors(-1000, 1000);
  	delay(100);
	seeMap();
	delay(100);
}
