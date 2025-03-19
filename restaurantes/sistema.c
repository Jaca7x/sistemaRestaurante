// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

// Other files
#include "files/struct/products/products.h"
#include "files/struct/restaurant/restaurant.h"
#include "files/struct/comand/comand.h"
#include "files/database/databaseProducts.c"
#include "files/database/databaseRestaurant.c"
#include "files/database/databaseComands.c"

// Função para mudar a cor do console
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Função para limpar tela e exibir título
void clearScreenWithTitle()
{
    system("cls");
    setColor(14); // Amarelo
    printf("\n==================================\n");
    printf("      SISTEMA DE RESTAURANTES     \n");
    printf("==================================\n\n");
    setColor(7); // Branco
}

// Função para exibir menu principal
void mainMenu()
{
    setColor(11); // Azul claro
    printf("Selecione uma opção:\n");
    printf("1 - Restaurantes\n");
    printf("2 - Produtos\n");
    printf("3 - Comandas\n");
    printf("4 - Sair\n");
    setColor(7); // Branco
}

// Função para exibir menu de restaurantes
void restaurantMenu()
{
    setColor(10); // Verde
    printf("Selecione uma opção:\n");
    printf("1 - Adicionar restaurante\n");
    printf("2 - Visualizar restaurantes\n");
    printf("3 - Visualizar produtos\n");
    printf("4 - Visualizar comandas\n");
    printf("5 - Voltar\n");
    setColor(7);
}

// Função para exibir menu de produtos
void productMenu()
{
    setColor(12); // Vermelho claro
    printf("Selecione uma opção:\n");
    printf("1 - Adicionar produtos\n");
    printf("2 - Visualizar produtos\n");
    printf("3 - Visualizar restaurantes\n");
    printf("4 - Voltar\n");
    setColor(7);
}

// Função para exibir menu de comandas
void comandMenu()
{
    setColor(5);
    printf("Selecione uma opção:\n");
    printf("1 - Adicionar comandas\n");
    printf("2 - Visualizar comandas\n");
    printf("3 - Adicionar produtos\n");
    printf("4 - Pagamento\n");
    printf("5 - Voltar\n");
    setColor(7);
}

int main()
{
    // Definir idioma e codificação UTF-8
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_UTF8); // Configurar console para UTF-8

    int option, optionRestaurant, optionProducts, optionComand;

    do
    {
        clearScreenWithTitle();
        mainMenu();
        printf("Escolha: ");
        scanf("%d", &option);
        getchar(); // Limpa o buffer após scanf

        switch (option)
        {
        case 1: // Menu Restaurantes
            do
            {
                clearScreenWithTitle();
                restaurantMenu();
                printf("Escolha: ");
                scanf("%d", &optionRestaurant);
                getchar(); // Limpa o buffer após scanf

                switch (optionRestaurant)
                {
                case 1:
                    addRestaurant();
                    break;
                case 2:
                    listRestaurants();
                    break;
                case 3:
                    listProducts();
                    break;
                case 4:
                    listComands();
                    break;
                case 5:
                    break;
                default:
                    printf("Opção inválida! Tente novamente.\n");
                    Sleep(1000);
                }

                system("pause"); // Pausa antes de limpar a tela
            } while (optionRestaurant != 5);
            break;

        case 2: // Menu Produtos
            do
            {
                clearScreenWithTitle();
                productMenu();
                printf("Escolha: ");
                scanf("%d", &optionProducts);
                getchar(); // Limpa o buffer após scanf

                switch (optionProducts)
                {
                case 1:
                    addProduct();
                    break;
                case 2:
                    listProducts();
                    break;
                case 3:
                    listRestaurants();
                    break;
                case 4:
                    break;
                default:
                    printf("Opção inválida! Tente novamente.\n");
                    Sleep(1000);
                }

                system("pause"); // Pausa antes de limpar a tela
            } while (optionProducts != 4);
            break;
        case 3:
            do
            {
                clearScreenWithTitle();
                comandMenu();
                printf("Escolha: ");
                scanf("%d", &optionComand);
                getchar();

                switch (optionComand)
                {
                case 1:
                    addComands();
                    break;

                case 2:
                    listComands();
                    break;
                case 3:
                    addProductComand();
                    break;
                case 4:
                   
                case 5:
                    break;
                case 6:
                    break;
                default:
                    printf("Opção inválida! Tente novamente.\n");
                    Sleep(1000);
                }

                system("pause"); // Pausa antes de limpar a tela
            } while (optionComand != 6);
            break;
        case 4:
            printf("Saindo...\n");
            Sleep(1500);
            break;

        default:
            printf("Opção inválida! Tente novamente.\n");
            Sleep(1000);
        }

    } while (option != 4);

    return 0;
}
