#include <iostream>
#include <map>

#include "include/ThreadedOperations.hpp"
#include "include/ConversionFunction.hpp"
#include "include/LoadAndSave.hpp"



/**std::vector<std::string> columns = {"SOURCE_BEGIN_MONTH","VOIE_DEPOT","LANGUAGE_OF_FILLING","COUNTRY","FISRT_APP_TYPE","APP_NB_PAYS"};
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
    wrapper.saveCurrentFloat("floatFinal.txt");**/

void updateFloatData();
void executeBigData();

int main()
{
    srand(time(NULL));

    updateFloatData();
    //executeBigData();

    return 0;
}

void updateFloatData()
{
    std::vector<std::string> columns = {"VOIE_DEPOT","COUNTRY","SOURCE_BEGIN_MONTH","APP_NB","APP_NB_PAYS","APP_NB_TYPE","FISRT_APP_COUNTRY","FISRT_APP_TYPE","LANGUAGE_OF_FILLING","FIRST_CLASSE","NB_CLASSES","NB_ROOT_CLASSES","NB_SECTORS","NB_FIELDS","TECHNOLOGIE_SECTOR","TECHNOLOGIE_FIELD","MAIN_IPC","INV_NB","INV_NB_PAYS","INV_NB_TYPE","FISRT_INV_COUNTRY","FISRT_INV_TYPE","cited_n","cited_nmiss","cited_age_min","cited_age_median","cited_age_max","cited_age_mean","cited_age_std","SOURCE_CITED_AGE","NB_BACKWARD_NPL","NB_BACKWARD_XY","NB_BACKWARD_I","NB_BACKWARD_AUTRE","NB_BACKWARD_PL","NB_BACKWARD","pct_NB_IPC","pct_NB_IPC_LY","oecd_NB_ROOT_CLASSES","oecd_NB_BACKWARD_PL","oecd_NB_BACKWARD_NPL","IDX_ORIGIN","SOURCE_IDX_ORI","IDX_RADIC","SOURCE_IDX_RAD","PRIORITY_MONTH","FILING_MONTH","PUBLICATION_MONTH","BEGIN_MONTH","VARIABLE_CIBLE"};

    LoadAndSave<float> wrapper;

    wrapper.loadRaw("train.csv");
    //wrapper.loadRaw("tmpRaw.csv");
    wrapper.chooseColumns(columns);
    //wrapper.randomShuffle();
    //wrapper.resize(0.001);

    //wrapper.saveCurrentRaw("tmpRaw.csv");

    std::set<std::string> unsetElements;
    unsetElements.insert("");
    unsetElements.insert("(MISSING)");

    std::map<std::string, unsigned int> correspondances;
    std::map<std::string,ConversionFunction<float> > conversionObjects;
    std::map<std::string,std::function<float(std::string)> > conversionMap;

    correspondances["VOIE_DEPOT"] = 3;
    correspondances["COUNTRY"] = 3;
    correspondances["SOURCE_BEGIN_MONTH"] = 3;
    correspondances["APP_NB"] = 1;
    correspondances["APP_NB_PAYS"] = 0;
    correspondances["APP_NB_TYPE"] = 1;
    correspondances["FISRT_APP_COUNTRY"] = 3;
    correspondances["FISRT_APP_TYPE"] = 3;
    correspondances["LANGUAGE_OF_FILLING"] = 3;
    correspondances["FIRST_CLASSE"] = 3;
    correspondances["NB_CLASSES"] = 1;
    correspondances["NB_ROOT_CLASSES"] = 1;
    correspondances["NB_SECTORS"] = 1;
    correspondances["NB_FIELDS"] = 1;
    correspondances["TECHNOLOGIE_SECTOR"] = 3;
    correspondances["TECHNOLOGIE_FIELD"] = 3;
    correspondances["MAIN_IPC"] = 3;
    correspondances["INV_NB"] = 1;
    correspondances["INV_NB_PAYS"] = 1;
    correspondances["INV_NB_TYPE"] = 1;
    correspondances["FISRT_INV_COUNTRY"] = 3;
    correspondances["FISRT_INV_TYPE"] = 3;
    correspondances["cited_n"] = 1;
    correspondances["cited_nmiss"] = 1;
    correspondances["cited_age_min"] = 1;
    correspondances["cited_age_median"] = 1;
    correspondances["cited_age_max"] = 1;
    correspondances["cited_age_mean"] = 1;
    correspondances["cited_age_std"] = 1;
    correspondances["SOURCE_CITED_AGE"] = 3;
    correspondances["NB_BACKWARD_NPL"] = 1;
    correspondances["NB_BACKWARD_XY"] = 1;
    correspondances["NB_BACKWARD_I"] = 1;
    correspondances["NB_BACKWARD_AUTRE"] = 1;
    correspondances["NB_BACKWARD_PL"] = 1;
    correspondances["NB_BACKWARD"] = 1;
    correspondances["pct_NB_IPC"] = 1;
    correspondances["pct_NB_IPC_LY"] = 1;
    correspondances["oecd_NB_ROOT_CLASSES"] = 1;
    correspondances["oecd_NB_BACKWARD_PL"] = 1;
    correspondances["oecd_NB_BACKWARD_NPL"] = 1;
    correspondances["IDX_ORIGIN"] = 1;
    correspondances["SOURCE_IDX_ORI"] = 3;
    correspondances["IDX_RADIC"] = 1;
    correspondances["SOURCE_IDX_RAD"] = 3;
    correspondances["PRIORITY_MONTH"] = 2;
    correspondances["FILING_MONTH"] = 2;
    correspondances["PUBLICATION_MONTH"] = 2;
    correspondances["BEGIN_MONTH"] = 2;
    correspondances["VARIABLE_CIBLE"] = 3;

    for(auto it=correspondances.begin(); it!=correspondances.end(); it++)
        conversionObjects[it->first] = ConversionFunction<float>(it->second);

    for(auto it=conversionObjects.begin(); it!=conversionObjects.end(); it++)
    {
        it->second.setUnknown(unsetElements);
        conversionMap[it->first] = it->second.conversionFunction();
    }

    wrapper.setConversionArray(conversionMap);
    wrapper.updateChosenColumns();

    std::map<std::string, std::set<std::string> > labelsOfAbnormalForCols;
    for(unsigned int i=0;i<columns.size();i++)
        labelsOfAbnormalForCols[columns[i]] = unsetElements;

    std::map<std::string,std::map<std::string,unsigned int> > abnormals = wrapper.countAbnormal(labelsOfAbnormalForCols);

    for(auto it=abnormals.begin(); it!=abnormals.end(); it++)
        std::cout<<it->first<<" => "<<it->second[""]<<" empty and "<<it->second["(MISSING)"]<<" missing"<<std::endl;


    std::map<std::string,BEHAVIOUR> classBehaviour;
    classBehaviour[""] = Other_class;
    classBehaviour["(MISSING)"] = Other_class;

    std::map<std::string,BEHAVIOUR> floatBehaviour;
    floatBehaviour[""] = Mean_float;
    floatBehaviour["(MISSING)"] = Mean_float;

    std::map<std::string,std::map<std::string,BEHAVIOUR> > behaviours;
    for(auto it=correspondances.begin(); it!=correspondances.end(); it++)
        if(it->second>=3)
            behaviours[it->first] = classBehaviour;
        else
            behaviours[it->first] = floatBehaviour;

    wrapper.replaceAbnormalInFloat(behaviours);

    std::vector<std::pair<std::string,int> > tmp = wrapper.columnsWhichCouldBeCategorized(130);
    for(unsigned int i=0;i<tmp.size();i++)
        std::cout<<tmp[i].first<<" "<<tmp[i].second<<std::endl;

    std::set<std::string> toCategorize = {"COUNTRY","FISRT_APP_COUNTRY","FISRT_APP_TYPE","LANGUAGE_OF_FILLING","TECHNOLOGIE_SECTOR","TECHNOLOGIE_FIELD","FISRT_INV_COUNTRY","FISRT_INV_TYPE","VARIABLE_CIBLE"};
    wrapper.categorize(toCategorize);
    wrapper.saveCurrentFloatCategorizedAsRaw("data/floatCategorized.csv");

    /*wrapper.saveDensity(columns, "seriousAll/densities.txt");
    wrapper.saveCurrentFloat("seriousAll/floatFinal.txt");
    wrapper.saveTitles("seriousAll/titles.txt");
    wrapper.saveCurrentFloatAsRaw("seriousAll/train.csv");*/

    columns.resize(columns.size()-1);
    wrapper.loadRaw("test.csv");
    wrapper.chooseColumns(columns);
    //wrapper.randomShuffle();

    wrapper.updateChosenColumns();
    abnormals = wrapper.countAbnormal(labelsOfAbnormalForCols);

    for(auto it=abnormals.begin(); it!=abnormals.end(); it++)
        std::cout<<it->first<<" => "<<it->second[""]<<" empty and "<<it->second["(MISSING)"]<<" missing"<<std::endl;

    wrapper.replaceAbnormalInFloatWithPreviousComputed();
    wrapper.saveCurrentFloatCategorizedAsRaw("data/floatCategorizedTest.csv");
}

/*
void executeBigData()
{
    std::vector<std::string> columns = {"VOIE_DEPOT","COUNTRY","SOURCE_BEGIN_MONTH","APP_NB","APP_NB_PAYS","APP_NB_TYPE","FISRT_APP_COUNTRY","FISRT_APP_TYPE","LANGUAGE_OF_FILLING","FIRST_CLASSE","NB_CLASSES","NB_ROOT_CLASSES","NB_SECTORS","NB_FIELDS","TECHNOLOGIE_SECTOR","TECHNOLOGIE_FIELD","MAIN_IPC","INV_NB","INV_NB_PAYS","INV_NB_TYPE","FISRT_INV_COUNTRY","FISRT_INV_TYPE","cited_n","cited_nmiss","cited_age_min","cited_age_median","cited_age_max","cited_age_mean","cited_age_std","SOURCE_CITED_AGE","NB_BACKWARD_NPL","NB_BACKWARD_XY","NB_BACKWARD_I","NB_BACKWARD_AUTRE","NB_BACKWARD_PL","NB_BACKWARD","pct_NB_IPC","pct_NB_IPC_LY","oecd_NB_ROOT_CLASSES","oecd_NB_BACKWARD_PL","oecd_NB_BACKWARD_NPL","IDX_ORIGIN","SOURCE_IDX_ORI","IDX_RADIC","SOURCE_IDX_RAD","PRIORITY_MONTH","FILING_MONTH","PUBLICATION_MONTH","BEGIN_MONTH","VARIABLE_CIBLE"};

    LoadAndSave<float> wrapper;

    wrapper.loadFloat("seriousAll/floatFinal.txt",columns.size());
    wrapper.loadTitles("seriousAll/titles.txt");
    wrapper.chooseColumns(columns);
    wrapper.randomShuffleFloat();
    wrapper.resize(0.2);

    std::cout<<"Basic operations ended"<<std::endl;

    wrapper.loadRaw("../train.csv");
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

    wrapper.saveCurrentFloat("serious/currentConverted.txt");

    std::array<std::string,32> cols = {"VOIE_DEPOT","COUNTRY","APP_NB","APP_NB_PAYS","APP_NB_TYPE","FISRT_APP_COUNTRY","FISRT_APP_TYPE","LANGUAGE_OF_FILLING","NB_CLASSES","NB_SECTORS","NB_FIELDS","TECHNOLOGIE_SECTOR","MAIN_IPC","INV_NB","INV_NB_PAYS","cited_n","cited_age_median","SOURCE_CITED_AGE","NB_BACKWARD_XY","NB_BACKWARD_PL","NB_BACKWARD","pct_NB_IPC","pct_NB_IPC_LY","oecd_NB_ROOT_CLASSES","oecd_NB_BACKWARD_PL","oecd_NB_BACKWARD_NPL","IDX_ORIGIN","SOURCE_IDX_ORI","IDX_RADIC","SOURCE_IDX_RAD","FILING_MONTH","PUBLICATION_MONTH"};

    std::vector<dlib::matrix<float,32,1> > samples;
    std::vector<float> labels;
    wrapper.convertToDlibMatrix<32>(cols,"VARIABLE_CIBLE",samples,labels);

    typedef dlib::matrix<float, 32, 1> sample_type;
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

    std::cout<<"We have now "<<samples.size()<<" samples"<<std::endl;


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


    // Useful to find best parameters

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


    double gamma = 0.00005;
    double nu = 0.78125;
    const double max_nu = maximum_nu(labels);
    std::cout<<"Max nu :"<<max_nu<<std::endl;
    svm_nu_trainer<kernel_type> trainer;
    trainer.set_kernel(kernel_type(gamma));
    trainer.set_nu(nu);

    typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;
    typedef normalized_function<probabilistic_funct_type> pfunct_type;

    pfunct_type learned_pfunct;
    learned_pfunct.normalizer = normalizer;
    learned_pfunct.function = train_probabilistic_decision_function(trainer, samples, labels, 3);
    //deserialize("saved_function.dat")>>learned_pfunct;

    std::cout<<"number of support vectors in our learned_function is "<<learned_pfunct.function.decision_funct.basis_vectors.size()<<std::endl;

    serialize("saved_function.dat")<<learned_pfunct;

    std::cout<<wrapper.getNumberRows()<<std::endl;
    wrapper.resize(10);
    std::cout<<wrapper.getNumberRows()<<std::endl;

    int okTrue = 0, okFalse = 0, FalsePredictedTrue = 0, TruePredictedFalse = 0;
    float floatingScore = 0;
    for(unsigned int i=0;i<wrapper.getNumberRows();i++)
    {
        dlib::matrix<float,32,1> sample;
        int r = wrapper.getRow<32>(i,cols,"VARIABLE_CIBLE",sample);
        double predicted = learned_pfunct(sample);
        if(r==1&&predicted>=0.5)
            okTrue++;
        else if(r==1)
            TruePredictedFalse++;
        else if(predicted>=0.5)
            FalsePredictedTrue++;
        else
            okFalse++;
        floatingScore += fabs(r-predicted);
    }

    std::cout<<"Final score : "<<std::endl;
    std::cout<<okTrue<<"true positive"<<std::endl;
    std::cout<<okFalse<<"true negative"<<std::endl;
    std::cout<<FalsePredictedTrue<<"false positive"<<std::endl;
    std::cout<<TruePredictedFalse<<"false negative"<<std::endl;
    std::cout<<"Floating score : "<<(float)(wrapper.getNumberRows()-floatingScore)/(float)(wrapper.getNumberRows())<<std::endl;
}*/



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
