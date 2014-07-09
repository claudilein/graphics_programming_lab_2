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

    //deleteMaterialButton = new QPushButton("Delete Material", this);
    //connect(deleteMaterialButton, SIGNAL(clicked()), this, SLOT(deleteMaterial()));

    gridLayout->addWidget(addMaterialButton, 0, 0);
    //gridLayout->addWidget(deleteMaterialButton, 0, 1);


}

void TerrainMaterialEditor::uploadMaterial() {
    nrMaterials++;

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Upload Material Image"), "../", tr("Image Files (*.jpg)"));
    if (fileName != "") {

        materialPixmaps.push_back(new QPixmap(fileName));

        materialLabels.push_back(new QLabel());
        materialLabels.back()->setPixmap(*materialPixmaps.back());
        materialLabels.back()->setScaledContents(true);
        materialLabels.back()->setFixedSize(50, 50);
        //connect(materialLabels.back(), SIGNAL(clicked), this, selectMaterial());

        gridLayout->addWidget(materialLabels.back(), nrMaterials, 0);

        emit materialAdded(fileName);
    }

}

void TerrainMaterialEditor::deleteMaterial() {
    /*
    for (int i = 0; i < materialLabels.size(); i++) {
        if (materialLabels[i]->isEnabled()) {
            emit materialDeleted(i);
        }
    }
    */
    emit materialDeleted(materialLabels.size() - 1);
    materialLabels.pop_back();
}

/*
 *void TerrainMaterialEditor::selectMaterial(int x) {
    for (int i = 0; i < materialLabels.size(); i++) {
        materialLabels[i]->setEnabled(false);
    }
    materialLabels[i]->setEnabled(true);
}
*/
