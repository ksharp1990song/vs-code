import pandas as pd
import numpy as np

data = {'rank': [1,2,3,4],
        'speed': [50.5, 60.6, 70.7, 80.8],
        'acceleration': ['dsad', 'dada', 'dada', 'adad'],
        }
df = pd.DataFrame(data)
nparr = np.array(df)
