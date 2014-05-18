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

    connect(view_, SIGNAL(createCube()), model_, SLOT(addCube()));
    connect(view_, SIGNAL(createSphere()), model_, SLOT(addSphere()));
    connect(view_, SIGNAL(createCylinder()), model_, SLOT(addCylinder()));
    connect(view_, SIGNAL(createCone()), model_, SLOT(addCone()));
    connect(view_, SIGNAL(createTorus()), model_, SLOT(addTorus()));

    connect(view_, SIGNAL(setSingleViewMode()), this, SLOT(setSingleViewMode()));
    connect(view_, SIGNAL(setDualViewMode()), this, SLOT(setDualViewMode()));
    connect(view_, SIGNAL(setQuadViewMode()), this, SLOT(setQuadViewMode()));
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

    connect(mousePerspective, SIGNAL(setActivePrimitive(int)), model_, SLOT(setActivePrimitive(int)));
    connect(mouseFront, SIGNAL(setActivePrimitive(int)), model_, SLOT(setActivePrimitive(int)));
    connect(mouseLeft, SIGNAL(setActivePrimitive(int)), model_, SLOT(setActivePrimitive(int)));
    connect(mouseTop, SIGNAL(setActivePrimitive(int)), model_, SLOT(setActivePrimitive(int)));
}

void Controller::setSingleViewMode()
{

    std::cout << "setting single view" << std::endl;
    view_->getViewport(Model::PERSPECTIVE)->show();
    view_->getViewport(Model::FRONT)->setHidden(true);
    view_->getViewport(Model::LEFT)->hide();
    view_->getViewport(Model::TOP)->hide();
    std::cout << "hidden: " << view_->getViewport(Model::FRONT)->isHidden() << std::endl;
}

void Controller::setDualViewMode()
{
    view_->getViewport(Model::PERSPECTIVE)->show();
    view_->getViewport(Model::FRONT)->show();
    view_->getViewport(Model::LEFT)->hide();
    view_->getViewport(Model::TOP)->hide();
}

void Controller::setQuadViewMode()
{
    view_->getViewport(Model::PERSPECTIVE)->show();
    view_->getViewport(Model::FRONT)->show();
    view_->getViewport(Model::LEFT)->show();
    view_->getViewport(Model::TOP)->show();
}
