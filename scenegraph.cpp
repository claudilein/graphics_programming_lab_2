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
            std::cout << "index (" << row << ", " << column << std::endl;
            index = createIndex(row, column, primitives_->at(row));
        }
    }
    std::cout << "index()" << std::endl;

    return index;

    /*if (parent.isValid()) return QModelIndex();

        if (row >= 0 && row < primitives_->size())
        {
            //RBTNode* node = m_sceneGraph->m_nodes.at(row);
            Primitive* p = primitives_->at(row);
            return createIndex(row, column, p);
        }
    return QModelIndex();*/
}

QModelIndex Scenegraph::parent(const QModelIndex &child) const
{
    std::cout << "parent" << std::endl;
    return QModelIndex();
}

int Scenegraph::rowCount(const QModelIndex &parent) const
{
    int count = 0;
    if (!parent.isValid()) {
        count = primitives_->size();
    }

    std::cout << "rowCount " << count << ", parent valid?: " << parent.isValid() << std::endl;
    return count;

    /*if (parent.isValid()) return 1;
        return primitives_->size();*/
}

int Scenegraph::columnCount(const QModelIndex &parent) const
{
    int count = 0;
    if (!parent.isValid()) {
        count = 1;
    }
    std::cout << "columnCount " << count << std::endl;
    return count;

    /*if (parent.isValid()) return 0;
        return 1;*/
}

QVariant Scenegraph::headerData(int section, Qt::Orientation orientation, int role) const {
    section;
    QVariant header = QVariant();
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) header = "Scene";
    std::cout << "headerData" << std::endl;
    return header;
}

QVariant Scenegraph::data(const QModelIndex &index, int role) const
{
    QVariant data = QVariant();
    if (index.isValid() && role == Qt::DisplayRole)
        data = QVariant(QString::fromStdString(static_cast<Primitive*>(index.internalPointer())->getName()));
    std::cout << "data" << std::endl;
    return data;

    /*if (index.isValid() && role == Qt::DisplayRole)
        {
            return QVariant(QString::fromStdString(static_cast<Primitive*>(index.internalPointer())->getName()));
        }
    return QVariant();*/
}

Qt::ItemFlags Scenegraph::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemFlags();
    if (index.isValid()) {
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    std::cout << "flags" << std::endl;
    return flags;

    /*if (index.isValid())
        {
            return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        }
    return 0;*/
}

bool Scenegraph::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool success = false;
    if (index.isValid() && role == Qt::EditRole) {
        static_cast<Primitive*>(index.internalPointer())->setName(value.toString().toStdString());
        success = true;
    }
    std::cout << "setData" << std::endl;
    return success;

    /*if (index.isValid())
        {
            //const QString *name = static_cast<const QString*>(value.data());
            static_cast<Primitive*>(index.internalPointer())->setName(value.toString().toStdString());
            // we have only a single list
            emit dataChanged(index, index);
            //emit selectionChanged(index);
            return true;
        }
    return false;*/
}


void Scenegraph::addPrimitive(Primitive *p)
{
    std::cout << "addPrimitive" << std::endl;
    // add Primitive to list

    int rowIndex = primitives_->size();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    std::cout << "after begininsertRows" << std::endl;
    primitives_->append(p);
    emit dataChanged(createIndex(rowIndex, 0, p), createIndex(rowIndex, 0, p));
    endInsertRows();

    /*beginInsertRows(QModelIndex(), primitives_->size(), primitives_->size());
        primitives_->append(p);


        endInsertRows();*/
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

