#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 100

int cpuqueue[15],cpu_fornt = 0,cpu_rear = -1,cpu_size = 0;//cpuqueue number
int disk1queue[15],disk1_fornt = 0,disk1_rear = -1,disk1_size = 0;//disk1 queue number
int disk2queue[15],disk2_fornt = 0,disk2_rear = -1,disk2_size = 0;//disk2 queue number
int netqueue[15],net_fornt = 0,net_rear = -1,net_size = 0;//net queue number
int SEED=0,itime=0,ftime=0,arrmin=0,arrmax=0,cpumin=0,cpumax=0,disk1min=0,disk1max=0;//config
double quit=0.0,netprob=0.0;//config
int disk2min=0,disk2max=0,networkmin=0,networkmax=0;//config
int jobnumber=0, globaltime=0;//global track number
int cpu_idle=0,disk1_idle=0,disk2_idle=0,net_idle=0;//idle trak 0for idle, 1for using
int cpu_finnish_time=0,disk1_finnish_time=0,disk2_finnish_time=0,net_finnish_time=0;// for queue waiting
typedef struct Event
{
    int location;
    int selftime;
    int pid;
} Event;
void initiallizer();
int probedinexit();
int jobrand();
int cpurand();
int disk1rand();
int disk2rand();
int netrand();
/*
    for location 1 for job_arr, 2 for cpu_arr, 3 for cpu compute, 4 for cpu eixt,
    5 for disk 1 arr,6 for disk1 exute, 7 for disk 2 arr, 8 for disk 2 excute,
    9 for net arr, 0 for net excute 
*/
Event Eventqueue[MAX];
int fornt = 0; //first queue
int rear = -1; //-1 means nothing in queue
int size = 0;  //num of stuff in queue

void job_arr(Event pop){
    Event out;
    out.location=2;
    out.pid=pop.pid;
    out.selftime=pop.selftime;
    enqueue(out,Eventqueue);
    Event nextjob;
    nextjob.location=1;
    nextjob.pid=++jobnumber;
    nextjob.selftime=globaltime+jobrand();
    enqueue(nextjob,Eventqueue);
}
void cpu_arr(Event pop){
    Event nextjob;
    if(cpu_idle==0){
        nextjob.location=3;
        nextjob.pid=pop.pid;
        nextjob.selftime=pop.selftime;
        enqueue(nextjob,Eventqueue);// go inside cpu
        cpu_idle=1;
        return 0;
    }
    nextjob.location=2;
    nextjob.pid=pop.pid;
    nextjob.selftime=pop.selftime+cpu_finnish_time;//stay inqueue to wait
    enqueue(nextjob,Eventqueue);

}
void cpu_excute(Event pop){
    Event nextjob;
    nextjob.location=4;
    nextjob.pid=pop.pid;
    int rand= cpurand();
    nextjob.selftime=pop.selftime+rand;
    cpu_finnish_time=globaltime+rand;
    enqueue(nextjob,Eventqueue);
    cpu_idle=0;
}
void cpu_exit(Event pop){
    Event nextjob;
    switch (probedinexit())
    {
    case 0:
        //exit
        return;
        break;
    case 1:
        //net
        nextjob.location=9;
    case 2:
        //disk
        if (disk1_idle==0)
        {
            nextjob.location=5;
        }else if (disk2_idle==0)
        {
            nextjob.location=7;
        }else if(disk1_size>disk2_size)
        {   
            nextjob.location=7;
        }else
        {
            nextjob.location=5;
        }
    default:
        break;
    }
    nextjob.pid=pop.pid;
    globaltime=pop.selftime;
    nextjob.selftime=globaltime;
}
Event dequeue(Event* inqueue)
{
    Event out = inqueue[fornt];
    for (size_t i = 0; i < rear; i++)
    {
        inqueue[i] = inqueue[i + 1];
    }
    return out;
}

int enqueue(Event input,Event* inqueue)
{
    if (rear == (MAX - 1))
    {
        return -1; //-1 for error
    }
    for (size_t i = 0; i < size; i++)
    {
        if (input.selftime<inqueue[rear-i].selftime)
        {
            inqueue[rear-i]=inqueue[rear-i-1];
            continue;
        }
        inqueue[rear-i]=input;
    }
    return 0;
}
void add(int queue[], int queue_rear, int queue_in,int queue_size){
    queue[++rear]=queue_in;
    queue_size++;
}
int qpop(int queue[],int queue_rear, int queue_front, int queue_size){
    int out=queue[queue_front];
    for (size_t i = 0; i < queue_rear; i++)
    {
        queue[i]=queue[i+1];
    }
    queue_size--;
    return out;
    
}

int main(){
    while (size!=0)
    {
        Event pop=dequeue(Eventqueue);
        switch (pop.location)
        {
        case 1:
            job_arr(pop);
            break;
        case 2:
            cpu_arr(pop);
            break;
        default:
            break;
        }
    }
    
}
void initiallizer(){
    srand(SEED);
}

int probedinexit(){
    //retrun 0 for exit, 1 for net, 2 for disk 
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
int jobrand(){
    int out=rand()%(arrmax-arrmin);
    return out+arrmin;
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
