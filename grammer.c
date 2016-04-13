#include <stdio.h>
struct production{
	int leftside;//����ʽ���
	int rightside[100];//����ʽ�Ҷˣ� ��10000��ʼ������ս��,10000����P'������Լ�ս��־ ��0��ʼ�����ս������Ӧ�ĺ꣩��0��ʾ�գ�
	struct production* next;
};
struct production_state{
	struct production prod;
	int state;
	struct production_state* next;
};
struct production production_list[200];
static int production_num = 0;
static int vara[500];
static int vara_num;
//����ʽA~a
void read_production(char production_file[])
{
	struct production *proc;
	FILE *fp = fopen(production_file, "r");
	char StrLine[1024];             //ÿ������ȡ���ַ���
	if (fp == NULL)
		printf("can not open produciton_file");
	while (!feof(fp))
    {
		fgets(StrLine, 1024, fp);  //��ȡһ��
		proc=std_production(StrLine);
		production_list[production_num++] = proc;
	}
    fclose(fp);                     //�ر��ļ�
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
struct production_state* closure(production_state*  proc)//��հ�
{
	struct production_state* production_state_return_head;
	struct production_state* p;
	production_state_return_head = proc;
	int expansion = 1;
	int expansionflag = 0;
	while (expansion)
	{
		p = production_state_return_head;
		while (p!=NULL)
		{
			int ex_var = p->prod.rightside[p->state];
			if (ex_var >= 10000){
				for (int j = 0; j < production_num; j++)
				{
					if (production_list[j].leftside == ex_var &&checkinclosure(production_state_return_head, production_list[j])==0)
					{
							expansionflag = 1;//�����˱հ���Ӧ�ü�������
							struct production_state* new_state = (struct production_state *)malloc(sizeof(struct production_state));
							new_state->prod = production_list[j];
							new_state->next = production_state_return_head->next;
							production_state_return_head->next = new_state;
							new_state->state = 0;
					}
				}
			}
			p = p->next;
		}
		if (expansionflag = 0)//�޷���������հ����˳�
			expansion = 0;
	}
}
int checkinclosure(production_state *head, production *pr)
{
	int inflag = 0;
	while (head!=NULL)
	{
		if (*pr = head->prod)//����ʽ�ṹ��ֵ�ıȽϣ��������ܻ������
		{
			return inflag;
			break;
		}
		head = head->next;
	}
	return inflag;
}


struct production_state* go(production_state* I, int x)
{
	//struct production_state* production_state_return=NULL;//������Ŀ��
	struct production_state* production_stateset_return = (struct production_state *)malloc(sizeof(struct production_state));//������Ŀ���հ�
	struct production_state* pI = I;
	struct production_state* p_return = NULL;
	
	while (pI!=NULL){
			if (pI->prod.leftside[pI->state] == x)
			{
				if (p_return == NULL)//������Ŀ����û��Ԫ��
				{
					production_stateset_return->next = NULL;
					production_stateset_return->state = pI->state + 1;
					production_stateset_return->prod = pI->prod;
					p_return = production_stateset_return;
				}
				else{
					p_return = (struct production_state *)malloc(sizeof(struct production_state));
					p_return->next = production_stateset_return->next;
					p_return->state = pI->state + 1;
					production_stateset_return->prod = pI->prod;
				}
			}
			pI = pI->next;
	}
}
int * firstalpha(int * alpha)
int * first(production G[], int x)
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
			if (G[i].leftside == x && (G[i].rightside[0] < 10000))//���ַ����ս��
			{
				if (checkinfirst(G[i].rightside[0],re_first)==0)
					re_first[cur_first++] = G[i].rightside[0];
			}
		}
		while (continue_flag)
		{
			for (int i = 0; i < production_num; i++)
			{
				if (G[i].leftside == x && (G[i].rightside[0] >= 10000))//���ַ��Ƿ��ս��
				{
					for (int j = 0; j < production_num; j++)
					{
						if (G[j].leftside == G[i].rightside[0] && first(G, G[j].leftside)[0] != -1)//�ݹ���first(G,G[j].rightside)
						{
							int k = 0;
							while (checkinfirst(first(G, G[j].leftside)[k], re_first) && first(G, G[j].leftside)[k]!=-1)
							{
								re_first[cur_first++] = first(G, G[j].rightside)[k++];
							}
						}
						else
						{
							continue_flag = 0;
						}
					}
				}
			}
		}
	}
}

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

struct production_state** makestateset(production_state G[])//�������еĲ���ʽ�����ĳ�ʼ״̬��
{
	struct production_state stateset[100][50];
	production_state start_state[50];

	start_state = closure(G[0]);

}
