#include"pth_akari_puzzle.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int handle = 0;

typedef struct {
	Elem_type* p_broad;
	Barrier_list barrier;
} Params;

typedef struct {
	Elem_type* p_broad;
	int pos;
} Params2;

int main() {

	FILE *fp;
	double start, finish, diff;
	if((fp = fopen("test4.txt", "r")) == NULL)
		return 1;
	read_dimension(fp, &m, &n);
	g_broad = (Elem_type*) malloc(sizeof(Elem_type) * m * n);
	read_broad(fp, g_broad);
	print_broad(g_broad);
	Barrier_list barriers = create_barrier_list(g_broad);

	Params* params = (Params*) malloc(sizeof(Params));
	Elem_type* cp_broad =(Elem_type*) malloc(sizeof(Elem_type)*m*n);
	copy_broad(cp_broad, g_broad);
	params->p_broad = cp_broad;
	params->barrier = barriers;
	start = clock();
	solve_puzzle((void*) params);
	finish = clock();
	diff = (finish - start) / CLOCKS_PER_SEC;
	if(handle) {
		print_broad(g_broad);
		printf("Time duration: %lf s\n", diff);
	}else
		printf("No solution!\n");

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


pthread_t* split(int flag, Elem_type* broad, Barrier_list barrier) {
	Elem_type* cp_broad = (Elem_type*) malloc(sizeof(Elem_type) * m*n);
	copy_broad(cp_broad,broad);
	int i, success = 0;
	int r = barrier->row, c = barrier->col;
	for(i = 0; i < 4; i++) {
		int direction  = (1<<i);
		int type = (flag & direction);
		switch(direction) {
		case NORTH:
			success = (type != 0) ? put_blub(r-1, c, cp_broad):put_dot(r-1, c, cp_broad);
			break;
		case EAST:
			success = (type != 0) ? put_blub(r, c-1, cp_broad):put_dot(r, c-1, cp_broad);
			break;
		case SOUTH:
			success = (type != 0) ? put_blub(r+1, c, cp_broad):put_dot(r+1, c, cp_broad);
			break;
		case WEST:
			success = (type != 0) ? put_blub(r, c+1, cp_broad):put_dot(r, c+1, cp_broad);
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
		Params* p_params = (Params*) malloc(sizeof(Params));
		pthread_t* p_handle = (pthread_t*) malloc(sizeof(pthread_t));
		p_params->barrier = barrier->next;
		p_params->p_broad = cp_broad;
		pthread_create(p_handle, NULL, solve_puzzle, (void*)p_params);//split
		return p_handle;
	}
}

void* solve_puzzle(void* p_params) {

	//printf("function called \n");
	Params* params = (Params*)p_params;
	Barrier_list barrier = params->barrier;
	Elem_type* broad = params->p_broad;

	if(handle) {
		free(broad);
		free(params);
		return NULL;
	}

	if(NULL != barrier) {//first phase

		int i;
		pthread_t** thread_handles;

		thread_handles = (pthread_t**) calloc(6, sizeof(pthread_t*));//max thread num is 6
		switch(barrier->number){
		case 4:
			thread_handles[0] = split(NORTH|EAST|SOUTH|WEST, broad, barrier);
			break;
		case 3:
			thread_handles[0] = split(NORTH|EAST|SOUTH, broad, barrier);
			thread_handles[1] = split(NORTH|EAST|WEST, broad, barrier);
			thread_handles[2] = split(NORTH|SOUTH|WEST, broad, barrier);
			thread_handles[3] = split(EAST|SOUTH|WEST, broad, barrier);
			break;
		case 2:
			thread_handles[0] = split(NORTH|EAST, broad, barrier);
			thread_handles[1] = split(NORTH|SOUTH, broad, barrier);
			thread_handles[2] = split(NORTH|WEST, broad, barrier);
			thread_handles[3] = split(EAST|SOUTH, broad, barrier);
			thread_handles[4] = split(EAST|WEST, broad, barrier);
			thread_handles[5] = split(SOUTH|WEST, broad, barrier);
			break;
		case 1:
			thread_handles[0] = split(NORTH, broad, barrier);
			thread_handles[1] = split(EAST, broad, barrier);
			thread_handles[2] = split(SOUTH, broad, barrier);
			thread_handles[3] = split(WEST, broad, barrier);
			break;
		case 0:
			thread_handles[0] = split(0, broad, barrier);
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
		Params2* pp = (Params2*) malloc(sizeof(Params2));
		pp->pos = next_empty(-1, broad);
		pp->p_broad = broad;
		handle_empty((void*) pp);
		return NULL;
	}
}

void *handle_empty(void* params) {

	Params2* pp = (Params2*) params , *pp2;//pp2 as the params of the splited thread
	Elem_type* broad, *cp_broad;
	pthread_t* thread_handle;
	int cur;

	broad = (Elem_type*)pp->p_broad;
	cur = pp->pos;

	if(check_complete(broad)){
		pthread_mutex_lock(&lock);
		if(!handle) {
			copy_broad(g_broad, broad);
			handle= 1;
		}
		pthread_mutex_unlock(&lock);
		free(pp->p_broad);
		free(pp);
		return NULL;
	}
	if(cur == -1) {
		free(pp->p_broad);
		free(pp);
		return NULL;
	}

	//alway check the result before we split another thread in case we do the unuseful work
	if(handle) {
		free(pp->p_broad);
		free(pp);
		return NULL;
	}

	thread_handle = (pthread_t*) malloc(sizeof(pthread_t));
	cp_broad = (Elem_type*)malloc(sizeof(Elem_type) * m * n);
	copy_broad(cp_broad, broad);
	put_blub(cur / n, cur % n, cp_broad);

	pp2 = (Params2*) malloc(sizeof(Params2));
	pp2->pos = next_empty(cur, cp_broad);
	pp2->p_broad = cp_broad;

	//split the thread when we put the blub on it
	pthread_create(thread_handle, NULL, handle_empty, (void *) pp2);

	//keep the initial thread for the condition that we do not put the blub
	pp->pos = next_empty(cur, broad);
	handle_empty((void*) pp);
	pthread_join(*thread_handle, NULL);
	free(thread_handle);
	return NULL;
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
