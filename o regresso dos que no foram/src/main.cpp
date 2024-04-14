#include <Arduino.h>
#include "FCTUC.h"
#include "ListLib.h"

FCTUC bot;

const Vec2 vec_top = Vec2(0, -1);
const Vec2 vec_right = Vec2(1, 0);
const Vec2 vec_bottom = Vec2(0, 1);
const Vec2 vec_left = Vec2(-1, 0);

Vec2 vetor, vetor2, vetor3;
Vec2 de_arrays;

Vec2* caminho;

Walls walls;


void ponto_a_ponto();
void virar_esquerda();
void virar_direita();
void frente(int x, int y);
void virar_paredes_primeiro();


int *identificaParedes(Walls paredes) {
    static int tab[4] = {0}; //----------------
	if (paredes.top) tab[0] = 1;
	else tab[0] = 0;
	if (paredes.right) tab[1] = 1;
	else tab[1] = 0;
	if (paredes.bottom) tab[2] = 1;
	else tab[2] = 0;
	if (paredes.left) tab[3] = 1;
	else tab[3] = 0;
    return tab;
}

int quantidadeBuracos(int tab[]){
	int contador = 0;
	for (int i = 0; i < 4; i++){
		if(tab[i] == 0) contador++;
	}
	return contador;
}

Vec2 mudaPosicao(Vec2 vector, int id) {
	Vec2 vector_novo;
	if(id == 0) {
		vector_novo = vector.operator+(vec_top);
	}
	if(id == 1) {
		vector_novo = vector.operator+(vec_right);
	}
	if(id == 2) {
		vector_novo = vector.operator+(vec_bottom);
	}
	if(id == 3) {
		vector_novo = vector.operator+(vec_left);
	}

	return vector_novo;
}

Vec2 *retornaPosicoesAdjacentes(byte * labirinto, Vec2& pos_atual, Vec2& pos_anterior, int buracos){
    int *tabela_walls = identificaParedes(GetWallsAtPos(pos_atual, labirinto));
    Vec2 *vetores = new Vec2[buracos]; // Dynamic array to store adjacent positions
    int count = 0; // Counter for available positions
    
    // Check each direction for available paths
    if (tabela_walls[0] == 0 && pos_atual != pos_anterior) {
        vetores[count++] = mudaPosicao(pos_atual, 0); // Add the position above
    }
    if (tabela_walls[1] == 0 && pos_atual != pos_anterior) {
        vetores[count++] = mudaPosicao(pos_atual, 1); // Add the position to the right
    }
    if (tabela_walls[2] == 0 && pos_atual != pos_anterior) {
        vetores[count++] = mudaPosicao(pos_atual, 2); // Add the position below
    }
    if (tabela_walls[3] == 0 && pos_atual != pos_anterior) {
        vetores[count++] = mudaPosicao(pos_atual, 3); // Add the position to the left
    }
    
    // Resize the array to remove unused slots
    Vec2 *result = new Vec2[count];
    for (int i = 0; i < count; i++) {
        result[i] = vetores[i];
    }
    delete[] vetores; // Free memory allocated for the dynamic array
    
    return result;
}

void calcula_proxima_pos(byte *labirinto, Vec2& pos_atual, Vec2 pos_destino, Vec2& pos_anterior){
    int *tabela_walls = identificaParedes(GetWallsAtPos(pos_atual, labirinto));
    int buracos = quantidadeBuracos(tabela_walls);
    
    bot.print("Current position: "); bot.print(pos_atual.x); bot.print(", "); bot.println(pos_atual.y);
    bot.print("Previous position: "); bot.print(pos_anterior.x); bot.print(", "); bot.println(pos_anterior.y);
    
    if (buracos == 1) {
        // Only one available path, move to that position
        for (int i = 0; i < 4; i++) {
            if (tabela_walls[i] == 0) {
                pos_anterior = pos_atual;
                pos_atual = mudaPosicao(pos_atual, i);
                bot.println("Moved to a new position.");
                return;
            }
        }
    } else if (buracos > 1) {
        // Multiple available paths, choose one
        Vec2 *adjacentes = retornaPosicoesAdjacentes(labirinto, pos_atual, pos_anterior, buracos);
        Vec2 *adjacentes_possiveis = new Vec2[buracos];
        int count_possiveis = 0;

        for (int i = 0; i < buracos; i++) {
            // Check if the position has 3 walls and is not the destination
            if (quantidadeBuracos(identificaParedes(GetWallsAtPos(adjacentes[i], labirinto))) <= 1 && adjacentes[i] != pos_destino) {
                continue; // Skip this position
            } else if (adjacentes[i] != pos_anterior) {
                adjacentes_possiveis[count_possiveis] = adjacentes[i];
                count_possiveis++;
            }
        }

        // Print available positions
        bot.println("Available positions:");
        for (int i = 0; i < count_possiveis; i++) {
            bot.print("Position "); bot.print(i); bot.print(": ");
            bot.print(adjacentes_possiveis[i].x); bot.print(", "); bot.println(adjacentes_possiveis[i].y);
        }

        // Choose a random position from the available ones
        if (count_possiveis > 0) {
            int random_index = random(0, count_possiveis); // Generate a random index within the range of possible positions
            // Move to the randomly chosen position
            pos_anterior = pos_atual;
            pos_atual = adjacentes_possiveis[random_index];
        } else {
            // If there are no available positions, return to the previous position
            Vec2 aux = pos_anterior;
			pos_anterior = pos_atual;
			pos_atual = aux;
            bot.println("No available positions. Returning to the previous position.");
        }
        
        // Free memory allocated for the arrays
        delete[] adjacentes;
        delete[] adjacentes_possiveis;
    }
}

Vec2* seeMap() {
	byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
			0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
			0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
			0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
		};

	Vec2 pos_atual = Vec2(9,9); //GetRobotPosition();
	Vec2 pos_final = Vec2(0,0); //GetThiefPosition();
    Vec2 pos_anterior=Vec2(-1,-1);

	Vec2* caminho = new Vec2[50];

	int caminhoSize = 0; // Keep track of the size of caminho	
    while(pos_atual.operator!=(pos_final)) {
		caminho[caminhoSize++] = pos_atual; // Add the current position to caminho
        calcula_proxima_pos(lab, pos_atual, pos_final, pos_anterior);
		bot.println(" ");
    }
    
	bot.println("CHEGOU AO DESTINO");

    return caminho;
}

void setup() {
	Serial.begin(115200);
	bot.begin();
	bot.beginOTA("teste");

	bot.waitStart();
	randomSeed(millis());
	caminho = seeMap();
}

void loop() {
	ponto_a_ponto();
    delay(50000);
}

void virar_esquerda() {
	int inicio = millis();
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

		if (distancia_frente < 50) {
			distancia_frente = bot.getLidarFrontDistance();
			bot.println("redirecionando frente");
			bot.moveMotors(-100, -100);
			delay(200);
			bot.stopMotors();
		}
		else if (distancia_direita < 110) {
			bot.println("direita");
			while (distancia_direita < 110) {
				distancia_direita = bot.getLidarRightDistance();
				bot.println("redirecionando direita");
				bot.moveMotors(-100, -100);
			}
			bot.moveMotorRight(370);
		}
		else if (distancia_esquerda < 110) {
			bot.println("esquerda");
			while (distancia_esquerda < 110) {
				distancia_esquerda = bot.getLidarLeftDistance();
				bot.println("redirecionando esquerda");
				bot.moveMotors(-100, -100);
			}
			bot.moveMotorLeft(375);
		}

		
		if(distancia_frente < 80 && distancia_direita >= 110 && distancia_esquerda >= 110){
			virar_direita();
			virar_direita();
			if(distancia_frente < 80 && distancia_direita >= 110 && distancia_esquerda >= 110) {
				virar_esquerda();
			}
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

	size_t array_size = sizeof(caminho) / sizeof(caminho[0]);
	int ciclo = 0;
	while (ciclo < array_size) {

		bot.print("Posicoes teste diz x = ");
		bot.print(caminho[ciclo].x);
		bot.print(" e y = ");
		bot.println(caminho[ciclo].y);
		
		vetor2 = bot.getRobotPosition();
		bot.print("posicao real x = ");
		bot.print(vetor2.x);
		bot.print(" e y = ");
		bot.println(vetor2.y);
		
		int diferenca_x = 0;
		int diferenca_y = 0;
		int diferenca_x_anterior = 0;
		int diferenca_y_anterior = 0;

		//diferenca_x = caminho[ciclo + 1][0] - vetor2.x;
		//diferenca_y = caminho[ciclo + 1][1] - vetor2.y;
		diferenca_x = caminho[ciclo + 1].x - caminho[ciclo].x;
		diferenca_y = caminho[ciclo + 1].y - caminho[ciclo].y;

		bool marcha_tras = false;
	
		if (ciclo > 0) {
			diferenca_x_anterior = caminho[ciclo].x - caminho[ciclo - 1].x;
			diferenca_y_anterior = caminho[ciclo].y - caminho[ciclo - 1].y;
			
			bot.print("dif anterior x = ");
			bot.print(diferenca_x_anterior);
			bot.print(" e dif anterior y = ");
			bot.println(diferenca_y_anterior);

			if (caminho[ciclo + 1].x == caminho[ciclo - 1].x && caminho[ciclo + 1].y == caminho[ciclo - 1].y){
				virar_esquerda();
				virar_esquerda();
				frente(caminho[ciclo].x, caminho[ciclo].y);
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
				
				frente(caminho[ciclo].x, caminho[ciclo].y);
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
				frente(caminho[ciclo].x, caminho[ciclo].y);
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
				frente(caminho[ciclo].x, caminho[ciclo].y);
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
					
				frente(caminho[ciclo].x, caminho[ciclo].y);
			}
			else {
				bot.println("A diferenca de posicoes deu mistake");
			}
		}	
		bot.println("novo ciclo\n\n");

		if (ciclo == array_size - 1) {
			bot.stopMotors();
		}	

		delay(200);
		ciclo++;
	}

	bot.stopMotors();
}