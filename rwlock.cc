#include<stdio.h>
#include <pthread.h>
#include <assert.h>

#include <iostream> 


#include "rwlock.h"

//Your solution to implement each of the following methods
RWLock::RWLock() {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&readGo, NULL);
    pthread_cond_init(&writeGo, NULL);

    activeReaders = 0;
    activeWriters = 0;
    waitingReaders = 0;
    waitingWriters = 0;
}

RWLock::~RWLock() {
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&readGo);
    pthread_cond_destroy(&writeGo);
}

// Wait until no active or waiting writes, then proceed
void RWLock::startRead() {
    // lock.acquire();
    pthread_mutex_lock(&lock);
    while (readShouldWait()) {
        // readGo.Wait(&lock);
        pthread_cond_wait(&readGo, &lock);
    }
    waitingReaders--;
    activeReaders++;
    // lock.release();
    pthread_mutex_unlock(&lock); 
}

// Done reading. If no other active reads, a write may proceed
void RWLock::doneRead() {
    // lock.acquire();
    pthread_mutex_lock(&lock);
    activeReaders--;
    if (activeReaders == 0
        && waitingWriters > 0) {
        // writeGo.signal();
        pthread_cond_signal(&writeGo);
    }
    // lock.release();
    pthread_mutex_unlock(&lock); 
}

// Wait until no active read or write then proceed
void RWLock::startWrite() {
    // lock.acquire();
    pthread_mutex_lock(&lock);
    waitingWriters++; 
    while (writeShouldWait()) {
        // writeGo.Wait(&lock);
        pthread_cond_wait(&writeGo, &lock);
    }
    waitingWriters--; 
    activeWriters++; 
    // lock.releaase();
    pthread_mutex_unlock(&lock); 
}

// Done writing. A waiting write or read may proceed
void RWLock::doneWrite() {
    // lock.acquire();
    pthread_mutex_lock(&lock);
    activeWriters--;
    assert(activeWriters == 0);
    if (waitingWriters > 0) {
        // writeGo.signal();
        pthread_cond_signal(&writeGo);
    } else {
        // readGo.broadcast();
        pthread_cond_broadcast(&readGo);
    }
    // lock.release();
    pthread_mutex_unlock(&lock); 
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

