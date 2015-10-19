/*
 * This file is part of MAMMULT: Metrics And Models for Multilayer Networks
 *  
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
