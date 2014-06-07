#ifndef TRANSFERFUNCTIONEDITOR_H
#define TRANSFERFUNCTIONEDITOR_H

#include <QWidget>
#include <transferfunctionwidget.h>
#include <volume.h>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <model.h>

class TransferFunctionEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFunctionEditor(QWidget *parent = 0, Model *model = 0);

signals:

public slots:
    void loadTransferFunction();
    void saveTransferFunction();
    void resetTransferFunction();
    void smoothTransferFunction();

    void setRed(bool on);
    void setBlue(bool on);
    void setGreen(bool on);
    void setAlpha(bool on);

private:

    Model *model_;

    QGridLayout *gridLayout;
    TransferFunctionWidget *transferFunctionDrawWidget;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QPushButton *resetButton;
    QPushButton *smoothButton;
    QCheckBox *redCheckBox;
    QCheckBox *greenCheckBox;
    QCheckBox *blueCheckBox;
    QCheckBox *alphaCheckBox;

    Volume::changeTransferScalar change_;

};

#endif // TRANSFERFUNCTIONEDITOR_H
