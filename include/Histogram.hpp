#ifndef HISTOGRAM_HPP_INCLUDED
#define HISTOGRAM_HPP_INCLUDED


#include <ostream>

#include "Interval.hpp"


template <typename T>
class Histogram
{
    public:
        Histogram();

        void clear();
        void set(const Interval<T>& interval, float cumulatedFreq);

        T getOutValue(unsigned int index);
        T getNearest(float freq);
        T getMean(float freq);

    private:
        std::map<Interval<T>,float> intervals;
        T min, max;
        bool minSet, maxSet;
};

template <typename T>
Histogram<T>::Histogram() :
    min(0),
    max(0),
    minSet(false),
    maxSet(false)
{}

template <typename T>
void Histogram<T>::clear()
{
    intervals.clear();
    minSet = false;
    maxSet = false;
}

template <typename T>
void Histogram<T>::set(const Interval<T>& interval, float cumulatedFreq)
{
    if(interval.leftInfinite())
    {
        if(!minSet||interval.right()<min)
            min = interval.right();
        else if(interval.left()<min)
            min = interval.left();
        minSet = true;
    }
    else if(!minSet||interval.left()<min)
    {
        min = interval.left();
        minSet = true;
    }
    else if(!maxSet||interval.right()>max)
    {
        max = interval.right();
        maxSet = true;
    }
    intervals[interval] = cumulatedFreq;
}


template <typename T>
T Histogram<T>::getOutValue(unsigned int index)
{
    if(intervals.size()>1)
    {
        auto it = intervals.begin();
        T t1 = (it->first)();
        it++;
        T t2 = (it->first)();
        return t1-(index+1)*(t2-t1);
    }
    else
        return ((intervals.begin())->first).left()-(index+1);
}

template <typename T>
T Histogram<T>::getNearest(float freq)
{
    auto it = intervals.begin();
    if(it->second>freq)
        return (it->first)();
    T cur = (it->first)();
    while(it!=intervals.end()&&freq>it->second)
    {
        it++;
        if(it!=intervals.end()&&freq>it->second)
            cur = (it->first)();
    }
    return cur;
}

template <typename T>
T Histogram<T>::getMean(float freq)
{
    auto it = intervals.begin();
    if(it->second>freq)
        return (it->first)();
    T cur = (it->first)();
    T next = cur;
    float curFreq = it->second, nextFreq = it->second;
    while(it!=intervals.end()&&freq>it->second)
    {
        it++;
        if(it!=intervals.end())
        {
            nextFreq = it->second;
            next = (it->first)();
            if(freq>it->second)
            {
                curFreq = it->second;
                cur = (it->first)();
            }
        }
    }
    if(fabs(nextFreq-curFreq)<0.00001)
        return (cur+next)/2;
    else
        return (cur+(next-cur)*(freq-curFreq)/(nextFreq-curFreq));
}

template <typename T>
std::ostream operator << (const Histogram<T>& h)
{
}


#endif
