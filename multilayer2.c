#include <stdio.h>
#include<math.h>

#define numbers 3
#define patterns 7

#define inputnuerons 100
#define hiddennuerons 25
#define outputnuerons 3

#define n 0.75
#define lambda 0.1

void activationfunc(float[] ,float[],int,int,int );
void activationderivative(float[],int);

time_t tl;
//srand (unsigned)time(&t1);
float input[inputnuerons];
float hiddenoutput[hiddennuerons];
float aoutput[outputnuerons]={0};
float doutput[outputnuerons]={0};

float w1[hiddennuerons][inputnuerons]={0};
float w2[outputnuerons][hiddennuerons]={0};

float vjo[outputnuerons]={0};
float vjh[hiddennuerons]={0};

float deltao[outputnuerons]={0};
float deltah[hiddennuerons]={0};

float error[outputnuerons] = {0};

int i,j,k,m,l,numiterations=100000;
float mini;
int indexmini;
char tempchar;

int main()
{

    FILE *testfile;
    FILE *wtfile;
    /**Initialize weight vectors**/

    /*for(i=0;i<100;i++)
    {
        printf("%f ",(((float)rand()/(float)(1000.0))/1000.0));
    }*/

    for(i=0;i<hiddennuerons;i++)
    {
        for(j=0;j<inputnuerons;j++)
        {
            w1[i][j] = (((float)rand()/(float)(1000.0))/1000.0);

        }
    }

    for(i=0;i<outputnuerons;i++)
    {
        for(j=0;j<hiddennuerons;j++)
        {
            w2[i][j] = (((float)rand()/(float)(1000.0))/1000.0);
        }
    }

    while(numiterations--)
    {


    FILE *inputfile = fopen("input.txt","r");

    if(inputfile!=NULL)
    {
        for(i=0;i<numbers;i++)
        {
            for(k=0;k<numbers;k++)
            {
                doutput[k]=0;
                if(k==i)
                {
                    doutput[k]=1.0;
                }
            }



            for(j=0;j<patterns;j++)
            {
                /*store input pattern for a variation of a number*/
                 for(l=0;l<10;l++)
                 {
                     for(m=0;m<11;m++)
                     {

                         if(m==10)
                         {
                             fscanf(inputfile,"%c",&tempchar);
                         }
                         else
                         {
                             fscanf(inputfile,"%1f",&input[l*10+m]);

                         }

                     }

                 }



                 /**** multiply input vector and w1*****/
                 for(l=0;l<hiddennuerons;l++)
                 {
                     vjh[l] = 0.0;

                     for(k=0;k<inputnuerons;k++)
                     {
                         vjh[l] += input[k]*w1[l][k];


                     }

                 }

                 /**call activation function***/
                 activationfunc(hiddenoutput,vjh,hiddennuerons,i,j);


                 /**multiply hidden and w2 to get output**/
                 for(l=0;l<outputnuerons;l++)
                 {

                     vjo[l] = 0.0;
                     for(k=0;k<hiddennuerons;k++)
                     {
                         vjo[l] += hiddenoutput[k]*w2[l][k];
                     }

                 }

                 /**call activation function***/
                 activationfunc(aoutput,vjo,outputnuerons,i,j);


                 /** Calculate error ***/
                 for(l=0;l<outputnuerons;l++)
                 {
                     error[l] = doutput[l] - aoutput[l];
                     if(error[l]>=-0.1 && error[l]<=0.1)
                     {
                         error[l] = 0.0;
                     }
                 }


                /** calculate derivatives of vjo*/
                activationderivative(vjo,outputnuerons);

                /** Modify the weights w2 **/

                for(l=0;l<outputnuerons;l++)
                {
                    deltao[l] = error[l]*vjo[l];

                        //printf("i %d j %d l %d deltao[l] %f vjo[l] %f \n",i,j,l,deltao[l],vjo[l]);


                    for(k=0;k<hiddennuerons;k++)
                    {

                        w2[l][k] = w2[l][k] + n*deltao[l]*hiddenoutput[k];

                        if(l==1 && k==0)
                        {
                            //printf("the mass is w2[1][0] %f deltao[1] %f hiddenoutput[0] %f\n",w2[l][k],deltao[l],hiddenoutput[k]);
                        }

                    }
                }

                if(i==2)
                {
                    //printf("the weight and mass is w2[1][0] %f\n",w2[1][0]);
                }


                /** calculate derivatives of vjh*/

                activationderivative(vjh,hiddennuerons);

                /** Calculate delta of hidden layer **/

                for(l=0;l<hiddennuerons;l++)
                {
                    deltah[l] = 0.0;

                    for(k=0;k<outputnuerons;k++)
                    {

                        deltah[l] += deltao[k]*w2[k][l];
                        if(i==2 && l==0)
                        {
                            //printf("k %d l %d w2[k][l] %f\n",k,l,w2[k][l]);
                        }
                    }

                    deltah[l] = vjh[l] * deltah[l];
                }

                /** modify the weights w1**/



                for(l=0;l<hiddennuerons;l++)
                {
                    for(k=0;k<inputnuerons;k++)
                    {
                        w1[l][k] = w1[l][k] + n*deltah[l]*input[k];
                        if(l==0 && k==0)
                        {
                            //printf("i %d j %d n %f w1[l][k] %f deltah[l] %f input[k]  %f\n",i,j,n,w1[l][k],deltah[l],input[k]);
                        }

                    }
                }



            }
            fscanf(inputfile,"%c",&tempchar);
        }

        fclose(inputfile);
    }
        else
    {
        printf("Input file not found\n");
    }

    }

        testfile = fopen("test.txt","r");

        if(testfile!=NULL)
        {   /****************** TESTING STARTS HERE ******************************/
            for(l=0;l<10;l++)
                 {
                     for(m=0;m<11;m++)
                     {

                         if(m==10)
                         {
                             fscanf(testfile,"%c",&tempchar);
                         }
                         else
                         {
                             fscanf(testfile,"%1f",&input[l*10+m]);

                         }

                     }

                 }
            fclose(testfile);

            wtfile = fopen("wtfile.txt","w");

            fprintf(wtfile,"W1 \n\n");
            for(l=0;l<hiddennuerons;l++)
            {
                for(k=0;k<inputnuerons;k++)
                {
                    fprintf(wtfile,"%f ",w1[l][k]);
                }
                fprintf(wtfile,"\n");
            }

            fprintf(wtfile,"\n\nW2 \n\n");
            for(l=0;l<outputnuerons;l++)
            {
                for(k=0;k<hiddennuerons;k++)
                {
                    fprintf(wtfile,"%f ",w2[l][k]);
                }
                fprintf(wtfile,"\n");
            }


            /**** multiply input vector and w1*****/
                 for(l=0;l<hiddennuerons;l++)
                 {
                     vjh[l] = 0.0;
                     //hiddenoutput[l] = 0;
                     for(k=0;k<inputnuerons;k++)
                     {
                         vjh[l] += input[k]*w1[l][k];

                     }
                     //vjh[l] = hiddenoutput[l];
                 }

                 /**call activation function***/
                 activationfunc(hiddenoutput,vjh,hiddennuerons,i,j);

                 /**multiply hidden and w2 to get output**/
                 for(l=0;l<outputnuerons;l++)
                 {
                     //aoutput[l] = 0.0;
                     vjo[l] = 0.0;
                     for(k=0;k<hiddennuerons;k++)
                     {
                         vjo[l] += hiddenoutput[k]*w2[l][k];
                     }
                     //vjo[l] = aoutput[l];
                 }

                 /**call activation function***/
                 activationfunc(aoutput,vjo,outputnuerons,i,j);


                 /** Calculate error ***/
                 for(l=0;l<outputnuerons;l++)
                 {
                     printf("\n\ntesting output  is %f  for iteration  l %d\n\n",aoutput[l],l);

                     if(i==2 && j == 1)
                     {
                         //printf("Number %d pattern %d error in %d node is %f\n",i,j,l,error[l]);
                         //printf("doutput is %f aoutput is %f\n",doutput[l],aoutput[l]);
                     }
                 }

        }
        else
        {
            printf("Cannot open test file\n");
        }


    return 0;
}

void activationfunc(float numarray[],float numarray2[],int size,int i,int j)
{

    int p;
    for(p=0;p<size;p++)
    {
        numarray[p] = 1/(1+expf(-1*lambda*numarray2[p]));
        if(i==2 && j==1)
        {
            //printf("p is %d bcf %f asd %f\n",p,numarray2[p],expf(-1*lambda*numarray2[p]));
            //printf("In act func %f\n",numarray[p]);
        }
    }
}

void activationderivative (float numarray[], int size)
{
    int i;
    float temp;
    for(i=0;i<size;i++)
    {
        temp = (1+expf(-1*lambda*numarray[i]));
        //if(size==3)
        //printf("old i %d numarray[i] %f temp %f ",i,numarray[i],temp);
        numarray[i] = (lambda*expf(-1*lambda*numarray[i]))/(temp*temp);
        //if(size==3)
        //printf("new numarray[i] %f\n",numarray[i]);
    }
}







