#pragma once

#include <QCoreApplication>
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
        QString getHomeboy();
        QString getXdeltaPatch();
        QStringList getPatches();
        
        bool testRomString(QString romName);
    private:
        QString absolutePathOfPatch(QString patchPath);

        QString version;
        QString name;
        QString channelId;
        QString channelTitle;
        QString homeboy;
        QString xdeltaPatch;
        QStringList patches;
        QStringList autoLoad;
        QString absolutePath;
        QString applicationPath;
};
