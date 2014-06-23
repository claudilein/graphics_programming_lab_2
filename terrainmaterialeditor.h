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
    void materialSelected(QString fileName);
    void rangeChanged(int materialID, int minRange, int maxRange);

public slots:
    void uploadMaterial();
    //void editMinRange1(QString newRange);

private:
    QGridLayout *gridLayout;
    QPushButton *addMaterialButton;

    std::vector<QPixmap*> materialPixmaps;
    std::vector<QLabel*> materialLabels;
    std::vector<QLineEdit*> materialRangesMinEdit;
    std::vector<QLineEdit*> materialRangesMaxEdit;
    std::vector<int> materialRangesMin;
    std::vector<int> materialRangesMax;

    uint nrMaterials;



};

#endif // TERRAINMATERIALEDITOR_H
