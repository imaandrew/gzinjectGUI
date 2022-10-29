#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>

class Preset {
    public:
        Preset(QString filename);

        QString getVersion();
        QString getName();
        QString getChannelId();
        QString getChannelTitle();
        QStringList getPatches();
        
    private:
        QString absolutePathOfPatch(QString patchPath);

        QString version;
        QString name;
        QString channelId;
        QString channelTitle;
        QStringList patches;
        QString absolutePath;
};
