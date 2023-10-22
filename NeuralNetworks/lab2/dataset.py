import pandas as pd
import numpy as np

import torch

class CancerDataset(torch.utils.data.Dataset):
    def __init__(self, path = 'data.csv'):
        super(CancerDataset, self).__init__()

        self.csv = pd.read_csv(path)

        # The diagnosis of breast tissues (M = malignant, B = benign)
        #     malignant - злокачественный
        #     benign - доброкачественный
        d = { 'M':0, 'B':1 }
        self.csv.diagnosis = self.csv.diagnosis.map(d)

        # axis {0 or ‘index’, 1 or ‘columns’}, default 0
        #     Whether to drop labels from the index (0 or ‘index’) or columns (1 or ‘columns’).
        self.x = self.csv.drop(["diagnosis"], axis = 1)
        self.x = self.x.drop(["id"], axis = 1)

        # Remove missing values.
        # axis " {0 or ‘index’, 1 or ‘columns’}, default 0
        #     Determine if rows or columns which contain missing values are removed.
        #         0, or ‘index’ : Drop rows which contain missing values.
        #         1, or ‘columns’ : Drop columns which contain missing value.
        #     Only a single axis is allowed.
        # how : {‘any’, ‘all’}, default ‘any’
        #     Determine if row or column is removed from DataFrame, when we have at least one NA or all NA.
        #         ‘any’ : If any NA values are present, drop that row or column.
        #         ‘all’ : If all values are NA, drop that row or column.
        self.x = self.x.dropna(axis = 'columns', how = 'any')

        # Normalization
        self.x = (self.x - np.min(self.x)) / (np.max(self.x) - np.min(self.x))

        self.y = self.csv.diagnosis.values

    def dataShape(self):
        return np.shape(self.x)

    def __len__(self):
        return len(self.y)

    def __getitem__(self, index):
        return torch.tensor(self.x.iloc[[index]].values), torch.tensor(self.y[index])
