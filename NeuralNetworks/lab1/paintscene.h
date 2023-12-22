#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class PaintScene : public QGraphicsScene {
  Q_OBJECT

public:
  PaintScene(QObject *parent = nullptr);
  ~PaintScene();

  qreal getPaintWidth() const;
  void setPaintWidth(qreal PaintWidth);

  const QColor &getPaintColor() const;
  void setPaintColor(const QColor &PaintColor);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
  QColor mPaintColor = Qt::red;
  qreal mPaintWidth = 10;

  QPointF mPreviousPoint;
};

#endif // PAINTSCENE_H
