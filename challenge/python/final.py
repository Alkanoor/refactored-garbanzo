#import sys
#sys.path.append("/cal/homes/jschoumacher/workspace/sd/Python/scikit-learn")

import pandas
import numpy as np
import matplotlib.pyplot as plt
from sklearn import preprocessing
from sklearn.utils import shuffle
from sklearn.metrics import roc_auc_score
from sklearn.preprocessing import Imputer
from sklearn.preprocessing import OneHotEncoder
from sklearn.ensemble import RandomForestClassifier,VotingClassifier


data1 = pandas.read_csv("floatCategorizedTest.csv",sep=";")
data2 = pandas.read_csv("floatCategorized.csv",sep=";")


cols = ['VOIE_DEPOT','SOURCE_BEGIN_MONTH','APP_NB',
       'APP_NB_PAYS', 'APP_NB_TYPE', 'FIRST_CLASSE',
       'NB_CLASSES', 'NB_ROOT_CLASSES', 'NB_SECTORS', 'NB_FIELDS',
       'MAIN_IPC', 'INV_NB', 'INV_NB_PAYS', 'INV_NB_TYPE',
       'cited_age_mean','SOURCE_CITED_AGE',
       'NB_BACKWARD_NPL', 'NB_BACKWARD_XY','NB_BACKWARD', 'pct_NB_IPC','oecd_NB_ROOT_CLASSES','oecd_NB_BACKWARD_PL',
       'IDX_ORIGIN', 'SOURCE_IDX_ORI', 'IDX_RADIC',
       'SOURCE_IDX_RAD', 'PUBLICATION_MONTH']

for i in range(1,92):
    cols.append('COUNTRY_'+str(i))

for i in range(1,129):
    cols.append('FISRT_APP_COUNTRY_'+str(i))

for i in range(1,5):
    cols.append('FISRT_APP_TYPE_'+str(i))

for i in range(1,30):
    cols.append('LANGUAGE_OF_FILLING_'+str(i))

for i in range(1,6):
    cols.append('TECHNOLOGIE_SECTOR_'+str(i))

for i in range(1,36):
    cols.append('TECHNOLOGIE_FIELD_'+str(i))

for i in range(1,137):
    cols.append('FISRT_INV_COUNTRY_'+str(i))

for i in range(1,6):
    cols.append('FISRT_INV_TYPE_'+str(i))

print(cols)
print(len(cols))

X1 = data1[cols]
#y1 = data1.VARIABLE_CIBLE_1
X2 = data2[cols]
y2 = data2.VARIABLE_CIBLE_1

scaler = preprocessing.StandardScaler().fit(X1)
X1 = scaler.transform(X1)
scaler = preprocessing.StandardScaler().fit(X2)
X2 = scaler.transform(X2)

#X1,y1 = shuffle(X1,y1)

print(X1.shape)
print(X2.shape)

def createVotingClassifier(n_trees,X,y,depth,min_saples=2,max_feat=0.2,overhead=2.0,voting_='soft'):
    N_data = int(overhead*len(X)/n_trees)
    print(str(N_data)+' will be used by classifier')
    estimators_ = []
    estimators = []
    for i in range(n_trees):
        print(i)
        clf = RandomForestClassifier(n_estimators=80,max_depth=depth,min_samples_leaf=min_saples,max_features=max_feat)
	if (i+1)*N_data<len(X):
        	clf.fit(X[i*N_data:(i+1)*N_data],y[i*N_data:(i+1)*N_data])
	else:
		X,y = shuffle(X,y)
		clf.fit(X[:N_data],y[:N_data])
        estimators_.append((str(i),clf))
        estimators.append(clf)
    tmp = VotingClassifier(estimators=estimators_, voting=voting_)
    tmp.estimators_ = estimators
    return tmp


M = 240000

max_score = 0
for i in range(100):
	X2,y2 = shuffle(X2,y2)

	subdata1 = X2[:M]
	subdata2 = X2[M:]
	y_sub1 = y2[:M]
	y_sub2 = y2[M:]

	myClassifier = createVotingClassifier(13,subdata1,y_sub1,55,5,0.262,7.5)
	y2_pred = myClassifier.predict_proba(subdata2)
	score = 1-roc_auc_score(y_sub2.values,y2_pred[:,0])
	if score>max_score:
		max_score = score
		y_pred = myClassifier.predict_proba(X1)[:, 1]
		np.savetxt('y_pred'+str(score)+'.txt', y_pred, fmt='%s')
		print("Got better score ! "+str(score))
	else:
		print("Got worse score :( "+str(score))
