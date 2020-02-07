#include <stdio.h>
#include <stdlib.h>

#define MAX 100
typedef struct Event
{
    int location;
    int selftime;
    int pid;
} Event;

Event Eventqueue[MAX];
int fornt = 0; //first queue
int rear = -1; //-1 means nothing in queue
int size = 0;  //num of stuff in queue

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
    inqueue[++rear] = input;
    return 0;
}
void print(Event in){
    printf("loc:%d,pid:%d,time:%d\n",in.location,in.pid,in.selftime);
}
int main()
{
    Event one;
    Event two;
    Event three;
    Event four;
    one.location = 1;
    one.pid = 1;
    one.selftime = 0;
    two.location = 2;
    two.pid = 2;
    two.selftime = 5;
    three.location = 3;
    three.pid = 3;
    three.selftime = 10;
    four.location = 4;
    four.pid = 4;
    four.selftime = 15;
    enqueue(one,Eventqueue);
    print(Eventqueue[rear]);
    enqueue(two,Eventqueue);
    print(Eventqueue[rear]);
    enqueue(three,Eventqueue);
    print(Eventqueue[rear]);
    enqueue(four,Eventqueue);
    print(Eventqueue[rear]);
    print(dequeue(Eventqueue));
    print(dequeue(Eventqueue));
    print(dequeue(Eventqueue));
    print(dequeue(Eventqueue));
}
