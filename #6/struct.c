#include <stdio.h>

typedef struct {   
    char name[20];
    char sex; 
    int age;
    double height; 
    double weight; 
} person_t;

main()
{
    person_t p = {"Tom", 'M', 19, 175.2, 69.5};
    
    printf("%s %c %d %f %f\n", p.name, p.sex, p.age, p.height, p.weight);
    p.age ++;
    p.height += 0.7;
    p.weight -= 1.5;
    printf("%s %c %d %f %f\n", p.name, p.sex, p.age, p.height, p.weight);

    return 0;
}