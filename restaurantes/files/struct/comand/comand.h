#ifndef COMAND_H
#define COMAND_H

#define MAX_NAME 50

typedef struct {
    int id;
    char name[MAX_NAME];
    float totalPrice;
    int restaurantId; 
} Comands;

#endif
