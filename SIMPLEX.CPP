#include<conio.h>
#include<stdio.h>
#include<iostream.h>


#define MAX 50
#define INFINITY 1e20

int fulltable=0;

void hline()
{
for(int i=0;i<80;i++)
printf("Ä");
return;
}

void displaytable(int,int,char [][9],char [],int [],float [][MAX],int,char []);

void main()
{
 float table[MAX][MAX],table2[MAX][MAX],d[MAX],pivotelem,temp;
 char var[MAX][9],obj[9],yesno,filename[80];
 int i,j,n,con,basic[MAX],amongbasic[MAX],entcol,pivoteq,notoptimum,count;

 float table_original[MAX][MAX];
 int basic_original[MAX];

 clrscr();
 cout<<"\t\t\tSimplex Tableau Generator\n";
 hline();
 cout<<"\nGenerates Simplex tables for problems of the kind:\n\n"
       <<"MAXIMIZATION problems given in standard form\n\n"
       <<"-Input is basically the initial Simplex tableau\n"
       <<"-Warns and stops in case of unbounded solution\n"
       <<"-Stops at first optima in case of infinite solutions and asks whether to iterate for next or not\n"
       <<"-Cannot deal with cases like those requiring Big-M method by itself\n\n"
       <<"NOTE : The problems of Big-M can of course be solved if a suitable large \n"
       <<"       positive value of M is assumed and the initial Simplex tableau\n"
       <<"       is entered in a complete numerical form.\n\n"
       <<"Press any key to continue...";
 getch();
 clrscr();
 cout<<"Enter the filename with \".rtf\" extension in which you want the output be saved : ";
 cin>>filename;

 cout<<"Enter no. of Variables (including slack variables) : ";
 cin>>n;
 cout<<"Enter no. of constraints			   : ";
 cin>>con;

 obj[0]='Z';obj[1]='\0';

 for(i=0;i<MAX;i++)
 {
  var[i][0]='x';
  var[i][1]=(i+1)+'0';
   var[i][2]='\0';
 }


 j=0;
 int k=0;
 for(i=0;i<n;i++)
  {
   if(i>=(n-con))
    {
     basic[j++]=i;
     basic_original[k++]=i;
    }
  }

 cout<<"Do you want to name the variables (or go by default names)?(y/n):";
 input_again:
 yesno=getche();

 switch(yesno)
 {
 case 'n':
 case 'N':
  break;
 case 'y':
 case 'Y':
 cout<<"\nEnter name of objective : ";
 cin>>obj;
 cout<<"Enter variable names:\n";

 j=0;
 k=0;
 for(i=0;i<n;i++)
  {
   cout<<"Enter name of var. "<<i+1<<" : ";
   cin>>var[i];
   if(i>=(n-con))
    {
     basic[j++]=i;
     basic_original[k++]=i;
    }
  }
 break;
 default:
  cout<<"\nError! Press y or n :";
  goto input_again;
 }

 cout<<"\nEnter coefficients of standard objective function :\n";
 for(i=0;i<n;i++)
  {
   cout<<var[i]<<" : ";
   cin>>table[0][i];
   table_original[0][i]=table[0][i];
  }
 cout<<"Solution : ";
 cin>>table[0][n];
 table_original[0][n]=table[0][n];

 cout<<"Enter coefficients of standard constraints :\n";
 for(i=1;i<=con;i++)
  {
   cout<<"Constraint "<<i<<" :\n";
   for(j=0;j<n;j++)
    {
     cout<<var[j]<<" : ";
     cin>>table[i][j];
     table_original[i][j]=table[i][j];
    }
    cout<<"Solution : ";
    cin>>table[i][n];
    table_original[i][n]=table[i][n];
  }

  cout<<"\nPress             1 : To see full tables\n"
	<<"      any other key : To see condensed tables";
  yesno='a';
  yesno=getche();
  if(yesno=='1') fulltable=1;

 ////////// input finishes, processing starts //////////////////
 processing:

 count=0;
 notoptimum=1;

  displaytable(n,con,var,obj,basic,table,count,filename);

  getch();
do
 {
 count++;
 for(i=0;i<MAX;i++) amongbasic[i]=0;
 for(i=0;i<con;i++)
  {
   amongbasic[basic[i]]=1;
  }

  temp=0;
  for(i=0;i<n;i++)
   {
     if(table[0][i]<0 && table[0][i]<temp)
      {
       temp=table[0][i];
       entcol=i;
      }
   }
   if(temp==0)
    {
     int alta=0;
     char yesno2='n';
     notoptimum=0;
     cout<<"\n\nOptimum Found!";
     for(i=0;i<n;i++)
      {
       if( (!amongbasic[i]) && table[0][i]==0 )
	{
	 alta=1;
	 cout<<"\nAlternate solution possible by choosing "<<var[i]
	 <<" (code: "<<i<<" ) as entering variable.";
	}
      }
    if(alta)
    {
     cout<<"\nDo you want to iterate further?(y/n):";
     input_again2:
     yesno2=getche();

     switch(yesno2)
      {
       case 'n':
       case 'N':
	break;
       case 'y':
       case 'Y':
	cout<<"\nEnter code of the possible entering variables you want to enter (as basic) : ";
	cin>>entcol;
	break;
       default:
	cout<<"\nError! Press y or n :";
	goto input_again2;
       }
     if(yesno2=='y'||yesno2=='Y');
     else if(yesno2=='n'||yesno2=='N')
     goto endofdowhile;
    }
     if(!alta) goto endofdowhile;
    }
   cout<<"\n\nEntering variable is : "<<var[entcol];

  for(i=1;i<=con;i++)
   {
    if(table[i][entcol]!=0)
     d[i-1]=table[i][n]/table[i][entcol];
    else
      d[i-1]=INFINITY;
   }

  temp=999999999;  //9 times 9
  for(i=0;i<con;i++)
   {
     if(d[i]>0 && d[i]<temp)
      {
       temp=d[i];
       pivoteq=i;
      }
   }

   if(temp==999999999)
    {
     notoptimum=0;
     cout<<"\n\nNo Leaving variable found!\nNon bounding solution";
     goto endofdowhile;
    }

   cout<<"\n\nLeaving variable is : "<<var[basic[pivoteq]]<<endl;

   basic[pivoteq]=entcol;

  pivotelem=table[pivoteq+1][entcol];

  for(i=0;i<=n;i++)
   {
    table2[0][i]=table[0][i]-table[0][entcol]*table[pivoteq+1][i]/pivotelem;
   }

  for(i=1;i<=con;i++)
   {
    if(i==pivoteq+1)
     {
      for(j=0;j<=n;j++)
      {
       table2[i][j]=table[i][j]/pivotelem;
      }
     }
    else
     {
      for(j=0;j<=n;j++)
       {
	table2[i][j]=table[i][j]-table[i][entcol]*table[pivoteq+1][j]/pivotelem;
       }
     }
   }

  for(i=0;i<=con;i++)
   {
    for(j=0;j<=n;j++)
     {
      table[i][j]=table2[i][j];
     }
   }

  displaytable(n,con,var,obj,basic,table,count,filename);
  getch();
  endofdowhile:
  ;
 }while(notoptimum);

 getch();
 cout<<"\nPress r or 1: To repeat calculation with same set of equations\n"
       <<"      m or 2: To repeat calculation with modified set of equations\n"
       <<"      e or 3: To exit :";

 input_again3:
  yesno=getche();

 switch(yesno)
 {
  case 'r':
  case 'R':
  case '1':
    for(i=0;i<=n;i++)
     {
      for(j=0;j<=n;j++)
       table[i][j]=table_original[i][j];
     }

    for(i=0;i<=n;i++)
     basic[i]=basic_original[i];

    goto processing;

  case 'm':
  case 'M':
  case '2':
   for(i=0;i<=n;i++)
     {
      for(j=0;j<=n;j++)
       table[i][j]=table_original[i][j];
     }

    for(i=0;i<=n;i++)
     basic[i]=basic_original[i];

   modifymore:
   int m;
   cout<<"\n\nEnter the equation no. you want to modify\nEnter constraint no. or enter 0 (zero) for objective function :";
   cin>>m;
   if(m==0)
    {
     cout<<"\nEnter coefficients of standard objective function :\n";
     for(i=0;i<n;i++)
      {
       cout<<var[i]<<" : ";
       cin>>table[0][i];
       table_original[0][i]=table[0][i];
      }

     cout<<"Solution : ";
     cin>>table[0][n];
     table_original[0][n]=table[0][n];
    }
   else
    {
     cout<<"Constraint "<<m<<" :\n";
     for(j=0;j<n;j++)
      {
       cout<<var[j]<<" : ";
       cin>>table[m][j];
       table_original[m][j]=table[m][j];
      }
     cout<<"Solution : ";
     cin>>table[m][n];
     table_original[m][n]=table[m][n];
    }

   cout<<"\nModify more? Press y : yes        any other key : n :";
   yesno='a';
   yesno=getche();
   if(yesno=='y'||yesno=='Y') goto modifymore;
   else goto processing;
  case 'e':
  case 'E':
  case '3':
   goto end;
  default:
   cout<<"\nERROR! Enter again : ";
   goto input_again3;
  }
 end:
 ;

}//Main ends
/************************************************************************/
void displaytable(int n,int con,char var[][9],char obj[],int basic[],
		  float table[][MAX],int count, char filename[])
{
 int i,j,amongbasic[MAX];
 FILE *fp;
 fp=fopen(filename,"a");

 clrscr(); ///////output

 cout<<"Iteration : "<<count<<endl;
 fprintf(fp,"\n\nIteration : %d\n\n",count);

 hline();

 for(i=0;i<MAX;i++) amongbasic[i]=0;

 for(i=0;i<con;i++)
  {
   amongbasic[basic[i]]=1;
  }


 cout<<"     ";
 fprintf(fp,"     ");

 for(i=0,j=0;i<n;i++)
  {
    if(fulltable)
     {
      cout<<var[i]<<"        ";
      fprintf(fp,   "%s        ",var[i]);
     }
    else
     {
      if(amongbasic[i]);
      else
      cout<<var[i]<<"        ";
      fprintf(fp,   "%s        ",var[i]);
     }
  }
  cout<<"Sol";
  cout<<endl;
  fprintf(fp,"Sol\n");

  cout<<obj<<"    ";
  fprintf(fp,"%s    ",obj);

 for(i=0;i<=n;i++)
  {
   if(fulltable)
    {
     printf("%-10.3g",table[0][i]);
     fprintf(fp,"%-10.3g",table[0][i]);
    }
   else
    {
     if(amongbasic[i]);
     else
     printf("%-10.3g",table[0][i]);
     fprintf(fp,"%-10.3g",table[0][i]);
    }
  }
  cout<<endl;
  fprintf(fp,"\n");

 for(i=1;i<=con;i++)
  {
   cout<<var[basic[i-1]]<<"  ";
   fprintf(fp,"%s  ",var[basic[i-1]]);

   for(j=0;j<=n;j++)
    {
     if(fulltable)
      {
       printf("%-10.3g",table[i][j]);
       fprintf(fp,"%-10.3g",table[i][j]);
      }
     else
      {
       if(amongbasic[j]);
       else
       printf("%-10.3g",table[i][j]);
       fprintf(fp,"%-10.3g",table[i][j]);
      }
    }
   cout<<endl;
   fprintf(fp,"\n");
  }         //output finishes

 fclose(fp);



 return;
}
