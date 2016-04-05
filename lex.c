#include <stdio.h>
#include<malloc.h>
#include <stdlib.h>
#include<errno.h>
#define ID 36
#define INT 37
#define AND 1
#define ARRAY 2
#define BEGIN 3
#define CASE 4 
#define CONST 5
#define DIV 6
#define DO 7 
#define DOWNTO 8
#define ELSE 9
#define END 1
//#define FILE 11 
#define FOR 12
#define FUNC 13
#define GOTO 14
#define IF 15
#define IN 16
#define LABEL 17
#define MOD 18
#define NIL 19
#define NOT 20
#define OF 21
#define OR 22
#define PACKED 23
#define PROC 24
#define PROG 25
#define RECORD 26
#define REPEAT 27
#define SET 28
#define THEN 29
#define TO 30
#define TYPE 31
#define UNTIL 32
#define VAR 33
#define WHILE 34
#define WITH 35
#define ASSIGN 57
#define COLON 56
#define COMMA 52
#define EQ 44
#define EXP 60
#define SEMIC 58
#define PLUS 40
#define MINUS 41
#define MULTI 42
#define RDIV 43
#define EQ 44
#define LT 45
#define GT 46
#define LE 47
#define GE 48
#define NE 49
#define bool int
#define true 1
#define false 0
struct token{
int classid;
int var;
};
char cache[1024000];
static int lexeme_begin=0;
static int forward=0;
char* item_table[50];
static int current_item = 0;


void makecache(char filename[])
{
	FILE *fp;
	int i=0;
	char ch;
	fp=fopen(filename,"r");
	if (fp==NULL){
		printf("cannot open sourcefile");
		printf("errno is: %d\n", errno);
		exit;
	}
	while ( (ch=fgetc(fp) )!= EOF){
		cache[i++]=ch;
		if( i==1024000)
		{
			printf ("memory failed");
		}	
	}
	cache[i]=0x0;
}
bool isletter(char ch)
{
	if ((ch >= 'A' && ch<='Z') || (ch>='a' && ch <= 'z'))
		return true;
	else
		return false;
}
bool isletnum(char ch)
{
	if ( isletter(ch) || (ch>='0'&& ch <= '9') )
		return true;
	else
		return false;
}

bool isdigit(char ch)
{
	if (ch>='0' &&ch <= '9')
		return true;
	else
		return false;
}
void retract (int i)
{
	forward -= i;
}
char getcharfromcache()
{	
	return cache[forward++];
} 
int get_token(char* token){
	if(token== "and")
			return AND;
	else if	(token== "array")
			return ARRAY;
	else if (token == "begin")	
			return BEGIN;
	else if (token == "case") 
			return CASE;
	else if (token == "const")
			return CONST;
	else if (token == "div")	
			return DIV;
	else if (token == "do")	
			return DO;
	else if (token == "downto")		
			return DOWNTO;
	else if (token == "else")		
			return ELSE;
	else if (token == "end")	
			return END;
	//else if (token == "file")	
			//return FILE;
	else if (token == "for")
			return FOR;
	else if (token == "function")	
			return FUNC;
	else if (token == "goto")	
			return GOTO;
	else if (token == "if")	
			return IF;
	else if (token == "in")		
			return IN;
	else if (token == "label")	
			return LABEL;
	else if (token == "mod")	
			return MOD;
	else if (token == "nil")	
			return NIL;
	else if (token == "not")	
			return NOT;
	else if (token == "of")	
			return OF;
	else if (token == "or")	
			return OR;
	else if (token == "packed")	
			return PACKED;
	else if (token == "procedure")	
			return PROC;
	else if (token == "program")	
			return PROG;
	else if (token == "record")
			return RECORD;
	else if (token == "repeat")	
			return REPEAT;
	else if (token == "set")
			return SET;
	else if (token == "then")
			return THEN;
	else if (token == "to")
			return TO;
	else if (token == "type")	
			return TYPE;
	else if (token == "until")	
			return UNTIL;
	else if (token == "var")	
			return VAR;
	else if (token == "while")	
			return WHILE;
	else if (token == "with")	
			return WITH;

	else
			return ID;
	
}
int install_id(char token[]){//return the index of the token in item_table
	/*if (current_item==0)
	{
		item_table[current_item]=token;
		return current_item++;
	}
	else{*/
	if (get_token(token)!=ID)
		return -1;
	for (int i=0;i<current_item;i++)
	{
		if(item_table[current_item]==token) return current_item;
	}
	item_table[current_item]=token;
	return current_item++;
	//}
}
char* copytoken()
{
	char* copy=(char*)malloc((forward-lexeme_begin+1)*sizeof(char));
	int j=0;
	for (int i = lexeme_begin; i < forward;i++)
	{
		copy[j++]=cache[i];
	}
	copy[j]=0;
	lexeme_begin = forward;
	printf("%s", copy);
	return copy;
}
struct token* scan()
{
	char ch;
	char* token_scan;
	struct token retoken;
	ch = getcharfromcache();
	while(ch==0x20||ch==0x0A||ch ==0x9){//delete the space enter table
		ch =getcharfromcache();
		lexeme_begin++;
	}
	if (isletter(ch)){
		ch =getcharfromcache();
		while(isletnum(ch))
			{
				ch=getcharfromcache();
			}
		retract(1);
		printf("%d,%d", lexeme_begin, forward);
		token_scan=copytoken();
		retoken.classid = get_token(token_scan);
		retoken.var = install_id(token_scan);
	
		return &retoken;
	}
	else if(isdigit(ch)){
		ch=getcharfromcache();
		while(isdigit(ch)){
			ch=getcharfromcache();
		}
		retract(1);
		token_scan =copytoken();
		retoken.classid = INT; 
		retoken.var = install_id(token_scan);
	
		return &retoken;
	}
	else
	switch (ch){
		case '*':ch=getchar();
			if (ch=='*') 
			{
				retoken.classid = EXP; retoken.var = 0;  return &retoken;
			}
			else{	
				retract(1);
				retoken.classid = MULTI; retoken.var = 0;
				return &retoken;
			}
		
		
		case ':':ch=getchar();
			if (ch == '=') { retoken.classid = ASSIGN; retoken.var = 0; return &retoken; }
			else{
				retract(1);
				retoken.classid = COLON; retoken.var = 0;
				return &retoken;
			}
		
		
		case '<':ch=getcharfromcache();
				if (ch=='=') 
				{	
					retoken.classid = LE; retoken.var = 0;
					return &retoken;
				}
				else if (ch=='>') 
				{
				return (NE,0);
				}
				else{
						retract(1);
						retoken.classid =LT; retoken.var = 0;
						return &retoken;
				  }
				break;
		case '=':retoken.classid=  EQ;retoken.var= 0 ; return &retoken; 
		case '>':ch=getchar();
			if (ch == '=') { retoken.classid = GE; retoken.var = 0;  return &retoken; }
			else{
					 retract(1);
					 retoken.classid = GT; retoken.var = 0;
					 return &retoken;
				}
		case '+':retoken.classid = PLUS; retoken.var = 0; return &retoken;
		case '-':retoken.classid = MINUS; retoken.var = 0; return &retoken; 
		case '/':retoken.classid = RDIV; retoken.var = 0; return &retoken; 
		case ',':retoken.classid = COMMA; retoken.var = 0; return &retoken;  
		case ';':retoken.classid = SEMIC; retoken.var = 0; return &retoken; 
		case 0x0:retoken.classid = 0; retoken.var = 0; return &retoken; 
	}	
}
int main()
{
	makecache("test.pascal");
	for (int i=0;; i++)
	{
		if (cache[i] != 0x0)
			printf("%c", cache[i]);
		else
			break;
	}
	int end=0;
	struct token token_list[100];
	int token_num=0;
	while(end==0)
	{
		struct token *get_token = scan();
		if(get_token->classid==0)
			end=1;
		else
			//lexeme_begin++;
			token_list[token_num++]=*get_token;
	}
	for(int i=0;i<token_num;i++)
	{
		printf("%d,%d \n",token_list[i].classid,token_list[i].var);
	}
}