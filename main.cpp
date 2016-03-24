#include <iostream>
#include <map>

#include "include/ThreadedOperations.hpp"
#include "include/ConversionFunction.hpp"
#include "include/LoadAndSave.hpp"


int main()
{
    srand(time(NULL));

    /*std::vector<std::string> columns = {"SOURCE_BEGIN_MONTH","VOIE_DEPOT","LANGUAGE_OF_FILLING","COUNTRY","FISRT_APP_TYPE","APP_NB_PAYS"};
    columns = {"FISRT_APP_TYPE"};

    LoadAndSave<float> wrapper;

    wrapper.loadRaw("../trainLittle.csv");
    wrapper.chooseColumns(columns);
    wrapper.saveRaw("log1_identity/raw.txt");
    wrapper.saveCurrentRaw("log1_identity/currentRaw.txt");
    wrapper.saveTitles("log1_identity/titles.txt");

    std::map<std::string,float> probaHolesInColumn;
    probaHolesInColumn["VOIE_DEPOT"] = 0.05;
    probaHolesInColumn["COUNTRY"] = 0.05;
    probaHolesInColumn["SOURCE_BEGIN_MONTH"] = 0.05;
    probaHolesInColumn["LANGUAGE_OF_FILLING"] = 0.05;
    probaHolesInColumn["FISRT_APP_TYPE"] = 0.05;
    probaHolesInColumn["APP_NB_PAYSerror"] = 0.05;


    wrapper.createHoles(probaHolesInColumn);
    wrapper.saveRaw("log2_with_holes/raw.txt");
    wrapper.saveCurrentRaw("log2_with_holes/currentRaw.txt");
    wrapper.saveTitles("log2_with_holes/titles.txt");


    probaHolesInColumn["APP_NB_PAYS"] = 0.05;

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
    conversionObjects["SOURCE_BEGIN_MONTH"] = ConversionFunction<float>(3);
    conversionObjects["LANGUAGE_OF_FILLING"] = ConversionFunction<float>(3);
    conversionObjects["FISRT_APP_TYPE"] = ConversionFunction<float>(3);
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

    std::map<std::string, std::set<std::string> > labelsOfAbnormalForCols;
    for(unsigned int i=0;i<columns.size();i++)
        labelsOfAbnormalForCols[columns[i]] = unsetElements;

    std::map<std::string,std::map<std::string,unsigned int> > abnormals = wrapper.countAbnormal(labelsOfAbnormalForCols);

    for(auto it=abnormals.begin(); it!=abnormals.end(); it++)
        std::cout<<it->first<<" => "<<it->second[""]<<" empty and "<<it->second["(MISSING)"]<<" missing"<<std::endl;


    std::map<std::string,BEHAVIOUR> defaultBehaviour;
    defaultBehaviour[""] = Other_class;
    defaultBehaviour["(MISSING)"] = Other_class;

    std::map<std::string,std::map<std::string,BEHAVIOUR> > behaviours;
    for(auto it=conversionObjects.begin(); it!=conversionObjects.end(); it++)
        behaviours[it->first] = defaultBehaviour;

    behaviours["APP_NB_PAYS"][""] = Mean_float;
    behaviours["APP_NB_PAYS"]["(MISSING)"] = Mean_float;

    wrapper.replaceAbnormalInFloat(behaviours);
    wrapper.saveDensity(columns, "densities.txt");
    wrapper.saveCurrentFloat("floatFinal.txt");*/



    std::vector<std::string> columns = {"IDX_RADIC","VARIABLE_CIBLE"};

    LoadAndSave<float> wrapper;

    wrapper.loadFloat("serious/currentConverted.txt",2);
    wrapper.loadTitles("serious/titles.txt");
    wrapper.chooseColumns(columns);

    /*wrapper.loadRaw("../train.csv");
    wrapper.chooseColumns(columns);
    wrapper.saveCurrentRaw("serious/currentRaw.txt");
    wrapper.saveTitles("serious/titles.txt");

    std::set<std::string> unsetElements;
    unsetElements.insert("");
    unsetElements.insert("(MISSING)");

    std::map<std::string,ConversionFunction<float> > conversionObjects;
    std::map<std::string,std::function<float(std::string)> > conversionMap;

    conversionObjects["IDX_RADIC"] = ConversionFunction<float>(1);
    conversionObjects["VARIABLE_CIBLE"] = ConversionFunction<float>(3);

    for(auto it=conversionObjects.begin(); it!=conversionObjects.end(); it++)
    {
        it->second.setUnknown(unsetElements);
        conversionMap[it->first] = it->second.conversionFunction();
    }

    wrapper.setConversionArray(conversionMap);
    wrapper.updateChosenColumns();

    wrapper.saveCurrentFloat("serious/currentConverted.txt");*/

    std::array<std::string,1> cols = {"IDX_RADIC"};
    std::vector<dlib::matrix<float,1,1> > samples;
    std::vector<float> labels;
    wrapper.convertToDlibMatrix<1>(cols,"VARIABLE_CIBLE",samples,labels);

    typedef dlib::matrix<float, 1, 1> sample_type;
    typedef dlib::radial_basis_kernel<sample_type> kernel_type;

    using namespace dlib;
    using namespace std;

     // Here we normalize all the samples by subtracting their mean and dividing by their
    // standard deviation.  This is generally a good idea since it often heads off
    // numerical stability problems and also prevents one large feature from smothering
    // others.  Doing this doesn't matter much in this example so I'm just doing this here
    // so you can see an easy way to accomplish this with the library.
    vector_normalizer<sample_type> normalizer;
    // let the normalizer learn the mean and standard deviation of the samples
    normalizer.train(samples);
    // now normalize each sample
    for (unsigned long i = 0; i < samples.size(); ++i)
        samples[i] = normalizer(samples[i]);


    // Now that we have some data we want to train on it.  However, there are two
    // parameters to the training.  These are the nu and gamma parameters.  Our choice for
    // these parameters will influence how good the resulting decision function is.  To
    // test how good a particular choice of these parameters is we can use the
    // cross_validate_trainer() function to perform n-fold cross validation on our training
    // data.  However, there is a problem with the way we have sampled our distribution
    // above.  The problem is that there is a definite ordering to the samples.  That is,
    // the first half of the samples look like they are from a different distribution than
    // the second half.  This would screw up the cross validation process but we can fix it
    // by randomizing the order of the samples with the following function call.
    randomize_samples(samples, labels);


    // The nu parameter has a maximum value that is dependent on the ratio of the +1 to -1
    // labels in the training data.  This function finds that value.
    const double max_nu = maximum_nu(labels);

    // here we make an instance of the svm_nu_trainer object that uses our kernel type.
    svm_nu_trainer<kernel_type> trainer;

    // Now we loop over some different nu and gamma values to see how good they are.  Note
    // that this is a very simple way to try out a few possible parameter choices.  You
    // should look at the model_selection_ex.cpp program for examples of more sophisticated
    // strategies for determining good parameter choices.
    cout << "doing cross validation" << endl;
    for (double gamma = 0.00001; gamma <= 1; gamma *= 5)
    {
        for (double nu = 0.00001; nu < max_nu; nu *= 5)
        {
            // tell the trainer the parameters we want to use
            trainer.set_kernel(kernel_type(gamma));
            trainer.set_nu(nu);

            cout << "gamma: " << gamma << "    nu: " << nu;
            // Print out the cross validation accuracy for 3-fold cross validation using
            // the current gamma and nu.  cross_validate_trainer() returns a row vector.
            // The first element of the vector is the fraction of +1 training examples
            // correctly classified and the second number is the fraction of -1 training
            // examples correctly classified.
            cout << "     cross validation accuracy: " << cross_validate_trainer(trainer, samples, labels, 3);
        }
    }

    /*template <size_t N>
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
