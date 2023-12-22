#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>

#include "paintscene.h"
#include "perceptron.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void resizeEvent(QResizeEvent *event) override;

protected slots:
  void recognize();

private:
  std::vector<float> toPerceptronInput(const QImage &image);

  QSize mRecognizableImageSize = QSize(15, 15);

  float mLearningRate = 0.5f; // in [0, 1]

  PaintScene *mScene = nullptr;
  QGraphicsView *mView = nullptr;

  QColor mRecognizeColor = Qt::green;

  Perceptron mPerceptron;

  std::vector<float> mLastInput;
  float mLastResult;

  QLabel *mResult = nullptr;
  QString mClasses[2] = {"O", "X"};
};

#endif // MAINWINDOW_H
