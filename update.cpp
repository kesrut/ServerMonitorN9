#include "update.h"

void update::getServers(QList <Host*> *h)
{
    QList <Host*> *tmp_hosts = h ;
    int i=0;
    for (i=0; i < tmp_hosts->count(); i++)
    {
        Host *host = tmp_hosts->at(i) ;
        UpdateElement *element = new UpdateElement(this, i, *tmp_hosts) ;
        UpdateElement *elm = (UpdateElement*)host->getUpdateElement() ;
        if (elm != NULL)
        {
            UpdateElement *e = (UpdateElement*) host->getUpdateElement() ;
            delete e ;
        }
        host->setUpdateElement(element);
        element->start(host);
    }
}
void update::start(int i)
{
    Host *host = hosts->at(i) ;
    UpdateElement *element = new UpdateElement(this, i, *hosts) ;
    UpdateElement *elm = (UpdateElement*)host->getUpdateElement() ;
    if (elm != NULL)
    {
        UpdateElement *e = (UpdateElement*) host->getUpdateElement() ;
        delete e ;
    }
    host->setUpdateElement(element);
    element->start(host);
}

void update::getModel(ServerListModel *m)
{
    model = m ;
    QObject::connect(this, SIGNAL(update_tbl()), model, SLOT(update_table())) ;
}

void update::update_info()
{
    emit update_tbl();
}
