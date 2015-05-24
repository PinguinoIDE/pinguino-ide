/****************************************************/
/* Gestion de listes doublement chainees            */
/* list.h                                           */
/*                                                  */
/* Ecrit par : Daniel Lacroix (all rights reserved) */
/*                                                  */
/* Used in Pinguino Project with Author Permissions */
/****************************************************/

#ifndef __LIST_H__
#define __LIST_H__

#include <typedef.h>

/* pour les listes chainees */
typedef struct _List List;

struct _List {
  void * data;
  List *next;
  List *prev;
};

/* Cree une nouvelle liste chainee. */
/* List *list_new()                 */
#define list_new() NULL

/* Rajoute en tete de la liste pList l'element data. */
/* Renvoie le nouveau point d'entree de la liste.    */
List *list_prepend(List *pList, void * data);

/* Rajoute en queue de la liste pList l'element data. */
/* Renvoie le nouveau point d'entree de la liste.     */
List *list_append(List *pList, void * data);

/* Rajoute un element apres l'element before. Si l'element before  */
/* n'est pas trouve alors le nouvel element sera rajoute a la fin. */
/* Renvoie ne nouveau point d'entree de la liste pList.            */
List *list_insert_after_item(List *pList, void * before, void * data);

/* Concatene la liste p2 a la liste p1 (p1 devant, p2 derriere)             */
/* Renvoie la nouvelle liste cree. p1 et p2 sont consideres comme detruites */
List *list_concat(List *p1, List *p2);

/* Empile data a la fin de la liste pList.              */
/* Renvoie le nouveau point d'entree de la liste pList. */
/* void list_push(List *pList, void * data);           */
#define list_push(pList,data) list_append(pList,data)

/* Depile le dernier element de la liste pList.                  */
/* Le dernier est place dans data et est sorti de la liste       */
/* list_pop renvoie le nouveau point d'entree de la liste pList. */
List *list_pop(List *pList, void * *data);

/* Renvoie une copie de la liste pList. Les pointeurs data sont copies */
/* pas le contenu pointe. Il s'agit d'une liste avec des "alias" sur   */
/* les elements de la premiere liste.                                  */
List *list_copy(List *pList);

/* Renvoie une copie de la liste pList. Les donnees contenu dans la liste sont copies */
/* avec la fonction copy_func. Elle recoit le pointeur de l'element d'origine et doit */
/* renvoyer un pointeur vers la copie.                                                */
List *list_copy_full(List *pList, void * (* copy_func)(void * data));

/* Libere la liste (pas les elements) */
void list_free(List *pList);

/* Libere la liste et appele la procedure free_func pour chaque elements */
void list_free_full(List *pList, void (* free_func)(void * data));

/* Libere la liste et appele la procedure free (du C) pour chaque elements */
void list_free_full_simple(List *pList);

/* Libere un element de la liste (pas ce que l'element pointe si c'est un pointeur) */
/* Renvoie le nouveau point d'entree de la liste.                                   */
List *list_free_item(List *pList, void * data);

/* Libere un element de la liste (pas ce que l'element pointe si c'est un pointeur) */
/* Si l'element a ete libere alors done == TRUE sinon FALSE.                        */
/* Renvoie le nouveau point d'entree de la liste.                                   */
List *list_free_item_with_check(List *pList, void * data, boolean *done);

/* Libere un element de la liste pList (pas la donnee pointe par data) */
/* Renvoie le nouveau point d'entree de la liste.                      */
List *list_free_chunk(List *pList, List *pToFree);

/* Renvoi le nombre d'element contenue dans la liste */
/* Cette operation est realisee par comptage.        */
u32 list_nb_item(List *pList);

/* Renvoie l'element a la position item_nb dans la liste pList */
/* Si l'element item_nb n'exite pas, la fonction renvoi NULL.  */
/* item_nb commence a 1.                                       */
void * list_item(List *pList, u32 item_nb);

/* Renvoie la donnee correspondant a l'element de liste pList */
/* void * list_data(List *pList);                            */
#define list_data(pList) ((pList)?(pList)->data:NULL)

/* Renvoie l'element a la derniere position de la liste pList. */
/* Renvoie NULL si la liste est vide.                          */
/* List *list_last(List *pList);                               */
#define list_last(pList) ((pList)?(pList)->prev:NULL)

/* Renvoie TRUE si la liste pList est vide. Renvoie FALSE sinon. */
/* boolean list_is_empty(List *pList);                           */
#define list_is_empty(pList) (pList == NULL)

/* Renvoie TRUE si pCurrentList est au dela du dernier   */
/* element de la liste pList. Sinon renvoie FALSE        */
/* boolean list_is_end(List *pList, List *pCurrentList); */
#define list_is_end(pList,pCurrentList) (pCurrentList == NULL)

/* Renvoie TRUE si pCurrentList est le premier            */
/* element de la liste pList. Sinon renvoie FALSE         */
/* boolean list_is_first(List *pList, List *pCurrentList);*/
#define list_is_first(pList,pCurrentList) ((pCurrentList) == pList)

/* Renvoie TRUE si pCurrentList est le dernier            */
/* element de la liste pList. Sinon renvoie FALSE         */
/* boolean list_is_last(List *pList, List *pCurrentList); */
#define list_is_last(pList,pCurrentList)\
 ((pCurrentList != NULL) && (pList != NULL) && ((pCurrentList)->next == pList))

/* Renvoie l'element apres pCurrentList dans la liste pList.       */
/* Si on arrive au dela de la fin de la liste pList, renvoie NULL. */
/* List *list_next(List *pList, List *pCurrentList);               */
#define list_next(pList,pCurrentList)\
 (((pCurrentList)&&((pCurrentList)->next!=pList))?(pCurrentList)->next:NULL)

/* Renvoie l'element avant pCurrentList dans la liste pList.          */
/* Si l'element precedent est avant le premier element, renvoie NULL. */
/* List *list_prev(List *pList, List *pCurrentList);                  */
#define list_prev(pList,pCurrentList)\
 (((pCurrentList)&&((pCurrentList)!=pList))?(pCurrentList)->prev:NULL)

/* Deplace pCurrentList sur le prochain element de la liste pList */
/* Si on arrive a la fin pCurrentList devient NULL.               */
/* void list_move_next(List *pList, List *pCurrentList);          */
#define list_move_next(pList,pCurrentList) (pCurrentList = list_next(pList,pCurrentList))

/* Deplace pCurrentList sur l'element precedent de la liste pList         */
/* Si on arrive avant le premier element alors pCurrentList devient NULL. */
/* void list_move_prev(List *pList, List *pCurrentList);                  */
#define list_move_prev(pList,pCurrentList) (pCurrentList = list_prev(pList,pCurrentList))

/* Trie la liste pList. La fonction cmp_func est utilise pour  */
/* determine l'ordre. Elle doit renvoyer 0 si p1 pointe sur un */
/* element equivalent a p2. < 0 si p1 est inferieur a p2, >0   */
/* si p2 est superieur a p1.                                   */
/* Renvoie la liste triee.                                     */
List *list_sort(List *pList,int (* cmp_func)(void * p1, void * p2));

/* Insere l'element data dans la liste pList de maniere trie */
/* d'apres la fonction cmp_func. Elle doit renvoyer 0 si p1  */
/* pointe sur un element equivalent a p2. < 0 si p1 est      */
/* inferieur a p2, >0 si p2 est superieur a p1.              */
/* Renvoie le nouveau point d'entree de la liste.            */
List *list_insert_sorted(List *pList,int (* cmp_func)(void * p1, void * p2),
  void * data);

/* Inverse l'ordre des elements de la liste pList          */
/* Renvoie le nouveau point d'entree de la liste           */
/* Cette operation peut ce faire pendant que l'on parcours */
/* (pas au sens multithreade du terme)                     */
/* la liste mais bien evidemment, le sens s'inverse.       */
List *list_invert(List *pList);

/* Recherche l'element de liste qui contient la premiere donnee data */
/* Renvoie l'element de liste si trouve, NULL sinon.                 */
List *list_find(List *pList, void * data);

/* Recherche l'element de liste qui contient la premiere donnee pour laquelle la */
/* fonction find_func renvoie TRUE. La fonction find_func prend en parametre le  */
/* pointeur contenu dans la liste et le pointeur your_data qui est donne par     */
/* l'utilisateur (pour y stocker ce dont il a besoin pour la comparaison).       */
/* Renvoie l'element de liste si trouve, NULL sinon.                             */
List *list_find_full(List *pList, void * your_data,
  boolean (* find_func)(void * data, void * your_data));

#endif /* __LIST_H__ */


