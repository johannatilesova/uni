# projekt bol robeny pomocou chatu GPT a videotutoriálov

import torch
import torch.nn as nn
import torch.optim as optim
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay

# 1. Načítanie Datasetu
transform = transforms.Compose([transforms.ToTensor()])

train_dataset = torchvision.datasets.MNIST(
    root='./data',
    train=True,
    download=True,
    transform=transform)

test_dataset = torchvision.datasets.MNIST(
    root='./data',
    train=False,
    download=True,
    transform=transform)

loaders = {
    'train': DataLoader(train_dataset, batch_size=64, shuffle=True),
    'test': DataLoader(test_dataset, batch_size=64, shuffle=False),
}

# 2. Vytvorenie modelu
class MNISTModel(nn.Module):
    def __init__(self):
        super(MNISTModel, self).__init__()
        self.fc1 = nn.Linear(784, 128)
        self.fc2 = nn.Linear(128, 64)
        self.fc3 = nn.Linear(64, 10)
        self.relu = nn.ReLU()
        self.softmax = nn.LogSoftmax(dim=1)

    def forward(self, x):
        x = x.view(-1, 784)
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.softmax(self.fc3(x))
        return x

# 3. Definovanie parametrov
learning_rate = 0.0001  # Rýchlosť učenia
epochs = 50 # Počet epoch

# 4. Loss funkcia a optimalizačné algoritmy
criterion = nn.CrossEntropyLoss()

optimizers = {
    'SGD': lambda model: optim.SGD(model.parameters(), lr=learning_rate),
    'SGD with momentum': lambda model: optim.SGD(model.parameters(), lr=learning_rate, momentum=0.9),
    'Adam': lambda model: optim.Adam(model.parameters(), lr=learning_rate),
}

# Funkcia na trénovanie modelu
def train_model(optimizer_name):
    model = MNISTModel()  # znova vytvoríme model, aby sme mohli trénovať odznova pre každý optimizer
    optimizer = optimizers[optimizer_name](model)
    train_losses = []
    test_losses = []
    train_accuracies = []
    test_accuracies = []

    for epoch in range(epochs):
        model.train()
        running_loss = 0.0
        correct_train = 0
        total_train = 0
        for images, labels in loaders['train']:
            optimizer.zero_grad()  # Vynulovanie gradientov
            outputs = model(images)  # Predikcie modelu
            loss = criterion(outputs, labels)  # Výpočet straty
            loss.backward()  # Spätná propagácia
            optimizer.step()  # Aktualizácia váh modelu
            running_loss += loss.item()
            _, predicted = torch.max(outputs.data, 1)
            total_train += labels.size(0)
            correct_train += (predicted == labels).sum().item()
        train_losses.append(running_loss / len(loaders['train']))
        train_accuracies.append(100 * correct_train / total_train)

        model.eval() # Prepne model do režimu vyhodnocovania
        test_loss = 0.0
        correct_test = 0
        total_test = 0
        with torch.no_grad():
            for images, labels in loaders['test']:
                outputs = model(images)
                loss = criterion(outputs, labels)
                test_loss += loss.item()
                _, predicted = torch.max(outputs.data, 1)
                total_test += labels.size(0)
                correct_test += (predicted == labels).sum().item()
        test_losses.append(test_loss / len(loaders['test']))
        test_accuracies.append(100 * correct_test / total_test)

        print(f'Epoch [{epoch+1}/{epochs}], Training Loss: {train_losses[-1]:.4f}, Testing Loss: {test_losses[-1]:.4f}, Training Accuracy: {train_accuracies[-1]:.2f}%, Testing Accuracy: {test_accuracies[-1]:.2f}%')

    return train_losses, test_losses, train_accuracies, test_accuracies, model

# Plotting function
def plot_metrics(train_losses, test_losses, train_accuracies, test_accuracies, optimizer_name):
    # Plot training and testing losses
    plt.figure()
    plt.plot(train_losses, label='Training Loss')
    plt.plot(test_losses, label='Testing Loss')
    plt.title(f'{optimizer_name} Optimizer')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.legend()
    plt.show()

    # Plot training and testing accuracies
    plt.figure()
    plt.plot(train_accuracies, label='Training Accuracy')
    plt.plot(test_accuracies, label='Testing Accuracy')
    plt.title(f'Accuracy with {optimizer_name} Optimizer')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.legend()
    plt.show()

def plot_confusion_matrix(model, optimizer_name):
    all_preds, all_labels = [], []
    model.eval()
    with torch.no_grad():
        for images, labels in loaders['test']:
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            all_preds.extend(predicted.cpu().numpy())
            all_labels.extend(labels.cpu().numpy())

    cm = confusion_matrix(all_labels, all_preds)
    disp = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=range(10))
    disp.plot(cmap=plt.cm.Blues)
    plt.title(f"Confusion Matrix for {optimizer_name} Optimizer")
    plt.show()

# Train and test the model with different optimizers
for optimizer_name in optimizers.keys():
    print(f'\nTraining with {optimizer_name} optimizer:')
    train_losses, test_losses, train_accuracies, test_accuracies, model = train_model(optimizer_name)
    plot_metrics(train_losses, test_losses, train_accuracies, test_accuracies, optimizer_name)
    plot_confusion_matrix(model, optimizer_name)
