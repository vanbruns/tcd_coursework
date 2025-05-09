import numpy as np
import roman_numerals as rn

def cleanProfessionColumn(profession):
    # Drop NaNs
    profession.dropna(inplace=True)
    
    # Truncate space
    profession = profession.str.strip()
    
    # Lowercase
    profession = profession.str.lower()
    
    # 'woodworker' to 'wood worker'
    indices = profession.index[profession.str.match('woodworker')]
    
    profession[indices] = 'wood worker'
    
    # Transform ' of '
    indices = profession.index[profession.str.contains(' of ')]
    
    splits = profession[indices].str.split(' of ', n=1)
    
    for i in indices:
        splits[i].reverse()
        splits[i] = ' '.join(splits[i])
    
    profession[indices] = splits[indices]
    
    # Transform 'sr.' to 'senior'
    indices = profession.index[profession.str.contains('sr.')]
    
    splits = profession[indices].str.split('sr.', n=1)
    
    for i in indices:
        splits[i] = 'senior'.join(splits[i])
    
    profession[indices] = splits[indices]
    
    # Transform ' level ' to ' '
    indices = profession.index[profession.str.contains(' level ')]
    
    splits = profession[indices].str.split(' level ', n=1)
    
    for i in indices:
        splits[i] = ' '.join(splits[i])
    
    profession[indices] = splits[indices]
    
    ## Transform roman numerals to numbers (1-10)
    for i in range(1,10):
        numeral = ' ' + rn.int_to_roman_numeral(i).lower()
        indices = profession.index[profession.str.endswith(numeral)]
        
        splits = profession[indices].str.split(numeral, n=1)
        
        for j in indices:
            splits[j] = splits[j][0] + ' ' + str(i)
        
        profession[indices] = splits[indices]
    
    # Truncate 's'
    indices = profession.index[profession.str.endswith('s')
                            & ~profession.str.endswith('ss')
                            & ~profession.str.match('human resources')
                            & ~profession.str.match('arts')
                            & ~profession.str.match('occupations')]  
    
    profession[indices] = profession[indices].str[:-1]
    
    # analysttimekeeper
    # claim to claims
    # scientistscientist
    
    return profession

def cleanGender(gender):
    # Drop NaNs
    gender.dropna(inplace=True)
    
    # '0' to 'unknown'
    indices = gender.index[gender.str.match('0')]
    
    gender[indices] = 'unknown'
    
    # 'other' to 'unknown'
    indices = gender.index[gender.str.match('other')]
    
    gender[indices] = 'unknown'
    
    return gender

def cleanHairColor(hair_color):
    # Drop NaNs
    hair_color.dropna(inplace=True)
    
    # '0' to 'Unknown'
    indices = hair_color.index[hair_color.str.match('0')]
    
    hair_color[indices] = 'Unknown'
    
    return hair_color

def cleanUniversityDegree(university_degree):
    # Drop NaNs
    university_degree.dropna(inplace=True)
    
    # '0' to 'No'
    indices = university_degree.index[university_degree.str.match('0')]
    
    university_degree[indices] = 'No'
    
    return university_degree

def cleanColumns(data_frame):
    data_frame['Gender'] = cleanGender(data_frame['Gender'])
    data_frame['Hair Color'] = cleanHairColor(data_frame['Hair Color'])
    data_frame['University Degree'] = cleanUniversityDegree(data_frame['University Degree'])
    data_frame['Profession'] = cleanProfessionColumn(data_frame['Profession'])
    
    return data_frame

def boxOutliersToCSV(data_frame, column_name):
    quantile25 = data_frame[column_name].quantile(0.25)
    quantile75 = data_frame[column_name].quantile(0.75)
    
    IQR = quantile75 - quantile25
    upper = quantile75 + 1.5 * IQR
    lower = quantile25 - 1.5 * IQR
    
    data_frame_upper = data_frame[data_frame[column_name] > upper]
    data_frame_lower = data_frame[data_frame[column_name] < lower]
    
    data_frame_upper.to_csv(column_name + '_large.csv', index=False)
    data_frame_lower.to_csv(column_name + '_small.csv', index=False)

def boxDropRowsForColumn(data_frame, column_name):
    quantile25 = data_frame[column_name].quantile(0.25)
    quantile75 = data_frame[column_name].quantile(0.75)
    
    IQR = quantile75 - quantile25
    upper = quantile75 + 1.5 * IQR
    lower = quantile25 - 1.5 * IQR
    
    indices_upper = data_frame.index[data_frame[column_name] > upper]
    indices_lower = data_frame.index[data_frame[column_name] < lower]
    
    data_frame = data_frame.drop(indices_upper)
    data_frame = data_frame.drop(indices_lower)
    
    return data_frame

def replaceNaNs(data_frame):    
    # Replace NaN throughout, giving specific defaults for each column
    data_frame = data_frame.fillna(value={
            # Set NaNs to one year before min, as a best guess
            'Year of Record':np.amin(data_frame['Year of Record']) - 1,
            # Set NaNs to the 'unknown' group, which already exists
            'Gender':'unknown',
            # Set NaNs to average age, as a best guess
            'Age':np.average(data_frame['Age']),
            # Set NaNs to 'unknown', following lower case trend
            'Profession':'unknown',
            # Set NaNs to 'Unknown', following upper case trend
            'University Degree':'Unknown',
            # Set NaNs to the 'Unknown' group, which already exists
            'Hair Color':'Unknown',
            })
    
    return data_frame

def clean(data_frame, drop=True):
    data_frame = cleanColumns(data_frame)
    
    if (drop):
        # 647 rows from training
        data_frame = boxDropRowsForColumn(data_frame, 'Age')
        # 158 additional rows from training
        data_frame = boxDropRowsForColumn(data_frame, 'Body Height [cm]')
        
        # 437 additional rows from training
        data_frame = data_frame.drop(data_frame.index[data_frame['Year of Record'].isna()])
        # 491 additional rows from training
        data_frame = data_frame.drop(data_frame.index[data_frame['Age'].isna()])
        # 319 additional rows from training
        data_frame = data_frame.drop(data_frame.index[data_frame['Profession'].isna()])
        
        # Only attempt to drop rows when column exists, which will be for training
        if 'Income in EUR' in data_frame.columns:
            # 162 additional rows from training
            data_frame = data_frame.drop(data_frame.index[data_frame['Income in EUR'] < 0])
    
    data_frame = replaceNaNs(data_frame)
    
    return data_frame