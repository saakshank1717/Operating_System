#include<stdio.h>
#include<stdlib.h>

int PC,last;
int mem[1000],reg[4];
void load()
{
    int address,code,i;
    FILE *fp;
    FILE *fp1;
    char fname[20];
    
    printf("\n ENter File name:");
    scanf("%s",&fname);

    fp=fopen(fname,"r");
    while(!feof(fp))
    {
        fscanf(fp,"%d %d",&address,&code);
        if(address!=-1)
        {
            last=address;
        }
        if(address==-1)
        {
            PC=code;
        }
    }
    fclose(fp);

    fp1=fopen(fname,"r");
    i=PC;
    while(!feof(fp1))
    {
        fscanf(fp1,"%d %d",&address,&code);        
            if(address!=-1)
            {
                mem[i++]=code;
            }
        
    }
    fclose(fp1);

    printf("\n CONTENTS are loaded");
}

void print()
{
    int i;
    printf("\n The contents are:");
    for(i=PC;i<=last;i++)
    {
        printf("\n %d \t %d",i,mem[i]);
    }
}

void execute(int f1)
{
    int temp,target;
    int inst;
    int reg1;
    int flag;
    int i,op1,op2;

    i=PC;
    while(1)
    {
    temp=mem[i++];

    target=temp%1000;
    inst=(temp/1000)/10;
    reg1=(temp/1000)%10;
     

     if(inst==0)
     {
        break;
     }

     switch(inst)
     {
        case 1://Add
               reg[reg1-1]=reg[reg1-1]+mem[target];
               break;
        case 2://Sub
               reg[reg1-1]=reg[reg1-1]-mem[target];
               break;
        case 3://Mul
               reg[reg1-1]=reg[reg1-1]*mem[target];
               break;
        case 4://memory to reg
               reg[reg1-1]=mem[target];
               break;
        case 5://reg to memory
               mem[target]=reg[reg1-1];
            break;
        case 6://load operands for comparison
              op1=reg[reg1-1];
              op2=mem[target];
              break;
        case 7:
                switch (reg1) // Check the type of comparison
                {
                    case 1: 
                        flag = (op1 < op2) ? 1 : 0;
                        break;
                    case 2: 
                        flag = (op1 <= op2) ? 1 : 0;
                        break;
                    case 3: 
                        flag = (op1 == op2) ? 1 : 0;
                        break;
                    case 4: 
                        flag = (op1 > op2) ? 1 : 0;
                        break;
                    case 5: 
                        flag = (op1 >= op2) ? 1 : 0;
                        break;
                    case 6: // Unconditional jump
                        flag = 1; // Always jump
                        break;
                }
                // If the flag is set, update the program counter to target
                if (flag == 1)
                {
                    i = target; // Jump to the target instruction
                }
                break;
        case 8: reg[reg1-1]=reg[reg1-1]/mem[target];
                break;
        case 9://input
               printf("\n Enter value:");
               scanf("%d",&mem[target]);
               break;
        case 10:
               //output
               printf("\n Output:%d",mem[target]);

     }
    }



}
int main()
{
    int ch,i;
    for(i=0;i<4;i++)
    {
        reg[i]=0;
    }
    while(1)
    {
        printf("\n1.LOAD\n2.PRINT\n3.EXECUTE\n4.EXIT");
        printf("\n Enter your choice:");
        scanf("%d",&ch);

        switch(ch)
        {
            case 1:load();
                   break;
           case 2:print();
                  break;
            case 3:execute(1);
            break;
            case 4:exit(0);

                   }
    }
    
}
