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
void deteta_millis(int timeout);

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
	int inicio = millis();
	bot.println("Turn left");
	bot.moveMotors(-375, 400);
	delay(330);
	//deteta_millis(330);
	bot.stopMotors();
	bot.println("a parar");
}

/*void deteta_millis(int timeout){
	int inicio = millis();

	do {
		
	} while (millis() - inicio < timeout);
	
	

}*/

void virar_direita() {
	bot.println("Turn right");
	bot.moveMotors(400, -365);
	delay(330);
	bot.stopMotors();
}

void frente(int x, int y) {
    
    bool parar = false;
    while (!parar) {
		bot.println("Em frente!");
    	bot.moveMotors(355, 350);
		vetor3 = bot.getRobotPosition();

		bot.print("posicao dentro do !parar: x = ");
		bot.print(vetor3.x);
		bot.print(", y = ");
		bot.println(vetor3.y);

		uint16_t distancia_direita = bot.getLidarRightDistance();
		uint16_t distancia_frente = bot.getLidarFrontDistance();
		uint16_t distancia_esquerda = bot.getLidarLeftDistance();

		bot.print("distancia: direita, frente e esquerda. direita: ");
		bot.print(distancia_direita);
		bot.print(", frente: ");
		bot.print(distancia_frente);
		bot.print(", esquerda: ");
		bot.println(distancia_esquerda);

		if (distancia_frente < 70) { 
			distancia_frente = bot.getLidarFrontDistance();
			bot.println("redirecionando frente");
			bot.moveMotors(-200, -200);
			delay(200);
			bot.stopMotors();
		}
		else if (distancia_esquerda < 110) {
			bot.println("esquerda");
			while (distancia_esquerda < 110) {
				distancia_esquerda = bot.getLidarLeftDistance();
				bot.println("redirecionando esquerda");
				bot.moveMotors(-100, -100);
			}
			bot.moveMotorLeft(455);
		}
		else if (distancia_direita < 110) {
			bot.println("direita");
			while (distancia_direita < 110) {
				distancia_direita = bot.getLidarRightDistance();
				bot.println("redirecionando direita");
				bot.moveMotors(-100, -100);
			}
			bot.moveMotorRight(450);
		}

		


        if (vetor3.x != x || vetor3.y != y)  {
			parar = true;
			bot.println("parar!!");
			bot.stopMotors();
        }
    }
}

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
	while (ciclo < 13) {

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

		bool marcha_tras = false;
		if (ciclo > 0) {
			diferenca_x_anterior = posicoes_teste[ciclo][0] - posicoes_teste[ciclo - 1][0];
			diferenca_y_anterior = posicoes_teste[ciclo][1] - posicoes_teste[ciclo - 1][1];
			
			bot.print("dif anterior x = ");
			bot.print(diferenca_x_anterior);
			bot.print(" e dif anterior y = ");
			bot.println(diferenca_y_anterior);

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

		delay(15);
		ciclo++;
	}

	bot.stopMotors();
}
