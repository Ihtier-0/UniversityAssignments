import torch.nn as nn
import torch
from torch.utils.data import Dataset, DataLoader

import numpy as np

from CancerDataset import CancerDataset

device = 'cuda' if torch.cuda.is_available() else 'cpu'

class MultilayerPerceptrone(nn.Module):
    def __init__(self, input_size, output_size):
        super(MultilayerPerceptrone, self).__init__()
        self.lin1 = nn.Linear(input_size,128)
        self.relu = nn.ReLU()
        self.lin2 = nn.Linear(128, 128)
        self.lin3 = nn.Linear(128, 1024)
        self.lin4 = nn.Linear(1024, 512)
        self.lin5 = nn.Linear(512, 64)
        self.lin6 = nn.Linear(64, output_size)
        self.sigm = nn.Sigmoid()

    def forward(self, x):
        x = x.squeeze(1)
        x = self.lin1(x)
        x = self.relu(x)
        x = self.lin2(x)
        x = self.relu(x)
        x = self.lin3(x)
        x = self.relu(x)
        x = self.lin4(x)
        x = self.relu(x)
        x = self.lin5(x)
        x = self.relu(x)
        x = self.lin6(x)
        x = self.sigm(x)
        return x

class Recognizer:
    def __init__(self, input_size, output_size, dataset_path='data.csv'):
        self.model = MultilayerPerceptrone(input_size, output_size).to(device)
        self.train_dl = DataLoader(CancerDataset(dataset_path), batch_size=32, shuffle=True)
        self.optimizer = torch.optim.Adam(self.model.parameters(), lr=1e-3)
        self.loss_fn = nn.BCELoss()

    @torch.no_grad()
    def accuracy(self, outs, labels):
             result = 0
             for i in range(len(outs)):
                if (outs[i] > 0.5) == labels[i]:
                    result+=1
             return result/len(outs)

    def train_one_epoch(self, epoch_index):
        epoch_losses = []
        epoch_accuracies = []
        for i, data in enumerate(self.train_dl):
           inputs, target = data
           inputs = inputs.float()
           target = target.float()
           inputs = inputs.to(device)
           target = target.to(device)
           self.optimizer.zero_grad()
           outputs = self.model(inputs)
           outputs = outputs.squeeze(1)
           loss = self.loss_fn(outputs, target)
           epoch_accuracies.append(self.accuracy(outputs, target))
           loss.backward()
           self.optimizer.step()
           epoch_losses.append(loss.item())
        return np.mean(epoch_losses), np.mean(epoch_accuracies)

    def train(self, epochs):
        for i in range(epochs):
            print(f'{i+1} epoch')
            loss, acc = self.train_one_epoch(i)
            print(f'Loss: {loss}\nAccuracy: {acc*100} %')

rzr = Recognizer(30,1)
rzr.train(50)