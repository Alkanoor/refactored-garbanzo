#ifndef LOADANDSAVE_HPP_INCLUDED
#define LOADANDSAVE_HPP_INCLUDED


#include "util.hpp"


enum BEHAVIOUR{Other_class, Nearest_float, Mean_float};


template <typename T>
class LoadAndSave
{
    public:
        ///chargement des données brutes (fichier train.csv, test.csv, ...)
        void loadRaw(const std::string& path, char separator = ';');
        ///chargement uniquement des données déjà prétraitées (il est nécessaire de spécifier le nombre de colonnes)
        void loadFloat(const std::string& path, unsigned int numberColumns);
        ///chargement uniquement des titres des colonnes (utile avec la méthode précédente qui importe uniquement les données)
        void loadTitles(const std::string& path);

        ///sauvegarde toutes données brutes (prenant en compte les trous et les resize, pour générer de nouveaux jeux de données éventuellement troués)
        void saveRaw(const std::string& path);
        ///sauvegarde toutes données traitées ou semi-traitées (prenant en compte les trous et resize)
        void saveFloat(const std::string& path);
        ///sauvegarde les données brutes actuellement traitées (prenant en compte les trous et les resize, pour générer de nouveaux jeux de données éventuellement troués)
        void saveCurrentRaw(const std::string& path);
        ///sauvegarde les données traitées (prenant en compte les trous et resize)
        void saveCurrentFloat(const std::string& path);
        ///sauvegarde des titres des colonnes actuellement traitées
        void saveTitles(const std::string& path);

        ///crée des trous artificiels attendant d'être utilisés plus tard
        void createHoles(const std::map<std::string,float>& probaHolesInColumn);
        ///met à jour la map permettant de convertir les string en float selon des politiques définies en dehors de cette classe
        void setConversionArray(const std::map<std::string,std::function<T(std::string)> >& howToConvert);

        ///choisit les colonnes sur lesquelles travailler
        void chooseColumns(const std::vector<std::string>& namesToKeep);
        ///met à jour (avec trous et politiques) les données les données flottantes à partir des données string
        void updateChosenColumns();
        ///compte les singularités en leur assignant un label défini par l'utilisateur
        const std::map<std::string,std::map<std::string,unsigned int> >& countAbnormal(const std::map<std::string, std::map<std::string, std::string> >& labelsOfAbnormalForCols);
        ///crée les histogrammes des données en prenant en compte ou non les données "anormales"
        void computeHistogramsOnChosenColumns(bool considerAbnormal = false);
        ///remplace les données "anormales" par d'autres données en fonction d'une politique parmi 3 se basant sur la fréquence de ces anomalies
        void replaceAbnormalInFloat(const std::map<std::string,std::map<std::string,BEHAVIOUR> >& behaviour);

        ///écrit dans le fichier associé les histogrammes des colonnes actuellement traitées et les données anormales remarquées
        void saveDensity(const std::vector<std::string>& columns, const std::string& path);
        ///convertit les données en données pour dlib
        template <size_t N>
        void convertToDlibMatrix(const std::array<std::string,N>& columns, const std::string& labelColumn, std::vector<matrix<T,N,1> >& samples, std::vector<T>& labels);

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
    noHoleColumnsOrdered.resize(numberColumns);
    T tmp;
    std::ifstream ifs(path.c_str());
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
        if(j<chosenColumns.size()-1)
            ofs<<columnNames[columnNamesCorrespondances[chosenColumns[i]]]<<" ";
        else
            ofs<<columnNames[columnNamesCorrespondances[chosenColumns[i]]];
    }
    ofs.close();
}

template <typename T>
void LoadAndSave<T>::createHoles(const std::map<std::string,T>& probaHolesInColumn)
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
const std::map<std::string,std::map<std::string,unsigned int> >& LoadAndSave<T>::countAbnormal(const std::map<std::string, std::map<std::string, std::string> >& labelsOfAbnormalForCols)
{
    abnormal.clear();
    for(auto it=labelsOfAbnormalForCols.begin(); it!=labelsOfAbnormalForCols.end(); it++)
    {
        unsigned int col = columnNamesCorrespondances[it->first];
        for(unsigned int i=0;i<N_features;i++)
        {
            if(it->second.count(rawDataColumnsOrdered[col][i]))
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
void LoadAndSave<T>::replaceAbnormalInFloat(const std::map<std::string,std::map<std::string,BEHAVIOUR> >& behaviour)
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
                            //on cherche dans l'histogramme un endroit où on est sûr qu'il n'y a aucun antécédant (le paramètre est là pour différencier les valeurs pour des anormaux différents)
                            correspondancesForAbnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]] = histograms[chosenColumns[i]].getOutValue(correspondancesForAbnormal[chosenColumns[i]].size());
                            break;
                        case Nearest_float:
                            //on cherche dans l'histogramme l'antécédant le plus proche par rapport à la fréquence de l'anormal
                            correspondancesForAbnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]] = histograms[chosenColumns[i]].getNearest(abnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]]);
                            break;
                        case Mean_float:
                            //on cherche dans l'histogramme l'antécédant moyen par rapport à la fréquence de l'anormal
                            correspondancesForAbnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]] = histograms[chosenColumns[i]].getMean(abnormal[chosenColumns[i]][rawDataColumnsOrdered[col][j]]);
                            break;
                        default:
                            std::cout<<"Be careful please, try to provide correct behaviours for every abnormal situation"<<std::endl;
                            break;
                    }
            }
        }
    }
    //on recalcule l'histogramme en prenant en compte les valeurs des anormaux modifiés
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
void LoadAndSave<T>::convertToDlibMatrix(const std::array<std::string,N>& columns, const std::string& labelColumn, std::vector<matrix<T,N,1> >& samples, std::vector<T>& labels);
{
    std::array<unsigned int,N> corresp;
    samples.resize(N_features);
    labels.resize(N_features);
    matrix<T,N,1> tmp;

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
