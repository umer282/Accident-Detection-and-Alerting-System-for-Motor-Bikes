import numpy as np
from sklearn.model_selection import train_test_split
from keras.utils import np_utils
import tensorflow as tf
import numpy as np                   # advanced math library
import matplotlib.pyplot as plt      # MATLAB like plotting routines

from keras.models import Sequential  # Model type to be used

# Types of layers to be used in our model
from keras.layers.core import Dense, Dropout, Activation
from keras.utils import np_utils

# NumPy related tools

import pandas as pd
DATA = pd.read_csv('ALL_DATA4.csv')
X = np.array([[DATA.X_acc[i], DATA.Y_acc[i], DATA.Z_acc[i], DATA.X_gyro[i],
             DATA.Y_gyro[i], DATA.Z_gyro[i]] for i in range(len(DATA.Z_gyro))])


y = []
for i in DATA.Label:
    if(i == 'Normal'):
        y.append(0)
    elif(i == 'Rash'):
        y.append(1)
    elif(i == 'Accident'):
        y.append(2)

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.33, random_state=42)


nb_classes = 3  # number of unique digits

Y_train = np_utils.to_categorical(y_train, nb_classes)
Y_test = np_utils.to_categorical(y_test, nb_classes)
Y_train

"""Y_train = np_utils.to_categorical(y_train, nb_classes)
Y_test = np_utils.to_categorical(y_test, nb_classes)

"""
model = Sequential()
model.add(tf.keras.layers.InputLayer(input_shape=(6,)))
model.add(Dense(128, input_shape=(6,)))
model.add(Activation('relu'))
model.add(Dropout(0.2))
model.add(Dense(128))
model.add(Activation('relu'))
model.add(Dropout(0.2))
model.add(Dense(3))
model.add(Activation('softmax'))
mod = model.compile(loss='categorical_crossentropy',
                    optimizer='adam', metrics=['accuracy'])


hist = model.fit(X_train, Y_train, epochs=200,
                 batch_size=25,
                 verbose=1, validation_split=0.1)

score = model.evaluate(X_test, Y_test)
print('Test score:', score[0])
print('Test accuracy:', score[1])


def get_model():
  import tensorflow as tf


    # NumPy related tools
  DATA = pd.read_csv('ALL_DATA4.csv')

  X = np.array([[DATA.X_acc[i], DATA.Y_acc[i], DATA.Z_acc[i], DATA.X_gyro[i],
             DATA.Y_gyro[i], DATA.Z_gyro[i]] for i in range(len(DATA.Z_gyro))])

  y = []
  for i in DATA.Label:
    if(i == 'Normal'):
        y.append(0)
    elif(i == 'Rash'):
        y.append(1)
    elif(i == 'Accident'):
        y.append(2)

  X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.33, random_state=42)


  nb_classes = 3  # number of unique digits

  Y_train = np_utils.to_categorical(y_train, nb_classes)
  Y_test = np_utils.to_categorical(y_test, nb_classes)


  """Y_train = np_utils.to_categorical(y_train, nb_classes)
  Y_test = np_utils.to_categorical(y_test, nb_classes)

  """

  model = Sequential()
  model.add(tf.keras.layers.InputLayer(input_shape=(6,)))
  model.add(Dense(128, input_shape=(6,)))
  model.add(Activation('relu'))
  model.add(Dropout(0.2))
  model.add(Dense(128))
  model.add(Activation('relu'))
  model.add(Dropout(0.2))
  model.add(Dense(3))
  model.add(Activation('softmax'))


  model.compile(loss='categorical_crossentropy',
                optimizer='adam', metrics=['accuracy'])


  model.fit(X_train, Y_train, epochs=200,
            batch_size=25,
            verbose=1, validation_split=0.1)

  score = model.evaluate(X_test, Y_test)

  return model, X_train,Y_train
