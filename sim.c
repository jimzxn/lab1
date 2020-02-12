#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#define MAX 1000

int cpuqueue[MAX], cpu_fornt = 0, cpu_ = -1, cpu_size = 0;                                                      //cpuqueue number
int disk1queue[MAX], disk1_fornt = 0, disk1_ = -1, disk1_size = 0;                                              //disk1 queue number
int disk2queue[MAX], disk2_fornt = 0, disk2_ = -1, disk2_size = 0;                                              //disk2 queue number
int netqueue[MAX], net_fornt = 0, net_ = -1, net_size = 0;                                                      //net queue number
int SEED = 0, itime = 0, ftime = 0, arrmin = 0, arrmax = 0, cpumin = 0, cpumax = 0, disk1min = 0, disk1max = 0; //config
double quit = 0.0, netprob = 0.0;                                                                               //config
int disk2min = 0, disk2max = 0, networkmin = 0, networkmax = 0;                                                 //config
int jobnumber = 1, globaltime = 0;                                                                              //global track number
int cpu_idle = 0, disk1_idle = 0, disk2_idle = 0, net_idle = 0;                                                 //idle trak 0for idle, 1for using
double cpu_respond_time = 0, disk1_respond_time = 0, disk2_respond_time = 0, net_respond_time = 0;              //responds time ave
int cpu_size_max = 0, disk1_size_max = 0, disk2_size_max = 0, net_size_max = 0, Event_max = 0;                  //size max1
double cpu_usage_time = 0, disk1_usage_time = 0, disk2_usage_time = 0, net_usage_time = 0;                      //usage time1
double cpu_job_c = 0, disk1_job_c = 0, disk2_job_c = 0, net_job_c = 0;                                          //pass though
double cpu_size_ave = 0, disk1_size_ave = 0, disk2_size_ave = 0, net_size_ave = 0, event_size_ave = 0;          // average size
int cpu_respond_time_max = 0, disk1_respond_time_max = 0, disk2_respond_time_max = 0, net_respond_time_max = 0; //respond max

//type event
typedef struct Event
{
    int location;
    int selftime;
    int pid;
} Event;
//func declear
void initiallizer();
int probedinexit();
int jobrand();
int cpurand();
int disk1rand();
int disk2rand();
int netrand();
int readfile();
Event dequeue();
int enqueue();
void add();
int qpop();

/*
    for location 1 for job_arr, 2 for cpu_arr, 3 for cpu eixt,4 for system exit(useless),
    5 for disk 1 arr,6 for disk1 exit, 7 for disk 2 arr, 8 for disk 2 exit,
    9 for net arr, 0 for net exit 
    the func logic for cpu, disk and net is same as the silde show in the lab.
    need config file format in specifical way.
    have 0.1 second space if you choose out put to stdout.
    use > to create log.txt

*/
Event Eventqueue[MAX];
int size = 0; //num of stuff in queue

void Eventqueueprint()
{
    printf("\npriting queue size:%d\n", size);
    for (int i = 0; i < size; i++)
    {
        printf("\nprinting job: %d location %d time %d\n", Eventqueue[i].pid, Eventqueue[i].location, Eventqueue[i].selftime);
    }
}
void job_arr(Event pop)
{
    printf("at time: %d, job %d arrival in the system\n", pop.selftime, pop.pid);
    Event out;
    if (cpu_idle == 0) //cpu is empty
    {
        out.location = 2;
        out.pid = pop.pid;
        out.selftime = pop.selftime;
        enqueue(out, Eventqueue); //cpu_arr event in
        cpu_idle = 1;
    }
    else
    {
        add(&cpuqueue, &cpu_, &pop.pid, &cpu_size); // cpu queue in
    }
    Event nextjob;
    nextjob.location = 1;
    jobnumber++;
    nextjob.pid = jobnumber;
    nextjob.selftime = pop.selftime + jobrand();
    enqueue(nextjob, Eventqueue); // next job
}
void cpu_arr(Event pop)
{
    printf("at time: %d, job %d arrival cpu\n", pop.selftime, pop.pid);
    //Eventqueueprint();
    Event nextjob;
    nextjob.location = 3;
    nextjob.pid = pop.pid;
    int rand = cpurand();
    nextjob.selftime = pop.selftime + rand;
    cpu_usage_time = rand + cpu_usage_time; //total time update
    int res = (nextjob.selftime - pop.selftime);
    cpu_respond_time = (cpu_respond_time + res) / 2; //average responstime
    cpu_job_c = cpu_job_c + 1;                       //job c
    if (cpu_respond_time > cpu_respond_time_max)
    {
        cpu_respond_time_max = cpu_respond_time;
    }
    enqueue(nextjob, Eventqueue); // cpu exit in
    //Eventqueueprint();
}
void cpu_exit(Event pop)
{
    printf("at time: %d, job %d exit the cpu\n", pop.selftime, pop.pid);
    cpu_idle = 0;
    Event out;
    if (cpu_size != 0)
    {
        out.location = 2;
        out.pid = qpop(&cpuqueue, &cpu_, &cpu_fornt, &cpu_size);
        out.selftime = pop.selftime;
        enqueue(out, Eventqueue);
        cpu_idle = 1;
    }
    Event nextjob;
    int rand = probedinexit();

    switch (rand)
    {
    case 0:
        //exit
        globaltime = pop.selftime;
        printf("at time: %d, job %d exit the system\n", pop.selftime, pop.pid);
        return;
        break;
    case 1:
        //net arr
        if (net_idle == 0)
        {
            nextjob.location = 9;
            nextjob.pid = pop.pid;
            globaltime = pop.selftime;
            nextjob.selftime = globaltime;
            enqueue(nextjob, Eventqueue);
            net_idle = 1;
        }
        else
        {
            add(&netqueue, &net_, &pop.pid, &net_size);
        }
        break;
    case 2:
        //disk
        if (disk1_idle == 0)
        {
            nextjob.location = 5;
            nextjob.pid = pop.pid;
            globaltime = pop.selftime;
            nextjob.selftime = globaltime;
            enqueue(nextjob, Eventqueue);
            disk1_idle = 1;
        }
        else if (disk2_idle == 0)
        {
            nextjob.location = 7;
            nextjob.pid = pop.pid;
            globaltime = pop.selftime;
            nextjob.selftime = globaltime;
            enqueue(nextjob, Eventqueue);
            disk2_idle = 1;
        }
        else if (disk1_size < disk2_size)
        {
            add(&disk1queue, &disk1_, &pop.pid, &disk1_size);
        }
        else
        {
            add(&disk2queue, &disk2_, &pop.pid, &disk2_size);
        }
        break;
    default:
        break;
    }
}
void disk1_arr(Event pop)
{
    printf("at time: %d, job %d arrvial the disk1\n", pop.selftime, pop.pid);
    Event nextjob;
    nextjob.location = 6;
    nextjob.pid = pop.pid;
    int rand = disk1rand();
    nextjob.selftime = pop.selftime + rand;
    disk1_usage_time = rand + disk1_usage_time; //total time update
    int res = (nextjob.selftime - pop.selftime);
    disk1_respond_time = (disk1_respond_time + res) / 2; //average responstime
    disk1_job_c = disk1_job_c + 1;                       //job c
    if (disk1_respond_time > disk1_respond_time_max)
    {
        disk1_respond_time_max = disk1_respond_time;
    }
    enqueue(nextjob, Eventqueue);
}
void disk2_arr(Event pop)
{
    printf("at time: %d, job %d arrvial the disk2\n", pop.selftime, pop.pid);
    Event nextjob;
    nextjob.location = 8;
    nextjob.pid = pop.pid;
    int rand = disk2rand();
    nextjob.selftime = pop.selftime + rand;
    disk2_usage_time = rand + disk2_usage_time; //total time update
    int res = (nextjob.selftime - pop.selftime);
    disk2_respond_time = (disk2_respond_time + res) / 2; //average responstime
    disk2_job_c = disk2_job_c + 1;                       //job c
    if (disk2_respond_time > disk2_respond_time_max)
    {
        disk2_respond_time_max = disk2_respond_time;
    }
    enqueue(nextjob, Eventqueue);
}
void net_arr(Event pop)
{
    printf("at time: %d, job %d arrival the net\n", pop.selftime, pop.pid);
    Event nextjob;
    nextjob.location = 0;
    nextjob.pid = pop.pid;
    int rand = netrand();
    nextjob.selftime = pop.selftime + rand;
    net_usage_time = rand + net_usage_time; //total time update
    int res = (nextjob.selftime - pop.selftime);
    net_respond_time = (net_respond_time + res) / 2; //average responstime
    net_job_c = net_job_c + 1;                       //job c
    if (net_respond_time > net_respond_time_max)
    {
        net_respond_time_max = net_respond_time;
    }
    enqueue(nextjob, Eventqueue);
}
void disk1_exit(Event pop)
{
    printf("at time: %d, job %d exit the disk1\n", pop.selftime, pop.pid);
    disk1_idle = 0;
    Event out;
    Event nextjob;
    if (cpu_size == 0)
    {
        nextjob.location = 2;
        nextjob.pid = pop.pid;
        nextjob.selftime = pop.selftime;
        enqueue(nextjob, Eventqueue);
        cpu_idle = 1;
    }
    else
    {
        add(&cpuqueue, &cpu_, &pop.pid, &cpu_size);
    }
    globaltime = pop.selftime;
    if (disk1_size != 0)
    {
        out.location = 5;
        out.pid = qpop(&disk1queue, &disk1_, &disk1_fornt, &disk1_size);
        out.selftime = pop.selftime;
        enqueue(out, Eventqueue);
        disk1_idle = 1;
    }
}
void disk2_exit(Event pop)
{
    printf("at time: %d, job %d arr the disk2\n", pop.selftime, pop.pid);
    disk2_idle = 0;
    Event out;
    if (cpu_size == 0)
    {
        out.location = 2;
        out.pid = pop.pid;
        out.selftime = pop.selftime;
        enqueue(out, Eventqueue);
        cpu_idle = 1;
    }
    else
    {
        add(&cpuqueue, &cpu_, &pop.pid, &cpu_size);
    }
    globaltime = pop.selftime;
    if (disk2_size != 0)
    {
        out.location = 7;
        out.pid = qpop(&disk2queue, &disk2_, &disk2_fornt, &disk2_size);
        out.selftime = pop.selftime;
        enqueue(out, Eventqueue);
        disk2_idle = 1;
    }
}
void net_exit(Event pop)
{
    printf("at time: %d, job %d exit the network\n", pop.selftime, pop.pid);
    net_idle = 0;
    Event out;
    if (cpu_size == 0)
    {
        out.location = 2;
        out.pid = pop.pid;
        out.selftime = pop.selftime;
        enqueue(out, Eventqueue);
        cpu_idle = 1;
    }
    else
    {
        add(&cpuqueue, &cpu_, &pop.pid, &cpu_size);
    }
    globaltime = pop.selftime;
    if (net_size != 0)
    {
        out.location = 9;
        out.pid = qpop(&netqueue, &net_, &net_fornt, &net_size);
        out.selftime = pop.selftime;
        enqueue(out, Eventqueue);
        net_idle = 1;
    }
}
Event dequeue(Event *inqueue)
{
    Event out = inqueue[0];
    if (size >= 1)
    {
        for (int i = 1; i < size; i++)
        {
            inqueue[i - 1] = inqueue[i];
        }
    }
    size--;
    //Eventqueueprint();
    return out;
}

int enqueue(Event input, Event *inqueue)
{
    if (size >= (MAX - 1))
    {
        fprintf(stderr, "-1\n");
        return -1; //-1 for error
    }
    if (size == 0) //initail
    {
        inqueue[0] = input;
        size++;
        return 0;
    }
    int i;
    for (i = 0; i < size; i++)
    {
        if (input.selftime < (inqueue[size - i - 1].selftime))
        {
            inqueue[(size - i)] = inqueue[size - i - 1];
        }
        else
        {

            break;
        }
    }
    inqueue[size - i] = input;
    size++;
    //Eventqueueprint();
    return 0;
}
void add(int queue[], int *queue_, int *queue_in, int *queue_size)
{
    queue[++(*queue_)] = *queue_in;
    (*queue_size)++;
}
int qpop(int queue[], int *queue_, int *queue_front, int *queue_size)
{
    int out = queue[(*queue_front)];
    for (size_t i = 0; i < (*queue_); i++)
    {
        queue[i] = queue[i + 1];
    }
    (*queue_size)--;
    return out;
}

int main()
{
    initiallizer();
    FILE *stat;
    stat = fopen("stat.txt", "w");
    while (size != 0 || cpu_size != 0 || disk1_size != 0 || disk2_size != 0 || net_size != 0)
    {
        Event pop = dequeue(Eventqueue);
        //printf("location:%d:%d:%d\n",pop.location,pop.pid,pop.selftime);
        switch (pop.location)
        {
        case 1:
            job_arr(pop);
            cpu_size_ave = (cpu_size_ave + cpu_size) / 2;
            break;
        case 2:
            cpu_arr(pop);
            break;
        case 3:
            cpu_exit(pop);
            break;
        case 5:
            disk1_arr(pop);
            break;
        case 6:
            disk1_size_ave = (disk1_size_ave + disk1_size) / 2;
            disk1_exit(pop);
            break;
        case 7:
            disk2_arr(pop);
            break;
        case 8:
            disk2_size_ave = (disk2_size_ave + disk2_size) / 2;
            disk2_exit(pop);
            break;
        case 9:
            net_arr(pop);
            break;
        case 0:
            net_size_ave = (net_size_ave + net_size) / 2;
            net_exit(pop);
            break;
        default:
            break;
        }
        sleep(0.1);
        //Eventqueueprint();
        //size update;
        if (cpu_size > cpu_size_max)
        {
            cpu_size_max = cpu_size;
        }
        if (disk1_size > disk1_size_max)
        {
            disk1_size_max = disk1_size;
        }
        if (disk2_size > disk2_size_max)
        {
            disk2_size_max = disk2_size;
        }
        if (net_size > net_size_max)
        {
            net_size_max = net_size;
        }
        if (size > Event_max)
        {
            Event_max = size;
        }
        event_size_ave = (event_size_ave + size) / 2;
        if (globaltime > ftime)
        {
            break;
        }
    }
    fprintf(stat, "-----------Queues---------\nAverage size of CPU Queue: %lf \nMax size of CPU Queue: %d \nAverage size of DISK 1 Queue: %lf\nMax size of DISK 1 Queue: %d\n", cpu_size_ave, cpu_size_max, disk1_size_ave, disk1_size_max);
    fprintf(stat, "Average size of DISK 2 Queue: %lf\nMax size of DISK 2 Queue: %d\n Average size of Event Queue: %lf\nMax size of Event Queue: %d\nAverage size of Network Queue: %lf\nMax size of Network Queue: %d\n\n", disk2_size_ave, disk2_size_max, event_size_ave, Event_max, net_size_ave, net_size_max);
    fprintf(stat, "-----------Utilization---------\nUtilization of CPU: %lf\n Utilization of Disk 1: %lf\n Utilization of Disk 2: %lf\nUtilization of Network: %lf\n\n", (cpu_usage_time / ftime), (disk1_usage_time / ftime), (disk2_usage_time / ftime), (net_usage_time / ftime));
    fprintf(stat,"-----------Repsonse Time---------\nCPU max response time: %d\nDISK 1 max response time: %d\nDISK 2 max response time: %d\nNETWORK max response time: %d\n\n",cpu_respond_time_max,disk1_respond_time_max,disk2_respond_time_max,net_respond_time_max);
    fprintf(stat,"CPU average response time: %lf\nDISK 1 average response time: %lf\nDISK 2 average response time: %lf\nNETWORK average response time: %lf\n\n",cpu_respond_time,disk1_respond_time,disk2_respond_time,net_respond_time);
    fprintf(stat,"-----------Throughput---------\nThroughput of CPU: %lf Jobs per Unit of Time\nThroughput of DISK 1: %lf Jobs per Unit of Time\nThroughput of DISK 2: %lf Jobs per Unit of Time\nThroughput of NETWORK: %lf Jobs per Unit of Time",(cpu_job_c/ftime),(disk1_job_c/ftime),(disk2_job_c/ftime),(net_job_c/ftime));
}
void initiallizer()
{
    srand(SEED);
    readfile();
    Event first;
    first.location = 1;
    first.pid = jobnumber;
    first.selftime = globaltime;
    enqueue(first, Eventqueue);
}

int probedinexit()
{
    //retrun 0 for exit, 1 for net, 2 for disk
    int a = rand();
    a = a % 100;
    double quitc = quit * 100;
    double net = netprob * 100;
    if (a < quitc)
    {
        return 0; //exit
    }
    else if (a < (quitc + net))
    {
        return 1; //net
    }
    else
    {
        return 2; //disk
    }
}
int jobrand()
{
    int out = rand() % (arrmax - arrmin);
    return out + arrmin;
}
int cpurand()
{
    int out = rand() % (cpumax - cpumin);
    return out + cpumin;
}

int disk1rand()
{
    int out = rand() % (disk1max - disk1min);
    return out + disk1min;
}
int disk2rand()
{
    int out = rand() % (disk2max - disk2min);
    return out + disk2min;
}
int netrand()
{
    int out = rand() % (networkmax - networkmin);
    return out + networkmin;
}
int readfile()
{
    FILE *file = fopen("CONFIG.txt", "r");
    if (file == NULL)
    {
        return -1;
    }
    fscanf(file, "%*s %d\n", &SEED);
    fscanf(file, "%*s %d\n", &itime);
    fscanf(file, "%*s %d\n", &ftime);
    fscanf(file, "%*s %d\n", &arrmin);
    fscanf(file, "%*s %d\n", &arrmax);
    fscanf(file, "%*s %lf\n", &quit);
    fscanf(file, "%*s %lf\n", &netprob);
    fscanf(file, "%*s %d\n", &cpumin);
    fscanf(file, "%*s %d\n", &cpumax);
    fscanf(file, "%*s %d\n", &disk1min);
    fscanf(file, "%*s %d\n", &disk1max);
    fscanf(file, "%*s %d\n", &disk2min);
    fscanf(file, "%*s %d\n", &disk2max);
    fscanf(file, "%*s %d\n", &networkmin);
    fscanf(file, "%*s %d\n", &networkmax);
    fclose(file);
    return 0;
}