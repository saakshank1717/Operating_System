#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct symtab
{
	int index;
	char symb[10];
	int addr,uf,df,cnt;
	struct symtab *next;
}*shead=NULL,*snew,*scur;

struct intcode
{
	int addr,regcode;
	char opcode[20],oper[20];
	struct intcode *next;
}*ihead=NULL,*inew,*icur;

struct tcode
{
	int addr;
	int regcode;
	int oper;
	char opcode[20];
	struct tcode *next;
}*tcode=NULL,*tcur,*tnew;

char *optab[11]={"STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV","READ","PRINT"};
char *regtab[11]={"AREG","BREG","CREG","DREG","LT","LE","EQ","GT","GE","ANY"};
int plc,lc=0;
int symcnt=0;

int searchopc(char sym[10])
{
	int i,flag=0;
	for(int i=0;i<=10;i++)
	{
		if(strcmp(optab[i],sym)==0)
		{
			flag=1;
			break;
		}
	}
	if(flag==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int searchreg(char sym[10])
{
	int i,j,flag=0;
	for(int i=0;i<=10;i++)
	{
		if(strcmp(regtab[i],sym)==0)
		{
			flag=1;
			j=i;
			break;
		}
	}
	if(flag==1)
	{
		if(j<=4)
		{
			inew->regcode=j+1;
		}
		else
		{
			inew->regcode=j-5;
		}
	}
}

int searchopcint(char sym[10])
{
	int i,k,flag=0;
	for(int i=0;i<=10;i++)
	{
		if(strcmp(optab[i],sym)==0)
		{
			flag=1;
			k=i;
			break;
		}
	}
	if(flag==1)
	{
		sprintf(inew->opcode,"<IS,%d>",k);
	}
	else
	{
		return 0;
	}
}

void entersymtab(char sym[10],int value)
{
	struct symtab *temp;
	int flag=0;
	for(temp=shead;temp!=NULL;temp=temp->next)
	{
		if(strcmp(temp->symb,sym)==0)
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		snew=(struct symtab *)malloc(sizeof(struct symtab));
		strcpy(snew->symb,sym);
		switch(value)
		{
			case 0:
				sprintf(inew->oper,"<S,%d>",++symcnt);
				snew->index=symcnt;
				snew->addr=-1;
				snew->uf=1;
				snew->cnt=0;
				break;

			case 1:
				snew->index=++symcnt;
				snew->addr=plc;
				snew->df=1;
				snew->cnt++;
				break;
		}
		snew->next=NULL;
		if(shead==NULL)
		{
			shead=scur=snew;
		}
		else
		{
			scur->next=snew;
			scur=snew;
		}
	}
	else
	{
		switch(value)
		{
			case 0:
				sprintf(inew->oper,"<S,%d>",temp->index);
				temp->uf=1;
				break;

			case 1:
				if(temp->addr==-1)
				{
					temp->addr=plc;
				}
				temp->cnt++;
				temp->df=1;
				break;
		}
	}
}

int main(int argc,char *argv[])
{
	struct symtab *s;
	struct intcode *i;
	FILE *fp;
	char progstmt[80],fname[10];
	char lbl[10],opc[10],reg[10],oper[10];
	char *dcconst;
	int ntok;
	if(argc!=2)
	{
		printf("\n Invalid Number of Arguments. \n");
		exit(1);
	}
	else
	{
		strcpy(fname,argv[1]);
	}
	fp=fopen(fname,"r");
	if(fp==NULL)
	{
		printf("\n FILE NOT FOUND \n");
		exit(1);
	}
	while(!feof(fp))
	{
		inew=(struct intcode *)malloc(sizeof(struct intcode));
		strcpy(progstmt,"");
		strcpy(lbl,"");
		strcpy(opc,"");
		strcpy(reg,"");
		strcpy(oper,"");
		fgets(progstmt,80,fp);
		ntok=sscanf(progstmt,"%s%s%s%s",lbl,opc,reg,oper);
		switch(ntok)
		{
			case 1:
				searchopcint(lbl);
				inew->addr=plc;
				plc=lc;
				lc++;
				if(strcmp(lbl,"END")==0)
				{
					strcpy(inew->opcode,"<AD,02>");
					strcpy(oper,opc);
					strcpy(opc,lbl);
					strcpy(lbl," ");
				}
				break;

			case 2:
				if(strcmp(lbl,"START")==0)
				{
					strcpy(inew->opcode,"<AD,01>");
					strcpy(oper,opc);
					strcpy(opc,lbl);
					strcpy(lbl," ");
					sprintf(inew->oper,"<C,%s>",oper);
					lc=atoi(oper);
				}
				else
				{
					if(searchopcint(lbl))
					{
						plc=lc;
						inew->addr=plc;
					        strcpy(oper,opc);
					        strcpy(opc,lbl);
					        strcpy(lbl," ");
						searchopcint(opc);
						entersymtab(oper,0);
						lc++;
					}
					else
					{
						plc=lc;
						inew->addr=plc;
						searchopcint(opc);
						entersymtab(oper,0);
						lc++;
					}
				}
				break;

			case 3:
				if(strcmp(opc,"DS")==0)
				{
					plc=lc;
					inew->addr=plc;
					strcpy(inew->opcode,"<DL,02>");
					entersymtab(lbl,1);
					lc=lc+atoi(reg);
					dcconst=strtok(reg," ");
					sprintf(inew->oper,"<C,%s>",dcconst);
				}
				if(strcmp(opc,"DC")==0)
				{
					plc=lc;
					inew->addr=plc;
					strcpy(inew->opcode,"<DL,01>");
					entersymtab(lbl,1);
					lc++;
					dcconst=strtok(reg," ");
					sprintf(inew->oper,"<C,%s>",dcconst);
				}
				if((strcmp(opc,"DS")!=0) && (strcmp(opc,"DC")!=0))
				{
					if(searchopcint(lbl))
					{
						plc=lc;
						inew->addr=plc;
						strcpy(oper,reg);
						strcpy(reg,opc);
						strcpy(opc,lbl);
						strcpy(lbl," ");
						searchopcint(opc);
						searchreg(reg);
						entersymtab(oper,0);
						lc++;
					}
					else
					{
						plc=lc;
						inew->addr=plc;
						strcpy(oper,reg);
						strcpy(reg," ");
						searchopcint(opc);
						entersymtab(lbl,1);
						entersymtab(oper,0);
						lc++;
					}
				}
				break;

			case 4:
				plc=lc;
				inew->addr=plc;
				searchopcint(opc);
				searchreg(reg);
				entersymtab(lbl,1);
				entersymtab(oper,0);
				lc++;
				break;
		}
		inew->next=NULL;
		if(ihead==NULL)
		{
			ihead=icur=inew;
		}
		else
		{
			icur->next=inew;
			icur=inew;
		}
	}

	printf("\n**********************************SYMBOL TABLE**********************************\n");
	printf("INDEX\t\tNAME\t\tADDRESS\t\tUSED\t\tDEFINED\t\t\n");
	printf("\n-----------------------------------------------------------------------\n");
	for(s=shead;s!=NULL;s=s->next)
	{
		printf("\n%d\t\t%s\t\t%d\t\t%d\t\t%d\t\t",s->index,s->symb,s->addr,s->uf,s->df);
	}
	printf("\n-----------------------------------------------------------------------\n");
	for(s=shead;s!=NULL;s=s->next)
	{
		if(s->df==0)
		{
			printf("\n ERROR:Symbol %s is used but not defined.\n",s->symb);
		}
		if(s->uf==0)
                {
                        printf("\n WARNING:Symbol %s is defined but not used.\n",s->symb);
                }
		if(s->cnt==0)
                {
                        printf("\n ERROR:Redeclaration of the symbol %s.\n",s->symb);
                }
	}
	printf("\n\n");
	for(i=ihead;i!=NULL;i=i->next)
	{
		printf("\n%d\t\t%s\t%d\t%s\n",i->addr,i->opcode,i->regcode,i->oper);
	}
	fclose(fp);
}
