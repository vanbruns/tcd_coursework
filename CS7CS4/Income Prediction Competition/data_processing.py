import numpy as np
import pandas as pd
import country_converter as coco

def addContinentAndRegionColumn(data_frame):
    country_names, set_indices = np.unique(data_frame['Country'], return_inverse = True)
    
    name_count = len(country_names)
    
    continents = np.full(name_count, '       ')
    regions = np.full(name_count, '                         ')
    
    for i in range(name_count):
        continents[i] = coco.convert(names = country_names[i], to = 'continent')
        regions[i] = coco.convert(names = country_names[i], to = 'UNregion')
    
    data_frame.insert(5, 'Continent', continents[set_indices])
    data_frame.insert(5, 'Region', regions[set_indices])
    
    return data_frame

def addProfessionTypeColumn(data_frame):
    profession = data_frame['Profession']
    
    indices = profession.index[~profession.isna() & profession.str[-1:].str.isnumeric()]
    
    profession[indices] = profession[indices].str[:-2]
    
    splits = profession.str.split(' ')
    indices = splits.index[~splits.isna()]
    last_token = splits
    
    for i in indices:
        last_token[i] = splits.loc[i][-1]
    
    data_frame.insert(7, 'Profession Type', last_token)
    
    return data_frame

def addProfessionTypeColumns(data_frame, columns):
    for column in columns:
        data_frame.insert(7, 'Profession Type_' + column, False)
        indices = data_frame.index[data_frame['Profession'].str.endswith(column)]
        data_frame.loc[indices]['Profession Type_' + column] = True
    
    return data_frame

def addScaledIncomeColumns(data_frame):
    data_frame['Income_log'] = np.log(data_frame['Income in EUR'])
#    data_frame['Income_sqrt'] = np.sqrt(data_frame['Income in EUR'])
#    data_frame['Income_pow2'] = np.power(data_frame['Income in EUR'], 2)
    
    return data_frame

def encodeColumn(data_frame, column_name, index, values=[]):
    dummies = pd.get_dummies(data_frame[column_name], prefix=column_name)
    
    # When values, select dummies
    if values:
        for i in range(len(values)):
            values[i] = column_name + '_' + values[i]
        values.reverse()
        dummies = dummies[values]
    
    for i in range(len(dummies.columns)):
        data_frame.insert(index, dummies.columns[i], dummies.iloc[:,i])
    
    return data_frame

def scaleInput(data_frame):
    data_frame['Size of City'] = data_frame['Size of City'].apply(np.log)
    data_frame['Body Height [cm]'] = data_frame['Body Height [cm]'].apply(np.log)
    
    return data_frame

def scaleOutput(data_frame, column_name, direction='pre'):
    if (direction == 'pre'):
        data_frame[column_name] = data_frame[column_name].apply(np.log)
    elif (direction == 'post'):
        data_frame[column_name] = data_frame[column_name].apply(np.exp)
    else:
        print('Warning: Direction for scaling not recognized. Scaling ignored.')
    
    return data_frame

def shape(data_frame):
#    # Profession type encoding (adding just before income)
#    data_frame = addProfessionTypeColumn(data_frame)
#    data_frame = encodeColumn(data_frame, 'Profession Type', 12)
#    data_frame = addProfessionTypeColumns(data_frame, ['historian',
#                                                       'technician',
#                                                       'assistant',
#                                                       'pair',
#                                                       'trapper',
#                                                       'anaesthetist',
#                                                       'therapist'])
    
    # Hair color encoding, selecting which columns we want
#    data_frame = encodeColumn(data_frame, 'Hair Color', 10, [
#                                                            'Black',
##                                                            'Blond',
##                                                            'Brown',
##                                                            'Red',
##                                                            'Unknown',
#                                                            '0'
#                                                            ]) #0
    
    # University degree encoding, selection which columns we want
#    data_frame = encodeColumn(data_frame, 'University Degree', 8, [
##                                                                'Bachelor',
#                                                                'Master',
##                                                                'No',
##                                                                'PhD',
##                                                                'Unknown',
#                                                                '0'
#                                                                ]) #Master, 0
    
    # Country, region, and continent encoding
    data_frame = addContinentAndRegionColumn(data_frame)
    
    # Gender encoding, selecting which columns we want
#    data_frame = encodeColumn(data_frame, 'Gender', 3, [
#                                                        'female',
#                                                        'male',
#                                                        'other',
##                                                        'unknown',
#                                                        '0'
#                                                        ]) #male, 0
    
#    if 'Income in EUR' in data_frame.columns:
#        data_frame = scaleOutput(data_frame, 'Income in EUR')
    
    
    data_frame = scaleInput(data_frame)
    
    return data_frame

