#ifndef __ILTREE_H__
#define __ILTREE_H__


typedef struct node{
  void* info;
  struct node* left;
  struct node* right;
} node_t;

typedef struct{
  void* (*alloc)();
  void (*dealloc)(void*);
  void (*copy)(void *src, void *dst);
  int (*compare)(void*, void*);
  void (*print)(void*, void*);
  void *fileout;
} ilfunc_t;


typedef struct {
  node_t* root;
  ilfunc_t funs;
} iltree_struct_t;



typedef iltree_struct_t* iltree_t;


void iltree_set_funs(iltree_t, ilfunc_t *);

void iltree_destroy(iltree_t);

void iltree_empty(iltree_t);

void iltree_insert(iltree_t, void*);

void* iltree_lookup(iltree_t, void*);

void iltree_view_pre(iltree_t);

iltree_t iltree_create(iltree_t);

void iltree_empty_cache(iltree_t);

void iltree_map(iltree_t, void (*func)(void*));

void iltree_map_args(iltree_t, void (*func)(void*, void*), void*);

void* iltree_get_fileout(iltree_t t);

void iltree_set_fileout(iltree_t t, void *f);

#endif /* __ILTREE_H__*/
