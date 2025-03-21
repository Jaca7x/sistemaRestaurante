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
int idRestaurantExists(int id)
{
    FILE *file = fopen(RESTAURANT_FILE, "rb");
    if (!file)
    {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        return 0; // Retorna 0 se o arquivo não existir
    }

    Restaurant restaurant;
    while (fread(&restaurant, sizeof(Restaurant), 1, file))
    {
        if (restaurant.id == id)
        {
            fclose(file);
            return 1; // Restaurante encontrado
        }
    }

    fclose(file);
    return 0; // Restaurante não encontrado
}

// Função para verificar se a comanda existe no restaurante
int idComandExits(int id)
{
    FILE *file = fopen(COMAND_FILE, "rb");
    if (!file)
    {
        printf("Erro ao abrir o arquivo de comandas.\n");
        return 0; // Retorna 0 se o arquivo não existir
    }

    Comands comands;
    while (fread(&comands, sizeof(Comands), 1, file))
    {
        if (comands.id == id)
        {
            fclose(file);
            return 1; // Restaurante encontrado
        }
    }
    fclose(file);
    return 0; // Restaurante não encontrado
}

// Função para verificar se um produto já existe para o restaurante
int productExists(int restaurantId, int productId)
{
    FILE *file = fopen(PRODUCT_FILE, "rb");
    if (!file)
        return 0; // Se o arquivo não existir, o produto não existe

    Products product;
    while (fread(&product, sizeof(Products), 1, file))
    {
        if (product.id == productId && product.restaurantId == restaurantId)
        {
            fclose(file);
            return 1; // Produto já cadastrado
        }
    }

    fclose(file);
    return 0; // Produto não encontrado
}

// Função para adicionar um produto a um restaurante
void addProduct()
{
    FILE *file = fopen(PRODUCT_FILE, "ab");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de produtos.");
        return;
    }

    int idRestaurant;
    printf("Digite o ID do restaurante para adicionar um produto: ");
    scanf("%d", &idRestaurant);
    getchar(); // Limpa o buffer

    if (!idRestaurantExists(idRestaurant))
    {
        printf("Restaurante não encontrado! Produto não pode ser adicionado.\n");
        fclose(file);
        return;
    }

    Products product;
    product.restaurantId = idRestaurant;
    product.comandId = -1; // Inicializa como -1 (sem comanda)

    printf("Digite o ID do produto: ");
    scanf("%d", &product.id);
    getchar(); // Limpa o buffer

    if (productExists(idRestaurant, product.id))
    {
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

    printf("Produto adicionado com sucesso ao restaurante ID %d!\n", idRestaurant);
}

void addProductComand()
{
    FILE *fileProduct = fopen(PRODUCT_FILE, "r+b"); // Abrir para leitura e escrita
    if (!fileProduct)
    {
        perror("Erro ao abrir o arquivo de produtos.");
        return;
    }

    int idRestaurant, idComand, idProduct;
    printf("Digite o ID do restaurante para adicionar um produto: ");
    scanf("%d", &idRestaurant);
    getchar();

    if (!idRestaurantExists(idRestaurant))
    {
        printf("Restaurante não encontrado! Produto não pode ser adicionado.\n");
        fclose(fileProduct);
        return;
    }

    printf("Digite o ID da comanda para adicionar um produto: ");
    scanf("%d", &idComand);
    getchar();

    if (!idComandExits(idComand))
    {
        printf("Comanda não encontrada! Produto não pode ser adicionado.\n");
        fclose(fileProduct);
        return;
    }

    printf("Digite o ID do produto: ");
    scanf("%d", &idProduct);
    getchar();

    Products product;
    int found = 0;
    long position; // Para armazenar a posição correta do arquivo

    while (fread(&product, sizeof(Products), 1, fileProduct))
    {
        if (product.id == idProduct && product.restaurantId == idRestaurant)
        {
            found = 1;
            product.comandId = idComand; // Atualiza a comanda

            position = ftell(fileProduct) - sizeof(Products); // Calcula a posição do registro
            fseek(fileProduct, position, SEEK_SET);           // Move o ponteiro para a posição correta
            fwrite(&product, sizeof(Products), 1, fileProduct);
            fflush(fileProduct); // Garante que os dados sejam escritos
            printf("Produto ID %d adicionado à comanda %d com sucesso!\n", idProduct, idComand);
            break;
        }
    }

    if (!found)
    {
        printf("Produto não encontrado no restaurante %d.\n", idRestaurant);
    }

    fclose(fileProduct);
}

void listProducts()
{
    FILE *productFile = fopen(PRODUCT_FILE, "rb");
    if (!productFile)
    {
        printf("Nenhum produto cadastrado ainda.\n");
        return;
    }

    FILE *restaurantFile = fopen(RESTAURANT_FILE, "rb");
    if (!restaurantFile)
    {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        fclose(productFile);
        return;
    }

    printf("\nLista de Produtos:\n");
    Products product;
    Restaurant restaurant;

    while (fread(&product, sizeof(Products), 1, productFile))
    {
        int restaurantFound = 0;

        // Buscar restaurante correspondente ao produto
        rewind(restaurantFile); // Voltar ao início do arquivo de restaurantes
        while (fread(&restaurant, sizeof(Restaurant), 1, restaurantFile))
        {
            if (restaurant.id == product.restaurantId)
            {
                restaurantFound = 1;
                break;
            }
        }

        // Exibir os produtos e seus restaurantes
        printf("------------------------------------------\n");
        if (restaurantFound)
        {
            printf("Restaurante ID: %d\n", restaurant.id);
            printf("Nome do Restaurante: %s\n", restaurant.name);
        }
        else
        {
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

void listProductsComand()
{
    FILE *productFile = fopen(PRODUCT_FILE, "rb");
    if (!productFile)
    {
        printf("Nenhum produto cadastrado ainda.\n");
        return;
    }

    FILE *comandFile = fopen(COMAND_FILE, "rb");
    if (!comandFile)
    {
        printf("Erro ao abrir o arquivo de comandas.\n");
        fclose(productFile);
        return;
    }

    printf("\nLista de Produtos nas Comandas:\n");
    Products product;
    Comands comand;

    while (fread(&product, sizeof(Products), 1, productFile))
    {
        if (product.comandId == -1)
        {
            continue; // Pula produtos que ainda não foram adicionados a uma comanda
        }

        printf("DEBUG - Produto ID: %d, Nome: %s, Comanda ID: %d, Restaurante ID: %d\n",
               product.id, product.name, product.comandId, product.restaurantId);

        int comandFound = 0;
        rewind(comandFile);
        while (fread(&comand, sizeof(Comands), 1, comandFile))
        {
            if (comand.id == product.comandId)
            {
                comandFound = 1;
                break;
            }
        }

        printf("------------------------------------------\n");
        if (comandFound)
        {
            printf("ID da Comanda: %d\n", product.comandId);
        }
        else
        {
            printf("ID da Comanda: %d (NÃO ENCONTRADA)\n", product.comandId);
        }
        printf("Produto ID: %d\n", product.id);
        printf("Nome do Produto: %s\n", product.name);
        printf("Preço: R$ %.2f\n", product.price);
        printf("------------------------------------------\n");
    }

    fclose(productFile);
    fclose(comandFile);
}
