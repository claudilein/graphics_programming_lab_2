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

    void setAmbientColor(uint red, uint green, uint blue);
    void setDiffuseColor(uint red, uint green, uint blue);
    void setSpecularColor(uint red, uint green, uint blue);
    void setRoughness(int roughness);
    void setSpecularRoughness(int roughness);
    void setRefractionIndex(int refractionIndex);
    void setTextureChecked(Primitive::Textures x, bool status);
    void setTexture(Primitive::Textures x, QImage texture);
    void setDiffuseShader(int i);
    void setSpecularShader(int i);
    void setKa(int ka);
    void setKd(int kd);
    void setKs(int ks);

private:
    View *view_;
    Model *model_;

    MouseController *mousePerspective;
    MouseController *mouseFront;
    MouseController *mouseLeft;
    MouseController *mouseTop;





};

#endif // CONTROLLER_H
