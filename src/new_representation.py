import sys
import keras
import numpy as np
import pandas as pd
from keras import layers, optimizers, losses, metrics 
from keras.models import load_model
from keras.models import Sequential
from keras.layers import Dense
from keras.utils import np_utils


if len(sys.argv) != 3:
    print("Wrong Number of Arguments!")
    exit(1)

if sys.argv[1] == "-i":
    nn_representations = sys.argv[2]
else:
    print("Wrong Argument!")
    exit(1)


data = pd.read_csv(nn_representations,header=None)
labels = pd.read_csv('actual.csv', header=None)
timestamps = data[0]
data = data.drop(data.columns[0], axis=1)
labels = labels.drop(labels.columns[0], axis=1)

model_A = load_model('../model/WindDenseNN.h5')
model_A.compile(loss='mean_squared_error', optimizer='rmsprop', metrics=['accuracy'])
model_A.evaluate(data, labels, batch_size=32)


model_B = keras.Sequential()
model_B.add(layers.Dense(64, activation='relu', input_shape=(128,)))
model_B.layers[0].set_weights(model_A.layers[0].get_weights())
model_B.compile(loss='mean_squared_error', optimizer='rmsprop', metrics=['accuracy'])

result = model_B.predict(data, batch_size=32)

result_DF = pd.DataFrame(result)
result_DF.insert(0,"",timestamps, True)
result_DF.to_csv("../datasets/new_nn_representations.csv",mode="w", header=None, index=None , sep="\t")
