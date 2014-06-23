#include "terrainmaterialeditor.h"
#include <QFileDialog>

TerrainMaterialEditor::TerrainMaterialEditor(QWidget *parent) :
    QWidget(parent),
    nrMaterials(0)
{

    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);

    addMaterialButton = new QPushButton("Add Material", this);
    connect(addMaterialButton, SIGNAL(clicked()), this, SLOT(uploadMaterial()));

    gridLayout->addWidget(addMaterialButton, 0, 0);

    materialRangesMin.push_back(0);
    materialRangesMin.push_back(50);
    materialRangesMin.push_back(100);
    materialRangesMin.push_back(500);


    materialRangesMax.push_back(49);
    materialRangesMax.push_back(99);
    materialRangesMax.push_back(499);
    materialRangesMax.push_back(999);

}

void TerrainMaterialEditor::uploadMaterial() {
    nrMaterials++;

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Upload Material Image"), "../", tr("Image Files (*.jpg)"));

    materialPixmaps.push_back(new QPixmap(fileName));

    materialLabels.push_back(new QLabel());
    materialLabels.back()->setPixmap(*materialPixmaps.back());
    materialLabels.back()->setScaledContents(true);
    materialLabels.back()->setFixedSize(50, 50);

    materialRangesMinEdit.push_back(new QLineEdit(this));
    //connect(materialRangesMinEdit.back(), SIGNAL(textChanged(QString)), this, SLOT(editMinRange1(QString)));
    materialRangesMaxEdit.push_back(new QLineEdit(this));

    gridLayout->addWidget(materialLabels.back(), nrMaterials, 0);
    gridLayout->addWidget(materialRangesMinEdit.back(), nrMaterials, 1);
    gridLayout->addWidget(materialRangesMaxEdit.back(), nrMaterials, 2);

    emit materialSelected(fileName);
    emit rangeChanged(nrMaterials, materialRangesMin[nrMaterials], materialRangesMax[nrMaterials]);

}
