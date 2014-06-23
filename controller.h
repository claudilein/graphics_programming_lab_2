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
    void createMouseControllers();

signals:
    void updateViewports();

public slots:
    void setSingleViewMode();
    void setDualViewMode();
    void setQuadViewMode();

    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);

private:
    View *view_;
    Model *model_;

    MouseController *mousePerspective;
    MouseController *mouseFront;
    MouseController *mouseLeft;
    MouseController *mouseTop;





};

#endif // CONTROLLER_H
