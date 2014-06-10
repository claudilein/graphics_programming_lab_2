#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainterPath>
#include <volume.h>

class TransferFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFunctionWidget(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void paintEvent(QPaintEvent *e);

signals:
    void functionChanged(int index, int value);

public slots:
    void mousePressEvent(QMouseEvent *event);
    void updateVolume(Volume* volume);

private:
    Volume *volume_;

    QPainterPath *redPath_;
    QPainterPath *bluePath_;
    QPainterPath *greenPath_;
    QPainterPath *alphaPath_;
    QPainterPath *histogramPath_;

};

#endif // TRANSFERFUNCTIONWIDGET_H
