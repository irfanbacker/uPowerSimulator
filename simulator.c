#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------

long int r[32],srr0,lr,cr;

//--------------------------------------------------------------------------------------------------------

struct datamem
{
  char name[100];
  int type;
  int size;
  int data1[1000];
  char data2[1000];
  long int start_add;
};

struct datamem dmem[100];
int ndmem=0;
//--------------------------------------------------------------------------------------------------------

int codeRead(FILE *f,unsigned int *s)
{
  if(f==NULL)
  {
    printf("\nFile does not exist");
    exit(1);
  }
  int i=0;
  while(fread(s+(sizeof(int)*i),sizeof(int),1,f)){
    i++;
  };
  return i;
}

//Function to read variable table, label table and initialize the memory
void initMem()
{
  FILE * f=open("vars.txt","r");
  int i=0;
  char str[1000];
  while(fread(s,1000,f))
  {
    char *tok = strtok(s," :");
    strcpy(dmem[i].name,tok);
    tok = strtok(NULL," \"");
    if(strcmp(tok,".word")==0)
    {
      tok=strtok(NULL,"\n\"");
      dmem[i].type=1;
      if(i==0)
      {
        dmem[i].start_add=0x10000000;
      }
      else
      {
        dmem[i].start_add=dmem[i-1].start_add+size;
      }
      char * ele = strtok(tok," \"\n");
      int j=0;
      while(ele)
      {
        dmem[i].data1[j]=atoi(ele);
        ele=strtok(NULL," \n");
        dmem[i].size+=4;
      }
    }
    else if(strcmp(tok,".asciiz")==0)
    {
      dmem[i].type=2;
      if(i==0)
      {
        dmem[i].start_add=0x10000000;
      }
      else
      {
        dmem[i].start_add=dmem[i-1].start_add+size;
      }
      tok=strtok(NULL,"\n\"");
      strcpy(dmem[i].data2,tok);
      dmem[i].size=4*(ceil(float(strlen(tok)/4.0)));
    }
    i++;
  }
  ndmem=i;
}

int extractBits(int num, int k, int pos)
{
    int p=32-pos-k;
    return (((1 << k) - 1) & (num >> (p)));
}

int signExt_16(int num) {
    long int value = (0x000000000000FFFF & num);
    long int mask = 0x0000000000008000;
    if (mask & num) {
        value += 0xFFFFFFFFFFFF0000;
    }
    return value;
}

int memory(int rs,int a,int b)
{
  for(int i=0;i<=ndmem;i++)
  {
    if(dmem[i].start_ad<=a && dmem[i].start_ad+dmem[i].size>=a+b)
    {
      r[rs] = dmem.data1[a+b-dmem[i].start_ad];
      break;
    }
  }
}

int displaymem()
{
  printf("\n----------------------------------------------------------------------");
  printf("\n r0 = %ld r1 = %ld r2 = %ld r3 = %ld r4 = %ld r5 = %ld r6 = %ld r7 = %ld",r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7]);
  printf("\n r8 = %ld r9 = %ld r10= %ld r11= %ld r12= %ld r13= %ld r14= %ld r15= %ld",r[8],r[9],r[10],r[11],r[12],r[13],r[14],r[15]);
  printf("\n r16= %ld r17= %ld r18= %ld r19= %ld r20= %ld r21= %ld r22= %ld r23= %ld",r[16],r[17],r[18],r[19],r[20],r[21],r[22],r[23]);
  printf("\n r24= %ld r25= %ld r26= %ld r27= %ld r28= %ld r29= %ld r30= %ld r31= %ld",r[24],r[25],r[26],r[27],r[28],r[29],r[30],r[31]);
  printf("\n srr0 = %ld lr = %ld cr = %ld",srr0,lr,cr);
  printf("\n----------------------------------------------------------------------\n");
}

//---------------------------------------INSTRUCTION FUNCTIONS--------------------------------------------

void add(int rs,int ra,int si)
{
  long int im=signExt_16(si);
  r[rs]=r[ra]+im;
}

void addi(int rs,int ra,int si)
{
  long int im=signExt_16(si);
  r[rs]=r[ra]+im;
}

void addis(int rs,int ra,int si)
{
  long int im=signExt_16(si);
  im=im<<16;
  r[rs]=r[ra]+im;
}

void andi(int rs,int ra,int si)
{
  long int im = (0x000000000000FFFF & si);
  r[rs] = r[ra] & im;
}

void extsw()
{

}

void nand(int rs,int ra,int rb)
{
    r[ra] = ~(r[rs] & r[rb]);
}

void or(int rs,int ra,aint rb)
{
    r[ra]=r[rs] | r[rb];
}

void ori(int rs,int ra,int si)
{
  if(!((!rs)&&(!ra)&&(!si)))
  {
    long int im = (0x000000000000FFFF & si);
    r[rs] = r[ra] | im;
  }
}

void xori(int rs,int ra,int si)
{
  if(!((!rs)&&(!ra)&&(!si)))
  {
    long int im = (0x000000000000FFFF & si);
    r[rs] = r[ra] ^ im;
  }
}

void ld(int rs,int ra,int ds)
{
  long int dsi = (0x000000000000FFFF & ds);
  dsi = dsi<<2;
  long int ea = signExt_16(dsi) + r[ra];
  memory(rs,ea,8);  // <--------------------------- MEMORY NOT STARTED
}

void lwz(int rs,int ra,int si)
{
  int b;
  if(ra==0)
    b=0;
  else
    b=r[ra]
  long int im = (0x000000000000FFFF & si);
  im = im<<2;
  long int ea = b + signExt_16(im);
  memory(rs,ea,4);
}

void std(int rs,int ra,int si)
{

}

void stw(int rs,int ra,int si)
{

}

void stwu(int rs,int ra,int si)
{

}

void lhz(int rs,int ra,int si)
{

}

void lha(int rs,int ra,int si)
{

}

void sth(int rs,int ra,int si)
{

}

void lbz(int rs,int ra,int si)
{

}

void stb(int rs,int ra,int si)
{

}

void rlwinm(int rs,int ra,int sh,int mb, int me)
{

}

void ba(int li)
{

}

void b(int li)
{

}

void bi(int li)
{

}

void bc()
{

}

void bca()
{

}

void cmpi(int li,int b, int c)
{

}

//--------------------------------------------------------------------------------------------------------

void main()
{
  int flag=0,mode=-1,c=0;
  char fname[100],ch;
  r[0]=0;
  //while(flag==0)
  //{
    printf("\n Enter filename: ");
    gets(fname);

    FILE *source=fopen(fname,"r");
    if( access(fname, F_OK ) == -1 ) {
      printf("\n File %s doesnt exist!",fname);
    }
    //else flag=1;
  //};
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

  FILE *f=fopen("program.o","rb");

  initMem();

  int n=0,i=0;
  unsigned int s[1000],opcode,xo,ra,rb,rs;
  unsigned int ds,sh,me,mb,li,aa,lk,bd;
  int si;

  fseek(f,0,SEEK_SET);

  n=codeRead(f,s);

  flag=0;
  while(flag==0)
  {
    if(c==i)
    {
      printf("\n Select mode 1.) Step 2.) Full : ");
      scanf("%d",&mode);
      if(mode==1) c++;
      else if(mode==2) c=n;
    }

    if(c==n){
      flag=1;
      break;
    }

    for(;i<c;i++)
    {
      opcode=extractBits(s[i],6,0);

      if(opcode==31) //OPCODE=31 <------------------- NOT COMPLETED / CONFUSION
      {
        xo=extractBits(s[i],9,22);
        if((xo==266)&&(!extractBits(s[i],1,21))&&(!extractBits(s[i],1,31)))// ADD
        {
          rs=extractBits(s[i],5,6);
          ra=extractBits(s[i],5,11);
          rb=extractBits(s[i],5,16);
          add(rs,ra,rb);
          printf("\n add %d,%d,%d\n",rs,ra,rb);
        }
        else if(xo==28)
        {

        }
      }
      else if(opcode==14)// ADDI
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        addi(rs,ra,si);
        printf("\n addi %d,%d,%d\n",rs,ra,si);
      }
      else if(opcode==15)// ADDIS
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        addis(rs,ra,si);
      }
      else if(opcode==28)// ANDI
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        andi(rs,ra,si);
        printf("\n add %d,%d,%d\n",ra,rs,si);
      }
      else if(opcode==24)// ORI
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        ori(rs,ra,si);
      }
      else if(opcode==26)// XORI
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        xori(rs,ra,si);
      }
      else if(opcode==58)
      {
        xo=extractBits(s[i],2,30);
        if(xo==0)// LD
        {
          rs=extractBits(s[i],5,6);
          ra=extractBits(s[i],5,11);
          ds=extractBits(s[i],14,16);
          ld(rs,ra,ds);
        }
      }
      else if(opcode==32)// LWZ
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        lwz(rs,ra,si);
      }
      else if(opcode==62)
      {
        xo=extractBits(s[i],2,30);
        if(xo==0)// STD
        {
          rs=extractBits(s[i],5,6);
          ra=extractBits(s[i],5,11);
          ds=extractBits(s[i],14,16);
          std(rs,ra,ds);
        }
      }
      else if(opcode==36)// STW
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        stw(rs,ra,si);
      }
      else if(opcode==37)// STWU
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        stwu(rs,ra,si);
      }
      else if(opcode==40)// LHZ
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        lhz(rs,ra,si);
      }
      else if(opcode==42)// LHA
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        lha(rs,ra,si);
      }
      else if(opcode==44)// STH
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        stw(rs,ra,si);
      }
      else if(opcode==34)// LBZ
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        lbz(rs,ra,si);
      }
      else if(opcode==38)// STB
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        stb(rs,ra,si);
      }
      else if((opcode==21)&&(!extractBits(s[i],1,31)))// RLWINM
      {
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        sh=extractBits(s[i],5,16);
        mb=extractBits(s[i],5,21);
        me=extractBits(s[i],5,26);
        rlwinm(rs,ra,sh,mb,me);
      }
      else if(opcode==18)
      {
        lk=extractBits(s[i],1,31);
        aa=extractBits(s[i],1,30);
        li=extractBits(s[i],24,6);
        if(lk==0)
        {
          if(aa) ba(li);// BA
          else b(li);// B
        }
        else bi(li);// BI
      }
      else if((opcode==38)&&(!extractBits(s[i],1,31)))
      {
        aa=extractBits(s[i],1,30);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        bd=extractBits(s[i],14,16);
        if(!aa) bc(); // BC  <------------------- NOT COMPLETED
        else bca();// BCA  <------------------- NOT COMPLETED
      }
      else if(opcode==11)// CMPI
      {
        si=extractBits(s[i],16,16);
        rs=extractBits(s[i],5,6);
        ra=extractBits(s[i],5,11);
        cmpi(rs,ra,si);
      }
      else if(opcode==17)// SC <------------------- NOT COMPLETED
      {

      }

    }
    displaymem();
  };

  fclose(f);
}
