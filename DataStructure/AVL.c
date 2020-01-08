#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<stddef.h>
#include<stdbool.h>

#define LH 1      //左边高
#define EH 0      //等高
#define RH -1     //右边高

typedef struct Info{
	int key;//关键字
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
	int bflag;//二叉树状态
	int h;//高度
	struct BATNode *lchild, *rchild;
}BATNode, *BATree;

typedef void(*visit)(BATree T);
void L_Rotate(BATree *p);//左旋函数
void R_Rotate(BATree *p);//右旋函数
void LeftBalance(BATree *T);//对节点实现左平衡
void RightBalance(BATree *T);//对节点实现右平衡
void avl_init(BATree *T);//AVL树初始化
void avl_destroy(BATree *T);//AVL树销毁
bool avl_insert(BATree *T, Info e, bool *taller);//AVL树插入
bool avl_delete(BATree *T, int key, bool *shorter);//AVL树删除
void avl_traverse(BATree T);//AVL树遍历
bool avl_search(BATree T, int key, BATree *Tsub);//AVL树搜索
void set_init(BATree *T);//集合初始化
void set_destory(BATree *T);//集合销毁
bool set_insert(BATree *T, Info e, bool *taller);//集合插入
bool set_remove(BATree *T, int key, bool *shorter);//集合删除
void set_intersection(BATree T, BATree T1, BATree *T0);//求集合交集
void set_union(BATree *T, BATree T1);//求集合并集
void set_difference(BATree *T, BATree T1);//求集合差集
void set_size(BATree T1, BATree T);//求集合元素个数
bool set_member(BATree T, int key, BATree *Tsub);//判断是否为集合中成员
bool set_subset(BATree Tsub, BATree T1);//判断一个集合是否是另一个集合的子集
bool set_equal(BATree T, BATree T1);//判断两个集合是否相等
bool load_data(BATree *T, char *filename);//加载主要的树
bool save_data(BATree T, FILE *fp);//保存主要的树
void input_key(Info *data);//输入关键字
void output_key(BATree T);//输出关键字
void create(BATree *T);//创建树
void height(BATree T, int i);//求树高度
void gotoxy(int x, int y);//锁定位置
void showtree(BATree T, int x, int y, visit fp);//显示树图
void InOrderTraverse(BATree T);//中序遍历
void output_relation(BATree T1, BATree T);//输出关系
void output_likes(BATree T1, BATree L);//输出爱好
bool input_pinfo(BATree *T, int p_size);//随机输入人员信息
bool input_likes(BATree *L, int *l_size);//随机生成爱好树
void input_relation(BATree *T, int p_size, int l_size);//输入关系信息
void input_information(BATree *T, int size, int max, int min);//输入个人信息
void improve_sets(BATree T, BATree T0);//完善随机生成的各种集合
void improve_friends(BATree T, BATree T1, BATree T2);//完善好友集
void improve_fans(BATree T, BATree T1, BATree T2);//完善粉丝集
void improve_following(BATree T, BATree T1, BATree T2);//完善关注集
void indirect_friends_traverse(BATree T, BATree T1, BATree *T0);//二度好友遍历
void indirect_friends_insert(BATree T, BATree *T0);//二度好友插入
void set_init_fors(BATree *T);//对一个人的好友集，粉丝集，关注集，爱好集等
bool insert_name(BATree T);//为一个节点赋名
bool init_friends(BATree T);//初始化好友集
bool insert_friends(BATree T);//插入好友
bool remove_friends(BATree T);//删除好友
bool search_friends(BATree T);//搜索好友
bool traverse_friends(BATree T);//遍历好友
bool init_fans(BATree T);//初始化粉丝集
bool insert_fans(BATree T);//插入粉丝
bool remove_fans(BATree T);//删除粉丝
bool search_fans(BATree T);//搜索粉丝
bool traverse_fans(BATree T);//遍历粉丝
bool init_following(BATree T);//初始化关注集
bool insert_following(BATree T);//插入关注
bool remove_following(BATree T);//删除关注
bool search_following(BATree T);//搜索关注
bool traverse_following(BATree T);//遍历关注
bool init_likes(BATree T,BATree L);//初始化爱好集
bool insert_likes(BATree T, BATree L);//插入爱好
bool remove_likes(BATree T, BATree L);//删除爱好
bool search_likes(BATree T, BATree L);//搜索爱好
bool traverse_likes(BATree T, BATree L);//遍历爱好
bool load(BATree *T);//加载更多信息
bool traverse_relation(BATree T, FILE *fp);//遍历人与人之间的关系
bool save(BATree T);//保存更多信息
bool save_relation(BATree T, FILE *fp, int i);//保存人与人之间的关系

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
		printf("请输入操作序号:");
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
			    printf("销毁成功!\n");
			    break;
		    }
			case 3: {
			    Info e;
				input_key(&e);
			    if (set_insert(&T, e, &taller)) {
				    printf("插入成功!\n");
					height(T, 0);
			    }
			    else printf("插入失败!\n");
				break;
		    }
			case 4: {
				int key;
				printf("请输入关键字:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (set_remove(&T, key, &shorter)) {
					printf("删除成功!\n");
					height(T, 0);
				}
				else printf("删除失败!\n");
				break;
			}
			case 5: {
				char filename[20];
				BATree T1 = NULL;
				BATree T0 = NULL;
				printf("请输入文件名:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("加载成功!\n");
					set_intersection(T, T1, &T0);
					set_init(&T);
					T = T0;
					height(T, 0);
					printf("取交集成功!\n");
				}
				else printf("加载失败!\n");
				break;
			}
			case 6: {
				char filename[20];
				BATree T1 = NULL;
				printf("请输入文件名:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("加载成功!\n");
					set_union(&T, T1);
					height(T, 0);
					printf("取并集成功!\n");
				}
				else printf("加载失败!\n");
				break;
			}
			case 7: {
				char filename[20];
				BATree T1 = NULL;
				printf("请输入文件名:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("加载成功!\n");
					set_difference(&T, T1);
					height(T, 0);
					printf("取差成功!\n");
				}
				else printf("加载失败!\n");
				break;
			} 
			case 8: {
				int key;
				printf("请输入关键字:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (set_member(T, key, &Tsub)) printf("是成员!\n");
				else printf("不是成员!\n");
				break;
			}
			case 9: {
				char filename[20];
				BATree T1 = NULL;
				printf("请输入要判断此文件是否为子集的文件名:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					printf("加载成功!\n");
					if (set_subset(T, T1)) printf("是子集!\n");
					else printf("不是子集!\n");
				}
				else printf("加载失败!\n");
				break;
			} 
			case 10: {
				char filename[20];
				BATree T1 = NULL;
				printf("请输入要判断的文件名:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T1, filename)) {
					if (set_equal(T, T1)) printf("相等!\n");
					else printf("不相等!\n");
				}
				else printf("加载失败!\n");
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
				printf("请输入文件名:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if ((fp = fopen(filename, "wb")) == NULL) {
					printf("文件打开失败!\n");
					break;
				}
				if (save_data(T, fp)) printf("保存成功!\n");
				else printf("保存失败!\n");
				fclose(fp);
				break;
			}
			case 15: {
				char filename[20];
				printf("请输入文件名:");
				scanf("%s", filename);
				getchar();
				printf("\n");
				if (load_data(&T, filename)) printf("加载成功!\n");
				break; 
			} 
			case 16:{ 
				BATree T0;
				if (input_pinfo(&T, p_size)) printf("成功输入个人信息!\n");
				else {
					printf("输入个人信息失败!\n");
					break;
				}
			    if (input_likes(&L, &l_size)) printf("成功输入爱好信息!\n");
				else {
				    printf("输入爱好信息失败!\n");
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
				printf("请输入关键字:");
				scanf("%d", &key);
				getchar();
				if (!set_member(T, key, &T1)) {
					printf("没有找到此人!\n");
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
				printf("请输入其中一个人的关键字:");
				scanf("%d", &key1);
				getchar();
				printf("请输入另一人的关键字:");
				scanf("%d", &key2);
				getchar();
				if (!set_member(T, key1, &T1)) {
					printf("没有找到此人!\n");
					break;
				}
				if (!set_member(T, key2, &T2)) {
					printf("没有找到此人!\n");
					break;
				}
				set_intersection(T1->data.following, T2->data.following, &T0);
				if (T0) {
					output_relation(T0, T);
					break;
				}
				else {
					printf("没有共同关注!\n");
					break;
				}
			}
			case 19: {
				int key1, key2;
				BATree T1 = NULL, T2 = NULL, T0 = NULL;
				printf("请输入其中一个人的关键字:");
				scanf("%d", &key1);
				getchar();
				printf("请输入另一人的关键字:");
				scanf("%d", &key2);
				getchar();
				if (!set_member(T, key1, &T1)) {
					printf("没有找到此人!\n");
					break;
				}
				if (!set_member(T, key2, &T2)) {
					printf("没有找到此人!\n");
					break;
				}
				set_intersection(T1->data.likes, T2->data.likes, &T0);
				if (T0) {
					output_likes(T0, L);
					break;
				}
				else {
					printf("没有共同爱好!\n");
					break;
				}
			}
			case 20: {
				int key;
				BATree T1 = NULL, T0 = NULL;
				printf("请输入关键字:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (!set_member(T, key, &T1)) {
					printf("没有找到此人!\n");
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
				printf("请选择对好友集执行的操作：\n");
				printf("0 初始化  1 增加好友  2 删除好友  3 查找好友  4 遍历好友\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_friends(T); break;
				case 1:insert_friends(T); break;
				case 2:remove_friends(T); break;
				case 3:search_friends(T); break;
				case 4:traverse_friends(T); break;
				default:printf("错误输入！");
				}
				getchar();
				break;
			}
			case 24: {
				printf("请选择对粉丝集执行的操作：\n");
				printf("0 初始化  1 增加粉丝  2 删除粉丝  3 查找粉丝  4 遍历粉丝\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_fans(T); break;
				case 1:insert_fans(T); break;
				case 2:remove_fans(T); break;
				case 3:search_fans(T); break;
				case 4:traverse_fans(T); break;
				default:printf("错误输入！");
				}
				getchar();
				break;
			}
			case 25: {
				printf("请选择对关注集执行的操作：\n");
				printf("0 初始化  1 增加关注  2 删除关注  3 查找关注  4 遍历关注\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_following(T); break;
				case 1:insert_following(T); break;
				case 2:remove_following(T); break;
				case 3:search_following(T); break;
				case 4:traverse_following(T); break;
				default:printf("请输入正确的操作符号！");
				}
				getchar();
				break;
			}
			case 26: {
				printf("请选择对爱好集执行的操作：\n");
				printf("0 初始化  1 增加爱好  2 删除爱好  3 查找爱好  4 遍历爱好\n");
				int i;
				scanf("%d", &i);
				switch (i)
				{
				case 0:init_likes(T,L); break;
				case 1:insert_likes(T,L); break;
				case 2:remove_likes(T,L); break;
				case 3:search_likes(T,L); break;
				case 4:traverse_likes(T,L); break;
				default:printf("请输入正确的操作符号！");
				}
				getchar();
				break;
			}
			case 27: {
				input_likes(&L, &l_size);
				if (load(&T))
					printf("加载成功！\n");
				else
					printf("加载失败！\n");
				getchar();
				break;
			}
			case 28: {
				if (save(T))
					printf("保存成功！\n");
				else
					printf("保存失败！\n");
				getchar();
				break;
			}
			case 29: {
				avl_init(&T);
				break;
			}
			case 30: {
				avl_destroy(&T);
				printf("销毁成功!\n");
				break;
			}
			case 31: {
				int key;
				printf("请输入关键字:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (avl_search(T, key, &Tsub)) printf("元素在树中!\n");
				else printf("元素不在树中!\n");
				break;
			}
			case 32: {
				Info e;
				input_key(&e);
				if (avl_insert(&T, e, &taller)) {
					printf("插入成功!\n");
					height(T, 0);
				}
				else printf("插入失败!\n");
				break;
			}
			case 33: {
				int key;
				printf("请输入关键字:");
				scanf("%d", &key);
				getchar();
				printf("\n");
				if (avl_delete(&T, key, &shorter)) {
					printf("删除成功!\n");
					height(T, 0);
				}
				else printf("删除失败!\n");
				break;
			}
			case 34: {
				avl_traverse(T);
				break;
			}
			default:
				printf("输入错误!\n");
            }
			getchar();
			system("cls");
			}while (i);
    printf("\n--------------------   欢迎下次使用!   --------------------\n");
	return 0;
 }




 /**
 * 函数名称：L_Rotate
 * 函数参数：二叉树的根*p
 * 函数功能：对以*p为根的二叉树作左旋处理
 * 返回值：新的树根结点
 **/
void L_Rotate(BATree *p) {
	BATree temp = NULL;
	temp = (*p)->rchild;//temp指向p的右子树根节点
	(*p)->rchild = temp->lchild;//temp左子树作为p的右子树
	temp->lchild = (*p);
	(*p) = temp;//*p指向新的根节点
}


/**
* 函数名称：R_Rotate
* 函数参数：二叉树的根*p
* 函数功能：对以*p为根的二叉树作右旋处理
* 返回值：新的树根结点
**/
void R_Rotate(BATree *p) {
	BATree temp = NULL;
	temp = (*p)->lchild;//temp指向p的左子树根节点
	(*p)->lchild = temp->rchild;//temp右子树作为p的左子树
	temp->rchild = (*p);
	(*p) = temp;//*p指向新的根节点
}


/**
* 函数名称：LeftBalance
* 函数参数：二叉树*T
* 函数功能：二叉树左子树过高，对其做左平衡
* 返回值：新二叉树*T
**/
void LeftBalance(BATree *T) {
	BATree l = NULL, r = NULL;
	l = (*T)->lchild;
	switch (l->bflag){
	case LH://LL
		(*T)->bflag = EH;
		l->bflag = EH;
		R_Rotate(T);//右旋T
		break;
	case EH:
		(*T)->bflag = LH;
		l->bflag = RH;
		R_Rotate(T);//右旋T
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
		L_Rotate(&((*T)->lchild));//左旋根左孩子
		R_Rotate(T);//右旋T
		break;
	}
}


/**
* 函数名称：RightBalance
* 函数参数：二叉树*T
* 函数功能：二叉树右子树过高，对其做右平衡
* 返回值：新二叉树*T
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
		R_Rotate(&((*T)->rchild));//右旋根右孩子
		L_Rotate(T);//左旋根
		break;
	case EH:
		(*T)->bflag = RH;
		r->bflag = LH;
		L_Rotate(T);//左旋根
		break;
	case RH://RR
		(*T)->bflag = EH;
		r->bflag = EH;
		L_Rotate(T);//左旋根
		break;
	}
}


/**
* 函数名称：set_init
* 函数参数：二叉树*T
* 函数功能：初始化二叉树
* 返回值：初始化后的二叉树
**/
void set_init(BATree *T) {
	 *T = NULL;
	 printf("初始化成功!\n");
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
* 函数名称：set_destory
* 函数参数：二叉树*T
* 函数功能：销毁二叉树
* 返回值：无
**/
void set_destory(BATree *T) {
	if ((*T) != NULL) {
		set_destory(&((*T)->lchild));
		set_destory(&((*T)->rchild));
		free(*T);
	}
}


/**
* 函数名称：set_insert
* 函数参数：二叉树*T，信息e，bool型变量taller(二叉树是否增高)
* 函数功能：在平衡二叉树中插入节点e，并保持二叉树平衡
* 返回值：成功返回true，否则返回false
**/
bool set_insert(BATree *T, Info e, bool *taller) {
	if ((*T)==NULL)
	{
		(*T) = (BATree)malloc(sizeof(BATNode));//分配空间
		(*T)->data = e;
		(*T)->lchild = NULL;
		(*T)->rchild = NULL;
		(*T)->bflag = EH;
		*taller = true;//变高为true
	}
	else {
		if (e.key == (*T)->data.key) {//含已有关键字
			*taller = false;
			return false;
		}
		if (e.key < (*T)->data.key) {//左子树插入
			if (!set_insert(&((*T)->lchild), e, taller))return false;//插入失败
			if (*taller)//插入成功
				switch ((*T)->bflag) {
				case LH://左边高
					LeftBalance(T);
					*taller = false;
					break;
				case EH://等高
					(*T)->bflag = LH;
					*taller = true;
					break;
				case RH://右边高
					(*T)->bflag = EH;
					*taller = false;
					break;
				}
		}
		else {//右子树中插入
			if (!set_insert(&((*T)->rchild), e, taller))return false;//插入失败
			if (*taller)//插入成功
				switch ((*T)->bflag) {
				case LH://左边高
					(*T)->bflag = EH;
					*taller = false;
					break;
				case EH://等高
					(*T)->bflag = RH;
					*taller = true;
					break;
				case RH://右边高
					RightBalance(T);
					*taller = false;
					break;
				}
		}
	}
	return true;
}


/**
* 函数名称：set_remove
* 函数参数：二叉树*T，关键字key，bool型变量shorter(二叉树是否变矮)
* 函数功能：在平衡二叉树中删除节点e，并保持二叉树平衡
* 返回值：成功返回true，否则返回false
**/
bool set_remove(BATree *T, int key, bool *shorter) {
	if ((*T) == NULL)return false;//不存在
	else if (key == (*T)->data.key) {
		BATree p;
		if ((*T)->lchild == NULL) {//左子树空
			p = (*T);
			(*T) = (*T)->rchild;
			free(p);
			*shorter = true;
		}
		else if ((*T)->rchild == NULL) {//右子树空
			p = (*T);
			(*T) = (*T)->lchild;
			free(p);
			*shorter = true;
		}
		else {//左右子树都不为空
			p = (*T)->lchild;
			while (p->rchild) {
				p = p->rchild;
			}
			(*T)->data = p->data;
			set_remove(&((*T)->lchild), p->data.key, shorter);//左子树递归删除前驱节点
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
					if ((*T)->rchild->bflag == EH) *shorter = false;//当前节点右孩子状态为EH
					else *shorter = true;
					RightBalance(T);//右平衡
					break;
				}
			}
		}
	}
	else if (key<(*T)->data.key) {//左子树中继续找节点
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
				if ((*T)->rchild->bflag == EH) *shorter = false;//节点右孩子状态EH
				else *shorter = true;
				RightBalance(T);//右平衡
				break;
			}
		}
	}
	else {//右子树中继续找节点
		if (!set_remove(&((*T)->rchild), key, shorter))return false;
		if (*shorter) {
			switch ((*T)->bflag) {
			case LH:
				if ((*T)->lchild->bflag == EH) *shorter = false;//节点左孩子状态为EH
				else *shorter = true;
				LeftBalance(T);//左平衡
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
* 函数名称：set_intersection
* 函数参数：二叉树T，二叉树T1，二叉树*T0
* 函数功能：求T与T1的交集
* 返回值：交集T0
**/
void set_intersection(BATree T, BATree T1, BATree *T0) {
	if (T1 == NULL)return;
	if (set_member(T, T1->data.key, &Tsub)) set_insert(T0, T1->data, &taller);//若元素为交集里的元素
	set_intersection(T, T1->lchild, T0);
	set_intersection(T, T1->rchild, T0);
}


/**
* 函数名称：set_union
* 函数参数：二叉树*T，二叉树T1
* 函数功能：求T与T1的并集
* 返回值：并集T
**/
void set_union(BATree *T, BATree T1) {
	if (T1 == NULL)return;
	set_insert(T, T1->data, &taller);//插入
	set_union(T, T1->lchild);
	set_union(T, T1->rchild);
}


/**
* 函数名称：set_difference
* 函数参数：二叉树*T，二叉树T1
* 函数功能：求T与T1的差
* 返回值：差T
**/
void set_difference(BATree *T, BATree T1) {
	if (*T == NULL || T1 == NULL)return;
	set_remove(T, T1->data.key, &shorter);//删除T1
	set_difference(T, T1->lchild);
	set_difference(T, T1->rchild);
}


/**
* 函数名称：set_size
* 函数参数：二叉树T1，二叉树T
* 函数功能：求T的个数
* 返回值：无
**/
void set_size(BATree T1, BATree T) {
	if (T1 == NULL)return;
	set_size(T1->lchild, T);
	if (set_member(T, T1->data.key, &Tsub))count++;//数目加1
	set_size(T1->rchild, T);
}


/**
* 函数名称：set_member
* 函数参数：二叉树T，关键字key,二叉树*Tsub(指向要找的节点)
* 函数功能：判断关键字为key的节点是否是T中的成员
* 返回值：是返回true，否则返回false
**/
bool set_member(BATree T, int key, BATree *Tsub) {
	if (T == NULL)return false;
	if (T->data.key == key) {
		*Tsub = T;
		return true;
	}
	else if (key<T->data.key) {//左子树查找
		if (set_member(T->lchild, key, Tsub))return true;
	}
	else {if (set_member(T->rchild, key, Tsub))return true; }//右子树查找
	return false;
}


/**
* 函数名称：set_subset
* 函数参数：二叉树T，二叉树T1
* 函数功能：判断T1是否是T的子集
* 返回值：是返回true，否则返回false
**/
bool set_subset(BATree T, BATree T1) {
	if (T1 == NULL)return true;
	if (set_member(T, T1->data.key, &Tsub)) {
		if (!set_subset(T, T1->lchild)) return false;//有成员不被T包含
		if (!set_subset(T, T1->rchild)) return false;//有成员不被T包含
		return true;
	}
	else return false;
}


/**
* 函数名称：set_equal
* 函数参数：二叉树T，二叉树T1
* 函数功能：判断T1是否与T相等
* 返回值：是返回true，否则返回false
**/
bool set_equal(BATree T, BATree T1) {
	if (!set_subset(T, T1))return false;//T是否是T1子集
	if (!set_subset(T1, T))return false;//T1是否是T的子集
	return true;
}


/**
* 函数名称：load_data
* 函数参数：二叉树*T，文件名filename
* 函数功能：加载数据
* 返回值：成功返回true，否则返回false
**/
bool load_data(BATree *T, char *filename) {
	BATree p;
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL) {
		printf("打开文件失败!\n");
		return false;
	}
	while (!feof(fp)) {
		if ((p = (BATree)malloc(sizeof(BATNode))) == NULL) {//分配空间
			printf("储存空间不足!\n");
			fclose(fp);
			return false;
		}
		if (fread(p, sizeof(BATNode), 1, fp) != 1) {
			free(p);//读完
			break;
		}
		set_insert(T, p->data, &taller);//插入
	}
	fclose(fp);
	height(*T, 0);
	return true;
}


/**
* 函数名称：save_data
* 函数参数：二叉树T，文件指针fp
* 函数功能：存储数据
* 返回值：成功返回true，否则返回false
**/
bool save_data(BATree T, FILE *fp) {
	if (T) {
		if (fwrite(T, sizeof(BATNode), 1, fp) != 1) {//写入文件
			printf("保存失败!\n");
			fclose(fp);
			return false;
		}
	}
	else return true;
	if (!save_data(T->lchild, fp))return false;//递归调用
	if (!save_data(T->rchild, fp))return false;
	return true;
}


/**
* 函数名称：input_key
* 函数参数：数据指针data
* 函数功能：输入数据的关键字
* 返回值：无
**/
void input_key(Info *data) {
	printf("请输入关键字:");
	scanf("%d", &(data->key));//输入关键字
	getchar();
	printf("\n");
}


/**
* 函数名称：height
* 函数参数：二叉树T，变量i
* 函数功能：计算二叉树高
* 返回值：二叉树高
**/
void height(BATree T, int i) {
	if(T) T->h = i + 1;
	else return;
	height(T->lchild, T->h);//递归求高度
	height(T->rchild, T->h);
}


/**
* 函数名称：output_key
* 函数参数：二叉树T
* 函数功能：输出关键字
* 返回值：无
**/
void output_key(BATree T) {
	printf("%d", T->data.key);//输出关键字
}


/**
* 函数名称：create
* 函数参数：二叉树*T
* 函数功能：创建二叉树
* 返回值：无
**/
void create(BATree *T) {
	char s;
	Info *e;
	if ((e = (Info *)malloc(sizeof(Info))) == NULL) {//分配空间
		printf("存储空间不足!\n");
		return;
	}
	input_key(e);
	if (set_insert(T, *e, &taller)) {//插入
		printf("创建成功，是否要继续?(y/n):");
		while (1) {
			s = getchar();
			getchar();
			printf("\n");
			switch (s) {
			case 'y':
				create(T);//递归调用
				height(*T, 0);
				return;
			case 'n':
				printf("完成创建!\n");
				return;
			default:
				printf("输入错误，请重新输入:");
			}
		}
	}
	else printf("创建失败!\n");
	return;
}


/**
* 函数名称：gotoxy
* 函数参数：变量x，y
* 函数功能：设置光标位置
* 返回值：无
**/
void gotoxy(int x, int y) {
	COORD coord = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


/**
* 函数名称：showtree
* 函数参数：二叉树T，变量x，y，output_t
* 函数功能：显示树图
* 返回值：无
**/
void showtree(BATree T, int x, int y, visit output_t) {
	if (T) {
		gotoxy(x + x_move, y + y_move);
		output_t(T);//输出
	}
	else return;
	if (T->lchild || T->rchild) {//左右孩子都有
		gotoxy(x + x_move, y + y_move + 1);
		printf(" | ");
		if (T->lchild) {//左孩子存在
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
			showtree(T->lchild, x - j, y + 4, output_t);//递归调用
		}
		if (T->rchild) {//右孩子存在
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
			showtree(T->rchild, x + j, y + 4, output_t);//递归调用
		}
	}
}


/**
* 函数名称：InOrderTraverse
* 函数参数：二叉树T
* 函数功能：中序遍历
* 返回值：无
**/
void InOrderTraverse(BATree T){
	if (T == NULL)return;
	InOrderTraverse(T->lchild);
	printf("%3d%s%s ", T->data.key, T->data.lastname, T->data.firstname);//输出姓名信息
	InOrderTraverse(T->rchild);
}


/**
* 函数名称：output_relation
* 函数参数：二叉树T1,二叉树T
* 函数功能：输出关系
* 返回值：无
**/
void output_relation(BATree T1, BATree T) {
	if (T1 == NULL)return;
	output_relation(T1->lchild, T);
	if (set_member(T, T1->data.key, &Tsub)) printf("%3d%s%s",Tsub->data.key,Tsub->data.lastname,Tsub->data.firstname);//输出信息
	output_relation(T1->rchild,T);
}


/**
* 函数名称：output_likes
* 函数参数：二叉树T1,二叉树T
* 函数功能：输出爱好
* 返回值：无
**/
void output_likes(BATree T1, BATree T) {
	if (T1 == NULL)return;
	output_likes(T1->lchild, T);
	if (set_member(T, T1->data.key, &Tsub)) printf("%3d%s", Tsub->data.key, Tsub->data.like);//输出信息
	output_likes(T1->rchild, T);
}


/**
* 函数名称：input_pinfo
* 函数参数：二叉树*T,变量p_size
* 函数功能：输入人信息
* 返回值：无
**/
bool input_pinfo(BATree *T, int p_size) {
	Info *e;
	FILE *fp1, *fp2;
	int num;
	if ((fp1 = fopen("lastname.txt", "r")) == NULL) {
		printf("打开lastname失败!\n");
		return false;
	}
	if ((fp2 = fopen("firstname.txt", "r")) == NULL) {
		printf("打开firstname失败!\n");
		return false;
	}
	for (num = 0; num <= p_size; num++) {//随机生成大数据集
		if ((e = (Info *)malloc(sizeof(Info))) == NULL) {
			printf("存储空间不足!\n");
			fclose(fp1);//关闭指针
			fclose(fp2);
			return false;
		}
		if (feof(fp1)) {
			fclose(fp1);
			if ((fp1 = fopen("lastname.txt", "r")) == NULL) {
				printf("打开lastname失败!\n");
				return false;
			}
		}
		else {
			if (fgets(e->lastname, 3, fp1) == NULL) {//读取姓
				free(e);
				break;
			}
		}
		if (feof(fp2)) {
			fclose(fp2);
			if ((fp2 = fopen("firstname.txt", "r")) == NULL){
				printf("打开firstname失败!\n");
				return false;
			}
		}
		else {
			if (fgets(e->firstname, 5, fp2) == NULL) {//读取名
				free(e);
				break;
			}
		}
		e->key = num;//关键字分配
		e->friends = NULL;//置空
		e->fans = NULL;
		e->following = NULL;
		e->likes = NULL;
		set_insert(T, *e, &taller);//插入
	}
	fclose(fp1);
	fclose(fp2);
	height(*T, 0);
	return true;
}
 

/**
* 函数名称：input_likes
* 函数参数：二叉树T,变量l_size
* 函数功能：输入爱好
* 返回值：无
**/
bool input_likes(BATree *T, int *l_size) {
	Info *e;
	FILE *fp;
	int num = 0;
	if ((fp = fopen("likes.txt", "r")) == NULL) {
		printf("打开likes失败!\n");
		return false;
	}
	while (!feof(fp)) {
		if ((e = (Info *)malloc(sizeof(Info))) == NULL) {//分配空间
			printf("存储空间不足!\n");
			fclose(fp);
			return false;
		}
		if (fgets(e->like, 5, fp) == NULL) {//读取爱好
			free(e);
			break;
		}
		e->key = num++;//分配关键字
		e->friends = NULL;
		e->fans = NULL;
		e->following = NULL;
		e->likes = NULL;
		set_insert(T, *e, &taller);//插入
	}
	*l_size = num;
	fclose(fp);//关闭文件指针
	height(*T, 0);
	return true;
}


/**
* 函数名称：input_relation
* 函数参数：二叉树T,变量p_size,l_size
* 函数功能：输入关系
* 返回值：无
**/
void input_relation(BATree *T, int p_size, int l_size) {
	if (*T == NULL)return;
	input_information(&((*T)->data.friends), p_size, 80, 20);//随机生成好友
	input_information(&((*T)->data.fans), p_size, 60, 10);//随机生成粉丝
	input_information(&((*T)->data.following), p_size, 30, 5);//随机生成关注
	input_information(&((*T)->data.likes), l_size, 8, 2);//随机生成爱好
	input_relation(&((*T)->lchild), p_size, l_size);//递归
	input_relation(&((*T)->rchild), p_size, l_size);
}


/**
* 函数名称：input_information
* 函数参数：二叉树T，变量size，max，min
* 函数功能：输入key
* 返回值：无
**/
void input_information(BATree *T, int size, int max, int min) {
	int i;
	Info *e;
	for (i = 0; i <= rand() % max + min; i++) {//随机分配
		if ((e = (Info *)malloc(sizeof(Info))) == NULL) {
			printf("存储空间不足!\n");
			break;
		}
		e->key = rand() % size;
		e->friends = NULL;//置空
		e->fans = NULL;
		e->following = NULL;
		e->likes = NULL;
		set_insert(T, *e, &taller);//插入信息
	}
}


/**
* 函数名称：improve_sets
* 函数参数：二叉树T,二叉树T0
* 函数功能：完善集合
* 返回值：无
**/
void improve_sets(BATree T, BATree T0) {
	if (T0 == NULL)return;
	improve_sets(T, T0->lchild);//递归
	improve_friends(T, T0, T0->data.friends);//完善好友
	improve_fans(T, T0, T0->data.fans);//完善粉丝
	improve_following(T, T0, T0->data.following);//完善关注
	set_remove(&T0->data.friends, T0->data.key, &shorter);//删除自己
	set_remove(&T0->data.fans, T0->data.key, &shorter);//删除自己
	set_remove(&T0->data.following, T0->data.key, &shorter);//删除自己
	improve_sets(T, T0->rchild);
}


/**
* 函数名称：improve_friends
* 函数参数：二叉树T1,二叉树T1，二叉树T2
* 函数功能：完善好友集，完善好友与好友之间的关系
* 返回值：无
**/
void improve_friends(BATree T, BATree T1, BATree T2) {
	if (T2 == NULL)return;
	BATree T3;
	set_member(T, T2->data.key, &Tsub);
	if (!set_member(Tsub->data.friends, T1->data.key, &T3)) set_insert(&Tsub->data.friends, T1->data, &taller);//使得好友是双向的
	improve_friends(T, T1, T2->lchild);//递归
	improve_friends(T, T1, T2->rchild);
}


/**
* 函数名称：improve_fans
* 函数参数：二叉树T1,二叉树T1，二叉树T2
* 函数功能：完善粉丝的关注集
* 返回值：无
**/
void improve_fans(BATree T, BATree T1, BATree T2) {
	if (T2 == NULL)return;
	BATree T3;
	set_member(T, T2->data.key, &Tsub);
	if (!set_member(Tsub->data.following, T1->data.key, &T3)) set_insert(&Tsub->data.following, T1->data, &taller);//自己没有在粉丝的关注人集里
	improve_fans(T, T1, T2->lchild);//递归
	improve_fans(T, T1, T2->rchild);
}


/**
* 函数名称：improve_following
* 函数参数：二叉树T1,二叉树T1，二叉树T2
* 函数功能：完善关注的粉丝集
* 返回值：无
**/
void improve_following(BATree T, BATree T1, BATree T2) {
	if (T2 == NULL)return;
	BATree T3;
	set_member(T, T2->data.key, &Tsub);
	if (!set_member(Tsub->data.fans, T1->data.key, &T3)) set_insert(&Tsub->data.fans, T1->data, &taller);//自己没有在关注人的粉丝集里
	improve_following(T, T1, T2->lchild);//递归
	improve_following(T, T1, T2->rchild);
}


/**
* 函数名称：indirect_friends_traverse
* 函数参数：二叉树T1,二叉树T1，二叉树*T0
* 函数功能：二度好友遍历
* 返回值：无
**/
void indirect_friends_traverse(BATree T, BATree T1, BATree *T0) {
	if (T1 == NULL)return;
	set_member(T, T1->data.key, &Tsub);
	indirect_friends_insert(Tsub->data.friends, T0);//插入所有连接好友
	indirect_friends_traverse(T, T1->lchild, T0);//遍历
	indirect_friends_traverse(T, T1->rchild, T0);
}


/**
* 函数名称：indirect_friends_insert
* 函数参数：二叉树T,二叉树*T0
* 函数功能：二度好友输入
* 返回值：无
**/
void indirect_friends_insert(BATree T, BATree *T0) {
	if (T == NULL)return;
	set_insert(T0, T->data, &taller);//插入
	indirect_friends_insert(T->lchild, T0);
	indirect_friends_insert(T->rchild, T0);
}


/**
* 函数名称：insert_name
* 函数参数：二叉树T
* 函数功能：插入姓名
* 返回值：无
**/
bool insert_name(BATree T) {
	int key;
	BATree T1;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	printf("请输入姓：");
	scanf("%s", T1->data.lastname);
	printf("请输入名：");
	scanf("%s", T1->data.firstname);
	printf("插入成功！");
	return true;
}


/**
* 函数名称：traverse_del_friends
* 函数参数：二叉树T，T1
* 函数功能：完善初始化好友集
* 返回值：无
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
* 函数名称：init_friends
* 函数参数：二叉树T
* 函数功能：输出化好友集
* 返回值：无
**/
bool init_friends(BATree T) {
	int key;
	BATree T1;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	set_init_fors(&(T1->data.friends));//初始化好友集
	traverse_del_friends(T, T1);//将其他
}


/**
* 函数名称：insert_friends
* 函数参数：二叉树T
* 函数功能：插入好友
* 返回值：无
**/
bool insert_friends(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！\n");
		return false;
	}
	if (T1->data.friends == NULL)//未初始化
	{
		printf("%d的好友集未初始化！\n", key1);
		return false;
	}
	if (T1->data.friends == NULL)
		printf("%d当前无好友！\n", key1);
	else
	{
		printf("%d当前好友有：", key1);
		output_relation(T1->data.friends, T);//输出好友
		printf("\n");
	}
	printf("请输入要插入的好友关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (T2->data.friends == NULL)
	{
		printf("%d的好友集未初始化！\n", T2->data.key);
		return false;
	}
	if (!set_member(T1->data.friends, key2, &T0))//不是好友
	{
		Info data1, data2;
		data1.key = key2;
		data2.key = key1;
		set_insert(&(T1->data.friends), data1, &taller);//T1好友插入T2
		set_insert(&(T2->data.friends), data2, &taller);//T2好友插入T1
		printf("%d与%d互相成为好友！\n", key1, key2);
		return true;
	}
	printf("该好友已存在！");
	return false;
}


/**
* 函数名称：remove_friends
* 函数参数：二叉树T
* 函数功能：删除好友
* 返回值：无
**/
bool remove_friends(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.friends == NULL)//未初始化
	{
		printf("%d的好友集未初始化！\n", key1);
		return false;
	}
	if (T1->data.friends == NULL)
		printf("%d当前无好友！\n", key1);
	else
	{
		printf("%d当前好友有：", key1);
		output_relation(T1->data.friends, T);//输出好友
		printf("\n");
	}
	printf("请输入要删除的好友关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.friends == NULL)//好友本就不存在
	{
		printf("该好友不存在！");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (set_member(T1->data.friends, key2, &T0))//T2是T1好友
	{
		set_remove(&(T1->data.friends), key2, &shorter);//T1好友集删除T2
		set_remove(&(T2->data.friends), key1, &shorter);//T2好友集删除T1
		printf("%d与%d互删成功！\n", key1, key2);
		return true;
	}
	printf("该好友不存在！");
	return false;
}


/**
* 函数名称：search_friends
* 函数参数：二叉树T
* 函数功能：搜索好友
* 返回值：无
**/
bool search_friends(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	printf("请输入好友关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))//成员不存在
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.friends == NULL)//无好友
	{
		printf("%d无好友！\n", key1);
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (set_member(T1->data.friends, key2, &T0))//是好友
	{
		printf("%d是%d的好友！\n", key2, key1);
		return true;
	}
	printf("%d不是%d的好友！\n", key2, key1);
	return false;
}


/**
* 函数名称：traverse_friends
* 函数参数：二叉树T
* 函数功能：遍历好友
* 返回值：无
**/
bool traverse_friends(BATree T) {
	int key;
	BATree T1;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.friends == NULL)//未初始化
	{
		printf("%d的好友集未初始化！\n", T1->data.key);
		return false;
	}
	if (T1->data.friends == NULL)
		printf("%d当前无好友！\n", key);
	else
	{
		printf("%d当前的好友有：", key);
		output_relation(T1->data.friends,T);//输出好友
		printf("\n");
	}
}


/**
* 函数名称：traverse_del_following
* 函数参数：二叉树T，T1
* 函数功能：完善初始化粉丝集
* 返回值：无
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
* 函数名称：init_fans
* 函数参数：二叉树T
* 函数功能：初始化粉丝集
* 返回值：无
**/
bool init_fans(BATree T) {
	int key;
	BATree T1;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！\n");
		return false;
	}
	set_init_fors(&(T1->data.fans));//初始化
	traverse_del_following(T, T1);
}


/**
* 函数名称：insert_fans
* 函数参数：二叉树T
* 函数功能：插入粉丝
* 返回值：无
**/
bool insert_fans(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！\n");
		return false;
	}
	if (T1->data.fans == NULL)//未初始化
	{
		printf("%d的粉丝集未初始化！\n", key1);
		return false;
	}
	if (T1->data.fans == NULL) printf("%d当前无粉丝！\n", key1);
	else
	{
		printf("%d当前粉丝有：", key1);
		output_relation(T1->data.fans,T);//输出粉丝
		printf("\n");
	}
	printf("请输入要插入的粉丝关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！\n");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！\n");
		return false;
	}
	if (!set_member(T1->data.fans, key2, &T0))//T2不是T1粉丝
	{
		Info data1;
		Info data2;
		data1.key = key2;
		data2.key = key1;
		set_insert(&(T1->data.fans), data1, &taller);//T2插入T1粉丝集
		set_insert(&(T2->data.following), data2, &taller);//T1插入T2关注集
		printf("插入成功！\n");
		return true;
	}
	printf("该粉丝已存在！\n");
	return false;
}


/**
* 函数名称：remove_fans
* 函数参数：二叉树T
* 函数功能：删除粉丝
* 返回值：无
**/
bool remove_fans(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！\n");
		return false;
	}
	if (T1->data.fans == NULL)//未初始化
	{
		printf("%d的粉丝集未初始化！\n", key1);
		return false;
	}
	if (T1->data.fans == NULL)
		printf("%d当前无粉丝！\n", key1);
	else
	{
		printf("%d当前的粉丝有：", key1);
		output_relation(T1->data.fans, T);//输出粉丝
		printf("\n");
	}
	printf("请输入要删除的粉丝关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (set_member(T1->data.fans, key2, &T0))
	{
		set_remove(&(T1->data.fans), key2,&shorter);//将T2从T1粉丝集里删除
		set_remove(&(T2->data.following), key1, &shorter);//将T1从T2关注集里删除
		printf("删除成功！\n");
		return true;
	}
	printf("该粉丝不存在！\n");
	return false;
}


/**
* 函数名称：search_fans
* 函数参数：二叉树T
* 函数功能：搜索粉丝
* 返回值：无
**/
bool search_fans(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！\n");
		return false;
	}
	if (T1->data.fans == NULL)//无粉丝
	{
		printf("%d无粉丝！\n", key1);
		return false;
	}
	printf("请输入要查找的粉丝关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (set_member(T1->data.fans, key2, &T0))//是成员
	{
		printf("%d是%d的粉丝！\n", key2, key1);
		return true;
	}
	printf("%d不是%d的粉丝！\n", key2, key1);
	return false;
}


/**
* 函数名称：traverse_fans
* 函数参数：二叉树T
* 函数功能：遍历粉丝
* 返回值：无
**/
bool traverse_fans(BATree T) {
	int key;
	BATree T1;
	printf("请输入成员编号：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.fans == NULL)//未初始化
	{
		printf("%d的粉丝集未初始化！\n", T1->data.key);
		return false;
	}
	if (T1->data.fans == NULL)
		printf("%d当前无粉丝！\n", key);
	else
	{
		printf("%d当前粉丝有：", key);
		output_relation(T1->data.fans, T);//输出粉丝
		printf("\n");
	}
}


/**
* 函数名称：traverse_del_fans
* 函数参数：二叉树T，T1
* 函数功能：完善初始化关注集
* 返回值：无
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
* 函数名称：init_following
* 函数参数：二叉树T
* 函数功能：初始化关注集
* 返回值：无
**/
bool init_following(BATree T) {
	int key;
	BATree T1;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	set_init_fors(&(T1->data.following));//初始化
	traverse_del_fans(T, T1);
}


/**
* 函数名称：insert_following
* 函数参数：二叉树T
* 函数功能：插入关注
* 返回值：无
**/
bool insert_following(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.following == NULL)//未初始化
	{
		printf("%d的粉丝集未初始化！\n", key1);
		return false;
	}
	if (T1->data.following == NULL) printf("%d当前无关注！\n", key1);
	else
	{
		printf("%d当前关注有：", key1);
		output_relation(T1->data.following,T);//输出关注
		printf("\n");
	}
	printf("请输入要插入的关注关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (!set_member(T1->data.following, key2, &T0))//不是成员
	{
		Info data1;
		Info data2;
		data1.key = key2;
		data2.key = key1;
		set_insert(&(T1->data.following), data1,&taller);//将T2插入T1的关注集
		set_insert(&(T2->data.fans), data2, &taller);//将T1插入T2的粉丝集
		printf("插入成功！");
		return true;
	}
	printf("该关注已存在！");
	return false;
}


/**
* 函数名称：remove_following
* 函数参数：二叉树T
* 函数功能：删除关注
* 返回值：无
**/
bool remove_following(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.following == NULL)//未初始化
	{
		printf("%d的粉丝集未初始化！\n", key1);
		return false;
	}
	if (T1->data.following == NULL)
		printf("%d当前无关注！\n", key1);
	else
	{
		printf("%d当前关注有：", key1);
		output_relation(T1->data.following,T);//输出关注
		printf("\n");
	}
	printf("请输入要取消关注的成员关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (set_member(T1->data.following, key2, &T0))
	{
		set_remove(&(T1->data.following), key2,&shorter);//将T2从T1关注集删除
		set_remove(&(T2->data.fans), key1, &shorter);//将T1从T2粉丝集删除
		printf("删除成功！");
		return true;
	}
	printf("该关注不存在！");
	return false;
}


/**
* 函数名称：search_following
* 函数参数：二叉树T
* 函数功能：搜索关注
* 返回值：无
**/
bool search_following(BATree T) {
	int key1, key2;
	BATree T1, T2, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key1);
	if (!set_member(T, key1, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.following == NULL)//无关注
	{
		printf("%d无关注！\n", key1);
		return false;
	}
	printf("请输入要查找的关注关键字：");
	scanf("%d", &key2);
	if (!set_member(T, key2, &T2))
	{
		printf("该成员不存在！");
		return false;
	}
	if (key1 == key2)//不能输入自己
	{
		printf("不能输入自己！");
		return false;
	}
	if (set_member(T1->data.following, key2, &T0))//是成员
	{
		printf("%d是%d的关注！\n", key2, key1);
		return true;
	}
	printf("%d不是%d的关注！\n", key2, key1);
	return false;
}


/**
* 函数名称：traverse_following
* 函数参数：二叉树T
* 函数功能：遍历关注
* 返回值：无
**/
bool traverse_following(BATree T) {
	int key;
	BATree T1;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.following == NULL)//未初始化
	{
		printf("%d的粉丝集未初始化！\n", key);
		return false;
	}
	if (T1->data.following == NULL)//无关注
		printf("%d当前无关注！\n", key);
	else
	{
		printf("%d当前关注有：", key);
		output_relation(T1->data.following,T);//输出关注
		printf("\n");
	}
}


/**
* 函数名称：init_likes
* 函数参数：二叉树T，二叉树L
* 函数功能：初始化爱好集
* 返回值：无
**/
bool init_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	set_init_fors(&(T1->data.likes));//初始化
	T1->data.likes->h = 0;
}


/**
* 函数名称：insert_likes
* 函数参数：二叉树T，二叉树L
* 函数功能：插入爱好
* 返回值：无
**/
bool insert_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.likes == NULL)//未初始化
	{
		printf("%d的爱好集未初始化！\n", key);
		return false;
	}
	if (T1->data.likes == NULL)
		printf("%d当前无爱好！\n", key);
	else
	{
		printf("%d当前爱好有：", key);
		output_likes(T1->data.likes, L);//输出爱好
		printf("\n");
	}
	int key2;
	printf("请输入要插入的爱好关键字：");
	scanf("%d", &key2);
	if (!set_member(L, key2, &T0))
	{
		printf("该爱好不存在！");
		return false;
	}
	if (!set_member(T1->data.likes, key2, &T0))//爱好没有存在
	{
		Info data1;
		data1.key = key2;
		set_insert(&(T1->data.likes), data1, &taller);//插入爱好
		printf("插入成功！");
		return true;
	}
	printf("该爱好已存在！");
	getchar();
	return false;
}


/**
* 函数名称：remove_likes
* 函数参数：二叉树T，二叉树L
* 函数功能：删除爱好
* 返回值：无
**/
bool remove_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.likes == NULL)//未初始化
	{
		printf("%d的爱好集未初始化！\n", T1->data.key);
		return false;
	}
	if (T1->data.likes == NULL)
		printf("%d当前无爱好！\n", key);
	else
	{
		printf("%d当前爱好有：", key);
		output_likes(T1->data.likes, L);//输出爱好
		printf("\n");
	}
	int key2;
	printf("请输入删除爱好的关键字：");
	scanf("%d", &key2);
	if (!set_member(L, key2, &T0))//爱好不存在
	{
		printf("该爱好不存在！");
		return false;
	}
	if (set_member(T1->data.likes, key2, &T0))//存在
	{
		set_remove(&(T1->data.likes), key2, &shorter);//删除
		printf("删除成功！");
		return true;
	}
	printf("该爱好不存在！");

	return false;
}


/**
* 函数名称：search_likes
* 函数参数：二叉树T，二叉树L
* 函数功能：搜索爱好
* 返回值：无
**/
bool search_likes(BATree T, BATree L) {
	int key;
	BATree T1, T0;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.likes == NULL)//未初始化
	{
		printf("%d的爱好集未初始化！\n", T1->data.key);
		return false;
	}
	int key2;
	printf("请输入要查找的爱好关键字：");
	scanf("%d", &key2);
	if (set_member(T1->data.likes, key2, &T0))//是爱好
	{
		printf("%d是爱好！\n", key2);
		return true;
	}
	printf("%d不是爱好！\n", key2);
	return false;
}


/**
* 函数名称：traverse_likes
* 函数参数：二叉树T，二叉树L
* 函数功能：遍历爱好
* 返回值：无
**/
bool traverse_likes(BATree T, BATree L) {
	int key;
	BATree T1;
	printf("请输入成员关键字：");
	scanf("%d", &key);
	if (!set_member(T, key, &T1))
	{
		printf("该成员不存在！");
		return false;
	}
	if (T1->data.likes == NULL)//未初始化
	{
		printf("%d的爱好集未初始化！\n", T1->data.key);
		return false;
	}
	if (T1->data.likes == NULL)
		printf("%d当前无爱好！\n", key);
	else
	{
		printf("%d当前的爱好有：", key);
		output_likes(T1->data.likes,L);//输出爱好
		printf("\n");
	}
}



/**
* 函数名称：load
* 函数参数：二叉树*T
* 函数功能：加载更多信息
* 返回值：无
**/
bool load(BATree *T) {
	char filename[20];
	FILE *fp;
	Info data;
	char s[10000];
	int i = 1;
	int key;
	printf("1 加载好友  2 加载粉丝  3 加载关注  4 加载爱好  0 退出\n");
	while (i)
	{
		printf("请选择操作：");
		scanf("%d", &i);
		if ((i != 0) && (i != 1) && (i != 2) && (i != 3) && (i != 4)) continue;//输入错误继续
		if (i == 0) break;
		printf("请输入文件名：");
		scanf("%s", filename);
		getchar();
		printf("\n");
		if ((fp = fopen(filename, "r")) == NULL)//打开文件
		{
			printf("文件打开失败！\n");
			return false;
		}
		switch (i)
		{
		case 1:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//找不到成员
			{
				printf("信息有误！\n");
				break;
			}
			set_init_fors(&(Ts->data.friends));//初始化朋友集
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//下一位
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.friends), data,&taller);//插入
			}
		}
			   break;
		case 2:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//找不到成员
			{
				printf("信息有误！\n");
				break;
			}
			set_init_fors(&(Ts->data.fans));//初始化
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//下一个
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.fans), data,&taller);//插入
			}
		}
			   break;
		case 3:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//找不到成员
			{
				printf("信息有误！\n");
				break;
			}
			set_init_fors(&(Ts->data.following));//初始化
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//下一个
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.following), data,&taller);//插入
			}
		}
			   break;
		case 4:while (fscanf(fp, "%d", &key) != EOF)
		{
			BATree Ts;
			if (!set_member(*T, key, &Ts))//找不到成员
			{
				printf("信息有误！\n");
				break;
			}
			set_init_fors(&(Ts->data.likes));//初始化
			while (1)
			{
				fscanf(fp, "%d", &key);
				if (key == 666) break;//下一个
				data.key = key;
				data.friends = NULL;
				data.fans = NULL;
				data.following = NULL;
				data.likes = NULL;
				set_insert(&(Ts->data.likes), data, &taller);//插入
			}
		}
			   break;
			   fclose(fp);
		}
	}
	return true;
}




/**
* 函数名称：save
* 函数参数：二叉树T
* 函数功能：保存更多信息
* 返回值：无
**/
bool save(BATree T) {
	FILE *fp;
	char filename[20];
	int i = 1;
	printf("1 保存好友   2 保存粉丝  3 保存关注  4 保存爱好  0 退出\n");
	while (i)
	{
		printf("请选择操作：");
		scanf("%d", &i);
		if ((i != 0) && (i != 1) && (i != 2) && (i != 3) && (i != 4)) continue;//输入错误继续输入
		if (i == 0) break;
		printf("请输入文件名：");
		scanf("%s", filename);
		getchar();
		printf("\n");
		if ((fp = fopen(filename, "w")) == NULL)//打开文件
		{
			printf("文件打开失败！\n");
			return false;
		}
		save_relation(T, fp, i);//保存关系
		fclose(fp);
	}
	return true;
}


/**
* 函数名称：save_relation
* 函数参数：二叉树T,文件指针*fp
* 函数功能：保存更多信息
* 返回值：无
**/
bool save_relation(BATree T, FILE *fp, int i) {
	if (T != NULL)
	{
		fprintf(fp, "%d ", T->data.key);
		switch (i)
		{
		case 1:
			traverse_relation(T->data.friends, fp);//保存朋友关系
			fprintf(fp, "%d ", 666);//标志
			break;
		case 2:
			traverse_relation(T->data.fans, fp);//保存粉丝关系
			fprintf(fp, "%d ", 666);
			break;
		case 3:
			traverse_relation(T->data.following, fp);//保存关注关系
			fprintf(fp, "%d ", 666);
			break;
		case 4:
			traverse_relation(T->data.likes, fp);//保存爱好
			fprintf(fp, "%d ", 666);
			break;
		}
		save_relation(T->lchild, fp, i);//递归
		save_relation(T->rchild, fp, i);
	}
}


/**
* 函数名称：traverse_relation
* 函数参数：二叉树T,文件指针*fp
* 函数功能：遍历保存
* 返回值：无
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
* 函数名称：avl_init
* 函数参数：二叉树*T
* 函数功能：初始化二叉树
* 返回值：初始化后的二叉树
**/
void avl_init(BATree *T) {
	*T = NULL;
	printf("初始化成功!\n");
}


/**
* 函数名称：avl_destroy
* 函数参数：二叉树*T
* 函数功能：销毁二叉树
* 返回值：无
**/
void avl_destroy(BATree *T) {
	if ((*T) != NULL) {
		avl_destroy(&((*T)->lchild));
		avl_destroy(&((*T)->rchild));
		free(*T);
	}
}


/**
* 函数名称：avl_insert
* 函数参数：二叉树*T，信息e，bool型变量taller(二叉树是否增高)
* 函数功能：在平衡二叉树中插入节点e，并保持二叉树平衡
* 返回值：成功返回true，否则返回false
**/
bool avl_insert(BATree *T, Info e, bool *taller) {
	if ((*T) == NULL)
	{
		(*T) = (BATree)malloc(sizeof(BATNode));//分配空间
		(*T)->data = e;
		(*T)->lchild = NULL;
		(*T)->rchild = NULL;
		(*T)->bflag = EH;
		*taller = true;
	}
	else {
		if (e.key == (*T)->data.key) {//含已有关键字
			*taller = false;
			return false;
		}
		if (e.key < (*T)->data.key) {//左子树插入
			if (!avl_insert(&((*T)->lchild), e, taller))return false;//插入失败
			if (*taller)//插入成功
				switch ((*T)->bflag) {
				case LH://左边高
					LeftBalance(T);
					*taller = false;
					break;
				case EH://等高
					(*T)->bflag = LH;
					*taller = true;
					break;
				case RH://右边高
					(*T)->bflag = EH;
					*taller = false;
					break;
				}
		}
		else {//右子树中插入
			if (!avl_insert(&((*T)->rchild), e, taller))return false;//插入失败
			if (*taller)//插入成功
				switch ((*T)->bflag) {
				case LH://左边高
					(*T)->bflag = EH;
					*taller = false;
					break;
				case EH://等高
					(*T)->bflag = RH;
					*taller = true;
					break;
				case RH://右边高
					RightBalance(T);
					*taller = false;
					break;
				}
		}
	}
	return true;
}


/**
* 函数名称：avl_delete
* 函数参数：二叉树*T，关键字key，bool型变量shorter(二叉树是否变矮)
* 函数功能：在平衡二叉树中删除节点e，并保持二叉树平衡
* 返回值：成功返回true，否则返回false
**/
bool avl_delete(BATree *T, int key, bool *shorter) {
	if ((*T) == NULL)return false;//不存在
	else if (key == (*T)->data.key) {
		BATree p;
		if ((*T)->lchild == NULL) {//左子树空
			p = (*T);
			(*T) = (*T)->rchild;
			free(p);
			*shorter = true;
		}
		else if ((*T)->rchild == NULL) {//右子树空
			p = (*T);
			(*T) = (*T)->lchild;
			free(p);
			*shorter = true;
		}
		else {//左右子树都不为空
			p = (*T)->lchild;
			while (p->rchild) {
				p = p->rchild;
			}
			(*T)->data = p->data;
			avl_delete(&((*T)->lchild), p->data.key, shorter);//左子树递归删除前驱节点
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
					if ((*T)->rchild->bflag == EH) *shorter = false;//右孩子为EH
					else *shorter = true;
					RightBalance(T);//右平衡
					break;
				}
			}
		}
	}
	else if (key<(*T)->data.key) {//左子树中继续找节点
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
				if ((*T)->rchild->bflag == EH) *shorter = false;//右孩子为EH
				else *shorter = true;
				RightBalance(T);//右平衡
				break;
			}
		}
	}
	else {//右子树中继续找节点
		if (!avl_delete(&((*T)->rchild), key, shorter))return false;
		if (*shorter) {
			switch ((*T)->bflag) {
			case LH:
				if ((*T)->lchild->bflag == EH) *shorter = false;//左孩子为EH
				else *shorter = true;
				LeftBalance(T);//左平衡
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
* 函数名称：avl_traverse
* 函数参数：二叉树T
* 函数功能：中序遍历
* 返回值：无
**/
void avl_traverse(BATree T) {
	if (T == NULL)return;
	avl_traverse(T->lchild);
	printf("%3d", T->data.key);
	avl_traverse(T->rchild);
}


/**
* 函数名称：avl_search
* 函数参数：二叉树T，关键字key,二叉树*Tsub(指向要找的节点)
* 函数功能：判断关键字为key的节点是否是T中的成员
* 返回值：是返回true，否则返回false
**/
bool avl_search(BATree T, int key, BATree *Tsub) {
	if (T == NULL)return false;
	if (T->data.key == key) {
		*Tsub = T;
		return true;
	}
	else if (key<T->data.key) {//左子树查找
		if (avl_search(T->lchild, key, Tsub))return true;
	}
	else { if (avl_search(T->rchild, key, Tsub))return true; }//右子树查找
	return false;
}