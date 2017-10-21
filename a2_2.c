#include <stdio.h>
#include <string.h>
#include <ctype.h>

int i2,j2,i,j,k,m2=0,n=0,o,p,ns=0,tn=0,rr=0,ch=0;
char read[15][10],gl[15],gr[15][10],temp,templ[15],tempr[15][10],*ptr,temp2[5],dfa[15][15];
char variables[15]={'\0'}, terminals[15]={'\0'};
char slr[15][15][10]={'\0'};
char foll[10];
int len_var, len_ter; 
struct states
{
	char lhs[15],rhs[15][10];
	int n;
}I[15];

void follow(char c);
void first(char c);

int compstruct(struct states s1,struct states s2)
{
	int t;
	if(s1.n!=s2.n)
		return 0;
	if( strcmp(s1.lhs,s2.lhs)!=0 )
		return 0;
	for(t=0;t<s1.n;t++)
		if( strcmp(s1.rhs[t],s2.rhs[t])!=0 )
			return 0;
	return 1;
}


void moreprod()
{
	int r,s,t,l1=0,rr1=0;
	char *ptr1,read1[15][10];

	for(r=0;r<I[ns].n;r++)
	{
		ptr1=strchr(I[ns].rhs[l1],'.');
		t=ptr1-I[ns].rhs[l1];
		if( t+1==strlen(I[ns].rhs[l1]) )
		{
			l1++;
			continue;
		}
		temp=I[ns].rhs[l1][t+1];
		l1++;
		for(s=0;s<rr1;s++)
			if( temp==read1[s][0] )
				break;
		if(s==rr1)
		{
			read1[rr1][0]=temp;
			rr1++;
		}
		else
			continue;

		for(s=0;s<n;s++)
		{
			if(gl[s]==temp)
			{
				I[ns].rhs[I[ns].n][0]='.';
				I[ns].rhs[I[ns].n][1]='\0';
				strcat(I[ns].rhs[I[ns].n],gr[s]);
				I[ns].lhs[I[ns].n]=gl[s];
				I[ns].lhs[I[ns].n+1]='\0';
				I[ns].n++;
			}
		}
	}
}

void canonical(int l)
{
	int t1;
	char read1[15][10],rr1=0,*ptr1;
	for(i=0;i<I[l].n;i++)
	{
		temp2[0]='.';
		ptr1=strchr(I[l].rhs[i],'.');
		t1=ptr1-I[l].rhs[i];
		if( t1+1==strlen(I[l].rhs[i]) )
			continue;

		temp2[1]=I[l].rhs[i][t1+1];
		temp2[2]='\0';

		for(j=0;j<rr1;j++)
			if( strcmp(temp2,read1[j])==0 )
				break;
		if(j==rr1)
		{
			strcpy(read1[rr1],temp2);
			read1[rr1][2]='\0';
			rr1++;
		}
		else
			continue;

		for(j=0;j<I[0].n;j++)
		{
			ptr=strstr(I[l].rhs[j],temp2);
			if( ptr )
			{
				templ[tn]=I[l].lhs[j];
				templ[tn+1]='\0';
				strcpy(tempr[tn],I[l].rhs[j]);
				tn++;
			}
		}

		for(j=0;j<tn;j++)
		{
			ptr=strchr(tempr[j],'.');
			p=ptr-tempr[j];
			tempr[j][p]=tempr[j][p+1];
			tempr[j][p+1]='.';
			I[ns].lhs[I[ns].n]=templ[j];
			I[ns].lhs[I[ns].n+1]='\0';
			strcpy(I[ns].rhs[I[ns].n],tempr[j]);
			I[ns].n++;
		}

		moreprod();
		for(j=0;j<ns;j++)
		{
			//if ( memcmp(&I[ns],&I[j],sizeof(struct states))==1 )
			if( compstruct(I[ns],I[j])==1 )
			{
				I[ns].lhs[0]='\0';
				for(k=0;k<I[ns].n;k++)
					I[ns].rhs[k][0]='\0';
				I[ns].n=0;
				dfa[l][j]=temp2[1];
				break;
			}
		}
		if(j<ns)
		{
			tn=0;
			for(j=0;j<15;j++)
			{
				templ[j]='\0';
				tempr[j][0]='\0';
			}
			continue;
		}

		dfa[l][j]=temp2[1];
		printf("\n\nI%d :",ns);
		for(j=0;j<I[ns].n;j++)
			printf("\n\t%c -> %s",I[ns].lhs[j],I[ns].rhs[j]);

		//getch();
		ns++;
		tn=0;
		for(j=0;j<15;j++)
		{
			templ[j]='\0';
			tempr[j][0]='\0';
		}
	}
}

void extract_var_char(){
	for (i=0;i<n;i++){
		if (gl[i] >= 'A' && gl[i] <= 'Z'){
			if (!strchr(variables,gl[i]))
				sprintf(variables,"%s%c",variables,gl[i]);
		}

		for (j=0;j<strlen(gr[i]);j++){
			if (gr[i][j] < 'A' || gr[i][j] > 'Z'){
				if (!strchr(terminals,gr[i][j]))
					sprintf(terminals,"%s%c",terminals,gr[i][j]);
			}
		}
	}
	strcat(terminals,"$");
	printf ("Variables : %s\nTerminals : %s\n",variables,terminals);
}

void print_dfa(){
	printf("\n\tDFA Table...\n\n");
	for(i=0;i<ns;i++)
	{
		printf("I%d : ",i);
		for(j=0;j<ns;j++)
		
			if(dfa[i][j]!='\0')
				printf("'%c'->I%d | ",dfa[i][j],j);
		
		printf("\n\n");
	}
}

void display_slr(){
	printf("\n\t\tSLR(1) Table...\n\n\t");
	for (i=0;i<len_ter;i++)
		printf("%c\t",terminals[i]);
	for (i=0;i<len_var;i++)
		printf("%c\t",variables[i]);
	printf("\n");
	for (i=0;i<ns;i++){
		printf("I%d\t",i);
		for (j=0;j<len_var+len_ter;j++)
			printf("%s\t",slr[i][j]);
		printf("\n");
	}
}

void main()	
{
	FILE *f;
	int l;
	//clrscr();

	for(i=0;i<15;i++)
	{
		I[i].n=0;
		I[i].lhs[0]='\0';
		I[i].rhs[0][0]='\0';
		dfa[i][0]='\0';
	}

	f=fopen("grammar.txt","r");
	while(!feof(f))
	{
		fscanf(f,"%c",&gl[n]);
		fscanf(f,"%s\n",gr[n]);
		n++;
	}

	printf("\tGiven Grammar...\n");
	for(i=0;i<n;i++)
		printf("\t\t%c -> %s\n",gl[i],gr[i]);

	I[0].lhs[0]='Z';
	strcpy(I[0].rhs[0],".S");
	I[0].n++;
	l=0;
	for(i=0;i<n;i++)
	{
		temp=I[0].rhs[l][1];
		l++;
		for(j=0;j<rr;j++)
			if( temp==read[j][0] )
				break;
		if(j==rr)
		{
			read[rr][0]=temp;
			rr++;
		}
		else
			continue;
		for(j=0;j<n;j++)
		{
			if(gl[j]==temp)
			{
				I[0].rhs[I[0].n][0]='.';
				strcat(I[0].rhs[I[0].n],gr[j]);
				I[0].lhs[I[0].n]=gl[j];
				I[0].n++;
			}
		}
	}
	ns++;

	printf("\n\tCanonicals...\n");
	printf("\nI%d :\n",ns-1);
	for(i=0;i<I[0].n;i++)
		printf("\t%c -> %s\n",I[0].lhs[i],I[0].rhs[i]);

	for(l=0;l<ns;l++){
		canonical(l);
	}

	printf("\n\n");
	print_dfa();

	/////////////////////////////Construction of SLR(1) Table/////////////////
	int t,tempo;
		
	extract_var_char();
	len_var = strlen(variables);
	len_ter = strlen(terminals);
	int columns = len_var + len_ter;

	for (i=0;i<ns;i++){

		for (j=0;j<ns;j++){
			//tempo = j - 48;
			if (dfa[i][j] != '\0'){
				if (strchr(terminals,dfa[i][j])){
					sprintf(slr[i][strchr(terminals,dfa[i][j])-terminals],"s%d",j);
				}
				else if (strchr(variables,dfa[i][j])){
					sprintf(slr[i][len_ter+strchr(variables,dfa[i][j])-variables],"%d",j);
				}
			}	
		}
		//printf("Here..i..%d\n",i);
		for (j=0;j<I[i].n;j++){
			int temp_ind = strlen(I[i].rhs[j])-1; // to make the 1st state that end with "S." accept. 
			
			if (I[i].rhs[j][temp_ind] == '.'){
				//printf("Here...j.%d\n",j);
				if (I[i].rhs[j][temp_ind-1] == 'S' && i==1)
					sprintf(slr[i][strchr(terminals,'$')-terminals],"accept");
				else{
					//printf("--->%d %d %c<---\n",i,j,I[i].lhs[j]);
					//printf("Here..j..%c\n",I[i].lhs[j]);
					
					follow(I[i].lhs[j]);

					int ha,prod_num,tempo_store;
					
					//for (int ip=0;ip<m;ip++)
  					//	printf("@@%c @",foll[ip]);
  					
					for (ha=0;gl[ha]!='\0';ha++){
						if (strncmp(gr[ha],I[i].rhs[j],temp_ind)==0)
							prod_num = ha + 1;
					}

					for (ha=0; ha<m2 ; ha++){
						tempo_store = strchr(terminals,foll[ha])-terminals;
						if (strlen(slr[i][tempo_store]) > 0){
							if (slr[i][tempo_store][0] == 's')
								printf("\nSR-Conflict occurred....\n");
							else
								printf("\nRR-Conflict occurred....\n");
						}
						else
							sprintf(slr[i][tempo_store],"r%d",prod_num);
					}
					m2=0;
				}
			}
		}
	}
	//printf("Here....\n");
	display_slr();
	//follow(I[3].lhs[0]);

	/////////////////////////////////////////Check for the given string/////////////////////////////////////
	char input[100],stack[100]={'\0'};
	printf("\nGive the input string : ");
	scanf("%s",input);
	int temp32,temp36,len_rhs,state,top=0,input_len = strlen(input);
	
	if (input[input_len-1] != '$')
		input[input_len++] = '$';

	i=0;
	stack[0] = '0';
	printf("\n\nSTACK\t\tINPUT STRING\t\tACTION\n");
	printf("-----\t\t------------\t\t------\n");
	while(i<input_len){
		printf("%s\t\t",stack);
		for (j=i;j<input_len;j++)
			printf("%c",input[j]);
			printf("\t\t\t");

		if (strchr(terminals,input[i])){
			temp32 = strchr(terminals,input[i])-terminals; // only terminals are present in the input....
			state = stack[top] - 48;			
		}
		else{
			printf("Unknown symbol encountered...");
			break;
		}

		if (stack[top]=='1' && input[i]=='$' && strcmp(slr[state][temp32],"accept") == 0 ) {
			printf("String Accepted...\n");
			break;
		}
		else if(slr[state][temp32][0]=='s'){
			top++;
			stack[top++] = input[i];
			stack[top] = slr[state][temp32][1];
			i++;
			printf("Shift %c\n",terminals[temp32]);
		}
		else if(slr[state][temp32][0]=='r'){
			len_rhs = strlen(gr[slr[state][temp32][1]-49]);
			printf("Reduce-step : %c -> %s \n",gl[slr[state][temp32][1]-49],gr[slr[state][temp32][1]-49]);
			for (j=0;j<2*len_rhs;j++){
				stack[top--]='\0';
			}
			temp36 = stack[top] - 48;
			stack[++top] = gl[slr[state][temp32][1]-49];
			//printf("--#-->%c%d<----\n",stack[top],len_ter + strchr(variables,stack[top])-variables);
			stack[++top] = slr[temp36][len_ter + strchr(variables,stack[top-1])-variables][0];
			//printf("Pushed [%c, %c]\n",stack[top-1],stack[top]);
		}
		else{
			printf("String rejected...\n");
			break;
		}	
	}

}

void follow(char c){
//printf("%c\n",c);
 if(gl[0]==c)
  foll[m2++]='$';
 for(i2=0;i2<n;i2++){
    for(j2=0;j2<strlen(gr[i2]);j2++){
      if(gr[i2][j2]==c){
        if(gr[i2][j2+1]!='\0')
          first(gr[i2][j2+1]);

        if(gr[i2][j2+1]=='\0'&&c!=gl[i2])
          follow(gl[i2]);

      }
    }
  }
}

void first(char c){
  int k;
  if(!(isupper(c)))
    foll[m2++]=c;
  for(k=0;k<n;k++){
    if(gl[k]==c){
      if(gr[k][0]=='$') 
        follow(gl[i2]);
      else if(islower(gr[k][0]))
        foll[m2++]=gr[k][0];
      else 
        first(gr[k][0]);
    }
  }

}

/* Comments : 
The code could still be improved, ofcourse by those who have enough time... 
-- KC --
*/
