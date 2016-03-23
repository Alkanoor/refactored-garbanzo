#include <iostream>
#include <map>

#include "include/ThreadedOperations.hpp"
#include "include/ConversionFunction.hpp"
#include "include/LoadAndSave.hpp"


int main()
{
    srand(time(NULL));

    std::vector<std::string> columns = {"SOURCE_BEGIN_MONTH","VOIE_DEPOT","LANGUAGE_OF_FILLING","COUNTRY","APP_NB_PAYS"};

    LoadAndSave<float> wrapper;

    wrapper.loadRaw("../trainLittle.csv");
    wrapper.chooseColumns(columns);
    wrapper.saveRaw("log1_identity/raw.txt");
    wrapper.saveCurrentRaw("log1_identity/currentRaw.txt");
    wrapper.saveTitles("log1_identity/titles.txt");

    std::map<std::string,float> probaHolesInColumn;
    probaHolesInColumn["VOIE_DEPOT"] = 0.1;
    probaHolesInColumn["COUNTRY"] = 0.05;
    probaHolesInColumn["SOURCE_BEGIN_MONTH"] = 0.8;
    probaHolesInColumn["LANGUAGE_OF_FILLING"] = 0.3;
    probaHolesInColumn["APP_NB_PAYSerror"] = 0.2;


    wrapper.createHoles(probaHolesInColumn);
    wrapper.saveRaw("log2_with_holes/raw.txt");
    wrapper.saveCurrentRaw("log2_with_holes/currentRaw.txt");
    wrapper.saveTitles("log2_with_holes/titles.txt");


    probaHolesInColumn["APP_NB_PAYS"] = 0.2;

    wrapper.randomShuffle();
    wrapper.createHoles(probaHolesInColumn);
    wrapper.saveRaw("log3_with_holes/raw.txt");
    wrapper.saveCurrentRaw("log3_with_holes/currentRaw.txt");
    wrapper.saveTitles("log3_with_holes/titles.txt");

    std::set<std::string> unsetElements;
    unsetElements.insert("");
    unsetElements.insert("(MISSING)");

    std::map<std::string,ConversionFunction<float> > conversionObjects;
    std::map<std::string,std::function<float(std::string)> > conversionMap;

    conversionObjects["VOIE_DEPOT"] = ConversionFunction<float>(3);
    conversionObjects["COUNTRY"] = ConversionFunction<float>(3);
    conversionObjects["SOURCE_BEGIN_MONTH"] = ConversionFunction<float>(2);
    conversionObjects["LANGUAGE_OF_FILLING"] = ConversionFunction<float>(3);
    conversionObjects["APP_NB_PAYS"] = ConversionFunction<float>(1);

    for(auto it=conversionObjects.begin(); it!=conversionObjects.end(); it++)
    {
        it->second.setUnknown(unsetElements);
        conversionMap[it->first] = it->second.conversionFunction();
    }

    wrapper.setConversionArray(conversionMap);
    wrapper.updateChosenColumns();

    wrapper.saveRaw("log4_with_all/raw.txt");
    wrapper.saveFloat("log4_with_all/converted.txt");
    wrapper.saveCurrentRaw("log4_with_all/currentRaw.txt");
    wrapper.saveCurrentFloat("log4_with_all/currentConverted.txt");
    wrapper.saveTitles("log4_with_all/titles.txt");


    /*const std::map<std::string,std::map<std::string,unsigned int> >& countAbnormal(std::map<std::string, std::map<std::string, std::string> >& labelsOfAbnormalForCols);
    void computeHistogramsOnChosenColumns(bool considerAbnormal = false);
    void replaceAbnormalInFloat(std::map<std::string,std::map<std::string,BEHAVIOUR> >& behaviour);

    void saveDensity(const std::vector<std::string>& columns, const std::string& path);

    template <size_t N>
    void convertToDlibMatrix(const std::array<std::string,N>& columns, const std::string& labelColumn, std::vector<dlib::matrix<T,N,1> >& samples, std::vector<T>& labels);*/


    /*wrapper.loadTitles("titles.txt");
    wrapper.loadFloat("currentConverted.txt", wrapper.getNumberColumnsRaw());*/

    return 0;
}



int autre()
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
    ///On s�pare les donn�es initiales par le retour � la ligne
    unsigned int N_features = loadInVec(dataLearn,dataLearnSplit);

    ///On cr�e et remplit la matrice finale des donn�es en s�parant par les ";"
    std::vector<std::vector<std::vector<unsigned char> > > finalDataset(N_features);
    for(unsigned int i=0;i<N_features;i++)
        loadInVec(dataLearnSplit[i],finalDataset[i],';');

    ///On utilise la premi�re ligne pour d�terminer les noms des colonnes, on utilise les suivantes pour se faire une id�e plus pr�cise de la distribution des valeurs diverses que peuvent prendre les items d'une colonne
    std::map<std::string,unsigned int> columnNames;
    std::map<std::string,std::map<std::string,unsigned int> > counter;
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

    ///On log le tout pour pouvoir analyser manuellement le r�sultat
    std::ofstream ofs("log.txt",std::ios::out|std::ios::trunc);
    for(std::map<std::string,std::map<std::string,unsigned int> >::iterator it=counter.begin();it!=counter.end();it++)
    {
        std::cout<<it->first<<std::endl;
        ofs<<it->first<<std::endl;
        if(it->second.size()<10)
            for(std::map<std::string,unsigned int>::iterator itbis=it->second.begin();itbis!=it->second.end();itbis++)
                std::cout<<itbis->first<<" => "<<itbis->second<<std::endl;
        else
            std::cout<<"=> "<<it->second.size()<<std::endl;
        unsigned int k=0;
        unsigned int sum=0;
        for(std::map<std::string,unsigned int>::iterator itbis=it->second.begin();itbis!=it->second.end()&&k<150;itbis++,k++)
        {
            sum += itbis->second;
            ofs<<itbis->first<<" => "<<itbis->second<<std::endl;
        }
        std::cout<<"gives "<<sum<<std::endl<<std::endl;
        ofs<<"gives "<<sum<<std::endl<<std::endl;
    }

    /**On utilise manuellement, pour chaque colonne, la fonction la plus adapt�e pour la conversion string/float
     On a le choix entre plusieurs fonctions :
        1- La fonction de conversion string->float classique, adapt�e pour les param�tres continus ou suppos�s l'�tre
        2- La fonction de conversion string->float avec remplacement de valeur par esp�rance quand valeur "inconnue" (non renseign�e)
        3- La fonction de conversion date-mois-ann�e(string)->float
        4- La fonction d'�crasement basique : string->float avec float une classe al�atoire fix�e pour chaque label
        5- La fonction d'�crasement bas�e sur la distance de hamming : string->float avec une notion de proximit�**/

    /**Voici la liste des fonctions utilis�es pour chaque cat�gorie :
        -APP_NB : 2]
        -APP_NB_PAYS : 2]
        -APP_NB_TYPE : 2]
        -BEGIN_MONTH : 3]
        -COUNTRY : 4]
        -FILING_MONTH : 3]
        -FIRST_CLASSE : 5]
        -FISRT_APP_COUNTRY : 4]
        -FISRT_APP_TYPE : 4]
        -FISRT_INV_COUNTRY : 4]
        -FISRT_INV_TYPE : 4]
        -IDX_ORIGIN : 1]
        -IDX_RADIC : 1]
        -INV_NB : 2]
        -INV_NB_PAYS : 2]
        -INV_NB_TYPE : 2]
        -LANGUAGE_OF_FILLING : 4]
        -MAIN_IPC : 5]
        -NB_BACKWARD : 1]
        -NB_BACKWARD_AUTRE : 1]
        -NB_BACKWARD_I : 1]
        -NB_BACKWARD_NPL : 1]
        -NB_BACKWARD_PL : 1]
        -NB_BACKWARD_XY : 1]
        -NB_CLASSES : 1]
        -NB_FIELDS : 1]
        -NB_ROOT_CLASSES : 1]
        -NB_SECTORS : 1]
        -PRIORITY_MONTH : 3]
        -PUBLICATION_MONTH : 3]
        -SOURCE_BEGIN_MONTH : 2]
        -SOURCE_CITED_AGE : 4]
        -SOURCE_IDX_ORI : 4]
        -SOURCE_IDX_RAD : 4]
        -TECHNOLOGIE_FIELD : 4]
        -TECHNOLOGIE_SECTOR : 4]
        -VARIABLE_CIBLE : 4]
        -VOIE_DEPOT : 4]
        -cited_age_max : 1]
        -cited_age_mean : 1]
        -cited_age_median : 1]
        -cited_age_min : 1]
        -cited_age_std : 1]
        -cited_n : 1]
        -cited_nmiss : 4]
        -oecd_NB_BACKWARD_NPL : 1]
        -oecd_NB_BACKWARD_PL : 1]
        -oecd_NB_ROOT_CLASSES : 1]
        -pct_NB_IPC : 1]
        -pct_NB_IPC_LY : 1]
    **/

    return 0;
}
