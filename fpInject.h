#pragma once

#include "commandOutput.h"
#include "fpguiwidget.h"
#include "settingtype.h"
#include "preset.h"

#include <QApplication>
#include <QMainWindow>
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QMessageBox>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QDesktopServices>

class fpGUIWidget;
class Preset;

class fpInject : public QMainWindow {
    Q_OBJECT

public:
        fpInject(QWidget *parent = nullptr);
        void injectWAD(QString romPath, QString wadPath, QString outputPath, bool openFolderWhenComplete, QString title = nullptr, QString channel_id = nullptr, QString additional_args = nullptr);
        void patchROM(QString romPath, QString patchPath, QString outPath);
        void extractROM(QString wadPath, QString outPath);
        void defineSetting(QString setting, SettingType type);
        void checkAutoLoad(QFileInfo rom);
        void checkAutoLoadWad(QFileInfo wad);
        void cleanup();
    private slots:
        void quitMainWindow();
        void setGzPath();
        void showAboutWindow();
        void applyPatch();
        void applyPatch(QString filePath);
        void addHomeboy(QString homeboyPath);
        void applyPreset(Preset preset);
        void clearMiscArgs();
    private:
        fpGUIWidget *guiwidget;
        CommandOutput executeCommand(QStringList arguments, bool isCommonKey = false);
        void openFileLocation(QString path);
        QString initializeGzPath();
        QString initializeXdeltaPath();
        bool isGzIncluded();
        bool isXdeltaIncluded();
        QString gzinjectVersion();
        void remakeAboutWindow();
        QSettings *settings;
        QString gzinjectPath;
        QString xdeltaPath;
        QString compressedRomPath;
        void initializePresets();

        QMenu *presets_menu;
        QList<Preset> presets;

        QMessageBox pathnotfound;
        QMessageBox xdeltanotfound;
        QMessageBox aboutwindow;
        QString appPath;

};
