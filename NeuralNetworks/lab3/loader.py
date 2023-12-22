import os
import cv2
import numpy as np

labels_name = ['buildings', 'forest', 'glacier', 'mountain', 'sea', 'street']
image_size = (150, 150)

def load(path):
    images = []
    labels = []

    print("Loading from {}".format(path))

    datasets = os.listdir(path)
    for dataset in datasets:
        print("\t {}".format(dataset))

        index = labels_name.index(dataset)

        images_path = os.path.join(path, dataset)
        images_name = os.listdir(images_path)
        for image_file in images_name:
            image_path = os.path.join(images_path, image_file)

            image = cv2.imread(image_path)
            image = cv2.resize(image, image_size)
            image = image / 255.0

            images.append(image)
            labels.append(index)

    images = np.array(images)
    labels = np.array(labels)

    return images, labels

class Loader:
    def __init__(self, train_path = 'train', test_path = 'test'):
        self.train_images, self.train_labels = load(train_path)
        self.test_images, self.test_labels = load(test_path)