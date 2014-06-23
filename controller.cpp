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
    connect(view_, SIGNAL(createVolume(QString)), model_, SLOT(addVolume(QString)));
    connect(view_, SIGNAL(createTerrain(QString)), model_, SLOT(addTerrain(QString)));

    connect(view_, SIGNAL(setSingleViewMode()), this, SLOT(setSingleViewMode()));
    connect(view_, SIGNAL(setDualViewMode()), this, SLOT(setDualViewMode()));
    connect(view_, SIGNAL(setQuadViewMode()), this, SLOT(setQuadViewMode()));

    connect(view_, SIGNAL(setTesselation(int)), model_, SLOT(setTesselation(int)));

    connect(view_, SIGNAL(setCameraMode()), model_, SLOT(setCameraInteractionMode()));
    connect(view_, SIGNAL(setObjectMode()), model_, SLOT(setObjectInteractionMode()));

    connect(view_, SIGNAL(setScaleX(bool)), model_, SLOT(setScaleX(bool)));
    connect(view_, SIGNAL(setScaleY(bool)), model_, SLOT(setScaleY(bool)));
    connect(view_, SIGNAL(setScaleZ(bool)), model_, SLOT(setScaleZ(bool)));

    connect(view_, SIGNAL(setHorizontalScale(int)), model_, SLOT(setHorizontalScale(int)));
    connect(view_, SIGNAL(setVerticalScale(int)), model_, SLOT(setVerticalScale(int)));

    connect(view_, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(keyPressed(QKeyEvent*)));
    connect(view_, SIGNAL(keyReleased(QKeyEvent*)), this, SLOT(keyReleased(QKeyEvent*)));
}

void Controller::createMouseControllers() {
    if (model_ != NULL && view_ != NULL) {
        mousePerspective = new MouseController(0, view_->getViewport(Model::PERSPECTIVE), model_->getCamera(Model::PERSPECTIVE), model_);
        mouseFront = new MouseController(0, view_->getViewport(Model::FRONT), model_->getCamera(Model::FRONT), model_);
        mouseLeft = new MouseController(0, view_->getViewport(Model::LEFT), model_->getCamera(Model::LEFT), model_);
        mouseTop = new MouseController(0, view_->getViewport(Model::TOP), model_->getCamera(Model::TOP), model_);
    } else {
        std::cout << "Model or View has not been set in the Controller previously to calling createMouseController()!" << std::endl;
    }

    connect(mousePerspective, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));
    connect(mouseFront, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));
    connect(mouseLeft, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));
    connect(mouseTop, SIGNAL(setViewportActive(Model::ViewportType)), model_, SLOT(setActive(Model::ViewportType)));

    connect(mousePerspective, SIGNAL(setActivePrimitive(float)), model_, SLOT(setActivePrimitive(float)));
    connect(mouseFront, SIGNAL(setActivePrimitive(float)), model_, SLOT(setActivePrimitive(float)));
    connect(mouseLeft, SIGNAL(setActivePrimitive(float)), model_, SLOT(setActivePrimitive(float)));
    connect(mouseTop, SIGNAL(setActivePrimitive(float)), model_, SLOT(setActivePrimitive(float)));

    connect(mousePerspective, SIGNAL(updateViewport()), view_, SIGNAL(updateViewports()));
    connect(mouseFront, SIGNAL(updateViewport()), view_, SIGNAL(updateViewports()));
    connect(mouseLeft, SIGNAL(updateViewport()), view_, SIGNAL(updateViewports()));
    connect(mouseTop, SIGNAL(updateViewport()), view_, SIGNAL(updateViewports()));
}

void Controller::setSingleViewMode()
{
    view_->getViewport(Model::PERSPECTIVE)->show();
    view_->getViewport(Model::FRONT)->hide();
    view_->getViewport(Model::LEFT)->hide();
    view_->getViewport(Model::TOP)->hide();
    model_->setActive(Model::PERSPECTIVE);
}

void Controller::setDualViewMode()
{
    view_->getViewport(Model::PERSPECTIVE)->show();
    view_->getViewport(Model::FRONT)->show();
    view_->getViewport(Model::LEFT)->hide();
    view_->getViewport(Model::TOP)->hide();
    if (model_->isActiveViewport(Model::LEFT) || model_->isActiveViewport(Model::TOP)) {
        model_->setActive(Model::FRONT);
    }
}

void Controller::setQuadViewMode()
{
    view_->getViewport(Model::PERSPECTIVE)->show();
    view_->getViewport(Model::FRONT)->show();
    view_->getViewport(Model::LEFT)->show();
    view_->getViewport(Model::TOP)->show();
}

void Controller::keyPressed(QKeyEvent *event) {
    MouseController *mouseController;
    if (model_->isActiveViewport(Model::PERSPECTIVE)) mouseController = mousePerspective;
    if (model_->isActiveViewport(Model::FRONT)) mouseController = mouseFront;
    if (model_->isActiveViewport(Model::LEFT)) mouseController = mouseLeft;
    if (model_->isActiveViewport(Model::TOP)) mouseController = mouseTop;

    mouseController->keyPressEvent(event);
}

void Controller::keyReleased(QKeyEvent *event) {
    MouseController *mouseController;
    if (model_->isActiveViewport(Model::PERSPECTIVE)) mouseController = mousePerspective;
    if (model_->isActiveViewport(Model::FRONT)) mouseController = mouseFront;
    if (model_->isActiveViewport(Model::LEFT)) mouseController = mouseLeft;
    if (model_->isActiveViewport(Model::TOP)) mouseController = mouseTop;

    mouseController->keyReleaseEvent(event);
}
