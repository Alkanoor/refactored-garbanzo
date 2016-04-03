#ifndef CONVERSIONFUNCTION_HPP_INCLUDED
#define CONVERSIONFUNCTION_HPP_INCLUDED


#include <functional>
#include <sstream>
#include <string>
#include <set>


/**On utilise manuellement, pour chaque colonne, la fonction la plus adaptée pour la conversion string/float
 On a le choix entre plusieurs fonctions :
    1- La fonction de conversion string->float classique, adaptée pour les paramètres continus ou supposés l'être
    2- La fonction de conversion string->float avec remplacement de valeur par espérance quand valeur "inconnue" (non renseignée)
    3- La fonction de conversion date-mois-année(string)->float
    4- La fonction d'écrasement basique : string->float avec float une classe aléatoire fixée pour chaque label
    5- La fonction d'écrasement basée sur la distance de hamming : string->float avec une notion de proximité**/

template <typename T>
class ConversionFunction
{
    public:
        ConversionFunction() {}
        ConversionFunction(int type);

        T convertBasic(std::string s);
        T convertWithMean(std::string s);
        T convertDate(std::string s);
        T convertClassBasic(std::string s);
        T convertClassHamming(std::string s);

        void setUnknown(const std::set<std::string>& unknown);

        const std::function<T(std::string)>& conversionFunction() const;
        const std::map<std::string,unsigned int>& getCounter() const;

        void reset();
        void resetType(int type);

    private:
        int t;
        int type;
        T n;
        T mean;
        T lastValue;
        std::set<std::string> unknown;
        std::function<T(std::string)> func;
        std::map<std::string,unsigned int> counter;
        std::map<std::string,unsigned int> index;
};


template <typename T>
ConversionFunction<T>::ConversionFunction(int type) :
    n(0),
    mean(0),
    lastValue(0)
{
    resetType(type);
    unknown.insert("");
}

template <typename T>
T ConversionFunction<T>::convertBasic(std::string s)
{
    n++;
    if(!unknown.count(s))
    {
        T tmp;
        std::istringstream iss(s);
        if(iss>>tmp)
            lastValue = tmp;
    }

    mean = (mean*n+lastValue)/(n+1);
    return lastValue;
}

template <typename T>
T ConversionFunction<T>::convertWithMean(std::string s)
{
    n++;
    if(!unknown.count(s))
    {
        T tmp;
        std::istringstream iss(s);
        if(iss>>tmp)
            lastValue = tmp;
        else
            lastValue = mean;
    }
    else
        lastValue = mean;

    mean = (mean*n+lastValue)/(n+1);
    return lastValue;
}

template <typename T>
T ConversionFunction<T>::convertDate(std::string s)
{
    if(s.size()!=7)
        return 0;
    else
    {
        T month=0, year=0;
        std::string tmp;
        tmp.resize(2);
        tmp[0] = s[0];
        tmp[1] = s[1];
        month = (tmp[0]-'0')*10+(tmp[1]-'0');
        tmp.resize(4);
        for(unsigned int i=0;i<4;i++)
            tmp[i] = s[i+3];
        year = (tmp[0]-'0')*1000+(tmp[1]-'0')*100+(tmp[2]-'0')*10+(tmp[3]-'0');
        return (year-1950)*12+month;
    }
}

template <typename T>
T ConversionFunction<T>::convertClassBasic(std::string s)
{
    if(index.count(s))
    {
        counter[s]++;
        return index[s];
    }
    else
    {
        index[s] = index.size();
        counter[s] = 1;
        return index[s];
    }
}

template <typename T>
T ConversionFunction<T>::convertClassHamming(std::string s)
{
    if(index.count(s))
    {
        counter[s]++;
        return index[s];
    }
    else
    {
        index[s] = index.size();
        counter[s] = 1;
        return index[s];
    }
}

template <typename T>
void ConversionFunction<T>::setUnknown(const std::set<std::string>& unknown)
{this->unknown = unknown;}

template <typename T>
const std::function<T(std::string)>& ConversionFunction<T>::conversionFunction() const
{return func;}

template <typename T>
const std::map<std::string,unsigned int>& ConversionFunction<T>::getCounter() const
{return counter;}

template <typename T>
void ConversionFunction<T>::reset()
{
    n = 0;
    mean = 0;
    lastValue = 0;
    index.clear();
    counter.clear();
}

template <typename T>
void ConversionFunction<T>::resetType(int type)
{
    this->type = type;
    switch(type)
    {
        case 0:
            func = std::bind(ConversionFunction<T>::convertBasic,*this,std::placeholders::_1);
            break;
        case 1:
            func = std::bind(ConversionFunction<T>::convertWithMean,*this,std::placeholders::_1);
            break;
        case 2:
            func = std::bind(ConversionFunction<T>::convertDate,*this,std::placeholders::_1);
            break;
        case 3:
            func = std::bind(ConversionFunction<T>::convertClassBasic,*this,std::placeholders::_1);
            break;
        case 4:
            func = std::bind(ConversionFunction<T>::convertClassHamming,*this,std::placeholders::_1);
            break;
        default:
            func = std::bind(ConversionFunction<T>::convertBasic,*this,std::placeholders::_1);
            break;
    }
}


#endif
