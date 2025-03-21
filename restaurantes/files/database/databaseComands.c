#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../struct/restaurant/restaurant.h"
#include "../struct/comand/comand.h"

#define RESTAURANT_FILE "restaurant.bin"
#define COMAND_FILE "comand.bin"
#define PRODUCT_FILE "product.bin"

// Função para verificar se o restaurante existe no arquivo
int idComandRestaurantExists(int id)
{
    FILE *file = fopen(RESTAURANT_FILE, "rb");
    if (!file)
    {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        return 0;
    }

    Restaurant restaurant;
    while (fread(&restaurant, sizeof(Restaurant), 1, file))
    {
        if (restaurant.id == id)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Função para verificar se uma comanda já existe para o restaurante
int comandExists(int restaurantId, int comandId)
{
    FILE *file = fopen(COMAND_FILE, "rb");
    if (!file)
        return 0;

    Comands comands;
    while (fread(&comands, sizeof(Comands), 1, file))
    {
        if (comands.id == comandId && comands.restaurantId == restaurantId)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Função para adicionar uma comanda a um restaurante
void addComands()
{
    FILE *file = fopen(COMAND_FILE, "ab");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de comandas.");
        return;
    }

    int idRestaurantComands;
    printf("Digite o ID do restaurante para adicionar uma comanda: ");
    scanf("%d", &idRestaurantComands);
    getchar();

    if (!idComandRestaurantExists(idRestaurantComands))
    {
        printf("Restaurante não encontrado! A comanda não pode ser adicionada.\n");
        fclose(file);
        return;
    }

    Comands comands;
    comands.restaurantId = idRestaurantComands;

    printf("Digite o ID da comanda: ");
    scanf("%d", &comands.id);
    getchar();

    if (comandExists(idRestaurantComands, comands.id))
    {
        printf("Já existe uma comanda com esse ID nesse restaurante.\n");
        fclose(file);
        return;
    }

    printf("Nome do cliente: ");
    fgets(comands.name, sizeof(comands.name), stdin);
    comands.name[strcspn(comands.name, "\n")] = 0;

    fwrite(&comands, sizeof(Comands), 1, file);
    fclose(file);

    printf("Comanda adicionada com sucesso ao restaurante ID %d!\n", idRestaurantComands);
}

// Função para listar as comandas e seus restaurantes associados
void listComands()
{
    FILE *comandFile = fopen(COMAND_FILE, "rb");
    if (!comandFile)
    {
        printf("Nenhuma comanda cadastrada ainda.\n");
        return;
    }

    FILE *restaurantFile = fopen(RESTAURANT_FILE, "rb");
    if (!restaurantFile)
    {
        printf("Erro ao abrir o arquivo de restaurantes.\n");
        fclose(comandFile);
        return;
    }

    printf("\nLista de Comandas:\n");
    Comands comands;
    Restaurant restaurant;

    while (fread(&comands, sizeof(Comands), 1, comandFile))
    {
        int restaurantFoundComands = 0;

        rewind(restaurantFile);
        while (fread(&restaurant, sizeof(Restaurant), 1, restaurantFile))
        {
            if (restaurant.id == comands.restaurantId)
            {
                restaurantFoundComands = 1;
                break;
            }
        }

        printf("------------------------------------------\n");
        if (restaurantFoundComands)
        {
            printf("Restaurante ID: %d\n", comands.restaurantId);
            printf("Nome do Restaurante: %s\n", restaurant.name);
        }
        else
        {
            printf("Restaurante ID: %d (NÃO ENCONTRADO)\n", comands.restaurantId);
        }
        printf("Comanda ID: %d\n", comands.id);
        printf("Nome do Cliente: %s\n", comands.name);
        printf("------------------------------------------\n");
    }

    fclose(comandFile);
    fclose(restaurantFile);
}

void payment()
{

    FILE *restaurantFile = fopen(RESTAURANT_FILE, "rb");
    if (!restaurantFile)
    {
        printf("Nenhum restaurtante cadastrado ainda.\n");
        return;
    }

    int restaurantId;
    printf("Digite o ID do restaurante que está a comanda para calcular o pagamento: ");
    scanf("%d", &restaurantId);
    getchar();

    if (!idComandRestaurantExists(restaurantId))
    {
        printf("Restaurante não encontrado!\n");
        fclose(restaurantFile);
        return;
    }

    Restaurant restaurants;
    int existe = 0;
    while (fread(&restaurants, sizeof(Restaurant), 1, restaurantFile))
    {
        if (!existe)
        {
            FILE *productFile = fopen(PRODUCT_FILE, "rb");
            if (!productFile)
            {
                printf("Nenhum produto cadastrado ainda.\n");
                return;
            }

            int comandId;
            printf("Digite o ID da comanda para calcular o pagamento: ");
            scanf("%d", &comandId);

            Products products;
            float total = 0;
            int found = 0;

            printf("\nProdutos da Comanda %d:\n", comandId);
            while (fread(&products, sizeof(Products), 1, productFile))
            {
                if (products.comandId == comandId)
                {
                    found = 1;
                    total += products.price;
                    printf("%s - R$ %.2f\n", products.name, products.price);
                }
            }

            fclose(productFile);

            if (found)
            {
                printf("------------------------------------\n");
                printf("Total a pagar: R$ %.2f\n", total);
                printf("------------------------------------\n");
            }
            else
            {
                printf("Nenhum produto encontrado para esta comanda.\n");
            }
        }
        else
        {
            printf("Restaurante não encontrado");
        }
    }
}
