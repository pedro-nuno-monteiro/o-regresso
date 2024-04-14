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

void recursivaCaminhos(byte *labirinto, Vec2& pos_atual, Vec2 pos_destination, Caminho& caminho_atual, List<Caminho>& lista_caminhos) {
	bot.println("\nENTROU RECURSIVA\n");
	bool leave = false;
	// Check Walls
	
	while(!leave) {
		/*if(pos_atual.operator==(Vec2(3,9))){
			leave = true;
			break;
		}*/
		//printMemory();
		bot.print("Vector atual: "); bot.print(pos_atual.x); bot.print(", "); bot.println(pos_atual.y); bot.println("");
		int *tabela_walls = identificaParedes(GetWallsAtPos(pos_atual, labirinto));
		printWalls(tabela_walls);
		bot.println("identifica paredes");
        if(pos_atual.operator!=(pos_destination)){
			bot.println("nao chegou ao destino");
			//bot.println();
            // Se não for a primeira posição
            if(pos_atual.operator!=(caminho_atual.sequencia[0])){
				bot.println("nao e a primeira posicao");
                // Analisar a posição anterior e mudar as paredes para 1
                int anterior = identificaAnterior(caminho_atual);

                for(int i = 0; i < 4; i++){ // Muda as walls para não ter a anterior
                    if(anterior == i && tabela_walls[i] == 0){
                        tabela_walls[i] = 1;
                    }
                }

				bot.println("CAMINHO ATUAL");
				bot.print("Distancia: "); bot.println(caminho_atual.distancia);
				for(int i = 0; i < caminho_atual.sequencia.Count(); i++) {
					bot.print("x: "); bot.print(caminho_atual.sequencia[i].x); bot.print(" ");
					bot.print("y: "); bot.print(caminho_atual.sequencia[i].y); bot.println(" ");
				}

				//printCaminhoAtual(caminho_atual);

				printWalls(tabela_walls);

				if(quantidadeBuracos(tabela_walls) == 0){
					bot.println("FICOU SEM PAREDES -> DENTRO");
					leave = true;
					//return;
				}
                else if(quantidadeBuracos(tabela_walls) == 1){ // Vai para essa posição e adiciona ao caminho
					bot.println("so ha um caminho possivel");
                    for(int i = 0; i < 4; i++) {
                        if(tabela_walls[i] == 0) {
							//bot.print("tabela_walls[i]: "); bot.println(tabela_walls[i]);
                            Vec2 pos_nova = mudaPosicao(pos_atual, i);
							bot.print("muda posicao -> "); bot.print(pos_nova.x); bot.print(", "); bot.println(pos_nova.y); 
							
							bot.println("CAMINHO ATUAL");
							bot.print("Distancia: "); bot.println(caminho_atual.distancia);
							for(int i = 0; i < caminho_atual.sequencia.Count(); i++) {
								bot.print("x: "); bot.print(caminho_atual.sequencia[i].x); bot.print(" ");
								bot.print("y: "); bot.print(caminho_atual.sequencia[i].y); bot.println(" ");
							}

                            if(!existeNoCaminho(pos_nova, caminho_atual)){
                                pos_atual = pos_nova;
                                caminho_atual.sequencia.Add(pos_nova);
                                caminho_atual.distancia++;
                            }
                            else{ // Acaba e não guarda
                                bot.println("Saiu sem guardar");
								leave = true;
                            }
                        }
                    }
                }
                else if(quantidadeBuracos(tabela_walls) >= 1){ // A primeira faz normal e as outras chama a recursiva
                    for(int i = 0; i < 4; i++){
						int ultimo_buraco = false;
						int contador = 0;
                        if(tabela_walls[i] == 0){ // Se não houver parede
							contador++;
							if(contador == quantidadeBuracos(tabela_walls)){
								ultimo_buraco = true;
							}
                            Vec2 pos_nova = mudaPosicao(pos_atual, i);
							bot.print("muda posicao"); bot.print(pos_nova.x); bot.print(", "); bot.println(pos_nova.y);
							
							bot.println("CAMINHO ATUAL");
							bot.print("Distancia: "); bot.println(caminho_atual.distancia);
							for(int i = 0; i < caminho_atual.sequencia.Count(); i++) {
								bot.print("x: "); bot.print(caminho_atual.sequencia[i].x); bot.print(" ");
								bot.print("y: "); bot.print(caminho_atual.sequencia[i].y); bot.println(" ");
							}

							if(!existeNoCaminho(pos_nova, caminho_atual)){
								bot.println("Nao existe no caminho");
                                if(!ultimo_buraco){ // Já não é a primeira vez, logo chama a recursiva
									bot.println("processo pai");
									Caminho aux = caminho_atual;
									Vec2 pos_aux = pos_nova;
									aux.sequencia.Add(pos_nova);
									aux.distancia++;
                                    recursivaCaminhos(labirinto, pos_aux, pos_destination, aux, lista_caminhos);
                                }
                                else{ // Primeira vez faz normal
									bot.println("processo filho");
									pos_atual = pos_nova;
									caminho_atual.sequencia.Add(pos_nova);
									caminho_atual.distancia++;
                                    ultimo_buraco = true;
                                }
                            }
                            else{
                                leave = true;
                            }
                        }
                    }
                }  
            }
            else if(pos_atual.operator==(caminho_atual.sequencia[0])){ // Se for o primeiro
				bot.println("e a primeira posicao");
				
                for(int i = 0; i < 4; i++){
					int ultimo_buraco = false;
					int contador = 0;
					if(tabela_walls[i] == 0){
						contador++;
						if(contador == quantidadeBuracos(tabela_walls)){
							ultimo_buraco = true;
						}
						Vec2 pos_nova = mudaPosicao(pos_atual, i);
						bot.println("muda posicao");
                        pos_atual = pos_nova;
                        caminho_atual.sequencia.Add(pos_nova);
                        caminho_atual.distancia++;
						if(ultimo_buraco){ // último processo continua normal
							bot.println("ultimo processo -> sem recursiva");
						}
						else{
							bot.println("processo recursiva");
							Caminho aux = caminho_atual;
							Vec2 pos_aux = pos_nova;
							aux.sequencia.Add(pos_nova);
							aux.distancia++;
							recursivaCaminhos(labirinto, pos_aux, pos_destination, aux, lista_caminhos);
						}
					}

                    /*if(tabela_walls[i] == 0){ // Se não houver parede
                        Vec2 pos_nova = mudaPosicao(pos_atual, i);
						bot.println("muda posicao");
                        pos_atual = pos_nova;
                        caminho_atual.sequencia.Add(pos_nova);
                        caminho_atual.distancia++;
                        if(processo_pai){ // Já não é a primeira vez, logo chama a recursiva
							bot.println("processo pai");
							Caminho aux = caminho_atual;
							Vec2 pos_aux = pos_nova;
							aux.sequencia.Add(pos_nova);
							aux.distancia++;
							recursivaCaminhos(labirinto, pos_aux, pos_destination, aux, lista_caminhos);
                            leave = true;
                        }
                        else{ // Primeira vez faz normal
							bot.println("processo filho");
                            processo_pai = true;
                        }
                    }*/
                }
            }
			else if(quantidadeBuracos(tabela_walls) == 0){
				bot.println("FICOU SEM PAREDES -> FORA");
				leave = true;
			}
        } 
        else { // Chegou ao destino e adiciona à lista de caminhos
            bot.println("Chegou ao destino!!!");
			printCaminhoAtual(caminho_atual);
            lista_caminhos.Add(Caminho{caminho_atual.distancia, caminho_atual.sequencia});
			
			bot.print(lista_caminhos[0].sequencia[0].x); bot.print(", ");
			bot.println(lista_caminhos[0].sequencia[0].y);
			bot.print(lista_caminhos[0].sequencia[1].x); bot.print(", ");
			bot.println(lista_caminhos[0].sequencia[1].y);
			bot.print(lista_caminhos[0].sequencia[2].x); bot.print(", ");
			bot.println(lista_caminhos[0].sequencia[2].y);

			bot.println("Adicionou à lista!!!");
			printCaminhos(lista_caminhos);
			bot.println("A sair da recursiva");
            leave = true;
        }
	} 
	return ;
}

Caminho caminhoEficaz(List<Caminho>& lista_caminhos){
	int id_caminho_pequeno = 0;
	for(int i = 1; i < lista_caminhos.Count(); i++){
		if(lista_caminhos[i].distancia < lista_caminhos[id_caminho_pequeno].distancia) {
			id_caminho_pequeno = i;
		}
	}
	Caminho caminho_escolhido = lista_caminhos[id_caminho_pequeno];
	//lista_caminhos.Trim();
	return caminho_escolhido;
}

void seeMap(List<Caminho>& lista_caminhos) {
	byte lab[51] = {10, 0x9e, 0x9a, 0xac, 0x9a, 0xac, 0x59, 0x69, 0xe3, 0xc, 0xb4, 0x53, 
			0xa2, 0x88, 0x63, 0xa4, 0x1a, 0xc9, 0x61, 0xcb, 0xc5, 0x5d, 0x36, 0xb4, 0x3c, 
			0x14, 0x34, 0x9a, 0xc3, 0xc3, 0x45, 0xd5, 0x1c, 0x5b, 0x2e, 0x57, 0x51, 0x61, 
			0x28, 0xac, 0x1e, 0x10, 0xa2, 0x82, 0xe5, 0x1c, 0x73, 0xae, 0x3a, 0xa2, 0x67
		};

	Vec2 pos_inicial = Vec2(1,6);
	Vec2 pos_final = Vec2(0,1);
	Caminho caminho_inicial;
	caminho_inicial.distancia = 1;
	caminho_inicial.sequencia.Add(pos_inicial);
	recursivaCaminhos(lab, pos_inicial, pos_final, caminho_inicial, lista_caminhos);
	//printCaminhos();
	Caminho o_melhor = caminhoEficaz(lista_caminhos);
	bot.println("ESTE E O MELHOR");
	printCaminhoAtual(o_melhor);

	//bot.println("\n");
	//bot.println(o_melhor.distancia);
	//for(int j = 0; j < o_melhor.sequencia.Count(); j++){
	//	bot.print(String(o_melhor.sequencia[j].x));
    //	bot.print(String(o_melhor.sequencia[j].y));
	//}
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
