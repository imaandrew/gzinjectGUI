#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class Preset {
    public:
        Preset(QString filename);

        QString getVersion();
        QString getName();
        QString getChannelId();
        QString getChannelTitle();
        QStringList getPatches();
        
        bool testRomString(QString romName);
    private:
        QString absolutePathOfPatch(QString patchPath);

        QString version;
        QString name;
        QString channelId;
        QString channelTitle;
        QStringList patches;
        QStringList autoLoad;
        QString absolutePath;
};
