#include "def.h"

extern int yylineno;

int breakPos;//指明位置

char *strcat0(char *s1,char *s2) {
    static char result[10];
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}

char *newAlias() {
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("v",s);
}

char *newLabel() {
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("label",s);
}

char *newTemp(){
    static int no=1;
    char s[10];
    itoa(no++,s,10);
    return strcat0("temp",s);
}

//生成TAC
struct codenode *genIR(int op,struct opn opn1,struct opn opn2,struct opn result){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=op;
    h->opn1=opn1;
    h->opn2=opn2;
    h->result=result;
    h->next=h->prior=h;
    return h;
}

//生成Label
struct codenode *genLabel(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=LABEL;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//生成GOTO
struct codenode *genGoto(char *label){
    struct codenode *h=(struct codenode *)malloc(sizeof(struct codenode));
    h->op=GOTO;
    strcpy(h->result.id,label);
    h->next=h->prior=h;
    return h;
}

//首尾相连
struct codenode *merge(int num,...){
    struct codenode *h1,*h2,*p,*t1,*t2;
    va_list ap;
    va_start(ap,num);
    h1=va_arg(ap,struct codenode *);
    while (--num>0) {
        h2=va_arg(ap,struct codenode *);
        if (h1==NULL) h1=h2;
        else if (h2){
            t1=h1->prior;
            t2=h2->prior;
            t1->next=h2;
            t2->next=h1;
            h1->prior=t2;
            h2->prior=t1;
            }
        }
    va_end(ap);
    return h1;
}

//输出中间代码
void prnIR(struct codenode *head){
    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head;
    do {
        if (h->opn1.kind==INT)
             sprintf(opnstr1,"#%d",h->opn1.const_int);
        if (h->opn1.kind==FLOAT)
             sprintf(opnstr1,"#%f",h->opn1.const_float);
        if (h->opn1.kind==ID)
             sprintf(opnstr1,"%s",h->opn1.id);
        if (h->opn2.kind==INT)
             sprintf(opnstr2,"#%d",h->opn2.const_int);
        if (h->opn2.kind==FLOAT)
             sprintf(opnstr2,"#%f",h->opn2.const_float);
        if (h->opn2.kind==ID)
             sprintf(opnstr2,"%s",h->opn2.id);
        sprintf(resultstr,"%s",h->result.id);
        switch (h->op) {
            case ASSIGNOP:  printf("  %s := %s\n",resultstr,opnstr1);
                            break;
            case PLUSASS:  printf("  %s := %s + %s\n",resultstr,resultstr,opnstr2);//here 复合赋值的中间代码
                            break;                            
            case SELF_PLUS: printf("  %s := %s + %s\n",resultstr,resultstr,opnstr1);
                            break;
            case PLUS:
            case MINUS:
            case STAR:
            case DIV: printf("  %s := %s %c %s\n",resultstr,opnstr1, \
                      h->op==PLUS?'+':h->op==MINUS?'-':h->op==STAR?'*':'\\',opnstr2);
                      break;
            case FUNCTION: printf("\nFUNCTION %s :\n",h->result.id);
                           break;
            case PARAM:    printf("  PARAM %s\n",h->result.id);
                           break;
            case LABEL:    printf("LABEL %s :\n",h->result.id);
                           break;
            case GOTO:     printf("  GOTO %s\n",h->result.id);
                           break;
            case JLE:      printf("  IF %s <= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JLT:      printf("  IF %s < %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JGE:      printf("  IF %s >= %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case JGT:      printf("  IF %s > %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case EQ:       printf("  IF %s == %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case NEQ:      printf("  IF %s != %s GOTO %s\n",opnstr1,opnstr2,resultstr);
                           break;
            case ARG:      printf("  ARG %s\n",h->result.id);
                           break;
            case CALL:     printf("  %s := CALL %s\n",resultstr, opnstr1);
                           break;
            case RETURN:   if (h->result.kind)
                                printf("  RETURN %s\n",resultstr);
                           else
                                printf("  RETURN\n");
                           break;
        }
    h=h->next;
    } while (h!=head);
}

void genObCode(struct codenode *head)
{
    int i;
    char opnstr1[32],opnstr2[32],resultstr[32];
    struct codenode *h=head,*p;
    FILE *fp;
    fp=fopen("OBCode.s","w");
    fprintf(fp,".data\n");
    fprintf(fp,"_Prompt: .asciiz \"Enter an integer:  \"\n");
    fprintf(fp,"_ret: .asciiz \"\\n\"\n");
    fprintf(fp,".globl main\n");
    fprintf(fp,".text\n");
    fprintf(fp,"read:\n");
    fprintf(fp,"  li $v0,4\n");
    fprintf(fp,"  la $a0,_Prompt\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  li $v0,5\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  jr $ra\n");
    fprintf(fp,"write:\n");
    fprintf(fp,"  li $v0,1\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  li $v0,4\n");
    fprintf(fp,"  la $a0,_ret\n");
    fprintf(fp,"  syscall\n");
    fprintf(fp,"  move $v0,$0\n");
    fprintf(fp,"  jr $ra\n");
    do {
        switch (h->op) {
            case ASSIGNOP:
                        if (h->opn1.kind==INT)
                            fprintf(fp, "  li $t3, %d\n", h->opn1.const_int);
                        else {
                            fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
                            fprintf(fp, "  move $t3, $t1\n");
                            }
                        fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
                        break;
            case PLUS:
            case MINUS:
            case STAR:
            case DIV:
                       fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
                       fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
                       if (h->op==PLUS)       fprintf(fp, "  add $t3,$t1,$t2\n");
                       else if (h->op==MINUS) fprintf(fp, "  sub $t3,$t1,$t2\n");
                            else if (h->op==STAR)  fprintf(fp, "  mul $t3,$t1,$t2\n");
                                 else  {
                                        fprintf(fp, "  div $t1, $t2\n");
                                        fprintf(fp, "  mflo $t3\n");
                                        }
                        fprintf(fp, "  sw $t3, %d($sp)\n", h->result.offset);
                        break;
            case FUNCTION:
                        fprintf(fp, "\n%s:\n", h->result.id);
                        if (!strcmp(h->result.id,"main")) //特殊处理main
                            fprintf(fp, "  addi $sp, $sp, -%d\n",symbolTable.symbols[h->result.offset].offset);
                        break;
            case PARAM: 
                        break;
            case LABEL: fprintf(fp, "%s:\n", h->result.id);
                        break;
            case GOTO:  fprintf(fp, "  j %s\n", h->result.id);
                        break;
            case JLE:
            case JLT:
            case JGE:
            case JGT:
            case EQ:
            case NEQ:
                        fprintf(fp, "  lw $t1, %d($sp)\n", h->opn1.offset);
                        fprintf(fp, "  lw $t2, %d($sp)\n", h->opn2.offset);
                        if (h->op==JLE) fprintf(fp, "  ble $t1,$t2,%s\n", h->result.id);
                        else if (h->op==JLT) fprintf(fp, "  blt $t1,$t2,%s\n", h->result.id);
                        else if (h->op==JGE) fprintf(fp, "  bge $t1,$t2,%s\n", h->result.id);
                        else if (h->op==JGT) fprintf(fp, "  bgt $t1,$t2,%s\n", h->result.id);
                        else if (h->op==EQ)  fprintf(fp, "  beq $t1,$t2,%s\n", h->result.id);
                        else                 fprintf(fp, "  bne $t1,$t2,%s\n", h->result.id);
                        break;
            case ARG:   
                        break;
            case CALL:  
                        if (!strcmp(h->opn1.id,"write")){
                            fprintf(fp, "  lw $a0, %d($sp)\n",h->prior->result.offset);
                            fprintf(fp, "  addi $sp, $sp, -4\n");
                            fprintf(fp, "  sw $ra,0($sp)\n"); //保留返回地址
                            fprintf(fp, "  jal write\n");
                            fprintf(fp, "  lw $ra,0($sp)\n"); //恢复返回地址
                            fprintf(fp, "  addi $sp, $sp, 4\n");
                            break;
                        }
                        if (!strcmp(h->opn1.id,"read")){
                            fprintf(fp, "  addi $sp, $sp, -4\n");
                            fprintf(fp, "  sw $ra,0($sp)\n"); //保留返回地址
                            fprintf(fp, "  jal read\n");
                            fprintf(fp, "  lw $ra,0($sp)\n"); //恢复返回地址
                            fprintf(fp, "  addi $sp, $sp, 4\n");
                            fprintf(fp, "  sw $v0, %d($sp)\n",h->result.offset);
                            break;
                        }
                        for(p=h,i=0;i<symbolTable.symbols[h->opn1.offset].paramnum;i++){
                            p=p->prior;
                        }
                        fprintf(fp, "  move $t0,$sp\n"); //取实参表达式的值
                        fprintf(fp, "  addi $sp, $sp, -%d\n", symbolTable.symbols[h->opn1.offset].offset);
                        fprintf(fp, "  sw $ra,0($sp)\n"); //保留返回地址
                        i=h->opn1.offset+1;  //第一个形参变量
                        while (symbolTable.symbols[i].flag=='P')
                            {
                                fprintf(fp, "  lw $t1, %d($t0)\n", p->result.offset);
                                fprintf(fp, "  move $t3,$t1\n");
                                fprintf(fp, "  sw $t3,%d($sp)\n",  symbolTable.symbols[i].offset);
                                p=p->next; i++;
                            }
                        fprintf(fp, "  jal %s\n",h->opn1.id); //恢复返回地址
                        fprintf(fp, "  lw $ra,0($sp)\n"); //恢复返回地址
                        fprintf(fp, "  addi $sp,$sp,%d\n",symbolTable.symbols[h->opn1.offset].offset);
                        fprintf(fp, "  sw $v0,%d($sp)\n", h->result.offset);
                        break;
            case RETURN:
                        fprintf(fp, "  lw $v0,%d($sp)\n",h->result.offset);
                        fprintf(fp, "  jr $ra\n");
                        break;

        }
    h=h->next;
    } while (h!=head);
fclose(fp);
}

void semantic_error(int line,char *msg1,char *msg2){
    printf("at line %d , %s %s\n",line,msg1,msg2);
}
void prn_symbol(){ //显示符号表
    int i=0;
    printf("%6s %6s %6s  %6s %4s %6s\n","VarId","Alias","Level","Type","Label","Offset");
    for(i=0;i<symbolTable.index;i++)
        printf("%6s %6s %6d  %6s %4c %6d\n",symbolTable.symbols[i].name,\
                symbolTable.symbols[i].alias,symbolTable.symbols[i].level,\
                symbolTable.symbols[i].type==INT?"int":"float",\
                symbolTable.symbols[i].flag,symbolTable.symbols[i].offset);
}

int searchSymbolTable(char *name) {
    int i;
    for(i=symbolTable.index-1;i>=0;i--)
        if (!strcmp(symbolTable.symbols[i].name, name))  return i;
    return -1;
}

int fillSymbolTable(char *name,char *alias,int level,int type,char flag,int offset) {
    //首先根据name查符号表, 不能重复定义
    int i;
    /*符号查重, 考虑外部变量 声明前有函数定义*/
    for(i=symbolTable.index-1;symbolTable.symbols[i].level==level||(level==0 && i>=0);i--) {
        if (level==0 && symbolTable.symbols[i].level==1) continue;
        if (!strcmp(symbolTable.symbols[i].name, name))  return -1;
        }
    //填写符号表内容
    strcpy(symbolTable.symbols[symbolTable.index].name,name);
    strcpy(symbolTable.symbols[symbolTable.index].alias,alias);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; //符号在符号表中的位置序号
}


int fill_Temp(char *name,int level,int type,char flag,int offset) {
    strcpy(symbolTable.symbols[symbolTable.index].name,"");
    strcpy(symbolTable.symbols[symbolTable.index].alias,name);
    symbolTable.symbols[symbolTable.index].level=level;
    symbolTable.symbols[symbolTable.index].type=type;
    symbolTable.symbols[symbolTable.index].flag=flag;
    symbolTable.symbols[symbolTable.index].offset=offset;
    return symbolTable.index++; 
}

int LEV=0;      //层号
int func_size;

void ext_var_list(struct node *T){  //处理变量列表
    int rtn,num=1;
    switch (T->kind){
        case EXT_DEC_LIST:
                T->ptr[0]->type=T->type;               
                T->ptr[0]->offset=T->offset;          
                T->ptr[1]->type=T->type;              
                T->ptr[1]->offset=T->offset+T->width; 
                T->ptr[1]->width=T->width;
                ext_var_list(T->ptr[0]);
                ext_var_list(T->ptr[1]);
                T->num=T->ptr[1]->num+1;
                break;
        case ID:
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'V',T->offset);  //最后一个
            if (rtn==-1)
                semantic_error(T->pos,T->type_id, "Variable redefined");
            else T->place=rtn;
            T->num=1;
            break;
        }
    }

int  match_param(int i,struct node *T, int pos){
    int j,num=symbolTable.symbols[i].paramnum;//
    int type1,type2;
    if (num==0 && T==NULL) return 1;
    for (j=1;j<=num;j++) {
        if (!T){
            semantic_error(pos,"", "Paramter is not enough");
            return 0;
            }
        type1=symbolTable.symbols[i+j].type;  //形参类型
        type2=T->ptr[0]->type;
        if (type1!=type2){
            semantic_error(T->pos,"", "Paramter unmatch");
            return 0;
        }
        T=T->ptr[1];
    }
    if (T){
        semantic_error(T->pos,"", "Paramter is superabundant");
        return 0;
        }
    return 1;
}

void boolExp(struct node *T){  // 参考文献[2]p84的思想
  struct opn opn1,opn2,result;
  int op;
  int rtn;
  if (T)
	{
	switch (T->kind) {
        case INT:  if (T->type_int!=0)
                        T->code=genGoto(T->Etrue);
                   else T->code=genGoto(T->Efalse);
                   T->width=0;
                   break;
        case FLOAT:  if (T->type_float!=0.0)
                        T->code=genGoto(T->Etrue);
                   else T->code=genGoto(T->Efalse);
                   T->width=0;
                   break;
        case ID:    //查符号表, 获得符号表中的位置
                   rtn=searchSymbolTable(T->type_id);
                   if (rtn==-1)
                        semantic_error(T->pos,T->type_id, "Variable undefined");
                   if (symbolTable.symbols[rtn].flag=='F')
                        semantic_error(T->pos,T->type_id, "is function name");
                   else {
                        opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[rtn].alias);
                        opn1.offset=symbolTable.symbols[rtn].offset;
                        opn2.kind=INT; opn2.const_int=0;
                        result.kind=ID; strcpy(result.id,T->Etrue);
                        T->code=genIR(NEQ,opn1,opn2,result);
                        T->code=merge(2,T->code,genGoto(T->Efalse));
                        }
                   T->width=0;
                   break;
        case RELOP: //处理关系运算exp ,2个操作数都按基本exp 处理
                    T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                    Exp(T->ptr[0]);
                    T->width=T->ptr[0]->width;
                    Exp(T->ptr[1]);
                    if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                    opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    opn2.kind=ID; strcpy(opn2.id,symbolTable.symbols[T->ptr[1]->place].alias);
                    opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,T->Etrue);
                    if (strcmp(T->type_id,"<")==0)
                            op=JLT;
                    else if (strcmp(T->type_id,"<=")==0)
                            op=JLE;
                    else if (strcmp(T->type_id,">")==0)
                            op=JGT;
                    else if (strcmp(T->type_id,">=")==0)
                            op=JGE;
                    else if (strcmp(T->type_id,"==")==0)
                            op=EQ;
                    else if (strcmp(T->type_id,"!=")==0)
                            op=NEQ;
                    T->code=genIR(op,opn1,opn2,result);
                    T->code=merge(4,T->ptr[0]->code,T->ptr[1]->code,T->code,genGoto(T->Efalse));
                    break;
        case AND:
        case OR:
                    if (T->kind==AND) {
                        strcpy(T->ptr[0]->Etrue,newLabel());
                        strcpy(T->ptr[0]->Efalse,T->Efalse);
                        }
                    else {
                        strcpy(T->ptr[0]->Etrue,T->Etrue);
                        strcpy(T->ptr[0]->Efalse,newLabel());
                        }
                    strcpy(T->ptr[1]->Etrue,T->Etrue);
                    strcpy(T->ptr[1]->Efalse,T->Efalse);
                    T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                    boolExp(T->ptr[0]);
                    T->width=T->ptr[0]->width;
                    boolExp(T->ptr[1]);
                    if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                    if (T->kind==AND)
                        T->code=merge(3,T->ptr[0]->code,genLabel(T->ptr[0]->Etrue),T->ptr[1]->code);
                    else
                        T->code=merge(3,T->ptr[0]->code,genLabel(T->ptr[0]->Efalse),T->ptr[1]->code);
                    break;
        case NOT:   strcpy(T->ptr[0]->Etrue,T->Efalse);
                    strcpy(T->ptr[0]->Efalse,T->Etrue);
                    boolExp(T->ptr[0]);
                    T->code=T->ptr[0]->code;
                    break;
        }
	}
}


void Exp(struct node *T)
{//处理基本exp , 参考文献[2]p82的思想
  int rtn,num,width;
  struct node *T0,*Ttemp;
  struct opn opn1,opn2,result;
  if (T)
	{
	switch (T->kind) {
	case ID:    //查符号表, 获得符号表中的位置
                rtn=searchSymbolTable(T->type_id);
                if (rtn==-1)
                    semantic_error(T->pos,T->type_id, "Variable undefined");
                if (symbolTable.symbols[rtn].flag=='F')
                    semantic_error(T->pos,T->type_id, "is function name");
                else {
                    T->place=rtn;       
                    T->code=NULL;       //标识符不需要生成 TAC
                    T->type=symbolTable.symbols[rtn].type;
                    T->offset=symbolTable.symbols[rtn].offset;
                    T->width=0;   //未再使用新单元
                    }
                break;
    case INT:   T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset); //为整常量生成 一个临时变量 
                T->type=INT;
                opn1.kind=INT;opn1.const_int=T->type_int;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=4;
                break;
    case FLOAT: T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset);   //为浮点常量生成 一个临时变量 
                T->type=FLOAT;
                opn1.kind=FLOAT; opn1.const_float=T->type_float;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=genIR(ASSIGNOP,opn1,opn2,result);
                T->width=4;
                break;
	case ASSIGNOP:
                if (T->ptr[0]->kind!=ID){
                    semantic_error(T->pos,"", "Left must be left-value(=)");
                }
                else {
                    Exp(T->ptr[0]); 
                    T->ptr[1]->offset=T->offset;
                    Exp(T->ptr[1]);
                    T->type=T->ptr[0]->type;
                    rtn=searchSymbolTable(T->ptr[0]->type_id);
                    if ( symbolTable.symbols[rtn].type != T->ptr[1]->type){//不匹配
                        semantic_error(T->pos,"", "Two sides' type unmatch");
                    }
                    T->width=T->ptr[1]->width;
                    T->code=merge(2,T->ptr[0]->code,T->ptr[1]->code);
                    opn1.kind=ID;   strcpy(opn1.id,symbolTable.symbols[T->ptr[1]->place].alias);
                    opn1.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ASSIGNOP,opn1,opn2,result));
                }
                break;
    case PLUSASS:
                if (T->ptr[0]->kind!=ID){
                    semantic_error(T->pos,"", "Left must be left-value(+=)");
                }
                else {
                    T->ptr[0]->offset = T->offset;
                    Exp(T->ptr[0]);
                    T->ptr[1]->offset = T->offset + T->ptr[0]->width;
                    Exp(T->ptr[1]);
                    T->type = T->ptr[0]->type;
                    rtn = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset + T->ptr[1]->width);
                    T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset + T->ptr[1]->width + 4);
                    T->width = T->ptr[1]->width + 4 * 2;
                    T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);
                    opn1.kind=ID; strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
                    opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    opn2.kind=ID; strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
                    opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                    result.kind=ID; strcpy(result.id, symbolTable.symbols[rtn].alias);
                    result.offset=symbolTable.symbols[rtn].offset;
                    T->code = merge(2, T->code, genIR(PLUS, opn1, opn2, result));
                    strcpy(opn1.id, symbolTable.symbols[rtn].alias);
                    opn1.offset = symbolTable.symbols[rtn].offset;
                    strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code = merge(2, T->code, genIR(ASSIGNOP, opn1, opn2, result));
                }
                break;
    case SELF_PLUS:
            if( T->ptr[1] ){//若有两个子树，则为非法
                semantic_error(T->pos,"", "expression cannot self plus");
            }
            if(T->type != INT  ){//只能整型自增
                semantic_error(T->pos,"", "expression cannot self plus");
            }
            T->ptr[0]->offset = T->offset;
            Exp(T->ptr[0]);

            T->type = T->ptr[0]->type;
            rtn = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset );
            T->place = fill_Temp(newTemp(), LEV, T->type, 'T', T->offset + 4);
            T->width = T->ptr[0]->width + 4 * 2;
            opn1.kind=ID; strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
            T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset);
            T->type=INT;
            opn2.kind=INT;opn2.const_int=1;
            T->width=4;
            result.kind=ID; strcpy(result.id, symbolTable.symbols[rtn].alias);
            result.offset=symbolTable.symbols[rtn].offset;
            T->code = genIR(PLUS, opn1, opn2, result);
            strcpy(opn1.id, symbolTable.symbols[rtn].alias); 
            opn1.offset = symbolTable.symbols[rtn].offset;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
            result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
            T->code = merge(2, T->code, genIR(ASSIGNOP, opn1, opn2, result));
            break;
    
	case AND:
	case OR:
	case RELOP:
            T->type=INT;
            T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
            Exp(T->ptr[0]);
            Exp(T->ptr[1]);
            break;

    case PLUS:
	case MINUS:
	case STAR:
	case DIV:   T->ptr[0]->offset=T->offset;
                Exp(T->ptr[0]);
                T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                Exp(T->ptr[1]);
                if (T->ptr[0]->type==FLOAT || T->ptr[1]->type==FLOAT)
                     T->type=FLOAT,T->width=T->ptr[0]->width+T->ptr[1]->width+4;
                else T->type=INT,T->width=T->ptr[0]->width+T->ptr[1]->width+2;
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->ptr[0]->width+T->ptr[1]->width);//
                opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T->ptr[0]->place].alias);
                opn1.type=T->ptr[0]->type;opn1.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                opn2.kind=ID; strcpy(opn2.id,symbolTable.symbols[T->ptr[1]->place].alias);
                opn2.type=T->ptr[1]->type;opn2.offset=symbolTable.symbols[T->ptr[1]->place].offset;
                result.kind=ID; strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.type=T->type;result.offset=symbolTable.symbols[T->place].offset;
                T->code=merge(3,T->ptr[0]->code,T->ptr[1]->code,genIR(T->kind,opn1,opn2,result));
                T->width=T->ptr[0]->width+T->ptr[1]->width+(T->type==INT?4:8);
                break;
	case NOT:   
                break;
	case UMINUS:
                break;
    case FUNC_CALL: //根据T->type_id查出函数的定义, 如果语言中增加了实验教材的read, write需要单独处理一下
                rtn=searchSymbolTable(T->type_id);
                if (rtn==-1){
                    semantic_error(T->pos,T->type_id, "function name undefined");
                    break;
                    }
                if (symbolTable.symbols[rtn].flag!='F'){
                    semantic_error(T->pos,T->type_id, "is not a function name");
                     break;
                    }
                T->type=symbolTable.symbols[rtn].type;
                width=T->type==INT?4:8;
                if (T->ptr[0]){
                    T->ptr[0]->offset=T->offset;
                    Exp(T->ptr[0]); 
                    T->width=T->ptr[0]->width+width;
                    T->code=T->ptr[0]->code;
                    }
                else {T->width=width; T->code=NULL;}
                match_param(rtn,T->ptr[0], T->pos);
                T0=T->ptr[0];
                while (T0) {
                    result.kind=ID;  strcpy(result.id,symbolTable.symbols[T0->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T0->ptr[0]->place].offset;
                    T->code=merge(2,T->code,genIR(ARG,opn1,opn2,result));
                    T0=T0->ptr[1];
                    }
                T->place=fill_Temp(newTemp(),LEV,T->type,'T',T->offset+T->width-width);
                opn1.kind=ID;     strcpy(opn1.id,T->type_id);
                opn1.offset=rtn;  
                result.kind=ID;   strcpy(result.id,symbolTable.symbols[T->place].alias);
                result.offset=symbolTable.symbols[T->place].offset;
                T->code=merge(2,T->code,genIR(CALL,opn1,opn2,result)); 
                break;
    case ARGS:  
                T->ptr[0]->offset=T->offset;
                Exp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                T->code=T->ptr[0]->code;
                if (T->ptr[1]) {
                    T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                    Exp(T->ptr[1]);
                    T->width+=T->ptr[1]->width;
                    T->code=merge(2,T->code,T->ptr[1]->code);
                }
                break;
         }
      }
}

void semantic_Analysis(struct node *T)
{//对抽象语法树的先根遍历
  int rtn,num,width;
  struct node *T0;
  struct opn opn1,opn2,result;
  if (T)
	{
	switch (T->kind) {
	case EXT_DEF_LIST:
            if (!T->ptr[0]) break;
            T->ptr[0]->offset=T->offset;
            semantic_Analysis(T->ptr[0]);    //访问外部定义列表中的第一个
            T->code=T->ptr[0]->code;
            if (T->ptr[1]){
                T->ptr[1]->offset=T->ptr[0]->offset+T->ptr[0]->width;
                semantic_Analysis(T->ptr[1]); //访问该外部定义列表中的其它外部定义
                T->code=merge(2,T->code,T->ptr[1]->code);
                }
            break;
	case EXT_VAR_DEF:   //处理外部说明
            T->type=T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;
            T->ptr[1]->offset=T->offset;        //这个外部变量 的偏移量向下传递
            T->ptr[1]->width=T->type==INT?4:8;  //将一个变量 的宽度向下传递
            ext_var_list(T->ptr[1]);            //处理外部变量 说明中的标识符序列
            T->width=(T->type==INT?4:8)* T->ptr[1]->num; //计算这个外部变量 说明的宽度
            T->code=NULL;             //这里假定外部变量 不支持初始化
            break;
	case FUNC_DEF:      //填写函数定义信息到符号表
            T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;//获取函数
            T->width=0;     //函数的宽度设置为0, 不会对外部变量 的地址分配产生影响
            T->offset=DX;   //设置局部变量 在活动记录中的偏移量初值
            semantic_Analysis(T->ptr[1]); 
            T->offset+=T->ptr[1]->width;   
            T->ptr[2]->offset=T->offset;
            strcpy(T->ptr[2]->Snext,newLabel()); 
            semantic_Analysis(T->ptr[2]);         //处理函数体
            //计算活动记录大小,这里offset属性存放的是活动记录大小, 不是偏移
            symbolTable.symbols[T->ptr[1]->place].offset=T->offset+T->ptr[2]->width;
            T->code=merge(3,T->ptr[1]->code,T->ptr[2]->code,genLabel(T->ptr[2]->Snext));
            break;
	case FUNC_DEC: 
            rtn=fillSymbolTable(T->type_id,newAlias(),LEV,T->type,'F',0);//函数不在数据区中分配单元, 偏移量为0
            if (rtn==-1){
                semantic_error(T->pos,T->type_id, "function name redefined");
                break;
                }
            else T->place=rtn;
            result.kind=ID;   strcpy(result.id,T->type_id);
            result.offset=rtn;
            T->code=genIR(FUNCTION,opn1,opn2,result);
            T->offset=DX;   //设置形参在活动记录中的偏移量初值
            if (T->ptr[0]) { //判断
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);//处理
                T->width=T->ptr[0]->width;
                symbolTable.symbols[rtn].paramnum=T->ptr[0]->num;
                T->code=merge(2,T->code,T->ptr[0]->code);
                }
            else symbolTable.symbols[rtn].paramnum=0,T->width=0;
            break;
	case PARAM_LIST:
            T->ptr[0]->offset=T->offset;
            semantic_Analysis(T->ptr[0]);
            if (T->ptr[1]){
                T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                semantic_Analysis(T->ptr[1]);
                T->num=T->ptr[0]->num+T->ptr[1]->num;        
                T->width=T->ptr[0]->width+T->ptr[1]->width;  
                T->code=merge(2,T->ptr[0]->code,T->ptr[1]->code); 
                }
            else {
                T->num=T->ptr[0]->num;
                T->width=T->ptr[0]->width;
                T->code=T->ptr[0]->code;
                }
            break;
	case  PARAM_DEC:
            rtn=fillSymbolTable(T->ptr[1]->type_id,newAlias(),1,T->ptr[0]->type,'P',T->offset);
            if (rtn==-1)
                semantic_error(T->ptr[1]->pos,T->ptr[1]->type_id, "Paramter name cannot be the same");
            else T->ptr[1]->place=rtn;
            T->num=1;       
            T->width=T->ptr[0]->type==INT?4:8; 
            result.kind=ID;   strcpy(result.id, symbolTable.symbols[rtn].alias);
            result.offset=T->offset;
            T->code=genIR(PARAM,opn1,opn2,result); 
            break;
	case COMP_STM:
            LEV++;
            //设置层号加1, 并且保存该层局部变量 在符号表中的起始位置
            symbol_scope_TX.TX[symbol_scope_TX.top++]=symbolTable.index;
            T->width=0;
            T->code=NULL;
            if (T->ptr[0]) {
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);  //处理该层的局部变量
                T->width+=T->ptr[0]->width;
                T->code=T->ptr[0]->code;
                }
            if (T->ptr[1]){
                T->ptr[1]->offset=T->offset+T->width;
                strcpy(T->ptr[1]->Snext,T->Snext);  //S.next属性向下传递
                semantic_Analysis(T->ptr[1]);
                T->width+=T->ptr[1]->width;
                T->code=merge(2,T->code,T->ptr[1]->code);
                } 
             prn_symbol();
             LEV--; 
             symbolTable.index=symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
             break;
    case DEF_LIST:
            T->code=NULL;
            if (T->ptr[0]){
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);   //处理一个局部变量 定义
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                }
            if (T->ptr[1]) {
                T->ptr[1]->offset=T->offset+T->ptr[0]->width;
                semantic_Analysis(T->ptr[1]);   //处理剩下的局部变量 定义
                T->code=merge(2,T->code,T->ptr[1]->code);
                T->width+=T->ptr[1]->width;
                }
                break;
    case VAR_DEF://类似于上面的外部变量 EXT_VAR_DEF
                T->code=NULL;
                T->ptr[1]->type=!strcmp(T->ptr[0]->type_id,"int")?INT:FLOAT;  
                T0=T->ptr[1]; 
                num=0;
                T0->offset=T->offset;
                T->width=0;
                width=T->ptr[1]->type==INT?4:8;  //一个变量 宽度
                while (T0) {  //处理所以DEC_LISTnode 
                    num++;
                    T0->ptr[0]->type=T0->type;  //类型属性向下传递
                    if (T0->ptr[1]) T0->ptr[1]->type=T0->type;
                    T0->ptr[0]->offset=T0->offset;  //类型属性向下传递
                    if (T0->ptr[1]) T0->ptr[1]->offset=T0->offset+width;
                    if (T0->ptr[0]->kind==ID){
                        rtn=fillSymbolTable(T0->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',T->offset+T->width);//此处偏移量未计算, 暂时为0
                        if (rtn==-1)
                            semantic_error(T0->ptr[0]->pos,T0->ptr[0]->type_id, "Variable undefined");
                        else T0->ptr[0]->place=rtn;
                        T->width+=width;
                        }
                    else if (T0->ptr[0]->kind==ASSIGNOP){
                            rtn=fillSymbolTable(T0->ptr[0]->ptr[0]->type_id,newAlias(),LEV,T0->ptr[0]->type,'V',T->offset+T->width);//此处偏移量未计算, 暂时为0
                            if (rtn==-1)
                                semantic_error(T0->ptr[0]->ptr[0]->pos,T0->ptr[0]->ptr[0]->type_id, "Variable redefined");
                            else {
                                T0->ptr[0]->place=rtn;
                                T0->ptr[0]->ptr[1]->offset=T->offset+T->width+width;
                                Exp(T0->ptr[0]->ptr[1]);
                                opn1.kind=ID; strcpy(opn1.id,symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
                                opn1.offset = symbolTable.symbols[T0->ptr[0]->ptr[1]->place].offset;
                                result.kind=ID; strcpy(result.id,symbolTable.symbols[T0->ptr[0]->place].alias);
                                result.offset = symbolTable.symbols[T0->ptr[0]->place].offset;
                                T->code=merge(3,T->code,T0->ptr[0]->ptr[1]->code,genIR(ASSIGNOP,opn1,opn2,result));
                                }
                            T->width+=width+T0->ptr[0]->ptr[1]->width;
                            }
                    T0=T0->ptr[1];
                    }
                break;
	case STM_LIST:
                if (!T->ptr[0]) { T->code=NULL; T->width=0; break;}
                if (T->ptr[1])
                    strcpy(T->ptr[0]->Snext,newLabel());
                else     
                    strcpy(T->ptr[0]->Snext,T->Snext);
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                if (T->ptr[1]){
                    strcpy(T->ptr[1]->Snext,T->Snext);
                    T->ptr[1]->offset=T->offset;  //顺序结构共享单元方式
                    semantic_Analysis(T->ptr[1]);
                    if (T->ptr[0]->kind==RETURN ||T->ptr[0]->kind==EXP_STMT ||T->ptr[0]->kind==COMP_STM)
                        T->code=merge(2,T->code,T->ptr[1]->code);
                    else
                        T->code=merge(3,T->code,genLabel(T->ptr[0]->Snext),T->ptr[1]->code);
                    if (T->ptr[1]->width>T->width) T->width=T->ptr[1]->width; //顺序结构共享单元方式
                    }
                break;
	case IF_THEN:
                strcpy(T->ptr[0]->Etrue,newLabel());
                strcpy(T->ptr[0]->Efalse,T->Snext);
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                boolExp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,T->Snext);
                semantic_Analysis(T->ptr[1]);      //if子句
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                T->code=merge(3,T->ptr[0]->code, genLabel(T->ptr[0]->Etrue),T->ptr[1]->code);
                break;
	case IF_THEN_ELSE:
                strcpy(T->ptr[0]->Etrue,newLabel());  //设置条件语句真假转移位置
                strcpy(T->ptr[0]->Efalse,newLabel());
                T->ptr[0]->offset=T->ptr[1]->offset=T->ptr[2]->offset=T->offset;
                boolExp(T->ptr[0]);
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,T->Snext);
                semantic_Analysis(T->ptr[1]);      //if子句
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                strcpy(T->ptr[2]->Snext,T->Snext);
                semantic_Analysis(T->ptr[2]);      //else子句
                if (T->width<T->ptr[2]->width) T->width=T->ptr[2]->width;
                T->code=merge(6,T->ptr[0]->code,genLabel(T->ptr[0]->Etrue),T->ptr[1]->code,\
                              genGoto(T->Snext),genLabel(T->ptr[0]->Efalse),T->ptr[2]->code);
                break;
	case WHILE: strcpy(T->ptr[0]->Etrue,newLabel());
                strcpy(T->ptr[0]->Efalse,T->Snext);
                T->ptr[0]->offset=T->ptr[1]->offset=T->offset;
                boolExp(T->ptr[0]);      //循环条件
                T->width=T->ptr[0]->width;
                strcpy(T->ptr[1]->Snext,newLabel());
                semantic_Analysis(T->ptr[1]);      //循环体
                if (T->width<T->ptr[1]->width) T->width=T->ptr[1]->width;
                T->code=merge(5,genLabel(T->ptr[1]->Snext),T->ptr[0]->code, \
                genLabel(T->ptr[0]->Etrue),T->ptr[1]->code,genGoto(T->ptr[1]->Snext));
                break;
    case EXP_STMT:
                T->ptr[0]->offset=T->offset;
                semantic_Analysis(T->ptr[0]);
                T->code=T->ptr[0]->code;
                T->width=T->ptr[0]->width;
                break;
    case BREAK: 
                breakPos = T->pos;
                break;
	case RETURN:if (T->ptr[0]){
                    T->ptr[0]->offset=T->offset;
                    Exp(T->ptr[0]);
                    num=symbolTable.index;
                    do num--; while (symbolTable.symbols[num].flag!='F');
                    if (T->ptr[0]->type!=symbolTable.symbols[num].type) {
                        semantic_error(T->pos, "","Return value type unmatch");
                        T->width=0;T->code=NULL;
                        break;
                        }
                    T->width=T->ptr[0]->width;
                    result.kind=ID; strcpy(result.id,symbolTable.symbols[T->ptr[0]->place].alias);
                    result.offset=symbolTable.symbols[T->ptr[0]->place].offset;
                    T->code=merge(2,T->ptr[0]->code,genIR(RETURN,opn1,opn2,result));
                    }
                else{
                    T->width=0;
                    result.kind=0;
                    T->code=genIR(RETURN,opn1,opn2,result);
                    }
                break;
	case ID:
    case INT:
    case FLOAT:
	case ASSIGNOP:
    case PLUSASS:
    case SELF_PLUS:
	case AND:
	case OR:
	case RELOP:
	case PLUS:
	case MINUS:
	case STAR:
	case DIV:
	case NOT:
	case UMINUS:
    case FUNC_CALL:
                    Exp(T);          //处理基本exp 
                    break;
    }
    }
}

void semantic_Analysis0(struct node *T) {
    symbolTable.index=0;
    fillSymbolTable("read","",0,INT,'F',4);
    symbolTable.symbols[0].paramnum=0;
    fillSymbolTable("x","",1,INT,'P',12);
    fillSymbolTable("write","",0,INT,'F',4);
    symbolTable.symbols[2].paramnum=1;
    symbol_scope_TX.TX[0]=0;  //外部变量 在符号表中的起始序号为0
    symbol_scope_TX.top=1;
    T->offset=0;              //外部变量 在数据区的偏移量
    semantic_Analysis(T);
    if(T->hasBreak){
        semantic_error(breakPos,"", "unexpected syntax break");
    }
    prnIR(T->code);
    genObCode(T->code);
 }