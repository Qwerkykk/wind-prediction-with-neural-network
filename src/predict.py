import csv
import sys
import pandas as pd
import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras.utils import np_utils
from keras.models import load_model


if len(sys.argv) != 3:
    print("Wrong Number of Arguments!")
    exit(1)

if sys.argv[1] == "-i":
    nn_representations = sys.argv[2]
else:
    print("Wrong Argument!")
    exit(1)

data = pd.read_csv(nn_representations,header=None)
labels = pd.read_csv('../datasets/actual.csv', header=None)
timestamps = data[0]
data = data.drop(data.columns[0], axis=1)
labels = labels.drop(labels.columns[0], axis=1)

model = load_model('../model/WindDenseNN.h5')
model.compile(loss='mean_squared_error', optimizer='rmsprop', metrics=['accuracy'])


model.evaluate(data, labels, batch_size=32)


result = model.predict(data, batch_size=32)

nlabels = labels.to_numpy()

#calculate MSE
mse = np.subtract(nlabels, result)
mse = np.square(mse)
mse = np.concatenate(mse).sum()
mse = mse/23988
mse = mse/7


#calculate MAE
mae = np.subtract(nlabels, result)
mae = np.absolute(mae)
mae = np.concatenate(mae).sum()
mae = mae/23988
mae = mae/7


#calculate MAPE
avg = np.mean(nlabels)
nlabels = np.where(nlabels==0, avg, nlabels) 

mape = np.subtract(nlabels, result)
mape = np.divide(mape, nlabels) 
mape = np.absolute(mape)
mape = np.concatenate(mape).sum()
mape = mape/23988
mape = mape/7
mape = mape*100


#Export results
with open('../datasets/predicted.csv', mode='w') as csv_file:
    file_writer = csv.writer(csv_file, delimiter=" ", quotechar=' ', quoting=csv.QUOTE_MINIMAL)
    
    file_writer.writerow(["MAE:", mae, "\tMSE:", mse, "\tMAPE:", mape,"%"])

result_DF = pd.DataFrame(result)

result_DF.insert(0,"",timestamps, True)

result_DF.to_csv("../datasets/predicted.csv",mode="a", header=None, index=None , sep="\t")


