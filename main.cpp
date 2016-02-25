#include <iostream>

#include "include/ThreadedOperations.hpp"


int main()
{
    std::vector<unsigned char> dataLearn;
    if(!read("../train.csv",dataLearn))
    {
        std::cout<<"Error loading data test"<<std::endl;
        return -1;
    }

    /*std::shared_ptr<std::vector<std::vector<unsigned char> > > dataLearnSplit(new std::vector<std::vector<unsigned char> >());
    unsigned int N_features = loadInVec(dataLearn,*dataLearnSplit);
    std::shared_ptr<std::vector<std::vector<std::vector<unsigned char> > > > finalDataset(new std::vector<std::vector<std::vector<unsigned char> > >(N_features));

    ThreadedOperations::setNumberThread(4);
    ThreadedOperations::setDataLearn(N_features,dataLearnSplit,finalDataset);
    ThreadedOperations::run1();
    ThreadedOperations::join();*/

    std::vector<std::vector<unsigned char> > dataLearnSplit;
    unsigned int N_features = loadInVec(dataLearn,dataLearnSplit);
    std::vector<std::vector<std::vector<unsigned char> > > finalDataset(N_features);

    for(unsigned int i=0;i<N_features;i++)
        loadInVec(dataLearnSplit[i],finalDataset[i],';');

    std::unordered_map<std::string,unsigned int> columnNames;
    std::unordered_map<std::string,std::unordered_map<std::string,unsigned int> > counter;
    std::string tmp1,tmp2;
    for(unsigned int i=0;i<(finalDataset)[0].size();i++)
    {
        toString((finalDataset)[0][i],tmp1);
        columnNames[tmp1] = i;

        for(unsigned int j=1;j<N_features;j++)
        {
            toString((finalDataset)[j][i],tmp2);
            if(counter[tmp1].count(tmp2))
                counter[tmp1][tmp2]++;
            else
                counter[tmp1][tmp2] = 1;
        }
    }

    std::ofstream ofs("log.txt",std::ios::out|std::ios::trunc);
    for(std::unordered_map<std::string,std::unordered_map<std::string,unsigned int> >::iterator it=counter.begin();it!=counter.end();it++)
    {
        std::cout<<it->first<<std::endl;
        ofs<<it->first<<std::endl;
        if(it->second.size()<10)
            for(std::unordered_map<std::string,unsigned int>::iterator itbis=it->second.begin();itbis!=it->second.end();itbis++)
                std::cout<<itbis->first<<" => "<<itbis->second<<std::endl;
        else
            std::cout<<"=> "<<it->second.size()<<std::endl;
        unsigned int k=0;
        unsigned int sum=0;
        for(std::unordered_map<std::string,unsigned int>::iterator itbis=it->second.begin();itbis!=it->second.end()&&k<150;itbis++,k++)
        {
            sum += itbis->second;
            ofs<<itbis->first<<" => "<<itbis->second<<std::endl;
        }
        std::cout<<"gives "<<sum<<std::endl<<std::endl;
        ofs<<"gives "<<sum<<std::endl<<std::endl;
    }
    std::cout<<(finalDataset)[0].size()<<std::endl;

    return 0;
}
