#pragma once

#include "SmartLogger.h"

#include <QObject>
#include <QVector>
#include <QtXml>

class Provisioning : public QObject
{
    Q_OBJECT
public:
    enum class ProvTypes
    {
        HTTP
    };

    Provisioning(QObject * obj = 0): QObject(obj) {}
    bool getProvisioning();

    void registerForProvisionig(ProvTypes type){}

private:

    QVector<ProvTypes> m_registeredList;

};
