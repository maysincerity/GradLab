# include <bits/stdc++.h>
#include<pthread.h>
# include "akari.h"
using namespace std;

namespace aka{
//请在命名空间内编写代码，否则后果自负
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>

#define EMPTY 0
#define REDX 1
#define COVER 2
#define BLACKBLOCK 3
#define BLACKBLOCK0 4
#define BLACKBLOCK1 5
#define BLACKBLOCK2 6
#define BLACKBLOCK3 7
#define BLACKBLOCK4 8
#define LIGHTBULB 9


#define D_UP  (1<<0)
#define D_RIGHT   (1<<1)
#define D_DOWN  (1<<2)
#define D_LEFT   (1<<3)

typedef struct Node{
	int number;
	size_t row;
	size_t col;
	struct Node* next;
}Blackblocknode, *BlackblockList;

int m;//row
int n;//col

int* global_checkerboard;//global chess board

BlackblockList create_blackblock_list(int*);

void cp_board(int*, const int*);

int final_check(int*);

void* solve_akari_puzzle(void*);
int put_lightbulb(int, int, int*);
int put_redx(int, int, int*);

void* fill_empty(void*);
int next_empty(int, int*);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int handle = 0;

typedef struct {
	int* boardp;
	BlackblockList blackblock;
} blackblockParam;

typedef struct {
	int* boardp;
	int pos;
} fillemptyParam;

BlackblockList create_blackblock_list(int* broad) {

	int i;
	BlackblockList header, p;
	header = p = NULL;
	for(i = 0; i < m*n; i++){
		if(broad[i] == BLACKBLOCK0){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 0;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}

	for(i = 0; i < m*n; i++){
		if(broad[i] == BLACKBLOCK4){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 4;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	for(i = 0; i < m*n; i++){
		if(broad[i] == BLACKBLOCK3){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 3;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	for(i = 0; i < m*n; i++){
		if(broad[i] == BLACKBLOCK1){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 1;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	for(i = 0; i < m*n; i++){
		if(broad[i] == BLACKBLOCK2){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 2;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	if(p != NULL) p->next = NULL;
	return header;
}


pthread_t* split(int flag, int* broad, BlackblockList blackblock) {
	int* cp_broad = (int*) malloc(sizeof(int) * m*n);
	cp_board(cp_broad,broad);
	int i, success = 0;
	int r = blackblock->row, c = blackblock->col;
	for(i = 0; i < 4; i++) {
		int direction  = (1<<i);
		int type = (flag & direction);
		switch(direction) {
		case D_UP:
			success = (type != 0) ? put_lightbulb(r-1, c, cp_broad):put_redx(r-1, c, cp_broad);
			break;
		case D_RIGHT:
			success = (type != 0) ? put_lightbulb(r, c-1, cp_broad):put_redx(r, c-1, cp_broad);
			break;
		case D_DOWN:
			success = (type != 0) ? put_lightbulb(r+1, c, cp_broad):put_redx(r+1, c, cp_broad);
			break;
		case D_LEFT:
			success = (type != 0) ? put_lightbulb(r, c+1, cp_broad):put_redx(r, c+1, cp_broad);
			break;
		}
		if(!success)
			break;
	}

	if(!success) {
		free(cp_broad);
		return NULL;
	}
	else {
		//alway check the result before we split another thread in case we do the unuseful work
		if(handle) {
			free(cp_broad);
			return NULL;
		}
		blackblockParam* p_params = (blackblockParam*) malloc(sizeof(blackblockParam));
		pthread_t* p_handle = (pthread_t*) malloc(sizeof(pthread_t));
		p_params->blackblock = blackblock->next;
		p_params->boardp = cp_broad;
		pthread_create(p_handle, NULL, solve_akari_puzzle, (void*)p_params);//split
		return p_handle;
	}
}

void* solve_akari_puzzle(void* p_params) {

	//printf("function called \n");
	blackblockParam* params = (blackblockParam*)p_params;
	BlackblockList blackblock = params->blackblock;
	int* broad = params->boardp;

	if(handle) {
		free(broad);
		free(params);
		return NULL;
	}

	if(NULL != blackblock) {//first phase

		int i;
		pthread_t** thread_handles;

		thread_handles = (pthread_t**) calloc(6, sizeof(pthread_t*));//max thread num is 6
		switch(blackblock->number){
		case 4:
			thread_handles[0] = split(D_UP|D_RIGHT|D_DOWN|D_LEFT, broad, blackblock);
			break;
		case 3:
			thread_handles[0] = split(D_UP|D_RIGHT|D_DOWN, broad, blackblock);
			thread_handles[1] = split(D_UP|D_RIGHT|D_LEFT, broad, blackblock);
			thread_handles[2] = split(D_UP|D_DOWN|D_LEFT, broad, blackblock);
			thread_handles[3] = split(D_RIGHT|D_DOWN|D_LEFT, broad, blackblock);
			break;
		case 2:
			thread_handles[0] = split(D_UP|D_RIGHT, broad, blackblock);
			thread_handles[1] = split(D_UP|D_DOWN, broad, blackblock);
			thread_handles[2] = split(D_UP|D_LEFT, broad, blackblock);
			thread_handles[3] = split(D_RIGHT|D_DOWN, broad, blackblock);
			thread_handles[4] = split(D_RIGHT|D_LEFT, broad, blackblock);
			thread_handles[5] = split(D_DOWN|D_LEFT, broad, blackblock);
			break;
		case 1:
			thread_handles[0] = split(D_UP, broad, blackblock);
			thread_handles[1] = split(D_RIGHT, broad, blackblock);
			thread_handles[2] = split(D_DOWN, broad, blackblock);
			thread_handles[3] = split(D_LEFT, broad, blackblock);
			break;
		case 0:
			thread_handles[0] = split(0, broad, blackblock);
			break;
		default:
			break;
		}
		for(i = 0; i < 6;i++)
			if(thread_handles[i] != NULL) {
				//printf("Thead %d is running!\n", i);
				pthread_join(*thread_handles[i], NULL);

			}

		for(i = 0; i < 6;i++)
			if(thread_handles[i] != NULL)
				free(thread_handles[i]);

		free(thread_handles);
		free(broad);
		free(params);
		return NULL;
	} else {//second phase
		fillemptyParam* pp = (fillemptyParam*) malloc(sizeof(fillemptyParam));
		pp->pos = next_empty(-1, broad);
		pp->boardp = broad;
		fill_empty((void*) pp);
		return NULL;
	}
}

void *fill_empty(void* params) {

	fillemptyParam* pp = (fillemptyParam*) params , *pp2;//pp2 as the params of the splited thread
	int* broad, *cp_broad;
	pthread_t* thread_handle;
	int cur;

	broad = (int*)pp->boardp;
	cur = pp->pos;

	if(final_check(broad)){
		pthread_mutex_lock(&lock);
		if(!handle) {
			cp_board(global_checkerboard, broad);
			handle= 1;
		}
		pthread_mutex_unlock(&lock);
		free(pp->boardp);
		free(pp);
		return NULL;
	}
	if(cur == -1) {
		free(pp->boardp);
		free(pp);
		return NULL;
	}

	//alway check the result before we split another thread in case we do the unuseful work
	if(handle) {
		free(pp->boardp);
		free(pp);
		return NULL;
	}

	thread_handle = (pthread_t*) malloc(sizeof(pthread_t));
	cp_broad = (int*)malloc(sizeof(int) * m * n);
	cp_board(cp_broad, broad);
	put_lightbulb(cur / n, cur % n, cp_broad);

	pp2 = (fillemptyParam*) malloc(sizeof(fillemptyParam));
	pp2->pos = next_empty(cur, cp_broad);
	pp2->boardp = cp_broad;

	//split the thread when we put the blub on it
	pthread_create(thread_handle, NULL, fill_empty, (void *) pp2);

	//keep the initial thread for the condition that we do not put the blub
	pp->pos = next_empty(cur, broad);
	fill_empty((void*) pp);
	pthread_join(*thread_handle, NULL);
	free(thread_handle);
	return NULL;
}

/*
*Copy the src broad to des
*/
void cp_board(int* des, const int* src) {
	int i;
	for(i = 0; i < m*n; i++)
		des[i] = src[i];
}

int final_check(int* broad){
	int i;
	for(i = 0; i < m*n; i++)
		if(broad[i] < COVER)
			return 0;

	return 1;
}

int put_lightbulb(int row, int col, int* broad) {

	int r, c;

	if(row < 0 || row >= m
		|| col < 0 || col >= n)
		return 0;
	//judge can put?
	if(broad[row * n + col] != EMPTY && broad[row*n+col] != LIGHTBULB)
		return 0;
	else if(broad[row*n+col] == LIGHTBULB)
		return 1;

	broad[row * n + col] = LIGHTBULB;
	// illuminate four directions
	for(r = row -1 , c = col; r >= 0 && broad[r*n+c] < BLACKBLOCK; r--)
		broad[r*n + c] = COVER;
	for(r = row, c = col - 1; c >= 0 && broad[r* n+c] < BLACKBLOCK; c--)
		broad[r*n + c] = COVER;
	for(r = row + 1, c = col; r < m && broad[r*n + c] < BLACKBLOCK; r++)
		broad[r*n + c] = COVER;
	for(r = row, c = col +1; c < n && broad[r*n + c] < BLACKBLOCK; c++)
		broad[r*n + c] = COVER;
	return 1;

}

int put_redx(int row, int col, int* broad) {
	if((row >= 0 && row < m)
		&& (col >= 0 && col < n)){
		if(broad[row*n + col] == LIGHTBULB)
			return 0;

		if(broad[row*n + col] == EMPTY) {
			broad[row*n + col] = REDX;
		}
	}
	return 1;
}

int next_empty(int cur, int* broad){

	int i;
	for(i = cur+1; i < m *n; i++)
		if(broad[i] == EMPTY)
			return i;
	return -1;
}

vector<vector<int> > solveAkari(vector<vector<int> > & g)
{
    // 请在此函数内返回最后求得的结果
    int len = 0;
    m = (int)g.size();
    n = (int)g[0].size();
    //cp g ans
    vector<vector<int> > ans(m);
    for(int i=0; i<m; ++i){
        ans[i].resize(n);
    }
    global_checkerboard = (int*) malloc(sizeof(int) * m * n);
    //change default array
    for(int r = 0; r < m; r++) {
        for(int c = 0; c < n; c++){
            ans[r][c] = g[r][c];
            switch(g[r][c]){
            case -2:
                global_checkerboard[r*n + c] = 0;
                break;
            case -1:
                global_checkerboard[r*n + c] = 3;
                break;
            case 0:
                global_checkerboard[r*n + c] = 4;
                break;
            case 1:
                global_checkerboard[r*n + c] = 5;
                break;
            case 2:
                global_checkerboard[r*n + c] = 6;
                break;
            case 3:
                global_checkerboard[r*n + c] = 7;
                break;
            case 4:
                global_checkerboard[r*n + c] = 8;
                break;
            default:
                global_checkerboard[r*n + c] = 0;
                break;
            }
        }
    }
    BlackblockList barriers = create_blackblock_list(global_checkerboard);

	blackblockParam* params = (blackblockParam*) malloc(sizeof(blackblockParam));
	int* cp_broad =(int*) malloc(sizeof(int)*m*n);
	cp_board(cp_broad, global_checkerboard);
	params->boardp = cp_broad;
	params->blackblock = barriers;

	solve_akari_puzzle((void*) params);

	if(handle) {
        for(int r = 0; r < m; r++) {
            for(int c = 0; c < n; c++){
                if(global_checkerboard[r*n + c] == 9){
                    ans[r][c] = 5;//replace LIGHTBULB
                }
            }
        }
	}else
		printf("No solution!\n");

	free(global_checkerboard);

	BlackblockList p;
	for(p = barriers; p;) {
		barriers = barriers->next;
		free(p);
		p = barriers;
	}

	return ans;

}

}
