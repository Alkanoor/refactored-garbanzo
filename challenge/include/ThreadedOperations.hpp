#ifndef THREADEDOPERATIONS_HPP
#define THREADEDOPERATIONS_HPP


#include <unordered_map>
#include <thread>
#include <vector>
#include <mutex>

#include "util.hpp"


class ThreadedOperations
{
    public:
        ThreadedOperations();

        void runThread1();
        void runThread2();

        static void setDataLearn(unsigned int N_features, std::shared_ptr<std::vector<std::vector<unsigned char> > > data, std::shared_ptr<std::vector<std::vector<std::vector<unsigned char> > > > finalData);
        static void setNumberThread(unsigned int n_threads);
        static void run1();
        static void run2();
        static void join();

    private:
        unsigned int idThread;
        static unsigned int currentThread;
        std::shared_ptr<std::thread> thread;

        static std::shared_ptr<std::vector<std::vector<unsigned char> > > dataLearnSplit;
        static std::shared_ptr<std::vector<std::vector<std::vector<unsigned char> > > > finalDataset;
        static std::unordered_map<std::string,unsigned int> columnNames;
        static std::unordered_map<std::string,std::unordered_map<std::string,unsigned int> > counter;
        static std::vector<ThreadedOperations> threads;
        static unsigned int N_threads;
        static unsigned int N_features;
};


#endif
