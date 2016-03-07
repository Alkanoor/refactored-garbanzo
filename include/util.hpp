#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED


#include <iostream>
#include <fstream>
#include <vector>


bool read(const std::string& path, std::vector<unsigned char>& ret);
unsigned int loadInVec(const std::vector<unsigned char>& in, std::vector<std::vector<unsigned char> >& ret, unsigned char separator = '\n');
unsigned int loadInString(const std::vector<unsigned char>& in, std::vector<std::string>& ret, unsigned char separator = '\n');

template <typename T>
void print(const std::vector<T>& v)
{
    for(unsigned int i=0;i<v.size();i++)
        std::cout<<v[i];
    std::cout<<std::endl;
}

template <typename T>
void toString(const std::vector<T>& v, std::string& ret)
{
    ret.resize(v.size());
    for(unsigned int i=0;i<v.size();i++)
        ret[i] = (char)(v[i]);
}

template <typename T>
void apply(const std::map<std::string,unsigned int>& correspondancesColumns, std::map<std::string, std::function<T(const std::vector<unsigned char>&)> >& toApply, std::vector<std::vector<std::vector<unsigned char> > >& toBeModified)
{

}


#endif
