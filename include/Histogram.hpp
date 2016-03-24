#ifndef HISTOGRAM_HPP_INCLUDED
#define HISTOGRAM_HPP_INCLUDED


#include <ostream>
#include <cmath>

#include "Interval.hpp"


template <typename T>
class Histogram
{
    public:
        Histogram();

        void clear();
        void set(const Interval<T>& interval, float cumulatedFreq);
        void set(const Interval<T>& interval, float cumulatedFreq, const std::set<std::string>& labels);

        T getOutValue(unsigned int index);
        T getNearest(float freq);
        T getMean(float freq);

        void setLastLabels(const std::set<std::string>& labels);

        template <typename U>
        friend std::ostream& operator << (std::ostream& cur, const Histogram<U>& h);

    private:
        std::map<Interval<T>,std::pair<float,std::set<std::string> > > intervals;
        T min, max;
        bool minSet, maxSet;
        std::set<std::string> lastLabels;

        static int width, height;
};

template <typename T>
int Histogram<T>::width = 20;
template <typename T>
int Histogram<T>::height = 10;

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
    intervals[interval].first = cumulatedFreq;
}

template <typename T>
void Histogram<T>::set(const Interval<T>& interval, float cumulatedFreq, const std::set<std::string>& labels)
{
    set(interval,cumulatedFreq);
    intervals[interval].second = labels;
}


template <typename T>
T Histogram<T>::getOutValue(unsigned int index)
{
    if(intervals.size()>1)
    {
        auto it = intervals.begin();
        T t1 = it->first.val();
        it++;
        T t2 = it->first.val();
        return t1-(index+1)*(t2-t1);
    }
    else
        return ((intervals.begin())->first).left()-(index+1);
}

template <typename T>
T Histogram<T>::getNearest(float freq)
{
    auto it = intervals.begin();
    if(it->second.first>freq)
        return it->first.val();
    T cur = it->first.val();
    while(it!=intervals.end()&&freq>it->second.first)
    {
        it++;
        if(it!=intervals.end()&&freq>it->second.first)
            cur = it->first.val();
    }
    return cur;
}

template <typename T>
T Histogram<T>::getMean(float freq)
{
    auto it = intervals.begin();
    if(it->second.first>freq)
        return it->first.val();
    T cur = it->first.val();
    T next = cur;
    float curFreq = it->second.first, nextFreq = it->second.first;
    while(it!=intervals.end()&&freq>it->second.first)
    {
        it++;
        if(it!=intervals.end())
        {
            nextFreq = it->second.first;
            next = it->first.val();
            if(freq>it->second.first)
            {
                curFreq = it->second.first;
                cur = it->first.val();
            }
        }
    }
    if(fabs(nextFreq-curFreq)<0.00001)
        return (cur+next)/2;
    else
        return (cur+(next-cur)*(freq-curFreq)/(nextFreq-curFreq));
}

template <typename T>
void Histogram<T>::setLastLabels(const std::set<std::string>& labels)
{lastLabels = labels;}

template <typename T>
std::ostream& operator << (std::ostream& cur, const Histogram<T>& h)
{
    unsigned int c = 0;
    auto it = h.intervals.begin();
    for(; it!=h.intervals.end() && c+1<h.intervals.size(); )
    {
        cur<<"[ "<<it->first.left()<<" ; ";
        float saved = it->second.first;
        it++;
        cur<<it->first.left()<<" ] :";
        cur<<saved<<" with labels ";
        for(auto it2=it->second.second.begin(); it2!=it->second.second.end(); it2++)
            cur<<(*it2)<<" ";
        cur<<std::endl;
        c++;
    }
    cur<<"[ "<<it->first.left()<<" ; +inf ] :";
    cur<<it->second.first<<" with labels ";
    for(auto it2=h.lastLabels.begin(); it2!=h.lastLabels.end(); it2++)
        cur<<(*it2)<<" ";
    cur<<std::endl;
    cur<<std::endl;
    return cur;
}


#endif
