#include "controller.h"
#include <iostream>

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

void Controller::createMouseControllers() {
    if (model_ != NULL && view_ != NULL) {
        mousePerspective = new MouseController(0, view_->getViewport(Model::PERSPECTIVE), model_->getCamera(Model::PERSPECTIVE));
        mouseFront = new MouseController(0, view_->getViewport(Model::FRONT), model_->getCamera(Model::FRONT));
        mouseLeft = new MouseController(0, view_->getViewport(Model::LEFT), model_->getCamera(Model::LEFT));
        mouseTop = new MouseController(0, view_->getViewport(Model::TOP), model_->getCamera(Model::TOP));
    } else {
        std::cout << "Model or View has not been set in the Controller previously to calling createMouseController()!" << std::endl;
    }

    connect(mousePerspective, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));
    connect(mouseFront, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));
    connect(mouseLeft, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));
    connect(mouseTop, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));
}
