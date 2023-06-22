import pandas as pd

data = {'current_time': [], 'predict_ng': [], 'predict_nr': [], 'vehicle_id': [],'signal_distance': [], 'current_velocity': [], 'desired_velocity': [], 'current_velocity_after': [], 'desired_velocity_after': []}
with open('D:/Simulationsmodell HERCULES/V0/test1her.txt', 'r') as f:
    for line in f:
        key, value = line.strip('\n').split(':')
        data[key].append(value)
        
df = pd.DataFrame(data)
df['current_time'] = df['current_time'].astype(int)
df['predict_ng'] = df['predict_ng'].astype(int)
df['predict_nr'] = df['predict_nr'].astype(int)
df['vehicle_id'] = df['vehicle_id'].astype(int)
df['signal_distance'] = df['signal_distance'].astype(float)
df['current_velocity'] = df['current_velocity'].astype(float)
df['desired_velocity'] = df['desired_velocity'].astype(float)
df['current_velocity_after'] = df['current_velocity_after'].astype(float)
df['desired_velocity_after'] = df['desired_velocity_after'].astype(float)
#df.drop_duplicates(subset=['current_time'], keep='first', inplace=True)
df.to_excel('D:/Simulationsmodell HERCULES/V0/test6her.xlsx', index=False)
