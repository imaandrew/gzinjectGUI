#pragma once

#include "commandOutput.h"
#include "gzguiwidget.h"
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

class gzGUIWidget;
class Preset;

class gzinjectGUI : public QMainWindow {
    Q_OBJECT

    public:
        gzinjectGUI(QWidget *parent = nullptr);
        void injectWAD(QString romPath, QString wadPath, QString outputPath, bool openFolderWhenComplete, QString title = nullptr, QString channel_id = nullptr, QString additional_args = nullptr);
        void defineSetting(QString setting, SettingType type);
        void checkAutoLoad(QFileInfo rom);
    private slots:
        void quitMainWindow();
        void setGzPath();
        void showAboutWindow();
        void applyPatch();
        void applyPatch(QString filePath);
        void applyPreset(Preset preset);
    private:
        gzGUIWidget *guiwidget;
        CommandOutput executeCommand(QStringList arguments, bool isCommonKey = false);
        void openFileLocation(QString path);
        QString initializeGzPath();
        bool isGzIncluded();
        QString gzinjectVersion();
        void remakeAboutWindow();
        QSettings *settings;
        QString gzinjectPath;
        void initializePresets();

        QMenu *presets_menu;
        QList<Preset> presets;

        QMessageBox pathnotfound;
        QMessageBox aboutwindow;
};
