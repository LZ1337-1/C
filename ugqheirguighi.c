#include <stdio.h>
#include <stdlib.h>
#define UNKNOWN NULL

typedef struct
{
    char day;
    char month;
    short year;
} date_t;

// Code pour la gestion des dates (constructeur, comparaison, affichage) ...

///////////////////////////////////////////////////////////////////////////////////////////

// Implement compare_date function
int compare_date(date_t d1, date_t d2)
{
    if (d1.year != d2.year)
    {
        return d1.year - d2.year;
    }
    if (d1.month != d2.month)
    {
        return d1.month - d2.month;
    }
    return d1.day - d2.day;
}

// Implement print_date function
void print_date(date_t d)
{
    printf("%02d/%02d/%04d", d.day, d.month, d.year);
}

// Implement date function
date_t date(char day, char month, short year)
{
    date_t d;
    d.day = day;
    d.month = month;
    d.year = year;
    return d;
}

typedef const char *person_t;

typedef enum
{
    BIRTH,
    WEDDING,
    DIVORCE,
    DEATH
} event_type_t;

typedef struct event_s
{
    date_t date;
    event_type_t type;
    person_t person1;
    person_t person2;
    person_t person3;
    struct event_s *previous;
    struct event_s *next;
} event_t;

event_t *event(date_t d, event_t *previous)
{
    event_t *e = malloc(sizeof(event_t));
    e->date = d;
    e->previous = previous;
    e->next = NULL;
    if (previous != NULL)
    {
        if (compare_date(previous->date, d) < 0 && previous->next == NULL)
        {
            previous->next = e;
        }
        else
        {
            // Erreur, date invalide ou liste mal formée
            free(e);
            return NULL;
        }
    }
    return e;
}

event_t *birth(date_t d, person_t mother, person_t father, person_t child, event_t *prev)
{
    event_t *e = event(d, prev);
    if (e == NULL)
        return NULL;
    e->type = BIRTH;
    e->person1 = mother;
    e->person2 = father;
    e->person3 = child;
    return e;
}

event_t *wedding(date_t d, person_t groom, person_t bride, event_t *prev)
{
    event_t *e = event(d, prev);
    if (e == NULL)
        return NULL;
    e->type = WEDDING;
    e->person1 = groom;
    e->person2 = bride;
    e->person3 = NULL;
    return e;
}

event_t *divorce(date_t d, event_t *wedding, event_t *prev)
{
    event_t *e = event(d, prev);
    if (e == NULL)
        return NULL;
    e->type = DIVORCE;
    e->person1 = wedding->person1;
    e->person2 = wedding->person2;
    e->person3 = NULL;
    return e;
}

event_t *death(date_t d, person_t person, event_t *prev)
{
    event_t *e = event(d, prev);
    if (e == NULL)
        return NULL;
    e->type = DEATH;
    e->person1 = person;
    e->person2 = NULL;
    e->person3 = NULL;
    return e;
}

void delete_event(event_t *e)
{
    if (e == NULL)
        return;
    delete_event(e->next);
    free(e);
}

void print_event(event_t *e)
{
    if (e == NULL)
        return;
    print_date(e->date);
    printf(": ");
    switch (e->type)
    {
    case BIRTH:
        printf("Birth of %s (father: %s, mother: %s)", e->person3, e->person2, e->person1);
        break;
    case WEDDING:
        printf("Wedding of %s and %s", e->person1, e->person2);
        break;
    case DIVORCE:
        printf("Divorce of %s and %s", e->person1, e->person2);
        break;
    case DEATH:
        printf("Death %s", e->person1);
        break;
    }
    printf("\n");
}

void print_chronology(event_t *e)
{
    if (e == NULL)
        return;
    print_event(e);
    print_chronology(e->next);
}

int main()
{
    const char *H8 = "Henri VIII";
    const char *CA = "Catherine d'Aragon";
    const char *AB = "Anne Boleyn";
    const char *E1 = "Elisabeth I";
    const char *JS = "Jane Seymour";
    const char *E6 = "Edouard VI";
    const char *AC = "Anne de Clèves";
    const char *CH = "Catherine Howard";
    const char *CP = "Catherine Parr";

    event_t *e, *start, *w1, *w2;
    e = NULL;
    start = e = birth(date(16, 12, 1485), UNKNOWN, UNKNOWN, CA, e);
    e = birth(date(28, 6, 1491), UNKNOWN, UNKNOWN, H8, e);
    w1 = e = wedding(date(11, 6, 1509), H8, CA, e);
    e = wedding(date(25, 1, 1533), H8, AB, e);
    e = divorce(date(23, 5, 1533), w1, e);
    e = birth(date(7, 9, 1533), AB, H8, E1, e);
    e = death(date(7, 1, 1536), CA, e);
    e = wedding(date(30, 5, 1536), H8, JS, e);
    e = birth(date(12, 10, 1537), JS, H8, E6, e);
    w2 = e = wedding(date(6, 1, 1540), H8, AC, e);
    e = divorce(date(9, 7, 1540), w2, e);
    e = wedding(date(28, 7, 1540), H8, CH, e);
    e = wedding(date(12, 7, 1543), H8, CP, e);
    e = death(date(28, 1, 1547), H8, e);
    e = death(date(6, 7, 1553), E6, e);
    e = death(date(24, 3, 1603), E1, e);

    print_chronology(start);
    delete_event(start);
    return EXIT_SUCCESS;
}
