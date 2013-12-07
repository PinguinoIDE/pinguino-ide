/****************************************************/
/* Gestion de listes doublement chainees            */
/* list.c                                           */
/*                                                  */
/* Ecrit par : Daniel Lacroix (all rights reserved) */
/*                                                  */
/* Used in Pinguino Project with Author Permissions */
/****************************************************/

#ifndef __LIST_C__
#define __LIST_C__

#include <typedef.h>
#include <const.h>
#include <stdlib.h>
#include <list.h>
#include <stdio.h>
#include <pinguinoserial1.c>

/*****************************************************/
/* Rajoute en tete de la liste pList l'element data. */
/* Renvoie le nouveau point d'entree de la liste.    */
List *list_prepend(List *pList, void * data)
{ List *vList;

  /* on alloue la memoire pour le nouvel element */
  if((vList = (List *)malloc(sizeof(List))) == NULL)
  {
	#ifdef DEBUG
	serial1printf("malloc failed\n");
	#endif
	exit(1);
  }

  if(pList == NULL)
  { /* la liste est vide */
    vList->data = data;
    vList->prev = vList;
    vList->next = vList;
  } else {
    /* il y a deja au moins un element dans la liste */
    vList->data = data;
    vList->prev = pList->prev;
    vList->next = pList;
    pList->prev->next = vList;
    pList->prev = vList;
  }
  return(vList);
}
/*****************************************************/

/******************************************************/
/* Rajoute en queue de la liste pList l'element data. */
/* Renvoie le nouveau point d'entree de la liste.     */
List *list_append(List *pList, void * data)
{ List *vList;

  /* on alloue la memoire pour le nouvelle element */
  if((vList = (List *)malloc(sizeof(List))) == NULL)
  {
	#ifdef DEBUG
	serial1printf("malloc failed\n ");
	#endif
	exit(1);
  }

  if(pList == NULL)
  { /* la liste est vide */
    vList->data = data;
    vList->prev = vList;
    vList->next = vList;
    return(vList);
  } else {
    /* il y a deja au moins un element dans la liste */
    vList->data = data;
    vList->prev = pList->prev;
    vList->next = pList;
    pList->prev->next = vList;
    pList->prev = vList;
    return(pList);
  }
}
/******************************************************/

/**************************************/
/* Libere la liste (pas les elements) */
void list_free(List *pList)
{ List *vListMove,*vListMoveNext;

  vListMove = pList;
  while(!list_is_end(pList,vListMove))
  {
    vListMoveNext = list_next(pList,vListMove);
    free(vListMove);
    vListMove = vListMoveNext;
  }
}
/**************************************/

/*************************************************************************/
/* Libere la liste et appele la procedure free_func pour chaque elements */
void list_free_full(List *pList, void (* free_func)(void * data))
{ List *vListMove,*vListMoveNext;

  vListMove = pList;
  while(!list_is_end(pList,vListMove))
  {
    vListMoveNext = list_next(pList,vListMove);
    free_func(vListMove->data);
    free(vListMove);
    vListMove = vListMoveNext;
  }
}
/*************************************************************************/

/***************************************************************************/
/* Libere la liste et appele la procedure free (du C) pour chaque elements */
void list_free_full_simple(List *pList)
{ List *vListMove,*vListMoveNext;

  vListMove = pList;
  while(!list_is_end(pList,vListMove))
  {
    vListMoveNext = list_next(pList,vListMove);
    if(vListMove->data != NULL) free(vListMove->data);
    free(vListMove);
    vListMove = vListMoveNext;
  }
}
/***************************************************************************/

/*****************************************************/
/* Renvoi le nombre d'element contenue dans la liste */
/* Cette operation est realisee par comptage.        */
u32 list_nb_item(List *pList)
{ List *vListMove;
  u32 compteur = 0;

  vListMove = pList;
  while(!list_is_end(pList,vListMove))
  {
    compteur++;
    list_move_next(pList,vListMove);
  }
  return(compteur);
}
/*****************************************************/

/************************************************************************************/
/* Libere un element de la liste (pas ce que l'element pointe si c'est un pointeur) */
/* Renvoie le nouveau point d'entree de la liste.                                   */
List *list_free_item(List *pList, void * data)
{ List *vListMove,*vListMoveNext;

  /* on parcourt la liste pList a la recherche de l'element a supprimer */
  vListMove = pList;
  while(!list_is_end(pList,vListMove) && (vListMove->data != data))
  { list_move_next(pList,vListMove); }
  /* si on a trouve l'element, on le libere */
  if(!list_is_end(pList,vListMove))
  {
    /* si il n'y a que notre element dans la liste */
    if(vListMove->next == vListMove)
    { /* on libere l'element */
      free(vListMove);
      /* on renvoie une liste vide */
      return(NULL);
    } else {
      /* on recupere l'element qui suit celui que l'on veut supprimer */
      vListMoveNext = vListMove->next;
      /* on chaine l'element precedent avec celui qui suit */
      vListMove->prev->next = vListMoveNext;
      vListMoveNext->prev = vListMove->prev;
      /* on libere l'element a liberer */
      free(vListMove);
      if(pList == vListMove)
      {
        /* l'element a supprimer etait le premier, c'est */
        /* donc le suivant qui devient le premier        */
        return(vListMoveNext);
      } else {
        /* l'element a supprimer n'etait pas le premier.       */
        /* le premiere element de la liste ne change donc pas. */
        return(pList);
      }
    }
  }
  /* si on est ici c'est l'element a supprimer n'existe */
  /* pas dans la liste. on rend donc la liste intacte.  */
  return(pList);
}
/************************************************************************************/

/************************************************************************************/
/* Libere un element de la liste (pas ce que l'element pointe si c'est un pointeur) */
/* Si l'element a ete libere alors done == TRUE sinon FALSE.                        */
/* Renvoie le nouveau point d'entree de la liste.                                   */
List *list_free_item_with_check(List *pList, void * data, boolean *done)
{ List *vListMove,*vListMoveNext;

  /* on parcourt la liste pList a la recherche de l'element a supprimer */
  vListMove = pList;
  while(!list_is_end(pList,vListMove) && (vListMove->data != data))
  { list_move_next(pList,vListMove); }
  /* si on a trouve l'element, on le libere */
  if(!list_is_end(pList,vListMove))
  {
    /* on signal que l'element a bien ete trouve */
    *done = TRUE;  
    /* si il n'y a que notre element dans la liste */
    if(vListMove->next == vListMove)
    { /* on libere l'element */
      free(vListMove);
      /* on renvoie une liste vide */
      return(NULL);
    } else {
      /* on recupere l'element qui suit celui que l'on veut supprimer */
      vListMoveNext = vListMove->next;
      /* on chaine l'element precedent avec celui qui suit */
      vListMove->prev->next = vListMoveNext;
      vListMoveNext->prev = vListMove->prev;
      /* on libere l'element a liberer */
      free(vListMove);
      if(pList == vListMove)
      {
        /* l'element a supprimer etait le premier, c'est */
        /* donc le suivant qui devient le premier        */
        return(vListMoveNext);
      } else {
        /* l'element a supprimer n'etait pas le premier.       */
        /* le premiere element de la liste ne change donc pas. */
        return(pList);
      }
    }
  }
  /* si on est ici c'est l'element a supprimer n'existe pas dans la liste.*/
  /* on signal de fait que l'on n'a pas trouve l'element cherche.         */
  *done = FALSE;
  /* on rend donc la liste intacte. */
  return(pList);
}
/************************************************************************************/

/***************************************************************/
/* Renvoie l'element a la position item_nb dans la liste pList */
/* Si l'element item_nb n'exite pas, la fonction renvoi NULL.  */
/* item_nb commence a 1.                                       */
void * list_item(List *pList, u32 item_nb)
{ List *vListMove;
  u32 offset=0;

  vListMove = pList;
  while(!list_is_end(pList,vListMove) && (offset<item_nb-1))
  { list_move_next(pList,vListMove); offset++; }

  if(!list_is_end(pList,vListMove))
  { /* si il y a bien un element a la position demande, on le renvoie */
    return(vListMove->data);
  } else {
    /* si on c'est retrouve a la fin de la liste, il n'y a pas d'element */
    /* a la postion demande. On renvoie donc NULL pour le signaler       */
    return(NULL);
  }
}
/***************************************************************/

/*******************************************************************/
/* Rajoute un element apres l'element before. Si l'element before  */
/* n'est pas trouve alors le nouvel element sera rajoute a la fin. */
/* Renvoie ne nouveau point d'entree de la liste pList.            */
List *list_insert_after_item(List *pList, void * before, void * data)
{ List *vList;
  List *vListMove;

  /* on alloue la memoire pour le nouvelle element */
  if((vList = (List *)malloc(sizeof(List))) == NULL)
  {
	#ifdef DEBUG
	serial1printf("malloc failed\n");
	#endif
	exit(1);
  }

  vListMove = pList;
  while(!list_is_end(pList,vListMove) && (vListMove->data != before))
  { list_move_next(pList,vListMove); }

  /* si on a trouve l'element precedent */
  if(!list_is_end(pList,vListMove))
  {
    vList->data = data;
    vList->prev = vListMove;
    vList->next = vListMove->next;
    vListMove->next = vList;
    vList->next->prev = vList;
    return(pList);
  /* si on n'a pas trouve l'element precedent */
  } else {
    if(pList == NULL)
    { /* si la liste est vide */
      vList->data = data;
      vList->prev = vList;
      vList->next = vList;
      return(vList);
    } else {
      /* c'est une insertion en queue d'une liste non vide */
      vList->data = data;
      vList->prev = pList->prev;
      vList->next = pList;
      vList->prev->next = vList;
      pList->prev = vList;
      return(pList);
    }
  }
}
/*********************************************************************/

/****************************************************************************/
/* Concatene la liste p2 a la liste p1 (p1 devant, p2 derriere)             */
/* Renvoie la nouvelle liste cree. p1 et p2 sont consideres comme detruites */
List *list_concat(List *p1, List *p2)
{ List *vListPrev;

  if(p1 == NULL) return(p2);
  if(p2 == NULL) return(p1);

  p1->prev->next = p2;
  vListPrev = p1->prev;
  p1->prev = p2->prev;
  p2->prev->next = p1;
  p2->prev = vListPrev;
    
  return(p1);
}
/****************************************************************************/

/*****************************************************************/
/* Depile le dernier element de la liste pList.                  */
/* Le dernier est place dans data et est sorti de la liste       */
/* list_pop renvoie le nouveau point d'entree de la liste pList. */
List *list_pop(List *pList, void * *data)
{ List *vListPrev;

  if(pList == NULL) { *data = NULL; return(NULL); }
  
  /* si il n'y a que notre element dans la liste */
  if(pList->next == pList)
  {
    *data = pList->data;
    /* on libere l'element */
    free(pList);
    /* on renvoie une liste vide */
    return(NULL);
  } else {
    *data = pList->prev->data;
    vListPrev = pList->prev->prev;
    vListPrev->next = pList;
    free(pList->prev);
    pList->prev = vListPrev;
    return(pList);
  }
}
/*****************************************************************/

/***********************************************************************/
/* Renvoie une copie de la liste pList. Les pointeurs data sont copies */
/* pas le contenu pointe. Il s'agit d'une liste avec des "alias" sur   */
/* les elements de la premiere liste.                                  */
List *list_copy(List *pList)
{ List *vListMove, *vCopy;

  vCopy = list_new();
  for(vListMove = pList;!list_is_end(pList,vListMove);list_move_next(pList,vListMove))
  {
    vCopy = list_append(vCopy, vListMove->data);
  }
  return(vCopy);
}
/***********************************************************************/

/**************************************************************************************/
/* Renvoie une copie de la liste pList. Les donnees contenu dans la liste sont copies */
/* avec la fonction copy_func. Elle recoit le pointeur de l'element d'origine et doit */
/* renvoyer un pointeur vers la copie.                                                */
List *list_copy_full(List *pList, void * (* copy_func)(void * data))
{ List *vListMove, *vCopy;

  vCopy = list_new();
  for(vListMove = pList;!list_is_end(pList,vListMove);list_move_next(pList,vListMove))
  {
    vCopy = list_append(vCopy, copy_func(vListMove->data));
  }
  return(vCopy);
}
/**************************************************************************************/

/***********************************************************************/
/* Libere un element de la liste pList (pas la donnee pointe par data) */
/* Renvoie le nouveau point d'entree de la liste.                      */
List *list_free_chunk(List *pList, List *pToFree)
{ List *vListNext;

  if(pList == NULL) return(NULL);
  if(pToFree == NULL) return(pList);

  /* si il n'y a que notre element dans la liste */
  if(pToFree->next == pToFree)
  { /* on libere l'element */
    free(pToFree);
    /* on renvoie une liste vide */
    return(NULL);
  } else {
    /* on recupere l'element qui suit celui que l'on veut supprimer */
    vListNext = pToFree->next;
    /* on chaine l'element precedent avec celui qui suit */
    pToFree->prev->next = vListNext;
    vListNext->prev = pToFree->prev;
    /* on libere l'element a liberer */
    free(pToFree);
    if(pList == pToFree)
    {
      /* l'element a supprimer etait le premier, c'est */
      /* donc le suivant qui devient le premier        */
      return(vListNext);
    } else {
      /* l'element a supprimer n'etait pas le premier.       */
      /* le premiere element de la liste ne change donc pas. */
      return(pList);
    }
  }
}
/***********************************************************************/

/***************************************************************/
/* Trie la liste pList. La fonction cmp_func est utilise pour  */
/* determine l'ordre. Elle doit renvoyer 0 si p1 pointe sur un */
/* element equivalent a p2. < 0 si p1 est inferieur a p2, >0   */
/* si p2 est superieur a p1.                                   */
/* Renvoie la liste triee.                                     */
List *list_sort(List *pList,int (* cmp_func)(void * p1, void * p2))
{ List *vListMove;
  List *vListSmallest;
  List *vListRes;

  vListRes = list_new();

  /* l'algorithme de trie utilise est le trie par selection          */
  /* cela consiste a chercher le plus petit element le sortir de     */
  /* la liste pList et le rajouter a la fin de la liste vListRes     */
  /* et de repeter cela jusqu'a ce qu'il n'y ai plus rien dans pList */
  while(!list_is_empty(pList))
  {
    for(vListMove = list_next(pList,pList), vListSmallest = pList;
      !list_is_end(pList,vListMove); list_move_next(pList,vListMove))
    {
      if(cmp_func(list_data(vListMove),list_data(vListSmallest)) < 0)
      { vListSmallest = vListMove; }
    }
    vListRes = list_append(vListRes,list_data(vListSmallest));
    pList = list_free_chunk(pList, vListSmallest);
  }

  return(vListRes);
}
/***************************************************************/

/*************************************************************/
/* Insere l'element data dans la liste pList de maniere trie */
/* d'apres la fonction cmp_func. Elle doit renvoyer 0 si p1  */
/* pointe sur un element equivalent a p2. < 0 si p1 est      */
/* inferieur a p2, >0 si p2 est superieur a p1.              */
/* Renvoie le nouveau point d'entree de la liste.            */
List *list_insert_sorted(List *pList,int (* cmp_func)(void * p1, void * p2), void * data)
{ List *vListMove, *vList;

  /* on recherche le premier element superieur ou egale au nouveau */
  for(vListMove = pList; !list_is_end(pList,vListMove) &&
    (cmp_func(list_data(vListMove),data) < 0);
    vListMove = list_move_next(pList,vListMove));

  /* on alloue la memoire pour le nouvelle element */
  if((vList = (List *)malloc(sizeof(List))) == NULL)
  {
	#ifdef DEBUG
	serial1printf("malloc failed\n");
	#endif
	exit(1);
  }

  if(list_is_empty(pList))
  { /* si la liste est vide */
    vList->data = data;
    vList->prev = vList;
    vList->next = vList;
    return(vList);
  } else {
    vList->data = data;
    vList->prev = vListMove->prev;
    vList->next = vListMove;
    vListMove->prev = vList;
    vList->prev->next = vList;
    if(pList == vListMove)
      return(vList);
    else
      return(pList);
  }
}
/*************************************************************/

/***********************************************************/
/* Inverse l'ordre des elements de la liste pList          */
/* Renvoie le nouveau point d'entree de la liste           */
/* Cette operation peut ce faire pendant que l'on parcours */
/* (pas au sens multithreade du terme)                     */
/* la liste mais bien evidemment, le sens s'inverse.       */
List *list_invert(List *pList)
{ List *vListMove, *vListMoveNext;
  
  if(pList == NULL) return(NULL);
  
  vListMove = pList;
  do {
    vListMoveNext = vListMove->next;
    vListMove->next = vListMove->prev;
    vListMove->prev = vListMoveNext;
    vListMove = vListMoveNext;
  } while(vListMove != pList);
  /* pList est devenu le dernier element, donc le premier est celui qui suit */
  return(pList->next);
}
/***********************************************************/

/*********************************************************************/
/* Recherche l'element de liste qui contient la premiere donnee data */
/* Renvoie l'element de liste si trouve, NULL sinon.                 */
List *list_find(List *pList, void * data)
{ List *vListMove;

  for(vListMove = pList; !list_is_end(pList,vListMove) && (list_data(vListMove) != data);
    list_move_next(pList,vListMove));

  return(vListMove);
}
/*********************************************************************/

/*********************************************************************************/
/* Recherche l'element de liste qui contient la premiere donnee pour laquelle la */
/* fonction find_func renvoie TRUE. La fonction find_func prend en parametre le  */
/* pointeur contenu dans la liste et le pointeur your_data qui est donne par     */
/* l'utilisateur (pour y stocker ce dont il a besoin pour la comparaison).       */
/* Renvoie l'element de liste si trouve, NULL sinon.                             */
List *list_find_full(List *pList, void * your_data,
  boolean (* find_func)(void * data, void * your_data))
{ List *vListMove;

  for(vListMove = pList;
    !list_is_end(pList,vListMove) && !find_func(list_data(vListMove),your_data);
    list_move_next(pList,vListMove));

  return(vListMove);
}
/*********************************************************************************/

#endif
