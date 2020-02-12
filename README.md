Xiaoning Zhao
3207 project 1
event simulater

theis program is use to make a simulate OS workflow.

I using a Event typedef stuct to track down event and unique pid for every job tracking.
I build for queue to be the FIFO of four vitral hardware buffer, and a giant Event type queue whihc sort by selftimer to tracking event.
first i start the programe by pushing first job arrival event in event queue to boot the strat program. then it will loop if all five queue is not empty. 
