#include<stdio.h>
#include <pthread.h>
#include <iostream> 


#include "rwlock.h"

//Your solution to implement each of the following methods
RWLock::RWLock() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&okToRead, NULL);
    pthread_cond_init(&okToWrite, NULL);

    activeReaders = 0;
    activeWriters = 0;
    waitingReaders = 0;
    waitingWriters = 0;
}

RWLock::~RWLock() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&okToRead);
    pthread_cond_destroy(&okToWrite);
}

// Wait until no active or waiting writes, then proceed
void RWLock::startRead() {
    lock.acquire();
    while (readShouldWait()) {
        readGo.Wait(&lock);
    }
    lock.release(); 
}

// Done reading. If no other active reads, a write may proceed
void RWLock::doneRead() {
    lock.acquire();
    activeReaders--;
    if (activeReaders == 0
        && waitingWriters > 0) {
        writeGo.signal();
    }
    lock.release();
}

// Wait until no active read or write then proceed
void RWLock::startWrite() {
    lock.acquire();
    waitingWriters++; 
    while (writeShouldWait()) {
        writeGo.Wait(&lock);
    }
    waitingWriters--; 
    activeWriters++; 
    lock.release();
}

// Done writing. A waiting write or read may proceed
void RWLock::doneWrite() {
    lock.acquire();
    activeWriters--;
    assert(activeWriters == 0);
    if (waitingWriters > 0) {
        writeGo.signal();
    } else {
        readGo.broadcast();
    }
    lock.release();
}

// Read waits if any active or waiting write ("writers preferred")
bool RWLock::readShouldWait() {
    return (activeWriters > 0 
        || waitingWriters > 0);
}

// Write waits for active read or write
bool
RWLock::writeShouldWait() {
    return (activeWriters > 0 || activeReaders > 0);
}

