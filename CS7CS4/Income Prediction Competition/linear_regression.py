import data_processing as dp
import data_clean as dc

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.svm import LinearSVR
from sklearn import linear_model
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import train_test_split
from sklearn.model_selection import KFold
from sklearn.decomposition import PCA
from sklearn.metrics import mean_squared_error, r2_score, accuracy_score
from sklearn.preprocessing import QuantileTransformer
from sklearn.linear_model import ElasticNet
from sklearn.neural_network import MLPRegressor

def realizePrediction(prediction, training):
    min_income = np.amin(training)
    
    neg_pred = np.where(prediction < 0)
    prediction[neg_pred] = min_income
    
    return prediction

def plot(training, predicting):
    training.plot(kind='scatter',x='Year of Record',y='Income in EUR',color='black')
    plt.show()
    predicting.plot(kind='scatter',x='Year of Record',y='Income',color='black')
    plt.show()
    training.groupby('Gender')['Income in EUR'].mean().plot(kind='bar')
    plt.show()
    predicting.groupby('Gender')['Income'].mean().plot(kind='bar')
    plt.show()
    training.plot(kind='scatter',x='Age',y='Income in EUR',color='black')
    plt.show()
    predicting.plot(kind='scatter',x='Age',y='Income',color='black')
    plt.show()
    training.groupby('Country')['Income in EUR'].mean().plot(kind='bar')
    plt.show()
    predicting.groupby('Country')['Income'].mean().plot(kind='bar')
    plt.show()
    training.groupby('Continent')['Income in EUR'].mean().plot(kind='bar')
    plt.show()
    predicting.groupby('Continent')['Income'].mean().plot(kind='bar')
    plt.show()
    training.groupby('Region')['Income in EUR'].mean().plot(kind='bar')
    plt.show()
    predicting.groupby('Region')['Income'].mean().plot(kind='bar')
    plt.show()
    training.plot(kind='scatter',x='Size of City',y='Income in EUR',color='black')
    plt.show()
    predicting.plot(kind='scatter',x='Size of City',y='Income',color='black')
    plt.show()
    training.groupby('Profession')['Income in EUR'].mean().plot(kind='bar')
    plt.show()
    predicting.groupby('Profession')['Income'].mean().plot(kind='bar')
    plt.show()
    training.groupby('University Degree')['Income in EUR'].nunique().plot(kind='bar')
    plt.show()
    predicting.groupby('University Degree')['Income'].nunique().plot(kind='bar')
    plt.show()
    training.plot(kind='scatter',x='Wears Glasses',y='Income in EUR',color='black')
    plt.show()
    predicting.plot(kind='scatter',x='Wears Glasses',y='Income',color='black')
    plt.show()
    training.groupby('Hair Color')['Income in EUR'].mean().plot(kind='bar')
    plt.show()
    predicting.groupby('Hair Color')['Income'].mean().plot(kind='bar')
    plt.show()
    training.plot(kind='scatter',x='Body Height [cm]',y='Income in EUR',color='black')
    plt.show()
    predicting.plot(kind='scatter',x='Body Height [cm]',y='Income',color='black')
    plt.show()

def main():
    # Load the training dataset
    training = pd.read_csv('tcd ml 2019-20 income prediction training (with labels).csv')
    
    # Clean training data
    training = dc.clean(training)
    
    # Format training data
    training = dp.shape(training)
    
#    training.to_csv('training_observe.csv', index=False)
    
#    # Split training into its different parts
#    X_train, X_test, y_train, y_test = train_test_split(training.drop(columns = ['Instance', 'Income in EUR'], axis = 1),
#                                                        training['Income in EUR'],
#                                                        test_size=0.2,
#                                                        stratify=y)
    
    model_columns = range(1, len(training.columns) - 2)
#    model_columns = [1,3,4,6,7,8]
    
    # Create linear regression object
#    lm = linear_model.LinearRegression()
#    lm = LinearSVR(tol=1e-4, max_iter=1000)
#    lm = linear_model.SGDRegressor(max_iter=10000, tol=1e-6)
#    lm = ElasticNet(alpha=0.3, l1_ratio=0.4, tol=1e-4, max_iter=10000)
    lm=MLPRegressor(hidden_layer_sizes=(5,),
                        alpha=0.6,
                                       activation='relu',
                                       solver='adam',
                                       learning_rate='constant',
                                       max_iter=10000,
#                                       n_iter_no_change=10000,
                                       learning_rate_init=0.01)
    
    # Use cross validation
    kFold = KFold(10, True, 1)
    
#    # Use principle component analysis
#    pca = PCA(n_components = 4)
    
    # Enumerate KFold splits
    for training_train, training_test in kFold.split(training):
        # Complete split
        X_train = training.iloc[training_train, model_columns]
        X_test = training.iloc[training_test, model_columns]
        y_train = training.iloc[training_train, -1]
        y_test = training.iloc[training_test, -1]
         
#        # Transform with PCA
#        X_train = pca.fit_transform(X_train)  
#        X_test = pca.transform(X_test)
#        
#        explained_variance = pca.explained_variance_ratio_ 
#        for i in explained_variance:
#            print(format(i*100, 'f'))
        
        # Train the model using the training sets
        lm.fit(X_train, y_train)
        
        # Make predictions using the testing set
        y_predict = lm.predict(X_test)
        
#        y_test = pd.DataFrame(y_test, columns= {'Income in EUR'})
#        y_predict = pd.DataFrame(y_predict, columns= {'Income in EUR'})
#        
#        y_test = dp.scaleOutput(y_test, 'Income in EUR', 'post')
#        y_predict = dp.scaleOutput(y_predict, 'Income in EUR', 'post')
        
        # Make values more realistic
#        y_predict = realizePrediction(y_predict, y_train)
        
        print(lm.score(X_test, y_test))
        
        # The coefficients    
#        print('Coefficients: \n', lm.coef_)
        # The mean squared error
        print("Mean squared error: %.2f"
              % mean_squared_error(y_test, y_predict))
        # Explained variance score: 1 is perfect prediction
        print('Variance score: %.2f' % r2_score(y_test, y_predict))
        print(' ')
    
    # Load the prediction dataset
    test = pd.read_csv('tcd ml 2019-20 income prediction test (without labels).csv')
    
    test = dc.clean(test, drop=False)
    
    #Format test data
    test = dp.shape(test)
    
    # Make predictions using the test set
    y_predict = lm.predict(test.iloc[:,model_columns])
    
    print(y_predict)
    
    test['Income'] = y_predict
    
#    realizePrediction(y_predict, training['Income in EUR'])
    
#    plot(training, test)
    
    # Write prediction to CSV
    pred_out = pd.read_csv('tcd ml 2019-20 income prediction submission file.csv')
    pred_out['Income'] = y_predict
#    pred_out = dp.scaleOutput(pred_out, 'Income', 'post')
    pred_out.to_csv('tcd ml 2019-20 income prediction submission file.csv', index=False)
    
    print(pd.DataFrame(pred_out['Income'], columns={'Income'}).describe())

main()