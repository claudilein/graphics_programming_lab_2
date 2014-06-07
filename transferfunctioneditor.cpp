#include "transferfunctioneditor.h"

TransferFunctionEditor::TransferFunctionEditor(QWidget *parent, Model *model) :
    QWidget(parent),
    model_(model)
{
    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);

    transferFunctionDrawWidget = new TransferFunctionWidget(this);

    loadButton = new QPushButton("Load", this);
    saveButton = new QPushButton("Save", this);
    resetButton = new QPushButton("Reset", this);
    smoothButton = new QPushButton("Smooth", this);

    redCheckBox = new QCheckBox("red", this);
    greenCheckBox = new QCheckBox("green", this);
    blueCheckBox = new QCheckBox("blue", this);
    alphaCheckBox = new QCheckBox("alpha", this);

    gridLayout->addWidget(transferFunctionDrawWidget, 0, 0);
    gridLayout->addWidget(loadButton, 1, 0);
    gridLayout->addWidget(saveButton, 1, 1);
    gridLayout->addWidget(resetButton, 2, 0);
    gridLayout->addWidget(smoothButton, 2, 1);
    gridLayout->addWidget(redCheckBox, 3, 0);
    gridLayout->addWidget(greenCheckBox, 3, 1);
    gridLayout->addWidget(blueCheckBox, 4, 0);
    gridLayout->addWidget(alphaCheckBox, 4, 1);

    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadTransferFunction()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveTransferFunction()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetTransferFunction()));
    connect(smoothButton, SIGNAL(clicked()), this, SLOT(smoothTransferFunction()));

    connect(redCheckBox, SIGNAL(toggled(bool)), this, SLOT(setRed(bool)));
    connect(blueCheckBox, SIGNAL(toggled(bool)), this, SLOT(setBlue(bool)));
    connect(greenCheckBox, SIGNAL(toggled(bool)), this, SLOT(setGreen(bool)));
    connect(alphaCheckBox, SIGNAL(toggled(bool)), this, SLOT(setAlpha(bool)));

}

void TransferFunctionEditor::loadTransferFunction() {

}

void TransferFunctionEditor::saveTransferFunction() {

}

void TransferFunctionEditor::resetTransferFunction() {
    if (model_->getActivePrimitive()->isVolume()) {
        static_cast<Volume*>(model_->getActivePrimitive())->resetTransferFunction(change_);
    }
}

void TransferFunctionEditor::smoothTransferFunction() {

}

void TransferFunctionEditor::setRed(bool on) {
    change_.r_ = on;
}

void TransferFunctionEditor::setBlue(bool on) {
    change_.b_ = on;
}

void TransferFunctionEditor::setGreen(bool on) {
    change_.g_ = on;
}

void TransferFunctionEditor::setAlpha(bool on) {
    change_.a_ = on;
}
