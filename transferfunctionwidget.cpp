#include "transferfunctionwidget.h"
#include <QPainter>
#include <iostream>

TransferFunctionWidget::TransferFunctionWidget(QWidget *parent) :
    QWidget(parent)
{
    redPath_ = new QPainterPath();
    greenPath_ = new QPainterPath();
    bluePath_ = new QPainterPath();
    alphaPath_ = new QPainterPath();
    volume_ = NULL;
}

QSize TransferFunctionWidget::sizeHint() const {
    return QSize(256, 256);
}
QSize TransferFunctionWidget::minimumSizeHint() const {
    return QSize(256, 256);
}



void TransferFunctionWidget::paintEvent(QPaintEvent *e) {
    QPainter *painter = new QPainter(this);
    // draw rectangle as outer bounds
    painter->setBrush(QBrush(QColor(255, 255, 255)));
    painter->drawRect(QRect(0, 0, 255, 255));

    redPath_ = new QPainterPath();
    greenPath_ = new QPainterPath();
    bluePath_ = new QPainterPath();
    alphaPath_ = new QPainterPath();

    if (volume_ != NULL) {
        // draw path for each component (R,G,B,A)
        Volume::transferScalar* transferFunction = volume_->getTransferFunction();

        redPath_->moveTo(0, this->height() - 1 - transferFunction[0].r_);
        greenPath_->moveTo(0, this->height() - 1 - transferFunction[0].g_);
        bluePath_->moveTo(0, this->height() - 1 - transferFunction[0].b_);
        alphaPath_->moveTo(0, this->height() - 1 - transferFunction[0].a_);

        for (int i = 0; i < 256; i++) {
            redPath_->lineTo(i, this->height() - 1 - transferFunction[i].r_);
            greenPath_->lineTo(i, this->height() - 1 - transferFunction[i].g_);
            bluePath_->lineTo(i, this->height() - 1 - transferFunction[i].b_);
            alphaPath_->lineTo(i, this->height() - 1 - transferFunction[i].a_);
        }

        QColor color = QColor(255, 0, 0);
        painter->setPen(color);
        painter->drawPath(*redPath_);

        color.setRgb(0, 255, 0);
        painter->setPen(color);
        painter->drawPath(*greenPath_);

        color.setRgb(0, 0, 255);
        painter->setPen(color);
        painter->drawPath(*bluePath_);

        color.setRgb(0, 0, 0);
        painter->setPen(color);
        painter->drawPath(*alphaPath_);

    }
    delete painter;

}


void TransferFunctionWidget::mousePressEvent(QMouseEvent *event) {
    if (volume_ != NULL && event->button() == Qt::LeftButton) {
        int index = event->x();
        int value = this->height() - 1 - event->y();
        emit functionChanged(index, value);
        this->repaint();
    }

    //int index =
    //volume_->setTransferFunction();

}

void TransferFunctionWidget::updateVolume(Volume* volume) {
    volume_ = volume;
    //paintEvent(&QPaintEvent(QRect(0,0,256,256)));

    this->repaint();
}
