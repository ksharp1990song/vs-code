import pandas as pd
import numpy as np

with open("C:/LOGIN_Loop/162/Modell/V1/test1her.txt", "r") as file:
    lines = file.readlines()
with open("C:/LOGIN_Loop/162/Modell/V1/test2her.txt", "w") as file:
    for line in lines[20485:]:
        file.write(line)