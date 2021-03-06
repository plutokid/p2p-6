#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<map>

#include "main.h"
#include "keepAliveTimer.h"


//Keep alive timer thread, sends the KEEP ALIVE message every keepalivetimeout/3
//this message indicates that the node is still alive
void *keepAliveTimer_thread(void *arg){

while(!shutDown)
{
	sleep(1);

	//global flag denttoing shutdown of the nodes
	if(shutDown)
	{
		//printf("KeepAlive thread halted\n");
		break;
	}
	//for (map<int, struct connectionNode>::iterator it = connectionMap.begin(); it != connectionMap.end(); ++it){
	//iterate through the neighbprs list and send KEEP ALIVE messages
	pthread_mutex_lock(&nodeConnectionMapLock) ;
	for (map<struct node, int>::iterator it = nodeConnectionMap.begin(); it != nodeConnectionMap.end(); ++it){
				//printf("Hi I am here!!!!\n");
		pthread_mutex_lock(&connectionMapLock) ;
		if(connectionMap[(*it).second].keepAliveTimer > 0)
			connectionMap[(*it).second].keepAliveTimer--;
		else
		{
			if(connectionMap[(*it).second].keepAliveTimeOut!=-1 && connectionMap[(*it).second].isReady==2)
			{
				struct Message mes;
				mes.type = 0xf8;
				mes.status = 0;
				//printf("Sent Keep Alive Message to: %d\n", (*it).first);
				pushMessageinQ((*it).second, mes);
			}
		}
		pthread_mutex_unlock(&connectionMapLock) ;		
	}
	pthread_mutex_unlock(&nodeConnectionMapLock) ;
}
pthread_exit(0);
return 0;
}

