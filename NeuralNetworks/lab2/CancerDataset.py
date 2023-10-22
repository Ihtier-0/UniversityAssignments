from torch.utils.data import Dataset, DataLoader

import pandas as pd
import numpy as np
import torch

class CancerDataset(Dataset):
    def __init__(self, path = 'data.csv'):
        self.csv = pd.read_csv(path)
        self.csv.diagnosis = [1 if i == "M" else 0 for i in self.csv.diagnosis]

        self.x = self.csv.drop(["diagnosis"], axis = 1) 
        self.x = self.x.drop(["id"], axis = 1) 
        self.x = self.x.dropna(how='all', axis='columns')
        self.x = (self.x - np.min(self.x)) / (np.max(self.x) - np.min(self.x))

        self.y = self.csv.diagnosis.values

    def __len__(self):
        return len(self.y)

    def __getitem__(self, index):
        return torch.tensor(self.x.iloc[[index]].values), torch.tensor(self.y[index])
