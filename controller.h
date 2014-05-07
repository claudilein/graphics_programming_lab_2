#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <view.h>
#include <model.h>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    void setModel(Model *model);
    void setView(View *view);

signals:

public slots:

private:
    View *view_;
    Model *model_;



};

#endif // CONTROLLER_H
