#include "materialeditor.h"
#include <iostream>
#include <QGLWidget>
#include <QFileDialog>
#include <QStringList>
#include <QString>
#include <sstream>

MaterialEditor::MaterialEditor(QWidget *parent) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);

    // create bold font for some labels
    QFont boldFont = QFont();
    boldFont.setBold(true);


    // Labels first row
    for (int i = 0; i < NR_COMPONENTS; i++) {
        componentLabels[i] = new QLabel(this);
        componentLabels[i]->setText(componentToString(static_cast<Component>(i)));
        gridLayout->addWidget(componentLabels[i], 0, i + 1, Qt::AlignHCenter);
    }

    // ambient color labels & lineEdits
    ambientLabel = new QLabel(this);
    ambientLabel->setText("ambient");
    gridLayout->addWidget(ambientLabel, 1, 0, Qt::AlignHCenter);

    for (int i = 0; i < NR_COMPONENTS; i++) {
        ambientColor[i] = 255;
        ambientEdits[i] = new QLineEdit(this);
        ambientEdits[i]->setInputMask("000");
        ambientEdits[i]->setText(QString::number(ambientColor[i]));
        connect(ambientEdits[i], SIGNAL(textChanged(QString)), this, SLOT(ambientComponentChanged(QString)));
        gridLayout->addWidget(ambientEdits[i], 1, i + 1, Qt::AlignHCenter);
    }

    // diffuse color labels & lineEdits
    diffuseLabel = new QLabel(this);
    diffuseLabel->setText("diffuse");
    gridLayout->addWidget(diffuseLabel, 2, 0, Qt::AlignHCenter);

    for (int i = 0; i < NR_COMPONENTS; i++) {
        diffuseColor[i] = 255;
        diffuseEdits[i] = new QLineEdit(this);
        diffuseEdits[i]->setInputMask("000");
        diffuseEdits[i]->setText(QString::number(diffuseColor[i]));
        connect(diffuseEdits[i], SIGNAL(textChanged(QString)), this, SLOT(diffuseComponentChanged(QString)));
        gridLayout->addWidget(diffuseEdits[i], 2, i + 1, Qt::AlignHCenter);
    }

    // specular color labels & lineEdits
    specularLabel = new QLabel(this);
    specularLabel->setText("specular");
    gridLayout->addWidget(specularLabel, 3, 0, Qt::AlignHCenter);

    for (int i = 0; i < NR_COMPONENTS; i++) {
        specularColor[i] = 255;
        specularEdits[i] = new QLineEdit(this);
        specularEdits[i]->setInputMask("000");
        specularEdits[i]->setText(QString::number(specularColor[i]));
        connect(specularEdits[i], SIGNAL(textChanged(QString)), this, SLOT(specularComponentChanged(QString)));
        gridLayout->addWidget(specularEdits[i], 3, i + 1, Qt::AlignHCenter);
    }

    // roughness label & slider
    roughnessLabel = new QLabel(this);
    roughnessLabel->setText("roughness");
    gridLayout->addWidget(roughnessLabel, 4, 0, Qt::AlignHCenter);

    roughnessSlider = new QSlider(Qt::Horizontal, this);
    roughnessSlider->setRange(0, 100);
    roughnessSlider->setFixedWidth(100);
    roughnessSlider->setValue(100);
    connect(roughnessSlider, SIGNAL(valueChanged(int)), this, SIGNAL(roughnessChanged(int)));
    gridLayout->addWidget(roughnessSlider, 4, 1, 1, 2, Qt::AlignLeft);

    // refractionIndex label & slider
    refractionIndexLabel = new QLabel(this);
    refractionIndexLabel->setText("refraction \n index");
    gridLayout->addWidget(refractionIndexLabel, 5, 0, Qt::AlignHCenter);


    refractionIndexSlider = new QSlider(Qt::Horizontal, this);
    refractionIndexSlider->setRange(0, 100);
    refractionIndexSlider->setFixedWidth(100);
    refractionIndexSlider->setValue(100);
    connect(refractionIndexSlider, SIGNAL(valueChanged(int)), this, SIGNAL(refractionIndexChanged(int)));
    gridLayout->addWidget(refractionIndexSlider, 5, 1, 1, 2, Qt::AlignLeft);


    // texture uploads

    texturesLabel = new QLabel("Textures", this);
    texturesLabel->setFont(boldFont);
    gridLayout->addWidget(texturesLabel, 6, 0, Qt::AlignHCenter);

    for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
        textureCheckboxes[i] = new QCheckBox(this);
        connect(textureCheckboxes[i], SIGNAL(toggled(bool)), this, SLOT(textureChecked(bool)));
        gridLayout->addWidget(textureCheckboxes[i], 7 + i, 0, Qt::AlignRight);

        textureLabels[i] = new QLabel(textureToString(static_cast<Primitive::Textures>(i)), this);
        gridLayout->addWidget(textureLabels[i], 7 + i, 1, Qt::AlignHCenter);

        textureUploadButtons[i] = new QPushButton("Upload", this);
        textureUploadButtons[i]->setFixedWidth(70);
        connect(textureUploadButtons[i], SIGNAL(clicked()), this, SLOT(textureUploadInitialized()));
        gridLayout->addWidget(textureUploadButtons[i], 7 + i, 2, Qt::AlignLeft);

        texturePreviewPixmaps[i] = new QPixmap();
        texturePreviewsLabels[i] = new QLabel(this);
        texturePreviewsLabels[i]->setPixmap(*texturePreviewPixmaps[i]);
        texturePreviewsLabels[i]->setScaledContents(true);
        texturePreviewsLabels[i]->setFixedSize(40, 40);
        gridLayout->addWidget(texturePreviewsLabels[i], 7 + i, 3, Qt::AlignHCenter);

        textureImage[i] = new QImage();
        hasTexture[i] = false;
    }

    int line = 7 + Primitive::NR_TEXTURES;

    // diffuse shaders group
    diffuseShadersLabel = new QLabel("Diffuse shaders", this);
    diffuseShadersLabel->setFont(boldFont);
    gridLayout->addWidget(diffuseShadersLabel, line, 0, 1, 2, Qt::AlignHCenter);

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

    gridLayout->addWidget(noDiffuseButton, line + 1, 0, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(lambertButton, line + 2, 0, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(orenNayarButton, line + 3, 0, 1, 2, Qt::AlignHCenter);

    connect(diffuseShadersGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(diffuseShaderChanged(int)));

    // specular shaders group
    specularShadersLabel = new QLabel("Specular shaders", this);
    specularShadersLabel->setFont(boldFont);
    gridLayout->addWidget(specularShadersLabel, line, 2, 1, 2, Qt::AlignHCenter);

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

    gridLayout->addWidget(noSpecularButton, line + 1, 2, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(phongButton, line + 2, 2, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(cookTorranceButton, line + 3, 2, 1, 2, Qt::AlignHCenter);

    connect(specularShadersGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(specularShaderChanged(int)));

    // load and save buttons
    loadButton = new QPushButton("Load Material", this);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadMaterial()));
    saveButton = new QPushButton("Save Material", this);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveMaterial()));

    gridLayout->addWidget(loadButton, line + 4, 0, 1, 2, Qt::AlignHCenter);
    gridLayout->addWidget(saveButton, line + 4, 2, 1, 2, Qt::AlignHCenter);



    // create line to separate sections
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Plain);
    gridLayout->addWidget(separator, line + 6, 0, 1, 4, Qt::AlignLeft);










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

        // read in roughness and refraction index
        line = file.readLine(maxLineLength);
        QStringList value = QString(line).split(' ');
        int roughness = value[0].toInt();
        roughnessSlider->setValue(roughness);
        emit roughnessChanged(roughness);

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

        data << roughnessSlider->value() << "\n";
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
        QImage texture = QGLWidget::convertToGLFormat(*textureImage[index]);
        delete texturePreviewPixmaps[index];
        texturePreviewPixmaps[index] = new QPixmap(fileName);
        texturePreviewsLabels[index]->setPixmap(*texturePreviewPixmaps[index]);
        emit textureUploaded(static_cast<Primitive::Textures>(index), texture);
    } else {
        std::cout << "texture upload failed." << std::endl;
    }
}

void MaterialEditor::textureChecked(bool status) {
    Primitive::Textures x = static_cast<Primitive::Textures>(0);
    for (int i = 0; i < Primitive::NR_TEXTURES; i++) {
        if (sender() == textureCheckboxes[i]) {
            x = static_cast<Primitive::Textures>(i);
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
    case Primitive::ROUGHNESS: return "roughness";
    case Primitive::REFRACTION_INDEX: return "refraction \n index";
    case Primitive::NORMAL: return "normal";
    default: return "default";
    }
}
/*
MaterialEditor::~MaterialEditor() {
    for (int i = 0; i < NR_COMPONENTS; i++) {
        delete componentLabels[i];
        delete ambientEdits[i];
        delete diffuseEdits[i];
        delete specularEdits[i];
    }

    delete ambientLabel;
    delete diffuseLabel;
    delete specularLabel;
}
*/
