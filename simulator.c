#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------

char r[32][64];

//--------------------------------------------------------------------------------------------------------

void codeRead(FILE *f,char s[][32])
{
  if(f==NULL)
  {
    printf("\nFile does not exist");
    exit(1);
  }
  int i=0;
  while(fgets(s[i],32,f)){
    i++;
  };
  return i;
}

//Function to read variable table, label table and populate the ram
void populateRAM()
{

}

//--------------------------------------------------------------------------------------------------------

void getOpcode(char *op,char s[][32], int i)
{
  for(int j=0;j<6;j++)
  {
    op[j]=s[i][j];
  }
}

void getxo(char *xo,char s[][32], int i)
{
  for(int j=21;j<32;j++)
  {
    xo[j]=s[i][j];
  }
}

//--------------------------------------------------------------------------------------------------------

void add(int ra[5],int rb[5],int rs[5])
{
  
}

//--------------------------------------------------------------------------------------------------------

void main(int argc, char **argv)
{
  int flag=0,mode=-1,c=0;
  char fname[100];
  while(flag==0)
  {
    printf("\n Enter filename: ");
    gets(fname);

    FILE *source=fopen(argv[1],"r");
    if( access(argv[1], F_OK ) == -1 ) {
      printf("\n File %s doesnt exist!",*argv[1]);
    }
    else flag=1;
  };
  FILE *dest=fopen("program.asm","w");

  while( ( ch = fgetc(source) ) != EOF )
      fputc(ch, dest);

  fclose(source);
  fclose(dest);

  pid_t pid = fork();
  if(pid==0)
  {
    char *args[]={"./asm",NULL};
    execvp(args[0],args);
    exit(0);
  }
  else
  {
    waitpid(pid,0,0);
  }

  FILE *f=fopen("program.o","r");

  populateRAM();

  int n=0,i=0;
  char s[1000][32],opcode[6],xo[10],ra[5],rb[5],rs[5];

  fseek(f,0,SEEK_SET);

  n=codeRead(f,s);

  flag=0;
  while(flag==0)
  {
    if(c==i)
    {
      printf("\n Select mode 1.) Step 2.) Full : ");
      scanf("%d",mode);
      if(mode==1) c++;
      else if(mode==2) c=n;
    }

    if(c==n){
      flag=1;
      break;
    }

    for(;i<c;i++)
    {
      getOpcode(opcode,s,i);

      if(strcmp(opcode,"011111")==0) //OPCODE=31
      {
        getxo(xo,s,i);
        if(strcmp(xo,"01000010100")==0)// ADD
        {
          for(int j=6;j<11;j++)
          {
            rs[j]=s[i][j];
          }
          for(int j=11;j<16;j++)
          {
            ra[j]=s[i][j];
          }
          for(int j=16;j<21;j++)
          {
            rb[j]=s[i][j];
          }
          add(ra,rb,rs);
        }
      }
      else if(strcmp(opcode,"")==0)
      {

      }

    }
  };

  fclose(f);
}
