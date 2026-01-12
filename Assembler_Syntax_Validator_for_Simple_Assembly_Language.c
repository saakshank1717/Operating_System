#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct node
{
    char line[80];
    struct node *next;
} NODE;

NODE *head;

char *mnemo[] = {"STOP", "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", "READ", "PRINT"};
char *reg[] = {"AREG", "BREG", "CREG", "DREG"};
char *storage[] = {"DC", "DS"};
char *directive[] = {"START", "END"};
char *cc[] = {"LT", "LE", "EQ", "GT", "GE", "ANY"};
int noArg = 0,comma_flag=0;

int isMnemonic(char *str)
{
    int i;
    for (i = 0; i < 11; i++)
    {
        if (strcasecmp(mnemo[i], str) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int isRegister(char *str)
{
    int i;
    for (i = 0; i < 4; i++)
    {

        if (strcasecmp(reg[i], str) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int isCC(char *str)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        if (strcasecmp(cc[i], str) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int isDirective(char *str)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        if (strcasecmp(directive[i], str) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int isStorage(char *str)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        if (strcasecmp(storage[i], str) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void remComa(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        char c = str[i];
        if (c == ',')
        {
            str[i] = ' ';
            comma_flag=1;
        }
    }
}
void FileToList(char *line)
{
    NODE *newnode, *temp;
    newnode = (NODE *)malloc(sizeof(NODE));
    strcpy(newnode->line, line);
    newnode->next = NULL;

    // If this is the first line of file:
    if (head == NULL)
    {
        head = newnode;
    }
    else
    {
        temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        // will be at the last node now:
        temp->next = newnode;
    }
}

void PrintList()
{
    NODE *p = head;
    while (p)
    {
        printf("%s\n", p->line);
        p = p->next;
    }
}

void ValidateAssembler()
{
    char *t1, *t2, *t3, *t4,*t5;
    t1 = (char *)malloc(sizeof(char *));
    t2 = (char *)malloc(sizeof(char *));
    t3 = (char *)malloc(sizeof(char *));
    t4 = (char *)malloc(sizeof(char *));
    t5 = (char *)malloc(sizeof(char *));

    NODE *current;
    int lcount = 1;
    for (current = head; current; current = current->next,lcount++)
    {
        comma_flag=0;
        remComa(current->line);

        noArg = sscanf(current->line, "%s %s %s %s %s", t1, t2, t3, t4,t4);
	if(noArg>4)
	{
		printf("\nError on line %d:Invalid Statement\n",lcount);
		printf("\n--------------------------------------------\n");
		continue;
	}
        //        printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
       //         printf("\n Number of tokens=%d\n", noArg);
       // printf("noArg=%d\n", noArg);
        if(noArg==1)
        {
            if ((strcasecmp(t1,"STOP") != 0) && (strcasecmp(t1,"END") != 0) && (strcasecmp(t1,"START") != 0))
            {
                printf("%s", current->line);
		printf("t1=%s\n", t1);
                printf("Error on line %d: Invalid Mnemonic Symbol - Expected START or STOP or END\n\n", lcount);
		printf("\n--------------------------------------------\n");
            }
        }
        if(noArg==2)
        {
            if ((strcasecmp(t2, "STOP") == 0) || (strcasecmp(t2, "END") == 0) || (strcasecmp(t2,"START") == 0))
            {
                if (isMnemonic(t1) != 0 || isRegister(t1) != 0 || isDirective(t1) != 0 || isCC(t1) != 0 || (isStorage(t1) !=0))
                {
                    printf("%s", current->line);
		    printf("t1=%s  t2=%s\n", t1, t2);
                    printf("Error on line %d: Invalid Lable \n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else if ((strcasecmp(t1, "READ")) == 0 || (strcasecmp(t1, "PRINT")) == 0)
            {
                if (isMnemonic(t2) != 0 || isRegister(t2) != 0 || isDirective(t2) != 0 || isCC(t2) != 0 || (isStorage(t2) !=0))
                {
                    printf("%s", current->line);
		    printf("t1=%s  t2=%s\n", t1, t2);
                    printf("Error on line %d: Invalid Variable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            
            else if (strcasecmp(t1, "START") == 0)
            {
                int temp = atoi(t2);
                printf("Memory Address=%d\n", temp);
                if (temp <= 0)
                {
                    printf("%s", current->line);
		    printf("t1=%s  t2=%s\n", t1, t2);
                    printf("#Invalid memory location. can't negative\n\n");
		printf("\n--------------------------------------------\n");
                }
                else if (temp > 999)
                {
                    printf("%s", current->line);
		    printf("t1=%s  t2=%s\n", t1, t2);
                    printf("Error on line %d: #Invalid memory location. can't be greater than 999.\n\n",lcount);
		printf("\n--------------------------------------------\n");
                }
            }
	    else if(strcasecmp(t2,"DS") ==0 || (strcasecmp(t2,"DC") == 0))
	    {
		printf("t1=%s  t2=%s\n", t1, t2);
	    	printf("\nError on line %d: Invalid Declaration\n\n",lcount);
		printf("\n--------------------------------------------\n");
	    }
            else
            {
                printf("%s", current->line);
		printf("t1=%s  t2=%s\n", t1, t2);
                printf("Error on line %d: #Invalid Mnemonics symbol. It must be either of READ, PRINT or START.\n\n",lcount);
		printf("\n--------------------------------------------\n");
            }
        }
        if(noArg==3)
        {
            if ((strcasecmp(t2, "READ") == 0) || (strcasecmp(t2, "PRINT")) == 0)
            {
                if ((isMnemonic(t1) != 0) || (isRegister(t1) != 0) || (isDirective(t1) != 0) || (isCC(t1) != 0) || (isStorage(t1) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t3) != 0) || (isRegister(t3) != 0) || (isDirective(t3) != 0) || (isCC(t3) != 0) || (isStorage(t3) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Variable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else if ((strcasecmp(t2, "DS") == 0) || (strcasecmp(t2, "DC") == 0))
            {
                if ((isMnemonic(t1) != 0) || (isRegister(t1) != 0) || (isDirective(t1) != 0) || (isCC(t1) != 0) || (isStorage(t1) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Variable\nInvalid Declaration\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t3) != 0) || (isRegister(t3) != 0) || (isDirective(t3) != 0) || (isCC(t3) != 0) || (isStorage(t3) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Declaration\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else if ((strcasecmp(t1, "ADD") == 0) || (strcasecmp(t1, "SUB") == 0) || (strcasecmp(t1, "MULT") == 0) || (strcasecmp(t1, "DIV") == 0) || (strcasecmp(t1, "MOVER") == 0) || (strcasecmp(t1, "MOVEM") == 0))
            {
                if (isRegister(t2) == 0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Register\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t3) != 0) || (isRegister(t3) != 0) || (isDirective(t3) != 0) || (isCC(t3) != 0) || (isStorage(t3) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Variable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if(comma_flag==0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Expected ',' between Register and Variable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else if (strcasecmp(t1, "COMP") == 0)
            {
                if ((isMnemonic(t2) != 0) || (isRegister(t2) != 0) || (isDirective(t2) != 0) || (isCC(t2) != 0) || (isStorage(t2) !=0) || (isMnemonic(t3) != 0) || (isRegister(t3) != 0) || (isDirective(t3) != 0) || (isDirective(t3) != 0) || (isCC(t3) != 0) || (isStorage(t3) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Comparison\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
/*                if(comma_flag==0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Expected ',' between two Variables\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
*/
            }
            else if (strcasecmp(t1, "BC") == 0)
            {
                if (isCC(t2) == 0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on line %d: Invalid Condition Code\n\n",lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t3) != 0) || (isRegister(t3) != 0) || (isDirective(t3) != 0) || (isCC(t3) != 0) || (isStorage(t3) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
/*                if(comma_flag==0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Expected ',' between Condition Code & Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
*/
            }
            else if (strcasecmp(t2, "START") == 0)
            {
                int temp = atoi(t3);
                printf("%d", temp);
                if (temp <= 0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("#Invalid memory location. can't negative\n\n");
		printf("\n--------------------------------------------\n");
                }
                else if (temp > 999)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s \n", t1, t2, t3);
                    printf("#Invalid memory location. can't be greater than 999.\n\n");
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t1) != 0) || (isRegister(t1) != 0) || (isDirective(t1) != 0) || (isCC(t1) != 0) || (isStorage(t1) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s\n", t1, t2, t3);
                    printf("Error on Line %d: Invalid Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else
            {
                printf("%s\n", current->line);
		printf("t1=%s  t2=%s  t3=%s \n", t1, t2, t3);
                printf("Error on Line %d: Invalid Statement\n\n", lcount);
		printf("\n--------------------------------------------\n");
            }
        }
        if(noArg==4)
        {
            if ((strcasecmp(t2, "ADD") == 0) || (strcasecmp(t2, "SUB") == 0) || (strcasecmp(t2, "MULT") == 0) || (strcasecmp(t2, "DIV") == 0) || (strcasecmp(t2, "MOVER") == 0) || (strcasecmp(t2, "MOVEM") == 0))
            {
                if ((isMnemonic(t1) != 0) || (isRegister(t1) != 0) || (isDirective(t1) != 0) || (isCC(t1) != 0) || (isStorage(t1) !=0) )
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if (isRegister(t3) == 0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Register\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t4) != 0) || (isRegister(t4) != 0) || (isDirective(t4) != 0) || (isCC(t4) != 0) || (isStorage(t4) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Variable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if(comma_flag==0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Expected ',' between Register and Variable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else if ((strcasecmp(t3, "DS") == 0) || (strcasecmp(t3, "DC") == 0))
            {
                if ((isMnemonic(t2) != 0) || (isRegister(t2) != 0) || (isDirective(t2) != 0) || (isCC(t2) != 0) || (isStorage(t2) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Variable\nInvalid Declaration\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t4) != 0) || (isRegister(t4) != 0) || (isDirective(t4) != 0) || (isCC(t4) != 0) || (isStorage(t4) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Declaration\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t1) != 0) || (isRegister(t1) != 0) || (isDirective(t1) != 0) || (isCC(t1) != 0) || (isStorage(t1) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Lable\nInvalid Declaration\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else if (strcasecmp(t2, "COMP") == 0)
            {
                if((isMnemonic(t1) != 0) || (isRegister(t1) != 0) || (isDirective(t1) != 0) || (isCC(t1) != 0) || (isStorage(t1) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Lable\n\n", lcount);  
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t3) != 0) || (isRegister(t3) != 0) || (isDirective(t3) != 0) || (isCC(t3) != 0) || (isStorage(t3) !=0) || (isMnemonic(t4) != 0) || (isRegister(t4) != 0) || (isDirective(t4) != 0) || (isCC(t4) != 0) || (isStorage(t4) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Comparison\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if(comma_flag==0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Expected ',' between two Variables\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else if (strcasecmp(t2, "BC") == 0)
            {
                if (isCC(t3) == 0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on line %d: Invalid Condition Code\n\n",lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t4) != 0) || (isRegister(t4) != 0) || (isDirective(t4) != 0) || (isCC(t4) != 0) || (isStorage(t4) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if ((isMnemonic(t1) != 0) || (isRegister(t1) != 0) || (isDirective(t1) != 0) || (isCC(t1) != 0) || (isStorage(t1) !=0))
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Invalid Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
                if(comma_flag==0)
                {
                    printf("%s\n", current->line);
		    printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                    printf("Error on Line %d: Expected ',' between Condition Code & Lable\n\n", lcount);
		printf("\n--------------------------------------------\n");
                }
            }
            else
            {
                printf("%s\n", current->line);
		printf("t1=%s  t2=%s  t3=%s  t4=%s \n", t1, t2, t3, t4);
                printf("Error on Line %d: Invalid Statement\n\n", lcount);
		printf("\n--------------------------------------------\n");
            }
        }
    
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nInvalid number of arguments.\n");
        exit(0);
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("\n File does not exists.\n");
        exit(0);
    }

    char data[1000];
    while (fgets(data, 1000, fp))
    {
        FileToList(data);
    }

   // printf("\n\t\t**Reading File**\n");
   // PrintList();
    fclose(fp);
   // printf("\n******* Validation checking**********\n");
    ValidateAssembler();
}
