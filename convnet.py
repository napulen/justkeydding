from __future__ import print_function
import numpy as np
import argparse
import torch
import os
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torch.utils.data
from torchvision import datasets, transforms
import dataset
from PIL import Image

class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(1, 24, 7, 1)
        self.fc1 = nn.Linear(4*6*24, 48)
        self.dropout1 = nn.Dropout(0.5)
        self.fc2 = nn.Linear(48, 24)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = F.max_pool2d(x, 3, 3)
        x = x.view(-1, 4*6*24)
        x = F.relu(self.fc1(x))
        x = self.dropout1(x)
        x = self.fc2(x)
        return F.log_softmax(x, dim=1)

# class Net(nn.Module):
#     def __init__(self):
#         super(Net, self).__init__()
#         self.conv1 = nn.Conv2d(1, 24, 4, 1)
#         self.conv2 = nn.Conv2d(24, 48, 2, 1)
#         self.fc1 = nn.Linear(2*3*48, 288)
#         self.dropout1 = nn.Dropout(0.5)
#         self.fc2 = nn.Linear(288, 24)

#     def forward(self, x):
#         x = F.relu(self.conv1(x))
#         x = F.max_pool2d(x, 3, 3)
#         x = F.relu(self.conv2(x))
#         x = F.max_pool2d(x, 2, 2)
#         x = x.view(-1, 2*3*48)
#         x = F.relu(self.fc1(x))
#         x = self.dropout1(x)
#         x = self.fc2(x)
#         return F.log_softmax(x, dim=1)

def train(args, model, device, train_loader, optimizer, epoch):
    model.train()
    for batch_idx, (data, target) in enumerate(train_loader):
        data, target = data.to(device), target.to(device)
        optimizer.zero_grad()
        output = model(data)
        loss = F.nll_loss(output, target)
        loss.backward()
        optimizer.step()
        if batch_idx % args.log_interval == 0:
            print('Train Epoch: {} [{}/{} ({:.0f}%)]\tLoss: {:.6f}'.format(
                epoch, batch_idx * len(data), len(train_loader.dataset),
                100. * batch_idx / len(train_loader), loss.item()))

def test(args, model, device, validation_loader):
    model.eval()
    test_loss = 0
    correct = 0
    with torch.no_grad():
        for data, target in validation_loader:
            data, target = data.to(device), target.to(device)
            output = model(data)
            test_loss += F.nll_loss(output, target, reduction='sum').item() # sum up batch loss
            pred = output.argmax(dim=1, keepdim=True) # get the index of the max log-probability
            correct += pred.eq(target.view_as(pred)).sum().item()

    test_loss /= len(validation_loader.dataset)

    print('\nTest set: Average loss: {:.4f}, Accuracy: {}/{} ({:.0f}%)\n'.format(test_loss, correct, len(validation_loader.dataset), 100. * correct / len(validation_loader.dataset)))


def predict(args, model, device, test_loader):
    model.eval()
    outputs = []
    with torch.no_grad():
        for data, _ in test_loader:
            data = data.to(device)
            output = model(data)
            pred = output.argmax(dim=1, keepdim=True)
            outputs.extend(pred)
    return outputs

class EXPERIMENT2(torch.utils.data.Dataset):
    def __init__(self, dataset_name, subset=None, data_augmentation=False, transform=None):
        data = dataset.Dataset(dataset_name)
        if subset:
            image_arrays = getattr(data, '{}_X'.format(subset))
            self.labels = getattr(data, '{}_y'.format(subset))
        else:
            image_arrays = data.X
            self.labels = data.y
        if data_augmentation:
            image_arrays, self.labels = dataset.data_augmentation(
                image_arrays, self.labels
            )
        image_arrays = dataset.feature_scaling(image_arrays, (0, 255))
        image_arrays = image_arrays.astype(np.uint8)
        image_arrays = image_arrays.reshape(-1, 18, 24)
        if transform:
            self.images = [transform(Image.fromarray(x)) for x in image_arrays]
        else:
            self.images = [Image.fromarray(x) for x in image_arrays]

    def __getitem__(self, index):
        return (self.images[index], int(self.labels[index]))

    def __len__(self):
        return len(self.images)

def main():
    # Training settings
    parser = argparse.ArgumentParser(description='PyTorch MNIST Example')
    parser.add_argument('--batch-size', type=int, default=64, metavar='N',
                        help='input batch size for training (default: 64)')
    parser.add_argument('--test-batch-size', type=int, default=1000, metavar='N',
                        help='input batch size for testing (default: 1000)')
    parser.add_argument('--epochs', type=int, default=10, metavar='N',
                        help='number of epochs to train (default: 10)')
    parser.add_argument('--lr', type=float, default=0.01, metavar='LR',
                        help='learning rate (default: 0.01)')
    parser.add_argument('--momentum', type=float, default=0.5, metavar='M',
                        help='SGD momentum (default: 0.5)')
    parser.add_argument('--no-cuda', action='store_true', default=False,
                        help='disables CUDA training')
    parser.add_argument('--seed', type=int, default=1, metavar='S',
                        help='random seed (default: 1)')
    parser.add_argument('--log-interval', type=int, default=10, metavar='N',
                        help='how many batches to wait before logging training status')
    parser.add_argument('--save-model', action='store_true', default=False,
                        help='For Saving the current Model')
    parser.add_argument('--load-model', action='store_true', default=False,
                        help='For loading a pre-trained model')

    args = parser.parse_args()
    use_cuda = not args.no_cuda and torch.cuda.is_available()

    torch.manual_seed(args.seed)

    device = torch.device("cuda" if use_cuda else "cpu")

    kwargs = {'num_workers': 0, 'pin_memory': True} if use_cuda else {}

    data_dir = 'data/comp-551-w2019-project-3-modified-mnist/'

    experiment1_symbolic = EXPERIMENT2(
        'experiment1_symbolic',
        subset=None,
        data_augmentation=True,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    experiment1_chroma = EXPERIMENT2(
        'experiment1_chroma',
        subset=None,
        data_augmentation=True,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    crossera_chroma = EXPERIMENT2(
        'cross-era_chroma',
        subset=None,
        data_augmentation=True,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    giantstepsmtg_chroma = EXPERIMENT2(
        'giantsteps-mtg-key_chroma',
        subset=None,
        data_augmentation=True,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    billboard_chroma_training = EXPERIMENT2(
        'billboard_chroma',
        subset='training',
        data_augmentation=True,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    billboard_chroma_validation = EXPERIMENT2(
        'billboard_chroma',
        subset='validation',
        data_augmentation=True,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    experiment2_training = torch.utils.data.ConcatDataset([
            experiment1_symbolic,
            experiment1_chroma,
            crossera_chroma,
            giantstepsmtg_chroma,
            billboard_chroma_training,
            billboard_chroma_validation,
    ])

    giantsteps_chroma = EXPERIMENT2(
        'giantsteps-key_chroma',
        subset=None,
        data_augmentation=False,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    billboard_chroma_test = EXPERIMENT2(
        'billboard_chroma',
        subset='test',
        data_augmentation=False,
        transform=transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize((0.5,), (0.25,))
        ])
    )

    train_loader = torch.utils.data.DataLoader(
        experiment2_training,
        batch_size=args.batch_size, shuffle=True, **kwargs
    )

    billboard_loader = torch.utils.data.DataLoader(
        billboard_chroma_test,
        batch_size=args.batch_size, shuffle=False, **kwargs
    )

    giantsteps_loader = torch.utils.data.DataLoader(
        giantsteps_chroma,
        batch_size=args.batch_size, shuffle=False, **kwargs
    )

    model = Net().to(device)
    if not args.load_model:
        optimizer = optim.SGD(model.parameters(), lr=args.lr, momentum=args.momentum)
        for epoch in range(1, args.epochs + 1):
            train(args, model, device, train_loader, optimizer, epoch)
            test(args, model, device, billboard_loader)
            test(args, model, device, giantsteps_loader)
        if (args.save_model):
            torch.save(model.state_dict(),"modmnist_cnn.pt")
    else:
        model.load_state_dict(torch.load('modmnist_cnn.pt'))

    billboard_preds = predict(args, model, device, billboard_loader)
    billboard_labels = [x[1] for x in billboard_chroma_test]
    all_scores = dataset.weighted_score(billboard_preds, billboard_labels)
    print(all_scores)

    giantsteps_preds = predict(args, model, device, giantsteps_loader)
    giantsteps_labels = [x[1] for x in giantsteps_chroma]
    all_scores = dataset.weighted_score(giantsteps_preds, giantsteps_labels)
    print(all_scores)

    #prepare_submission('submission.csv', outputs)

if __name__ == '__main__':
    main()