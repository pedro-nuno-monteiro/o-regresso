#include <Arduino.h>
#include "FCTUC.h"
#include "ListLib.h"

FCTUC bot;

const Vec2 vec_top = Vec2(0, -1);
const Vec2 vec_right = Vec2(1, 0);
const Vec2 vec_bottom = Vec2(0, 1);
const Vec2 vec_left = Vec2(-1, 0);

//List<Caminho> lista_caminhos;

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



void seeMap() {
	byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
			0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
			0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
			0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
		};

	Vec2 pos_atual = Vec2(9,9); //GetRobotPosition();
	Vec2 pos_final = Vec2(0,0); //GetThiefPosition();
    Vec2 pos_anterior=Vec2(-1,-1);
    while(pos_atual.operator!=(pos_final)){
        calcula_proxima_pos(lab, pos_atual, pos_final, pos_anterior);
		bot.println(" ");
    }
    bot.println("CHEGOU AO DESTINO");
}

void setup() {
	Serial.begin(115200);
	
	bot.begin();
	//bot.beginOTA("teste");
	
	bot.waitStart();
	printMemory();
	bot.println("Before");
	randomSeed(millis());
	//List<Caminho> lista;
	seeMap();
	bot.println("After");

}

void loop() {
	//bot.println("prints");
	//bot.moveMotors(-1000, 1000);
}