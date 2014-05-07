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
    mousePerspective = new MouseController(0, view_->getViewport(Model::PERSPECTIVE));
    mouseFront = new MouseController(0, view_->getViewport(Model::FRONT));
    mouseLeft = new MouseController(0, view_->getViewport(Model::LEFT));
    mouseTop = new MouseController(0, view_->getViewport(Model::TOP));
}
