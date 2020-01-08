# include <bits/stdc++.h>
#include<pthread.h>
# include "akari.h"
using namespace std;

namespace aka{
//请在命名空间内编写代码，否则后果自负
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define EMPTY 0
#define REDX 1
#define YELLOWLIGHT 2
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

BlackblockList create_blackblock_list(int* board) {
	//create_blackblock_list
	int i;
	BlackblockList headp, p;
	headp = p = NULL;
	for(i = 0; i < m*n; i++){
		if(board[i] == BLACKBLOCK0){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 0;
			//(headp == NULL) ? (headp = p = q) : (p->next = q, p=p->next);
			if(headp == NULL){
				headp = p = q;
			}else{
				p->next = q;
				p = p->next;
			}
		}
	}

	for(i = 0; i < m*n; i++){
		if(board[i] == BLACKBLOCK4){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 4;
			// (headp == NULL) ? (headp = p = q) : (p->next = q, p=p->next);
			if(headp == NULL){
				headp = p = q;
			}else{
				p->next = q;
				p = p->next;
			}
		}
	}
	for(i = 0; i < m*n; i++){
		if(board[i] == BLACKBLOCK3){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 3;
			// (headp == NULL) ? (headp = p = q) : (p->next = q, p=p->next);
			if(headp == NULL){
				headp = p = q;
			}else{
				p->next = q;
				p = p->next;
			}
		}
	}
	for(i = 0; i < m*n; i++){
		if(board[i] == BLACKBLOCK1){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 1;
			// (headp == NULL) ? (headp = p = q) : (p->next = q, p=p->next);
			if(headp == NULL){
				headp = p = q;
			}else{
				p->next = q;
				p = p->next;
			}
		}
	}
	for(i = 0; i < m*n; i++){
		if(board[i] == BLACKBLOCK2){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 2;
			// (headp == NULL) ? (headp = p = q) : (p->next = q, p=p->next);
			if(headp == NULL){
				headp = p = q;
			}else{
				p->next = q;
				p = p->next;
			}
		}
	}
	if(p != NULL) p->next = NULL;
	return headp;
}


pthread_t* mupltiplesolve(int flag, int* board, BlackblockList blackblock) {
	int* copied_board = (int*) malloc(sizeof(int) * m*n);
	cp_board(copied_board,board);
	int i, success = 0;
	int r = blackblock->row;
	int c = blackblock->col;
	for(i = 0; i < 4; i++) {
		int direction  = (1<<i);
		int type = (flag & direction);
		switch(direction) {
		case D_UP:
			success = (type != 0) ? put_lightbulb(r-1, c, copied_board):put_redx(r-1, c, copied_board);
			break;
		case D_RIGHT:
			success = (type != 0) ? put_lightbulb(r, c-1, copied_board):put_redx(r, c-1, copied_board);
			break;
		case D_DOWN:
			success = (type != 0) ? put_lightbulb(r+1, c, copied_board):put_redx(r+1, c, copied_board);
			break;
		case D_LEFT:
			success = (type != 0) ? put_lightbulb(r, c+1, copied_board):put_redx(r, c+1, copied_board);
			break;
		}
		if(!success)
			break;
	}

	if(!success) {
		free(copied_board);
		return NULL;
	}
	else {
		//if success
		if(handle) {
			//find a solution
			free(copied_board);
			return NULL;
		}
		blackblockParam* bbpara = (blackblockParam*) malloc(sizeof(blackblockParam));
		pthread_t* nthread = (pthread_t*) malloc(sizeof(pthread_t));
		bbpara->blackblock = blackblock->next;
		bbpara->boardp = copied_board;
		pthread_create(nthread, NULL, solve_akari_puzzle, (void*)bbpara);
		return nthread;
	}
}

void* solve_akari_puzzle(void* bbpara) {
	blackblockParam* params = (blackblockParam*)bbpara;
	BlackblockList blackblock = params->blackblock;
	int* board = params->boardp;

	if(handle) {
		//find a solution
		free(board);
		free(params);
		return NULL;
	}

	if(NULL != blackblock) {
        //first put lightbulbs aroud blackblocks
		int i;
		pthread_t** threads_for_akari;
		threads_for_akari = (pthread_t**) calloc(6, sizeof(pthread_t*));
		switch(blackblock->number){
		case 4:
			threads_for_akari[0] = mupltiplesolve(D_UP|D_RIGHT|D_DOWN|D_LEFT, board, blackblock);
			break;
		case 3:
			threads_for_akari[0] = mupltiplesolve(D_UP|D_DOWN|D_RIGHT, board, blackblock);
			threads_for_akari[1] = mupltiplesolve(D_DOWN|D_LEFT|D_RIGHT, board, blackblock);
			threads_for_akari[2] = mupltiplesolve(D_LEFT|D_UP|D_RIGHT, board, blackblock);
			threads_for_akari[3] = mupltiplesolve(D_LEFT|D_UP|D_DOWN, board, blackblock);
			break;
		case 2:
			threads_for_akari[0] = mupltiplesolve(D_LEFT|D_UP, board, blackblock);
			threads_for_akari[1] = mupltiplesolve(D_LEFT|D_DOWN, board, blackblock);
			threads_for_akari[2] = mupltiplesolve(D_LEFT|D_RIGHT, board, blackblock);
			threads_for_akari[3] = mupltiplesolve(D_UP|D_DOWN, board, blackblock);
			threads_for_akari[4] = mupltiplesolve(D_UP|D_RIGHT, board, blackblock);
			threads_for_akari[5] = mupltiplesolve(D_DOWN|D_RIGHT, board, blackblock);
			break;
		case 1:
			threads_for_akari[0] = mupltiplesolve(D_LEFT, board, blackblock);
			threads_for_akari[1] = mupltiplesolve(D_UP, board, blackblock);
			threads_for_akari[2] = mupltiplesolve(D_DOWN, board, blackblock);
			threads_for_akari[3] = mupltiplesolve(D_RIGHT, board, blackblock);
			break;
		case 0:
			threads_for_akari[0] = mupltiplesolve(0, board, blackblock);
			break;
		default:
			break;
		}
		for(i = 0; i < 6;i++)
			if(threads_for_akari[i] != NULL) {
				pthread_join(*threads_for_akari[i], NULL);

			}

		for(i = 0; i < 6;i++)
			if(threads_for_akari[i] != NULL)
				free(threads_for_akari[i]);

		free(threads_for_akari);
		free(board);
		free(params);
		return NULL;
	} else {
		//fill left empty space
		fillemptyParam* fillemptypara = (fillemptyParam*) malloc(sizeof(fillemptyParam));
		fillemptypara->pos = next_empty(-1, board);
		fillemptypara->boardp = board;
		fill_empty((void*) fillemptypara);
		return NULL;
	}
}

void *fill_empty(void* params) {

	fillemptyParam* fillemptypara = (fillemptyParam*) params;
	fillemptyParam* fillemptypara2;
	int* board, *copied_board;
	pthread_t* thread_handle;
	int empty_pos;
	board = (int*)fillemptypara->boardp;
	empty_pos = fillemptypara->pos;
	if(final_check(board)){
		pthread_mutex_lock(&lock);
		if(!handle) {
			cp_board(global_checkerboard, board);
			handle= 1;
		}
		pthread_mutex_unlock(&lock);
		free(fillemptypara->boardp);
		free(fillemptypara);
		return NULL;
	}
	if(empty_pos == -1) {
		free(fillemptypara->boardp);
		free(fillemptypara);
		return NULL;
	}

	if(handle) {
		//find a solution
		free(fillemptypara->boardp);
		free(fillemptypara);
		return NULL;
	}

	thread_handle = (pthread_t*) malloc(sizeof(pthread_t));
	copied_board = (int*)malloc(sizeof(int) * m * n);
	cp_board(copied_board, board);
	put_lightbulb(empty_pos / n, empty_pos % n, copied_board);
	fillemptypara2 = (fillemptyParam*) malloc(sizeof(fillemptyParam));
	fillemptypara2->pos = next_empty(empty_pos, copied_board);
	fillemptypara2->boardp = copied_board;
	//create new thread
	pthread_create(thread_handle, NULL, fill_empty, (void *) fillemptypara2);
	//fill next empty pos
	fillemptypara->pos = next_empty(empty_pos, board);
	fill_empty((void*) fillemptypara);
	pthread_join(*thread_handle, NULL);
	free(thread_handle);
	return NULL;
}


int final_check(int* board){
	int i;
	for(i = 0; i < m*n; i++)
		if(board[i] < YELLOWLIGHT)
			return 0;
	return 1;
}

int put_lightbulb(int row, int col, int* board) {

	int r, c;
	//starts with zero
	if(row < 0 || row >= m || col < 0 || col >= n)
		return 0;
	//judge
	if(board[row*n + col] != EMPTY && board[row*n+col] != LIGHTBULB)
		return 0;
	else if(board[row*n+col] == LIGHTBULB)
		return 1;
	board[row*n + col] = LIGHTBULB;
	//fill light
	for(r = row -1 , c = col; r >= 0 && board[r*n + c] < BLACKBLOCK; r--)
		board[r*n + c] = YELLOWLIGHT;
    for(r = row + 1, c = col; r < m && board[r*n + c] < BLACKBLOCK; r++)
		board[r*n + c] = YELLOWLIGHT;
	for(r = row, c = col - 1; c >= 0 && board[r*n + c] < BLACKBLOCK; c--)
		board[r*n + c] = YELLOWLIGHT;
	for(r = row, c = col +1; c < n && board[r*n + c] < BLACKBLOCK; c++)
		board[r*n + c] = YELLOWLIGHT;
	return 1;

}

int put_redx(int row, int col, int* board) {
	if((row >= 0 && row < m) && (col >= 0 && col < n)){
		if(board[row*n + col] == LIGHTBULB)
			return 0;
		if(board[row*n + col] == EMPTY) {
			board[row*n + col] = REDX;
		}
	}
	return 1;
}


void cp_board(int* dst, const int* src) {
	int i;
	for(i = 0; i < m*n; i++)
		dst[i] = src[i];
}


int next_empty(int cur_pos, int* board){
	int i;
	for(i = cur_pos+1; i < m *n; i++)
		if(board[i] == EMPTY)
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
	int* copied_board =(int*) malloc(sizeof(int)*m*n);
	cp_board(copied_board, global_checkerboard);
	params->boardp = copied_board;
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
