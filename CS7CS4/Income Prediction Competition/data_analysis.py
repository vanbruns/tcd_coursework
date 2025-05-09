import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import data_processing as dp
import data_clean as dc
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import chi2
from sklearn.ensemble import ExtraTreesClassifier
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.stats import boxcox
from sklearn.preprocessing import QuantileTransformer
import math

def uniqueAnalysis(col_name, data_frame):
    print('\'' + col_name + '\' value count: ', training[col_name].nunique())
    print('\'' + col_name + '\' unique values: ')
    
    u_vals = pd.DataFrame(training[col_name].unique())
    
    print(u_vals)
    print(u_vals.isnull().values.any())
    print('')

# Load the training dataset
training = pd.read_csv('tcd ml 2019-20 income prediction training (with labels).csv')

# Look at uniqueness of columns
#for column_name in training.columns[1:11]:
#    uniqueAnalysis(column_name, training)

# After looking at uniqueness, we know this:
# 1. 'Country', 'Size of City', 'Wears Glasses', and 'Body Height [cm]' are complete columns (without NaNs), but set defaults anyway
# 2. Though not complete, 'Gender' NaNs can be put into the 'unknown' category
# 3. Though not complete, 'Hair Color' NaNs can be put into the 'Unknown' category
# 4. The rest of the columns with NaNs will just have to use some best guess for replacing NaNs
# 5. Since 'Country' is complete, might be worth adding a regions column for checking whether there's correlation
# 6. Should look at distribution of 'Body Height [cm]

plt.style.use('ggplot')

training = dc.clean(training)
#training = dp.scaleOutput(training, 'Income in EUR')
training = dp.shape(training)

#print('Year of Record ' + str(len(training.index[training['Year of Record'].isna()])))
#print('Gender ' + str(len(training.index[training['Gender'].isna()])))
#print('Age ' + str(len(training.index[training['Age'].isna()])))
#print('Country ' + str(len(training.index[training['Country'].isna()])))
#print('Size of City ' + str(len(training.index[training['Size of City'].isna()])))
#print('Profession ' + str(len(training.index[training['Profession'].isna()])))
#print('University Degree ' + str(len(training.index[training['University Degree'].isna()])))
#print('Wears Glasses ' + str(len(training.index[training['Wears Glasses'].isna()])))
#print('Hair Color ' + str(len(training.index[training['Hair Color'].isna()])))
#print('Body Height [cm] ' + str(len(training.index[training['Body Height [cm]'].isna()])))
#print('Income in EUR ' + str(len(training.index[training['Income in EUR'].isna()])))

#training = dp.shape(training)

#for i in range(len(training.columns)):
#    print(training.iloc[:,i].describe())

# power transform
#training['Year of Record'] = boxcox(training['Size of City'], 0)

# quantile transform
#qt = QuantileTransformer(n_quantiles=10, random_state=0)
#training.iloc[:,[1,3,5,10,12]] = qt.fit_transform(training.iloc[:,[1,3,5,10,12]]) 

# Histogram of the height
#for i in [1,3,5,10,12]:
#for i in [13, 14, 15, 16]:
#    training.iloc[:,i].plot(kind='hist',color='purple',edgecolor='black',figsize=(10,7))
#    plt.title('Distribution', size=24)
#    plt.xlabel(training.columns[i], size=18)
#    plt.ylabel('Frequency', size=18)
#    plt.show()

# Histogram of the height
#training['Body Height [cm]'].plot(kind='hist',color='purple',edgecolor='black',figsize=(10,7))
#plt.title('Distribution of Height', size=24)
#plt.xlabel('Height (cm)', size=18)
#plt.ylabel('Frequency', size=18)

# After looking at distribution, we know this:
# 1. Normal distribution

#training[training['Gender']=='male']['Body Height [cm]'].plot(kind='hist',color='blue',edgecolor='black',alpha=0.5,figsize=(10,7))
#training[training['Gender']=='female']['Body Height [cm]'].plot(kind='hist',color='magenta',edgecolor='black',alpha=0.5,figsize=(10,7))
##training[training['Gender']=='other']['Body Height [cm]'].plot(kind='hist',color='orange',edgecolor='black',alpha=0.5,figsize=(10,7))
##training[training['Gender']=='unknown']['Body Height [cm]'].plot(kind='hist',color='brown',edgecolor='black',alpha=0.5,figsize=(10,7))
##training[training['Gender']=='0']['Body Height [cm]'].plot(kind='hist',color='red',edgecolor='black',alpha=0.5,figsize=(10,7))
#plt.legend(labels=['Males','Females'])
#plt.title('Distribution of Height', size=24)
#plt.xlabel('Height (inches)', size=18)
#plt.ylabel('Frequency', size=18);

#training[training['Gender']=='male']['Income in EUR'].plot(kind='hist',color='blue',edgecolor='black',alpha=0.5,figsize=(10,7))
#training[training['Gender']=='female']['Income in EUR'].plot(kind='hist',color='magenta',edgecolor='black',alpha=0.5,figsize=(10,7))
##training[training['Gender']=='other']['Income in EUR'].plot(kind='hist',color='orange',edgecolor='black',alpha=0.5,figsize=(10,7))
##training[training['Gender']=='unknown']['Income in EUR'].plot(kind='hist',color='brown',edgecolor='black',alpha=0.5,figsize=(10,7))
##training[training['Gender']=='0']['Income in EUR'].plot(kind='hist',color='red',edgecolor='black',alpha=0.5,figsize=(10,7))
#plt.legend(labels=['Males','Females'])
#plt.title('Distribution of Income', size=24)
#plt.xlabel('Income (EUR)', size=18)
#plt.ylabel('Frequency', size=18);

# After looking at 'Gender' to 'Height' and 'Income in EUR', we know this:
# 1. Non-male and non-female genders have heights uniformly distributed, and will possibly be combined into one category
# 2. Males and females are normal distributions, with a smaller set of females and slightly shorter, but the spread is similar
# 3. All income frequency seems to follow Lognormal

## Descriptive statistics male
#statistics_male = training[training['Gender']=='male'].describe()
#statistics_male.rename(columns=lambda x:x+'_male',inplace=True)
#
## Descriptive statistics female
#statistics_female = training[training['Gender']=='female'].describe()
#statistics_female.rename(columns=lambda x:x+'_female',inplace=True)
#
#with pd.option_context("display.max_columns", 7):
#    print(statistics_male)
#    print(statistics_female)

#ax1= training[training['Gender']=='male'].plot(kind='scatter', x='Body Height [cm]',y='Income in EUR', color='blue',alpha=0.5, figsize=(10,7))
#training[training['Gender']=='female'].plot(kind='scatter', x='Body Height [cm]',y='Income in EUR', color='magenta',alpha=0.5, figsize=(10,7),ax=ax1)
#plt.legend(labels=['Males','Females'])
#plt.title('Relationship between Height and Income', size=24)
#plt.xlabel('Height (cm)', size=18)
#plt.ylabel('Income (EUR)', size=18);

#with pd.option_context("display.max_columns", 14):
#    print(training)

#with pd.option_context("display.max_columns", 14):
#    print(training[training['Gender']=='male'].corr())
#    print(training[training['Gender']=='female'].corr())
#    print(training[training['Gender']=='other'].corr())
#    print(training[training['Gender']=='unknown'].corr())
#    print(training[training['Gender']=='0'].corr())

#X = training.iloc[:,1:12]
#y = training.iloc[:,-1]
#y = pd.DataFrame(data=y, columns=['Income in EUR'])

#X = X[:15000]
#y = y[:15000]

##apply SelectKBest class to extract top 10 best features
#bestfeatures = SelectKBest(score_func=chi2, k=10)
#fit = bestfeatures.fit(X_encoded,y_encoded)
#dfscores = pd.DataFrame(fit.scores_)
#dfcolumns = pd.DataFrame(X_encoded.columns)
##concat two dataframes for better visualization 
#featureScores = pd.concat([dfcolumns,dfscores],axis=1)
#featureScores.columns = ['Specs','Score']  #naming the dataframe columns
#print(featureScores.nlargest(10,'Score'))  #print 10 best features

#model = ExtraTreesClassifier()
#model.fit(X_encoded,y_encoded)
#print(model.feature_importances_) #use inbuilt class feature_importances of tree based classifiers
##plot graph of feature importances for better visualization
#feat_importances = pd.Series(model.feature_importances_, index=X_encoded.columns)
#feat_importances.nlargest(10).plot(kind='barh')
#plt.show()

#indices = training.index[~training['Income in EUR'].isna()]
#
#for i in indices:
#    print(training['Income in EUR'][i])
#    training['Income in EUR'][i] = math.exp(training['Income in EUR'][i])

#get correlations of each features in dataset
#corrmat = training[:,1:20].corr()
#top_corr_features = corrmat.index
plt.figure(figsize=(30,30))
#plot heat map
g=sns.heatmap(training.corr(),annot=True,cmap="RdYlGn")

#corr = training.corr()
#corr = pd.DataFrame(corr)
#corr['Income in EUR'] = corr['Income in EUR'].apply(np.abs)
#print(corr.sort_values(by='Income in EUR'))
#print('hello')

#'Year of Record':np.amin(dataFrame['Year of Record']) - 1,
#'Gender':'unknown',
#'Age':np.amin(dataFrame['Age']) - 1,
#'Country':'Unknown',
#'Size of City':0,
#'Profession':'\b',
#'University Degree':'\b',
#'Wears Glasses':2,
#'Hair Color':'Unknown',
#'Body Height [cm]':0
#'Income in EUR'