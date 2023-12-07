import matplotlib.pyplot as plt
from network import Network
import tensorflow as tf
import numpy as np
import cv2

# epochs = 100
#
# n = Network()
# history = n.train(epochs)
#
# epochs_range = range(epochs)
#
# plt.subplot(1, 2, 1)
# plt.plot(epochs_range, history.history['accuracy'], label = 'accuracy')
# plt.plot(epochs_range, history.history['val_accuracy'], label = 'val_accuracy')
# plt.xlabel('Epoch')
# plt.ylabel('Accuracy')
# plt.ylim([0, 1])
# plt.legend(loc = 'lower right')
#
# plt.subplot(1, 2, 2)
# plt.plot(epochs_range, history.history['loss'], label = 'loss')
# plt.plot(epochs_range, history.history['val_loss'], label = 'val_loss')
# plt.xlabel('Epoch')
# plt.ylabel('Loss')
# plt.legend(loc = 'lower right')
#
# plt.savefig('plt_{}.png'.format(epochs))
#
# plt.show()
#

labels_name = ['buildings', 'forest', 'glacier', 'mountain', 'sea', 'street']
image_size = (150, 150)

image = cv2.imread("C:\\Users\\Ihtie\\company\\ihtier\\UniversityAssignments\\NeuralNetworks\\lab3\\test\\forest\\20089.jpg")
image = cv2.resize(image, image_size)
image = image / 255.0

# https://qna.habr.com/q/1263540
data = np.ndarray(shape=(1, 150, 150, 3), dtype = np.float32)
data[0] = image

model = tf.keras.models.load_model("C:\\Users\\Ihtie\\company\\ihtier\\UniversityAssignments\\NeuralNetworks\\lab3\\location_100.keras")
y_pred = model.predict(data)

k = y_pred.argmax()
print(labels_name[k])