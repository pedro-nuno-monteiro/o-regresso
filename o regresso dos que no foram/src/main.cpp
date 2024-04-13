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
void marcha_tras(int x, int y);

void setup() {
	Serial.begin(115200);
	bot.begin();
	//bot.beginOTA("teste");
	bot.waitStart();
}

void loop() {

	// TEMOS DE VER PARA ONDE ESTÁ O ROBOT VIRADO!! X++ NEM SEMPRE É VIRAR DIREITA

	bot.println("a comecar");
	ponto_a_ponto();
	delay(10000);
}

void virar_esquerda() {
	bot.println("Turn left");
	bot.moveMotors(-350, 340);
	delay(325);
	bot.stopMotors();
	bot.println("a parar");

}

void virar_direita() {
	bot.println("Turn right");
	bot.moveMotors(340, -350);
	delay(325);
	bot.stopMotors();
}

void frente(int x, int y) {
    bot.println("Em frente!");
    bot.moveMotors(360, 350);
    
    bool parar = false;
    while (!parar) {
        //parar = bot.getTagDetected();
		vetor3 = bot.getRobotPosition();
		
		//bot.print("posicao vetor3, x = ");
		//bot.print(vetor3.x);
		//bot.print(" e y = ");
		//bot.println(vetor3.y);

        if (vetor3.x != x || vetor3.y != y)  {
			parar = true;
			bot.println("parar!!");
			bot.stopMotors();
        }
    }
}

// funcionalidade -> não parar em linhas retas

void marcha_tras(int x, int y) {
	bot.println("Marcha re");
	bot.moveMotors(-350, -350);
	
	bool parar = false;
	while (!parar) {
        //parar = bot.getTagDetected();
		vetor3 = bot.getRobotPosition();
        if (vetor3.x != x || vetor3.y != y)  {
			parar = true;
			bot.println("parar!!");
			bot.stopMotors();
        }
    }
}

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
		{3, 3}, 
		{3, 4}
    };

	int ciclo = 0;
	while (ciclo < 8) {

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
		//diferenca_x = posicoes_teste[ciclo + 1][0] - vetor2.x;
		//diferenca_y = posicoes_teste[ciclo + 1][1] - vetor2.y;
		diferenca_x = posicoes_teste[ciclo + 1][0] - posicoes_teste[ciclo][0];
		diferenca_y = posicoes_teste[ciclo + 1][1] - posicoes_teste[ciclo][1];

		bot.print("dif x = ");
		bot.print(diferenca_x);
		bot.print(" e dif y = ");
		bot.println(diferenca_y);
		
		// enviar para diferentes sítios
		if (diferenca_x == 1) {
			bot.println("vamos avancar para a direita");
			virar_esquerda();
			frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
		}
		else if (diferenca_x == -1) {
			bot.println("vamos avancar para a esquerda");
			virar_direita();
			frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
		}
		else if (diferenca_y == -1) {
			bot.println("vamos fazer marcha re");
			marcha_tras(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
		}
		else if (diferenca_y == 1) {
			bot.println("vamos andar em frente");
			frente(posicoes_teste[ciclo][0], posicoes_teste[ciclo][1]);
		}
		else {
			bot.println("A diferenca de posicoes deu mistake");
		}
		bot.println("novo ciclo\n\n");
		delay(1000);
		ciclo++;
	}

}