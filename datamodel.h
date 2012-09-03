#ifndef DATAMODEL_H
#define DATAMODEL_H

 #include <QAbstractListModel>

class DataModel : public QAbstractItemModel
{
public:
    DataModel(QObject *parent=0) ;
    QVariant data(const QModelIndex &index, int role) const ;
    int rowCount(const QModelIndex &parent) const ;
};

#endif // DATAMODEL_H
