#include<stdio.h>
#include<string.h>
#include<math.h>
#include <stdlib.h>


struct symtbl{
  char name[100];
  int location;
};

struct ins{
  char mne[100];
  char op1[100];
  char op2[100];
  char op3[100];
  char op4[100];
  char op5[100];
  char type[100];
}insset[1000];

struct symtbl st[1000];
struct symtbl vt[1000];
struct symtbl jt[1000];
struct ins inst[1000];

int vars=0,jumps=0,labels=0;

void remove_comments(char s[][1000],int *p)
{
  int i,j,k,n;
  n=*p;
  for(i=0;i<n;i++)
  {
    printf("%s",s[i]);
    for(j=0;j<strlen(s[i])-1;j++)
    {
      if(s[i][j]=='/' && s[i][j+1]=='/')
      {
        s[i][j]='\n';
        s[i][j+1]='\0';
        break;
      }
      else if(s[i][j]=='/' && s[i][j+1]=='*')
      {
        int flag=0;
        for(k=j+2;s[i][k+1]!='\0';k++)
        {
          if(s[i][k]=='*' && s[i][k+1]=='/')
          {
            s[i][j]='\n';
            s[i][j+1]='\0';
            flag=1;
            break;
          }
        }
        if(flag==0)
        {
          for(int l=i+1;l<n;l++)
          {
            int f=0;
            for(int m=0;s[l][m+1]!='\0';m++)
            {
              if(s[l][m]=='*' && s[l][m+1]=='/')
              {
                f=1;
                break;
              }
            }
            if(f==1)
            {
              s[i][j]='\n';
              s[i][j+1]='\0';
              s[l][0]='\0';
              break;
            }
            else
            {
              s[l][0]='\0';
            }
          }
        }
      }
    }
  }
  for(i=0;i<n;i++)
  {
    int f=0;
    for(j=0;s[i][j]!='\0';j++)
    {
      if(s[i][j]!=' ' || s[i][j]!='\n')
        f=1;
    }
    if(f==0 || s[i][0]=='\0' || (s[i][0]=='\n' && s[i][1]=='\0'))
    {
      for(k=i;k<n-1;k++)
      {
        strcpy(s[k],s[k+1]);
      }
      n--;
    }
  }
  *p=n;
}

void asmRead(char s[][1000])
{
  FILE * f = fopen("program.asm","r");
  //printf("\nin asm");
  int i=0,n=100;
  while(fgets(s[i++],n,f)){
    //printf("\n%s",s[i-1]);
  };
  fclose(f);
  //printf("\nASM.......");
}


void checkInstructionType(char s[],int i)
{
  char *o1=NULL;
  char *o2=NULL;
  char *o3=NULL;
  char temptok[100];
  strcpy(temptok,s);
  char *token=strtok(temptok,"\n\t\r ");

  if(token==NULL) ;
  else if (strcmp(token,"ld")==0)      //------------LD INSTRUCTION---------------------
  {
    //printf("\n////////////////////");
      strcpy(insset[i].mne,token);
      o1 = strtok(NULL,"\t ,");
      //printf("\n.%s.",o1);
      strcpy(insset[i].op1,o1);
      o2 = strtok(NULL,"\t ,(");
      //printf("\n.%s.",o2);
      strcpy(insset[i].op2,o2);
      o2 = strtok(NULL,"\t ,)\n");
      //printf("\n.%s.",o2);
      strcpy(insset[i].op3,o2);
      strcpy(insset[i].type,"DS");
  }
  else if (strcmp(token,"add")==0) //----------------- ADD -------------------------------
  {
      strcpy(insset[i].mne,token);
      o1 = strtok(NULL,"\t ,");
      strcpy(insset[i].op1,o1);
      o2 = strtok(NULL,"\t ,(");
      strcpy(insset[i].op2,o2);
      o2 = strtok(NULL,"\t ,)\n");
      strcpy(insset[i].op3,o2);
      strcpy(insset[i].type,"XO");
  }
  else if (strcmp(token,"addi")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL," ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,(\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"addis")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"and")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"andi")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"extsw")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o1 = strtok(NULL,"\t ,\n");
    strcpy(insset[i].op2,o1);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"nand")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"or")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"ori")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"subf")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"XO");
  }
  else if (strcmp(token,"xor")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"xori")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"lwz")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"std")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"DS");
  }
  else if (strcmp(token,"stw")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"stwu")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"lhz")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"lha")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"sth")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"lbz")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"stb")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"rlwinm")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op3,o2);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op4,o2);
    o2 = strtok(NULL,"\t ,\n");
    strcpy(insset[i].op5,o2);
    strcpy(insset[i].type,"M");
  }
  else if (strcmp(token,"sld")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"srd")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"srad")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"sradi")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"XS");
  }
  else if (strcmp(token,"b")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,\n");
    strcpy(insset[i].op1,o1);
    //o2 = strtok(NULL,"\t ,(");
    //strcpy(insset[i].op2,o2);
    //o2 = strtok(NULL,"\t ,)\n");
    //strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"I");
  }
  else if (strcmp(token,"ba")==0)
  {
      //to be added
      strcpy(insset[i].mne,token);
      o1 = strtok(NULL,"\t ,\n");
      strcpy(insset[i].op1,o1);
      strcpy(insset[i].type,"I");

  }
  else if (strcmp(token,"bl")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,\n");
    strcpy(insset[i].op1,o1);
    //o2 = strtok(NULL,"\t ,(");
    //strcpy(insset[i].op2,o2);
    //o2 = strtok(NULL,"\t ,)\n");
    //strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"I");
  }
  else if (strcmp(token,"bclr")==0)
  {
    strcpy(insset[i].mne,token);
    o1=strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o1 = strtok(NULL,"\t ,\n");
    strcpy(insset[i].op2,o1);
    strcpy(insset[i].type,"XL");

  }
  else if (strcmp(token,"bc")==0)
  {
    strcpy(insset[i].mne,token);
    o1=strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o1=strtok(NULL,"\t ,");
    strcpy(insset[i].op2,o1);
    o1 = strtok(NULL,"\t ,\n");
    strcpy(insset[i].op3,o1);
    strcpy(insset[i].type,"B");
  }
  else if (strcmp(token,"bca")==0)
  {
    strcpy(insset[i].mne,token);
    o1=strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o1=strtok(NULL,"\t ,");
    strcpy(insset[i].op2,o1);
    o1=strtok(NULL,"\t ,\n");
    strcpy(insset[i].op3,o1);
    strcpy(insset[i].type,"B");
  }
  else if (strcmp(token,"cmp")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"cmpi")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if(strcmp(token,"mr")==0)
  {
    strcpy(insset[i].mne,"addi");
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o1 = strtok(NULL,"\t\n ");
    strcpy(insset[i].op2,o1);
    strcpy(insset[i].op3,"0");
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"subi")==0)
  {
    strcpy(insset[i].mne,"addi");
    o1 = strtok(NULL," ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,(\n");
    char ch[]="-";
    strcat(ch,o2);
    strcpy(insset[i].op3,ch);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"subis")==0)
  {
    strcpy(insset[i].mne,"addis");
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,)\n");
    char ch[]="-";
    strcat(ch,o2);
    strcpy(insset[i].op3,ch);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"sub")==0)
  {
    strcpy(insset[i].mne,"subf");
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op3,o2);
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op2,o2);
    strcpy(insset[i].type,"XO");
  }
  else if (strcmp(token,"sc")==0)
  {
    strcpy(insset[i].mne,token);
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    strcpy(insset[i].type,"SC");
  }
  else if (strcmp(token,"xnop")==0)
  {
    strcpy(insset[i].mne,"xori");
    //o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,"0");
    //o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,"0");
    //o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,"0");
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"slwi")==0)
  {
    strcpy(insset[i].mne,"rlwinm");
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op2,o2);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op3,o2);
    int n=atoi(o2);
    n=31-n;
    //o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op4,"0");
    //o2 = strtok(NULL,"\t ,\n");
    char a[3]="";
    int ghi=0;
    if (n<10) {
      /* code */
      a[0]='0';
    }
    while(n>0)
    {
      int d=n%10;
      n=n/10;
      a[1-ghi]=d+48;
      ghi++;
    }
    strcpy(insset[i].op5,a);
    strcpy(insset[i].type,"M");
  }
  else if (strcmp(token,"la")==0)
  {
    strcpy(insset[i].mne,"addi");
    o1 = strtok(NULL," ,");
    strcpy(insset[i].op1,o1);
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op2,o2);
    //o2 = strtok(NULL,"\t ,(\n");
    //strcpy(insset[i].op3,o2);
    int flag=0;
    for(int i=0;i<vars;i++)
    {
      if(strcmp(vt[i].name,o2)==0)             // incomplete
      {
        int n=vt[i].location;
        flag=1;
        char a[3]="";
        int ghi=0;
        if (n<10) {
          /* code */
          a[0]='0';
        }
        while(n>0)
        {
          int d=n%10;
          n=n/10;
          a[2-ghi]=d+48;
          ghi++;
        }
        break;
      }
    }
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"li")==0)
  {
    strcpy(insset[i].mne,"addi");
    o1 = strtok(NULL," ,");
    strcpy(insset[i].op1,o1);
    //o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op2,"0");
    o2 = strtok(NULL,"\t ,(\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"lis")==0)
  {
    strcpy(insset[i].mne,"addis");
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,o1);
    //o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,"0");
    o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else if (strcmp(token,"cmpd")==0)
  {
    strcpy(insset[i].mne,"cmp");
    //o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,"7");
    //o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op2,"1");
    o2 = strtok(NULL,"\t ,");
    strcpy(insset[i].op3,o2);
    o2 = strtok(NULL,"\t ,\n");
    strcpy(insset[i].op4,o2);
    strcpy(insset[i].type,"X");
  }
  else if (strcmp(token,"cmpdi")==0)
  {
    strcpy(insset[i].mne,"cmpi");
    o1 = strtok(NULL,"\t ,");
    strcpy(insset[i].op1,"7");
    o2 = strtok(NULL,"\t ,(");
    strcpy(insset[i].op2,o1);
    //o2 = strtok(NULL,"\t ,)\n");
    strcpy(insset[i].op3,o2);
    strcpy(insset[i].type,"D");
  }
  else //------WHAT IS ENCOUNTERED IS NOT AN INSTRUCTION BUT A LABEL. UPDATE THE LABEL TABLE--------
  {                                           // no space between label name and :
      strcpy(insset[i].type,"label");
  }
}






char mapreg(struct ins insset[],char tr[][33],int i,int j)
{
  if(j==1)
  {
    if(strcmp(insset[i].op1,"0")==0)
    {
      strcat(tr[i],"00000");
    }
    else if(strcmp(insset[i].op1,"1")==0)
    {
      strcat(tr[i],"00001");
    }
    else if(strcmp(insset[i].op1,"2")==0)
    {
      strcat(tr[i],"00010");
    }
    else if(strcmp(insset[i].op1,"3")==0)
    {
      strcat(tr[i],"00011");
    }
    else if(strcmp(insset[i].op1,"4")==0)
    {
      strcat(tr[i],"00100");
    }
    else if(strcmp(insset[i].op1,"5")==0)
    {
      strcat(tr[i],"00101");
    }
    else if(strcmp(insset[i].op1,"6")==0)
    {
      strcat(tr[i],"00110");
    }
    else if(strcmp(insset[i].op1,"7")==0)
    {
      strcat(tr[i],"00111");
    }
    else if(strcmp(insset[i].op1,"8")==0)
    {
      strcat(tr[i],"01000");
    }
    else if(strcmp(insset[i].op1,"9")==0)
    {
      strcat(tr[i],"01001");
    }
    else if(strcmp(insset[i].op1,"10")==0)
    {
      strcat(tr[i],"01010");
    }
    else if(strcmp(insset[i].op1,"11")==0)
    {
      strcat(tr[i],"01011");
    }
    else if(strcmp(insset[i].op1,"12")==0)
    {
      strcat(tr[i],"01100");
    }
    else if(strcmp(insset[i].op1,"13")==0)
    {
      strcat(tr[i],"01101");
    }
    else if(strcmp(insset[i].op1,"14")==0)
    {
      strcat(tr[i],"01110");
    }
    else if(strcmp(insset[i].op1,"15")==0)
    {
      strcat(tr[i],"01111");
    }
    else if(strcmp(insset[i].op1,"16")==0)
    {
      strcat(tr[i],"10000");
    }
    else if(strcmp(insset[i].op1,"17")==0)
    {
      strcat(tr[i],"10001");
    }
    else if(strcmp(insset[i].op1,"18")==0)
    {
      strcat(tr[i],"10010");
    }
    else if(strcmp(insset[i].op1,"19")==0)
    {
      strcat(tr[i],"10011");
    }
    else if(strcmp(insset[i].op1,"20")==0)
    {
      strcat(tr[i],"10100");
    }
    else if(strcmp(insset[i].op1,"21")==0)
    {
      strcat(tr[i],"10101");
    }
    else if(strcmp(insset[i].op1,"22")==0)
    {
      strcat(tr[i],"10110");
    }
    else if(strcmp(insset[i].op1,"23")==0)
    {
      strcat(tr[i],"10111");
    }
    else if(strcmp(insset[i].op1,"24")==0)
    {
      strcat(tr[i],"11000");
    }
    else if(strcmp(insset[i].op1,"25")==0)
    {
      strcat(tr[i],"11001");
    }
    else if(strcmp(insset[i].op1,"26")==0)
    {
      strcat(tr[i],"11010");
    }
    else if(strcmp(insset[i].op1,"27")==0)
    {
      strcat(tr[i],"11011");
    }
    else if(strcmp(insset[i].op1,"28")==0)
    {
      strcat(tr[i],"11100");
    }
    else if(strcmp(insset[i].op1,"29")==0)
    {
      strcat(tr[i],"11101");
    }
    else if(strcmp(insset[i].op1,"30")==0)
    {
      strcat(tr[i],"11110");
    }
    else if(strcmp(insset[i].op1,"31")==0)
    {
      strcat(tr[i],"11111");
    }
  }
  else if(j==2)
  {
    if(strcmp(insset[i].op2,"0")==0)
    {
      strcat(tr[i],"00000");
    }
    else if(strcmp(insset[i].op2,"1")==0)
    {
      strcat(tr[i],"00001");
    }
    else if(strcmp(insset[i].op2,"2")==0)
    {
      strcat(tr[i],"00010");
    }
    else if(strcmp(insset[i].op2,"3")==0)
    {
      strcat(tr[i],"00011");
    }
    else if(strcmp(insset[i].op2,"4")==0)
    {
      strcat(tr[i],"00100");
    }
    else if(strcmp(insset[i].op2,"5")==0)
    {
      strcat(tr[i],"00101");
    }
    else if(strcmp(insset[i].op2,"6")==0)
    {
      strcat(tr[i],"00110");
    }
    else if(strcmp(insset[i].op2,"7")==0)
    {
      strcat(tr[i],"00111");
    }
    else if(strcmp(insset[i].op2,"8")==0)
    {
      strcat(tr[i],"01000");
    }
    else if(strcmp(insset[i].op2,"9")==0)
    {
      strcat(tr[i],"01001");
    }
    else if(strcmp(insset[i].op2,"10")==0)
    {
      strcat(tr[i],"01010");
    }
    else if(strcmp(insset[i].op2,"11")==0)
    {
      strcat(tr[i],"01011");
    }
    else if(strcmp(insset[i].op2,"12")==0)
    {
      strcat(tr[i],"01100");
    }
    else if(strcmp(insset[i].op2,"13")==0)
    {
      strcat(tr[i],"01101");
    }
    else if(strcmp(insset[i].op2,"14")==0)
    {
      strcat(tr[i],"01110");
    }
    else if(strcmp(insset[i].op2,"15")==0)
    {
      strcat(tr[i],"01111");
    }
    else if(strcmp(insset[i].op2,"16")==0)
    {
      strcat(tr[i],"10000");
    }
    else if(strcmp(insset[i].op2,"17")==0)
    {
      strcat(tr[i],"10001");
    }
    else if(strcmp(insset[i].op2,"18")==0)
    {
      strcat(tr[i],"10010");
    }
    else if(strcmp(insset[i].op2,"19")==0)
    {
      strcat(tr[i],"10011");
    }
    else if(strcmp(insset[i].op2,"20")==0)
    {
      strcat(tr[i],"10100");
    }
    else if(strcmp(insset[i].op2,"21")==0)
    {
      strcat(tr[i],"10101");
    }
    else if(strcmp(insset[i].op2,"22")==0)
    {
      strcat(tr[i],"10110");
    }
    else if(strcmp(insset[i].op2,"23")==0)
    {
      strcat(tr[i],"10111");
    }
    else if(strcmp(insset[i].op2,"24")==0)
    {
      strcat(tr[i],"11000");
    }
    else if(strcmp(insset[i].op2,"25")==0)
    {
      strcat(tr[i],"11001");
    }
    else if(strcmp(insset[i].op2,"26")==0)
    {
      strcat(tr[i],"11010");
    }
    else if(strcmp(insset[i].op2,"27")==0)
    {
      strcat(tr[i],"11011");
    }
    else if(strcmp(insset[i].op2,"28")==0)
    {
      strcat(tr[i],"11100");
    }
    else if(strcmp(insset[i].op2,"29")==0)
    {
      strcat(tr[i],"11101");
    }
    else if(strcmp(insset[i].op2,"30")==0)
    {
      strcat(tr[i],"11110");
    }
    else if(strcmp(insset[i].op2,"31")==0)
    {
      strcat(tr[i],"11111");
    }
  }
  else if(j==3)
  {
    if(strcmp(insset[i].op3,"0")==0)
    {
      strcat(tr[i],"00000");
    }
    else if(strcmp(insset[i].op3,"1")==0)
    {
      strcat(tr[i],"00001");
    }
    else if(strcmp(insset[i].op3,"2")==0)
    {
      strcat(tr[i],"00010");
    }
    else if(strcmp(insset[i].op3,"3")==0)
    {
      strcat(tr[i],"00011");
    }
    else if(strcmp(insset[i].op3,"4")==0)
    {
      strcat(tr[i],"00100");
    }
    else if(strcmp(insset[i].op3,"5")==0)
    {
      strcat(tr[i],"00101");
    }
    else if(strcmp(insset[i].op3,"6")==0)
    {
      strcat(tr[i],"00110");
    }
    else if(strcmp(insset[i].op3,"7")==0)
    {
      strcat(tr[i],"00111");
    }
    else if(strcmp(insset[i].op3,"8")==0)
    {
      strcat(tr[i],"01000");
    }
    else if(strcmp(insset[i].op3,"9")==0)
    {
      strcat(tr[i],"01001");
    }
    else if(strcmp(insset[i].op3,"10")==0)
    {
      strcat(tr[i],"01010");
    }
    else if(strcmp(insset[i].op3,"11")==0)
    {
      strcat(tr[i],"01011");
    }
    else if(strcmp(insset[i].op3,"12")==0)
    {
      strcat(tr[i],"01100");
    }
    else if(strcmp(insset[i].op3,"13")==0)
    {
      strcat(tr[i],"01101");
    }
    else if(strcmp(insset[i].op3,"14")==0)
    {
      strcat(tr[i],"01110");
    }
    else if(strcmp(insset[i].op3,"15")==0)
    {
      strcat(tr[i],"01111");
    }
    else if(strcmp(insset[i].op3,"16")==0)
    {
      strcat(tr[i],"10000");
    }
    else if(strcmp(insset[i].op3,"17")==0)
    {
      strcat(tr[i],"10001");
    }
    else if(strcmp(insset[i].op3,"18")==0)
    {
      strcat(tr[i],"10010");
    }
    else if(strcmp(insset[i].op3,"19")==0)
    {
      strcat(tr[i],"10011");
    }
    else if(strcmp(insset[i].op3,"20")==0)
    {
      strcat(tr[i],"10100");
    }
    else if(strcmp(insset[i].op3,"21")==0)
    {
      strcat(tr[i],"10101");
    }
    else if(strcmp(insset[i].op3,"22")==0)
    {
      strcat(tr[i],"10110");
    }
    else if(strcmp(insset[i].op3,"23")==0)
    {
      strcat(tr[i],"10111");
    }
    else if(strcmp(insset[i].op3,"24")==0)
    {
      strcat(tr[i],"11000");
    }
    else if(strcmp(insset[i].op3,"25")==0)
    {
      strcat(tr[i],"11001");
    }
    else if(strcmp(insset[i].op3,"26")==0)
    {
      strcat(tr[i],"11010");
    }
    else if(strcmp(insset[i].op3,"27")==0)
    {
      strcat(tr[i],"11011");
    }
    else if(strcmp(insset[i].op3,"28")==0)
    {
      strcat(tr[i],"11100");
    }
    else if(strcmp(insset[i].op3,"29")==0)
    {
      strcat(tr[i],"11101");
    }
    else if(strcmp(insset[i].op3,"30")==0)
    {
      strcat(tr[i],"11110");
    }
    else if(strcmp(insset[i].op3,"31")==0)
    {
      strcat(tr[i],"11111");
    }
  }
  else if(j==4)
  {
      if(strcmp(insset[i].op4,"0")==0)
      {
        strcat(tr[i],"00000");
      }
      else if(strcmp(insset[i].op4,"1")==0)
      {
        strcat(tr[i],"00001");
      }
      else if(strcmp(insset[i].op4,"2")==0)
      {
        strcat(tr[i],"00010");
      }
      else if(strcmp(insset[i].op4,"3")==0)
      {
        strcat(tr[i],"00011");
      }
      else if(strcmp(insset[i].op4,"4")==0)
      {
        strcat(tr[i],"00100");
      }
      else if(strcmp(insset[i].op4,"5")==0)
      {
        strcat(tr[i],"00101");
      }
      else if(strcmp(insset[i].op4,"6")==0)
      {
        strcat(tr[i],"00110");
      }
      else if(strcmp(insset[i].op4,"7")==0)
      {
        strcat(tr[i],"00111");
      }
      else if(strcmp(insset[i].op4,"8")==0)
      {
        strcat(tr[i],"01000");
      }
      else if(strcmp(insset[i].op4,"9")==0)
      {
        strcat(tr[i],"01001");
      }
      else if(strcmp(insset[i].op4,"10")==0)
      {
        strcat(tr[i],"01010");
      }
      else if(strcmp(insset[i].op4,"11")==0)
      {
        strcat(tr[i],"01011");
      }
      else if(strcmp(insset[i].op4,"12")==0)
      {
        strcat(tr[i],"01100");
      }
      else if(strcmp(insset[i].op4,"13")==0)
      {
        strcat(tr[i],"01101");
      }
      else if(strcmp(insset[i].op4,"14")==0)
      {
        strcat(tr[i],"01110");
      }
      else if(strcmp(insset[i].op4,"15")==0)
      {
        strcat(tr[i],"01111");
      }
      else if(strcmp(insset[i].op4,"16")==0)
      {
        strcat(tr[i],"10000");
      }
      else if(strcmp(insset[i].op4,"17")==0)
      {
        strcat(tr[i],"10001");
      }
      else if(strcmp(insset[i].op4,"18")==0)
      {
        strcat(tr[i],"10010");
      }
      else if(strcmp(insset[i].op4,"19")==0)
      {
        strcat(tr[i],"10011");
      }
      else if(strcmp(insset[i].op4,"20")==0)
      {
        strcat(tr[i],"10100");
      }
      else if(strcmp(insset[i].op4,"21")==0)
      {
        strcat(tr[i],"10101");
      }
      else if(strcmp(insset[i].op4,"22")==0)
      {
        strcat(tr[i],"10110");
      }
      else if(strcmp(insset[i].op4,"23")==0)
      {
        strcat(tr[i],"10111");
      }
      else if(strcmp(insset[i].op4,"24")==0)
      {
        strcat(tr[i],"11000");
      }
      else if(strcmp(insset[i].op4,"25")==0)
      {
        strcat(tr[i],"11001");
      }
      else if(strcmp(insset[i].op4,"26")==0)
      {
        strcat(tr[i],"11010");
      }
      else if(strcmp(insset[i].op4,"27")==0)
      {
        strcat(tr[i],"11011");
      }
      else if(strcmp(insset[i].op4,"28")==0)
      {
        strcat(tr[i],"11100");
      }
      else if(strcmp(insset[i].op4,"29")==0)
      {
        strcat(tr[i],"11101");
      }
      else if(strcmp(insset[i].op4,"30")==0)
      {
        strcat(tr[i],"11110");
      }
      else if(strcmp(insset[i].op4,"31")==0)
      {
        strcat(tr[i],"11111");
      }
  }
  else if(j==5)
  {
    if(strcmp(insset[i].op5,"0")==0)
    {
      strcat(tr[i],"00000");
    }
    else if(strcmp(insset[i].op5,"1")==0)
    {
      strcat(tr[i],"00001");
    }
    else if(strcmp(insset[i].op5,"2")==0)
    {
      strcat(tr[i],"00010");
    }
    else if(strcmp(insset[i].op5,"3")==0)
    {
      strcat(tr[i],"00011");
    }
    else if(strcmp(insset[i].op5,"4")==0)
    {
      strcat(tr[i],"00100");
    }
    else if(strcmp(insset[i].op5,"5")==0)
    {
      strcat(tr[i],"00101");
    }
    else if(strcmp(insset[i].op5,"6")==0)
    {
      strcat(tr[i],"00110");
    }
    else if(strcmp(insset[i].op5,"7")==0)
    {
      strcat(tr[i],"00111");
    }
    else if(strcmp(insset[i].op5,"8")==0)
    {
      strcat(tr[i],"01000");
    }
    else if(strcmp(insset[i].op5,"9")==0)
    {
      strcat(tr[i],"01001");
    }
    else if(strcmp(insset[i].op5,"10")==0)
    {
      strcat(tr[i],"01010");
    }
    else if(strcmp(insset[i].op5,"11")==0)
    {
      strcat(tr[i],"01011");
    }
    else if(strcmp(insset[i].op5,"12")==0)
    {
      strcat(tr[i],"01100");
    }
    else if(strcmp(insset[i].op5,"13")==0)
    {
      strcat(tr[i],"01101");
    }
    else if(strcmp(insset[i].op5,"14")==0)
    {
      strcat(tr[i],"01110");
    }
    else if(strcmp(insset[i].op5,"15")==0)
    {
      strcat(tr[i],"01111");
    }
    else if(strcmp(insset[i].op5,"16")==0)
    {
      strcat(tr[i],"10000");
    }
    else if(strcmp(insset[i].op5,"17")==0)
    {
      strcat(tr[i],"10001");
    }
    else if(strcmp(insset[i].op5,"18")==0)
    {
      strcat(tr[i],"10010");
    }
    else if(strcmp(insset[i].op5,"19")==0)
    {
      strcat(tr[i],"10011");
    }
    else if(strcmp(insset[i].op5,"20")==0)
    {
      strcat(tr[i],"10100");
    }
    else if(strcmp(insset[i].op5,"21")==0)
    {
      strcat(tr[i],"10101");
    }
    else if(strcmp(insset[i].op5,"22")==0)
    {
      strcat(tr[i],"10110");
    }
    else if(strcmp(insset[i].op5,"23")==0)
    {
      strcat(tr[i],"10111");
    }
    else if(strcmp(insset[i].op5,"24")==0)
    {
      strcat(tr[i],"11000");
    }
    else if(strcmp(insset[i].op5,"25")==0)
    {
      strcat(tr[i],"11001");
    }
    else if(strcmp(insset[i].op5,"26")==0)
    {
      strcat(tr[i],"11010");
    }
    else if(strcmp(insset[i].op5,"27")==0)
    {
      strcat(tr[i],"11011");
    }
    else if(strcmp(insset[i].op5,"28")==0)
    {
      strcat(tr[i],"11100");
    }
    else if(strcmp(insset[i].op5,"29")==0)
    {
      strcat(tr[i],"11101");
    }
    else if(strcmp(insset[i].op5,"30")==0)
    {
      strcat(tr[i],"11110");
    }
    else if(strcmp(insset[i].op5,"31")==0)
    {
      strcat(tr[i],"11111");
    }
  }
//  printf("\nmapreg on %d %s",j,tr[i]);
}

// should do for -ve values
void decToBinary(int n,int bit,char tr[][33],int i)
{
    // array to store binary number
    char binaryNum[32];

    // counter for binary array
    int j = 0,l;
    if(n==0)
    {
      for(j=0;j<bit;j++)
        strcat(tr[i],"0");
    }
    else if(n>0)
    {


    while (n != 0) {

        // storing remainder in binary array
        binaryNum[j] = (n % 2) + 48;
        n = n / 2;
        j++;
    }
    while(j<bit)
    {
      binaryNum[j]= '0';
      j++;
    }
    binaryNum[j]='\0';
  }
  else
  {
    int x=(-1)*n;
    while (x != 0) {

        // storing remainder in binary array
        binaryNum[j] = (x % 2) + 48;
        x = x / 2;
        j++;
    }
    while(j<bit)
    {
      binaryNum[j]= '0';
      j++;
    }
    binaryNum[j]='\0';

  }
    // printing binary array in reverse order
    for (int k = 0; k < j/2; k++)
        {
          char t=binaryNum[k];
          binaryNum[k]=binaryNum[j-1-k];
          binaryNum[j-1-k]=t;

        }
    if(n<0)
    {
      for(l=0;l<bit;l++)
      {
        if(binaryNum[l]=='0')
          binaryNum[l]='1';
        else
          binaryNum[l]='0';
      }
      if(binaryNum[bit-1]=='0')
        binaryNum[l]='1';
      else
      {

      for(l=bit-1;l>=0;l--)
      {
        if(binaryNum[l]=='1')
          binaryNum[l]='0';
        else
          break;
      }
      binaryNum[l]='1';
    }
    }
    strcat(tr[i],binaryNum);
    //printf("\n%s",tr[i]);


}



int convert(char n[])
{
    int dec = 0, i = 0,j=0, rem;
    int k=strlen(n);
    while (n[j] != '\0') {
        //rem = n % 10;
        //n /= 10;
        dec +=  pow(2, k-1-j) * (n[j]-48);
        ++i;
        ++j;
    }
    return dec;
}


void main()
{
  FILE * vfp = fopen("vars.txt","w");
  FILE * fp = fopen("program.asm","r");

  int n=0;
  char a[1000];

  while(fgets(a,1000,fp))
    n++;

  fseek(fp,0,SEEK_SET);
  fclose(fp);

  char s[n][1000],t[n][1000];

  asmRead(s);


  char dat[n][1000];
  int jk=-1;



  char *token;

  int f=-1;
  int i,j;
  int k=0,h;
  char tr[n][33];
  int ij=-1;
  char temptok[100];


  remove_comments(s,&n);

  printf("\nAfter removing comments : \n");
  for(i=0;i<n;i++)
  {
    printf("%s",s[i]);
  }

  printf("\n");


  for(i=0;i<n;i++)
  {
    strcpy(temptok,s[i]);
    token=strtok(temptok,":\n");
    if(strcmp(token,".data")==0)
      {
        f=0;
        continue;
      }
    else if(strcmp(token,".text")==0)
      {
        f=1;
        continue;
      }
    else
    {
      if(f==0)
      {
        jk++;
        strcpy(dat[jk],s[i]);
        fprintf(vfp,"%s",s[i]);
      }
      else if(f==1)
      {
        checkInstructionType(s[i],i);
        if(strcmp(inst[i].type,"label")==0)
        {
          continue;
        }
        else
        {
          ij++;
          strcpy(t[ij],s[i]);
        }
      }
    }
  }
  int nofl=0;
  f=-1;
  for(i=0;i<n;i++)
  {
    strcpy(temptok,s[i]);
    token=strtok(temptok,":\n");
    if(strcmp(token,".data")==0)
      {
        f=0;
        nofl++;
        strcpy(insset[i].type,"AD");
        strcpy(insset[i].mne,".data");
      }
    else if(strcmp(token,".text")==0)
      {
        f=1;
        nofl++;
        strcpy(insset[i].type,"AD");
        strcpy(insset[i].mne,".text");
      }
    else
    {
      if(f==0)
      {
        nofl++;
        strcpy(vt[vars].name,token);
        strcpy(insset[i].type,"label");
        vt[vars].location = vars;
        vars++;
      }
      else if(f==1)
      {
        checkInstructionType(s[i],i);
        if(strcmp(insset[i].type,"label")==0)
        {
          strcpy(st[labels].name,token);
          st[labels].location=i;
          labels++;
        }
        else if(strcmp(insset[i].mne,"ld")==0)
        {
          printf("\n.%s.",tr[i]);
          strcat(tr[i],"111010");
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,14,tr,i);
          strcat(tr[i],"0");
          strcat(tr[i],"0");
          tr[i][32]='\0';
        }
        else if(strcmp(insset[i].mne,"add")==0)
        {
          strcat(tr[i],"011111");       //opcode 31   XO
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"0");
          strcat(tr[i],"100001010");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"addi")==0)
        {
          strcat(tr[i],"001110");   //opcode 14    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          h=atoi(insset[i].op3);
          //printf("\nh=%d\n",h);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"addis")==0)
        {
          strcat(tr[i],"001111");   //opcode 15   D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          h=atoi(insset[i].op3);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"and")==0)
        {
          strcat(tr[i],"011111");     //opcode 31  X
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          //printf("\n.%s.",insset[i].op3);
          strcat(tr[i],"0000011100");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"andi")==0)
        {
          strcat(tr[i],"011100");   //opcode 28    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          h=atoi(insset[i].op3);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"extsw")==0)
        {
          strcpy(tr[i],"011111");
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          strcat(tr[i],"00000");
          strcat(tr[i],"1111011010");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"nand")==0)
        {
          strcat(tr[i],"011111");     //opcode 31  X
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"0111011100");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"or")==0)
        {
          strcat(tr[i],"011111");     //opcode 31  X
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"0110111100");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"ori")==0)
        {
          strcat(tr[i],"011000");   //opcode 24    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          h=atoi(insset[i].op3);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"subf")==0)
        {
          strcat(tr[i],"011111");       //opcode 31   XO
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"0");
          strcat(tr[i],"000101000");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"xor")==0)
        {
          strcat(tr[i],"011111");     //opcode 31  X
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"0100111100");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"xori")==0)
        {
          strcat(tr[i],"011010");   //opcode 26    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          h=atoi(insset[i].op3);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"lwz")==0)
        {
          strcat(tr[i],"100000");   //opcode 32    D rt,ra,si
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"std")==0)
        {
          strcat(tr[i],"111110");      //opcode 62       DS
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,14,tr,i);
          strcat(tr[i],"00");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"stw")==0)
        {
          strcat(tr[i],"100100");   //opcode 36    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"stwu")==0)
        {
          strcat(tr[i],"100101");   //opcode 37   D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"lhz")==0)
        {
          strcat(tr[i],"101000");   //opcode 40    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"lha")==0)
        {
          strcat(tr[i],"101010");   //opcode 42    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"sth")==0)
        {
          strcat(tr[i],"101100");   //opcode 44    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"lbz")==0)
        {
          strcat(tr[i],"100010");   //opcode 34    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"stb")==0)
        {
          strcat(tr[i],"100110");   //opcode 38    D
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          h=atoi(insset[i].op2);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"rlwinm")==0)
        {
          strcat(tr[i],"010101");
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          h=atoi(insset[i].op3);
          decToBinary(h,5,tr,i);
          h=atoi(insset[i].op4);
          decToBinary(h,5,tr,i);
          h=atoi(insset[i].op5);
          decToBinary(h,5,tr,i);
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"sld")==0)
        {
          strcat(tr[i],"011111");     //opcode 31  X
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"0000011011");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"srd")==0)
        {
          strcat(tr[i],"011111");     //opcode 31  X
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"1000011011");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"srad")==0)
        {
          strcat(tr[i],"011111");     //opcode 31  X
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,3);
          strcat(tr[i],"1100011010");
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"sradi")==0)
        {
          //ra,rs,sh {
          strcat(tr[i],"011111");
          mapreg(insset,tr,i,2);
          mapreg(insset,tr,i,1);
          h=atoi(insset[i].op3);
          int u=h%(int)(pow(2,5));
          int v=h/(int)(pow(2,5));
          decToBinary(u,5,tr,i);
          strcat(tr[i],"110011101");
          decToBinary(v,1,tr,i);
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"b")==0)
        {
          char aa[]="0";
          strcat(tr[i],"010010");
          //check for label or immediate and encode bit-6 to 29
          for(j=0;j<labels;j++)
          {
            if(strcmp(st[j].name,insset[i].op1)==0)
              break;
          }
          if(j==labels)
          {
            h=atoi(insset[i].op1);
            decToBinary(h,24,tr,i);
          }
          else
          {
            decToBinary(st[j].location-j-nofl,24,tr,i);
          }
          strcat(tr[i],aa);
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"ba")==0)
        {
          char aa[]="1";
          strcat(tr[i],"010010");
          for(j=0;j<labels;j++)
          {
            if(strcmp(st[j].name,insset[i].op1)==0)
              break;
          }
          if(j==labels)
          {
            h=atoi(insset[i].op1);
            decToBinary(h,24,tr,i);
          }
          else
          {
            decToBinary(st[j].location-j-nofl,24,tr,i);
          }
          strcat(tr[i],aa);
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"bl")==0)
        {
          //char aa[]="1";
          strcat(tr[i],"010010");
          for(j=0;j<labels;j++)
          {
            if(strcmp(st[j].name,insset[i].op1)==0)
              break;
          }
          if(j==labels)
          {
            h=atoi(insset[i].op1);
            decToBinary(h,24,tr,i);
          }
          else
          {
            decToBinary(st[j].location-j-nofl,24,tr,i);
          }
          strcat(tr[i],"01");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"bclr")==0)
        {
          strcpy(tr[i],"010011");
          strcat(tr[i],"10100");  //BO
          mapreg(insset,tr,i,2);   //BI
          strcat(tr[i],"00");   //BH
          strcat(tr[i],"0000010000");
          strcat(tr[i],"0");
          tr[i][32]='\0';
              //LK
        }
        else if(strcmp(insset[i].mne,"bc")==0)
        {
          strcat(tr[i],"010011");
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          for(j=0;j<labels;j++)
          {
            if(strcmp(st[j].name,insset[i].op3)==0)
              break;
          }
          if(j==labels)
          {
            h=atoi(insset[i].op3);
            decToBinary(h,14,tr,i);
          }
          else
          {
            decToBinary(st[j].location-j-nofl,14,tr,i);
          }
          strcat(tr[i],"00");   // AA and LK
          tr[i][32]='\0';
        }
        else if(strcmp(insset[i].mne,"bca")==0)
        {
          strcat(tr[i],"010011");
          mapreg(insset,tr,i,1);
          mapreg(insset,tr,i,2);
          for(j=0;j<labels;j++)
          {
            if(strcmp(st[j].name,insset[i].op3)==0)
              break;
          }
          if(j==labels)
          {
            h=atoi(insset[i].op3);
            decToBinary(h,14,tr,i);
          }
          else
          {
            decToBinary(st[j].location-j-nofl,14,tr,i);
          }
          strcat(tr[i],"10");   // AA and LK
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"cmp")==0)     //incomplete
        {
          strcpy(tr[i],"011111");
          h=atoi(insset[i].op1);
          decToBinary(h,3,tr,i);
          //  '/'
          strcat(tr[i],"0");
          strcat(tr[i],"1");
          mapreg(insset,tr,i,3);
          mapreg(insset,tr,i,4);
          strcat(tr[i],"0000000000");
          //  '/'
          strcat(tr[i],"0");
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"cmpi")==0)
        {
          strcat(tr[i],"001011");
          h=atoi(insset[i].op1);
          decToBinary(h,3,tr,i);
          //   '/'
          strcat(tr[i],"0");
          strcat(tr[i],"1");
          mapreg(insset,tr,i,2);
          h=atoi(insset[i].op3);
          decToBinary(h,16,tr,i);
          tr[i][32]='\0';

        }
        else if(strcmp(insset[i].mne,"sc")==0)
        {
          strcpy(tr[i],"010001");
          strcat(tr[i],"00000000000000");
          //strcat(tr[i],insset[i].op1);       //LEV
          h=atoi(insset[i].op1);
          decToBinary(h,7,tr,i);
          strcat(tr[i],"00010");
          tr[i][32]='\0';

        }

      }
    }
  }

  fclose(vfp);
  //write into output file
  fp=fopen("program.o","wb");
  for(i=0;i<n;i++)
  {

    if(strcmp(insset[i].type,"AD")&&strcmp(insset[i].type,"label"))
    {
      unsigned int def=convert(tr[i]);
      fwrite(&def,sizeof(int),1,fp);
      printf("%u \n",def);
      printf("%s \n",tr[i]);
    }
  }
  fclose(fp);

}
