#ifndef PRODUCT_H
#define PRODUCT_H

#define MAX_NAME 50

typedef struct
{
    int id;
    char name[MAX_NAME];
    float price;
    int restaurantId; // Adicionado para vincular um produto a um restaurante
    int comandId;     // Adicionado para vincular um produto a uma comanda
} Products;

#endif
