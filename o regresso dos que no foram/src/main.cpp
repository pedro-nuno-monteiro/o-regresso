#include <Arduino.h>
#include "FCTUC.h"

FCTUC bot;

Vec2 vetor, vetor2, vetor3;

Walls walls;

byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
		0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
		0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
		0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
	};

void ponto_a_ponto();
void virar_esquerda();
void virar_direita();
void frente(int x, int y);

void setup() {
	Serial.begin(115200);
	bot.begin();
	bot.beginOTA("teste");
	bot.waitStart();
}

Vec2 teste;

void loop() {
	bot.println("a comecar");
	ponto_a_ponto();
	delay(5000);
}

void virar_esquerda() {
	bot.println("Turn left");
	bot.moveMotors(-375, 400);
	delay(330);
	bot.stopMotors();
	bot.println("a parar");
}

void virar_direita() {
	bot.println("Turn right");
	bot.moveMotors(400, -365);
	delay(330);
	bot.stopMotors();
}

void frente(int x, int y) {
    bot.println("Em frente!");
    bot.moveMotors(355, 350);
    
    bool parar = false;
    while (!parar) {
		vetor3 = bot.getRobotPosition();

		int16_t distancia_direita = bot.getLidarRightDistance();
		uint16_t distancia_frente = bot.getLidarFrontDistance();
		uint16_t distancia_esquerda = bot.getLidarLeftDistance();

		if (distancia_direita < 700) {
			while (distancia_direita < 700) {
				bot.moveMotorRight(370);
				delay(200);
				bot.moveMotorRight(350);
			}
		}
		else if (distancia_esquerda < 700) {
			while (distancia_esquerda < 700) {
				bot.moveMotorLeft(375);
				delay(200);
				bot.moveMotorRight(355);
			}
		}
		else if (distancia_frente < 500) {
			bot.moveMotors(-200, -200);
			delay(200);
			bot.stopMotors();
		}


        if (vetor3.x != x || vetor3.y != y)  {
			parar = true;
			bot.println("parar!!");
			bot.stopMotors();
        }
    }
}

// funcionalidade -> não parar em linhas retas

// so ideias

/*
void calibrar_paredes(){

	uint16_t distancia_direita = bot.getLidarRightDistance();
	uint16_t distancia_frente = bot.getLidarFrontDistance();
	uint16_t distancia_esquerda = bot.getLidarLeftDistance();
	
	bool linha_reta = true;
	
	while (distancia_direita < 1200){
		bot.moveMotorRight(100);
	}

	while(distancia_frente < 1200){
		bot.moveMotors(-105, -100);
	}

	while(distancia_esquerda < 1200){
		bot.moveMotorLeft(100);
	}

	ponto_a_ponto();

}*/

// DENTRO DESTA FUNCAO COLOCAR UM
void ponto_a_ponto() {
	vetor = bot.getRobotPosition();

	bot.print("O Bot esta nesta posicao, ");
	bot.print(vetor.x);
	bot.print(" e ");
	bot.println(vetor.y);

	// devole top, right, bottom, left
	// devolve 1 onde há parede
	walls = GetWallsAtPos(vetor.x, vetor.y, lab); 
	
	int posicoes_teste[][2] = {
        {0, 0}, 
		{0, 1}, 
		{0, 2}, 
		{0, 3}, 
        {1, 3}, // 1 = posicoes_teste[4][0]
		{2, 3}, 
		{2, 4},
		{3, 4}, 
		{3, 3}, 
		{4, 3}, 
		{4, 2}, 
        {3, 2},
		{3, 1}, 
		{4, 1}
    };

	int ciclo = 0;
	while (ciclo < 14) {

		bot.print("Posicoes teste diz x = ");
		bot.print(posicoes_teste[ciclo][0]);
		bot.print(" e y = ");
		bot.println(posicoes_teste[ciclo][1]);
		
		vetor2 = bot.getRobotPosition();
		bot.print("posicao real x = ");
		bot.print(vetor2.x);
		bot.print(" e y = ");
		bot.println(vetor2.y);
		
		int diferenca_x = 0;
		int diferenca_y = 0;
		int diferenca_x_anterior = 0;
		int diferenca_y_anterior = 0;

		//diferenca_x = posicoes_teste[ciclo + 1][0] - vetor2.x;
		//diferenca_y = posicoes_teste[ciclo + 1][1] - vetor2.y;
		diferenca_x = posicoes_teste[ciclo + 1][0] - posicoes_teste[ciclo][0];
		diferenca_y = posicoes_teste[ciclo + 1][1] - posicoes_teste[ciclo][1];

		if (ciclo > 0) {
			diferenca_x_anterior = posicoes_teste[ciclo][0] - posicoes_teste[ciclo - 1][0];
			diferenca_y_anterior = posicoes_teste[ciclo][1] - posicoes_teste[ciclo - 1][1];
			
			bot.print("dif anterior x = ");
			bot.print(diferenca_x_anterior);
			bot.print(" e dif anterior y = ");
			bot.println(diferenca_y_anterior);
			bool marcha_tras = false;

			if (posicoes_teste[ciclo + 1][0] == posicoes_teste[ciclo - 1][0] && posicoes_teste[ciclo + 1][1] == posicoes_teste[ciclo - 1][1]){
				virar_esquerda();
				virar_esquerda();
				frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
				marcha_tras = true;
			}
		}

		bot.print("dif x = ");
		bot.print(diferenca_x);
		bot.print(" e dif y = ");
		bot.println(diferenca_y);
		
		// enviar para diferentes sítios
		if (!marcha_tras) {
			if (diferenca_x == 1) {
				if (diferenca_y_anterior == 1) {
					virar_esquerda();
					bot.println("vamos avancar para a esquerda");
				}
				else if (diferenca_y_anterior == -1) {
					virar_direita();
					bot.println("vamos avancar para a direita");
				}
				
				frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
			}

			else if (diferenca_x == -1) {
				if (diferenca_y_anterior == 1) {
					virar_direita();
					bot.println("vamos avancar para a direita");
				}
				else if (diferenca_y_anterior == -1) {
					virar_esquerda();
					bot.println("vamos avancar para a esquerda");
				}
				frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
			}
			// enviar para diferentes sítios
			else if (diferenca_y == 1) {
				if (diferenca_x_anterior == 1) {
					virar_direita();
					bot.println("vamos avancar para a esquerda");
				}
				else if (diferenca_x_anterior == -1) {
					virar_esquerda();
					bot.println("vamos avancar para a direita");
				}
				frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
			}

			else if (diferenca_y == -1) {
				if (diferenca_x_anterior == 1) {
					virar_esquerda();
					bot.println("vamos avancar para a esquerda");
				}
				else if (diferenca_x_anterior == -1) {
					virar_direita();
					bot.println("vamos avancar para a direita");
				}
					
				frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
			}
			else {
				bot.println("A diferenca de posicoes deu mistake");
			}
		}	
		bot.println("novo ciclo\n\n");

		if (ciclo == 13) {
			bot.stopMotors();
		}

		delay(1000);
		ciclo++;
	}

	bot.stopMotors();
}
