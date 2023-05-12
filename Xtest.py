import pandas as pd

data = {'current_time': [], 'signal_state1': [], 'gs': [], 'rs': []}

with open('D:/Simulationsmodell HERCULES/V0/test1her.txt', 'r') as f:
    for line in f:
        key, value = line.strip('\n').split(':')
        data[key].append(int(value))
        
df = pd.DataFrame(data)
df.drop_duplicates(subset=['current_time'], keep='first', inplace=True)
df.to_excel('D:/Simulationsmodell HERCULES/V0/test2her.xlsx', index=False)
