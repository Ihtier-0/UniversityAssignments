#include "paintscene.h"

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent) {}

PaintScene::~PaintScene() {}

void PaintScene::mousePressEvent(
    QGraphicsSceneMouseEvent *event) /* override */ {
  const auto halfPaintWidth = mPaintWidth / 2.0;

  const auto x = event->scenePos().x();
  const auto y = event->scenePos().y();

  const auto ellipseCenterX = x - halfPaintWidth;
  const auto ellipseCenterY = y - halfPaintWidth;

  const auto pen = Qt::NoPen;
  const auto brush = QBrush(mPaintColor);

  addEllipse(ellipseCenterX, ellipseCenterY, mPaintWidth, mPaintWidth, pen,
             brush);
  mPreviousPoint = event->scenePos();
}

void PaintScene::mouseMoveEvent(
    QGraphicsSceneMouseEvent *event) /* override */ {
  const auto prevX = mPreviousPoint.x();
  const auto prevY = mPreviousPoint.y();

  const auto x = event->scenePos().x();
  const auto y = event->scenePos().y();

  const auto pen = QPen(mPaintColor, mPaintWidth, Qt::SolidLine, Qt::RoundCap);

  addLine(prevX, prevY, x, y, pen);
  mPreviousPoint = event->scenePos();
}

const QColor &PaintScene::getPaintColor() const { return mPaintColor; }

void PaintScene::setPaintColor(const QColor &PaintColor) {
  mPaintColor = PaintColor;
}

qreal PaintScene::getPaintWidth() const { return mPaintWidth; }

void PaintScene::setPaintWidth(qreal PaintWidth) { mPaintWidth = PaintWidth; }
