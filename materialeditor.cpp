#include "materialeditor.h"
#include <iostream>
#include <QGLWidget>
#include <QFileDialog>
#include <QStringList>
#include <QString>
#include <sstream>

MaterialEditor::MaterialEditor(QWidget *parent, MaterialPreview *preview) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);

    // create bold font for some labels
    QFont boldFont = QFont();
    boldFont.setBold(true);

    int row = 0;

    preview_ = preview;

    // position preview widget
    gridLayout->addWidget(preview_, row, 0, 1, 4, Qt::AlignHCenter);

    row++;

    // Labels first row
    for (int i = 0; i < NR_COMPONENTS; i++) {
        componentLabels[i] = new QLabel(this);
        componentLabels[i]->setText(componentToString(static_cast<Component>(i)));
        gridLayout->addWidget(componentLabels[i], row, i + 1, Qt::AlignHCenter);
    }
    row++;

    // ambient color labels & lineEdits
    ambientLabel = new QLabel(this);
    ambientLabel->setText("ambient");
    gridLayout->addWidget(ambientLabel, row, 0, Qt::AlignHCenter);

    for (int i = 0; i < NR_COMPONENTS; i++) {
        ambientColor[i] = 0;
        ambientEdits[i] = new QLineEdit(this);
        ambientEdits[i]->setInputMask("000");
        ambientEdits[i]->setText(QString::number(ambientColor[i]));
        ambientEdits[i]->setMaximumWidth(70);
        connect(ambientEdits[i], SIGNAL(textEdited(QString)), this, SLOT(ambientComponentChanged(QString)));

        gridLayout->addWidget(ambientEdits[i], row, i + 1, Qt::AlignHCenter);
    }
    row++;

    // diffuse color labels & lineEdits
    diffuseLabel = new QLabel(this);
    diffuseLabel->setText("diffuse");
    gridLayout->addWidget(diffuseLabel, row, 0, Qt::AlignHCenter);

    for (int i = 0; i < NR_COMPONENTS; i++) {
        diffuseColor[i] = 0;
        diffuseEdits[i] = new QLineEdit(this);
        diffuseEdits[i]->setInputMask("000");
        diffuseEdits[i]->setText(QString::number(diffuseColor[i]));
        diffuseEdits[i]->setMaximumWidth(70);
        connect(diffuseEdits[i], SIGNAL(textEdited(QString)), this, SLOT(diffuseComponentChanged(QString)));
        gridLayout->addWidget(diffuseEdits[i], row, i + 1, Qt::AlignHCenter);
    }
    row++;

    // specular color labels & lineEdits
    specularLabel = new QLabel(this);
    specularLabel->setText("specular");
    gridLayout->addWidget(specularLabel, row, 0, Qt::AlignHCenter);

    for (int i = 0; i < NR_COMPONENTS; i++) {
        specularColor[i] = 0;
        specularEdits[i] = new QLineEdit(this);
        specularEdits[i]->setInputMask("000");
        specularEdits[i]->setText(QString::number(specularColor[i]));
        specularEdits[i]->setMaximumWidth(70);
        connect(specularEdits[i], SIGNAL(textEdited(QString)), this, SLOT(specularComponentChanged(QString)));
        gridLayout->addWidget(specularEdits[i], row, i + 1, Qt::AlignHCenter);
    }
    row++;


    // ka, kd, ks factors
    factorsLabel = new QLabel(this);
    factorsLabel->setText("ka / kd / ks");
    gridLayout->addWidget(factorsLabel, row, 0, Qt::AlignHCenter);

    kaSlider = new QSlider(Qt::Horizontal, this);
    kaSlider->setRange(0, 100);
    kaSlider->setFixedWidth(70);
    kaSlider->setValue(100);
    connect(kaSlider, SIGNAL(valueChanged(int)), this, SIGNAL(kaChanged(int)));
    connect(kaSlider, SIGNAL(valueChanged(int)), this, SLOT(kaChangedSlot(int)));
    gridLayout->addWidget(kaSlider, row, 1, Qt::AlignHCenter);

    kdSlider = new QSlider(Qt::Horizontal, this);
    kdSlider->setRange(0, 100);
    kdSlider->setFixedWidth(70);
    kdSlider->setValue(100);
    connect(kdSlider, SIGNAL(valueChanged(int)), this, SIGNAL(kdChanged(int)));
    connect(kdSlider, SIGNAL(valueChanged(int)), this, SLOT(kdChangedSlot(int)));
    gridLayout->addWidget(kdSlider, row, 2, Qt::AlignHCenter);

    ksSlider = new QSlider(Qt::Horizontal, this);
    ksSlider->setRange(0, 100);
    ksSlider->setFixedWidth(70);
    ksSlider->setValue(100);
    connect(ksSlider, SIGNAL(valueChanged(int)), this, SIGNAL(ksChanged(int)));
    connect(ksSlider, SIGNAL(valueChanged(int)), this, SLOT(ksChangedSlot(int)));
    gridLayout->addWidget(ksSlider, row, 3, Qt::AlignHCenter);

    row++;

    // roughness label & slider
    roughnessLabel = new QLabel(this);
    roughnessLabel->setText("roughness d/s");
    gridLayout->addWidget(roughnessLabel, row, 0, Qt::AlignHCenter);

    roughnessSlider = new QSlider(Qt::Horizontal, this);
    roughnessSlider->setRange(1, 99);
    roughnessSlider->setFixedWidth(70);
    roughnessSlider->setValue(50);
    connect(roughnessSlider, SIGNAL(valueChanged(int)), this, SIGNAL(roughnessChanged(int)));
    gridLayout->addWidget(roughnessSlider, row, 1, 1, 1, Qt::AlignLeft);

    roughnessSpecularSlider = new QSlider(Qt::Horizontal, this);
    roughnessSpecularSlider->setRange(1, 99);
    roughnessSpecularSlider->setFixedWidth(70);
    roughnessSpecularSlider->setValue(50);
    connect(roughnessSpecularSlider, SIGNAL(valueChanged(int)), this, SIGNAL(roughnessSpecularChanged(int)));
    gridLayout->addWidget(roughnessSpecularSlider, row, 2, 1, 1, Qt::AlignLeft);

    row++;

    // refractionIndex label & slider
    refractionIndexLabel = new QLabel(this);
    refractionIndexLabel->setText("refraction \n index");
    gridLayout->addWidget(refractionIndexLabel, row, 0, Qt::AlignHCenter);


    refractionIndexSlider = new QSlider(Qt::Horizontal, this);
    refractionIndexSlider->setRange(0, 100);
    refractionIndexSlider->setFixedWidth(100);
    refractionIndexSlider->setValue(50);
    connect(refractionIndexSlider, SIGNAL(valueChanged(int)), this, SIGNAL(refractionIndexChanged(int)));
    gridLayout->addWidget(refractionIndexSlider, row, 1, 1, 2, Qt::AlignLeft);

    row++;

    // texture uploads

    texturesLabel = new QLabel("Textures", this);
    texturesLabel->setFont(boldFont);
    gridLayout->addWidget(texturesLabel, row, 0, Qt::AlignHCenter);

    row++;

    for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
        textureCheckboxes[i] = new QCheckBox(this);
        connect(textureCheckboxes[i], SIGNAL(toggled(bool)), this, SLOT(textureChecked(bool)));
        gridLayout->addWidget(textureCheckboxes[i], row + i, 0, Qt::AlignRight);

        textureLabels[i] = new QLabel(textureToString(static_cast<Primitive::Textures>(i)), this);
        gridLayout->addWidget(textureLabels[i], row + i, 1, Qt::AlignHCenter);

        textureUploadButtons[i] = new QPushButton("Upload", this);
        textureUploadButtons[i]->setFixedWidth(70);
        connect(textureUploadButtons[i], SIGNAL(clicked()), this, SLOT(textureUploadInitialized()));
        gridLayout->addWidget(textureUploadButtons[i], row + i, 2, Qt::AlignLeft);

        texturePreviewPixmaps[i] = new QPixmap();
        texturePreviewsLabels[i] = new QLabel(this);
        texturePreviewsLabels[i]->setPixmap(*texturePreviewPixmaps[i]);
        texturePreviewsLabels[i]->setScaledContents(true);
        texturePreviewsLabels[i]->setFixedSize(40, 40);
        gridLayout->addWidget(texturePreviewsLabels[i], row + i, 3, Qt::AlignHCenter);

        textureImage[i] = new QImage();
        hasTexture[i] = false;
    }

    row += Primitive::NR_TEXTURES;

    // diffuse shaders group
    diffuseShadersLabel = new QLabel("Diffuse shaders", this);
    diffuseShadersLabel->setFont(boldFont);
    gridLayout->addWidget(diffuseShadersLabel, row, 0, 1, 2, Qt::AlignHCenter);

    noDiffuseButton = new QPushButton("none", this);
    noDiffuseButton->setCheckable(true);
    noDiffuseButton->setChecked(true);
    lambertButton = new QPushButton("Lambert", this);
    lambertButton->setCheckable(true);
    orenNayarButton = new QPushButton("Oren-Nayar", this);
    orenNayarButton->setCheckable(true);

    diffuseShadersGroup = new QButtonGroup(this);
    diffuseShadersGroup->addButton(noDiffuseButton, 0);
    diffuseShadersGroup->addButton(lambertButton, 1);
    diffuseShadersGroup->addButton(orenNayarButton, 2);

    gridLayout->addWidget(noDiffuseButton, row + 1, 0, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(lambertButton, row + 2, 0, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(orenNayarButton, row + 3, 0, 1, 2, Qt::AlignHCenter);

    connect(diffuseShadersGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(diffuseShaderChanged(int)));
    connect(diffuseShadersGroup, SIGNAL(buttonClicked(int)), this, SLOT(diffuseShaderChangedSlot(int)));

    // specular shaders group
    specularShadersLabel = new QLabel("Specular shaders", this);
    specularShadersLabel->setFont(boldFont);
    gridLayout->addWidget(specularShadersLabel, row, 2, 1, 2, Qt::AlignHCenter);

    noSpecularButton = new QPushButton("none", this);
    noSpecularButton->setCheckable(true);
    noSpecularButton->setChecked(true);
    phongButton = new QPushButton("Phong", this);
    phongButton->setCheckable(true);
    cookTorranceButton = new QPushButton("Cook-Torrance", this);
    cookTorranceButton->setCheckable(true);

    specularShadersGroup = new QButtonGroup(this);
    specularShadersGroup->addButton(noSpecularButton, 0);
    specularShadersGroup->addButton(phongButton, 1);
    specularShadersGroup->addButton(cookTorranceButton, 2);

    gridLayout->addWidget(noSpecularButton, row + 1, 2, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(phongButton, row + 2, 2, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(cookTorranceButton, row + 3, 2, 1, 2, Qt::AlignHCenter);

    connect(specularShadersGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(specularShaderChanged(int)));
    connect(specularShadersGroup, SIGNAL(buttonClicked(int)), this, SLOT(specularShaderChangedSlot(int)));

    row += 4;

    // load and save buttons
    loadButton = new QPushButton("Load Material", this);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadMaterial()));
    saveButton = new QPushButton("Save Material", this);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveMaterial()));

    gridLayout->addWidget(loadButton, row, 0, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(saveButton, row, 2, 1, 2, Qt::AlignHCenter);

    row++;

    // create line to separate sections
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Plain);
    gridLayout->addWidget(separator, row, 0, 1, 4, Qt::AlignLeft);





    resetProperties();

}


void MaterialEditor::adjustToPrimitive(Primitive *p) {
    if (p != NULL) {
        preview_->setEnabled(true);

        for (int i = 0; i < NR_COMPONENTS; i++) {
            ambientEdits[i]->setEnabled(true);
            diffuseEdits[i]->setEnabled(true);
            specularEdits[i]->setEnabled(true);
        }

        ambientEdits[0]->setText(QString::number(p->getAmbientColor().x_ * 255));
        ambientEdits[1]->setText(QString::number(p->getAmbientColor().y_ * 255));
        ambientEdits[2]->setText(QString::number(p->getAmbientColor().z_ * 255));
        emit ambientColorChanged(ambientEdits[0]->text().toInt(), ambientEdits[1]->text().toInt(), ambientEdits[2]->text().toInt());

        diffuseEdits[0]->setText(QString::number(p->getDiffuseColor().x_ * 255));
        diffuseEdits[1]->setText(QString::number(p->getDiffuseColor().y_ * 255));
        diffuseEdits[2]->setText(QString::number(p->getDiffuseColor().z_ * 255));
        emit diffuseColorChanged(diffuseEdits[0]->text().toInt(), diffuseEdits[1]->text().toInt(), diffuseEdits[2]->text().toInt());

        specularEdits[0]->setText(QString::number(p->getSpecularColor().x_ * 255));
        specularEdits[1]->setText(QString::number(p->getSpecularColor().y_ * 255));
        specularEdits[2]->setText(QString::number(p->getSpecularColor().z_ * 255));
        emit specularColorChanged(specularEdits[0]->text().toInt(), specularEdits[1]->text().toInt(), specularEdits[2]->text().toInt());

        kaSlider->setValue(p->getKa() * 100);
        kaSlider->setEnabled(true);
        kdSlider->setValue(p->getKd() * 100);
        kdSlider->setEnabled(true);
        ksSlider->setValue(p->getKs() * 100);
        ksSlider->setEnabled(true);

        roughnessSlider->setValue(p->getRoughness() * 100);
        roughnessSlider->setEnabled(true);
        roughnessSpecularSlider->setValue(p->getSpecularRoughness() * 100);
        roughnessSpecularSlider->setEnabled(true);

        refractionIndexSlider->setValue((p->getRefractionIndex() - 1) / 3 * 100);
        refractionIndexSlider->setEnabled(true);

        int id = p->getDiffuseShader();
        switch (id) {
        case 0: noDiffuseButton->setChecked(true); break;
        case 1: lambertButton->setChecked(true); break;
        case 2: orenNayarButton->setChecked(true); break;
        default: noDiffuseButton->setChecked(true); break;
        }

        emit diffuseShadersGroup->buttonClicked(id);


        id = p->getSpecularShader();
        switch (id) {
        case 0: noSpecularButton->setChecked(true); break;
        case 1: phongButton->setChecked(true); break;
        case 2: cookTorranceButton->setChecked(true); break;
        default: noSpecularButton->setChecked(true); break;
        }

        emit specularShadersGroup->buttonClicked(id);

        noDiffuseButton->setEnabled(true);
        lambertButton->setEnabled(true);
        orenNayarButton->setEnabled(true);
        noSpecularButton->setEnabled(true);
        phongButton->setEnabled(true);
        cookTorranceButton->setEnabled(true);
        saveButton->setEnabled(true);
        loadButton->setEnabled(true);

        for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
            if (p->isTextureActive(static_cast<Primitive::Textures>(i))) {
                textureCheckboxes[i]->setChecked(true);
            } else {
                textureCheckboxes[i]->setChecked(false);
            }

            textureCheckboxes[i]->setEnabled(true);
            textureUploadButtons[i]->setEnabled(true);

            textureImage[i] = p->getTexture(static_cast<Primitive::Textures>(i));
            *texturePreviewPixmaps[i] = QPixmap::fromImage(*textureImage[i]);
            texturePreviewsLabels[i]->setPixmap(*texturePreviewPixmaps[i]);


            if (textureImage[i]->width() > 0) {
                std::cout << "Primitive " << p->getID() << " has texture " << i << std::endl;
                // upload texture to preview, but not again to the selected primitive, so not emitting the textureUploaded() signal here
                preview_->setTexture(static_cast<Primitive::Textures>(i), *textureImage[i]);
                hasTexture[i] = true;
            } else {
                hasTexture[i] = false;
            }
        }
    } else {
        resetProperties();
    }
}

void MaterialEditor::resetProperties() {
    preview_->setEnabled(false);

    for (int i = 0; i < NR_COMPONENTS; i++) {
        ambientEdits[i]->setText(QString::number(0));
        diffuseEdits[i]->setText(QString::number(0));
        specularEdits[i]->setText(QString::number(0));
        ambientEdits[i]->setEnabled(false);
        diffuseEdits[i]->setEnabled(false);
        specularEdits[i]->setEnabled(false);
    }
    emit ambientColorChanged(ambientEdits[0]->text().toInt(), ambientEdits[1]->text().toInt(), ambientEdits[2]->text().toInt());
    emit diffuseColorChanged(diffuseEdits[0]->text().toInt(), diffuseEdits[1]->text().toInt(), diffuseEdits[2]->text().toInt());
    emit specularColorChanged(specularEdits[0]->text().toInt(), specularEdits[1]->text().toInt(), specularEdits[2]->text().toInt());

    kaSlider->setValue(100);
    kaSlider->setEnabled(false);
    kdSlider->setValue(100);
    kdSlider->setEnabled(false);
    ksSlider->setValue(100);
    ksSlider->setEnabled(false);

    roughnessSlider->setValue(50);
    roughnessSlider->setEnabled(false);
    roughnessSpecularSlider->setValue(50);
    roughnessSpecularSlider->setEnabled(false);

    refractionIndexSlider->setValue(50);
    refractionIndexSlider->setEnabled(false);

    for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
        hasTexture[i] = false;
        textureCheckboxes[i]->setChecked(false);
        textureCheckboxes[i]->setEnabled(false);
        texturePreviewPixmaps[i] = new QPixmap();
        texturePreviewsLabels[i]->setPixmap(*texturePreviewPixmaps[i]);
        textureUploadButtons[i]->setEnabled(false);
    }
    noDiffuseButton->setChecked(true);
    noSpecularButton->setChecked(true);

    emit diffuseShadersGroup->buttonClicked(0);
    emit specularShadersGroup->buttonClicked(0);

    noDiffuseButton->setEnabled(false);
    lambertButton->setEnabled(false);
    orenNayarButton->setEnabled(false);
    noSpecularButton->setEnabled(false);
    phongButton->setEnabled(false);
    cookTorranceButton->setEnabled(false);
    saveButton->setEnabled(false);
    loadButton->setEnabled(false);
}

void MaterialEditor::loadMaterial() {

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Load Material"), "../", tr("Material files (*.mat)"));
    QFile file(fileName);

    if (file.open(QFile::ReadOnly)) {
        int maxLineLength = 128;

        // read in the colors
        QByteArray line = file.readLine(maxLineLength);
        QStringList rgb = QString(line).split(' ');
        for (int i = 0; i < NR_COMPONENTS; i++) {
            ambientColor[i] = rgb[i].toInt();
            ambientEdits[i]->setText(rgb[i]);
        }
        emit ambientColorChanged(ambientColor[RED], ambientColor[GREEN], ambientColor[BLUE]);

        line = file.readLine(maxLineLength);
        rgb = QString(line).split(' ');
        for (int i = 0; i < NR_COMPONENTS; i++) {
            diffuseColor[i] = rgb[i].toInt();
            diffuseEdits[i]->setText(rgb[i]);
        }
        emit diffuseColorChanged(diffuseColor[RED], diffuseColor[GREEN], diffuseColor[BLUE]);

        line = file.readLine(maxLineLength);
        rgb = QString(line).split(' ');
        for (int i = 0; i < NR_COMPONENTS; i++) {
            specularColor[i] = rgb[i].toInt();
            specularEdits[i]->setText(rgb[i]);
        }
        emit specularColorChanged(specularColor[RED], specularColor[GREEN], specularColor[BLUE]);

        line = file.readLine(maxLineLength);
        QStringList value = QString(line).split(' ');

        int ka = value[0].toInt();
        int kd = value[1].toInt();
        int ks = value[2].toInt();
        kaSlider->setValue(ka);
        kdSlider->setValue(kd);
        ksSlider->setValue(ks);
        emit kaChanged(ka);
        emit kdChanged(kd);
        emit ksChanged(ks);

        // read in roughness and refraction index
        line = file.readLine(maxLineLength);
        value = QString(line).split(' ');
        int roughness = value[0].toInt();
        roughnessSlider->setValue(roughness);
        emit roughnessChanged(roughness);

        line = file.readLine(maxLineLength);
        value = QString(line).split(' ');
        roughness = value[0].toInt();
        roughnessSpecularSlider->setValue(roughness);
        emit roughnessSpecularChanged(roughness);

        line = file.readLine(maxLineLength);
        value = QString(line).split(' ');
        int refractionIndex = value[0].toInt();
        refractionIndexSlider->setValue(refractionIndex);
        emit refractionIndexChanged(refractionIndex);

        // read in checked shaders
        line = file.readLine(maxLineLength);
        value = QString(line).split(' ');
        int id = value[0].toInt();
        switch (id) {
        case 0: noDiffuseButton->setChecked(true); break;
        case 1: lambertButton->setChecked(true); break;
        case 2: orenNayarButton->setChecked(true); break;
        default: noDiffuseButton->setChecked(true); break;
        }

        emit diffuseShadersGroup->buttonClicked(id);

        line = file.readLine(maxLineLength);
        value = QString(line).split(' ');
        id = value[0].toInt();
        switch (id) {
        case 0: noSpecularButton->setChecked(true); break;
        case 1: phongButton->setChecked(true); break;
        case 2: cookTorranceButton->setChecked(true); break;
        default: noSpecularButton->setChecked(true); break;
        }

        emit specularShadersGroup->buttonClicked(id);

        QStringList textureInfo;
        for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
            line = file.readLine(maxLineLength);
            textureInfo = QString(line).split('$');

            if (textureInfo[0].toInt() == 0) {
                textureCheckboxes[i]->setChecked(false);
            } else {
                textureCheckboxes[i]->setChecked(true);
            }


            // if texture for this property was stored
            if (textureInfo.size() == 2) {
                uploadTexture(i, textureInfo[1].trimmed());
            } else {
                hasTexture[i] = false;
            }
        }

        file.close();
    } else {
        std::cout << "Could not open file " << fileName.toStdString() << std::endl;
    }
}

void MaterialEditor::saveMaterial() {
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("Save Material"), "../", tr("Material files(*.mat")) + ".mat";
    QFile file(fileName);

    if (file.open(QFile::WriteOnly)) {
        std::stringstream data;

        for (int i = 0; i < NR_COMPONENTS; i++) {
            data << ambientColor[i] << " ";
        }
        data << "\n";
        for (int i = 0; i < NR_COMPONENTS; i++) {
            data << diffuseColor[i] << " ";
        }
        data << "\n";
        for (int i = 0; i < NR_COMPONENTS; i++) {
            data << specularColor[i] << " ";
        }
        data << "\n";

        data << kaSlider->value() << " " << kdSlider->value() << " " << ksSlider->value() << "\n";

        data << roughnessSlider->value() << "\n";
        data << roughnessSpecularSlider->value() << "\n";
        data << refractionIndexSlider->value() << "\n";

        data << diffuseShadersGroup->checkedId() << "\n";
        data << specularShadersGroup->checkedId() << "\n";

        std::cout << "before saving textures" << std::endl;

        for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
            data << int(textureCheckboxes[i]->isChecked());

            QString texFileName = "";
            if (hasTexture[i]) {
                texFileName = fileName + "-" + QString::number(i) + ".jpg";
                textureImage[i]->save(texFileName);
                data << "$" << texFileName.toStdString();
            }
            data << "\n";
        }

        file.write(data.str().data());
        file.close();

    } else {
        std::cout << "File " << fileName.toStdString() << "could not be saved." << std::endl;
    }
}

void MaterialEditor::textureUploadInitialized() {
    Primitive::Textures x = static_cast<Primitive::Textures>(0);
    for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
        if (sender() == textureUploadButtons[i]) {
            x = static_cast<Primitive::Textures>(i);
        }
    }

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
        tr("Upload Texture"), "../", tr("Image Files (*.jpg)"));
    uploadTexture(x, fileName);
}

void MaterialEditor::uploadTexture(int index, QString fileName) {
    if (fileName != "") {
        std::cout << "uploading texture: " << fileName.toStdString() << " to index " << index << std::endl;
        textureImage[index] = new QImage(fileName);
        hasTexture[index] = true;
        *texturePreviewPixmaps[index] = QPixmap::fromImage(*textureImage[index]);
        texturePreviewsLabels[index]->setPixmap(*texturePreviewPixmaps[index]);
        emit textureUploaded(static_cast<Primitive::Textures>(index), *textureImage[index]);
    } else {
        std::cout << "texture upload failed." << std::endl;
    }
}

void MaterialEditor::textureChecked(bool status) {
    Primitive::Textures x = static_cast<Primitive::Textures>(0);
    for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
        if (sender() == textureCheckboxes[i]) {
            x = static_cast<Primitive::Textures>(i);
            std::cout << "textureChecked() was set to " << status << " for texture " << x << std::endl;
        }
    }

    emit textureChecked(x, status);
}

void MaterialEditor::ambientComponentChanged(QString value) {

    uint color = value.toInt();
    if (color > 255) color = 255;

    if (sender() == ambientEdits[RED]) {
        ambientColor[RED] = color;
    } else if (sender() == ambientEdits[GREEN]) {
        ambientColor[GREEN] = color;
    } else if (sender() == ambientEdits[BLUE]) {
        ambientColor[BLUE] = color;
    }

    emit ambientColorChanged(ambientColor[RED], ambientColor[GREEN], ambientColor[BLUE]);
}

void MaterialEditor::diffuseComponentChanged(QString value) {
    uint color = value.toInt();
    if (color > 255) color = 255;

    if (sender() == diffuseEdits[RED]) {
        diffuseColor[RED] = color;
    } else if (sender() == diffuseEdits[GREEN]) {
        diffuseColor[GREEN] = color;
    } else if (sender() == diffuseEdits[BLUE]) {
        diffuseColor[BLUE] = color;
    }

    emit diffuseColorChanged(diffuseColor[RED], diffuseColor[GREEN], diffuseColor[BLUE]);
}

void MaterialEditor::specularComponentChanged(QString value) {
    uint color = value.toInt();
    if (color > 255) color = 255;

    if (sender() == specularEdits[RED]) {
        specularColor[RED] = color;
    } else if (sender() == specularEdits[GREEN]) {
        specularColor[GREEN] = color;
    } else if (sender() == specularEdits[BLUE]) {
        specularColor[BLUE] = color;
    }

    emit specularColorChanged(specularColor[RED], specularColor[GREEN], specularColor[BLUE]);
}

QString MaterialEditor::componentToString(Component x) {
    switch (x) {
    case RED: return "red";
    case GREEN: return "green";
    case BLUE: return "blue";
    default: return "default";
    }
}

QString MaterialEditor::textureToString(Primitive::Textures x) {
    switch (x) {
    case Primitive::AMBIENT: return "ambient";
    case Primitive::DIFFUSE: return "diffuse";
    case Primitive::SPECULAR: return "specular";
    case Primitive::ROUGHNESS_D: return "roughness \n diffuse";
    case Primitive::REFRACTION_INDEX: return "refraction \n index";
    case Primitive::NORMAL: return "normal";
    //case Primitive::ROUGHNESS_S: return "roughness \n specular";
    default: return "default";
    }
}

void MaterialEditor::diffuseShaderChangedSlot(int id) {
    //if (id == 0)
}

void MaterialEditor::specularShaderChangedSlot(int id) {
    //if (id == 0)
}

void MaterialEditor::kaChangedSlot(int value) {
    /*int ka = value;
    int kd = kdSlider->value();
    int ks = ksSlider->value();

    int total = ka + kd + ks;
    int rest = 0;
    int over = total - 100;
    int overHalf = over / 2;
    if (over % 2 == 1 && over > 0) rest = 1;
    if (over % 2 == 1 && over < 0) rest = -1;

    if (changedKd) {
        changedKd = false;
        kdSlider->setValue(kd - overHalf);
        ksSlider->setValue(ks - (overHalf + rest));
    } else {
        changedKd = true;
        kdSlider->setValue(kd - (overHalf + rest));
        ksSlider->setValue(ks - overHalf);
    }
    std::cout << "ka: " << ka << ", kd: " << kdSlider->value() << ", ks: " << ksSlider->value() << std::endl;
*/
}

void MaterialEditor::kdChangedSlot(int value) {

}

void MaterialEditor::ksChangedSlot(int value) {

}
