import pandas as pd

data = {'current_time': [], 'vehicle_id': [],'signal_distance': [], 'signal_route_distance': [],'signal_state4': [], 'signal_route_state4': [], 'current_velocity': [], 'current_acceleration': []}
with open('C:/LOGIN_Loop/162/Modell/V1/test1her.txt', 'r') as f:
    for line in f:
        key, value = line.strip('\n').split(':')
        data[key].append(value)
        
df = pd.DataFrame(data)
df['current_time'] = df['current_time'].astype(int)
#df['real_time'] = df['real_time'].astype(int)
df['vehicle_id'] = df['vehicle_id'].astype(int)
df['signal_distance'] = df['signal_distance'].astype(float)
#df['signal_distance1'] = df['signal_distance1'].astype(float)
df['signal_route_distance'] = df['signal_route_distance'].astype(float)
df['signal_state4'] = df['signal_state4'].astype(int)
df['signal_route_state4'] = df['signal_route_state4'].astype(int)
#df['signal_route_distance1'] = df['signal_route_distance1'].astype(float)
df['current_velocity'] = df['current_velocity'].astype(float)
#df['desired_velocity'] = df['desired_velocity'].astype(float)
df['current_acceleration'] = df['current_acceleration'].astype(float)
#df.drop_duplicates(subset=['current_time'], keep='first', inplace=True)
df.to_excel('C:/LOGIN_Loop/162/Modell/V1/test4her.xlsx', index=False)