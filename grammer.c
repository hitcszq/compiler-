#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
struct production{
	int leftside;//产生式左端
	int rightside[100] ;//产生式右端， 从10000开始编码非终结符,10000代表P'，即规约终结标志 从0开始编码终结符（对应的宏），0表示空，
	//struct production* next;
};
struct production_state{
	struct production prod;
	int state;
	int lookahead;
	struct production_state* next;
};

#define statesetnum 100 //状态转移图状态的数目
#define tnum 72 //终结符数目，0-71，0为空
#define vnum 10012 //非终结符 ，10000-11000
#define stack_depth 200
#define buffer_length 1000
#define sharp 123
#define acc 888

struct production production_list[200];
static int production_num = 0;

static int stack_num[stack_depth] = { -1 };
static int stack_num_point = -1;

static int stack_state[stack_depth] = { -1 };
static int stack_state_point = -1;

static int buffer[buffer_length] = { -1 };
static int buffer_point = 0;



static int gototable[statesetnum][vnum - 10000] = {-1};
static int action[statesetnum][tnum] = {-1};

static struct production_state* stateset[statesetnum];//规范项目集族

void innit_table()
{
	for (int i = 0; i < statesetnum; i++){
		for (int j = 10000; j < vnum; j++)
		{
			gototable[i][j] = -1;
		}
	}
	for (int i = 0; i < statesetnum; i++){
		for (int j = 0; j < tnum; j++)
		{
			gototable[i][j] = -1;
		}
	}
}


void read_production(char production_file[])
{
	struct production *proc = (struct production *)malloc(sizeof(struct production));
	FILE *fp = fopen(production_file, "r");
	char StrLine[1024] = {0};             //每行最大读取的字符数
	char rightnum[10] = { 0 };
	if (fp == NULL)
		printf("can not open produciton_file");
	while (!feof(fp))
    {
		fgets(StrLine, 1024, fp);  //读取产生式第一行，产生式索引
		printf("read production %s from file \n", StrLine);
		fgets(StrLine, 1024, fp);  //读取产生式第二行，产生式左端
		proc->leftside = atoi(StrLine);
		fgets(StrLine, 1024, fp);  //读取产生式第三行，产生式右端
		int index = 0;
		int start_index = 0;
		int right_sidex_index = 0;
		while (StrLine[index] != 0)
		{
			if (StrLine[index] = ',')
			{
				int k = 0;
				for (int i = start_index; i < index; i++){
					rightnum[k++] = StrLine[i];
				}
				start_index = index + 1;
				proc->rightside[right_sidex_index++] = atoi(rightnum);
			}
			index++;
		}
		production_list[production_num++] = *proc;
	}
    fclose(fp);                     //关闭文件
	return 0;
}

/*struct production* std_production(char* p)
{
	struct production* re_proc;
	int i = 0;
	int var_index = 0;
	while (p[i] != '\0')
	{
		if (i == 0)
		{
			//re_proc->leftside = p[i++];
			if ((var_index = checkvarindex(p[i])) == -1)
			{
				vara[vara_num++] = p[i];
				re_proc->leftside = 10000 + vara_num - 1;
			}
			else
			{
				re_proc->leftside = 10000 + var_index;
			}
		}
		else if (i > 0)
		{
			
		}
	}
}*/
int checkinfirst(int x, int first[])
{
	int inflag = 0;
	for (int i = 0; i < 50; i++)
	{
		if (first[i] != -1)
		{
			if (first[i] == x)
			{
				inflag = 1;
				break;
			}
		}
	}
	return inflag;
}
int prostateequal(struct production_state* p1, struct production_state* p2)//p1,p2是否相等
{
	int equal = 1;
	if (p1->prod.leftside != p2->prod.leftside)
		equal = 0;
	int k = 0;
	while (p1->prod.rightside[k] != -1 || p2->prod.rightside[k] != -1)
	{
		if (p1->prod.rightside[k++] != p2->prod.rightside[k++])
		{
			equal = 0;
			break;
		}
	}
	if (p1->lookahead != p2->lookahead)
		equal = 0;
	if (p1->state != p2->state)
		equal = 0;
	return equal;
}
int checkinclosure(struct production_state* head,struct production_state *pr)
{
	int inflag = 0;
	while (head!=NULL)
	{
		if (prostateequal(pr , head))//产生式结构体值的比较？？？可能会出问题
		{
			return inflag;
			break;
		}
		head = head->next;
	}
	return inflag;
}


struct production_state* go(struct production_state* I, int x)
{
	//struct production_state* production_state_return=NULL;//后续项目集
	struct production_state* production_stateset_return = (struct production_state *)malloc(sizeof(struct production_state));//后续项目集闭包
	struct production_state* pI = I;
	struct production_state* p_return = NULL;
	
	while (pI!=NULL){
			if (pI->prod.rightside[pI->state] == x)
			{
				if (p_return == NULL)//后续项目集还没有元素
				{
					production_stateset_return->next = NULL;
					production_stateset_return->state = pI->state + 1;
					production_stateset_return->prod = pI->prod;
					production_stateset_return->lookahead = pI->lookahead;
					p_return = production_stateset_return;
				}
				else{
					p_return = (struct production_state *)malloc(sizeof(struct production_state));
					p_return->next = production_stateset_return->next;
					p_return->state = pI->state + 1;
					production_stateset_return->prod = pI->prod;
					production_stateset_return->lookahead = pI->lookahead;
					production_stateset_return->next = p_return;
				}
			}
			pI = pI->next;
	}
	return production_stateset_return;
}
int * first(int x)
{
	int re_first[50];
	int cur_first = 0;
	int continue_flag = 1;
	for (int i = 0; i < 50; i++)
	{
		re_first[i] = -1;
	}
	if (x < 10000)
	{
		re_first[cur_first++] = x;
		return re_first;
	}
	else{
		for (int i = 0; i < production_num; i++)
		{
			if (production_list[i].leftside == x && (production_list[i].rightside[0] < 10000))//首字符是终结符
			{
				if (checkinfirst(production_list[i].rightside[0], re_first) == 0)
					re_first[cur_first++] = production_list[i].rightside[0];
			}
		}
		while (continue_flag)
		{
			for (int i = 0; i < production_num; i++)
			{
				if (production_list[i].leftside == x && (production_list[i].rightside[0] >= 10000))//首字符是非终结符
				{
					for (int j = 0; j < production_num; j++)
					{
						if (production_list[j].leftside == production_list[i].rightside[0] && first(production_list[j].leftside)[0] != -1)//递归求first(G,G[j].rightside)
						{
							int k = 0;
							while (checkinfirst(first(production_list[j].leftside)[k], re_first) == 0 && first(production_list[j].leftside)[k] != -1)
							{
								if (first(production_list[j].leftside)[k] != 0)//去除空
									re_first[cur_first++] = first(production_list[j].leftside)[k++];
								else
									k++;
							}
						}
						else
						{
							continue_flag = 0;
						}
					}
				}
				if (production_list[i].leftside == x)
				{
					int m = 0;
					int emptyflag = 0;
					while (production_list[i].rightside[m] != -1 && checkinfirst(0, first(production_list[i].rightside[m])))
					{
						m++;
						emptyflag = 1;
					}
					if (emptyflag = 1)//m为产生式右端第一个不能产生空的非终结符索引
					{
						for (int h = 0; h < m; h++)
						{
							int k2 = 0;
							while (checkinfirst(first(production_list[i].rightside[h])[k2], re_first) == 0 && first(production_list[i].rightside[h])[k2] != -1)
							{
								if (first(production_list[i].rightside[h])[k2] != 0)//去除空
									re_first[cur_first++] = first(production_list[i].rightside[h])[k2++];
							}
						}
					}
					if (emptyflag = 1 && production_list[i].rightside[m] == -1)//产生式所有非终结符均可以推导出空
					{
						re_first[cur_first++] = 0;
					}

				}
			}
		}
	}
	return re_first;
}

int * firstalpha(int  alpha[])
{
	int re_first[50];
	int cur_first = 0;
	for (int i = 0; i < 50; i++)
	{
		re_first[i] = -1;
	}
	int k = 0;
	while (checkinfirst(first(alpha[0])[k], re_first)==0 && first(alpha[0])[k] != -1)
	{
		if (first(alpha[0])[k] != 0)//去除空
			re_first[cur_first++] = first(alpha[0])[k++];
	}
	int j = 1;
	while (checkinfirst(0, first(alpha[j])) && alpha[j] != -1)//-1表示序列结束
	{
		int kk = 0;
		while (checkinfirst(first( alpha[j])[kk], re_first)==0 && first( alpha[j])[kk] != -1)
		{
			if (first(alpha[j])[kk] != 0)//去除空
				re_first[cur_first++] = first(alpha[j])[kk++];
			else
				k++;
		}
		j++;
	}
	if (alpha[j] == -1)//全可以推出空
	{
		re_first[cur_first++] = 0;
	}
	return re_first;

}
struct production_state* closure(struct production_state*  proc)//求闭包,传入可以为项目集合
{
	struct production_state* production_state_return_head;
	struct production_state* p;
	production_state_return_head = proc;
	int first_ah[50];
	int* re_ahead;
	for (int i = 0; i < 50; i++)
	{
		first_ah[i] = -1;
	}
	int expansion = 1;
	int expansionflag = 0;
	while (expansion)
	{
		p = production_state_return_head;
		while (p != NULL)
		{
			int ex_var = p->prod.rightside[p->state];
			if (ex_var >= 10000){
				int k = 1;
				int n = 0;
				while (p->prod.rightside[p->state + k] != -1)
				{
					first_ah[n++] = p->prod.rightside[p->state + k];
					k++;
				}
				first_ah[n] = p->lookahead;
				re_ahead = firstalpha(first_ah);
				for (int j = 0; j < production_num; j++)
				{
					if (production_list[j].leftside == ex_var)
					{
						expansionflag = 1;//扩充了闭包，应该继续扩充
						int m = 0;
						while (first_ah[m] != -1){
							struct production_state* new_state = (struct production_state *)malloc(sizeof(struct production_state));
							new_state->prod = production_list[j];
							//new_state->next = production_state_return_head->next;
							//production_state_return_head->next = new_state;
							new_state->state = 0;
							new_state->lookahead = first_ah[m];
							if (checkinclosure(production_state_return_head, new_state) == 0)
							{
								new_state->next = production_state_return_head->next;
								production_state_return_head->next = new_state;
							}
						}
					}
				}
			}
			p = p->next;
		}
		if (expansionflag = 0)//无法继续扩充闭包，退出
			expansion = 0;
	}
	return production_state_return_head;
}
int lookupproducitonindex(struct production proc)
{
	int index = 0;
	int equal = 1;
	for (int i = 0; i < production_num; i++)
	{
		equal = 1;
		if (proc.leftside!=production_list[i].leftside)
		{
			equal = 0;
			continue;
		}
		else{
			int k = 0;
			while (proc.rightside[k]!=-1&&production_list[k].rightside[k]!=-1)
			{
				if (proc.rightside[k] != production_list[k].rightside[k]){
					equal = 0;
					break;
				}
			}
		}
		if (equal = 1)
			return i;
	}
}
void makestateset()//利用0号产生式求闭包，接着求后续状态集，得到规范项目集族,填分析表
{
	//struct production_state* stateset[statesetnum];
	int cur_state = 0;
	int continue_flag = 0;
	for (int i = 0; i < statesetnum; i++)
	{
		stateset[i] = NULL;
	}
	stateset[0] = (struct production_state *)malloc(sizeof(struct production_state));
	struct production_state first_state;
	first_state.prod = production_list[0];
	first_state.state = 0;
	first_state.next = NULL;
	first_state.lookahead = sharp;//100='#'
	stateset[0] = closure(&first_state);
	cur_state++;
	int k = 0;
	while (stateset[k] != NULL || continue_flag == 1)
	{
		if (stateset[k] == NULL && continue_flag == 1)
		{
			k = 0;
			continue_flag = 1;
		}
		for (int i = 0; i < tnum; i++)
		{
			if (go(stateset[k], i) != NULL && checkinstateset(go(stateset[k], i)) == 0)//判断后续项目集是否已经属于当前的项目集规范族
			{
				stateset[cur_state++] = closure(go(stateset[k], i));
				action[k][i] = cur_state - 1;//填表//移进项
				continue_flag = 1;
			}
		}
		for (int i = 10000; i < vnum; i++)
		{
			if (go(stateset[k], i) != NULL && checkinstateset(go(stateset[k], i)) == 0)
			{
				stateset[cur_state++] = closure(go(stateset[k], i));
				gototable[k][i] = cur_state - 1;//填表//状态转移项
				continue_flag = 1;
			}
		}
	}
	k = 0;
	while (stateset[k] != NULL)//继续填表//规约项
	{
		struct production_state* p = stateset[k];
		while (p!=NULL)
		{
			if (p->prod.rightside[p->state + 1] == -1)
				action[k][p->lookahead] =10000+ lookupproducitonindex(p->prod);
		}
		k++;
	}
	action[0][ sharp] = acc;
	//return stateset;
}

void innit_environment(char lex[])
{
	FILE *fp = fopen(lex, "r");
	char StrLine[1024] = { 0 };             //每行最大读取的字符数
	if (fp == NULL)
		printf("can not open lex_file");
	while (!feof(fp))
	{
		fgets(StrLine, 1024, fp);  //读取词法分析分析结果
		buffer[buffer_point++] = atoi(StrLine);
	}
	buffer[buffer_point++] = sharp;
	fclose(fp);                     //关闭文件
	buffer_point = 0;
	stack_num[++stack_num_point] = sharp;
	stack_state[++stack_state_point] = 0;
	return 0;
}

int  make_environment()
{
	read_production("grammer.txt");
	innit_table();
	makestateset();
	innit_environment("lex.txt");
	int table_item = -1;
	while (buffer[buffer_point] != -1)
	{
		if (action[stack_state[stack_state_point], buffer[buffer_point]] < 10000)//移进项目
		{
			stack_state[++stack_state_point] = action[stack_state[stack_state_point], buffer[buffer_point]];
			stack_num[++stack_num_point] = buffer[buffer_point++];
		}
		else if (action[stack_state[stack_state_point], buffer[buffer_point]] >10000){//规约项目
			table_item = action[stack_state[stack_state_point], buffer[buffer_point]];
			printf("%d-->", production_list[table_item-10000].leftside);//打印产生式
			int k = 0;
			while (production_list[table_item - 10000].rightside[k] != -1)//产生式右端
			{
				printf("%d", production_list[table_item - 10000].rightside[k]);
					k++;
			}
			stack_num_point = stack_num_point - k;
			stack_state_point = stack_state_point - k;
			stack_num[++stack_num_point] = production_list[table_item - 10000].leftside;//非终结符符压栈
			stack_state[++stack_state_point] = gototable[stack_state[stack_state_point]][stack_num[stack_num_point]];//新状态压栈
		}
		else if (action[stack_state[stack_state_point], buffer[buffer_point]] ==acc){
			return 0;
		}
		else{
			return -1;
		}
	}
}