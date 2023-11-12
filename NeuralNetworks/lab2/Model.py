import torch
import torch.nn as nn

import numpy as np

from dataset import CancerDataset

class MultilayerPerceptrone(nn.Module):
    def __init__(self, input_size, output_size):
        super(MultilayerPerceptrone, self).__init__()

        # Applies the rectified linear unit function element-wise:
        #     ReLU(x) = max(0, x)
        self.relu = nn.ReLU()

        # Applies a linear transformation to the incoming data: y = xA^T + b
        self.lin1 = nn.Linear(input_size, 128)
        self.lin2 = nn.Linear(128, 1024)
        self.lin3 = nn.Linear(1024, 512)
        self.lin4 = nn.Linear(512, 64)
        self.lin5 = nn.Linear(64, output_size)

        # 1 / (1 + e ^ (-x))
        self.sigm = nn.Sigmoid()

    # Defines the computation performed at every call.
    def forward(self, x):
        # [[[]]] -> [[]]
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
        x = self.sigm(x)

        return x

class Recognizer:
    device = 'cuda' if torch.cuda.is_available() else 'cpu'

    def __init__(self, dataset_path = 'data.csv'):
        dataset = CancerDataset(dataset_path)
        columnCount = dataset.dataShape()[1]

        self.dl = torch.utils.data.DataLoader(dataset, batch_size = 32, shuffle = True)
        self.model = MultilayerPerceptrone(columnCount, 1).to(self.device)
        self.optimizer = torch.optim.Adam(self.model.parameters())
        self.loss_fn = nn.BCELoss()

    def accuracy(self, outs, labels):
        result = 0
        for i in range(len(outs)):
            result += 1 if (outs[i] > 0.5) == labels[i] else 0
        return result / len(outs)

    def train_one_epoch(self):
        epoch_losses = []
        epoch_accuracies = []

        for data in self.dl:
            inputs, target = data

            inputs = inputs.float()
            target = target.float()

            inputs = inputs.to(self.device)
            target = target.to(self.device)

            # Resets the gradients of all optimized torch.Tensor s.
            # https://stackoverflow.com/questions/48001598/why-do-we-need-to-call-zero-grad-in-pytorch
            self.optimizer.zero_grad()

            outputs = self.model(inputs)

            # [[[]]] -> [[]]
            outputs = outputs.squeeze(1)

            loss = self.loss_fn(outputs, target)

            epoch_accuracies.append(self.accuracy(outputs, target))

            # https://stackoverflow.com/questions/53975717/pytorch-connection-between-loss-backward-and-optimizer-step
            loss.backward()

            # Performs a single optimization step.
            self.optimizer.step()

            epoch_losses.append(loss.item())

        return np.mean(epoch_losses), np.mean(epoch_accuracies)

    def train(self, epochs):
        for i in range(epochs):
            loss, accuracy = self.train_one_epoch()

            print(f'{i+1} epoch')
            print(f'Loss: {loss}')
            print(f'Accuracy: {accuracy * 100}%')
            print('')