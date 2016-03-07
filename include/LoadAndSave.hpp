#ifndef LOADANDSAVE_HPP_INCLUDED
#define LOADANDSAVE_HPP_INCLUDED


#include "util.hpp"


enum BEHAVIOUR{Other_class, Nearest_float, Mean_float};

template <typename T>
class LoadAndSave
{
    public:
        void loadRaw(const std::string& path, char separator = ';');
        void loadFloat(const std::string& path, unsigned int numberColumns);
        void loadTitles(const std::string& path);
        void saveRaw(const std::string& path);
        void saveFloat(const std::string& path);
        void saveTitles(const std::string& path);

        void createHoles(const std::map<std::string,float>& probaHolesInColumn);
        void setConversionArray(const std::map<std::string,std::function<float(std::string)> >& howToConvert);

        void chooseColumns(const std::vector<std::string>& namesToKeep);
        void updateChosenColumns();
        const std::map<std::string,std::map<std::string,unsigned int> >& countAbnormal(const std::map<std::string, std::map<std::string, std::string> >& labelsOfAbnormalForCols);
        void replaceAbnormalInFloat(const std::map<std::string,std::map<std::string,BEHAVIOUR> >& behaviour);

        void saveDensity(const std::vector<std::string>& columns, const std::string& path);
        template <size_t N>
        matrix<T,N,1> convertToDlibMatrix(const std::array<std::string,N>& columns);

        unsigned int getNumberRows() const;
        unsigned int getNumberColumnsRaw() const;
        unsigned int getNumberColumnsFloat() const;

    private:
        std::map<std::string,std::map<std::string,unsigned int> > abnormal;
        std::vector<std::vector<std::string> > rawDataColumnsOrdered;
        std::vector<std::vector<T> > transformedDataColumnsOrdered;
        std::map<std::string,unsigned int> columnNamesCorrespondances;
        std::vector<std::string> columnNames;
};


template <typename T>
void LoadAndSave<T>::loadRaw(const std::string& path, char separator)
{
    std::vector<unsigned char> dataLearn;
    if(!read(path,dataLearn))
    {
        std::cout<<"Error loading data test"<<std::endl;
        exit(-1);
    }

    std::vector<std::vector<unsigned char> > dataLearnSplit;
    unsigned int N_features = loadInVec(dataLearn,dataLearnSplit);

    std::vector<std::string> line;
    unsigned int N_parameters = loadInString(dataLearnSplit[0],line,separator);
    rawDataColumnsOrdered.resize(N_parameters);

    for(unsigned int j=0;j<N_parameters;j++)
        rawDataColumnsOrdered[j].resize(N_features);

    for(unsigned int i=0;i<N_features;i++)
    {
        loadInString(dataLearnSplit[i],line,separator);
        for(unsigned int j=0;j<N_parameters;j++)
            rawDataColumnsOrdered[j][i] = line[j];
    }

    ///On utilise la première ligne pour déterminer les noms des colonnes, on utilise les suivantes pour se faire une idée plus précise de la distribution des valeurs diverses que peuvent prendre les items d'une colonne
    for(unsigned int i=0;i<N_parameters;i++)
    {
        columnNames.push_back(rawDataColumnsOrdered[0][i]);
        columnNamesCorrespondances[rawDataColumnsOrdered[0][i]] = i;
    }
}

template <typename T>
void LoadAndSave<T>::loadFloat(const std::string& path, unsigned int numberColumns)
{
    transformedDataColumnsOrdered.resize(numberColumns);
    float tmp;
    std::ifstream ifs(path.c_str());
    unsigned int i=0, j=0;
    while(ifs>>tmp)
    {
        transformedDataColumnsOrdered[i][j] = tmp;
        i++;
        if(i>=numberColumns)
        {
            i = 0;
            j++;
            for(unsigned int k=0;k<numberColumns;k++)
                transformedDataColumnsOrdered[k].push_back(0);
        }
    }
}

template <typename T>
void LoadAndSave<T>::loadTitles(const std::string& path)
{
    std::ifstream ifs(path.c_str());
    std::string tmp;
    unsigned int i=0;
    while(ifs>>tmp)
    {
        columnNames.push_back(tmp);
        columnNamesCorrespondances[tmp] = i;
        i++;
    }
}

template <typename T>
void LoadAndSave<T>::saveRaw(const std::string& path)
{
    for(unsigned int i=0;i<columnNames)
}

template <typename T>
void LoadAndSave<T>::saveFloat(const std::string& path)
{

}

template <typename T>
void LoadAndSave<T>::saveTitles(const std::string& path)
{

}

template <typename T>
void LoadAndSave<T>::createHoles(const std::map<std::string,float>& probaHolesInColumn)
{

}

template <typename T>
void LoadAndSave<T>::setConversionArray(const std::map<std::string,std::function<float(std::string)> >& howToConvert)
{

}

template <typename T>
void LoadAndSave<T>::chooseColumns(const std::vector<std::string>& namesToKeep)
{

}

template <typename T>
void LoadAndSave<T>::updateChosenColumns()
{

}
template <typename T>
const std::map<std::string,std::map<std::string,unsigned int> >& LoadAndSave<T>::countAbnormal(const std::map<std::string, std::map<std::string, std::string> >& labelsOfAbnormalForCols)
{

}
template <typename T>
void LoadAndSave<T>::replaceAbnormalInFloat(const std::map<std::string,std::map<std::string,BEHAVIOUR> >& behaviour)
{

}

template <typename T>
void LoadAndSave<T>::saveDensity(const std::vector<std::string>& columns, const std::string& path)
{

}

template <typename T>
template <size_t N>
matrix<T,N,1> LoadAndSave<T>::convertToDlibMatrix(const std::array<std::string,N>& columns)
{

}

template <typename T>
unsigned int LoadAndSave<T>::getNumberRows() const
{

}
template <typename T>
unsigned int LoadAndSave<T>::getNumberColumnsRaw() const
{

}
template <typename T>
unsigned int LoadAndSave<T>::getNumberColumnsFloat() const
{

}


#endif
