import pandas as pd
import numpy as np

data = {'rank': [1.1,2.2,3.3,4.4],
        'speed': [50.5, 60.6, 70.7, 80.8],
        'acceleration': [1, 2, 2, 3],
        }
df = pd.DataFrame(data)
nparr = np.array(df)