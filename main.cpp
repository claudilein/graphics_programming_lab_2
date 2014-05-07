#include "view.h"
#include <model.h>
#include <controller.h>
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* Model-View-Controller pattern.
     * The Controller knows the View and the Model. It can change the data stored in
     * the model and it can select and change the view that should present this data.
     * The View knows the Model as it has to call several get() functions to get the
     * data it should represent.
     * The Model needs to notify the View when the data has changed. This is done via
     * a signal that the model emits. It is connected here to a slot in the View.
     * Similarly, the View needs to notify the Controller of user inputs and can
     * emit several signals which are dealt with in the respective slots of the Controller.
     */
    Model model;
    View view;
    Controller controller;

    view.setModel(&model);

    controller.setModel(&model);
    controller.setView(&view);

    QObject::connect(&view, SIGNAL(mousePressed(QMouseEvent *event, Model::ViewportType type)),
            &controller, SIGNAL(mousePressed(QMouseEvent *event, Model::ViewportType type)));


    view.show();

    return a.exec();
}
