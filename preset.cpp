#include "preset.h"

Preset::Preset(QString filename) {
    QFile file(filename);
    QFileInfo fileInfo(filename);
    absolutePath = fileInfo.absoluteDir().absolutePath();
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString file_text = file.readAll();
    file.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(file_text.toUtf8());
    QJsonObject object = jsonDocument.object();
    version = object.value("_version").toString(nullptr);
    name = object.value("name").toString(nullptr);
    channelId = object.value("channelId").toString(nullptr);
    channelTitle = object.value("channelTitle").toString(nullptr);
    QJsonArray patch_array = object.value("patches").toArray();
    QJsonArray autoload_array = object.value("autoload").toArray();
    
    foreach (QJsonValue value, patch_array) {
        if (value.isString())
            patches.append(absolutePathOfPatch(value.toString()));
    }
    foreach (QJsonValue value, autoload_array) {
        if (value.isString())
            autoLoad.append(value.toString());
    }
}

QString Preset::absolutePathOfPatch(QString patchPath) {
    if (patchPath.startsWith("./"))
        return absolutePath + QDir::separator() + patchPath.mid(2);
    else
        return patchPath;
}

bool Preset::testRomString(QString romName) {
    foreach (QString item, autoLoad) {
        QRegularExpression re(item);
        QRegularExpressionMatch match = re.match(romName);
        if (match.hasMatch())
            return true;
    }
    return false;
}

QString Preset::getVersion() {
    return version;
}

QString Preset::getName() {
    return name;
}

QString Preset::getChannelId() {
    return channelId;
}

QString Preset::getChannelTitle() {
    return channelTitle;
}

QStringList Preset::getPatches() {
    return patches;
}
