#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <view.h>
#include <model.h>
#include <mousecontroller.h>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    void setModel(Model *model);
    void setView(View *view);

signals:

public slots:    
    void mousePressEvent(QMouseEvent *event, Model::ViewportType type);
    void mouseMoveEvent(QMouseEvent *event, Model::ViewportType type);
    void wheelEvent(QWheelEvent *event, Model::ViewportType type);

private:
    View *view_;
    Model *model_;

    MouseController *mousePerspective;
    MouseController *mouseFront;
    MouseController *mouseLeft;
    MouseController *mouseTop;





};

#endif // CONTROLLER_H
