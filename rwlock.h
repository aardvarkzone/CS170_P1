#ifndef __RWLOCK_H__
#define __RWLOCK_H__
#include<semaphore.h>
#include <pthread.h>

class RWLock{
private:
    //Your solution to add more private fields

    // Synchronization variables
    // Lock lock;
    // CV readGo;
    // CV writeGo;

    pthread_mutex_t lock;      // Mutex lock for the readers-writer lock
    pthread_cond_t readGo;    // Condition variable for readers
    pthread_cond_t writeGo;   // Condition variable for writers


    // State Variables
    int activeReaders;
    int activeWriters;
    int waitingReaders;
    int waitingWriters;
    
public:
    RWLock();
    ~RWLock();
    //Reader
    void startRead();
    void doneRead();
    // Writer
    void startWrite();
    void  doneWrite();

    // Helper
    bool readShouldWait();
    bool writeShouldWait();
};


#endif
