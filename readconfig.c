#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int SEED=0,itime=0,ftime=0,arrmin=0,arrmax=0,cpumin=0,cpumax=0,disk1min=0,disk1max=0;
double quit=0.0,netprob=0.0;
int disk2min=0,disk2max=0,networkmin=0,networkmax=0;


int readfile()
{
    char* useless="\0";
    FILE* file = fopen("CONFIG.txt", "r");
    if (file == NULL)
    {
        return -1;
    }
    fscanf(file, "%s %d\n", useless, &SEED);
    fscanf(file, "%s %d\n", useless, &itime);
    fscanf(file, "%s %d\n", useless, &ftime);
    fscanf(file, "%s %d\n", useless, &arrmin);
    fscanf(file, "%s %d\n", useless, &arrmax);
    fscanf(file, "%s %lf\n", useless, &quit);
    fscanf(file, "%s %lf\n", useless, &netprob);
    fscanf(file, "%s %d\n", useless, &cpumin);
    fscanf(file, "%s %d\n", useless, &cpumax);
    fscanf(file, "%s %d\n", useless, &disk1min);
    fscanf(file, "%s %d\n", useless, &disk1max);
    fscanf(file, "%s %d\n", useless, &disk2min);
    fscanf(file, "%s %d\n", useless, &disk2max);
    fscanf(file, "%s %d\n", useless, &networkmin);
    fscanf(file, "%s %d\n", useless, &networkmax);
    fclose(file);
    return 0;
}

int main()
{
    readfile();
    printf("%d,%d,%d,%d,%d,%lf,%lf,%d,%d,%d,%d,%d,%d,%d,%d",
           SEED,
           itime,
           ftime,
           arrmin,
           arrmax,
           quit,
           netprob,
           cpumin,
           cpumax,
           disk1min,
           disk1max,
           disk2min,
           disk2max,
           networkmin,
           networkmax);
           return 0;
}