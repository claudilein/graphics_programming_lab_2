#ifndef TERRAINMATERIALEDITOR_H
#define TERRAINMATERIALEDITOR_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

class TerrainMaterialEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TerrainMaterialEditor(QWidget *parent = 0);

signals:
    void materialAdded(QString fileName);
    void materialDeleted(int i);

public slots:
    void uploadMaterial();
    void deleteMaterial();
  //  void selectMaterial();

private:
    QGridLayout *gridLayout;
    QPushButton *addMaterialButton;
    QPushButton *deleteMaterialButton;

    std::vector<QPixmap*> materialPixmaps;
    std::vector<QLabel*> materialLabels;
    //std::vector<QLabel*> labelBackgrounds

    uint nrMaterials;



};

#endif // TERRAINMATERIALEDITOR_H
