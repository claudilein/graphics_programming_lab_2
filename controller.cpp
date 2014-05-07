#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent)
{
}


void Controller::setModel(Model *model)
{
    model_ = model;
}

void Controller::setView(View *view) {
    view_ = view;
}
