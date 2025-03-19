#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../struct/restaurant/restaurant.h"
#include "../struct/comand/comand.h"

#define RESTAURANT_FILE "restaurant.bin"
#define COMAND_FILE "comand.bin"

// Função para verificar se o restaurante existe no arquivo
int idComandRestauranteExiste(int id) {
    FILE *file = fopen(RESTAURANT_FILE, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        return 0;
    }

    Restaurant restaurant;
    while (fread(&restaurant, sizeof(Restaurant), 1, file)) {
        if (restaurant.id == id) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Função para verificar se uma comanda já existe para o restaurante
int comandaExiste(int restaurantId, int comandId) {
    FILE *file = fopen(COMAND_FILE, "rb");
    if (!file) return 0;

    Comands comands;
    while (fread(&comands, sizeof(Comands), 1, file)) {
        if (comands.id == comandId && comands.restaurantId == restaurantId) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Função para adicionar uma comanda a um restaurante
void addComands() {
    FILE *file = fopen(COMAND_FILE, "ab");
    if (!file) {
        perror("Erro ao abrir o arquivo de comandas.");
        return;
    }
    
    int idRestauranteComands;
    printf("Digite o ID do restaurante para adicionar uma comanda: ");
    scanf("%d", &idRestauranteComands);
    getchar();

    if (!idComandRestauranteExiste(idRestauranteComands)) {
        printf("Restaurante não encontrado! A comanda não pode ser adicionada.\n");
        fclose(file);
        return;
    }

    Comands comands;
    comands.restaurantId = idRestauranteComands;

    printf("Digite o ID da comanda: ");
    scanf("%d", &comands.id);
    getchar();

    if (comandaExiste(idRestauranteComands, comands.id)) {
        printf("Já existe uma comanda com esse ID nesse restaurante.\n");
        fclose(file);
        return;
    }

    printf("Nome do cliente: ");
    fgets(comands.name, sizeof(comands.name), stdin);
    comands.name[strcspn(comands.name, "\n")] = 0;

    fwrite(&comands, sizeof(Comands), 1, file);
    fclose(file);

    printf("Comanda adicionada com sucesso ao restaurante ID %d!\n", idRestauranteComands);
}

// Função para listar as comandas e seus restaurantes associados
void listComands() {
    FILE *comandFile = fopen(COMAND_FILE, "rb");
    if (!comandFile) {
        printf("Nenhuma comanda cadastrada ainda.\n");
        return;
    }

    FILE *restaurantFile = fopen(RESTAURANT_FILE, "rb");
    if (!restaurantFile) {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        fclose(comandFile);
        return;
    }

    printf("\nLista de Comandas:\n");
    Comands comands;
    Restaurant restaurant;

    while (fread(&comands, sizeof(Comands), 1, comandFile)) {
        int restauranteEncontradoComands = 0;

        rewind(restaurantFile);
        while (fread(&restaurant, sizeof(Restaurant), 1, restaurantFile)) {
            if (restaurant.id == comands.restaurantId) {
                restauranteEncontradoComands = 1;
                break;
            }
        }

        printf("------------------------------------------\n");
        if (restauranteEncontradoComands) {
            printf("Restaurante ID: %d\n", comands.restaurantId);
            printf("Nome do Restaurante: %s\n", restaurant.name);
        } else {
            printf("Restaurante ID: %d (NÃO ENCONTRADO)\n", comands.restaurantId);
        }
        printf("Comanda ID: %d\n", comands.id);
        printf("Nome do Cliente: %s\n", comands.name);
        printf("------------------------------------------\n");
    }

    fclose(comandFile);
    fclose(restaurantFile);
}

// Função de pagamento das comandas
void payment() {
    
}
