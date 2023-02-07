#pragma once

#include "gzinjectGUI.h"
#include "settingtype.h"

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QCheckBox>
#include <QStandardPaths>

class gzinjectGUI;

class gzGUIWidget : public QWidget {
    Q_OBJECT

    public:
        gzGUIWidget(QWidget *parent = nullptr);
        void appendToArgs(QString argument);
        void initializeSettings(QString setting, SettingType type);
        void updateChannelId(QString channelId);
        void updateChannelTitle(QString channelTitle);
        void updateXdeltaPatch(QString xdeltaPatch);
        void updateHomeboyPath(QString homeboyPath);
        void clearArgs();
    private slots:
        void rom_btn_clicked();
        void patch_btn_clicked();
        void wad_btn_clicked();
        void output_btn_clicked();
        void generate();
        void updateCheckBoxSetting(int state);
        void updateCustomCheckBoxSetting(int state);
    private:
        gzinjectGUI *gzinject_gui;
        QString handleFileDialog(QString windowTitle = "Select File", QString fileType = "Any File (*)", QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

        QString defaultRomSearchPath;
        
        QLineEdit *og_rom_path;
        QPushButton *og_rom_btn;
        QLineEdit *og_patch_path;
        QString homeboy;
        QLineEdit *og_wad_path;
        QLineEdit *output_path;
        QLineEdit *channel_title;
        QLineEdit *channel_id;
        QLineEdit *additional_args;
        QCheckBox *output_cbox;
        QCheckBox *custom_cbox;
};
