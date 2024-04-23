#ifndef __RWLOCK_H__
#define __RWLOCK_H__
#include<semaphore.h>

class RWLock{
private:
    //Your solution to add more private fields

    // Synchronization variables
    Lock lock;
    CV readGo;
    CV writeGo;

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
