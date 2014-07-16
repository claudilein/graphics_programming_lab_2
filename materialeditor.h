#ifndef MATERIALEDITOR_H
#define MATERIALEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QPixmap>
#include <primitive.h>

class MaterialEditor : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialEditor(QWidget *parent = 0);
    //~MaterialEditor();
    enum Component {RED, GREEN, BLUE};
    static const int NR_COMPONENTS = 3;



signals:
    void ambientColorChanged(uint red, uint green, uint blue);
    void diffuseColorChanged(uint red, uint green, uint blue);
    void specularColorChanged(uint red, uint green, uint blue);
    void roughnessChanged(int roughness);
    void refractionIndexChanged(int refractionIndex);
    void textureChecked(Primitive::Textures x, bool status);
    void textureUploaded(Primitive::Textures x, QImage texture);
    void diffuseShaderChanged(int id);
    void specularShaderChanged(int id);

public slots:
    void ambientComponentChanged(QString value);
    void diffuseComponentChanged(QString value);
    void specularComponentChanged(QString value);
    void textureChecked(bool status);
    void textureUploadInitialized();
    void loadMaterial();
    void saveMaterial();

private:

    QGridLayout *gridLayout;

    // Labels first row
    QLabel *componentLabels[NR_COMPONENTS];

    // ambient color
    QLabel *ambientLabel;
    QLineEdit *ambientEdits[NR_COMPONENTS];
    uint ambientColor[NR_COMPONENTS];

    // diffuse color
    QLabel *diffuseLabel;
    QLineEdit *diffuseEdits[NR_COMPONENTS];
    uint diffuseColor[NR_COMPONENTS];

    // specular color
    QLabel *specularLabel;
    QLineEdit *specularEdits[NR_COMPONENTS];
    uint specularColor[NR_COMPONENTS];

    // roughnessne Freundin getroffen
    QLabel *roughnessLabel;
    QSlider *roughnessSlider;

    // refraction index for fresnel term
    QLabel *refractionIndexLabel;
    QSlider *refractionIndexSlider;

    // texture uploads
    QLabel *texturesLabel;
    QCheckBox *textureCheckboxes[Primitive::NR_TEXTURES];
    QLabel *textureLabels[Primitive::NR_TEXTURES];
    QPushButton *textureUploadButtons[Primitive::NR_TEXTURES];
    QLabel *texturePreviewsLabels[Primitive::NR_TEXTURES];
    QPixmap *texturePreviewPixmaps[Primitive::NR_TEXTURES];
    QImage *textureImage[Primitive::NR_TEXTURES];
    bool hasTexture[Primitive::NR_TEXTURES];

    // shader buttons
    QLabel *shadersLabel;

    // QButtonGroup *diffuseShadersGroup;
    QButtonGroup *diffuseShadersGroup;
    QLabel *diffuseShadersLabel;
    QPushButton *noDiffuseButton;
    QPushButton *lambertButton;
    QPushButton *orenNayarButton;

    QButtonGroup *specularShadersGroup;
    QLabel *specularShadersLabel;
    QPushButton *noSpecularButton;
    QPushButton *phongButton;
    QPushButton *cookTorranceButton;

    // load and save buttons
    QPushButton *loadButton;
    QPushButton *saveButton;



    void uploadTexture(int index, QString fileName);
    QString componentToString(Component x);
    QString textureToString(Primitive::Textures x);
};

#endif // MATERIALEDITOR_H
