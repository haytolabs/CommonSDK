/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef __THREAD__H__
#define __THREAD__H__

#include "mem.h"
#include "platform.h"
#include "public.h"

/*
================================================
CSystemMutex provides a C++ wrapper to the low level system mutex functions.  A mutex is an
object that can only be locked by one thread at a time.  It's used to prevent two threads
from accessing the same piece of pData simultaneously.
================================================
*/
class CSystemMutex
{
  public:
    CSystemMutex()
    {
        platform()->MutexCreate(m_Handle);
    }
    ~CSystemMutex()
    {
        platform()->MutexDestroy(m_Handle);
    }

    bool Lock(bool blocking = true)
    {
        return platform()->MutexLock(m_Handle, blocking);
    }
    void Unlock()
    {
        platform()->MutexUnlock(m_Handle);
    }

  private:
    mutexHandle_t m_Handle;

    CSystemMutex(const CSystemMutex &s)
    {
    }
    void operator=(const CSystemMutex &s)
    {
    }
};

/*
================================================
CScopedCriticalSection is a helper class that automagically locks a mutex when it's created
and unlocks it when it goes out of scope.
================================================
*/
class CScopedCriticalSection
{
  public:
    CScopedCriticalSection(CSystemMutex &m) : mutex(&m)
    {
        mutex->Lock();
    }
    ~CScopedCriticalSection()
    {
        mutex->Unlock();
    }

  private:
    CSystemMutex *mutex; // NOTE: making this a reference causes a TypeInfo crash
};

/*
================================================
CSystemSignal is a C++ wrapper for the low level system signal functions.  A signal is an object
that a thread can wait on for it to be raised.  It's used to indicate pData is available or that
a thread has reached a specific point.
================================================
*/
class CSystemSignal
{
  public:
    static const int WAIT_INFINITE = -1;

    CSystemSignal(bool manualReset = false)
    {
        platform()->SignalCreate(m_Handle, manualReset);
    }
    ~CSystemSignal()
    {
        platform()->SignalDestroy(m_Handle);
    }

    void Raise()
    {
        platform()->SignalRaise(m_Handle);
    }

    void Clear()
    {
        platform()->SignalClear(m_Handle);
    }

    // Wait returns true if the object is in a signalled state and
    // returns false if the wait timed out. Wait also clears the signalled
    // state when the signalled state is reached within the time out period.
    bool Wait(int iTimeout = WAIT_INFINITE)
    {
        return platform()->SignalWait(m_Handle, iTimeout);
    }

  private:
    signalHandle_t m_Handle;

    CSystemSignal(const CSystemSignal &s)
    {
    }
    void operator=(const CSystemSignal &s)
    {
    }
};

/*
================================================
CSysInterlockedInteger is a C++ wrapper for the low level system interlocked integer
routines to atomically increment or decrement an integer.
================================================
*/
class CSysInterlockedInteger
{
  public:
    CSysInterlockedInteger() : m_iValue(0)
    {
    }

    // atomically increments the integer and returns the new value
    int Increment()
    {
        return platform()->InterlockedIncrement(m_iValue);
    }

    // atomically decrements the integer and returns the new value
    int Decrement()
    {
        return platform()->InterlockedDecrement(m_iValue);
    }

    // atomically adds a value to the integer and returns the new value
    int Add(int v)
    {
        return platform()->InterlockedAdd(m_iValue, (interlockedInt_t)v);
    }

    // atomically subtracts a value from the integer and returns the new value
    int Sub(int v)
    {
        return platform()->InterlockedSub(m_iValue, (interlockedInt_t)v);
    }

    // returns the current value of the integer
    int GetValue() const
    {
        return m_iValue;
    }

    // sets a new value, Note: this operation is not atomic
    void SetValue(int v)
    {
        m_iValue = (interlockedInt_t)v;
    }

  private:
    interlockedInt_t m_iValue;
};

/*
================================================
CSysInterlockedPointer is a C++ wrapper around the low level system interlocked pointer
routine to atomically bSet a pointer while retrieving the previous value of the pointer.
================================================
*/
template <typename T> class CSysInterlockedPointer
{
  public:
    CSysInterlockedPointer() : ptr(NULL)
    {
    }

    // atomically sets the pointer and returns the previous pointer value
    T *Set(T *newPtr)
    {
        return (T *)platform()->InterlockedExchangePointer((void *&)ptr, newPtr);
    }

    // atomically sets the pointer to 'newPtr' only if the previous pointer is equal to 'comparePtr'
    // ptr = ( ptr == comparePtr ) ? newPtr : ptr
    T *CompareExchange(T *comparePtr, T *newPtr)
    {
        return (T *)platform()->InterlockedCompareExchangePointer((void *&)ptr, comparePtr, newPtr);
    }

    // returns the current value of the pointer
    T *Get() const
    {
        return ptr;
    }

  private:
    T *ptr;
};

/*
================================================
CSystemThread is an abstract base class, to be extended by classes implementing the
CSystemThread::Run() method.

    class nfMyThread : public CSystemThread {
    public:
        virtual int Run() {
            ## run thread code here
            return 0;
        }
        ## specify thread pData here
    };

    nfMyThread thread;
    thread.Start( "myThread" );

A worker thread is a thread that waits in place (without consuming CPU)
until work is available. A worker thread is implemented as normal, except that, instead of
calling the Start() method, the StartWorker() method is called to start the thread.
Note that the platform()->CreateThread function does not support the concept of worker threads.

    class nfMyWorkerThread : public CSystemThread {
    public:
        virtual int Run() {
            ## run thread code here
            return 0;
        }
        ## specify thread pData here
    };

    nfMyWorkerThread thread;
    thread.StartThread( "myWorkerThread" );

    ## main thread loop
    for ( ; ; ) {
        ## setup work for the thread here (by modifying class pData on the thread)
        thread.SignalWork();           // kick in the worker thread
        ## run other code in the main thread here (in parallel with the worker thread)
        thread.WaitForThread();        // wait for the worker thread to finish
        ## use results from worker thread here
    }

In the above example, the thread does not continuously run in parallel with the main Thread,
but only for a certain period of time in a very controlled manner. Work is bSet up for the
Thread and then the thread is signalled to process that work while the main thread continues.
After doing other work, the main thread can wait for the worker thread to finish, if it has not
finished already. When the worker thread is done, the main thread can safely use the results
from the worker thread.

Note that worker threads are useful on all platforms but they do not map to the SPUs on the PS3.
================================================
*/
class CSystemThread
{
  public:
    CSystemThread();
    virtual ~CSystemThread();

    const char *GetName() const
    {
        return pszName.c_str();
    }
    uintptr_t GetThreadHandle() const
    {
        return threadHandle;
    }
    bool IsRunning() const
    {
        return isRunning;
    }
    bool IsTerminating() const
    {
        return isTerminating;
    }

    //------------------------
    // Thread Start/Stop/Wait
    //------------------------

    bool StartThread(const char *pszName, core_t core, xthreadPriority priority = THREAD_NORMAL,
                     int stackSize = DEFAULT_THREAD_STACK_SIZE);

    bool StartWorkerThread(const char *pszName, core_t core, xthreadPriority priority = THREAD_NORMAL,
                           int stackSize = DEFAULT_THREAD_STACK_SIZE);

    void StopThread(bool wait = true);

    // This can be called from multiple other threads. However, in the case
    // of a worker thread, the work being "done" has little meaning if other
    // threads are continuously signalling more work.
    void WaitForThread();

    //------------------------
    // Worker Thread
    //------------------------

    // Signals the thread to notify work is available.
    // This can be called from multiple other threads.
    void SignalWork();

    // Returns true if the work is done without waiting.
    // This can be called from multiple other threads. However, the work
    // being "done" has little meaning if other threads are continuously
    // signalling more work.
    bool IsWorkDone();

  protected:
    // The routine that performs the work.
    virtual int Run();

  private:
    std::string pszName;
    uintptr_t threadHandle;
    bool isWorker;
    bool isRunning;
    volatile bool isTerminating;
    volatile bool moreWorkToDo;
    CSystemSignal signalWorkerDone;
    CSystemSignal signalMoreWorkToDo;
    CSystemMutex signalMutex;

    static int ThreadProc(CSystemThread *thread);

    CSystemThread(const CSystemThread &s)
    {
    }
    void operator=(const CSystemThread &s)
    {
    }
};

/*
================================================
CSysWorkerThreadGroup implements a group of worker threads that
typically crunch through a collection of similar tasks.

    class nfMyWorkerThread : public CSystemThread {
    public:
        virtual int Run() {
            ## run thread code here
            return 0;
        }
        ## specify thread pData here
    };

    CSysWorkerThreadGroup<nfMyWorkerThread> workers( "myWorkers", 4 );
    for ( ; ; ) {
        for ( int i = 0; i < workers.GetNumThreads(); i++ ) {
            ## workers.GetThread( i )-> // setup work for this thread
        }
        workers.SignalWorkAndWait();
        ## use results from the worker threads here
    }

The concept of worker thread Groups is probably most useful for tools and compilers.
For instance, the AAS Compiler is using a worker thread group. Although worker threads
will work well on the PC, Mac and the 360, they do not directly map to the PS3,
in that the worker threads won't automatically run on the SPUs.
================================================
*/
template <class threadType> class CSysWorkerThreadGroup
{
  public:
    CSysWorkerThreadGroup(const char *pszName, int numThreads, xthreadPriority priority = THREAD_NORMAL,
                          int stackSize = DEFAULT_THREAD_STACK_SIZE);

    virtual ~CSysWorkerThreadGroup();

    int GetNumThreads() const
    {
        return m_ThreadList.size();
    }
    threadType &GetThread(int i)
    {
        return *m_ThreadList[i];
    }

    void SignalWorkAndWait();

  private:
    std::vector<threadType *> m_ThreadList;
    bool m_bRunOneThread;   // use the signalling thread as one of the threads
    bool m_bSingleThreaded; // bSet to true for debugging
};

/*
================================================
CSystemThreadSynchronizer, allows a group of threads to
synchronize with each other half-way through execution.

    CSystemThreadSynchronizer sync;

    class nfMyWorkerThread : public CSystemThread {
    public:
        virtual int Run() {
            ## perform first part of the work here
            sync.Synchronize( nThread );	// synchronize all threads
            ## perform second part of the work here
            return 0;
        }
        ## specify thread pData here
        unsigned int nThread;
    };

    CSysWorkerThreadGroup<nfMyWorkerThread> workers( "myWorkers", 4 );
    for ( int i = 0; i < workers.GetNumThreads(); i++ ) {
        workers.GetThread( i )->nThread = i;
    }

    for ( ; ; ) {
        for ( int i = 0; i < workers.GetNumThreads(); i++ ) {
            ## workers.GetThread( i )-> // setup work for this thread
        }
        workers.SignalWorkAndWait();
        ## use results from the worker threads here
    }

================================================
*/
class CSystemThreadSynchronizer
{
  public:
    static const int WAIT_INFINITE = -1;

    void SetNumThreads(unsigned int num);
    void Signal(unsigned int nThread);
    bool Synchronize(unsigned int nThread, int iTimeout = WAIT_INFINITE);

  private:
    std::vector<CSystemSignal *> m_Signals;
    CSysInterlockedInteger m_nBusy;
};

#endif //!__THREAD__H__