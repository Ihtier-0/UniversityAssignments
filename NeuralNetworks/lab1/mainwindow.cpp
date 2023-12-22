#include "mainwindow.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

////////////////////////////////////////////////////////////////////////////////
/// MainWindow
////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mPerceptron(mRecognizableImageSize.width() *
                                           mRecognizableImageSize.height(),
                                       /* randomWeights */ true) {
  const auto main_layout = layout();
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0, 0, 0, 0);

  auto main = new QWidget(this);
  setCentralWidget(main);

  auto layout = new QHBoxLayout(main);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);

  mScene = new PaintScene;
  mView = new QGraphicsView(mScene);
  mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  layout->addWidget(mView);

  auto buttonLayout = new QVBoxLayout;
  layout->addLayout(buttonLayout);

  auto clear = new QPushButton("clear");
  QObject::connect(clear, &QPushButton::clicked, this,
                   [this]() { mView->scene()->clear(); });
  buttonLayout->addWidget(clear);

  auto recognize = new QPushButton("recognize");
  QObject::connect(recognize, &QPushButton::clicked, this,
                   &MainWindow::recognize);
  buttonLayout->addWidget(recognize);

  mResult = new QLabel;
  buttonLayout->addWidget(mResult);

  auto wrongResult = new QPushButton("wrongResult");
  QObject::connect(wrongResult, &QPushButton::clicked, this, [this]() {
    mPerceptron.update(mLearningRate, (1 - mLastResult) - mLastResult,
                       mLastInput);
  });
  buttonLayout->addWidget(wrongResult);

  // QToolBar* toolbar = new QToolBar(this);
  auto menuBar = new QMenuBar(this);
  // edit -> preferences
  auto settingsAction = menuBar->addAction("Preferences");

  QObject::connect(settingsAction, &QAction::triggered, this, [this]() {
    auto settings = new QDialog(this);
    settings->setFixedSize(200, 200);

    auto mainLayout = new QVBoxLayout(settings);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    settings->setLayout(mainLayout);

    auto controlLayout = new QGridLayout;
    mainLayout->addLayout(controlLayout);

    controlLayout->addWidget(new QLabel("Class 0:"), 0, 0);
    controlLayout->addWidget(new QLabel("Class 1:"), 1, 0);

    auto class0Editor = new QLineEdit(this->mClasses[0]);
    auto class1Editor = new QLineEdit(this->mClasses[1]);

    controlLayout->addWidget(class0Editor, 0, 1);
    controlLayout->addWidget(class1Editor, 1, 1);

    auto buttonBox = new QDialogButtonBox;
    auto saveButton = new QPushButton("Save");
    saveButton->setEnabled(false);
    QObject::connect(saveButton, &QPushButton::clicked, this,
                     [this, settings, class0Editor, class1Editor]() {
                       if (class0Editor->isModified()) {
                         this->mClasses[0] = class0Editor->text();
                       }

                       if (class1Editor->isModified()) {
                         this->mClasses[1] = class0Editor->text();
                       }

                       settings->close();
                     });

    auto enableSave = [saveButton, class0Editor, class1Editor]() {
      bool isEnabled = saveButton->isEnabled();

      if (!isEnabled) {
        saveButton->setEnabled(true);
      }
    };

    QObject::connect(class0Editor, &QLineEdit::textChanged, this, enableSave);
    QObject::connect(class1Editor, &QLineEdit::textChanged, this, enableSave);

    buttonBox->addButton(saveButton, QDialogButtonBox::ApplyRole);

    auto closeButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    QObject::connect(closeButton, &QPushButton::clicked, settings,
                     &QDialog::close);

    mainLayout->addWidget(buttonBox);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->show();
  });

  this->setMenuBar(menuBar);
}

MainWindow::~MainWindow() {}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);

  const auto w = mView->width(), h = mView->height();
  mView->setSceneRect(0, 0, w, h);
}

void MainWindow::recognize() {
  const auto rect = mScene->itemsBoundingRect();

  if (rect.isEmpty()) {
    return;
  }

  mScene->setSceneRect(rect);
  QImage image(rect.size().toSize(), QImage::Format_RGB32);
  image.fill(Qt::transparent);

  // the QPainter has to clean up before the QImage does, otherwise you get the
  // error.
  {
    QPainter painter(&image);
    mScene->render(&painter);
  }

  mLastInput = toPerceptronInput(image);
  mLastResult = mPerceptron.recognize(mLastInput);
  mResult->setText(mClasses[qRound(mLastResult)]);
  // TODO! gui

  //

  const auto pen = QPen(mRecognizeColor, mScene->getPaintWidth(), Qt::SolidLine,
                        Qt::RoundCap);

  mScene->addRect(rect, pen);
}

std::vector<float> MainWindow::toPerceptronInput(const QImage &image) {
  const auto scaled = image.scaled(
      mRecognizableImageSize, Qt::AspectRatioMode::IgnoreAspectRatio,
      Qt::TransformationMode::SmoothTransformation);
  scaled.save("file_name.png");

  const auto w = mRecognizableImageSize.width();
  const auto h = mRecognizableImageSize.height();

  std::vector<float> perceptronInput;
  perceptronInput.resize(w * h, 0);

  const auto paintColor = mScene->getPaintColor();

  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      const auto color = scaled.pixelColor(x, y);
      // color.black() TODO
      const auto r = color.red();
      const auto g = color.green();
      const auto b = color.blue();
      perceptronInput[x * w + y] = static_cast<float>(r > 0 || g > 0 || b > 0);
    }
  }

  return std::move(perceptronInput);
}
