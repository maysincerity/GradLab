#include"akari_puzzle.h"

int main() {

	FILE *fp;
	double start, finish, diff;
	if((fp = fopen("akari_puzzle_easy1.txt", "r")) == NULL)
		return 1;
	read_dimension(fp, &m, &n);
	g_broad = (Elem_type*) malloc(sizeof(Elem_type) * m * n);
	read_broad(fp, g_broad);
	print_broad(g_broad);
	Barrier_list barriers = create_barrier_list(g_broad);

	start = clock();
	int solved = solve_puzzle(barriers, g_broad);
	finish = clock();
	diff = (finish - start) / CLOCKS_PER_SEC;
	if(solved) {
		print_broad(g_broad);
		printf("Time duration: %lf s \n", diff);
	}else
		printf("no solution!");

	free(g_broad);
	fclose(fp);

	Barrier_list p;
	for(p = barriers; p;) {
		barriers = barriers->next;
		free(p);
		p = barriers;
	}
	return 0;

}

void read_dimension(FILE* fp, int* row, int* col) {
	printf("Please input the row & column:\n");
	fscanf(fp, "%d %d", row, col);
}

void read_broad(FILE* fp, Elem_type* broad) {
	printf("Please input the broad:\n");
	int i;
	for(i = 0; i < m*n; i++)
		fscanf(fp, "%d", &broad[i]);
}

void print_broad(Elem_type* broad) {
	int r, c;
	printf("============================\n");
	for(r = 0; r < m; r++) {
		for(c = 0; c < n; c++)
			printf("%d ", broad[r*n + c]);
		printf("\n");
	}
}

Barrier_list create_barrier_list(Elem_type* broad) {

	int i;
	Barrier_list header, p;
	header = p = NULL;
	for(i = 0; i < m*n; i++){
		if(broad[i] == BARRIER_0){
			Barrier_list q = (Barrier_list)malloc(sizeof(Num_barrier));
			q->row = i / n;
			q->col = i % n;
			q->number = 0;
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
			if(put_blub(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_blub(r, c-1,cp_broad)
				&&put_dot(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
			if(!handle && put_dot(r-1, c, cp_broad)
				&&put_dot(r, c-1,cp_broad)
				&&put_blub(r+1, c, cp_broad)
				&&put_dot(r, c+1,cp_broad))
				handle = solve_puzzle(p->next, cp_broad);
			copy_broad(cp_broad, broad);
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
		free(cp_broad);
		cp_broad = NULL;
		return handle;
	} else {//second phase
		return handle_empty(next_empty(-1, broad), broad);
	}
}

int handle_empty(int cur, Elem_type* broad) {

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
