#include <Arduino.h>
#include "FCTUC.h"
#include "ListLib.h"

FCTUC bot;
struct Caminho
{
	int distancia;
	List<Vec2> sequencia;
};

/////////////////////////////////////////////////////////////////

void printMemory(){
	bot.print("Free memory: ");
	bot.print(ESP.getFreeHeap());
	bot.println(" bytes");
}
//////////////////////////////////////////////////////////7

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

bool existeNoCaminho(Vec2 vector, Caminho& caminho) {
	bot.println("Entrou existeNoCaminho");
	bot.print("vector -> "); bot.print(vector.x); bot.print(", "); bot.println(vector.y);
	if(caminho.sequencia.Count() > 0){
		for(int i = 0; i < caminho.sequencia.Count(); i++) {
			bot.print("caminho.sequencia[i]: "); bot.print(caminho.sequencia[i].x); bot.print(", "); bot.println(caminho.sequencia[i].y);
			if (vector.operator==(caminho.sequencia[i])) {
				bot.println("Saiu existeNoCaminho -> true");
				return true;
			}
		}
	}
	bot.println("Saiu existeNoCaminho -> false");
	return false;
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

int identificaAnterior(Caminho& caminho) {
	bot.println("IDENTIFICA ANTERIOR");
    if (caminho.sequencia.Count() < 2) {
        bot.println("Error: Not enough positions in the sequence");
        return -1; // Return an error code
    }
    Vec2 vec_atual = caminho.sequencia[caminho.sequencia.Count() - 1];
    bot.print("vec_atual: "); bot.print(vec_atual.x); bot.print(", "); bot.println(vec_atual.y);
    Vec2 vec_anterior = caminho.sequencia[caminho.sequencia.Count() - 2];
    bot.print("vec_anterior: "); bot.print(vec_anterior.x); bot.print(", "); bot.println(vec_anterior.y);

    if (vec_atual.y > vec_anterior.y) {
		bot.println("cima");
        return 0; // Moveu-se para cima
    } else if (vec_atual.x < vec_anterior.x) {
		bot.println("direita");
        return 1; // Moveu-se para a direita
    } else if (vec_atual.y < vec_anterior.y) {
		bot.println("baixo");
        return 2; // Moveu-se para baixo
    } else if (vec_atual.x > vec_anterior.x) {
		bot.println("esquerda");
        return 3; // Moveu-se para a esquerda
    } else {
        return -1;
    }
}

void printCaminhoAtual(Caminho& caminho){
    bot.println("CAMINHO ATUAL");
    bot.print("Distancia: "); bot.println(caminho.distancia);
    for(int i = 0; i < caminho.sequencia.Count(); i++) {
        bot.print("x: "); bot.print(caminho.sequencia[i].x); bot.print(" ");
        bot.print("y: "); bot.print(caminho.sequencia[i].y); bot.println(" ");
    }
}

void printCaminhos(List<Caminho>& lista_caminhos){
	for(int i = 0; i < lista_caminhos.Count(); i++){
    bot.print("CAMINHO "); bot.println(i); 
    bot.print("DISTANCIA: "); bot.println(lista_caminhos[i].sequencia.Count());
    bot.println("SEQUENCIA:");
    for(int j = 0; j < lista_caminhos[i].sequencia.Count(); j++) {
        bot.print("x: "); bot.print(lista_caminhos[i].sequencia[j].x); bot.print(" ");
        bot.print("y: "); bot.print(lista_caminhos[i].sequencia[j].y); bot.println(" ");
    }
    bot.println(" ");
}
}

void printWalls(int tab[]){
	bot.print("walls.top: "); bot.println(tab[0]);
	bot.print("walls.right: "); bot.println(tab[1]);
	bot.print("walls.bottom: "); bot.println(tab[2]);
	bot.print("walls.left: "); bot.println(tab[3]);
	bot.print("walls.total: "); bot.println(quantidadeBuracos(tab));
	bot.println("");
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
    
    if (buracos == 1){
        // Only one available path, move to that position
        for(int i = 0; i < 4; i++){
            if(tabela_walls[i] == 0) {
                pos_anterior = pos_atual;
                pos_atual = mudaPosicao(pos_atual, i);
                return;
            }
        }
    }
    else if(buracos > 1) {
        // Multiple available paths, choose one
        Vec2 *adjacentes = retornaPosicoesAdjacentes(labirinto, pos_atual, pos_anterior, buracos);
        Vec2 *adjacentes_possiveis = new Vec2[buracos];
        int count_possiveis = 0;

        for (int i = 0; i < buracos; i++) {
            // Check if the position has 3 walls and is not the destination
            if (quantidadeBuracos(identificaParedes(GetWallsAtPos(adjacentes[i], labirinto))) > 3 && adjacentes[i] != pos_destino) {
                continue; // Skip this position
            }
            else{
                adjacentes_possiveis[count_possiveis] = adjacentes[i];
                count_possiveis++;
            }
            
            // Move to the chosen position
            pos_anterior = pos_atual;
            pos_atual = adjacentes[i];
            
            return;
        }
        if (count_possiveis > 0) {
            int random_index = random(0, count_possiveis); // Generate a random index within the range of possible positions
            pos_anterior = pos_atual;
            pos_atual = adjacentes_possiveis[random_index]; // Move to the randomly chosen position
            return;
        }
        
        // Free memory allocated for the arrays
        delete[] adjacentes;
        delete[] adjacentes_possiveis;
    }
}


void seeMap(Vec2 pos_inicial,Vec2 pos_destino, Vec2 pos_anterior) {
	byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
			0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
			0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
			0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
		};

	Vec2 pos_atual = Vec2(1,0);//GetRobotPosition();
	Vec2 pos_final = Vec2(0,1);//GetThiefPosition();
    Vec2 pos_anterior=Vec2(-1,-1);
    while(pos_atual.operator!=(pos_destino)){
        calcula_proxima_pos(lab,pos_inicial, pos_final, pos_anterior);
        bot.println("\nCalculou proxima posicao");
        bot.print("Vector atual: "); bot.print(pos_atual.x); bot.print(", "); bot.println(pos_atual.y); bot.println("");
        bot.print("Vector anterior: "); bot.print(pos_final.x); bot.print(", "); bot.println(pos_final.y); bot.println("");
        bot.print("Vector destino: "); bot.print(pos_anterior.x); bot.print(", "); bot.println(pos_anterior.y); bot.println("");
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
	List<Caminho> lista;
	seeMap(lista);
	bot.println("After");
}

void loop() {
	//bot.println("prints");
	//bot.moveMotors(-1000, 1000);
}
