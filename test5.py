import pandas as pd
import importlib
import sys

df = pd.DataFrame(columns=['x1', 'x2'])


def process_dataframe(df):
    global x3, x4
    print(df)
    csv_data = df.to_csv(index=False, sep=';')
    with open('df_erweitert_Test.txt', 'w') as f:
        f.write(csv_data)

    x3 = df.at[0, 'x1']
    x4 = df.at[0, 'x2']
    
    print('x3 =', x3)
    print('x4 =', x4)

