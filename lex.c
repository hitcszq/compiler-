#include <stdio.h>
#define ID 36
#define INT 37
struct token{
int class;
int var;
}
char cache[1024000];

void makecache(char filename[])
{
	FILE *fp;
	i=0;
	*fp=fopen(filename,'r');
	if (fp==NULL){
		printf("cannot open sourcefile");
	}
	while(feof(fp)){
		cache[i++]=fgetc(fp);
		if( i==1024000)
		{
			printf ("memory failed");
		}	
	}
	cache[i]=0x0;
}
static lexeme_begin=0;
static forward=0;
char* item_table[50];
void retract (int i)
{
	forward-=i+1
}
char getcharfromcache()
{	
	return cache[forward++];
}
int get_token(char* token){
	switch (token){
		case "and":
			return AND;
		case "array":
			return ARRAY;
		case "begin":
			return BEGIN;
		case "case":
			return CASE;
		case "const":
			return CONST;
		case "div":
			return DIV;
		case "do":
			return DO;
		case "downto":
			return DOWNTO;
		case "else":
			return ELSE;
		case "end":
			return END;
	}	
}
void install_id()

token scan()
{
	char ch;
	char* token;
	ch =getcharfromcache()
	struct token retoken;
	while(ch==0x20||ch==0x0A||ch ==0x9){//delete the space enter table
		ch =getcharfromcache();
		lexeme_begin++;
	}
	if (isletter(ch)){
		ch =getcharfromcache();
		while(isalnum(ch))
			{
				ch=getchar();
			}
		retract(1);
		token=copytoken();
		retoken={get_token(token),install_id(token)};
		return retoken;
	}
	else if(isdigit(ch)){
	ch=getchar();
	while(isdigit(ch)){
	ch=getchar();
	}
	retract(1);
	token =copytoken();
	retoken={INT,install_num(token)};
	return retoken;
	}
	else:
	switch (ch){
		case '*':ch=getchar();
		if (ch=='*') 
		{retoken={EXP,0} ; return retoken;}
		else{	
			retract(1);
			retoken={MULTI,0};
			return retoken;
		}
		break;
		
		
		case ':':ch=getchar();
		if (ch=='=') {retoken ={ASSIGN,0};return retoken;}
		else{
			retract(1);
			retoken={CONLON ,0};
			return retoken;
		}
		break;
		
		
		case '<':ch=getchar();
				if (ch=='=') 
				{	
				retoken={LE,0};
				return retoken;
				}
				else if (ch=='>') 
				{
				return (NE,0);
				}
				else{
						retract(1);
						retoken={CONLON ,0};
						return retoken;
				  }
				break;
		case '=':retoken={EQ,0};return retoken;break;
		case '>':ch=getchar();
		if (ch=='=') retoken=(LE,0);return retoken;
		else if (ch=='>') retoken={GE,0};return retoken;
		else{
				 retract(1);
				 retoken={GT ,0};
				 return retoken;
			}
		break;
		case '+':retoken={PLUS,0};return retoken;break;
		case '-':retoken={MINUS,0};return retoken;break;
		case '/':retoken={RDIV,0};return retoken;break;
		case ',':retoken={COMMA,0};return retoken;break;
		case ';':retoken={SEMIC,0};return retoken;break;
		case 0x0:retoken={0,0};return token;break; 
	}	
}
int main(int argc,char* argv[])
{
	makecache(argv[1]);
	int end=0;
	struct token token_list[100];
	int token_num=0;
	while(end==0)
	{
		struct token get_token;
		get_token=scan()
		if(get_token.class==0)
			end=1;
		else
			lexeme_begin++;
			token_list[token_num++]=get_token;
	}
	for(int i=0;i<token_num;i++)
	{
		printf("%d,%d \n",token_list[i].class,token_list[i].var);
	}
}