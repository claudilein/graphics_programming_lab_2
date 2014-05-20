#include "scenegraph.h"
#include <primitive.h>
#include <iostream>

Scenegraph::Scenegraph(QObject *parent) :
    QAbstractItemModel(parent)
{
    primitives_ = new QList<Primitive*>();
}

QModelIndex Scenegraph::index(int row, int column, const QModelIndex &parent) const {

    QModelIndex index = QModelIndex();
    if (hasIndex(row, column, parent)) {
        if (row < primitives_->size()) {
            index = createIndex(row, column, primitives_->at(row));
        }
    }

    return index;

}

QModelIndex Scenegraph::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int Scenegraph::rowCount(const QModelIndex &parent) const
{
    int count = 0;
    if (!parent.isValid()) {
        count = primitives_->size();
    }

    return count;

}

int Scenegraph::columnCount(const QModelIndex &parent) const
{
    int count = 0;
    if (!parent.isValid()) {
        count = 1;
    }
    return count;

}

QVariant Scenegraph::headerData(int section, Qt::Orientation orientation, int role) const {
    section;
    QVariant header = QVariant();
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) header = "Scene";
    return header;
}

QVariant Scenegraph::data(const QModelIndex &index, int role) const
{
    QVariant data = QVariant();
    if (index.isValid() && role == Qt::DisplayRole)
        data = QVariant(QString::fromStdString(static_cast<Primitive*>(index.internalPointer())->getName()));
    return data;

}

Qt::ItemFlags Scenegraph::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemFlags();
    if (index.isValid()) {
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return flags;

}

bool Scenegraph::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool success = false;
    if (index.isValid() && role == Qt::EditRole) {
        static_cast<Primitive*>(index.internalPointer())->setName(value.toString().toStdString());
        success = true;
        emit itemNameChanged();
    }
    return success;

}



void Scenegraph::addPrimitive(Primitive *p)
{
    // add Primitive to list

    int rowIndex = primitives_->size();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    primitives_->append(p);
    emit dataChanged(createIndex(rowIndex, 0, p), createIndex(rowIndex, 0, p));
    endInsertRows();

}

void Scenegraph::deletePrimitive(Primitive *p) {

    // get index and delete primitive from list
    int rowIndex = primitives_->indexOf(p);

    beginRemoveRows(QModelIndex(), rowIndex, rowIndex);

    primitives_->removeAt(rowIndex);
    endRemoveRows();

    delete p;
}

QList<Primitive*>* Scenegraph::getPrimitiveList() {
    return primitives_;
}

Primitive* Scenegraph::getPrimitive(QModelIndex index) {
    if (index.isValid()) {
        return static_cast<Primitive*>(index.internalPointer());
    }
}
