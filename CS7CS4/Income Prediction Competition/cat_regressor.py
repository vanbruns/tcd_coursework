# This is the one I ultimately used, not linear_regression.

import data_processing as dp
import data_clean as dc

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from catboost import CatBoostRegressor

def main():
    # Load the training dataset
    training = pd.read_csv('tcd ml 2019-20 income prediction training (with labels).csv')
    
    # Clean training data
    training = dc.clean(training)
    
    # Format training data
    training = dp.shape(training)
    
    # Split training into its different parts
    X_train, X_test, y_train, y_test = train_test_split(training.drop(columns = ['Instance', 'Income in EUR'], axis = 1),
                                                        training['Income in EUR'],
                                                        test_size=0.1,
                                                        shuffle=True)
    
    categorical_features_indices = np.where(X_train.dtypes != np.float)[0]
    
    model=CatBoostRegressor(iterations=5000,
                            depth=6,
                            learning_rate=0.15,
                            loss_function='RMSE',
                            random_seed = 37,
                            od_type='Iter',
                            metric_period = 50,
                            od_wait=200,
                            use_best_model=True,
                            task_type='GPU')
    model.fit(X_train, y_train,cat_features=categorical_features_indices,eval_set=(X_test, y_test),plot=True)
    
    print(model.score(X_test, y_test))
    
    fea_imp = pd.DataFrame({'imp': model.feature_importances_, 'col': X_train.columns})
    fea_imp = fea_imp.sort_values(['imp', 'col'], ascending=[True, False]).iloc[-30:]
    fea_imp.plot(kind='barh', x='col', y='imp', figsize=(10, 7), legend=None)
    plt.title('CatBoost - Feature Importance')
    plt.ylabel('Features')
    plt.xlabel('Importance');
        
    
    # Load the prediction dataset
    test = pd.read_csv('tcd ml 2019-20 income prediction test (without labels).csv')
    
    test = dc.clean(test, drop=False)
    
    #Format test data
    test = dp.shape(test)
    
    # Make predictions using the test set
    y_predict = model.predict(test.iloc[:,1:-1])
    
    print(y_predict)
    
    test['Income'] = y_predict
    
    # Write prediction to CSV
    pred_out = pd.read_csv('tcd ml 2019-20 income prediction submission file.csv')
    pred_out['Income'] = y_predict
    pred_out.to_csv('tcd ml 2019-20 income prediction submission file.csv', index=False)
    
    print(pd.DataFrame(pred_out['Income'], columns={'Income'}).describe())

main()