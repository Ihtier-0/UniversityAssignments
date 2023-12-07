import tensorflow as tf
from loader import Loader
from keras.preprocessing.image import ImageDataGenerator

class Network:
    def __init__(self):
        self.data = Loader()
        self.model = tf.keras.Sequential(
            [
                tf.keras.layers.Conv2D(
                    32,                         # количество фильтров
                    (3, 3),                     # размер фильтра
                    activation = 'relu',        # функция активации
                    input_shape = (150, 150, 3) # размер картинки
                ),
                tf.keras.layers.MaxPooling2D(2, 2), # уменьшить фильтры по ширине и высоте в 2 раза

                tf.keras.layers.Conv2D(
                    64,
                    (3, 3),
                    activation = 'relu'
                ),
                tf.keras.layers.MaxPooling2D(2, 2),

                tf.keras.layers.Conv2D(
                    128,
                    (3, 3),
                    activation = 'relu'
                ),
                tf.keras.layers.MaxPooling2D(2, 2),

                tf.keras.layers.Conv2D(
                    256,
                    (3, 3),
                    activation = 'relu'
                ),
                tf.keras.layers.MaxPooling2D(2, 2),

                tf.keras.layers.Conv2D(
                    512,
                    (3, 3),
                    activation = 'relu'
                ),
                tf.keras.layers.MaxPooling2D(2, 2),

                tf.keras.layers.Flatten(), # вытягивает матрицу в вектор

                tf.keras.layers.Dense(
                    128,
                    activation = 'relu'
                ),
                tf.keras.layers.Dropout(0.5), # не обучать с вероятностью 0.5
                                              # для защиты от переобучения

                tf.keras.layers.Dense(
                    6,
                    activation = 'softmax'
                )
        ])

        self.model.compile(
            optimizer = 'adam',
            loss = 'sparse_categorical_crossentropy', # минимизируется в процессе обучения
                                                      # преобразовать y из меток классов в вероятность
                                                      # categorical_crossentropy если бы y был вектором вероятностей
            metrics = ['accuracy'] # Используются для мониторинга обучения
                                   # accuracy - вычисляет, как часто прогнозы совпадают с метками.
        )

    def train(self, e):
        history = self.model.fit(
            self.data.train_images,
            self.data.train_labels,
            
            epochs = e,
            
            validation_data = (self.data.test_images, self.data.test_labels)
        )

        self.model.save('location_{}.keras'.format(e))

        return history
