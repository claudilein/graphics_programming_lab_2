#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include <QWidget>

class TransferFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFunctionWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *e);

signals:

public slots:

};

#endif // TRANSFERFUNCTIONWIDGET_H
