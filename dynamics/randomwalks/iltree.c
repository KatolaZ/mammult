/*
 *
 * A simple insert-lookup static btree datatype
 *
 */

#include <stdlib.h>
#include "iltree.h"
#include <stdio.h>


void __recursive_preorder(node_t *cur, ilfunc_t *funs){
  
  if(cur->left){
    __recursive_preorder(cur->left, funs);
  }
  funs->print(cur->info, funs->fileout);
  if(cur->right){
    __recursive_preorder(cur->right, funs);
  }
}

/*
 *
 * Recursive push of nodes in the nodecache :-)
 *
 */

void __recursive_destroy(node_t *cur, ilfunc_t *funs){
  if(cur->left){
    __recursive_destroy(cur->left, funs);
    cur->left = NULL;
  }
  if(cur->right){
    __recursive_destroy(cur->right, funs);
    cur->right = NULL;
  }
}


int __recursive_insert(node_t *cur, node_t *elem, ilfunc_t *f){
  
  int res ;
  res = f->compare(cur->info, elem->info);
  /*  printf("res: %d\n", res); */
  if ( res > 0){
    if (cur->left){
      return __recursive_insert(cur->left, elem, f);
    }
    else{
      cur->left = elem;
      return 0;
    }
  }
  else if (res < 0){
    if (cur->right){
      return __recursive_insert(cur->right, elem, f);
    }
    else{
      cur->right = elem;
      return 0;
    }
  }
  printf("warning!!!!! duplicate entry!!!!!!\n\n");
  return -1;
}



void* __recursive_lookup(node_t *cur, void *v, ilfunc_t *f){
  
  int res;

  res = f->compare(cur->info, v);

  if (res > 0){
    if(cur->left)
      return __recursive_lookup(cur->left, v, f);
    else
      return NULL;
    
  }
  else if (res < 0){
    if(cur->right)
      return __recursive_lookup(cur->right, v, f);
    else
      return NULL;
  }
  else
    return cur->info;
}

void __recursive_map(node_t *cur, void (*func)(void*)){
  
  if (cur->left)
    __recursive_map(cur->left, func);
  func(cur->info);
  if (cur->right)
    __recursive_map(cur->right, func);
}

void __recursive_map_args(node_t *cur, void (*func)(void*, void*), void *args){
  
  if (cur->left)
    __recursive_map_args(cur->left, func, args);
  func(cur->info, args);
  if (cur->right)
    __recursive_map_args(cur->right, func, args);
}



iltree_t iltree_create(iltree_t t){
  if (!t) {
    t = (iltree_t)malloc(sizeof(iltree_struct_t));
  }
  t->root = NULL;
  return t;
}


void iltree_set_funs(iltree_t t, ilfunc_t *funs){
  
  t->funs = *funs;
}


void iltree_insert(iltree_t t, void *elem){
  
  node_t *n;
  
  n = (node_t*)malloc(sizeof(node_t));
  n->info = t->funs.alloc(); 
  t->funs.copy(elem, n->info);
  n->left = n->right = NULL;
  if (t->root == NULL){
    t->root = n;
  }
  else{
    __recursive_insert(t->root, n, & (t->funs));
  }
}


void iltree_destroy(iltree_t t){
  
  if(t->root)
    __recursive_destroy(t->root, & (t->funs));
  free(t);
}




void iltree_view_pre(iltree_t t){
  
  if (t->root){
    /*printf("----\n");*/
    __recursive_preorder(t->root, & (t->funs));
    /*printf("----\n");*/
  }
  else
    printf("----- Empty tree!!!! -----\n");
  
}



void* iltree_lookup(iltree_t t , void *elem){

  node_t n;
  
  if(t->root)
    return __recursive_lookup(t->root, elem, & (t->funs) );
  else 
    return NULL;
}


void iltree_map(iltree_t t, void (*func)(void*)){
  
  __recursive_map(t->root, func);
  
}


void iltree_map_args(iltree_t t, void (*func)(void*, void*), void *args){
  
  __recursive_map_args(t->root, func, args);
  
}

void* iltree_get_fileout(iltree_t t){

  return t->funs.fileout;
}

void iltree_set_fileout(iltree_t t, void *f){
  
  t->funs.fileout = f;
}
