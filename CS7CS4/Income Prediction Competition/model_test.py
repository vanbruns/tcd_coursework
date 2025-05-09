import data_processing as dp
import data_clean as dc

import numpy as np
import pandas as pd
from sklearn.neural_network import MLPRegressor
from sklearn.linear_model import LinearRegression
from sklearn.linear_model import SGDRegressor
from sklearn.linear_model import ElasticNet
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import mean_squared_error
from sklearn.svm import LinearSVR
from sklearn.preprocessing import StandardScaler

def main():
# Load the training dataset
    training = pd.read_csv('tcd ml 2019-20 income prediction training (with labels).csv')
    
    # Clean training data
    training = dc.clean(training)
    
    # Format training data
    training = dp.shape(training)
    
    model_columns = range(1, len(training.columns) - 2)
    
    X = training.iloc[:,model_columns]
    y = training.iloc[:,-1]
    
    scaler=StandardScaler()
    
    X=scaler.fit_transform(X,y)
    
    regression=LinearRegression()
    regression.fit(X,y)
    first_model=(mean_squared_error(y_true=y,y_pred=regression.predict(X)))
    print(first_model)

#    lm=ElasticNet(normalize=True, max_iter=5000)
#    lm=SGDRegressor(max_iter=5000, penalty='elasticnet', tol=1e-3)
    lm=MLPRegressor(hidden_layer_sizes=(5,),
#                        alpha=0.001,
                                       activation='relu',
                                       solver='adam',
                                       learning_rate='constant',
                                       max_iter=10000,
#                                       n_iter_no_change=10000,
                                       learning_rate_init=0.01)
#    lm=LinearSVR(epsilon=0.0, C=0.5, tol=1e-4, max_iter=5000)
    search=GridSearchCV(estimator=lm,param_grid={
#                                                    'C':np.linspace(0.5,1,5)
                                                    'alpha':np.logspace(-10,1,10)
#                                                    ,'l1_ratio':np.linspace(.3,.7,5)
                                                      },
                                                  scoring='neg_mean_squared_error',n_jobs=1,refit=True,cv=10)
    
    search.fit(X,y)
#    first_model=(mean_squared_error(y_true=y,y_pred=lm.predict(X)))
#    print(first_model)
    print(search.best_params_)
    print(abs(search.best_score_))
    
main()