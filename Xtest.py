import pandas as pd

data = {'current_time': [], 'real_time': [], 'signal_state1_szp': [], 'signal_state2_szp': [], 'signal_state3_szp': [], 'signal_state4_szp': [], 'signal_state4_right_szp':[], 'signal_state5_szp': [], 'signal_state6_szp': [], 'gs1': [], 'rs1': [], 'gs2': [], 'rs2': [], 'gs3': [], 'rs3': [], 'gs4': [], 'rs4': [], 'gs5': [], 'rs5': [], 'gs6': [], 'rs6': [], 'predict_ng': [], 'predict_nr': [], 'nexts': []}
with open('C:/LOGIN_Loop/162/Modell/V1/test1her.txt', 'r') as f:
    for line in f:
        key, value = line.strip('\n').split(':')
        data[key].append(int(value))
        
df = pd.DataFrame(data)
df.drop_duplicates(subset=['current_time'], keep='first', inplace=True)
df.to_excel('C:/LOGIN_Loop/162/Modell/V1/test5her.xlsx', index=False)
