# include <bits/stdc++.h>
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

int solve_akari_puzzle(BlackblockList, int*);
int put_lightbulb(int, int, int*);
int put_redx(int, int, int*);

int fill_empty(int, int*);
int next_empty(int, int*);

vector<vector<int> > solveAkari(vector<vector<int> > & g)
{
    // 请在此函数内返回最后求得的结果
    //get n & m
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

    //create list of blackblock 0,1,2,3,4
    BlackblockList blackblocks = create_blackblock_list(global_checkerboard);
	int solved = solve_akari_puzzle(blackblocks, global_checkerboard);
	if(solved) {
        //solved
        for(int r = 0; r < m; r++) {
            for(int c = 0; c < n; c++){
                if(global_checkerboard[r*n + c] == 9){
                    ans[r][c] = 5;//replace LIGHTBULB
                }
            }
        }
	}else{
        //no solution
    }

	free(global_checkerboard);

    //free blackblocks
	BlackblockList p;
	for(p = blackblocks; p ;) {
		blackblocks = blackblocks->next;
		free(p);
		p = blackblocks;
	}

    //return vector<vector<int> >;
    return ans;
}

BlackblockList create_blackblock_list(int* board) {
	int i;
	BlackblockList header, p;
	header = p = NULL;
    //first add block0 because it only has one situation
	for(i = 0; i < m * n; ++i){
		if(board[i] == BLACKBLOCK0){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 0;//q's label is 0
			(header == NULL) ? (header = p = q) : (p->next = q, p = p->next);
		}
	}

    //second add block4 because it only has one situation
	for(i = 0; i < m * n; ++i){
		if(board[i] == BLACKBLOCK4){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 4;
			(header == NULL) ? (header = p = q) : (p->next = q, p = p->next);
		}
	}

    //third add block3 because it has four situation & has more lightbulbs
	for(i = 0; i < m * n; ++i){
		if(board[i] == BLACKBLOCK3){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 3;
			(header == NULL) ? (header = p = q) : (p->next = q, p = p->next);
		}
	}

    //fourth add block1 because it has four situation
	for(i = 0; i < m * n; ++i){
		if(board[i] == BLACKBLOCK1){
			BlackblockList q = (BlackblockList)malloc(sizeof(Blackblocknode));
			q->row = i / n;
			q->col = i % n;
			q->number = 1;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}

    //fifth add block2 because it has six situation
	for(i = 0; i < m * n; ++i){
		if(board[i] == BLACKBLOCK2){
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


int solve_akari_puzzle(Blackblocknode* barrier, int* board) {
    //solve akari
	if(NULL != barrier) {
        //first put lightbulbs aroud blackblocks
		int r, c;
		int handle = 0;
        Blackblocknode* p;
		int* copied_board;
		copied_board = (int*)malloc(sizeof(int) * m * n);
		cp_board(copied_board, board);

		p = barrier;
		r = p->row;
		c = p->col;
		switch(p->number){
		case 4:
			if(put_lightbulb(r-1, c, copied_board)&&put_lightbulb(r, c-1, copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_lightbulb(r, c+1, copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			break;
		case 3:
			if(put_lightbulb(r-1, c, copied_board)&&put_lightbulb(r, c-1,copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_redx(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			//restore the state of board
			cp_board(copied_board, board);
			if(!handle && put_redx(r-1, c, copied_board)&&put_lightbulb(r, c-1,copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_lightbulb(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			//
			cp_board(copied_board, board);
			if(!handle && put_lightbulb(r-1, c, copied_board)&&put_redx(r, c-1,copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_lightbulb(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			//
			cp_board(copied_board, board);
			if(!handle && put_lightbulb(r-1, c, copied_board)&&put_lightbulb(r, c-1,copied_board)
				&&put_redx(r+1, c, copied_board)&&put_lightbulb(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next,copied_board);
			break;
		case 2:
			if(put_lightbulb(r-1, c, copied_board)&&put_lightbulb(r, c-1,copied_board)
				&&put_redx(r+1, c, copied_board)&&put_redx(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board, board);
			if(!handle && put_redx(r-1, c, copied_board)&&put_lightbulb(r, c-1,copied_board)
				&&put_redx(r+1, c, copied_board)&&put_lightbulb(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board, board);
			if(!handle && put_lightbulb(r-1, c, copied_board)&&put_redx(r, c-1,copied_board)
				&&put_redx(r+1, c, copied_board)&&put_lightbulb(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board,board);
			if(!handle && put_redx(r-1, c, copied_board)&&put_redx(r, c-1,copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_lightbulb(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board, board);
			if(!handle && put_redx(r-1, c, copied_board)&&put_lightbulb(r, c-1,copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_redx(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board, board);
			if(!handle && put_lightbulb(r-1, c, copied_board)&&put_redx(r, c-1,copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_redx(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			break;
		case 1 :
			//printf("r : %d\n", r);
			//printf("1\n");
			if(put_lightbulb(r-1, c, copied_board)&&put_redx(r, c-1,copied_board)
				&&put_redx(r+1, c, copied_board)&&put_redx(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board, board);
			//printf("2\n");
			if(!handle && put_redx(r-1, c, copied_board)&&put_lightbulb(r, c-1,copied_board)
				&&put_redx(r+1, c, copied_board)&&put_redx(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board, board);
			//printf("3\n");
			if(!handle && put_redx(r-1, c, copied_board)&&put_redx(r, c-1,copied_board)
				&&put_lightbulb(r+1, c, copied_board)&&put_redx(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			cp_board(copied_board, board);
			//printf("4\n");
			if(!handle && put_redx(r-1, c, copied_board)&&put_redx(r, c-1,copied_board)
				&&put_redx(r+1, c, copied_board)&&put_lightbulb(r, c+1,copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			break;
		case 0:
			if(put_redx(r-1, c,copied_board)&& put_redx(r, c-1, copied_board)
				&& put_redx(r+1, c, copied_board)&& put_redx(r, c+1, copied_board))
				handle = solve_akari_puzzle(p->next, copied_board);
			break;
		default:
			break;
		}
		// print_broad(copied_board);
		free(copied_board);
		copied_board = NULL;
		return handle;
	} else {
		return fill_empty(next_empty(-1, board), board);
	}
}

int fill_empty(int cur_pos, int* board) {
	//handle left blank spaces
	int* copied_board;
	int handle = 0;
	if(final_check(board)){
		cp_board(global_checkerboard, board);
		return 1;
	}
	if(cur_pos == -1)
		return 0;
	copied_board = (int*)malloc(sizeof(int) * m * n);
	cp_board(copied_board, board);
	put_lightbulb(cur_pos / n, cur_pos % n, copied_board);
	handle = fill_empty(next_empty(cur_pos, copied_board), copied_board);
	if(!handle){
		//retore
		cp_board(copied_board, board);
		handle =  fill_empty(next_empty(cur_pos, copied_board), copied_board);
	}
	free(copied_board);
	return handle;
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
	if(board[row * n + col] != EMPTY && board[row*n+col] != LIGHTBULB)
		return 0;
	else if(board[row*n+col] == LIGHTBULB)
		return 1;
	board[row * n + col] = LIGHTBULB;
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

}
