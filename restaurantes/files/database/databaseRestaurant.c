#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../struct/restaurant/restaurant.h"

#define RESTAURANT_FILE "restaurant.bin"

// Função para verificar se o ID já existe no arquivo
int idExists(int id)
{
    FILE *file = fopen(RESTAURANT_FILE, "rb");
    if (!file)
        return 0; // Se o arquivo não existe, o ID não existe

    Restaurant restaurant;
    while (fread(&restaurant, sizeof(Restaurant), 1, file))
    {
        if (restaurant.id == id)
        {
            fclose(file);
            return 1; // ID já existe
        }
    }

    fclose(file);
    return 0; // ID não encontrado
}

void addRestaurant()
{
    FILE *file = fopen(RESTAURANT_FILE, "ab");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Restaurant restaurant;

    // Solicitar e validar o ID do restaurante
    printf("Digite o ID do restaurante: ");
    scanf("%d", &restaurant.id);
    getchar(); // Limpar buffer do Enter

    if (idExists(restaurant.id))
    {
        printf("Já existe um restaurante com esse ID!\n");
        fclose(file);
        return;
    }

    // Entrada dos outros dados do restaurante
    printf("Nome: ");
    fgets(restaurant.name, sizeof(restaurant.name), stdin);
    restaurant.name[strcspn(restaurant.name, "\n")] = 0; // Remover o '\n'

    printf("Tipo de Restaurante: ");
    fgets(restaurant.type, sizeof(restaurant.type), stdin);
    restaurant.type[strcspn(restaurant.type, "\n")] = 0;

    printf("Contato: ");
    fgets(restaurant.contact, sizeof(restaurant.contact), stdin);
    restaurant.contact[strcspn(restaurant.contact, "\n")] = 0;

    // Escrevendo no arquivo
    fwrite(&restaurant, sizeof(Restaurant), 1, file);
    fclose(file);

    printf("Restaurante adicionado com sucesso!\n");
}

void listRestaurants()
{
    FILE *file = fopen(RESTAURANT_FILE, "rb");
    if (!file)
    {
        printf("Nenhum restaurante cadastrado ainda.\n");
        return;
    }

    Restaurant restaurant;
    printf("Lista de Restaurantes:\n");
    while (fread(&restaurant, sizeof(Restaurant), 1, file))
    {
        printf("ID: %d, Nome: %s, Tipo: %s, Contato: %s\n",
               restaurant.id, restaurant.name, restaurant.type, restaurant.contact);
    }
    fclose(file);
}
