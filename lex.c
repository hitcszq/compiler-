#include <stdio.h>
#include<malloc.h>
#include <stdlib.h>
#include<errno.h>
#include <string.h>
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
#define END 10
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
#define LR_BRAC 50
#define RR_BRAC 51
#define Q_MARK 63
#define STRING 39
#define F_STOP 54
#define LS_BRAC 61
#define RS_BRAC 62
#define RANGE 55
#define REAL 38
#define input 64
#define output 65
#define integer 66
#define real 67
#define Ptrue 70
#define Pflase 71
#define step 72
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
		if (ch >= 'A'&&ch <= 'Z')
		{
			ch = ch - ('A' - 'a');
		}
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
	else if (strcmp(token, "array") == 0)
			return ARRAY;
	else if (strcmp(token, "begin") == 0)
			return BEGIN;
	else if (strcmp(token, "case") == 0)
			return CASE;
	else if (strcmp(token, "const") == 0)
			return CONST;
	else if (strcmp(token, "div") == 0)
			return DIV;
	else if (strcmp(token, "do") == 0)
			return DO;
	else if (strcmp(token, "downto") == 0)
			return DOWNTO;
	else if (strcmp(token, "else") == 0)
			return ELSE;
	else if (strcmp(token, "end") == 0)
			return END;
	//else if (token == "file")	
			//return FILE;
	else if (strcmp(token, "for") == 0)
			return FOR;
	else if (strcmp(token, "function") == 0)
			return FUNC;
	else if (strcmp(token, "goto") == 0)
			return GOTO;
	else if (strcmp(token, "if") == 0)
			return IF;
	else if (strcmp(token, "in") == 0)
			return IN;
	else if (strcmp(token, "label") == 0)
			return LABEL;
	else if (strcmp(token, "mod") == 0)
			return MOD;
	else if (strcmp(token, "nil") == 0)
			return NIL;
	else if (strcmp(token, "not") == 0)
			return NOT;
	else if (strcmp(token, "of") == 0)
			return OF;
	else if (strcmp(token, "or") == 0)
			return OR;
	else if (strcmp(token, "packed") == 0)
			return PACKED;
	else if (strcmp(token, "procedure") == 0)
			return PROC;
	else if (strcmp(token,"program")==0)	
			return PROG;
	else if (strcmp(token, "record") == 0)
			return RECORD;
	else if (strcmp(token, "repeat") == 0)
			return REPEAT;
	else if (strcmp(token, "set") == 0)
			return SET;
	else if (strcmp(token, "then") == 0)
			return THEN;
	else if (strcmp(token, "to") == 0)
			return TO;
	else if (strcmp(token, "type") == 0)
			return TYPE;
	else if (strcmp(token, "until") == 0)
			return UNTIL;
	else if (strcmp(token, "var") == 0)
			return VAR;
	else if (strcmp(token, "while") == 0)
			return WHILE;
	else if (strcmp(token, "with") == 0)
		return WITH;
	else if (strcmp(token, "input") == 0)
		return input;
	else if (strcmp(token, "output") == 0)
		return output;
	else if (strcmp(token, "integer") == 0)
		return integer;
	else if (strcmp(token, "real") == 0)
		return real;
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
		if(strcmp(item_table[i],token)==0) return i;
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
		printf("\n");
		printf("%d,%d", lexeme_begin, forward);
		printf("\n");
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
		if (ch = '.')
		{
			ch = getcharfromcache();
			while (isdigit(ch)){
				ch = getcharfromcache();
			}
			retract(1);
			printf("\n");
			printf("%d,%d", lexeme_begin, forward);
			printf("\n");
			token_scan = copytoken();
			retoken.classid = REAL;
			retoken.var = install_id(token_scan);
			return &retoken;
		}
		retract(1);
		printf("\n");
		printf("%d,%d", lexeme_begin, forward);
		printf("\n");
		token_scan =copytoken();
		retoken.classid = INT; 
		retoken.var = install_id(token_scan);
	
		return &retoken;
	}
	else if (ch=='\''){
		ch = getcharfromcache();
		while (ch!='\''){
			ch = getcharfromcache();
		}
		//retract(1);
		printf("\n");
		printf("%d,%d", lexeme_begin, forward);
		printf("\n");
		token_scan = copytoken();
		retoken.classid = STRING;
		retoken.var = install_id(token_scan);

		return &retoken;
	}
	else
	switch (ch){
	    case '*':ch = getcharfromcache(); 
			if (ch=='*') 
			{
				retoken.classid = EXP; retoken.var = 0;   lexeme_begin+=2; return &retoken;
			}
			else{	
				retract(1);
				retoken.classid = MULTI; retoken.var = 0;
				lexeme_begin++;
				return &retoken;
			}
		
		
		case ':':ch = getcharfromcache();
			if (ch == '=') { retoken.classid = ASSIGN; retoken.var = 0; lexeme_begin+=2; return &retoken; }
			else{
				retract(1);
				retoken.classid = COLON; retoken.var = 0;
				lexeme_begin++;
				return &retoken;
			}
		
		
		case '<':ch = getcharfromcache();
				if (ch=='=') 
				{	
					retoken.classid = LE; retoken.var = 0;
					lexeme_begin+=2;
					return &retoken;
				}
				else if (ch=='>') 
				{
				lexeme_begin+=2;
				return (NE,0);
				}
				else{
						retract(1);
						retoken.classid =LT; retoken.var = 0;
						lexeme_begin++;
						return &retoken;
				  }
		case '=':retoken.classid = EQ; retoken.var = 0; lexeme_begin++; return &retoken;
		case '>':ch = getcharfromcache(); 
			if (ch == '=') { retoken.classid = GE; retoken.var = 0; lexeme_begin+=2;  return &retoken; }
			else{
					 retract(1);
					 retoken.classid = GT; retoken.var = 0;
					 lexeme_begin++;
					 return &retoken;
				}
		case '+':retoken.classid = PLUS; retoken.var = 0; lexeme_begin++; return &retoken;
		case '-':retoken.classid = MINUS; retoken.var = 0; lexeme_begin++; return &retoken;
		case '/':retoken.classid = RDIV; retoken.var = 0; lexeme_begin++; return &retoken;
		case ',':retoken.classid = COMMA; retoken.var = 0;lexeme_begin++; return &retoken;
		case ';':retoken.classid = SEMIC; retoken.var = 0; lexeme_begin++; return &retoken;
		case '(':retoken.classid = LR_BRAC; retoken.var = 0; lexeme_begin++; return &retoken;
		case ')':retoken.classid = RR_BRAC; retoken.var = 0; lexeme_begin++; return &retoken;
		case '[':retoken.classid = LS_BRAC; retoken.var = 0; lexeme_begin++; return &retoken;
		case ']':retoken.classid = RS_BRAC; retoken.var = 0; lexeme_begin++; return &retoken;
		case '.':ch = getcharfromcache();
			if (ch == '.') { retoken.classid = RANGE; retoken.var = 0; lexeme_begin+=2; return &retoken; }
			else{
				retract(1);
				retoken.classid = F_STOP; retoken.var = 0; lexeme_begin++;
				return &retoken;
			}
		case 0x0:retoken.classid = 0; retoken.var = 0; return &retoken; 

	}	
}
int main()
{
	makecache("test2.txt");
	FILE *fp = fopen("lex.txt", "w+");
	for (int i=0;; i++)
	{
		if (cache[i] != 0x0)
			printf("%c", cache[i]);
		else
			break;
	}
	printf("\n");
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
	printf("\n");
	for(int i=0;i<token_num;i++)
	{
		fprintf(fp, "%d\n", token_list[i].classid);
		printf("%d,%d \n",token_list[i].classid,token_list[i].var);
	}
}