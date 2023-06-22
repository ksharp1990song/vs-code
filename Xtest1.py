import pandas as pd

data = {'current_time': [], 'vehicle_id': [],'current_velocity': [], 'desired_velocity': [], 'current_acceleration': []}
with open('C:/LOGIN_Loop/162/Modell/V1/test1her.txt', 'r') as f:
    for line in f:
        key, value = line.strip('\n').split(':')
        data[key].append(value)
        
df = pd.DataFrame(data)
df['current_time'] = df['current_time'].astype(int)
df['vehicle_id'] = df['vehicle_id'].astype(int)
df['current_velocity'] = df['current_velocity'].astype(float)
df['desired_velocity'] = df['desired_velocity'].astype(float)
df['current_acceleration'] = df['current_acceleration'].astype(float)
#df['predict_ng'] = df['predict_ng'].astype(int)
#df['predict_nr'] = df['predict_nr'].astype(int)
#df['current_velocity_after'] = df['current_velocity_after'].astype(float)
#df['desired_velocity_after'] = df['desired_velocity_after'].astype(float)
#df['current_acceleration_after'] = df['current_acceleration_after'].astype(float)
#df.drop_duplicates(subset=['current_time'], keep='first', inplace=True)
df.to_excel('C:/LOGIN_Loop/162/Modell/V1/test19her.xlsx', index=False)
