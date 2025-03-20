# **Struct Restaurants**

```bash
#ifndef RESTAURANT_H
#define RESTAURANT_H

#define MAX_NAMERESTAURANT 70
#define MAX_CONTACT 50
#define MAX_TYPE 30

typedef struct {
    int id;
    char name[MAX_NAMERESTAURANT];
    char type[MAX_TYPE]; 
    char contact[MAX_CONTACT];
} Restaurant;

#endif

```

