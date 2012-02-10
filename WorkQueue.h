/* 
 * File:   WorkQueue.h
 * Author: dan
 *
 * Created on February 10, 2012, 12:21 PM
 */

#ifndef WORKQUEUE_H
#define	WORKQUEUE_H

class WorkQueue {
public:
	WorkQueue();
	WorkQueue(const WorkQueue& orig);
	virtual ~WorkQueue();
	void  reset();
	char* pop(char *);
	bool  push(char *);
	bool  isEmpty();
	bool  isFull();
	void  resultReceived();
	int   getOutstandingWork();
private:
	int	  outstandingWork;
	int   maxRequestSize;
	int   qs;
	char* endOfQueue;
	char* head;
	char* tail;
	char* queue;
	char* wrap(char* &);
};

#endif	/* WORKQUEUE_H */

