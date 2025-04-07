import numpy as np
import matplotlib.pyplot as plt

# Training parameters
learning_rate = 0.01
epochs = 500
momentum = 0.9

# implementácia dát
class Linear:
    # inicializácia váh a biasov
    def __init__(self, input_size, output_size):
        self.weights = np.random.uniform(size=(input_size, output_size))
        self.bias = np.random.uniform(size=(1, output_size))

    # metóda forward
    def forward(self, X):
        self.input = X
        return np.dot(X, self.weights) + self.bias

    # metóda backward
    def backward(self, grad_output, learning_rate):
        grad_input = np.dot(grad_output, self.weights.T) # gradient na vstupe
        grad_weights = np.dot(self.input.T, grad_output) # gradient na váhach
        grad_bias = np.sum(grad_output, axis=0, keepdims=True) # gradient na biasoch

        self.weights -= learning_rate * grad_weights # update váh
        self.bias -= learning_rate * grad_bias # update bias

        return grad_input

# altivacne funkcie
class Sigmoid:
    @staticmethod
    def activate(X):
        X = np.clip(X, -500, 500)
        return 1 / (1 + np.exp(-X))

    @staticmethod
    def derivative(output):
        output = np.clip(output, 1e-7, 1 - 1e-7)
        return output * (1 - output)

class Tanh:
    @staticmethod
    def activate(X):
        X = np.clip(X, -500, 500)
        return np.tanh(X)

    @staticmethod
    def derivative(output):
        output = np.clip(output, -1 + 1e-7, 1 - 1e-7)
        return 1 - output ** 2

class ReLU:
    @staticmethod
    def activate(X):
        return np.maximum(0, X)

    @staticmethod
    def derivative(output):
        return (output > 0).astype(float)

# Loss function
def mse(y_true, y_pred):
    return np.mean((y_true - y_pred) ** 2)

# Loss function derivative
def mse_derivative(y_true, y_pred):
    return 2 * (y_pred - y_true) / y_true.size

class NeuralNetwork:
    def __init__(self, input_size, hidden_size1, output_size, hidden_size2=None, activation='tanh', momentum=momentum):
        np.random.seed(42)
        self.hidden_size2 = hidden_size2
        self.momentum = momentum

        # vyber aktivacnej funkcie
        activation_map = {'sigmoid': Sigmoid, 'tanh': Tanh, 'relu': ReLU}
        self.activation = activation_map[activation]

        # inicializácia vrstiev
        self.layer1 = Linear(input_size, hidden_size1)
        if hidden_size2:
            self.layer2 = Linear(hidden_size1, hidden_size2)
            self.output_layer = Linear(hidden_size2, output_size)
        else:
            self.output_layer = Linear(hidden_size1, output_size)

        # inicializácia aktivacnej funkcie pre vystup
        self.output_activation = Sigmoid()

        # incializácia momenta
        self.velocity_w1 = np.zeros_like(self.layer1.weights)
        self.velocity_b1 = np.zeros_like(self.layer1.bias)
        if hidden_size2:
            self.velocity_w2 = np.zeros_like(self.layer2.weights)
            self.velocity_b2 = np.zeros_like(self.layer2.bias)
        self.velocity_wo = np.zeros_like(self.output_layer.weights) # wo = weights_output
        self.velocity_bo = np.zeros_like(self.output_layer.bias) # bo = bias_output

    # metóda forward
    def forward(self, X):
        self.hidden1 = self.layer1.forward(X)
        self.activated_hidden1 = self.activation.activate(self.hidden1)
        if self.hidden_size2:
            self.hidden2 = self.layer2.forward(self.activated_hidden1)
            self.activated_hidden2 = self.activation.activate(self.hidden2)
            self.output = self.output_layer.forward(self.activated_hidden2)
        else:
            self.output = self.output_layer.forward(self.activated_hidden1)
        self.predicted_output = self.output_activation.activate(self.output)
        return self.predicted_output

    # metóda backward
    def backward(self, X, y, learning_rate):
        # Compute the error at the output
        error = mse_derivative(y, self.predicted_output)
        grad_output = error * self.output_activation.derivative(self.predicted_output)  # gradient na vystupe

        if self.hidden_size2:
            # Backpropagation na druhej vrstve
            grad_hidden2 = self.output_layer.backward(grad_output, learning_rate)
            grad_hidden2_activated = self.activation.derivative(self.hidden2) * grad_hidden2
            grad_hidden1 = self.layer2.backward(grad_hidden2_activated, learning_rate)
        else:
            grad_hidden1 = self.output_layer.backward(grad_output, learning_rate)

        grad_hidden1_activated = self.activation.derivative(self.hidden1) * grad_hidden1

        # update váh a biasov pomocou momenta
        self.velocity_wo = self.momentum * self.velocity_wo + learning_rate * np.dot(self.activated_hidden1.T, grad_output)
        self.velocity_bo = self.momentum * self.velocity_bo + learning_rate * np.sum(grad_output, axis=0, keepdims=True)
        self.output_layer.weights -= self.velocity_wo
        self.output_layer.bias -= self.velocity_bo

        if self.hidden_size2:
            self.velocity_w2 = self.momentum * self.velocity_w2 + learning_rate * np.dot(self.activated_hidden1.T, grad_hidden2_activated)
            self.velocity_b2 = self.momentum * self.velocity_b2 + learning_rate * np.sum(grad_hidden2_activated, axis=0, keepdims=True)
            self.layer2.weights -= self.velocity_w2
            self.layer2.bias -= self.velocity_b2

        self.velocity_w1 = self.momentum * self.velocity_w1 + learning_rate * np.dot(X.T, grad_hidden1_activated)
        self.velocity_b1 = self.momentum * self.velocity_b1 + learning_rate * np.sum(grad_hidden1_activated, axis=0, keepdims=True)
        self.layer1.weights -= self.velocity_w1
        self.layer1.bias -= self.velocity_b1

# funkcia na trénovanie modelu
def train_model(X, y, hidden_size1, epochs, learning_rate, hidden_size2=None, activation='tanh', momentum=momentum):
    input_size = X.shape[1]
    output_size = y.shape[1]
    model = NeuralNetwork(input_size, hidden_size1, output_size, hidden_size2, activation, momentum)
    train_losses = []
    for epoch in range(epochs):
        model.forward(X)
        mse_error = mse(y, model.predicted_output)
        train_losses.append(mse_error)
        model.backward(X, y, learning_rate)
        if (epoch + 1) % 100 == 0:
            print(f'Epoch {epoch + 1}, MSE: {mse_error:.4f}')
    return train_losses, model

# vstup and vystup
X = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
y_XOR = np.array([[0], [1], [1], [0]])
y_AND = np.array([[0], [0], [0], [1]])
y_OR = np.array([[0], [1], [1], [1]])

# trénovanie modelov s jednou skrytou vrstvou
print("\nTraining XOR model with one hidden layer")
train_losses_XOR_1, model_XOR_1 = train_model(X, y_XOR, hidden_size1=4, epochs=epochs, learning_rate=learning_rate)

print("\nTraining AND model with one hidden layer")
train_losses_AND_1, model_AND_1 = train_model(X, y_AND, hidden_size1=4, epochs=epochs, learning_rate=learning_rate)

print("\nTraining OR model with one hidden layer")
train_losses_OR_1, model_OR_1 = train_model(X, y_OR, hidden_size1=4, epochs=epochs, learning_rate=learning_rate)

# trénovanie modelov s dvoma skrytými vrstvami
print("\nTraining XOR model with two hidden layers")
train_losses_XOR_2, model_XOR_2 = train_model(X, y_XOR, hidden_size1=4, hidden_size2=4, epochs=epochs, learning_rate=learning_rate)

print("\nTraining AND model with two hidden layers")
train_losses_AND_2, model_AND_2 = train_model(X, y_AND, hidden_size1=4, hidden_size2=4, epochs=epochs, learning_rate=learning_rate)

print("\nTraining OR model with two hidden layers")
train_losses_OR_2, model_OR_2 = train_model(X, y_OR, hidden_size1=4, hidden_size2=4, epochs=epochs, learning_rate=learning_rate)

# grafy
plt.plot(train_losses_XOR_1, label='XOR (1 Hidden Layer)')
plt.plot(train_losses_XOR_2, label='XOR (2 Hidden Layers)')
plt.plot(train_losses_AND_1, label='AND (1 Hidden Layer)')
plt.plot(train_losses_AND_2, label='AND (2 Hidden Layers)')
plt.plot(train_losses_OR_1, label='OR (1 Hidden Layer)')
plt.plot(train_losses_OR_2, label='OR (2 Hidden Layers)')
plt.title(f'Training loss over Epochs - Tanh activation \n Momentum: {momentum}, Learning Rate: {learning_rate}')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.legend()
plt.show()
