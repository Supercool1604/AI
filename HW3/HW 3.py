import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from random import randrange
import sys


# training_data = pd.read_csv('train_image.csv', header = None)
# training_label = pd.read_csv('train_label.csv', header = None)
# test_data = pd.read_csv('test_image.csv', header = None)
# test_label = pd.read_csv('test_label.csv', header = None)


training_data = pd.read_csv(sys.argv[1], header = None)
training_label = pd.read_csv(sys.argv[2], header = None)
test_data = pd.read_csv(sys.argv[3], header = None)
# test_label = pd.read_csv(sys.argv[2], header = None)


X = np.array(training_data)
y = np.array(training_label)
X_test = np.array(test_data)
#y_test = np.array(test_label)


# plt.imshow(X[0].reshape((28,28)), cmap='gray')
#print(X.shape)
#print(y.shape)


#X = (X/255).astype('float64')


def oneHotEncode(y):
    ans = []
    for i in range(len(y)):
        currentLabel = np.zeros(10)
        currentLabel[y[i]] = 1
        ans.append(currentLabel)
    return ans

y_train_encoded = oneHotEncode(y)
#y_test_encoded = oneHotEncode(y_test)
#print(np.array(y_train_encoded).shape, np.array(y_test_encoded).shape)


# y_encoded = oneHotEncode(y)
# print(np.array(y_encoded).shape)


def sigmoid(x):
    return 1. / (1. + np.exp(-x))


def softmax(x):
    return np.exp(x - x.max()) / np.sum(np.exp(x - x.max()), axis = 0, keepdims = True)

def CalculateLoss(yHat, y):
#     yHat = yHat*y
    L_sum = np.sum(-1 * np.multiply(y, np.log(yHat + 1e-8)))
 #   L_sum = np.sum(np.multiply(y, np.log(yHat)) + np.multiply((1 - y), np.log(1 - yHat)))
    m = y.shape[1]
    L = (1./m) * L_sum

    return L


def splitData(X, y, trainSize = 0.85):
    X_train = []
    y_train = []
    lenTrain = len(X) * trainSize
    X_copy = list(X)
    y_copy = list(y)
    while len(X_train) < lenTrain:
        ind = randrange(len(X_copy))
        X_train.append(X_copy.pop(ind))
        y_train.append(y_copy.pop(ind))
    return X_train, X_copy, y_train, y_copy


# X_train, X_test, y_train, y_test = splitData(X, y_encoded)



# """
#     Run this cell only once.
#     Else, it will transpose X multiple times and we won't get the desired results.
# """
# X_train = (np.array(X_train)).T
# y_train = (np.array(y_train)).T
# X_test = (np.array(X_test)).T
# y_test = (np.array(y_test)).T
# print(X_train.shape, y_train.shape, X_test.shape, y_test.shape)
# print(np.random.permutation(X_train.shape[1]))


"""
    Run this cell only once.
    Else, it will transpose X multiple times and we won't get the desired results.
"""
X_train = (np.array(X)).T
y_train = (np.array(y_train_encoded)).T
X_test = (np.array(X_test)).T

#y_test = (np.array(y_test_encoded)).T
#print(X_train.shape, y_train.shape, X_test.shape, y_test.shape)
#print(np.random.permutation(X_train.shape[1]))



nnL1 = 512
nnL2 = 128
numFeat = 784
classes = 10
weightsAndBiases = {
    'W1' : np.random.randn(nnL1, numFeat) * 0.01,
    'W2' : np.random.randn(nnL2, nnL1) * 0.01,
    'W3' : np.random.randn(classes, nnL2) * 0.01,
    'b1' : np.zeros(nnL1).reshape(nnL1,1),
    'b2' : np.zeros(nnL2).reshape(nnL2,1),
    'b3' : np.zeros(classes).reshape(classes,1)
}


def forwardProp(X):
    W1 = weightsAndBiases['W1']
    W2 = weightsAndBiases['W2']
    W3 = weightsAndBiases['W3']
    b1 = weightsAndBiases['b1']
    b2 = weightsAndBiases['b2']
    b3 = weightsAndBiases['b3']
    z1 = np.dot(W1, X) + b1
    o1 = sigmoid(z1)

    z2 = np.dot(W2, o1) + b2
    o2 = sigmoid(z2)
    z3 = np.dot(W3, o2) + b3
    o3 = softmax(z3)
    importantValuesForBackPropagation = []
    importantValuesForBackPropagation.append((z1, o1))
    importantValuesForBackPropagation.append((z2, o2))
    importantValuesForBackPropagation.append((z3, o3))
    
    return importantValuesForBackPropagation


# In[16]:


def backProp(X, Y, z_a_values, batchSize):
    yHat = z_a_values[2][1]
    O2 = z_a_values[1][1]
    z2 = z_a_values[1][0]
    z1 = z_a_values[0][0]
    O1 = z_a_values[0][1]
    """
    https://towardsdatascience.com/derivative-of-the-softmax-function-and-the-categorical-cross-entropy-loss-ffceefc081d1
    dLossByZ3 = (dCost/dOut3) * (dOut3/dZ3)
    dLossBydW3 = dLossBydOutput * (dZ3/dW3)
    
    dLossBydOutput2 = dLossBydOutput * (dZ3/dOut2)
    dLossBydW2 = dLossBydOutput2 * (dOut2/dZ2) * (dZ2/dW2)  [dOut2/dZ2 = sigmoid(z2) * (1-sigmoid(z2))]
    
    
    """
    
    dZ3 = yHat - Y 
    
    dW3 = (1/batchSize) * np.dot(dZ3, O2.T)
    db3 = (1/batchSize) * np.sum(dZ3, axis=1, keepdims=True)
    
    
    dO2 = np.dot(weightsAndBiases['W3'].T, dZ3)
    
    
    dZ2 = dO2 * sigmoid(z2) * (1-sigmoid(z2))
    
    dW2 = (1/batchSize) * np.dot(dZ2, O1.T)
    db2 = (1/batchSize) * np.sum(dZ2, axis=1, keepdims=True)


    dO1 = np.dot(weightsAndBiases['W2'].T, dZ2) 
    dZ1 = dO1 * sigmoid(z1) * (1-sigmoid(z1))

    
    dW1 = (1/batchSize) * np.dot(dZ1, X.T)
    db1 = (1/batchSize) * np.sum(dZ1, axis=1, keepdims=True)
    
    weightDerivatives = [dW1, dW2, dW3]
    biasDerivatives = [db1, db2, db3]
    
    return weightDerivatives, biasDerivatives
    
    
    
"""
def accuracyScore(x,y):
    yHat = forwardProp(x)
    yHat = yHat[2][1]
    yHat = yHat.argmax(axis=0)
    y = y.argmax(axis=0)
    c1 = (yHat==y)*1
    return np.mean(c1) * 100
    

def train(X_train, y_train, X_test, y_test, epochs, batchSize, learningRate):
    numImg = X_train.shape[1]
    print('num: ',numImg)
    for epoch in range(epochs):
#         permutation = np.random.permutation(numImg)
#         X_train = X_train[:, permutation]
#         Y_train = y_train[:, permutation]
        X_train = X_train
        Y_train = y_train
        batches = numImg//batchSize
        
        for batchNum in range(batches):
            startBatchFrom = batchNum * batchSize
            endBatchAt = min(startBatchFrom + batchSize, numImg - 1)
#             print(startBatchFrom, endBatchAt)
            currX = X_train[:, startBatchFrom: endBatchAt+1]
            currY = Y_train[:, startBatchFrom: endBatchAt+1] 
#             print(currY)

            outputForwardPropagation = forwardProp(currX)
#             print(outputForwardPropagation[2][1])
            weightD, biasD = backProp(currX, currY, outputForwardPropagation,batchSize) #endBatchAt - startBatchFrom)
            
            weightsAndBiases['W1'] = weightsAndBiases['W1'] - (learningRate * weightD[0])
            weightsAndBiases['W2'] = weightsAndBiases['W2'] - (learningRate * weightD[1])
            weightsAndBiases['W3'] = weightsAndBiases['W3'] - (learningRate * weightD[2])
            weightsAndBiases['b1'] = weightsAndBiases['b1'] - (learningRate * biasD[0])
            weightsAndBiases['b2'] = weightsAndBiases['b2'] - (learningRate * biasD[1])
            weightsAndBiases['b3'] = weightsAndBiases['b3'] - (learningRate * biasD[2])
#             print(weightsAndBiases['W1'])
        outputForwardPropagation = forwardProp(X_train)
#         print(outputForwardPropagation[2][1])
        lossCurrentEpochTrain = CalculateLoss(outputForwardPropagation[2][1], Y_train)
        
            
        outputForwardPropagationTest = forwardProp(X_test)
        lossCurrentEpochTest = CalculateLoss(outputForwardPropagationTest[2][1], y_test)
        print("Accuracy: ", accuracyScore(X_test, y_test))
        print("Epoch {}: training loss = {}, test loss = {}".format(
        epoch + 1, lossCurrentEpochTrain, lossCurrentEpochTest))
    return outputForwardPropagationTest
        
    


"""










def train(X_train, y_train, X_test, epochs, batchSize, learningRate):
    numImg = X_train.shape[1]
    for epoch in range(epochs):
        X_train = X_train
        Y_train = y_train
        batches = numImg//batchSize
        
        for batchNum in range(batches):
            startBatchFrom = batchNum * batchSize
            endBatchAt = min(startBatchFrom + batchSize, numImg - 1)
            currX = X_train[:, startBatchFrom: endBatchAt+1]
            currY = Y_train[:, startBatchFrom: endBatchAt+1] 

            outputForwardPropagation = forwardProp(currX)
            weightD, biasD = backProp(currX, currY, outputForwardPropagation,batchSize) #endBatchAt - startBatchFrom)
            
            weightsAndBiases['W1'] = weightsAndBiases['W1'] - (learningRate * weightD[0])
            weightsAndBiases['W2'] = weightsAndBiases['W2'] - (learningRate * weightD[1])
            weightsAndBiases['W3'] = weightsAndBiases['W3'] - (learningRate * weightD[2])
            weightsAndBiases['b1'] = weightsAndBiases['b1'] - (learningRate * biasD[0])
            weightsAndBiases['b2'] = weightsAndBiases['b2'] - (learningRate * biasD[1])
            weightsAndBiases['b3'] = weightsAndBiases['b3'] - (learningRate * biasD[2])
        outputForwardPropagation = forwardProp(X_train)
        lossCurrentEpochTrain = CalculateLoss(outputForwardPropagation[2][1], Y_train)
        
            
        outputForwardPropagationTest = forwardProp(X_test)
        # lossCurrentEpochTest = CalculateLoss(outputForwardPropagationTest[2][1], y_test)
        # print("Accuracy: ", accuracyScore(X_test, y_test))
        # print("Epoch {}: training loss = {}, test loss = {}".format(
        # epoch + 1, lossCurrentEpochTrain, lossCurrentEpochTest))
    return outputForwardPropagationTest
        
    

epochs = 60
batch_size = 16
learningRate = 0.0075
pred = train(X_train, y_train, X_test, epochs, batch_size, learningRate)
pred = pred[2][1]
pred = np.argmax(pred , axis= 0 )
df = pd.DataFrame(pred)
df.to_csv('test_predictions.csv', index = False, header = False)






