#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<stddef.h>
#include<stdbool.h>

#define LH 1      //��߸�
#define EH 0      //�ȸ�
#define RH -1     //�ұ߸�

typedef struct Info{
	int key;//�ؼ���
	char lastname[3];
	char firstname[5];
	char like[5];
	struct BATNode *friends;
	struct BATNode *fans;
	struct BATNode *following;
	struct BATNode *likes;
}Info;
typedef struct BATNode {
	Info data;
	int bflag;//������״̬
	int h;//�߶�
	struct BATNode *lchild, *rchild;
}BATNode, *BATree;

typedef void(*visit)(BATree T);
void L_Rotate(BATree *p);//��������
void R_Rotate(BATree *p);//��������
void LeftBalance(BATree *T);//�Խڵ�ʵ����ƽ��
void RightBalance(BATree *T);//�Խڵ�ʵ����ƽ��
void avl_init(BATree *T);//AVL����ʼ��
void avl_destroy(BATree *T);//AVL������
bool avl_insert(BATree *T, Info e, bool *taller);//AVL������
bool avl_delete(BATree *T, int key, bool *shorter);//AVL��ɾ��
void avl_traverse(BATree T);//AVL������
bool avl_search(BATree T, int key, BATree *Tsub);//AVL������
void set_init(BATree *T);//���ϳ�ʼ��
void set_destory(BATree *T);//��������
bool set_insert(BATree *T, Info e, bool *taller);//���ϲ���
bool set_remove(BATree *T, int key, bool *shorter);//����ɾ��
void set_intersection(BATree T, BATree T1, BATree *T0);//�󼯺Ͻ���
void set_union(BATree *T, BATree T1);//�󼯺ϲ���
void set_difference(BATree *T, BATree T1);//�󼯺ϲ
void set_size(BATree T1, BATree T);//�󼯺�Ԫ�ظ���
bool set_member(BATree T, int key, BATree *Tsub);//�ж��Ƿ�Ϊ�����г�Ա
bool set_subset(BATree Tsub, BATree T1);//�ж�һ�������Ƿ�����һ�����ϵ��Ӽ�
bool set_equal(BATree T, BATree T1);//�ж����������Ƿ����
bool load_data(BATree *T, char *filename);//������Ҫ����
bool save_data(BATree T, FILE *fp);//������Ҫ����
void input_key(Info *data);//����ؼ���
void output_key(BATree T);//����ؼ���
void create(BATree *T);//������
void height(BATree T, int i);//�����߶�
void gotoxy(int x, int y);//����λ��
void showtree(BATree T, int x, int y, visit fp);//��ʾ��ͼ
void InOrderTraverse(BATree T);//�������
void output_relation(BATree T1, BATree T);//�����ϵ
void output_likes(BATree T1, BATree L);//�������
bool input_pinfo(BATree *T, int p_size);//���������Ա��Ϣ
bool input_likes(BATree *L, int *l_size);//������ɰ�����
void input_relation(BATree *T, int p_size, int l_size);//�����ϵ��Ϣ
void input_information(BATree *T, int size, int max, int min);//���������Ϣ
void improve_sets(BATree T, BATree T0);//����������ɵĸ��ּ���
void improve_friends(BATree T, BATree T1, BATree T2);//���ƺ��Ѽ�
void improve_fans(BATree T, BATree T1, BATree T2);//���Ʒ�˿��
void improve_following(BATree T, BATree T1, BATree T2);//���ƹ�ע��
void indirect_friends_traverse(BATree T, BATree T1, BATree *T0);//���Ⱥ��ѱ���
void indirect_friends_insert(BATree T, BATree *T0);//���Ⱥ��Ѳ���
void set_init_fors(BATree *T);//��һ���˵ĺ��Ѽ�����˿������ע�������ü���
bool insert_name(BATree T);//Ϊһ���ڵ㸳��
bool init_friends(BATree T);//��ʼ�����Ѽ�
bool insert_friends(BATree T);//�������
bool remove_friends(BATree T);//ɾ������
bool search_friends(BATree T);//��������
bool traverse_friends(BATree T);//��������
bool init_fans(BATree T);//��ʼ����˿��
bool insert_fans(BATree T);//�����˿
bool remove_fans(BATree T);//ɾ����˿
bool search_fans(BATree T);//������˿
bool traverse_fans(BATree T);//������˿
bool init_following(BATree T);//��ʼ����ע��
bool insert_following(BATree T);//�����ע
bool remove_following(BATree T);//ɾ����ע
bool search_following(BATree T);//������ע
bool traverse_following(BATree T);//������ע
bool init_likes(BATree T,BATree L);//��ʼ�����ü�
bool insert_likes(BATree T, BATree L);//���밮��
bool remove_likes(BATree T, BATree L);//ɾ������
bool search_likes(BATree T, BATree L);//��������
bool traverse_likes(BATree T, BATree L);//��������
bool load(BATree *T);//���ظ�����Ϣ
bool traverse_relation(BATree T, FILE *fp);//����������֮��Ĺ�ϵ
bool save(BATree T);//���������Ϣ
bool save_relation(BATree T, FILE *fp, int i);//����������֮��Ĺ�ϵ

bool taller, shorter;
int x_move = 70, y_move= 25;
int count = 0;
BATree Tsub = NULL;

int main(){
	int i;
	int p_size = 200, l_size=10;
	BATree T = NULL;
	BATree L = NULL;
	srand((unsigned)time(NULL));
	system("color f0");
	do {
		printf("\n");
		printf("                                                           Menu\n");
		printf("                         ----------------------------------------------------------------------\n");
		printf("                                1. set init                       18.mutual following\n");
		printf("                                2. set destroy                    19.mutual likes\n");
		printf("                                3. set insert                     20.indirect friends\n");
		printf("                                4. set remove                     21.traverse\n");
		printf("                                5. set intersection               22.insert name\n");
		printf("                                6. set union                      23.options for friends\n");
		printf("                                7. set difference                 24.options for fans\n");
		printf("                                8. set member                     25.options for following\n");
		printf("                                9. set subset                     26.options for likes\n");
		printf("                                10.set equal                      27.load more\n");
		printf("                                11.set size                       28.save more\n");
		printf("                                12.create                         29.InitAVL\n");
		printf("                                13.showtree                       30.DestroyAVL\n");
		printf("                                14.save person                    31.SearchAVL\n");
		printf("                                15.load person                    32.InsertAVL\n");
		printf("                                16.create at ramdon               33.DeleteAVL\n");
		printf("                                17.personal information           34.TraverseAVL\n");
		printf("                                0. Exit\n");
		printf("                         ----------------------------------------------------------------------\n");
		printf("������������:");
		scanf("%d", &i);
		getchar();
		printf("\n");
		switch (i) {
		    case 0:
				break;
			case 1: {
				set_init(&T);
				break;
			}
			case 2: {
			    set_destory(&T);
			    printf("���ٳɹ�!\n");
			    break;
		    }
			case 3: {
			    Info e;
				input_key(&e);
			    if (set_insert(&T, e, &taller)) {
				    printf("����ɹ�!\n");
					height(T, 0);
			    }
			    else printf("����ʧ��!\n");
				break;
		    }
			case 4: {
				int key;
				printf("������ؼ���:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (set_remove(&T, key, &shorter)) {
					printf("ɾ���ɹ�!\n");
					height(T, 0);
				}
				else printf("ɾ��ʧ��!\n");
				break;
			}
			case 5: {
				char filename[20];
				BATree T1 = NULL;
				BATree T0 = NULL;
				printf("�������ļ���:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("���سɹ�!\n");
					set_intersection(T, T1, &T0);
					set_init(&T);
					T = T0;
					height(T, 0);
					printf("ȡ�����ɹ�!\n");
				}
				else printf("����ʧ��!\n");
				break;
			}
			case 6: {
				char filename[20];
				BATree T1 = NULL;
				printf("�������ļ���:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("���سɹ�!\n");
					set_union(&T, T1);
					height(T, 0);
					printf("ȡ�����ɹ�!\n");
				}
				else printf("����ʧ��!\n");
				break;
			}
			case 7: {
				char filename[20];
				BATree T1 = NULL;
				printf("�������ļ���:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("���سɹ�!\n");
					set_difference(&T, T1);
					height(T, 0);
					printf("ȡ��ɹ�!\n");
				}
				else printf("����ʧ��!\n");
				break;
			} 
			case 8: {
				int key;
				printf("������ؼ���:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (set_member(T, key, &Tsub)) printf("�ǳ�Ա!\n");
				else printf("���ǳ�Ա!\n");
				break;
			}
			case 9: {
				char filename[20];
				BATree T1 = NULL;
				printf("������Ҫ�жϴ��ļ��Ƿ�Ϊ�Ӽ����ļ���:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("���سɹ�!\n");
					if (set_subset(T, T1)) printf("���Ӽ�!\n");
					else printf("�����Ӽ�!\n");
				}
				else printf("����ʧ��!\n");
				break;
			} 
			case 10: {
				char filename[20];
				BATree T1 = NULL;
				printf("������Ҫ�жϵ��ļ���:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					if (set_equal(T, T1)) printf("���!\n");
					else printf("�����!\n");
				}
				else printf("����ʧ��!\n");
				break;
			}
			case 11:
			{
				count = 0;
				set_size(T, T, &count);
				printf("%d\n", count);
				break;
			}
			case 12: {
				create(&T);
				break;
			}
			case 13: {
				visit fp = &output_key;
				showtree(T, 0, 0, fp);
				break;
			}
			case 14: {
				FILE *fp;
				char filename[20];
				printf("�������ļ���:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if ((fp = fopen(filename, "wb")) == NULL) {
					printf("�ļ���ʧ��!\n");
					break;
				}
				if (save_data(T, fp)) printf("����ɹ�!\n");
				else printf("����ʧ��!\n");
				fclose(fp);
				break;
			}
			case 15: {
				char filename[20];
				printf("�������ļ���:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T, filename)) printf("���سɹ�!\n");
				break; 
			} 
			case 16:{ 
				BATree T0;
				if (input_pinfo(&T, p_size)) printf("�ɹ����������Ϣ!\n");
				else {
					printf("���������Ϣʧ��!\n");
					break;
				}
			    if (input_likes(&L, &l_size)) printf("�ɹ����밮����Ϣ!\n");
				else {
				    printf("���밮����Ϣʧ��!\n");
				    break;
				}
			    input_relation(&T, p_size, l_size);
			    T0 = T;
			    improve_sets(T, T0);
		    	break;
			}
			case 17: {
				BATree T1;
				int key;
				printf("������ؼ���:");
				scanf("%d", &key);
				getchar();
				if (!set_member(T, key, &T1)) {
					printf("û���ҵ�����!\n");
					break;
				}
				printf("\n");
				printf("name:%s%s\n", T1->data.lastname, T1->data.firstname);
				printf("\n");
				printf("friends:");
				output_relation(T1->data.friends, T);
				printf("\n\n");
				printf("fans:");
				output_relation(T1->data.fans, T);
				printf("\n\n");
				printf("following:");
				output_relation(T1->data.following, T);
				printf("\n\n");
				printf("likes:");
				output_likes(T1->data.likes, L);
				printf("\n\n");
				break;
			} 
			case 18: {
				int key1, key2;
				BATree T1 = NULL, T2 = NULL, T0 = NULL;
				printf("����������һ���˵Ĺؼ���:");
				scanf("%d", &key1);
				getchar();
				printf("��������һ�˵Ĺؼ���:");
				scanf("%d", &key2);
				getchar();
				if (!set_member(T, key1, &T1)) {
					printf("û���ҵ�����!\n");
					break;
				}
				if (!set_member(T, key2, &T2)) {
					printf("û���ҵ�����!\n");
					break;
				}
				set_intersection(T1->data.following, T2->data.following, &T0);
				if (T0) {
					output_relation(T0, T);
					break;
				}
				else {
					printf("û�й�ͬ��ע!\n");
					break;
				}
			}
			case 19: {
				int key1, key2;
				BATree T1 = NULL, T2 = NULL, T0 = NULL;
				printf("����������һ���˵Ĺؼ���:");
				scanf("%d", &key1);
				getchar();
				printf("��������һ�˵Ĺؼ���:");
				scanf("%d", &key2);
				getchar();
				if (!set_member(T, key1, &T1)) {
					printf("û���ҵ�����!\n");
					break;
				}
				if (!set_member(T, key2, &T2)) {
					printf("û���ҵ�����!\n");
					break;
				}
				set_intersection(T1->data.likes, T2->data.likes, &T0);
				if (T0) {
					output_likes(T0, L);
					break;
				}
				else {
					printf("û�й�ͬ����!\n");
					break;
				}
			}
			case 20: {
				int key;
				BATree T1 = NULL, T0 = NULL;
				printf("������ؼ���:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (!set_member(T, key, &T1)) {
					printf("û���ҵ�����!\n");
					break;
				}
				indirect_friends_traverse(T, T1->data.friends, &T0);
				set_difference(&T0, T1->data.friends);
				set_remove(&T0, T1->data.key, &shorter);
				output_relation(T0, T);
				break;
			}
			case 21: {
				InOrderTraverse(T);
				break;
			}
			case 22: {
				insert_name(T);
				getchar();
				break;
			}
			case 23: {
				printf("��ѡ��Ժ��Ѽ�ִ�еĲ�����\n");
				printf("0 ��ʼ��  1 ���Ӻ���  2 ɾ������  3 ���Һ���  4 ��������\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_friends(T); break;
				case 1:insert_friends(T); break;
				case 2:remove_friends(T); break;
				case 3:search_friends(T); break;
				case 4:traverse_friends(T); break;
				default:printf("�������룡");
				}
				getchar();
				break;
			}
			case 24: {
				printf("��ѡ��Է�˿��ִ�еĲ�����\n");
				printf("0 ��ʼ��  1 ���ӷ�˿  2 ɾ����˿  3 ���ҷ�˿  4 ������˿\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_fans(T); break;
				case 1:insert_fans(T); break;
				case 2:remove_fans(T); break;
				case 3:search_fans(T); break;
				case 4:traverse_fans(T); break;
				default:printf("�������룡");
				}
				getchar();
				break;
			}
			case 25: {
				printf("��ѡ��Թ�ע��ִ�еĲ�����\n");
				printf("0 ��ʼ��  1 ���ӹ�ע  2 ɾ����ע  3 ���ҹ�ע  4 ������ע\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_following(T); break;
				case 1:insert_following(T); break;
				case 2:remove_following(T); break;
				case 3:search_following(T); break;
				case 4:traverse_following(T); break;
				default:printf("��������ȷ�Ĳ������ţ�");
				}
				getchar();
				break;
			}
			case 26: {
				printf("��ѡ��԰��ü�ִ�еĲ�����\n");
				printf("0 ��ʼ��  1 ���Ӱ���  2 ɾ������  3 ���Ұ���  4 ��������\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_likes(T,L); break;
				case 1:insert_likes(T,L); break;
				case 2:remove_likes(T,L); break;
				case 3:search_likes(T,L); break;
				case 4:traverse_likes(T,L); break;
				default:printf("��������ȷ�Ĳ������ţ�");
				}
				getchar();
				break;
			}
			case 27: {
				input_likes(&L, &l_size);
				if (load(&T))
					printf("���سɹ���\n");
				else
					printf("����ʧ�ܣ�\n");
				getchar();
				break;
			}
			case 28: {
				if (save(T))
					printf("����ɹ���\n");
				else
					printf("����ʧ�ܣ�\n");
				getchar();
				break;
			}
			case 29: {
				avl_init(&T);
				break;
			}
			case 30: {
				avl_destroy(&T);
				printf("���ٳɹ�!\n");
				break;
			}
			case 31: {
				int key;
				printf("������ؼ���:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (avl_search(T, key, &Tsub)) printf("Ԫ��������!\n");
				else printf("Ԫ�ز�������!\n");
				break;
			}
			case 32: {
				Info e;
				input_key(&e);
				if (avl_insert(&T, e, &taller)) {
					printf("����ɹ�!\n");
					height(T, 0);
				}
				else printf("����ʧ��!\n");
				break;
			}
			case 33: {
				int key;
				printf("������ؼ���:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (avl_delete(&T, key, &shorter)) {
					printf("ɾ���ɹ�!\n");
					height(T, 0);
				}
				else printf("ɾ��ʧ��!\n");
				break;
			}
			case 34: {
				avl_traverse(T);
				break;
			}
			default:
				printf("�������!\n");
            }
			getchar();
			system("cls");
			}while (i);
    printf("\n--------------------   ��ӭ�´�ʹ��!   --------------------\n");
	return 0;
 }




 /**
 * �������ƣ�L_Rotate
 * �����������������ĸ�*p
 * �������ܣ�����*pΪ���Ķ���������������
 * ����ֵ���µ��������
 **/
void L_Rotate(BATree *p) {
	BATree temp = NULL;
	temp = (*p)->rchild;//tempָ��p�����������ڵ�
	(*p)->rchild = temp->lchild;//temp��������Ϊp��������
	temp->lchild = (*p);
	(*p) = temp;//*pָ���µĸ��ڵ�
}


/**
* �������ƣ�R_Rotate
* �����������������ĸ�*p
* �������ܣ�����*pΪ���Ķ���������������
* ����ֵ���µ��������
**/
void R_Rotate(BATree *p) {
	BATree temp = NULL;
	temp = (*p)->lchild;//tempָ��p�����������ڵ�
	(*p)->lchild = temp->rchild;//temp��������Ϊp��������
	temp->rchild = (*p);
	(*p) = temp;//*pָ���µĸ��ڵ�
}


/**
* �������ƣ�LeftBalance
* ����������������*T
* �������ܣ����������������ߣ���������ƽ��
* ����ֵ���¶�����*T
**/
void LeftBalance(BATree *T) {
	BATree l = NULL, r = NULL;
	l = (*T)->lchild;
	switch (l->bflag){
	case LH://LL
		(*T)->bflag = EH;
		l->bflag = EH;
		R_Rotate(T);//����T
		break;
	case EH:
		(*T)->bflag = LH;
		l->bflag = RH;
		R_Rotate(T);//����T
		break;
	case RH://LR
		r = l->rchild;
		switch (r->bflag){
		case LH:
			(*T)->bflag = RH;
			l->bflag = EH;
			break;
		case EH:
			(*T)->bflag = EH;
			l->bflag = EH;
			break;
		case RH:
			(*T)->bflag = EH;
			l->bflag = LH;
			break;
		}
		r->bflag = EH;
		L_Rotate(&((*T)->lchild));//����������
		R_Rotate(T);//����T
		break;
	}
}


/**
* �������ƣ�RightBalance
* ����������������*T
* �������ܣ����������������ߣ���������ƽ��
* ����ֵ���¶�����*T
**/
void RightBalance(BATree *T) {
	BATree r = NULL, l = NULL;
	r = (*T)->rchild;
	switch (r->bflag) {
	case LH://RL
		l = r->lchild;
		switch (l->bflag)
		{
		case LH:
			(*T)->bflag = EH;
			r->bflag = RH;
			break;
		case EH:
			(*T)->bflag = EH;
			r->bflag = EH;
			break;
		case RH:
			(*T)->bflag = LH;
			r->bflag = EH;
			break;
		}
		l->bflag = EH;
		R_Rotate(&((*T)->rchild));//�������Һ���
		L_Rotate(T);//������
		break;
	case EH:
		(*T)->bflag = RH;
		r->bflag = LH;
		L_Rotate(T);//������
		break;
	case RH://RR
		(*T)->bflag = EH;
		r->bflag = EH;
		L_Rotate(T);//������
		break;
	}
}


/**
* �������ƣ�set_init
* ����������������*T
* �������ܣ���ʼ��������
* ����ֵ����ʼ����Ķ�����
**/
void set_init(BATree *T) {
	 *T = NULL;
	 printf("��ʼ���ɹ�!\n");
}

void set_init_fors(BATree *T)
{
	(*T) = (BATree)malloc(sizeof(BATNode));
	(*T)->lchild = NULL;
	(*T)->rchild = NULL;
	(*T)->bflag = EH;
	(*T)->h = 0;
}


/**
* �������ƣ�set_destory
* ����������������*T
* �������ܣ����ٶ�����
* ����ֵ����
**/
void set_destory(BATree *T) {
	if ((*T) != NULL) {
		set_destory(&((*T)->lchild));
		set_destory(&((*T)->rchild));
		free(*T);
	}
}


/**
* �������ƣ�set_insert
* ����������������*T����Ϣe��bool�ͱ���taller(�������Ƿ�����)
* �������ܣ���ƽ��������в���ڵ�e�������ֶ�����ƽ��
* ����ֵ���ɹ�����true�����򷵻�false
**/
bool set_insert(BATree *T, Info e, bool *taller) {
	if ((*T)==NULL)
	{
		(*T) = (BATree)malloc(sizeof(BATNode));//����ռ�
		(*T)->data = e;
		(*T)->lchild = NULL;
		(*T)->rchild = NULL;
		(*T)->bflag = EH;
		*taller = true;//���Ϊtrue
	}
	else {
		if (e.key == (*T)->data.key) {//�����йؼ���
			*taller = false;
			return false;
		}
		if (e.key < (*T)->data.key) {//����������
			if (!set_insert(&((*T)->lchild), e, taller))return false;//����ʧ��
			if (*taller)//����ɹ�
				switch ((*T)->bflag) {
				case LH://��߸�
					LeftBalance(T);
					*taller = false;
					break;
				case EH://�ȸ�
					(*T)->bflag = LH;
					*taller = true;
					break;
				case RH://�ұ߸�
					(*T)->bflag = EH;
					*taller = false;
					break;
				}
		}
		else {//�������в���
			if (!set_insert(&((*T)->rchild), e, taller))return false;//����ʧ��
			if (*taller)//����ɹ�
				switch ((*T)->bflag) {
				case LH://��߸�
					(*T)->bflag = EH;
					*taller = false;
					break;
				case EH://�ȸ�
					(*T)->bflag = RH;
					*taller = true;
					break;
				case RH://�ұ߸�
					RightBalance(T);
					*taller = false;
					break;
				}
		}
	}
	return true;
}


/**
* �������ƣ�set_remove
* ����������������*T���ؼ���key��bool�ͱ���shorter(�������Ƿ�䰫)
* �������ܣ���ƽ���������ɾ���ڵ�e�������ֶ�����ƽ��
* ����ֵ���ɹ�����true�����򷵻�false
**/
bool set_remove(BATree *T, int key, bool *shorter) {
	if ((*T) == NULL)return false;//������
	else if (key == (*T)->data.key) {
		BATree p;
		if ((*T)->lchild == NULL) {//��������
			p = (*T);
			(*T) = (*T)->rchild;
			free(p);
			*shorter = true;
		}
		else if ((*T)->rchild == NULL) {//��������
			p = (*T);
			(*T) = (*T)->lchild;
			free(p);
			*shorter = true;
		}
		else {//������������Ϊ��
			p = (*T)->lchild;
			while (p->rchild) {
				p = p->rchild;
			}
			(*T)->data = p->data;
			set_remove(&((*T)->lchild), p->data.key, shorter);//�������ݹ�ɾ��ǰ���ڵ�
			if (*shorter) {
				switch ((*T)->bflag) {
				case LH:
					(*T)->bflag = EH;
					*shorter = true;
					break;
				case EH:
					(*T)->bflag = RH;
					*shorter = false;
					break;
				case RH:
					if ((*T)->rchild->bflag == EH) *shorter = false;//��ǰ�ڵ��Һ���״̬ΪEH
					else *shorter = true;
					RightBalance(T);//��ƽ��
					break;
				}
			}
		}
	}
	else if (key<(*T)->data.key) {//�������м����ҽڵ�
		if (!set_remove(&((*T)->lchild), key, shorter))return false;
		if (*shorter) {
			switch ((*T)->bflag) {
			case LH:
				(*T)->bflag = EH;
				*shorter = true;
				break;
			case EH:
				(*T)->bflag = RH;
				*shorter = false;
				break;
			case RH:
				if ((*T)->rchild->bflag == EH) *shorter = false;//�ڵ��Һ���״̬EH
				else *shorter = true;
				RightBalance(T);//��ƽ��
				break;
			}
		}
	}
	else {//�������м����ҽڵ�
		if (!set_remove(&((*T)->rchild), key, shorter))return false;
		if (*shorter) {
			switch ((*T)->bflag) {
			case LH:
				if ((*T)->lchild->bflag == EH) *shorter = false;//�ڵ�����״̬ΪEH
				else *shorter = true;
				LeftBalance(T);//��ƽ��
				break;
			case EH:
				(*T)->bflag = LH;
				*shorter = false;
				break;
			case RH:
				(*T)->bflag = EH;
				*shorter = true;
				break;
			}
		}
	}
	return true;
}


/**
* �������ƣ�set_intersection
* ����������������T��������T1��������*T0
* �������ܣ���T��T1�Ľ���
* ����ֵ������T0
**/
void set_intersection(BATree T, BATree T1, BATree *T0) {
	if (T1 == NULL)return;
	if (set_member(T, T1->data.key, &Tsub)) set_insert(T0, T1->data, &taller);//��Ԫ��Ϊ�������Ԫ��
	set_intersection(T, T1->lchild, T0);
	set_intersection(T, T1->rchild, T0);
}


/**
* �������ƣ�set_union
* ����������������*T��������T1
* �������ܣ���T��T1�Ĳ���
* ����ֵ������T
**/
void set_union(BATree *T, BATree T1) {
	if (T1 == NULL)return;
	set_insert(T, T1->data, &taller);//����
	set_union(T, T1->lchild);
	set_union(T, T1->rchild);
}


/**
* �������ƣ�set_difference
* ����������������*T��������T1
* �������ܣ���T��T1�Ĳ�
* ����ֵ����T
**/
void set_difference(BATree *T, BATree T1) {
	if (*T == NULL || T1 == NULL)return;
	set_remove(T, T1->data.key, &shorter);//ɾ��T1
	set_difference(T, T1->lchild);
	set_difference(T, T1->rchild);
}


/**
* �������ƣ�set_size
* ����������������T1��������T
* �������ܣ���T�ĸ���
* ����ֵ����
**/
void set_size(BATree T1, BATree T) {
	if (T1 == NULL)return;
	set_size(T1->lchild, T);
	if (set_member(T, T1->data.key, &Tsub))count++;//��Ŀ��1
	set_size(T1->rchild, T);
}


/**
* �������ƣ�set_member
* ����������������T���ؼ���key,������*Tsub(ָ��Ҫ�ҵĽڵ�)
* �������ܣ��жϹؼ���Ϊkey�Ľڵ��Ƿ���T�еĳ�Ա
* ����ֵ���Ƿ���true�����򷵻�false
**/
bool set_member(BATree T, int key, BATree *Tsub) {
	if (T == NULL)return false;
	if (T->data.key == key) {
		*Tsub = T;
		return true;
	}
	else if (key<T->data.key) {//����������
		if (set_member(T->lchild, key, Tsub))return true;
	}
	else {if (set_member(T->rchild, key, Tsub))return true; }//����������
	return false;
}


/**
* �������ƣ�set_subset
* ����������������T��������T1
* �������ܣ��ж�T1�Ƿ���T���Ӽ�
* ����ֵ���Ƿ���true�����򷵻�false
**/
bool set_subset(BATree T, BATree T1) {
	if (T1 == NULL)return true;
	if (set_member(T, T1->data.key, &Tsub)) {
		if (!set_subset(T, T1->lchild)) return false;//�г�Ա����T����
		if (!set_subset(T, T1->rchild)) return false;//�г�Ա����T����
		return true;
	}
	else return false;
}


/**
* �������ƣ�set_equal
* ����������������T��������T1
* �������ܣ��ж�T1�Ƿ���T���
* ����ֵ���Ƿ���true�����򷵻�false
**/
bool set_equal(BATree T, BATree T1) {
	if (!set_subset(T, T1))return false;//T�Ƿ���T1�Ӽ�
	if (!set_subset(T1, T))return false;//T1�Ƿ���T���Ӽ�
	return true;
}


/**
* �������ƣ�load_data
* ����������������*T���ļ���filename
* �������ܣ���������
* ����ֵ���ɹ�����true�����򷵻�false
**/
bool load_data(BATree *T, char *filename) {
	BATree p;
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL) {
		printf("���ļ�ʧ��!\n");
		return false;
	}
	while (!feof(fp)) {
		if ((p = (BATree)malloc(sizeof(BATNode))) == NULL) {//����ռ�
			printf("����ռ䲻��!\n");
			fclose(fp);
			return false;
		}
		if (fread(p, sizeof(BATNode), 1, fp) != 1) {
			free(p);//����
			break;
		}
		set_insert(T, p->data, &taller);//����
	}
	fclose(fp);
	height(*T, 0);
	return true;
}


/**
* �������ƣ�save_data
* ����������������T���ļ�ָ��fp
* �������ܣ��洢����
* ����ֵ���ɹ�����true�����򷵻�false
**/
bool save_data(BATree T, FILE *fp) {
	if (T) {
		if (fwrite(T, sizeof(BATNode), 1, fp) != 1) {//д���ļ�
			printf("����ʧ��!\n");
			fclose(fp);
			return false;
		}
	}
	else return true;
	if (!save_data(T->lchild, fp))return false;//�ݹ����
	if (!save_data(T->rchild, fp))return false;
	return true;
}


/**
* �������ƣ�input_key
* ��������������ָ��data
* �������ܣ��������ݵĹؼ���
* ����ֵ����
**/
void input_key(Info *data) {
	printf("������ؼ���:");
	scanf("%d", &(data->key));//����ؼ���
	getchar();
	printf("\n");
}


/**
* �������ƣ�height
* ����������������T������i
* �������ܣ������������
* ����ֵ����������
**/
void height(BATree T, int i) {
	if(T) T->h = i + 1;
	else return;
	height(T->lchild, T->h);//�ݹ���߶�
	height(T->rchild, T->h);
}


/**
* �������ƣ�output_key
* ����������������T
* �������ܣ�����ؼ���
* ����ֵ����
**/
void output_key(BATree T) {
	printf("%d", T->data.key);//����ؼ���
}


/**
* �������ƣ�create
* ����������������*T
* �������ܣ�����������
* ����ֵ����
**/
void create(BATree *T) {
	char s;
	Info *e;
	if ((e = (Info *)malloc(sizeof(Info))) == NULL) {//����ռ�
		printf("�洢�ռ䲻��!\n");
		return;
	}
	input_key(e);
	if (set_insert(T, *e, &taller)) {//����
		printf("�����ɹ����Ƿ�Ҫ����?(y/n):");
		while (1) {
			s = getchar();
			getchar();
			printf("\n");
			switch (s) {
			case 'y':
				create(T);//�ݹ����
				height(*T, 0);
				return;
			case 'n':
				printf("��ɴ���!\n");
				return;
			default:
				printf("�����������������:");
			}
		}
	}
	else printf("����ʧ��!\n");
	return;
}


/**
* �������ƣ�gotoxy
* ��������������x��y
* �������ܣ����ù��λ��
* ����ֵ����
**/
void gotoxy(int x, int y) {
	COORD coord = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


/**
* �������ƣ�showtree
* ����������������T������x��y��output_t
* �������ܣ���ʾ��ͼ
* ����ֵ����
**/
void showtree(BATree T, int x, int y, visit output_t) {
	if (T) {
		gotoxy(x + x_move, y + y_move);
		output_t(T);//���
	}
	else return;
	if (T->lchild || T->rchild) {//���Һ��Ӷ���
		gotoxy(x + x_move, y + y_move + 1);
		printf(" | ");
		if (T->lchild) {//���Ӵ���
			int i,j;
			i = 80 / (pow(2, T->h) + 1);
			j = i;
			gotoxy(x + x_move - j, y + y_move + 3);
			printf(" | ");
			gotoxy(x + x_move - i + 2, y + y_move + 2);
			while (i) {
				printf("_");
				i--;
			}
			showtree(T->lchild, x - j, y + 4, output_t);//�ݹ����
		}
		if (T->rchild) {//�Һ��Ӵ���
			int i, j;
			i = 80 / (pow(2, T->h) + 1);
			j = i;
			gotoxy(x + x_move + 1, y + y_move + 2);
			while (i) {
				printf("_");
				i--;
			}
			gotoxy(x + x_move + j, y + y_move + 3);
			printf(" | ");
			showtree(T->rchild, x + j, y + 4, output_t);//�ݹ����
		}
	}
}


/**
* �������ƣ�InOrderTraverse
* ����������������T
* �������ܣ��������
* ����ֵ����
**/
void InOrderTraverse(BATree T){
	if (T == NULL)return;
	InOrderTraverse(T->lchild);
	printf("%3d%s%s ", T->data.key, T->data.lastname, T->data.firstname);//���������Ϣ
	InOrderTraverse(T->rchild);
}


/**
* �������ƣ�output_relation
* ����������������T1,������T
* �������ܣ������ϵ
* ����ֵ����
**/
void output_relation(BATree T1, BATree T) {
	if (T1 == NULL)return;
	output_relation(T1->lchild, T);
	if (set_member(T, T1->data.key, &Tsub)) printf("%3d%s%s",Tsub->data.key,Tsub->data.lastname,Tsub->data.firstname);//�����Ϣ
	output_relation(T1->rchild,T);
}


/**
* �������ƣ�output_likes
* ����������������T1,������T
* �������ܣ��������
* ����ֵ����
**/
void output_likes(BATree T1, BATree T) {
	if (T1 == NULL)return;
	output_likes(T1->lchild, T);
	if (set_member(T, T1->data.key, &Tsub)) printf("%3d%s", Tsub->data.key, Tsub->data.like);//�����Ϣ
	output_likes(T1->rchild, T);
}


/**
* �������ƣ�input_pinfo
* ����������������*T,����p_size
* �������ܣ���������Ϣ
* ����ֵ����
**/
bool input_pinfo(BATree *T, int p_size) {
	Info *e;
	FILE *fp1, *fp2;
	int num;
	if ((fp1 = fopen("lastname.txt", "r")) == NULL) {
		printf("��lastnameʧ��!\n");
		return false;
	}
	if ((fp2 = fopen("firstname.txt", "r")) == NULL) {
		printf("��firstnameʧ��!\n");
		return false;
	}
	for (num = 0; num <= p_size; num++) {//������ɴ����ݼ�
		if ((e = (Info *)malloc(sizeof(Info))) == NULL) {
			printf("�洢�ռ䲻��!\n");
			fclose(fp1);//�ر�ָ��
			fclose(fp2);
			return false;
		}
		if (feof(fp1)) {
			fclose(fp1);
			if ((fp1 = fopen("lastname.txt", "r")) == NULL) {
				printf("��lastnameʧ��!\n");
				return false;
			}
		}
		else {
			if (fgets(e->lastname, 3, fp1) == NULL) {//��ȡ��
				free(e);
				break;
			}
		}
		if (feof(fp2)) {
			fclose(fp2);
			if ((fp2 = fopen("firstname.txt", "r")) == NULL){
				printf("��firstnameʧ��!\n");
				return false;
			}
		}
		else {
			if (fgets(e->firstname, 5, fp2) == NULL) {//��ȡ��
				free(e);
				break;
			}
		}
		e->key = num;//�ؼ��ַ���
		e->friends = NULL;//�ÿ�
		e->fans = NULL;
		e->following = NULL;
		e->likes = NULL;
		set_insert(T, *e, &taller);//����
	}
	fclose(fp1);
	fclose(fp2);
	height(*T, 0);
	return true;
}
 

/**
* �������ƣ�input_likes
* ����������������T,����l_size
* �������ܣ����밮��
* ����ֵ����
**/
bool input_likes(BATree *T, int *l_size) {
	Info *e;
	FILE *fp;
	int num = 0;
	if ((fp = fopen("likes.txt", "r")) == NULL) {
		printf("��likesʧ��!\n");
		return false;
	}
	while (!feof(fp)) {
		if ((e = (Info *)malloc(sizeof(Info))) == NULL) {//����ռ�
			printf("�洢�ռ䲻��!\n");
			fclose(fp);
			return false;
		}
		if (fgets(e->like, 5, fp) == NULL) {//��ȡ����
			free(e);
			break;
		}
		e->key = num++;//����ؼ���
		e->friends = NULL;
		e->fans = NULL;
		e->following = NULL;
		e->likes = NULL;
		set_insert(T, *e, &taller);//����
	}
	*l_size = num;
	fclose(fp);//�ر��ļ�ָ��
	height(*T, 0);
	return true;
}


/**
* �������ƣ�input_relation
* ����������������T,����p_size,l_size
* �������ܣ������ϵ
* ����ֵ����
**/
void input_relation(BATree *T, int p_size, int l_size) {
	if (*T == NULL)return;
	input_information(&((*T)->data.friends), p_size, 80, 20);//������ɺ���
	input_information(&((*T)->data.fans), p_size, 60, 10);//������ɷ�˿
	input_information(&((*T)->data.following), p_size, 30, 5);//������ɹ�ע
	input_information(&((*T)->data.likes), l_size, 8, 2);//������ɰ���
	input_relation(&((*T)->lchild), p_size, l_size);//�ݹ�
	input_relation(&((*T)->rchild), p_size, l_size);
}


/**
* �������ƣ�input_information
* ����������������T������size��max��min
* �������ܣ�����key
* ����ֵ����
**/
void input_information(BATree *T, int size, int max, int min) {
	int i;
	Info *e;
	for (i = 0; i <= rand() % max + min; i++) {//�������
		if ((e = (Info *)malloc(sizeof(Info))) == NULL) {
			printf("�洢�ռ䲻��!\n");
			break;
		}
		e->key = rand() % size;
		e->friends = NULL;//�ÿ�
		e->fans = NULL;
		e->following = NULL;
		e->likes = NULL;
		set_insert(T, *e, &taller);//������Ϣ
	}
}


/**
* �������ƣ�improve_sets
* ����������������T,������T0
* �������ܣ����Ƽ���
* ����ֵ����
**/
void improve_sets(BATree T, BATree T0) {
	if (T0 == NULL)return;
	improve_sets(T, T0->lchild);//�ݹ�
	improve_friends(T, T0, T0->data.friends);//���ƺ���
	improve_fans(T, T0, T0->data.fans);//���Ʒ�˿
	improve_following(T, T0, T0->data.following);//���ƹ�ע
	set_remove(&T0->data.friends, T0->data.key, &shorter);//ɾ���Լ�
	set_remove(&T0->data.fans, T0->data.key, &shorter);//ɾ���Լ�
	set_remove(&T0->data.following, T0->data.key, &shorter);//ɾ���Լ�
	improve_sets(T, T0->rchild);
}


/**
* �������ƣ�improve_friends
* ����������������T1,������T1��������T2
* �������ܣ����ƺ��Ѽ������ƺ��������֮��Ĺ�ϵ
* ����ֵ����
**/
void improve_friends(BATree T, BATree T1, BATree T2) {
	if (T2 == NULL)return;
	BATree T3;
	set_member(T, T2->data.key, &Tsub);
	if (!set_member(Tsub->data.friends, T1->data.key, &T3)) set_insert(&Tsub->data.friends, T1->data, &taller);//ʹ�ú�����˫���
	improve_friends(T, T1, T2->lchild);//�ݹ�
	improve_friends(T, T1, T2->rchild);
}


/**
* �������ƣ�improve_fans
* ����������������T1,������T1��������T2
* �������ܣ����Ʒ�˿�Ĺ�ע��
* ����ֵ����
**/
void improve_fans(BATree T, BATree T1, BATree T2) {
	if (T2 == NULL)return;
	BATree T3;
	set_member(T, T2->data.key, &Tsub);
	if (!set_member(Tsub->data.following, T1->data.key, &T3)) set_insert(&Tsub->data.following, T1->data, &taller);//�Լ�û���ڷ�˿�Ĺ�ע�˼���
	improve_fans(T, T1, T2->lchild);//�ݹ�
	improve_fans(T, T1, T2->rchild);
}


/**
* �������ƣ�improve_following
* ����������������T1,������T1��������T2
* �������ܣ����ƹ�ע�ķ�˿��
* ����ֵ����
**/
void improve_following(BATree T, BATree T1, BATree T2) {
	if (T2 == NULL)return;
	BATree T3;
	set_member(T, T2->data.key, &Tsub);
	if (!set_member(Tsub->data.fans, T1->data.key, &T3)) set_insert(&Tsub->data.fans, T1->data, &taller);//�Լ�û���ڹ�ע�˵ķ�˿����
	improve_following(T, T1, T2->lchild);//�ݹ�
	improve_following(T, T1, T2->rchild);
}


/**
* �������ƣ�indirect_friends_traverse
* ����������������T1,������T1��������*T0
* �������ܣ����Ⱥ��ѱ���
* ����ֵ����
**/
void indirect_friends_traverse(BATree T, BATree T1, BATree *T0) {
	if (T1 == NULL)return;
	set_member(T, T1->data.key, &Tsub);
	indirect_friends_insert(Tsub->data.friends, T0);//�����������Ӻ���
	indirect_friends_traverse(T, T1->lchild, T0);//����
	indirect_friends_traverse(T, T1->rchild, T0);
}


/**
* �������ƣ�indirect_friends_insert
* ����������������T,������*T0
* �������ܣ����Ⱥ�������
* ����ֵ����
**/
void indirect_friends_insert(BATree T, BATree *T0) {
	if (T == NULL)return;
	set_insert(T0, T->data, &taller);//����
	indirect_friends_insert(T->lchild, T0);
	indirect_friends_insert(T->rchild, T0);
}


/**
* �������ƣ�insert_name
* ����������������T
* �������ܣ���������
* ����ֵ����
**/
bool insert_name(BATree T) {
	int key;
	BATree T1;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	printf("�������գ�");
	scanf("%s", T1->data.lastname);
	printf("����������");
	scanf("%s", T1->data.firstname);
	printf("����ɹ���");
	return true;
}


/**
* �������ƣ�traverse_del_friends
* ����������������T��T1
* �������ܣ����Ƴ�ʼ�����Ѽ�
* ����ֵ����
**/
bool traverse_del_friends(BATree T, BATree T1) {
	if (T)
	{
		if (set_member(T->data.friends, T1->data.key, &Tsub))
		{
			set_remove(&(T->data.friends), T1->data.key, &shorter);
		}
		traverse_del_friends(T->lchild, T1);
		traverse_del_friends(T->rchild, T1);
	}
	return true;
}


/**
* �������ƣ�init_friends
* ����������������T
* �������ܣ���������Ѽ�
* ����ֵ����
**/
bool init_friends(BATree T) {
	int key;
	BATree T1;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	set_init_fors(&(T1->data.friends));//��ʼ�����Ѽ�
	traverse_del_friends(T, T1);//������
}


/**
* �������ƣ�insert_friends
* ����������������T
* �������ܣ��������
* ����ֵ����
**/
bool insert_friends(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�\n");
		return false;
	}
	if (T1->data.friends == NULL)//δ��ʼ��
	{
		printf("%d�ĺ��Ѽ�δ��ʼ����\n", key1);
		return false;
	}
	if (T1->data.friends == NULL)
		printf("%d��ǰ�޺��ѣ�\n", key1);
	else
	{
		printf("%d��ǰ�����У�", key1);
		output_relation(T1->data.friends, T);//�������
		printf("\n");
	}
	printf("������Ҫ����ĺ��ѹؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (T2->data.friends == NULL)
	{
		printf("%d�ĺ��Ѽ�δ��ʼ����\n", T2->data.key);
		return false;
	}
	if (!set_member(T1->data.friends, key2, &T0))//���Ǻ���
	{
		Info data1, data2;
		data1.key = key2;
		data2.key = key1;
		set_insert(&(T1->data.friends), data1, &taller);//T1���Ѳ���T2
		set_insert(&(T2->data.friends), data2, &taller);//T2���Ѳ���T1
		printf("%d��%d�����Ϊ���ѣ�\n", key1, key2);
		return true;
	}
	printf("�ú����Ѵ��ڣ�");
	return false;
}


/**
* �������ƣ�remove_friends
* ����������������T
* �������ܣ�ɾ������
* ����ֵ����
**/
bool remove_friends(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.friends == NULL)//δ��ʼ��
	{
		printf("%d�ĺ��Ѽ�δ��ʼ����\n", key1);
		return false;
	}
	if (T1->data.friends == NULL)
		printf("%d��ǰ�޺��ѣ�\n", key1);
	else
	{
		printf("%d��ǰ�����У�", key1);
		output_relation(T1->data.friends, T);//�������
		printf("\n");
	}
	printf("������Ҫɾ���ĺ��ѹؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.friends == NULL)//���ѱ��Ͳ�����
	{
		printf("�ú��Ѳ����ڣ�");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (set_member(T1->data.friends, key2, &T0))//T2��T1����
	{
		set_remove(&(T1->data.friends), key2, &shorter);//T1���Ѽ�ɾ��T2
		set_remove(&(T2->data.friends), key1, &shorter);//T2���Ѽ�ɾ��T1
		printf("%d��%d��ɾ�ɹ���\n", key1, key2);
		return true;
	}
	printf("�ú��Ѳ����ڣ�");
	return false;
}


/**
* �������ƣ�search_friends
* ����������������T
* �������ܣ���������
* ����ֵ����
**/
bool search_friends(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	printf("��������ѹؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))//��Ա������
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.friends == NULL)//�޺���
	{
		printf("%d�޺��ѣ�\n", key1);
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (set_member(T1->data.friends, key2, &T0))//�Ǻ���
	{
		printf("%d��%d�ĺ��ѣ�\n", key2, key1);
		return true;
	}
	printf("%d����%d�ĺ��ѣ�\n", key2, key1);
	return false;
}


/**
* �������ƣ�traverse_friends
* ����������������T
* �������ܣ���������
* ����ֵ����
**/
bool traverse_friends(BATree T) {
	int key;
	BATree T1;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.friends == NULL)//δ��ʼ��
	{
		printf("%d�ĺ��Ѽ�δ��ʼ����\n", T1->data.key);
		return false;
	}
	if (T1->data.friends == NULL)
		printf("%d��ǰ�޺��ѣ�\n", key);
	else
	{
		printf("%d��ǰ�ĺ����У�", key);
		output_relation(T1->data.friends,T);//�������
		printf("\n");
	}
}


/**
* �������ƣ�traverse_del_following
* ����������������T��T1
* �������ܣ����Ƴ�ʼ����˿��
* ����ֵ����
**/
bool traverse_del_following(BATree T, BATree T1) {
	if (T)
	{
		if (set_member(T->data.following, T1->data.key, &Tsub))
		{
			set_remove(&(T->data.following), T1->data.key, &shorter);
		}
		traverse_del_following(T->lchild, T1);
		traverse_del_following(T->rchild, T1);
	}
	return true;
}


/**
* �������ƣ�init_fans
* ����������������T
* �������ܣ���ʼ����˿��
* ����ֵ����
**/
bool init_fans(BATree T) {
	int key;
	BATree T1;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�\n");
		return false;
	}
	set_init_fors(&(T1->data.fans));//��ʼ��
	traverse_del_following(T, T1);
}


/**
* �������ƣ�insert_fans
* ����������������T
* �������ܣ������˿
* ����ֵ����
**/
bool insert_fans(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�\n");
		return false;
	}
	if (T1->data.fans == NULL)//δ��ʼ��
	{
		printf("%d�ķ�˿��δ��ʼ����\n", key1);
		return false;
	}
	if (T1->data.fans == NULL) printf("%d��ǰ�޷�˿��\n", key1);
	else
	{
		printf("%d��ǰ��˿�У�", key1);
		output_relation(T1->data.fans,T);//�����˿
		printf("\n");
	}
	printf("������Ҫ����ķ�˿�ؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�\n");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���\n");
		return false;
	}
	if (!set_member(T1->data.fans, key2, &T0))//T2����T1��˿
	{
		Info data1;
		Info data2;
		data1.key = key2;
		data2.key = key1;
		set_insert(&(T1->data.fans), data1, &taller);//T2����T1��˿��
		set_insert(&(T2->data.following), data2, &taller);//T1����T2��ע��
		printf("����ɹ���\n");
		return true;
	}
	printf("�÷�˿�Ѵ��ڣ�\n");
	return false;
}


/**
* �������ƣ�remove_fans
* ����������������T
* �������ܣ�ɾ����˿
* ����ֵ����
**/
bool remove_fans(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�\n");
		return false;
	}
	if (T1->data.fans == NULL)//δ��ʼ��
	{
		printf("%d�ķ�˿��δ��ʼ����\n", key1);
		return false;
	}
	if (T1->data.fans == NULL)
		printf("%d��ǰ�޷�˿��\n", key1);
	else
	{
		printf("%d��ǰ�ķ�˿�У�", key1);
		output_relation(T1->data.fans, T);//�����˿
		printf("\n");
	}
	printf("������Ҫɾ���ķ�˿�ؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (set_member(T1->data.fans, key2, &T0))
	{
		set_remove(&(T1->data.fans), key2,&shorter);//��T2��T1��˿����ɾ��
		set_remove(&(T2->data.following), key1, &shorter);//��T1��T2��ע����ɾ��
		printf("ɾ���ɹ���\n");
		return true;
	}
	printf("�÷�˿�����ڣ�\n");
	return false;
}


/**
* �������ƣ�search_fans
* ����������������T
* �������ܣ�������˿
* ����ֵ����
**/
bool search_fans(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�\n");
		return false;
	}
	if (T1->data.fans == NULL)//�޷�˿
	{
		printf("%d�޷�˿��\n", key1);
		return false;
	}
	printf("������Ҫ���ҵķ�˿�ؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (set_member(T1->data.fans, key2, &T0))//�ǳ�Ա
	{
		printf("%d��%d�ķ�˿��\n", key2, key1);
		return true;
	}
	printf("%d����%d�ķ�˿��\n", key2, key1);
	return false;
}


/**
* �������ƣ�traverse_fans
* ����������������T
* �������ܣ�������˿
* ����ֵ����
**/
bool traverse_fans(BATree T) {
	int key;
	BATree T1;
	printf("�������Ա��ţ�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.fans == NULL)//δ��ʼ��
	{
		printf("%d�ķ�˿��δ��ʼ����\n", T1->data.key);
		return false;
	}
	if (T1->data.fans == NULL)
		printf("%d��ǰ�޷�˿��\n", key);
	else
	{
		printf("%d��ǰ��˿�У�", key);
		output_relation(T1->data.fans, T);//�����˿
		printf("\n");
	}
}


/**
* �������ƣ�traverse_del_fans
* ����������������T��T1
* �������ܣ����Ƴ�ʼ����ע��
* ����ֵ����
**/
bool traverse_del_fans(BATree T, BATree T1) {
	if (T)
	{
		if (set_member(T->data.fans, T1->data.key, &Tsub))
		{
			set_remove(&(T->data.fans), T1->data.key, &shorter);
		}
		traverse_del_fans(T->lchild, T1);
		traverse_del_fans(T->rchild, T1);
	}
	return true;
}


/**
* �������ƣ�init_following
* ����������������T
* �������ܣ���ʼ����ע��
* ����ֵ����
**/
bool init_following(BATree T) {
	int key;
	BATree T1;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	set_init_fors(&(T1->data.following));//��ʼ��
	traverse_del_fans(T, T1);
}


/**
* �������ƣ�insert_following
* ����������������T
* �������ܣ������ע
* ����ֵ����
**/
bool insert_following(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.following == NULL)//δ��ʼ��
	{
		printf("%d�ķ�˿��δ��ʼ����\n", key1);
		return false;
	}
	if (T1->data.following == NULL) printf("%d��ǰ�޹�ע��\n", key1);
	else
	{
		printf("%d��ǰ��ע�У�", key1);
		output_relation(T1->data.following,T);//�����ע
		printf("\n");
	}
	printf("������Ҫ����Ĺ�ע�ؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (!set_member(T1->data.following, key2, &T0))//���ǳ�Ա
	{
		Info data1;
		Info data2;
		data1.key = key2;
		data2.key = key1;
		set_insert(&(T1->data.following), data1,&taller);//��T2����T1�Ĺ�ע��
		set_insert(&(T2->data.fans), data2, &taller);//��T1����T2�ķ�˿��
		printf("����ɹ���");
		return true;
	}
	printf("�ù�ע�Ѵ��ڣ�");
	return false;
}


/**
* �������ƣ�remove_following
* ����������������T
* �������ܣ�ɾ����ע
* ����ֵ����
**/
bool remove_following(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.following == NULL)//δ��ʼ��
	{
		printf("%d�ķ�˿��δ��ʼ����\n", key1);
		return false;
	}
	if (T1->data.following == NULL)
		printf("%d��ǰ�޹�ע��\n", key1);
	else
	{
		printf("%d��ǰ��ע�У�", key1);
		output_relation(T1->data.following,T);//�����ע
		printf("\n");
	}
	printf("������Ҫȡ����ע�ĳ�Ա�ؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (set_member(T1->data.following, key2, &T0))
	{
		set_remove(&(T1->data.following), key2,&shorter);//��T2��T1��ע��ɾ��
		set_remove(&(T2->data.fans), key1, &shorter);//��T1��T2��˿��ɾ��
		printf("ɾ���ɹ���");
		return true;
	}
	printf("�ù�ע�����ڣ�");
	return false;
}


/**
* �������ƣ�search_following
* ����������������T
* �������ܣ�������ע
* ����ֵ����
**/
bool search_following(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.following == NULL)//�޹�ע
	{
		printf("%d�޹�ע��\n", key1);
		return false;
	}
	printf("������Ҫ���ҵĹ�ע�ؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (key1 == key2)//���������Լ�
	{
		printf("���������Լ���");
		return false;
	}
	if (set_member(T1->data.following, key2, &T0))//�ǳ�Ա
	{
		printf("%d��%d�Ĺ�ע��\n", key2, key1);
		return true;
	}
	printf("%d����%d�Ĺ�ע��\n", key2, key1);
	return false;
}


/**
* �������ƣ�traverse_following
* ����������������T
* �������ܣ�������ע
* ����ֵ����
**/
bool traverse_following(BATree T) {
	int key;
	BATree T1;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.following == NULL)//δ��ʼ��
	{
		printf("%d�ķ�˿��δ��ʼ����\n", key);
		return false;
	}
	if (T1->data.following == NULL)//�޹�ע
		printf("%d��ǰ�޹�ע��\n", key);
	else
	{
		printf("%d��ǰ��ע�У�", key);
		output_relation(T1->data.following,T);//�����ע
		printf("\n");
	}
}


/**
* �������ƣ�init_likes
* ����������������T��������L
* �������ܣ���ʼ�����ü�
* ����ֵ����
**/
bool init_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	set_init_fors(&(T1->data.likes));//��ʼ��
	T1->data.likes->h = 0;
}


/**
* �������ƣ�insert_likes
* ����������������T��������L
* �������ܣ����밮��
* ����ֵ����
**/
bool insert_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.likes == NULL)//δ��ʼ��
	{
		printf("%d�İ��ü�δ��ʼ����\n", key);
		return false;
	}
	if (T1->data.likes == NULL)
		printf("%d��ǰ�ް��ã�\n", key);
	else
	{
		printf("%d��ǰ�����У�", key);
		output_likes(T1->data.likes, L);//�������
		printf("\n");
	}
	int key2;
	printf("������Ҫ����İ��ùؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(L, key2, &T0))
	{
		printf("�ð��ò����ڣ�");
		return false;
	}
	if (!set_member(T1->data.likes, key2, &T0))//����û�д���
	{
		Info data1;
		data1.key = key2;
		set_insert(&(T1->data.likes), data1, &taller);//���밮��
		printf("����ɹ���");
		return true;
	}
	printf("�ð����Ѵ��ڣ�");
	getchar();
	return false;
}


/**
* �������ƣ�remove_likes
* ����������������T��������L
* �������ܣ�ɾ������
* ����ֵ����
**/
bool remove_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.likes == NULL)//δ��ʼ��
	{
		printf("%d�İ��ü�δ��ʼ����\n", T1->data.key);
		return false;
	}
	if (T1->data.likes == NULL)
		printf("%d��ǰ�ް��ã�\n", key);
	else
	{
		printf("%d��ǰ�����У�", key);
		output_likes(T1->data.likes, L);//�������
		printf("\n");
	}
	int key2;
	printf("������ɾ�����õĹؼ��֣�");
	scanf("%d", &key2);
	if (!set_member(L, key2, &T0))//���ò�����
	{
		printf("�ð��ò����ڣ�");
		return false;
	}
	if (set_member(T1->data.likes, key2, &T0))//����
	{
		set_remove(&(T1->data.likes), key2, &shorter);//ɾ��
		printf("ɾ���ɹ���");
		return true;
	}
	printf("�ð��ò����ڣ�");

	return false;
}


/**
* �������ƣ�search_likes
* ����������������T��������L
* �������ܣ���������
* ����ֵ����
**/
bool search_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.likes == NULL)//δ��ʼ��
	{
		printf("%d�İ��ü�δ��ʼ����\n", T1->data.key);
		return false;
	}
	int key2;
	printf("������Ҫ���ҵİ��ùؼ��֣�");
	scanf("%d", &key2);
	if (set_member(T1->data.likes, key2, &T0))//�ǰ���
	{
		printf("%d�ǰ��ã�\n", key2);
		return true;
	}
	printf("%d���ǰ��ã�\n", key2);
	return false;
}


/**
* �������ƣ�traverse_likes
* ����������������T��������L
* �������ܣ���������
* ����ֵ����
**/
bool traverse_likes(BATree T, BATree L) {
	int key;
	BATree T1;
	printf("�������Ա�ؼ��֣�");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("�ó�Ա�����ڣ�");
		return false;
	}
	if (T1->data.likes == NULL)//δ��ʼ��
	{
		printf("%d�İ��ü�δ��ʼ����\n", T1->data.key);
		return false;
	}
	if (T1->data.likes == NULL)
		printf("%d��ǰ�ް��ã�\n", key);
	else
	{
		printf("%d��ǰ�İ����У�", key);
		output_likes(T1->data.likes,L);//�������
		printf("\n");
	}
}



/**
* �������ƣ�load
* ����������������*T
* �������ܣ����ظ�����Ϣ
* ����ֵ����
**/
bool load(BATree *T) {
	char filename[20];
	FILE *fp;
	Info data;
	char s[10000];
	int i = 1;
	int key;
	printf("1 ���غ���  2 ���ط�˿  3 ���ع�ע  4 ���ذ���  0 �˳�\n");
	while (i)
	{
		printf("��ѡ�������");
		scanf("%d", &i);
		if ((i != 0) && (i != 1) && (i != 2) && (i != 3) && (i != 4)) continue;//����������
		if (i == 0) break;
		printf("�������ļ�����");
		scanf("%s", filename);
		getchar();
		printf("\n");
		if ((fp = fopen(filename, "r")) == NULL)//���ļ�
		{
			printf("�ļ���ʧ�ܣ�\n");
			return false;
		}
		switch (i)
		{
		case 1:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//�Ҳ�����Ա
			{
				printf("��Ϣ����\n");
				break;
			}
			set_init_fors(&(Ts->data.friends));//��ʼ�����Ѽ�
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//��һλ
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.friends), data,&taller);//����
			}
		}
			   break;
		case 2:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//�Ҳ�����Ա
			{
				printf("��Ϣ����\n");
				break;
			}
			set_init_fors(&(Ts->data.fans));//��ʼ��
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//��һ��
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.fans), data,&taller);//����
			}
		}
			   break;
		case 3:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//�Ҳ�����Ա
			{
				printf("��Ϣ����\n");
				break;
			}
			set_init_fors(&(Ts->data.following));//��ʼ��
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//��һ��
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.following), data,&taller);//����
			}
		}
			   break;
		case 4:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//�Ҳ�����Ա
			{
				printf("��Ϣ����\n");
				break;
			}
			set_init_fors(&(Ts->data.likes));//��ʼ��
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//��һ��
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.likes), data, &taller);//����
			}
		}
			   break;
			   fclose(fp);
		}
	}
	return true;
}




/**
* �������ƣ�save
* ����������������T
* �������ܣ����������Ϣ
* ����ֵ����
**/
bool save(BATree T) {
	FILE *fp;
	char filename[20];
	int i = 1;
	printf("1 �������   2 �����˿  3 �����ע  4 ���氮��  0 �˳�\n");
	while (i)
	{
		printf("��ѡ�������");
		scanf("%d", &i);
		if ((i != 0) && (i != 1) && (i != 2) && (i != 3) && (i != 4)) continue;//��������������
		if (i == 0) break;
		printf("�������ļ�����");
		scanf("%s", filename);
		getchar();
		printf("\n");
		if ((fp = fopen(filename, "w")) == NULL)//���ļ�
		{
			printf("�ļ���ʧ�ܣ�\n");
			return false;
		}
		save_relation(T, fp, i);//�����ϵ
		fclose(fp);
	}
	return true;
}


/**
* �������ƣ�save_relation
* ����������������T,�ļ�ָ��*fp
* �������ܣ����������Ϣ
* ����ֵ����
**/
bool save_relation(BATree T, FILE *fp, int i) {
	if (T != NULL)
	{
		fprintf(fp, "%d ", T->data.key);
		switch (i)
		{
		case 1:
			traverse_relation(T->data.friends, fp);//�������ѹ�ϵ
			fprintf(fp, "%d ", 666);//��־
			break;
		case 2:
			traverse_relation(T->data.fans, fp);//�����˿��ϵ
			fprintf(fp, "%d ", 666);
			break;
		case 3:
			traverse_relation(T->data.following, fp);//�����ע��ϵ
			fprintf(fp, "%d ", 666);
			break;
		case 4:
			traverse_relation(T->data.likes, fp);//���氮��
			fprintf(fp, "%d ", 666);
			break;
		}
		save_relation(T->lchild, fp, i);//�ݹ�
		save_relation(T->rchild, fp, i);
	}
}


/**
* �������ƣ�traverse_relation
* ����������������T,�ļ�ָ��*fp
* �������ܣ���������
* ����ֵ����
**/
bool traverse_relation(BATree T, FILE *fp) {
	if (T)
	{
		fprintf(fp, "%d ", T->data.key);
		traverse_relation(T->lchild, fp);
		traverse_relation(T->rchild, fp);
	}
	return true;
}




/**
* �������ƣ�avl_init
* ����������������*T
* �������ܣ���ʼ��������
* ����ֵ����ʼ����Ķ�����
**/
void avl_init(BATree *T) {
	*T = NULL;
	printf("��ʼ���ɹ�!\n");
}


/**
* �������ƣ�avl_destroy
* ����������������*T
* �������ܣ����ٶ�����
* ����ֵ����
**/
void avl_destroy(BATree *T) {
	if ((*T) != NULL) {
		avl_destroy(&((*T)->lchild));
		avl_destroy(&((*T)->rchild));
		free(*T);
	}
}


/**
* �������ƣ�avl_insert
* ����������������*T����Ϣe��bool�ͱ���taller(�������Ƿ�����)
* �������ܣ���ƽ��������в���ڵ�e�������ֶ�����ƽ��
* ����ֵ���ɹ�����true�����򷵻�false
**/
bool avl_insert(BATree *T, Info e, bool *taller) {
	if ((*T) == NULL)
	{
		(*T) = (BATree)malloc(sizeof(BATNode));//����ռ�
		(*T)->data = e;
		(*T)->lchild = NULL;
		(*T)->rchild = NULL;
		(*T)->bflag = EH;
		*taller = true;
	}
	else {
		if (e.key == (*T)->data.key) {//�����йؼ���
			*taller = false;
			return false;
		}
		if (e.key < (*T)->data.key) {//����������
			if (!avl_insert(&((*T)->lchild), e, taller))return false;//����ʧ��
			if (*taller)//����ɹ�
				switch ((*T)->bflag) {
				case LH://��߸�
					LeftBalance(T);
					*taller = false;
					break;
				case EH://�ȸ�
					(*T)->bflag = LH;
					*taller = true;
					break;
				case RH://�ұ߸�
					(*T)->bflag = EH;
					*taller = false;
					break;
				}
		}
		else {//�������в���
			if (!avl_insert(&((*T)->rchild), e, taller))return false;//����ʧ��
			if (*taller)//����ɹ�
				switch ((*T)->bflag) {
				case LH://��߸�
					(*T)->bflag = EH;
					*taller = false;
					break;
				case EH://�ȸ�
					(*T)->bflag = RH;
					*taller = true;
					break;
				case RH://�ұ߸�
					RightBalance(T);
					*taller = false;
					break;
				}
		}
	}
	return true;
}


/**
* �������ƣ�avl_delete
* ����������������*T���ؼ���key��bool�ͱ���shorter(�������Ƿ�䰫)
* �������ܣ���ƽ���������ɾ���ڵ�e�������ֶ�����ƽ��
* ����ֵ���ɹ�����true�����򷵻�false
**/
bool avl_delete(BATree *T, int key, bool *shorter) {
	if ((*T) == NULL)return false;//������
	else if (key == (*T)->data.key) {
		BATree p;
		if ((*T)->lchild == NULL) {//��������
			p = (*T);
			(*T) = (*T)->rchild;
			free(p);
			*shorter = true;
		}
		else if ((*T)->rchild == NULL) {//��������
			p = (*T);
			(*T) = (*T)->lchild;
			free(p);
			*shorter = true;
		}
		else {//������������Ϊ��
			p = (*T)->lchild;
			while (p->rchild) {
				p = p->rchild;
			}
			(*T)->data = p->data;
			avl_delete(&((*T)->lchild), p->data.key, shorter);//�������ݹ�ɾ��ǰ���ڵ�
			if (*shorter) {
				switch ((*T)->bflag) {
				case LH:
					(*T)->bflag = EH;
					*shorter = true;
					break;
				case EH:
					(*T)->bflag = RH;
					*shorter = false;
					break;
				case RH:
					if ((*T)->rchild->bflag == EH) *shorter = false;//�Һ���ΪEH
					else *shorter = true;
					RightBalance(T);//��ƽ��
					break;
				}
			}
		}
	}
	else if (key<(*T)->data.key) {//�������м����ҽڵ�
		if (!avl_delete(&((*T)->lchild), key, shorter))return false;
		if (*shorter) {
			switch ((*T)->bflag) {
			case LH:
				(*T)->bflag = EH;
				*shorter = true;
				break;
			case EH:
				(*T)->bflag = RH;
				*shorter = false;
				break;
			case RH:
				if ((*T)->rchild->bflag == EH) *shorter = false;//�Һ���ΪEH
				else *shorter = true;
				RightBalance(T);//��ƽ��
				break;
			}
		}
	}
	else {//�������м����ҽڵ�
		if (!avl_delete(&((*T)->rchild), key, shorter))return false;
		if (*shorter) {
			switch ((*T)->bflag) {
			case LH:
				if ((*T)->lchild->bflag == EH) *shorter = false;//����ΪEH
				else *shorter = true;
				LeftBalance(T);//��ƽ��
				break;
			case EH:
				(*T)->bflag = LH;
				*shorter = false;
				break;
			case RH:
				(*T)->bflag = EH;
				*shorter = true;
				break;
			}
		}
	}
	return true;
}


/**
* �������ƣ�avl_traverse
* ����������������T
* �������ܣ��������
* ����ֵ����
**/
void avl_traverse(BATree T) {
	if (T == NULL)return;
	avl_traverse(T->lchild);
	printf("%3d", T->data.key);
	avl_traverse(T->rchild);
}


/**
* �������ƣ�avl_search
* ����������������T���ؼ���key,������*Tsub(ָ��Ҫ�ҵĽڵ�)
* �������ܣ��жϹؼ���Ϊkey�Ľڵ��Ƿ���T�еĳ�Ա
* ����ֵ���Ƿ���true�����򷵻�false
**/
bool avl_search(BATree T, int key, BATree *Tsub) {
	if (T == NULL)return false;
	if (T->data.key == key) {
		*Tsub = T;
		return true;
	}
	else if (key<T->data.key) {//����������
		if (avl_search(T->lchild, key, Tsub))return true;
	}
	else { if (avl_search(T->rchild, key, Tsub))return true; }//����������
	return false;
}