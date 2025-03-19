#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../struct/products/products.h"
#include "../struct/restaurant/restaurant.h"
#include "../struct/comand/comand.h"

#define PRODUCT_FILE "product.bin"
#define RESTAURANT_FILE "restaurant.bin"
#define COMAND_FILE "comand.bin"

// Função para verificar se o restaurante existe no arquivo
int idRestauranteExiste(int id) {
    FILE *file = fopen(RESTAURANT_FILE, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        return 0;  // Retorna 0 se o arquivo não existir
    }

    Restaurant restaurant;
    while (fread(&restaurant, sizeof(Restaurant), 1, file)) {
        if (restaurant.id == id) {
            fclose(file);
            return 1;  // Restaurante encontrado
        }
    }

    fclose(file);
    return 0;  // Restaurante não encontrado
}

// Função para verificar se a comanda existe no restaurante
int idComandaExiste(int id) {
    FILE *file = fopen(COMAND_FILE, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo de comandas.\n");
        return 0;  // Retorna 0 se o arquivo não existir
    }

    Comands comands;
    while (fread(&comands, sizeof(Comands), 1, file)) {
        if (comands.id == id) {
            fclose(file);
            return 1;  // Restaurante encontrado
        }
    }
    fclose(file);
    return 0;  // Restaurante não encontrado
}


// Função para verificar se um produto já existe para o restaurante
int produtoExiste(int restaurantId, int productId) {
    FILE *file = fopen(PRODUCT_FILE, "rb");
    if (!file) return 0; // Se o arquivo não existir, o produto não existe

    Products product;
    while (fread(&product, sizeof(Products), 1, file)) {
        if (product.id == productId && product.restaurantId == restaurantId) {
            fclose(file);
            return 1; // Produto já cadastrado
        }
    }

    fclose(file);
    return 0; // Produto não encontrado
}

// Função para adicionar um produto a um restaurante 
void addProduct() {
    FILE *file = fopen(PRODUCT_FILE, "ab");
    if (!file) {
        perror("Erro ao abrir o arquivo de produtos.");
        return;
    }   
    
    int idRestaurante;
    printf("Digite o ID do restaurante para adicionar um produto: ");
    scanf("%d", &idRestaurante);
    getchar(); // Limpa o buffer

    if (!idRestauranteExiste(idRestaurante)) {
        printf("Restaurante não encontrado! Produto não pode ser adicionado.\n");
        fclose(file);
        return;
    }
    

    Products product;
    product.restaurantId = idRestaurante;

    printf("Digite o ID do produto: ");
    scanf("%d", &product.id);
    getchar(); // Limpa o buffer

    if (produtoExiste(idRestaurante, product.id)) {
        printf("Já existe um produto com esse ID nesse restaurante.\n");
        fclose(file);
        return;
    }

    printf("Nome do produto: ");
    fgets(product.name, sizeof(product.name), stdin);
    product.name[strcspn(product.name, "\n")] = 0; // Remove o '\n' final

    printf("Preço do produto: ");
    scanf("%f", &product.price);
    getchar(); // Limpa o buffer

    fwrite(&product, sizeof(Products), 1, file);
    fclose(file);

    printf("Produto adicionado com sucesso ao restaurante ID %d!\n", idRestaurante);
}

void addProductComand() {
    FILE *fileProduct = fopen(PRODUCT_FILE, "rb"); // Abre para leitura
    if (!fileProduct) {
        perror("Erro ao abrir o arquivo de produtos.");
        return;
    }

    int idRestaurante;
    printf("Digite o ID do restaurante para adicionar um produto: ");
    scanf("%d", &idRestaurante);
    getchar(); // Limpa o buffer

    if (!idRestauranteExiste(idRestaurante)) {
        printf("Restaurante não encontrado! Produto não pode ser adicionado.\n");
        fclose(fileProduct);
        return;
    }

    int idComand;
    printf("Digite o ID da comanda para adicionar um produto: ");
    scanf("%d", &idComand);
    getchar(); // Limpa o buffer

    if (!idComandaExiste(idComand)) {
        printf("Comanda não encontrada! Produto não pode ser adicionado.\n");
        fclose(fileProduct);
        return;
    }

    int idProduto;
    printf("Digite o ID do produto: ");
    scanf("%d", &idProduto);
    getchar(); // Limpa o buffer

    if (!produtoExiste(idRestaurante, idProduto)) {
        printf("Não existe um produto com esse ID no restaurante %d.\n", idRestaurante);
    } else {
        printf("Produto ID %d adicionado com sucesso à comanda %d do restaurante!\n", idProduto, idComand);
    }

    fclose(fileProduct);
}

// Função para listar produtos nas comandas

// Função para listar os produtos e seus restaurantes associados
void listProducts() {
    FILE *productFile = fopen(PRODUCT_FILE, "rb");
    if (!productFile) {
        printf("Nenhum produto cadastrado ainda.\n");
        return;
    }

    FILE *restaurantFile = fopen(RESTAURANT_FILE, "rb");
    if (!restaurantFile) {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        fclose(productFile);
        return;
    }

    printf("\nLista de Produtos:\n");
    Products product;
    Restaurant restaurant;

    while (fread(&product, sizeof(Products), 1, productFile)) {
        int restauranteEncontrado = 0;

        // Buscar restaurante correspondente ao produto
        rewind(restaurantFile); // Voltar ao início do arquivo de restaurantes
        while (fread(&restaurant, sizeof(Restaurant), 1, restaurantFile)) {
            if (restaurant.id == product.restaurantId) {
                restauranteEncontrado = 1;
                break; // Encontrou o restaurante correspondente
            }
        }

        // Exibir os produtos e seus restaurantes
        printf("------------------------------------------\n");
        if (restauranteEncontrado) {
            printf("Restaurante ID: %d\n", product.restaurantId);
            printf("Nome do Restaurante: %s\n", restaurant.name);
        } else {
            printf("Restaurante ID: %d (NÃO ENCONTRADO)\n", product.restaurantId);
        }
        printf("Produto ID: %d\n", product.id);
        printf("Nome do Produto: %s\n", product.name);
        printf("Preço: R$ %.2f\n", product.price);
        printf("------------------------------------------\n");
    }

    fclose(productFile);
    fclose(restaurantFile);
}
