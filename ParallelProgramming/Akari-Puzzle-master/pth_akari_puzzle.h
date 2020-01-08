#ifndef PTH_AKARI_PUZZLE_H
#define PTH_AKARI_PUZZLE_H

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<pthread.h> 
#include<time.h>

#pragma comment(lib, "pthreadVC2.lib")

#define EMPTY 0
#define DOT 1
#define COVER 2
#define BARRIER 3
#define BARRIER_0 4
#define BARRIER_1 5
#define BARRIER_2 6
#define BARRIER_3 7
#define BARRIER_4 8
#define BLUB 9 


#define NORTH  (1<<0)
#define EAST   (1<<1)
#define SOUTH  (1<<2)
#define WEST   (1<<3)

typedef int Elem_type;

typedef struct Node{
	int number;
	size_t row;
	size_t col;
	struct Node* next;
}Num_barrier, *Barrier_list;

int m;//row num
int n;//column num

Elem_type* g_broad;//the global broad

void read_dimension(FILE*, int*, int*);
void read_broad(FILE*,Elem_type*);
void print_broad(Elem_type*);
Barrier_list create_barrier_list(Elem_type*);

void copy_broad(Elem_type*, const Elem_type*);

int check_complete(Elem_type*);

void* solve_puzzle(void*);
//int solve_puzzle(Barrier_list, Elem_type*);
int put_blub(int, int, Elem_type*);
int put_dot(int, int, Elem_type*);

void* handle_empty(void*);
int next_empty(int, Elem_type*);

#endif