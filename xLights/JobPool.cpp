//
//  JobPool.cpp
//  xLights


#include <string>
#include <mutex>
#include <condition_variable>
#include <sstream> 
#include <iomanip>

#include "JobPool.h"

#ifdef LINUX
    #include <X11/Xlib.h>
#endif

#include <wx/thread.h>
#include <log4cpp/Category.hh>

class JobPoolWorker : public wxThread
{
    std::mutex *lock;
    std::condition_variable *signal;
    volatile int &idleThreads;
    volatile int &numThreads;
    std::deque<Job*> *queue;

    Job *currentJob;

public:
    JobPoolWorker(std::mutex *l,
                  std::condition_variable *signal,
                  std::deque<Job*> *queue,
                  volatile int &idleThreadPtr,
                  volatile int &numThreadsPtr);
    virtual ~JobPoolWorker();

    void Stop();
    void Start();
    void* Entry();

    Job *GetJob();

    void ProcessJob(Job *job);
    std::string GetStatus();
};

JobPoolWorker::JobPoolWorker(std::mutex *l, std::condition_variable *s, std::deque<Job*> *queue,
                             volatile int &idleThreadPtr, volatile int &numThreadsPtr)
: wxThread(wxTHREAD_JOINABLE), lock(l) ,signal(s), queue(queue), idleThreads(idleThreadPtr), numThreads(numThreadsPtr), currentJob(nullptr)
{
}

JobPoolWorker::~JobPoolWorker()
{
}

std::string JobPoolWorker::GetStatus()
{
    std::stringstream ret;
    ret << "Thread: ";
    
    
    ret << std::showbase // show the 0x prefix
        << std::internal // fill between the prefix and the number
        << std::setfill('0') << std::setw(10)
        << std::hex << GetId()
        << "    ";
    if (currentJob != nullptr) {
        ret << currentJob->GetStatus();
    } else {
        ret << "<idle>";
    }
    return ret.str();
}
void JobPoolWorker::Stop()
{
    if ( IsAlive() )
        Delete();

    while ( IsAlive() ) {
        wxThread::Sleep( 5 );
    }
}

void JobPoolWorker::Start()
{
    Create();
    Run();
}

Job *JobPoolWorker::GetJob()
{
    std::unique_lock<std::mutex> mutLock(*lock);
    Job *req(NULL);
    if (queue->empty()) {
        idleThreads++;
        signal->wait_for(mutLock, std::chrono::milliseconds(100));
        idleThreads--;
    }
    if ( !queue->empty() ) {
        req = queue->front();
        queue->pop_front();
    }
    return req;
}

void* JobPoolWorker::Entry()
{
#ifdef LINUX
    XInitThreads();
#endif
    while ( true ) {
        // Did we get a request to terminate?
        if (TestDestroy())
            break;

        Job *job = GetJob();
        if (job) {
            // Call user's implementation for processing request
            ProcessJob(job);
            if (job->DeleteWhenComplete()) {
                delete job;
            }
            job = NULL;
        } else {
            std::unique_lock<std::mutex> mutLock(*lock);
            if (idleThreads > 5) {
                break;
            }
        }
    }
    std::unique_lock<std::mutex> mutLock(*lock);
    numThreads--;
    return NULL;
}

void JobPoolWorker::ProcessJob(Job *job)
{
    log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_jobpool"));
    if (job) {
		logger_base.debug("Starting job on background thread.");
		currentJob = job;
        job->Process();
        currentJob = nullptr;
		logger_base.debug("Job on background thread done.");
	}
}

JobPool::JobPool() : lock(), signal(), queue()
{
    idleThreads = 0;
    numThreads = 0;
    maxNumThreads = 8;
}

JobPool::~JobPool()
{
    if ( !queue.empty() ) {
        std::deque<Job*>::iterator iter = queue.begin();
        for (; iter != queue.end(); iter++) {
            delete (*iter);
        }
        queue.clear();
    }
    Stop();
}

void JobPool::PushJob(Job *job)
{
	std::unique_lock<std::mutex> locker(lock);
    if (idleThreads == 0 && numThreads < maxNumThreads) {
        numThreads++;

        JobPoolWorker *worker = new JobPoolWorker(&lock, &signal, &queue, idleThreads, numThreads);
        worker->Start();
		threads.push_back(worker);
    }
    queue.push_back(job);
    signal.notify_all();
}

void JobPool::Start(size_t poolSize)
{
    maxNumThreads = poolSize > 250 ? 250 : poolSize;
    idleThreads = 0;
    numThreads = 0;
}

void JobPool::Stop()
{
    for(size_t i=0; i<threads.size(); i++){
        JobPoolWorker *worker = threads.at(i);
        worker->Stop();
        delete worker;
    }
    threads.clear();
}

std::string JobPool::GetThreadStatus() {
    std::stringstream ret;
    ret << "\n";
    for(size_t i=0; i<threads.size(); i++){
        JobPoolWorker *worker = threads.at(i);
        ret << worker->GetStatus();
        ret << "\n\n";
    }
    return ret.str();
}

