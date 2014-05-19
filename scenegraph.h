#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QAbstractItemModel>
#include <primitive.h>

class Scenegraph : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit Scenegraph(QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void addPrimitive(Primitive *p);
    void deletePrimitive(Primitive *p);
    QList<Primitive*>* getPrimitiveList();
    Primitive* getPrimitive(QModelIndex index);

signals:
    void itemNameChanged();

public slots:

private:
    QList<Primitive*> *primitives_;

};

#endif // SCENEGRAPH_H
