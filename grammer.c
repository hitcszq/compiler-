#include <stdio.h>
struct production{
	int rightside;//产生式左端
	int leftside[100];//产生式右端， 从10000开始编码非终结符
};
struct production_state{
	struct production prod;
	int state;
};
struct production production_list[200];
static int production_num = 0;
static int vara[500];
static int vara_num;
//产生式A~a
void read_production(char production_file[])
{
	struct production *proc;
	FILE *fp = fopen(production_file, "r");
	char StrLine[1024];             //每行最大读取的字符数
	if (fp == NULL)
		printf("can not open produciton_file");
	while (!feof(fp))
    {
		fgets(StrLine, 1024, fp);  //读取一行
		proc=std_production(StrLine);
		production_list[production_num++] = proc;
	}
    fclose(fp);                     //关闭文件
	return 0;
}

struct production* std_production(char* p)
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
}
struct production_state* closure(production_state  proc)//求闭包
{
	struct production_state production_state_return[50];
	for (int i = 0; i < 50; i++)//设定项目集中产生式状态的默认值
	{
		production_state_return[i].prod.rightside = -1;
	}
	int cur_state = 0;
	production_state_return[cur_state++] = proc;
	int expansion = 1;
	int production_index[50];
	int inflag = 0;
	for (int i = 0; i < production_num; i++)
	{
		if (proc.prod == production_list[i])
		{
			production_index[0] = i;
			break;
		}
	}
	while (expansion)
	{
		for (int i = 0; i < cur_state; i++)
		{
			int ex_var = production_state_return[i].prod.leftside[production_state_return[i].state];
			if (ex_var >= 10000){
				for (int j = 0; j < production_num; j++)
				{
					if (production_list[j].rightside == ex_var)
					{
						for (int k = 0; k < cur_state; k++)//判断该产生式是否已经在闭包中
						{
							if (production_index[k] == j)
							{
								inflag = 1;
								break;
							}
						}
						if (inflag != 1)//不在闭包中
						{
							//inflag = 0;
							production_index[cur_state] = j;
							production_state_return[cur_state].prod.rightside = ex_var;
							production_state_return[cur_state].prod.leftside = production_list[j].leftside;
							production_state_return[cur_state].state = 0;
						}
					}
				}
			}
		}
		if (flag = 0)
			expansion = 0;
		else:
			flag = 1;
	}
}

struct production_state* go(production_state I[], int x)
{
	struct production_state production_state_return[50];//后续项目集
	struct production_state production_stateset_return[50];//后续项目集闭包
	int curstate = 0;
	int curset = 0;
	for (int i = 0; i < 50; i++)//设定项目集中产生式状态的默认值
	{
		production_state_return[i].prod.rightside = -1;
	}
	for (int i = 0; i < 50; i++)
	{
		if (I[i].prod.rightside != -1)//求参数项目集的后续项目集
		{
			if (I[i].prod.leftside[I[i].state] == x)
			{
				production_state_return[curstate++] = I[i];
				(production_state_return[curstate - 1].state)++;
			}
		}
		else
			break;
	}

	
}
