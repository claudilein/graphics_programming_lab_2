#include "transferfunctioneditor.h"
#include <QPainter>
#include <QFileDialog>
#include <QFile>
#include <iostream>
#include <sstream>

TransferFunctionEditor::TransferFunctionEditor(QWidget *parent, Model *model) :
    QWidget(parent),
    model_(model)
{
    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);

    transferFunctionDrawWidget = new TransferFunctionWidget(this);
    transferFunctionDrawWidget->setFixedWidth(256);
    transferFunctionDrawWidget->setFixedHeight(256);


    loadButton = new QPushButton("Load", this);
    saveButton = new QPushButton("Save", this);
    resetButton = new QPushButton("Reset", this);
    smoothButton = new QPushButton("Smooth", this);

    redCheckBox = new QCheckBox("red", this);
    redCheckBox->setChecked(false);
    greenCheckBox = new QCheckBox("green", this);
    greenCheckBox->setChecked(false);
    blueCheckBox = new QCheckBox("blue", this);
    blueCheckBox->setChecked(false);
    alphaCheckBox = new QCheckBox("alpha", this);
    alphaCheckBox->setChecked(false);

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

    connect(model_, SIGNAL(activePrimitiveChanged()), this, SLOT(update()));
    connect(this, SIGNAL(volumeChanged(Volume*)), transferFunctionDrawWidget, SLOT(updateVolume(Volume*)));
    connect(transferFunctionDrawWidget, SIGNAL(functionChanged(int,int)), this, SLOT(updateTransferFunction(int,int)));

    volume_ = NULL;
    change_ = Volume::changeTransferScalar(false, false, false, false);
    //this->hide();

}


void TransferFunctionEditor::loadTransferFunction() {    
    if (volume_ != NULL) {
        QString fileName;
        fileName = QFileDialog::getOpenFileName(this, tr("Load Transfer Function"), "../", tr("Tf files (*.tf)"));
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            Volume::transferScalar function[256];

            for (uint i = 0; i < 256 && !file.atEnd(); i++) {

                QByteArray line = file.readLine(20);
                QStringList rgba = QString(line).split(' ');
                if (rgba.size() == 4) {
                    function[i].r_ = rgba[0].toInt();
                    function[i].g_ = rgba[1].toInt();
                    function[i].b_ = rgba[2].toInt();
                    function[i].a_ = rgba[3].toInt();
                } else {
                    std::cout << "File read error on " << fileName.toStdString() << ": Line " << i  << " contains only " << rgba.size() << "numbers. " << std::endl;
                }
            }
            volume_->setTransferFunction(function);
            file.close();
        } else {
            std::cout << "Could not open file " << fileName.toStdString() << std::endl;
        }
        transferFunctionDrawWidget->repaint();
        emit functionChanged(volume_);
    }
}

void TransferFunctionEditor::saveTransferFunction() {
    if (volume_ != NULL) {
        QString fileName;
        fileName = QFileDialog::getSaveFileName(this, tr("Save Transfer Function"), "../", tr("Tf files (*.tf"));
        QFile file(fileName);

        if (file.open(QFile::WriteOnly)) {
            std::stringstream data;
            Volume::transferScalar* function = volume_->getTransferFunction();

            // data is stored in RGBA order, one line equals one index of [0,255]
            for (int i = 0; i < 256; i++) {
                data << function[i].r_ << " " << function[i].g_ << " " << function[i].b_ << " " << function[i].a_ << "\n";
            }

            file.write(data.str().data());
            file.close();
        } else {
            std::cout << "File " << fileName.toStdString() << "could not be saved." << std::endl;
        }
    }
}

void TransferFunctionEditor::resetTransferFunction() {
    if (volume_ != NULL) {
        volume_->resetTransferFunction(change_);
    }
    transferFunctionDrawWidget->repaint();
    emit functionChanged(volume_);
}

void TransferFunctionEditor::smoothTransferFunction() {

}

void TransferFunctionEditor::update() {
    if (model_->getActivePrimitive()->isVolume()) {
        volume_ = static_cast<Volume*>(model_->getActivePrimitive());
        emit volumeChanged(volume_);
        this->show();
    } else {
        volume_ = NULL;
        //this->hide();
    }

}

void TransferFunctionEditor::updateTransferFunction(int index, int value) {
    if (volume_ != NULL) {
        volume_->setTransferFunction(index, change_, Volume::transferScalar(value, value, value, value));
        emit functionChanged(volume_);
    }
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
