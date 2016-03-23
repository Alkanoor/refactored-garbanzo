#ifndef LOADANDSAVE_HPP_INCLUDED
#define LOADANDSAVE_HPP_INCLUDED


#include <dlib/svm.h>

#include "Histogram.hpp"
#include "Interval.hpp"
#include "util.hpp"


enum BEHAVIOUR{Other_class, Nearest_float, Mean_float};


template <typename T>
class LoadAndSave
{
    public:
        ///chargement des donn�es brutes (fichier train.csv, test.csv, ...)
        void loadRaw(const std::string& path, char separator = ';');
        ///chargement uniquement des donn�es d�j� pr�trait�es (il est n�cessaire de sp�cifier le nombre de colonnes)
        void loadFloat(const std::string& path, unsigned int numberColumns);
        ///chargement uniquement des titres des colonnes (utile avec la m�thode pr�c�dente qui importe uniquement les donn�es)
        void loadTitles(const std::string& path);

        ///sauvegarde toutes donn�es brutes (prenant en compte les trous et les resize, pour g�n�rer de nouveaux jeux de donn�es �ventuellement trou�s)
        void saveRaw(const std::string& path);
        ///sauvegarde toutes donn�es trait�es ou semi-trait�es (prenant en compte les trous et resize)
        void saveFloat(const std::string& path);
        ///sauvegarde les donn�es brutes actuellement trait�es (prenant en compte les trous et les resize, pour g�n�rer de nouveaux jeux de donn�es �ventuellement trou�s)
        void saveCurrentRaw(const std::string& path);
        ///sauvegarde les donn�es trait�es (prenant en compte les trous et resize)
        void saveCurrentFloat(const std::string& path);
        ///sauvegarde des titres des colonnes actuellement trait�es
        void saveTitles(const std::string& path);

        ///cr�e des trous artificiels attendant d'�tre utilis�s plus tard
        void createHoles(std::map<std::string,float>& probaHolesInColumn);
        ///met � jour la map permettant de convertir les string en float selon des politiques d�finies en dehors de cette classe
        void setConversionArray(const std::map<std::string,std::function<T(std::string)> >& howToConvert);

        ///choisit les colonnes sur lesquelles travailler
        void chooseColumns(const std::vector<std::string>& namesToKeep);
        ///met � jour (avec trous et politiques) les donn�es les donn�es flottantes � partir des donn�es string
        void updateChosenColumns();
        ///compte les singularit�s en leur assignant un label d�fini par l'utilisateur
        const std::map<std::string,std::map<std::string,unsigned int> >& countAbnormal(std::map<std::string, std::map<std::string, std::string> >& labelsOfAbnormalForCols);
        ///cr�e les histogrammes des donn�es en prenant en compte ou non les donn�es "anormales"
        void computeHistogramsOnChosenColumns(bool considerAbnormal = false);
        ///remplace les donn�es "anormales" par d'autres donn�es en fonction d'une politique parmi 3 se basant sur la fr�quence de ces anomalies
        void replaceAbnormalInFloat(std::map<std::string,std::map<std::string,BEHAVIOUR> >& behaviour);

        ///�crit dans le fichier associ� les histogrammes des colonnes actuellement trait�es et les donn�es anormales remarqu�es
        void saveDensity(const std::vector<std::string>& columns, const std::string& path);
        ///convertit les donn�es en donn�es pour dlib
        template <size_t N>
        void convertToDlibMatrix(const std::array<std::string,N>& columns, const std::string& labelColumn, std::vector<dlib::matrix<T,N,1> >& samples, std::vector<T>& labels);

        unsigned int getNumberRows() const;
        unsigned int getNumberColumnsRaw() const;
        unsigned int getCurrentNumberColumns() const;

    private:
        std::vector<std::vector<std::string> > rawDataColumnsOrdered;
        std::vector<std::vector<T> > transformedDataColumnsOrdered;
        std::vector<std::vector<bool> > noHoleColumnsOrdered;
        std::map<std::string,unsigned int> columnNamesCorrespondances;
        std::vector<std::string> columnNames;
        std::vector<std::string> chosenColumns;

        std::map<std::string,std::function<T(std::string)> > conversionMap;
        std::map<std::string,std::map<std::string,unsigned int> > abnormal;
        std::map<std::string,std::map<Interval<T>, unsigned int> > counter;
        std::map<std::string,Histogram<T> > histograms;
        std::map<std::string,std::map<std::string,T> > correspondancesForAbnormal;

        unsigned int N_features;
        unsigned int N_parameters;
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
    N_features = loadInVec(dataLearn,dataLearnSplit);

    std::vector<std::string> line;
    N_parameters = loadInString(dataLearnSplit[0],line,separator);
    transformedDataColumnsOrdered.resize(N_parameters);
    rawDataColumnsOrdered.resize(N_parameters);
    noHoleColumnsOrdered.resize(N_parameters);

    for(unsigned int j=0;j<N_parameters;j++)
    {
        transformedDataColumnsOrdered[j].resize(N_features);
        rawDataColumnsOrdered[j].resize(N_features);
        noHoleColumnsOrdered[j].resize(N_features);
    }

    for(unsigned int i=0;i<N_features;i++)
    {
        loadInString(dataLearnSplit[i],line,separator);
        for(unsigned int j=0;j<N_parameters;j++)
        {
            transformedDataColumnsOrdered[j][i] = -1;
            rawDataColumnsOrdered[j][i] = line[j];
            noHoleColumnsOrdered[j][i] = true;
        }
    }

    ///On utilise la premi�re ligne pour d�terminer les noms des colonnes, on utilise les suivantes pour se faire une id�e plus pr�cise de la distribution des valeurs diverses que peuvent prendre les items d'une colonne
    for(unsigned int i=0;i<N_parameters;i++)
    {
        columnNames.push_back(rawDataColumnsOrdered[i][0]);
        columnNamesCorrespondances[rawDataColumnsOrdered[i][0]] = i;
    }
}

template <typename T>
void LoadAndSave<T>::loadFloat(const std::string& path, unsigned int numberColumns)
{
    transformedDataColumnsOrdered.resize(numberColumns);
    noHoleColumnsOrdered.resize(numberColumns);
    N_parameters = numberColumns;
    N_features = 0;
    T tmp;
    std::ifstream ifs(path.c_str());
    if(!ifs)
    {
        std::cout<<"Error loading floating data"<<std::endl;
        exit(-1);
    }
    unsigned int i=0, j=0;
    while(ifs>>tmp)
    {
        transformedDataColumnsOrdered[i][j] = tmp;
        noHoleColumnsOrdered[i][j] = true;
        i++;
        if(i>=numberColumns)
        {
            i = 0;
            j++;
            N_features++;
            for(unsigned int k=0;k<numberColumns;k++)
            {
                transformedDataColumnsOrdered[k].push_back(0);
                noHoleColumnsOrdered[k].push_back(true);
            }
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

    N_parameters = columnNames.size();
}

template <typename T>
void LoadAndSave<T>::saveRaw(const std::string& path)
{
    std::ofstream ofs(path.c_str(),std::ios::out|std::ios::trunc);

    for(unsigned int i=0;i<N_features;i++)
    {
        for(unsigned int j=0;j<N_parameters;j++)
            if(noHoleColumnsOrdered[j][i])
            {
                if(j<N_parameters-1)
                    ofs<<rawDataColumnsOrdered[j][i]<<";";
                else
                    ofs<<rawDataColumnsOrdered[j][i];
            }
            else
                if(j<N_parameters-1)
                    ofs<<";";

        ofs<<std::endl;
    }
    ofs.close();
}

template <typename T>
void LoadAndSave<T>::saveFloat(const std::string& path)
{
    std::ofstream ofs(path.c_str(),std::ios::out|std::ios::trunc);
    for(unsigned int i=0;i<N_features;i++)
    {
        for(unsigned int j=0;j<N_parameters;j++)
            if(noHoleColumnsOrdered[j][i])
            {
                if(j<N_parameters-1)
                    ofs<<transformedDataColumnsOrdered[j][i]<<" ";
                else
                    ofs<<transformedDataColumnsOrdered[j][i];
            }
            else
                if(j<N_parameters-1)
                    ofs<<" -999999999 ";
        ofs<<std::endl;
    }
    ofs.close();
}

template <typename T>
void LoadAndSave<T>::saveCurrentRaw(const std::string& path)
{
    std::ofstream ofs(path.c_str(),std::ios::out|std::ios::trunc);

    std::vector<unsigned int> index(chosenColumns.size());
    for(unsigned int i=0;i<chosenColumns.size();i++)
        index[i] = columnNamesCorrespondances[chosenColumns[i]];

    for(unsigned int i=0;i<N_features;i++)
    {
        for(unsigned int j=0;j<index.size();j++)
            if(noHoleColumnsOrdered[index[j]][i])
            {
                if(j<index.size()-1)
                    ofs<<rawDataColumnsOrdered[index[j]][i]<<";";
                else
                    ofs<<rawDataColumnsOrdered[index[j]][i];
            }
            else
                if(j<index.size()-1)
                    ofs<<";";
        ofs<<std::endl;
    }
    ofs.close();
}

template <typename T>
void LoadAndSave<T>::saveCurrentFloat(const std::string& path)
{
    std::ofstream ofs(path.c_str(),std::ios::out|std::ios::trunc);

    std::vector<unsigned int> index(chosenColumns.size());
    for(unsigned int i=0;i<chosenColumns.size();i++)
        index[i] = columnNamesCorrespondances[chosenColumns[i]];

    for(unsigned int i=0;i<N_features;i++)
    {
        for(unsigned int j=0;j<chosenColumns.size();j++)
            if(noHoleColumnsOrdered[index[j]][i])
            {
                if(j<index.size()-1)
                    ofs<<transformedDataColumnsOrdered[index[j]][i]<<" ";
                else
                    ofs<<transformedDataColumnsOrdered[index[j]][i];
            }
            else
                if(j<index.size()-1)
                    ofs<<" -999999999 ";
        ofs<<std::endl;
    }
    ofs.close();
}

template <typename T>
void LoadAndSave<T>::saveTitles(const std::string& path)
{
    std::ofstream ofs(path.c_str(),std::ios::out|std::ios::trunc);
    for(unsigned int i=0;i<chosenColumns.size();i++)
    {
        if(i<chosenColumns.size()-1)
            ofs<<columnNames[columnNamesCorrespondances[chosenColumns[i]]]<<" ";
        else
            ofs<<columnNames[columnNamesCorrespondances[chosenColumns[i]]];
    }
    ofs.close();
}

template <typename T>
void LoadAndSave<T>::createHoles(std::map<std::string,float>& probaHolesInColumn)
{
    for(unsigned int i=0;i<N_parameters;i++)
    {
        int a = probaHolesInColumn[columnNames[i]]*10000;
        for(unsigned int j=0;j<N_features;j++)
            if(rand()%10000>=a)
                noHoleColumnsOrdered[i][j] = true;
            else
                noHoleColumnsOrdered[i][j] = false;
    }
}

template <typename T>
void LoadAndSave<T>::setConversionArray(const std::map<std::string,std::function<T(std::string)> >& howToConvert)
{
    conversionMap = howToConvert;
}

template <typename T>
void LoadAndSave<T>::chooseColumns(const std::vector<std::string>& namesToKeep)
{
    chosenColumns = namesToKeep;
}

template <typename T>
void LoadAndSave<T>::updateChosenColumns()
{
    for(unsigned int i=0;i<chosenColumns.size();i++)
    {
        unsigned int col = columnNamesCorrespondances[chosenColumns[i]];
        for(unsigned int j=0;j<N_features;j++)
            if(noHoleColumnsOrdered[col][j])
                transformedDataColumnsOrdered[col][j] = conversionMap[chosenColumns[i]](rawDataColumnsOrdered[col][j]);
            else
                transformedDataColumnsOrdered[col][j] = conversionMap[chosenColumns[i]]("");
    }
}

template <typename T>
const std::map<std::string,std::map<std::string,unsigned int> >& LoadAndSave<T>::countAbnormal(std::map<std::string, std::map<std::string, std::string> >& labelsOfAbnormalForCols)
{
    abnormal.clear();
    for(auto it=labelsOfAbnormalForCols.begin(); it!=labelsOfAbnormalForCols.end(); it++)
    {
        unsigned int col = columnNamesCorrespondances[it->first];
        for(unsigned int i=0;i<N_features;i++)
            if(it->second.count(rawDataColumnsOrdered[col][i]))
            {
                if(abnormal[it->first].count(it->second[rawDataColumnsOrdered[col][i]]))
                    abnormal[it->first][it->second[rawDataColumnsOrdered[col][i]]]++;
                else
                    abnormal[it->first][it->second[rawDataColumnsOrdered[col][i]]] = 1;
            }
    }
    return abnormal;
}

template <typename T>
void LoadAndSave<T>::computeHistogramsOnChosenColumns(bool considerAbnormal)
{
    histograms.clear();
    counter.clear();
    for(unsigned int i=0;i<chosenColumns.size();i++)
    {
        unsigned int col = columnNamesCorrespondances[chosenColumns[i]];
        unsigned int N = 0;
        for(unsigned int j=0;j<N_features;j++)
            if(considerAbnormal||(!considerAbnormal&&!abnormal[chosenColumns[i]].count(rawDataColumnsOrdered[col][j])))
            {
                auto interval = Interval<T>(transformedDataColumnsOrdered[col][j],rawDataColumnsOrdered[col][j]);
                if(counter[chosenColumns[i]].count(interval))
                    counter[chosenColumns[i]][interval]++;
                else
                    counter[chosenColumns[i]][interval] = 1;
                N++;
            }
        float cur=0;
        Interval<T> curInterval("-inf",0);
        for(auto it=counter[chosenColumns[i]].begin(); it!=counter[chosenColumns[i]].end(); it++)
        {
            curInterval.setRight(it->first.left());
            histograms.set(curInterval,cur);
            cur += (float)it->second/(float)N;
            curInterval.setLabel(it->first.label());
            curInterval.setLeft(it->first.left());
            curInterval.setRight(it->first.right());
            histograms.set(curInterval,cur);
        }
    }
}

template <typename T>
void LoadAndSave<T>::replaceAbnormalInFloat(std::map<std::string,std::map<std::string,BEHAVIOUR> >& behaviour)
{
    updateChosenColumns();
    computeHistogramsOnChosenColumns(false);
    correspondancesForAbnormal.clear();
    for(unsigned int i=0;i<chosenColumns.size();i++)
    {
        unsigned int col = columnNamesCorrespondances[chosenColumns[i]];
        for(unsigned int j=0;j<N_features;j++)
        {
            if(abnormal[chosenColumns[i]].count(rawDataColumnsOrdered[col][j]))
            {
                if(correspondancesForAbnormal[chosenColumns[i]].count(rawDataColumnsOrdered[col][j]))
                    transformedDataColumnsOrdered[col][j] = correspondancesForAbnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]];
                else
                    switch(behaviour[chosenColumns[i]][rawDataColumnsOrdered[col][j]])
                    {
                        case Other_class:
                            //on cherche dans l'histogramme un endroit o� on est s�r qu'il n'y a aucun ant�c�dant (le param�tre est l� pour diff�rencier les valeurs pour des anormaux diff�rents)
                            correspondancesForAbnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]] = histograms[chosenColumns[i]].getOutValue(correspondancesForAbnormal[chosenColumns[i]].size());
                            break;
                        case Nearest_float:
                            //on cherche dans l'histogramme l'ant�c�dant le plus proche par rapport � la fr�quence de l'anormal
                            correspondancesForAbnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]] = histograms[chosenColumns[i]].getNearest((float)abnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]]/(float)N_features);
                            break;
                        case Mean_float:
                            //on cherche dans l'histogramme l'ant�c�dant moyen par rapport � la fr�quence de l'anormal
                            correspondancesForAbnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]] = histograms[chosenColumns[i]].getMean((float)abnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]]/(float)N_features);
                            break;
                        default:
                            std::cout<<"Be careful please, try to provide correct behaviours for every abnormal situation"<<std::endl;
                            break;
                    }
            }
        }
    }
    //on recalcule l'histogramme en prenant en compte les valeurs des anormaux modifi�s
    computeHistogramsOnChosenColumns(true);
}

template <typename T>
void LoadAndSave<T>::saveDensity(const std::vector<std::string>& columns, const std::string& path)
{
    std::ofstream ofs(path.c_str(),std::ios::out);
    for(unsigned int i=0;i<columns.size();i++)
    {
        unsigned int col = columnNamesCorrespondances[columns[i]];
        if(!histograms.count(columns[i]))
            ofs<<"Histogram for column "<<columns[i]<<" has not been created"<<std::endl<<std::endl;
        else
        {
            ofs<<"============================"<<std::endl;
            ofs<<"====Histogram of "<<columns[i]<<"===="<<std::endl;
            ofs<<"============================"<<std::endl;
            for(auto it=correspondancesForAbnormal[columns[i]].begin(); it!=correspondancesForAbnormal[columns[i]].end(); it++)
                ofs<<"Correspondance for abnormal "<<it->first<<" => "<<it->second<<std::endl;
            ofs<<std::endl;
            for(auto it=histograms[columns[i]].begin(); it!=histograms[columns[i]].end(); it++)
                ofs<<(*it);
            ofs<<"============================"<<std::endl<<std::endl;;
        }
    }
    ofs.close();
}

template <typename T>
template <size_t N>
void LoadAndSave<T>::convertToDlibMatrix(const std::array<std::string,N>& columns, const std::string& labelColumn, std::vector<dlib::matrix<T,N,1> >& samples, std::vector<T>& labels)
{
    std::array<unsigned int,N> corresp;
    samples.resize(N_features);
    labels.resize(N_features);
    dlib::matrix<T,N,1> tmp;

    for(unsigned int i=0;i<N;i++)
        corresp[i] = columnNamesCorrespondances[columns[i]];
    unsigned int targetColumn = columnNamesCorrespondances[labelColumn];
    for(unsigned int j=0;j<N_features;j++)
    {
        for(unsigned int i=0;i<N;i++)
            tmp[i] = transformedDataColumnsOrdered[corresp[i]][j];
        samples[j] = tmp;
        labels[j] = transformedDataColumnsOrdered[targetColumn][j];
    }
    return samples;
}

template <typename T>
unsigned int LoadAndSave<T>::getNumberRows() const
{
    return N_features;
}

template <typename T>
unsigned int LoadAndSave<T>::getNumberColumnsRaw() const
{
    return N_parameters;
}

template <typename T>
unsigned int LoadAndSave<T>::getCurrentNumberColumns() const
{
    return chosenColumns.size();
}


#endif
