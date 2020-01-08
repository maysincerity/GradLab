# include <bits/stdc++.h>
# include "akari.h"
using namespace std;

namespace aka{
//请在命名空间内编写代码，否则后果自负
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>

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

int solve_puzzle(Barrier_list, Elem_type*);
int put_blub(int, int, Elem_type*);
int put_dot(int, int, Elem_type*);

int handle_empty(int, Elem_type*);
int next_empty(int, Elem_type*);

vector<vector<int> > solveAkari(vector<vector<int> > & g)
{
    // 请在此函数内返回最后求得的结果
    //get n & m
    int len = 0;
    m = (int)g.size();
    n = (int)g[0].size();
    //m = len/n;
    vector<vector<int> > ans(m);
    for(int i=0; i<m; ++i){
        ans[i].resize(n);
    }
    g_broad = (Elem_type*) malloc(sizeof(Elem_type) * m * n);
    // read_broad(fp, g_broad);// input the chess board
    for(int r = 0; r < m; r++) {
		for(int c = 0; c < n; c++){
			//g_broad[r*n + c] = g[r][c];
            ans[r][c] = g[r][c];
            switch(g[r][c]){
    		case -2:
    			g_broad[r*n + c] = 0;
    			break;
            case -1:
    			g_broad[r*n + c] = 3;
    			break;
            case 0:
    			g_broad[r*n + c] = 4;
    			break;
            case 1:
    			g_broad[r*n + c] = 5;
    			break;
            case 2:
    			g_broad[r*n + c] = 6;
    			break;
            case 3:
    			g_broad[r*n + c] = 7;
    			break;
            case 4:
    			g_broad[r*n + c] = 8;
    			break;
            default:
    			g_broad[r*n + c] = 0;
    			break;
            }
        }
	}


	//print_broad(g_broad);// print the chess board
    Barrier_list barriers = create_barrier_list(g_broad);//create list of barrier 0,1,2,3,4
    // start = clock();
	int solved = solve_puzzle(barriers, g_broad);
	// finish = clock();
	// diff = (finish - start) / CLOCKS_PER_SEC;
	if(solved) {
	//	print_broad(g_broad);
	//	printf("Time duration: %lf s \n", diff);
        for(int r = 0; r < m; r++) {
            for(int c = 0; c < n; c++){
                //g_broad[r*n + c] = g[r][c];
                //ans[r][c] = g[r][c];
                if(g_broad[r*n + c] == 9){
                    ans[r][c] = 5;
                }
            }
        }
	}else{
	//	printf("no solution!");
    }

	free(g_broad);
//	fclose(fp);

	Barrier_list p;
	for(p = barriers; p;) {
		barriers = barriers->next;
		free(p);
		p = barriers;
	}

    int ntemp = ans.size(), mtemp = ans[0].size();
    for (int i = 0; i < ntemp; ++i) {
        for (int j = 0; j < mtemp; ++j) {
            printf("%4d", ans[i][j]);
        }
        printf("\n");
    }

    return ans;
	// return 0;


}

// int main() {
//
// 	FILE *fp;
// 	double start, finish, diff;
// 	if((fp = fopen("test1.txt", "r")) == NULL)
// 		return 1;
// 	read_dimension(fp, &m, &n);// read row and column
// 	g_broad = (Elem_type*) malloc(sizeof(Elem_type) * m * n);
// 	//read_broad(fp, g_broad);// input the chess board
//
// 	print_broad(g_broad);// print the chess board
// 	Barrier_list barriers = create_barrier_list(g_broad);//create list of barrier 0,1,2,3,4
//
// 	start = clock();
// 	int solved = solve_puzzle(barriers, g_broad);
// 	finish = clock();
// 	diff = (finish - start) / CLOCKS_PER_SEC;
// 	if(solved) {
// 		print_broad(g_broad);
// 		printf("Time duration: %lf s \n", diff);
// 	}else
// 		printf("no solution!");
//
// 	free(g_broad);
// 	fclose(fp);
//
// 	Barrier_list p;
// 	for(p = barriers; p;) {
// 		barriers = barriers->next;
// 		free(p);
// 		p = barriers;
// 	}
// 	return 0;
//
// }

// void read_dimension(FILE* fp, int* row, int* col) {
// 	printf("Please input the row & column:\n");
// 	fscanf(fp, "%d %d", row, col);
// }

// void read_broad(FILE* fp, Elem_type* broad) {
// 	printf("Please input the broad:\n");
// 	int i;
// 	for(i = 0; i < m*n; i++)
// 		fscanf(fp, "%d", &broad[i]);
// }

// void print_broad(Elem_type* broad) {
// 	int r, c;
// 	printf("============================\n");
// 	for(r = 0; r < m; r++) {
// 		for(c = 0; c < n; c++)
// 			printf("%d ", broad[r*n + c]);
// 		printf("\n");
// 	}
// }

Barrier_list create_barrier_list(Elem_type* broad) {

	int i;
	Barrier_list header, p;
	header = p = NULL;
	for(i = 0; i < m*n; i++){
		if(broad[i] == BARRIER_0){
			Barrier_list q = (Barrier_list)malloc(sizeof(Num_barrier));
			q->row = i / n;
			q->col = i % n;
			q->number = 0;//means q's number is 0(black block)
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}

	for(i = 0; i < m*n; i++){
		if(broad[i] == BARRIER_4){
			Barrier_list q = (Barrier_list)malloc(sizeof(Num_barrier));
			q->row = i / n;
			q->col = i % n;
			q->number = 4;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	for(i = 0; i < m*n; i++){
		if(broad[i] == BARRIER_3){
			Barrier_list q = (Barrier_list)malloc(sizeof(Num_barrier));
			q->row = i / n;
			q->col = i % n;
			q->number = 3;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	for(i = 0; i < m*n; i++){
		if(broad[i] == BARRIER_1){
			Barrier_list q = (Barrier_list)malloc(sizeof(Num_barrier));
			q->row = i / n;
			q->col = i % n;
			q->number = 1;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	for(i = 0; i < m*n; i++){
		if(broad[i] == BARRIER_2){
			Barrier_list q = (Barrier_list)malloc(sizeof(Num_barrier));
			q->row = i / n;
			q->col = i % n;
			q->number = 2;
			(header == NULL) ? (header = p = q) : (p->next = q, p=p->next);
		}
	}
	if(p != NULL) p->next = NULL;
	return header;
}
int solve_puzzle(Num_barrier* barrier, Elem_type* broad) {

	if(NULL != barrier) {//first phase
		Num_barrier* p;
		Elem_type* cp_broad;
		int r, c;
		int handle = 0;

		cp_broad = (Elem_type*)malloc(sizeof(Elem_type) * m * n);
		copy_broad(cp_broad, broad);

		p = barrier;
		r = p->row;
		c = p->col;
		// print_broad(cp_broad);
		// printf("number:%d  r:%d   c:%d\n", p->number, r, c);
		switch(p->number){
		case 4:
			if(put_blub(r-1, c, cp_broad)
				&&put_blub(r, c-1, cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_blub(r, c+1, cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			break;
		case 3:
			if(put_blub(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad)) {

				handle = solve_puzzle(p->next, cp_broad);
			}
			//restore the state of broad
			copy_broad(cp_broad, broad);
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_blub(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			//
			copy_broad(cp_broad, broad);
			if(!handle && put_blub(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_blub(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			//
			copy_broad(cp_broad, broad);
			if(!handle && put_blub(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_blub(r, c+1,cp_broad))
				handle = solve_puzzle(p->next,cp_broad);
			break;
		case 2:
			if(put_blub(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_blub(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			if(!handle && put_blub(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_blub(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad,broad);
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_blub(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			if(!handle && put_blub(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			break;
		case 1 :
			//printf("r : %d\n", r);
			//printf("1\n");
			if(put_blub(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			//printf("2\n");
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			//printf("3\n");
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			//printf("4\n");
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_blub(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			break;
		case 0:
			if(put_dot(r-1, c,cp_broad)
				&& put_dot(r, c-1, cp_broad)
				&& put_dot(r+1, c, cp_broad)
				&& put_dot(r, c+1, cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			break;
		default:
			break;
		}
		// print_broad(cp_broad);
		free(cp_broad);
		cp_broad = NULL;
		return handle;
	} else {//second phase
		return handle_empty(next_empty(-1, broad), broad);
	}
}

int handle_empty(int cur, Elem_type* broad) {
	//handle left blank spaces
	Elem_type* cp_broad;
	int handle = 0;

	if(check_complete(broad)){
		copy_broad(g_broad, broad);
		return 1;
	}
	if(cur == -1)
		return 0;

	cp_broad = (Elem_type*)malloc(sizeof(Elem_type) * m * n);
	copy_broad(cp_broad, broad);

	put_blub(cur / n, cur % n, cp_broad);
	handle = handle_empty(next_empty(cur, cp_broad), cp_broad);
	if(!handle){
		//retore the previous state of the broad
		copy_broad(cp_broad, broad);
		handle =  handle_empty(next_empty(cur, cp_broad), cp_broad);
	}

	free(cp_broad);
	return handle;
}
/*
*Copy the src broad to des
*/
void copy_broad(Elem_type* des, const Elem_type* src) {
	int i;
	for(i = 0; i < m*n; i++)
		des[i] = src[i];
}

int check_complete(Elem_type* broad){
	int i;
	for(i = 0; i < m*n; i++)
		if(broad[i] < COVER)
			return 0;

	return 1;
}

int put_blub(int row, int col, Elem_type* broad) {

	int r, c;
	//starts with zero
	if(row < 0 || row >= m
		|| col < 0 || col >= n)
		return 0;
	//judge can put?
	if(broad[row * n + col] != EMPTY && broad[row*n+col] != BLUB)
		return 0;
	else if(broad[row*n+col] == BLUB)
		return 1;

	broad[row * n + col] = BLUB;
	// illuminate four directions
	for(r = row -1 , c = col; r >= 0 && broad[r*n+c] < BARRIER; r--)
		broad[r*n + c] = COVER;
	for(r = row, c = col - 1; c >= 0 && broad[r* n+c] < BARRIER; c--)
		broad[r*n + c] = COVER;
	for(r = row + 1, c = col; r < m && broad[r*n + c] < BARRIER; r++)
		broad[r*n + c] = COVER;
	for(r = row, c = col +1; c < n && broad[r*n + c] < BARRIER; c++)
		broad[r*n + c] = COVER;
	return 1;

}

int put_dot(int row, int col, Elem_type* broad) {
	if((row >= 0 && row < m)
		&& (col >= 0 && col < n)){
		if(broad[row*n + col] == BLUB)
			return 0;

		if(broad[row*n + col] == EMPTY) {
			broad[row*n + col] = DOT;
		}
	}
	return 1;
}

int next_empty(int cur, Elem_type* broad){
	int i;
	for(i = cur+1; i < m *n; i++)
		if(broad[i] == EMPTY)
			return i;

	return -1;
}

}
