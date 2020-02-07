#include<stdio.h>
#include<stdlib.h>

int SEED=0;
int itime=0;
int ftime=0;
int arrmin=0;
int arrmax=0;
double quit=0.0;
double netprob=0.0;
int cpumin=0;
int cpumax=0;
int disk1min=0;
int disk1max=0;
int disk2min=0;
int disk2max=0;
int networkmin=0;
int networkmax=0;

void initiallizer(){
    srand(SEED);
}

int probedinexit(){
    //retrun 
    int a=rand();
    a=a%3;
    double quitc= quit*3;
    double net= netprob*3;
    if (a<quitc)
    {
        return 0; //exit
    }else if (a<(quitc+net))
    {
        return 1;//net
    }else
    {
        return 2;//disk
    } 
}

int cpurand(){
    int out=rand()%(cpumax-cpumin);
    return out+cpumin;
}

int disk1rand(){
    int out=rand()%(disk1max-disk1min);
    return out+disk1min;
}
int disk2rand(){
    int out=rand()%(disk2max-disk2min);
    return out+disk2min;
}
int netrand(){
    int out=rand()%(networkmax-networkmin);
    return out+networkmin;
}