#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct symtab
{
	char symb[10];
	int addr;
	int uf;
	int df;
	int cnt;
	struct symtab *next;
}NODE;

NODE *shead = NULL;
NODE *scur;
NODE *snewnode;

char *nemo[] = {"STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV","READ","PRINT" };
char *reg[] = {"-1","AREG","BREG","CREG","DREG"};
char *cc[] = {"LT","LE","EQ","GT","GE","ANY"};

int argno = 0;
int plc , lc = 0;

int searchopc(char sym[10])
{
	int i , flag = 0;
	for(i = 0; i < 11 ; i++)
	{
		if(strcasecmp(nemo[i],sym ) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void entersymtab(char sym[10] , int value)
{
	NODE *temp;
	int flag = 0;

	for(temp = shead ; temp != NULL ; temp = temp -> next)
	{
		if(strcmp(temp -> symb, sym) == 0)
		{
			flag = 1;
			break;
		}
	}

	if(flag == 0)
	{
		snewnode = malloc(sizeof(NODE));
		strcpy(snewnode -> symb,sym);

		switch(value)
		{
			case 0:snewnode -> addr = -1;
			       snewnode -> uf = 1;
			       snewnode -> cnt;
			       break;

			case 1:snewnode -> addr = plc;
			       snewnode ->df = 1;
			       snewnode -> cnt++;
			       break;
		}

		snewnode -> next = NULL;

		if(shead == NULL)
		{
			shead = scur = snewnode;
		}
		else
		{
			scur -> next = snewnode;
			scur =snewnode;
		}
	}
	else
	{
		switch(value)
		{
			case 0:temp -> uf = 1;
			       break;

			case 1:if(temp -> addr == -1)
			       {
				       temp -> addr = plc;
			       }
			       temp->cnt++;
			       temp->df = 1;
			       break;
		}
	}
}

void printSymbolTable()
{
	NODE *temp;
	printf("\nSymbol table\n\n");
	printf("Name\t\tAddress\t\tUsed\t\tDefine\t\tLength\n");

	for(temp = shead ; temp != NULL ; temp = temp -> next)
	{
		printf("\n%s\t\t%d\t\t%d\t\t%d\t\t%d",temp->symb,temp->addr,temp->uf,temp->df,temp->cnt);
	}
	printf("\n");
}

void checkSymbolTable()
{
	NODE *s;
	for(s = shead ; s != NULL ; s = s->next)
	{
		if(s->df == 0)
		{
			printf("\nError : Symbol %s is used but not define\n",s->symb);
		}
		if(s->uf == 0)
		{
			printf("\nWarning : Symbol %s is defined but  not used",s->symb);
		}
		if(s->cnt > 1)
		{
			printf("\nError : Redeclaration of symbol %s ",s->symb);
		}
	}
}

int main(int argc,char *argv[],int argNo)
{
	NODE *s;

	FILE *fp;

	char line[100];
	char fname[10],lbl[10],opc[10],reg[10],oper[10];

	if(argc != 2)
	{
		printf("Invalid number of arguments\n");
		exit(0);
	}

	fp = fopen(argv[1],"r");

	if(fp == NULL)
	{
		printf("Unable to open the file\n");
		exit(0);
	}

	while(!feof(fp))
	{
		argNo = 0;
		strcpy(line,"");
		strcpy(fname,"");
		strcpy(lbl,"");
		strcpy(opc,"");
		strcpy(reg,"");
		strcpy(oper,"");

		fgets(line,100,fp);
		printf("%s",line);
		argNo = sscanf(line,"%s %s %s %s",lbl,opc,reg,oper);

		switch(argNo)
		{
			case 1:plc = lc;
			       lc++;
			       break;

			case 2:if(strcasecmp(lbl,"START")==0)
			       {
				       strcpy(oper,opc);
				       strcpy(opc,lbl);
				       strcpy(lbl,"");
				       lc = atoi(oper);
			       }
			       else
			       {
				       if(searchopc(lbl))
				       {
					       plc = lc;
					       strcpy(oper,opc);
					       strcpy(opc,lbl);
					       strcpy(lbl,"");
					       entersymtab(oper,0);
					       lc++;
				       }

				       else
				       {
					       plc = lc;
					       entersymtab(lbl,1);
					       lc++;
				       }

			       }
			       break;

			case 3:if(strcmp(opc,"DS")==0)
			       {
				       plc = lc;
				       entersymtab(lbl,1);
				       lc += atoi(reg);
			       }

			       if(strcmp(opc,"DC")==0)
			       {
				       plc = lc;
				       entersymtab(lbl,1);
				       lc++;
			       }

			       if((strcmp(opc,"DS") != 0) && (strcmp(opc,"DC") != 0))
			       {
				       if(searchopc(lbl))
				       {
					       plc = lc;
					       strcpy(oper,reg);
					       strcpy(reg,opc);
					       strcpy(opc,lbl);

					       entersymtab(oper,0);
					       lc++;
				       }
				       else
				       {
					       plc = lc;
					       strcpy(oper,reg);
					       strcpy(reg,"");
					       entersymtab(lbl,1);
					       entersymtab(oper,0);
					       lc++;
				       }
			       }
			       break;

			case 4:plc = lc;
			       entersymtab(lbl,1);
			       entersymtab(oper,0);
			       lc++;
			       break;
		}
	}

	printSymbolTable();
	checkSymbolTable();

	printf("\n");

	fclose(fp);

	return 0;
}






