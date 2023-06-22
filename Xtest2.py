import pandas as pd
import numpy as np

# Read the data, only consider the columns 'current_time', 'vehicle_id' and 'current_velocity'
df = pd.read_csv('C:/LOGIN_Loop/162/Modell/V1/test1her.txt', delimiter=':', header=None)
df.columns = ['variable', 'value']
df['value'] = df['value'].astype(float)

# Filter the dataframe for the three variables we are interested in and reset the index
df_filtered = df[df['variable'].isin(['current_time', 'vehicle_id', 'current_velocity'])].copy()
df_filtered.reset_index(drop=True, inplace=True)
#print(df_filtered)

# Make every three lines a group to form a new DataFrame
df_filtered['group'] = df_filtered.index // 3
df_pivot = df_filtered.pivot(index='group', columns='variable', values='value')

#print(df_pivot)

# Sort the DataFrame based on 'vehicle_id' and 'current_time'
df_sorted = df_pivot.sort_values(by=['vehicle_id', 'current_time'])

# Calculate the stop count: when 'current_velocity' is 0 and the previous 'current_velocity' is not 0
df_sorted['stop'] = (df_sorted['current_velocity'] == 0) & (df_sorted.groupby('vehicle_id')['current_velocity'].shift() != 0)

# Calculate the total stop counts for each vehicle
df_stop_counts = df_sorted.groupby('vehicle_id')['stop'].sum().astype(int).reset_index()
df_stop_counts.columns = ['vehicle_id', 'stop_count']

print(df_stop_counts)

# Calculate the total stop count
total_stop_count = df_stop_counts['stop_count'].sum()
print(f"Total stop count: {total_stop_count}")

# Calculate the average stop count
average_stop_count = df_stop_counts['stop_count'].mean()
print(f"Average stop count: {average_stop_count}")