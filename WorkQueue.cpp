/* 
 * File:   WorkQueue.cpp
 * Author: dan
 * Note:  THIS IS NOT THREAD SAFE
 * 
 * Created on February 10, 2012, 12:21 PM
 */
#include "othello.h"
#include "WorkQueue.h"

WorkQueue::WorkQueue() {  // board size dictates the amount of memory to allocate
	qs = BS*BS*(MAX_REQUEST_SIZE);
	queue = (char*)malloc(qs);
	head = queue;
	tail = queue;
	*head = 0;
	endOfQueue = queue + qs;
}

WorkQueue::WorkQueue(const WorkQueue& orig) {
}

WorkQueue::~WorkQueue() {
}
char* WorkQueue::wrap(char* &p) {
	p++;
	if (p > endOfQueue)
		p -= qs;
	return p;
}

char* WorkQueue::pop(char *p) {
	char *start;
	start = p;
	do {
		*p++ = *wrap(tail);
	} while (*tail);
	return start; 
}

bool WorkQueue::push(char* p) {
	if (isFull()) {
		return false;
	}
	do {
		*wrap(head) = *p++;
	} while (*p);
	*wrap(head) = 0;
	outstandingWork++;
	
	return true;
}

bool  WorkQueue::isEmpty() {
	return head == tail;
}

bool  WorkQueue::isFull() {
	int freeSpace = (tail - head + qs) % qs;
	return freeSpace > MAX_REQUEST_SIZE;
}

void WorkQueue::reset() {
	head = queue;
	tail = queue;
	*head = 0;
}

void WorkQueue::resultReceived() {
	outstandingWork--;
}

int WorkQueue::getOutstandingWork() {
	return outstandingWork;
}