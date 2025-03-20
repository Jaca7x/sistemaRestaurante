# **Struct Products**

```bash
#ifndef PRODUCT_H
#define PRODUCT_H

#define MAX_NAME 50

typedef struct {
    int id;
    char name[MAX_NAME];
    float price;
    int restaurantId;
    int comandId;
} Products;

#endif


```

## restaurantId

- Adicionado para vincular um produto a um restaurante

## comandId

- Adicionado para vincular um produto a uma comanda
