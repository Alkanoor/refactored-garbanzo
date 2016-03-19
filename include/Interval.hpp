#ifndef INTERVAL_HPP_INCLUDED
#define INTERVAL_HPP_INCLUDED


#include <set>


template <typename T>
class Interval
{
    public:
        Interval(T val);
        Interval(const std::string& begin_, T end_);
        Interval(T begin_, const std::string& end_);

        bool operator == (const Interval<T>& other) const;
        bool operator <= (const Interval<T>& other) const;
        bool operator < (const Interval<T>& other) const;
        T operator() () const;

        void setLeft(T val);
        void setRight(T val);
        void setLabel(const std::string& label);

        bool leftInfinite() const;
        bool rightInfinite() const;
        T left() const;
        T right() const;
        const std::set<std::string>& getLabels() const;

        static void setPrecision(int precision);

    private:
        static int precisionBetween0And1;
        T value;
        int begin, end;
        bool beginInfinite, endInfinite;

        std::set<std::string> labels;
};

template <typename T>
int Interval<T>::precisionBetween0And1 = 100;

template <typename T>
Interval<T>::Interval(T val) :
    beginInfinite(false),
    endInfinite(false)
{
    value = val;
    begin = (int)(val*precisionBetween0And1);
    end = begin+1;
}

template <typename T>
Interval<T>::Interval(const std::string& begin_, T end_) :
    beginInfinite(true),
    endInfinite(false)
{
    value = end_;
    begin = (int)(value*precisionBetween0And1);
    end = begin+1;
}

template <typename T>
Interval<T>::Interval(T begin_, const std::string& end_) :
    beginInfinite(false),
    endInfinite(true)
{
    value = begin_;
    begin = (int)(value*precisionBetween0And1);
    end = begin+1;
}

template <typename T>
bool Interval<T>::operator == (const Interval<T>& other) const
{
    return begin==other.begin;
}

template <typename T>
bool Interval<T>::operator <= (const Interval<T>& other) const
{
    if(beginInfinite&&!other.beginInfinite)
        return true;
    else if(!beginInfinite&&other.beginInfinite)
        return false;
    else
        return begin<=other.begin;
}

template <typename T>
bool Interval<T>::operator < (const Interval<T>& other) const
{
    if(beginInfinite&&!other.beginInfinite)
        return true;
    else if(!beginInfinite&&other.beginInfinite)
        return false;
    else
        return begin<other.begin;
}

template <typename T>
T Interval<T>::operator() () const
{
    return value;
}

template <typename T>
void Interval<T>::setLeft(T val)
{begin = (int)(val*precisionBetween0And1);}

template <typename T>
void Interval<T>::setRight(T val)
{end = (int)(val*precisionBetween0And1)+1;}

template <typename T>
void Interval<T>::setLabel(const std::string& label)
{labels.insert(label);}

template <typename T>
bool Interval<T>::leftInfinite() const
{return beginInfinite;}

template <typename T>
bool Interval<T>::rightInfinite() const
{return endInfinite;}

template <typename T>
T Interval<T>::left() const
{return (T)begin/(T)precisionBetween0And1;}

template <typename T>
T Interval<T>::right() const
{return (T)end/(T)precisionBetween0And1;}

template <typename T>
const std::set<std::string>& Interval<T>::getLabels() const
{return labels;}

template <typename T>
void Interval<T>::setPrecision(int precision)
{
    precisionBetween0And1 = precision;
}


#endif
