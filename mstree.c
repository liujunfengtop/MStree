/* mstree.c

   Junfeng Liu, 2018
   
   This is an program to compute theta0, theta1, tau0, and tau1 for the 3-species migration problem.

   gcc -o mstree mstree.c tools.c -lm
   
   mstree <ctlfile>

*/

#include "paml.h"


struct CommonInfo {
   char outf[128], seqf[128], ctlf[128];
   int seed, getSE;
   int ndata;
}  com;

int multiplier=100;
double inipara[8]; /* theta0 theta1 tau0 tau1 qbeta */
double msmultiplier;
double acc;

int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0;
int m[3];


int GetOptions (char *ctlf);
int ReadTreeData(FILE*fout, char seqfile[]);
int CountTreeData(char seqfile[], double tau0, float tau1);
int CountTreeData1(char seqfile[], float t, int j);
int CountTreeData2(char seqfile[], double t, int j);
int CountTreeData3(char seqfile[], double t, int j);
int CountTreeData4(char seqfile[], float t, float s, int j);
int CountTreeData5(char seqfile[], double tau0, double tau1);
int GetInitialpara();



FILE *fout;

int main (int argc, char* argv[])
{
   char VerStr[32] = "Version 1.1, July 2018";
   int i;
   
   strcpy(com.ctlf, "mstree.ctl");
   if(argc>1) strcpy(com.ctlf, argv[1]);
   starttimer();
   GetOptions (com.ctlf);

   fout=gfopen(com.outf, "w");

   printf("mstree (%s)\n", VerStr);
   fprintf(fout, "mstree (%s)\n", VerStr);

   ReadTreeData(fout,com.seqf);
  
   GetInitialpara();
   fprintf(fout, "theta0 theta1 tau0 tau1 are %.5f %.5f %.5f %.5f\n", inipara[0], inipara[1], inipara[2], inipara[3]);

   fclose(fout);
   return 0;
}


int GetOptions (char *ctlf)
{
   int iopt,i, nopt=4, lline=4096;
   char line[4096],*pline, opt[32], *comment="*#", *seqerrstr="0EF";
   char *optstr[] = {"seed", "outfile", "seqfile", "getSE"};
   double t=1;
   FILE  *fctl=gfopen (ctlf, "r");
   
   int m1, m2, m3, m4;

   if (fctl) {
      for (;;) {
         if(fgets(line, lline, fctl) == NULL) break;
         if(line[0]=='/' && line[1]=='/') 
            break;
         for (i=0,t=0,pline=line; i<lline&&line[i]; i++)
            if (isalnum(line[i]))  { t=1; break; }
            else if (strchr(comment,line[i])) break;
         if (t==0) continue;
         sscanf (line, "%s%*s%lf", opt, &t);
         if ((pline=strstr(line, "="))==NULL)
            continue;

         for (iopt=0; iopt<nopt; iopt++) {
            if (strncmp(opt, optstr[iopt], 8)==0)  {
               switch (iopt) {
                  case ( 0): com.seed=(int)t;                    break;
                  case ( 1): sscanf(pline+1, "%s", com.outf);    break;
                  case ( 2): sscanf(pline+1, "%s", com.seqf);    break;
                  case ( 3): 
                     sscanf(pline+1, "%d%d%d%d", &m1, &m2, &m3, &m4); 
                     msmultiplier=(double)m1/m2;
                     acc=(double)m3/m4;
                     break;
               }
               break;
            }
         }
         if (iopt==nopt)
            { printf ("\noption %s in %s\n", opt, ctlf);  exit (-1); }
      }
      fclose(fctl);
   }

   return(0);
}




int ReadTreeData(FILE*fout, char seqfile[])
{
	FILE *fin = gfopen(seqfile,"r");
	char str1[100];
	while(!feof(fin)) {
        fgets(str1,100,fin); 
    	if(str1[0]=='('&&str1[1]=='3') {
    		n1=n1+1;
    	}
    	if(str1[0]=='('&&str1[1]=='2') {
    		n2=n2+1;
    	}
    	if(str1[0]=='('&&str1[1]=='1') {
    		n3=n3+1;
    	}

    }
    fclose(fin);
    fprintf(fout, "The number of tree ((1,2),3)is %d\n", n1);
    fprintf(fout, "The number of tree ((1,3),2)is %d\n", n2);
    fprintf(fout, "The number of tree ((2,3),1)is %d\n", n3);   
	return(0);
}

int CountTreeData(char seqfile[], double tau0, float tau1)
{
	FILE *fin = gfopen(seqfile,"r");
	char str1[100],str2[100];
	float t;
	int i, k;
	
	n4=0;
	n5=0;
	n6=0;
	
	while(!feof(fin)) {
		fgets(str1,100,fin);
		if(str1[0]=='(') {
			memset(str2,0,sizeof(str2));
			k=0;
			for(i=3;i<100;i++) {
				if(str1[i]==','&&k==0) {
					k=i+4;
				} else {
					if(str1[i]==',') {
						break;
					} else {
						if(k>0&&i>=k) {
							str2[i-k]=str1[i];
						}
					}
				}
			}
			sscanf(str2, "%f", &t);
			if(t>=tau1&&t<tau0) {
				n4=n4+1;
			}
			if(t>=tau1) {
				n5=n5+1;
			}
			if(t<tau1) {
				n6=n6+1;
			}
		}
	}
	fclose(fin);
	return(0);
}

int CountTreeData1(char seqfile[], float t, int j)
{
	FILE *fin = gfopen(seqfile,"r");
	char str1[100],str2[100];
	float t1, t2, t3, t4;
	int i;
	
	for(i=0;i<3;i++) {
		m[i]=0;
	}
	
	t2=(1.0+0.1*j)*t;
	t3=(1.0+0.1*(j+1.0))*t;
	t4=(1.0+0.1*(j+2.0))*t;
	while(!feof(fin)) {
		fgets(str1,100,fin);
		if(str1[0]=='(') {
			memset(str2,0,sizeof(str2));
			for(i=3;i<100;i++) {
				if(str1[i]==',') {
					break;
				} else {
					str2[i-3]=str1[i];
				}
			}
			sscanf(str2, "%f", &t1);
			if(t1>=t2&&t1<t4) {
				m[0]=m[0]+1;
			}
			if(t1>=t3&&t1<t4) {
				m[1]=m[1]+1;
			}
			if(t1>=t4) {
				m[2]=m[2]+1;
			}
		}
	}
	fclose(fin);
	return(0);
}

int CountTreeData2(char seqfile[], double t, int j)
{
	FILE *fin = gfopen(seqfile,"r");
	char str1[100],str2[100];
	float t1; 
	double t2, t3, t4;
	int i, k;
	
	for(i=0;i<3;i++) {
		m[i]=0;
	}
	
	t2=(0.5+0.01*j)*t;
	t3=(0.5+0.01*(j+1.0))*t;
	t4=t;
	while(!feof(fin)) {
		fgets(str1,100,fin);
		if(str1[0]=='(') {
			memset(str2,0,sizeof(str2));
			k=0;
			for(i=3;i<100;i++) {
				if(str1[i]==','&&k==0) {
					k=i+4;
				} else {
					if(str1[i]==',') {
						break;
					} else {
						if(k>0&&i>=k) {
							str2[i-k]=str1[i];
						}
					}
				}
			}
			sscanf(str2, "%f", &t1);
			if(t1>=t2&&t1<t4) {
				m[0]=m[0]+1;
			}
			if(t1>=t3&&t1<t4) {
				m[1]=m[1]+1;
			}
			if(t1>=t4) {
				m[2]=m[2]+1;
			}
		}
	}
	fclose(fin);
	return(0);
}

int CountTreeData3(char seqfile[], double t, int j)
{
	FILE *fin = gfopen(seqfile,"r");
	char str1[100],str2[100];
	float t1; 
	double t2, t3, t4;
	int i, k;
	
	for(i=0;i<3;i++) {
		m[i]=0;
	}
	
	t2=(0.01*j)*t;
	t3=(0.01*(j+1.0))*t;
	t4=t;
	while(!feof(fin)) {
		fgets(str1,100,fin);
		if(str1[0]=='(') {
			memset(str2,0,sizeof(str2));
			k=0;
			for(i=3;i<100;i++) {
				if(str1[i]==','&&k==0) {
					k=i+4;
				} else {
					if(str1[i]==',') {
						break;
					} else {
						if(k>0&&i>=k) {
							str2[i-k]=str1[i];
						}
					}
				}
			}
			sscanf(str2, "%f", &t1);
			if(t1>=t2&&t1<t4) {
				m[0]=m[0]+1;
			}
			if(t1>=t3&&t1<t4) {
				m[1]=m[1]+1;
			}
			if(t1>=t4) {
				m[2]=m[2]+1;
			}
		}
	}
	fclose(fin);
	return(0);
}

int CountTreeData4(char seqfile[], float t, float s, int j)
{
	FILE *fin = gfopen(seqfile,"r");
	char str1[100],str2[100];
	float t1, t2;
	int i, k;
	
	for(i=0;i<3;i++) {
		m[i]=0;
	}
	
	t2=t+((s-t)/1000)*(j-1);

	
	while(!feof(fin)) {
		fgets(str1,100,fin);
		if(str1[0]=='('&&str1[1]=='3') {
			memset(str2,0,sizeof(str2));
			k=0;
			for(i=3;i<100;i++) {
				if(str1[i]==','&&k==0) {
					k=i+4;
				} else {
					if(str1[i]==',') {
						break;
					} else {
						if(k>0&&i>=k) {
							str2[i-k]=str1[i];
						}
					}
				}
			}
			sscanf(str2, "%f", &t1);
			if(t1>=t2) {
				m[0]=m[0]+1;
			}
		}
	}	
	
	fclose(fin);
	return(0);
}

int CountTreeData5(char seqfile[], double tau0, double tau1)
{
	FILE *fin = gfopen(seqfile,"r");
	char str1[100],str2[100];
	float t;
	int i, k;

	for(i=0;i<3;i++) {
		m[i]=0;
	}
	
	while(!feof(fin)) {
		fgets(str1,100,fin);
		if(str1[0]=='(') {
			memset(str2,0,sizeof(str2));
			k=0;
			for(i=3;i<100;i++) {
				if(str1[i]==','&&k==0) {
					k=i+4;
				} else {
					if(str1[i]==',') {
						break;
					} else {
						if(k>0&&i>=k) {
							str2[i-k]=str1[i];
						}
					}
				}
			}
			sscanf(str2, "%f", &t);
			if(t>=tau1&&t<tau0) {
				m[0]=m[0]+1;
			}
			if(t>=tau0) {
				m[1]=m[1]+1;
			}
		}
	}
	fclose(fin);
	return(0);
}



int GetInitialpara ()
{
   int i,j,k;
   double e, e1=1.0,e2=1.0;
   float t=10000.0, t1, t2, t3, t4=10000.0, t5;
   char str1[100],str2[100];

   FILE *fin = gfopen(com.seqf,"r");
   
   e=acc;
   
   j=0;
   t2=0.0;
   t5=0.0;
   
   while(!feof(fin)) {
   	
  
		fgets(str1,100,fin);
		if(str1[0]=='(') {
			memset(str2,0,sizeof(str2));
			for(i=3;i<100;i++) {
				if(str1[i]==',') {
					k=i+4;
					break;
				} else {
					str2[i-3]=str1[i];
				}
			}
			sscanf(str2, "%f", &t1);
			memset(str2,0,sizeof(str2));
			for(i=k;i<100;i++) {
				if(str1[i]==',') {
					break;
				} else {
					str2[i-k]=str1[i];
				}
			}
			sscanf(str2, "%f", &t3);
			if (t1<t) {
				t=t1;
			}
			if (t3<t4) {
				t4=t3;
			}
			if(t3>t5) {
				t5=t3;
			}
			t2=t2+t1;
			j=j+1;
	  }
  }
	
	fclose(fin);
	
	com.ndata=j;
	
	i=1;
	while(e1>e) {
		CountTreeData4(com.seqf,t4,t5,i);
		if(i==1001) {
			e1=0.5*e;
		} else {
			e1=((double)(m[0]*2)/(n2+n3))-1.0;
			if(e1<0) {
				e1=0-e1;
			}
			if(e1<e2) {
				e2=e1;
				inipara[2]=(t4+((t5-t4)/1000)*(i-1))*msmultiplier;
			}
			i=i+1;
		}
	}
	
	
	i=1;
	e1=1.0;
	e2=1.0;
	while(e1>e||e2>e) {
		CountTreeData1(com.seqf,t,i);
		if((m[0]-m[1])==0||(m[0]+m[2])==(m[0]-m[1])) {
			inipara[0]=2*((t2*msmultiplier/j)-inipara[2]);
			e1=0.5*e;
			e2=0.5*e;
		} else {
		inipara[0]=(-2)*(0.1*msmultiplier*t)/log(1-(double)(m[0]-m[1])/(m[0]+m[2]));
	  e1=(exp((-2)*(0.2*msmultiplier*t)/inipara[0])-(double)(m[2])/(m[2]+m[0]))/((double)(m[2])/(m[2]+m[0]));
	  if(e1<0) {
	  	e1=0-e1;
	  }
	  e2=(exp((-2)*(0.1*msmultiplier*t)/inipara[0])-(double)(m[2])/(m[2]+m[1]))/((double)(m[2])/(m[2]+m[1]));
	  if(e2<0) {
	  	e2=0-e2;
	  }
	  i=i+1;
	  if((((double)(m[1])/m[2])<0.1)||(((double)(m[0]-m[1])/m[0])<0.5)) {
	  	e1=1.1*e;
	  	e2=1.1*e;
	  }	  
		}
	}
	

	i=0;
	while(((0.5+0.01*i)*inipara[2]/msmultiplier)<t4) {
		i=i+1;
	}
	
	e1=1.0;
	e2=1.0;
	
	while(e1>e||e2>e) {
		CountTreeData2(com.seqf,inipara[2]/msmultiplier,i);
		if((m[0]-m[1])==0||(m[0]+m[2])==(m[0]-m[1])||i==50) {
			CountTreeData5(com.seqf, inipara[2]/msmultiplier, (inipara[2]*0.5)/msmultiplier);
			inipara[1]=(-2)*(inipara[2]*0.5)/log(1-(double)(m[0])/(m[0]+m[1]));
			e1=0.5*e;
			e2=0.5*e;
		} else {
		inipara[1]=(-2)*(0.01*inipara[2])/log(1-(double)(m[0]-m[1])/(m[0]+m[2]));
		e1=(exp((-2)*((0.5-0.01*i)*inipara[2])/inipara[1])-(double)(m[2])/(m[2]+m[0]))/((double)(m[2])/(m[2]+m[0]));
	  if(e1<0) {
	  	e1=0-e1;
	  }
	  e2=(exp((-2)*((0.5-0.01*(i+1))*inipara[2])/inipara[1])-(double)(m[2])/(m[2]+m[1]))/((double)(m[2])/(m[2]+m[1]));
	  if(e2<0) {
	  	e2=0-e2;
	  }
	  i=i+1;
		}
	}

	i=1;
	while(((0.01*(i+1))*inipara[2]/msmultiplier)<t4) {
		i=i+1;
	}
	
	e1=1.0;
	e2=1.0;

	
	while(e1>e||e2>e) {
		CountTreeData3(com.seqf,inipara[2]/msmultiplier,i);
		if((m[0]-m[1])==0||(m[0]+m[2])==(m[0]-m[1])||i==100) {
			inipara[3]=0.5*inipara[2];;
			e1=0.5*e;
			e2=0.5*e;
		} else {
		inipara[3]=0.01*i*inipara[2];
	  e1=(exp((-2)*((1.0-0.01*i)*inipara[2])/inipara[1])-(double)(m[2])/(m[2]+m[0]))/((double)(m[2])/(m[2]+m[0]));
	  if(e1<0) {
	  	e1=0-e1;
	  }
	  e2=(exp((-2)*((1.0-0.01*(i+1))*inipara[2])/inipara[1])-(double)(m[2])/(m[2]+m[1]))/((double)(m[2])/(m[2]+m[1]));
	  if(e2<0) {
	  	e2=0-e2;
	  }
	  i=i+1;
		}
	}


	CountTreeData(com.seqf, inipara[2]/msmultiplier, t4);
	e1=((1-exp(-2*(inipara[2]-(t4*msmultiplier))/inipara[1]))-((double)(n4)/n5))/((double)(n4)/n5);
	if(e1<0) {
		e1=0-e1;
	}
	
	if(e1<=e) {
		inipara[3]=t4*msmultiplier;
		inipara[4]=0;
	} else {
		inipara[4]=(((double)2/3)*exp(-2*(inipara[2]-inipara[3])/inipara[1])*(n1+n2+n3)-(n2+n3))/(((double)2/3)*exp(-2*(inipara[2]-inipara[3])/inipara[1])*(n1+n2+n3));
		if(inipara[4]<0) {
			CountTreeData(com.seqf, inipara[2]/msmultiplier, inipara[3]/msmultiplier);
			inipara[4]=(double)(n6)/com.ndata;
		}
	}
	
	for(i=0; i<4; i++) inipara[i]=inipara[i]*multiplier;
	inipara[3]=inipara[3]/inipara[2];
	
	printf("\n theta0 tau0 theta1 tau1 p are %9.5f %9.5f %9.5f %9.5f %9.5f\n", inipara[0],inipara[2],inipara[1],inipara[3],inipara[4]); 
  return(0);
}



