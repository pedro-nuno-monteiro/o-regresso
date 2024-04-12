#include <Arduino.h>
#include "FCTUC.h"

FCTUC bot;

Vec2 vetor;

Walls walls, walls_seguinte;

byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
		0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
		0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
		0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
	};

void ponto_a_ponto();
void virar_esquerda();
void virar_direita();

void setup() {
	Serial.begin(115200);
	bot.begin();
	bot.waitStart();
}

void loop() {
	ponto_a_ponto();

	bot.println("Tudo terminado, FINAL FUCKING MENTE");
	delay(10000);
}

void virar_esquerda() {
	bot.println("Turn left");
	bot.moveMotors(-250, 250);
	delay(1000);
}

void virar_direita() {
	bot.println("Turn right");
	bot.moveMotors(250, -250);
	delay(1000);
}

void frente() {
	bot.println("Turn right");
	bot.moveMotors(250, 250);
	
	bool parar = false;
	while (!parar) {
		parar = bot.getTagDetected();
		if (parar) {
			bot.stopMotors();
			bot.println("O bot encontrou um novo RFID! A parar engines.");
		}
	}
}

void marcha_tras() {
	bot.println("Marcha re");
	bot.moveMotors(-250, -250);
	
	bool parar = false;
	while (!parar) {
		parar = bot.getTagDetected();
		if (parar) {
			bot.stopMotors();
			bot.println("O bot encontrou um novo RFID! A parar engines.");
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
		int diferenca_x = 0;
		int diferenca_y = 0;
		for(int i = 0; i < 7; i++) { 	// perceber alteração do valor
			diferenca_x = posicoes_teste[i + 1][0] - posicoes_teste[i][0];
			diferenca_y = posicoes_teste[i + 1][1] - posicoes_teste[i][1];
		}

		// enviar para diferentes sítios
		if (diferenca_x == 1) {
			virar_direita();
			frente();
		}
		else if (diferenca_x == -1) {
			virar_esquerda();
			frente();
		}
		else if (diferenca_y == 1) {
			marcha_tras();
		}
		else if (diferenca_y == -1) {
			frente();
		}
		else {
			bot.println("A diferenca de posicoes deu mistake");
		}
		ciclo++;
	}

}