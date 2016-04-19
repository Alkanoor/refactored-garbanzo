#include "../include/ThreadedOperations.hpp"


unsigned int ThreadedOperations::currentThread = 0;
std::shared_ptr<std::vector<std::vector<unsigned char> > > ThreadedOperations::dataLearnSplit;
std::shared_ptr<std::vector<std::vector<std::vector<unsigned char> > > > ThreadedOperations::finalDataset;
std::unordered_map<std::string,unsigned int> ThreadedOperations::columnNames;
std::unordered_map<std::string,std::unordered_map<std::string,unsigned int> > ThreadedOperations::counter;
std::vector<ThreadedOperations> ThreadedOperations::threads;
unsigned int ThreadedOperations::N_threads = 0;
unsigned int ThreadedOperations::N_features = 0;

ThreadedOperations::ThreadedOperations():
    idThread(currentThread)
{
    currentThread++;
}

void ThreadedOperations::runThread1()
{
    for(unsigned int i=idThread;i<N_features;i+=threads.size())
        loadInVec((*dataLearnSplit)[i],(*finalDataset)[i],';');
}

void ThreadedOperations::runThread2()
{}


void ThreadedOperations::setDataLearn(unsigned int N, std::shared_ptr<std::vector<std::vector<unsigned char> > > data, std::shared_ptr<std::vector<std::vector<std::vector<unsigned char> > > > finalData)
{
    dataLearnSplit = data;
    finalDataset = finalData;
    N_features = N;
}

void ThreadedOperations::setNumberThread(unsigned int n_threads)
{
    N_threads = n_threads;
    threads.resize(N_threads);
}

void ThreadedOperations::run1()
{
    for(unsigned int i=0;i<N_threads;i++)
        threads[i].thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&ThreadedOperations::runThread1,threads[i])));
}

void ThreadedOperations::run2()
{
    for(unsigned int i=0;i<N_threads;i++)
        threads[i].thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&ThreadedOperations::runThread2,threads[i])));
}

void ThreadedOperations::join()
{
    for(unsigned int i=0;i<N_threads;i++)
        if(threads[i].thread)
            threads[i].thread->join();
}
