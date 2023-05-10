#include <cstdio>
#include <stdio.h>
#include <string.h>

#define NSIZE 8

void print_2(int val2);

void Text2Bin(const char* sIn,const char* sOut)
{

  char a[255];
  int count = 0;
  int ch;

  for(int j=0; j<255;j++)a[j]='\0';

  FILE* fin=fopen(sIn,"r");

  FILE* fout=fopen(sOut,"w");

  for(int i=0 ; i<=255 ; i++)fscanf(fin,"%c",&a[i]);

  for(int k=0 ; k<=254&&a[k] !='\0'; k++)

  { 

    ch = a[k];

    for(int a=7;a>=0;a--) fprintf(fout,"%d",ch>>a&1);

  }

  fclose(fin);

  fclose(fout);

}


int main()
{

  Text2Bin("test:\\a1.txt","test:\\a2.txt");

  printf("\nSuccessfully converted file!\n");

  return 0;

}
