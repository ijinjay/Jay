//
//  main.c
//  acm
//
//  Created by 靳杰 on 12/17/13.
//  Copyright (c) 2013 靳杰. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct word_map{
	const char *key_word;
	int value;
} word_map;

const word_map MAP_TABLE[] = {
	{"main",1}, {"int",2}, {"float",3}, {"double",4}, {"char",5},
	{"if",6}, {"else",7}, {"do",8}, {"while",9}, {"#var",10},
	{"#number",20}, {"=",21}, {"+",22},
	{"-",23}, {"*",24}, {"/",25}, {"(",26}, {")",27}, {"{",28}, {"}",29},
	{",",30}, {";",31}, {">",32}, {">=",33}, {"<",34}, {"<=",35},
	{"==",36}, {"!=",37}, {"[",38}, {"]",39}, {"#",0}
};

char ifStack[BUFSIZ][BUFSIZ];
int ifLen=0;
void pushIf(char *ifSentence) {
	strcpy(ifStack[ifLen++], ifSentence);
}
char *popIf() {
	return ifStack[--ifLen];
}

typedef struct exp {
    char *expression;
    char *result;
} exp;

const int MAP_TABLE_LENGTH = sizeof(MAP_TABLE) / sizeof(word_map);

char gBuf[BUFSIZ];
char finalInput[BUFSIZ];
int gBuflen;
int finalInputlen;
char wordResult[BUFSIZ][BUFSIZ];
int wordIndex[BUFSIZ];
int wordlen;
char gStack[BUFSIZ];
int top = 0;
int tapnum = 1;
//  opStack is used to store operator
int opStack[BUFSIZ];
int opTop = 0;

int finalIndex=100;
char finalResult[BUFSIZ];
int ifNumber = 0;

void opPush(int i){
	opStack[opTop++] = i;
}
int opPop(){
	return opStack[--opTop];
}
//This is the Stack of word analysis
void push(char ch){
	gStack[top++] = ch;
}
char pop(){
	char ch = gStack[--top];
	gStack[top] = '\0';
	return ch;
}
// delete notes
int notes(char *ch,int *i) {
	if (*ch == '/'){
		char temp;
		if ('*' == (temp = getchar())) {
			*ch = getchar();
			temp = getchar();
			while(!(*ch == '*' && temp == '/')) {
				*ch = temp;
				temp = getchar();
			}
			gBuf[(*i)++] = ' ';
			*ch = getchar();
		}
		else {
			gBuf[(*i)++] = '/';
			if (temp == '/') {
				notes(ch,i);
			}
			else
				*ch = temp;
		}
		return 1;
	}
	return 0;
}

void input() {
	char ch;
	int i=0;
	ch = getchar();
	while('#' != ch) {
		if (ch == '\t' || ch == '\n') {
			gBuf[i++] = ' ';
			ch = getchar();
			continue;
		}
		if(notes(&ch,&i))
			continue;
		gBuf[i++] = ch;
		ch = getchar();
	}
	gBuf[i++] = '#';
	gBuflen = i;
}

void deletespace() {
	int i=0;
	int j=0;
	for ( ; i < gBuflen; ++i) {
		if (gBuf[i] == ' ') {
			while(gBuf[++i] == ' ');
			finalInput[j++] = ' ';
		}
		finalInput[j++] = gBuf[i];
	}
	finalInputlen = j;
}

int queryTable(const char *s) {
	int i = 0;
	for (; i < MAP_TABLE_LENGTH; ++i) {
		const char *key_word = MAP_TABLE[i].key_word;
		if (!strcmp(s,key_word)) {
			return MAP_TABLE[i].value;
		}
	}
	return -1;
}

int digit(char ch,int *i,int *j,int *wordlen) {
	while(isdigit(ch)) {
		gBuf[(*j)++] = ch;
		ch = finalInput[++(*i)];
	}
	(*i)--;
	if (ch == '.') {
		(*i)++;
		gBuf[(*j)++] = ch;
		ch = finalInput[++(*i)];
		if (!isdigit(ch)) {
			char strtemp[BUFSIZ];
			strcpy(strtemp,gBuf);
			strcat(strtemp," is error! There is no digit after .");
			wordIndex[*wordlen] = -1;
			strcpy(wordResult[(*wordlen)++],strtemp);
			(*i)--;return 0;
		}
		while(isdigit(ch)) {
			gBuf[(*j)++] = ch;
			ch = finalInput[++(*i)];
		}
		(*i)--;
	}
	if (ch == 'e') {
		(*i)++;
		gBuf[(*j)++] = ch;
		ch = finalInput[++(*i)];
		if (!(isdigit(ch) || ch == '+'||ch == '-')) {
			char strtemp[BUFSIZ];
			strcpy(strtemp,gBuf);
			strcat(strtemp," is error! There is no digit after e");
			wordIndex[*wordlen] = -1;
			strcpy(wordResult[(*wordlen)++],strtemp);(*i)--;return 0;
		}
		else if (isdigit(ch)) {
			gBuf[(*j)++] = ch;
			ch = finalInput[++(*i)];
			while(isdigit(ch)) {
				gBuf[(*j)++] = ch;
				ch = finalInput[++(*i)];
			}
			(*i)--;
		}
		else {
			gBuf[(*j)++] = ch;
			ch = finalInput[++(*i)];
			if (!isdigit(ch)) {
				printf("(%s,there is no digit after %c)\n", gBuf,gBuf[(*j)-1]);(*i)--;
				wordIndex[*wordlen] = -1;
				char strtemp[BUFSIZ];
				strcpy(strtemp,gBuf);
				strcat(strtemp," is error! There is no digit after the last letter");
				strcpy(wordResult[(*wordlen)++],strtemp);
				return 0;
			}
			while(isdigit(ch)) {
				gBuf[(*j)++] = ch;
				ch = finalInput[++(*i)];
			}
			(*i)--;
		}
	}
	wordIndex[*wordlen] = 20;
	strcpy(wordResult[(*wordlen)++],gBuf);
	return 1;
}
// word parse
void parse() {
	int i = 0;
	char ch;
	wordlen = 0;
	for (; i < finalInputlen; ++i) {
		memset(gBuf,0,BUFSIZ);
		ch = finalInput[i];
		int j=0;
		if (isalpha(ch)) {
			gBuf[j++] = ch;
			ch = finalInput[++i];
			while(isalpha(ch) || isdigit(ch)) {
				gBuf[j++] = ch;
				ch = finalInput[++i];
			}
			i--;
			if (queryTable(gBuf) != -1)
			{
				wordIndex[wordlen] = queryTable(gBuf);
				strcpy(wordResult[wordlen++],gBuf);
			}
			else
			{
				wordIndex[wordlen] = 10;
				strcpy(wordResult[wordlen++],gBuf);
			}
		}
		else if (isdigit(ch)||ch == '+'||ch == '-') {
			gBuf[j++] = ch;
			if (isdigit(ch)) {
				ch = finalInput[++i];
				digit(ch,&i,&j,&wordlen);
			}
			else {
				ch = finalInput[++i];
				if (!isdigit(ch)) {
					wordIndex[wordlen] = queryTable(gBuf);
					strcpy(wordResult[wordlen++],gBuf);
					i--;
					continue;
				}
				else {
					if ((((i>=2)&&( isdigit(finalInput[i-2]) || isalpha(finalInput[i-2])))||(finalInput[i-2]==')'))
						||((i>=3)&&(isspace(finalInput[i-2]) && (isdigit(finalInput[i-3]) || isalpha(finalInput[i-3])) ))){
						wordIndex[wordlen] = queryTable(gBuf);
						strcpy(wordResult[wordlen++],gBuf);
						i--;
						continue;
					}
					digit(ch,&i,&j,&wordlen);
				}
			}
		}
		else {
			switch(ch) {
				case ' ':break;
				case '>':{
					ch = finalInput[++i];
					if (ch == '=') {
						wordIndex[wordlen] = queryTable(">=");
						strcpy(wordResult[wordlen++],">=");
					}
					else {
						wordIndex[wordlen] = queryTable(">");
						strcpy(wordResult[wordlen++],">");
						i--;
					}
				};break;
				case '<':{
					ch = finalInput[++i];
					if (ch == '=') {
						wordIndex[wordlen] = queryTable("<=");
						strcpy(wordResult[wordlen++],"<=");
					}
					else {
						wordIndex[wordlen] = queryTable("<");
						strcpy(wordResult[wordlen++],"<");
						i--;
					}
				};break;
				case '=':{
					ch = finalInput[++i];
					if (ch == '=') {
						wordIndex[wordlen] = queryTable("==");
						strcpy(wordResult[wordlen++],"==");
					}
					else {
						wordIndex[wordlen] = queryTable("=");
						strcpy(wordResult[wordlen++],"=");
						i--;
					}
				};break;
				case '!':{
					ch = finalInput[++i];
					if (ch == '=') {
						wordIndex[wordlen] = queryTable("!=");
						strcpy(wordResult[wordlen++],"!=");
					}
					else {
						wordIndex[wordlen] = -1;
						strcpy(wordResult[wordlen++],"! is error,this is not a recognised letter");
						i--;
						
					}
				};break;
				default:
                    gBuf[j++] = ch;
                    wordIndex[wordlen] = queryTable(gBuf);
                    if (queryTable(gBuf) != -1) {
                        strcpy(wordResult[wordlen++],gBuf);
                    }
                    else {
                        char strtemp[BUFSIZ];
                        strcpy(strtemp,gBuf);
                        strcat(strtemp," is error. This is not a recognised letter");
                        strcpy(wordResult[wordlen++],strtemp);
                    }
                    break;
			}
		}
	}
}
int printWordResult()
{
	int wordResultIndex=0;
	printf("******* The Result of Wordanlyze *******\n");
	int i = 0;
	int errorNum = 0;
	for (i = 0; i < wordlen; ++i) {
		
		if (wordIndex[i] == -1) {
			printf("%d\t(error,%s)\n",wordResultIndex++, wordResult[i]);
			errorNum ++;
		}
		else if (wordIndex[i] == 10) {
			printf("%d\t(10,'%s')\n",wordResultIndex++, wordResult[i]);
		}
		else if (wordIndex[i] == 20) {
			printf("%d\t(20,%g)\n",wordResultIndex++, atof(wordResult[i]));
		}
		else
			printf("%d\t(%d,%s)\n",wordResultIndex++, wordIndex[i],wordResult[i]);
	}
	return errorNum;
}

char wordToG(int i){
	switch(i){
		case 1 :return 'm';//main
		case 6 :return 'i';//if
		case 7 :return 'e';//else
		case 8 :return 'd';//do
		case 9 :return 'w';//while
		case 21:return '=';
		case 22:return '+';
		case 23:return '-';
		case 24:return '*';
		case 25:return '/';
		case 26:return '(';
		case 27:return ')';
		case 28:return '{';
		case 29:return '}';
		case 31:return ';';
		case 32:return '>';
		case 33:return 'g';//>=
		case 34:return '<';
		case 35:return 'l';//<=
		case 36:return 'q';//==equal
		case 37:return 'n';//!=not
		case 38:return '[';
		case 39:return ']';
		case 0 :return '$';
		case 20:return 'N';
		case 10:return 't';//ID
		default:break;
	}
	return ' ';
}
int isVT(char x){
	return (   x== '(' || x== ')' || x== '+' || x== '-'
			|| x== '*' || x== '/' || x== 't' || x== 'N'
			|| x== 'm' || x== 'i' || x== 'e' || x== 'd'
			|| x== 'w' || x== '=' || x== '>' || x== '<'
			|| x== 'g' || x== 'l' || x== 'q' || x== 'n'
			|| x== '[' || x== ']' || x== '{' || x== '}'
			|| x== ';' );
}
int checkMatrix(char x,char a,int *i){
	if ( x=='S'&&a=='m') {
		push('A');push(')');push('(');push('m');
		printf("S->main()A");tapnum = 0;
		return 1;
	}
	else if (x=='A' && a=='{') {
		push('}');push('B');push('{');
		printf("A->{B}");
		return 1;
	}
	else if (x=='B' && (a=='i'||a=='d'||a=='t')) {
		push('c');push(';');push('C');
		printf("B->C;c");
		return 1;
	}
	else if (x=='C') {
		if (a == 'i'){
			push('E');
			printf("C->E");
			return 1;
		}
		else if (a == 'd'){
			push('F');
			printf("C->F");
			return 1;
		}
		else if (a=='t'){
			push('D');
			printf("C->D");
			return 1;
		}
		else if (a == '}') {
			printf("C-> ");
			return 1;
		}
		else
			return 0;
	}
	else if (x == 'c') {
		if (a=='}'){
			printf("c-> ");
			return 1;
		}
		else if (a == 't' ||a=='i'||a=='d') {
			push('c');push(';');push('C');
			printf("c->C;c");
			return 1;
		}
		else
			return 0;
	}
	else if (x == 'D' && a=='t'){
		push('G');push('=');push('t');
		printf("D->id=G");
		return 1;
	}
	else if (x == 'E' && a=='i') {
		push('X');push('A');push('H');push('i');
		printf("E->if HA X");tapnum = 0;
		return 1;
	}
	else if (x == 'X') {
		if (a=='e') {
			push('A');push('e');
			printf("X->else A");tapnum = 0;
			return 1;
		}
		else if (a == ';'){
			printf("X-> ");
			return 1;
		}
		else
			return 0;
	}
	else if (x == 'F' && a=='d') {
		push('H');push('w');push('A');push('d');
		printf("F->do A while H");tapnum = 0;
		return 1;
	}
	else if (x == 'H' && (a=='t' || a== 'N' || a=='(')) {
		push('G');push('I');push('G');
		printf("H->GIG");
		return 1;
	}
	else if ( x=='G' && (a=='('||a=='t'||a=='N')) {
		push('j');push('J');
		printf("G->Jj");
		return 1;
	}
	else if ( x=='I' ) {
		switch(a) {
			case '>':push('>');printf("I->>");;return 1;
			case 'g':push('g');printf("I->>=");;return 1;
			case '<':push('<');printf("I-><");;return 1;
			case 'l':push('l');printf("I-><=");;return 1;
			case 'q':push('q');printf("I->==");;return 1;
			case 'n':push('n');printf("I->!=");;return 1;
			default:
				return 0;
		}
	}
	else if (x=='J'&&(a=='('||a=='t'||a=='N')) {
		push('k');push('K');
		printf("J->Kk");
		return 1;
	}
	else if (x=='j'||x=='k') {
		if (a==')') {
			printf("%c-> \t", x);
			return 1;
		}
		else if (a=='+') {
			push('j');push('J');push('+');
			printf("j->+Jj");
			return 1;
		}
		else if (a=='-') {
			push('j');push('J');push('-');
			printf("j->-Jj");
			return 1;
		}
		else if (a == '*') {
			push('k');push('K');push('*');
			printf("k->*Kk");
			return 1;
		}
		else if (a == '/') {
			push('k');push('K');push('/');
			printf("k->/Kk");
			return 1;
		}
		else if (a == '{' ||a =='>' ||a=='<' ||a=='q'||
				 a == 'g' ||a == 'l'||a==';' ||a=='n'||a==')'){
			printf("%c-> \t", x);
			return 1;
		}
		else
			return 0;
	}
	else if (x=='K') {
		if (a == '(') {
			push(')');push('G');push('(');
			printf("K->(G)");
			return 1;
		}
		else if (a=='t') {
			push('t');
			printf("K->id");
			return 1;
		}
		else if (a=='N') {
			printf("K->num");
			push('N');return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}
int stepNum=0;

void printStack(){
	int j = 0;
	if(tapnum){
		printf("\t");
	}
	printf("\t%d\t", ++stepNum);
	for (; j < top; ++j){
		printf("%c", gStack[j]);
	}
	tapnum = 1;
}
int grammarParse() {
	push('$');
	push('S');
	printf("Rule\t\tStep\tStack\n");
	printf("\t\t0\t$S\n");
	int i = 0;
	char a;
	char x;
	for (i = 0; i < wordlen; ++i){
		a = wordToG(wordIndex[i]);
    FOO:
		x = pop();
		if (isVT(x)) {
			if (x == a){
				printStack();
				printf("\n");
				continue;
			}
			else {
				printf("Error1!The position is %d\n",i);return 0;
			}
		}
		else {
			if (x == '$') {
				if ( x == a) {
					printf("Success\n");break;
				}
				else {
					printf("Error2!The postition is %d\n",i);
					return 0;
				}
			}
			else {
				if (!checkMatrix(x,a,&i)) {
					printf("(x=%c,a=%c)\n", x,a);
					printf("Error3!The postition is %d\n",i);
					return 0;
				}
				else{
					printf("\b");
					printStack();
					printf("\n");
					goto FOO;
				}
			}
		}
		printStack();
		printf("\n");
	}
	return 1;
}

int tempnum=0;		//Use tempnum to generate a temp variable as T1,T2,T3
char *newtemp(){
	char *p;
	char m[8];
	p = (char *)malloc(8);
	sprintf(m,"%d",tempnum++);
	strcpy(p+1,m);
	p[0]='T';
	return p;
}

int compareOp(int x){
	switch(x){
		case 22:
		case 23:if (wordIndex[opStack[opTop-1]] != 26)
            return 1;
        else
            return 0;
		case 24:
		case 25:if (wordIndex[opStack[opTop-1]] == 25 || wordIndex[opStack[opTop-1]] == 24)
            return 1;
        else
            return 0;
		case 26:return 3;
		case 27:return 4;
		default:return 0;
	}
}
//This Stack is used to get the equation.
char expressionBuf[BUFSIZ][BUFSIZ];
void pushResult(char *g) {
	strcpy(expressionBuf[top++],g);
}
char *popResult() {
	return expressionBuf[--top];
}

exp expression(int *i,int isAssignment){
	int tempResult[BUFSIZ];
	int tindex = 0,k;
	int operatorNum = 0;
	exp expressionResult;
	expressionResult.expression = (char *)malloc(sizeof(char)*BUFSIZ);
	expressionResult.result = (char *)malloc(sizeof(char)*20);
	while(!(wordIndex[*i] >= 28 && wordIndex[*i] <= 37)){
		if (wordIndex[*i] == 10 || wordIndex[*i] == 20) {
			tempResult[tindex++]= (*i);
		}
		else {
			if (opTop != 0) {
				int x;
				switch(compareOp(wordIndex[*i])){
					case 0:	break;
					case 1:	x=opPop();operatorNum++;
                        tempResult[tindex++]= x;
                        if (opTop != 0 && wordIndex[opStack[opTop-1]]<=23) {
                            x=opPop();operatorNum++;
                            tempResult[tindex++]= x;
                        }
                        break;
					case 3:	break;
					case 4:	while(wordIndex[(x=opPop())] != 26){
                        tempResult[tindex++]= x;operatorNum++;
                    };(*i)++;continue;
					default:break;
				}
			}
			opPush((*i));
		}
		(*i)++;
	}
	while(opTop){
		tempResult[tindex++] = opPop();operatorNum++;
	}
	if (tindex == 1) {
		if (isAssignment) {
			sprintf(expressionResult.expression,"(%d)\t=\t%s\t-\t",finalIndex++,wordResult[tempResult[0]]);
		}
		else {
			sprintf(expressionResult.result,"%s",wordResult[tempResult[0]]);
		}
	}
	else {
		for (k = 0; k < tindex; ++k) {
			if (!(22 <= wordIndex[tempResult[k]] && wordIndex[tempResult[k]] <= 25)) {
				pushResult(wordResult[tempResult[k]]);
			}
			else {
				char *arg2=popResult();
				char *arg1=popResult();
				char *result=newtemp();
				char temp[BUFSIZ];
				sprintf(temp,"(%d)\t%s\t%s\t%s\t%s\n",finalIndex++,wordResult[tempResult[k]],arg1,arg2,result);
				sprintf(expressionResult.expression, "%s%s",expressionResult.expression,temp);
                
				pushResult(result);
				if (k == tindex-1) {
					if (isAssignment) {
						sprintf(temp, "(%d)\t=\t%s\t-\t",finalIndex++,wordResult[tempResult[0]]);
						strcat(expressionResult.expression,result);
					}
					else
						strcat(expressionResult.result, result);
				}
			}
		}
	}
	(*i) ++;// Point at next element after ';'
	return expressionResult;
}
char* semanticAnalysis(int *i);

char * assignment(int *i) {
	char result[BUFSIZ];
	strcpy(result,wordResult[*i]);
	*i += 2;
	char *temp;
	temp = (char *)malloc(BUFSIZ*sizeof(char));
	sprintf(temp,"%s%s\n",expression(i,1).expression,result);
	if (!ifNumber) {
		strcat(finalResult, temp);
		return NULL;
	}
	else
		return temp;
}

char * compareStatement(int *i,int type){
	char *compareStr;
	compareStr = (char*)malloc(BUFSIZ*sizeof(char));
	exp left=expression(i,0);
	(*i) --;
	char *op=wordResult[*i];
	(*i)++;
	exp right=expression(i,0);
	sprintf(compareStr, "%s%s",left.expression,right.expression);
	if (type) {
		char temp[BUFSIZ];
		sprintf(temp,"(%d)\tif %s%s%s goto (%d)\n",finalIndex++, left.result,op,right.result,type);
		strcat(compareStr, temp);
		if (!ifNumber) {
			strcat(finalResult,compareStr);
			return NULL;
		}
		else
			return compareStr;
	}
	else {
		char temp[BUFSIZ];
		sprintf(temp,"(%d)\tif %s%s%s goto (%d)\n(%d)\tgoto\t",finalIndex++, left.result,op,right.result,finalIndex+1,finalIndex++);
		strcat(compareStr, temp);
		return compareStr;
	}
}

char *condition(int *i){
	(*i) ++;
	ifNumber ++;
	char * fChain = compareStatement(i,0);
	char * semanStr = semanticAnalysis(i);
	(*i) ++;
	ifNumber --;
	char *temp;
	temp = (char *)malloc(BUFSIZ*sizeof(char));
	if (wordIndex[*i] == 7) {
		(*i) += 2;
		int falseIndex = finalIndex;
		finalIndex ++;
		char * elsesemStr = semanticAnalysis(i);
		sprintf(temp, "%s(%d)\n%s(%d)\tgoto\t(%d)\n%s",fChain,falseIndex+1,semanStr,falseIndex,finalIndex,elsesemStr);
	}
	else {
		sprintf(temp, "%s(%d)\n%s",fChain,finalIndex,semanStr);
	}
	(*i) +=2;
	if (!ifNumber) {
		strcat(finalResult, temp);
		return NULL;
	}
	else {
		return temp;
	}
}

char* doloop(int *i){
	int loopIndex = finalIndex;
	(*i)+=2;
	char *temp;
	temp = (char*)malloc(BUFSIZ*(sizeof(char)));
	char *semStr = semanticAnalysis(i);
	(*i)+=2;
	char *comStr = compareStatement(i,loopIndex);
	sprintf(temp,"%s%s",semStr,comStr);
	if (!ifNumber) {
		strcat(finalResult, temp);
		return NULL;
	}
	else {
		return temp;
	}
}

char *semanticAnalysis(int *i){
	char *temp;
	temp = (char*)malloc(BUFSIZ*sizeof(char));
	switch(wordIndex[*i]){
		case 10:sprintf(temp,"%s%s",temp,assignment(i));break;
		case 6 :sprintf(temp,"%s%s",temp,condition(i));break;
		case 8 :sprintf(temp,"%s%s",temp,doloop(i));break;
		case 29:return NULL;
		default:return NULL;
	};
	if(ifNumber){
		sprintf(temp,"%s%s",temp,semanticAnalysis(i));
		return temp;
	}
	else{
		semanticAnalysis(i);
		return NULL;
	}
}

int main(int argc, char const *argv[]){
	int errorNum = 0;
	printf("Please enter your code...\n");
	input();
	deletespace();
	
	printf("%s\n", finalInput);
	printf("-----------------------------------------\n");
	parse();
	if( errorNum = printWordResult() )
		printf("There are %d errors, please check your code!\n", errorNum);
	else {
		printf("********* GrammarParse Result **********\n");
		if(grammarParse()){		//After the success of grammarParse ,do semanticAnalysis
			printf("********* Semantic Analysis Result **********\n");
			int i = 4;
			semanticAnalysis(&i);
			printf("%s\n", finalResult);
			printf("-------------end------------\n");
		}
	}
	getchar();
	getchar();
	return 0;
}

