#include <Arduino.h>
#include "FCTUC.h"

FCTUC bot;

Vec2 vetor, vetor2;

Walls walls;

byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
		0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
		0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
		0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
	};

void ponto_a_ponto();
void virar_esquerda();
void virar_direita();
void frente();
void marcha_tras();

void setup() {
	Serial.begin(115200);
	bot.begin();
	//bot.beginOTA("teste");
	bot.waitStart();
}

void loop() {
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

void frente() {
    bot.println("Em frente!");
    bot.moveMotors(360, 350);
    
    bool parar = false;
    while (!parar) {
        parar = bot.getTagDetected();
        if (parar) {
			bot.println("parar!!");
			bot.stopMotors();
        }
    }
}

// funcionalidade -> não parar em linhas retas

void marcha_tras() {
	bot.println("Marcha re");
	bot.moveMotors(-350, -350);
	
	bool parar = false;
    while (!parar) {
        parar = bot.getTagDetected();
        if (parar) {
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
		bot.print("Estamos numa nova posicao: ");
		bot.print(posicoes_teste[ciclo][0]);
		bot.print(" e ");
		bot.println(posicoes_teste[ciclo][1]);
		vetor2 = bot.getRobotPosition();
		bot.print("realidade: ");
		bot.print(vetor2.x);
		bot.print(" e ");
		bot.println(vetor2.y);
		int diferenca_x = 0;
		int diferenca_y = 0;
		for(int i = 0; i < 7; i++) { 	// perceber alteração do valor
			diferenca_x = posicoes_teste[i + 1][0] - vetor2.x;
			diferenca_y = posicoes_teste[i + 1][1] - vetor2.y;
		}


		bot.println("dif x");
		bot.println(diferenca_x);
		bot.println("dif y");
		bot.println(diferenca_y);
		
		// enviar para diferentes sítios
		if (diferenca_x == 1) {
			bot.println("vamos avancar para a direita");
			virar_direita();
			frente();
		}
		else if (diferenca_x == -1) {
			bot.println("vamos avancar para a esquerda");
			virar_esquerda();
			frente();
		}
		else if (diferenca_y == -1) {
			bot.println("vamos fazer marcha re");
			marcha_tras();
		}
		else if (diferenca_y == 1) {
			bot.println("vamos andar em frente");
			frente();
		}
		else {
			bot.println("A diferenca de posicoes deu mistake");
		}
		bot.println("novo ciclo");
		delay(1000);
		ciclo++;
	}

}