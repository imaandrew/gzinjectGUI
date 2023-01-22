#include "gzguiwidget.h"

gzGUIWidget::gzGUIWidget(QWidget *parent) : QWidget(parent) {
    gzinject_gui = reinterpret_cast<gzinjectGUI*>(parent);

    auto *og_rom_lbl = new QLabel("ROM File", this);
    og_rom_lbl->setFixedHeight(20);
    og_rom_path = new QLineEdit(this);
    auto *og_rom_btn = new QPushButton("Browse...", this);

    auto *og_patch_lbl = new QLabel("ROM Patch", this);
    og_patch_lbl->setFixedHeight(20);
    og_patch_path = new QLineEdit(this);
    auto *og_patch_btn = new QPushButton("Browse...", this);

    auto *og_wad_lbl = new QLabel("WAD File", this);
    og_wad_lbl->setFixedHeight(20);
    og_wad_path = new QLineEdit(this);
    auto *og_wad_btn = new QPushButton("Browse...", this);

    auto *output_lbl = new QLabel("Output Directory", this);
    output_lbl->setFixedHeight(20);
    output_path = new QLineEdit(this);
    auto *output_btn = new QPushButton("Browse...", this);
    output_cbox = new QCheckBox("Open folder when complete", this);

    channel_title = new QLineEdit(this);
    auto *channel_title_lbl = new QLabel("Channel Title", this);
    channel_title_lbl->setFixedHeight(20);

    channel_id = new QLineEdit(this);
    auto *channel_id_lbl = new QLabel("Channel ID", this);
    channel_id_lbl->setFixedHeight(20);

    additional_args = new QLineEdit(this);
    auto *addition_args_lbl = new QLabel("Additional Arguments", this);
    addition_args_lbl->setFixedHeight(20);


    auto *generate_btn = new QPushButton("Generate!", this);

    auto *box_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    auto *channel_info_grid = new QGridLayout();
    channel_info_grid->addWidget(channel_title_lbl, 0, 0);
    channel_info_grid->addWidget(channel_title, 1, 0);
    channel_info_grid->addWidget(channel_id_lbl, 0, 1);
    channel_info_grid->addWidget(channel_id, 1, 1);

    box_layout->addWidget(og_rom_lbl);
    box_layout->addWidget(og_rom_path);
    box_layout->addWidget(og_rom_btn);
    box_layout->addWidget(og_patch_lbl);
    box_layout->addWidget(og_patch_path);
    box_layout->addWidget(og_patch_btn);
    box_layout->addWidget(og_wad_lbl);
    box_layout->addWidget(og_wad_path);
    box_layout->addWidget(og_wad_btn);
    box_layout->addWidget(output_lbl);
    box_layout->addWidget(output_path);
    box_layout->addWidget(output_btn);
    box_layout->addWidget(output_cbox);
    box_layout->addLayout(channel_info_grid);
    box_layout->addWidget(addition_args_lbl);
    box_layout->addWidget(additional_args);
    box_layout->addWidget(generate_btn);

    setLayout(box_layout);

    connect(og_rom_btn, &QPushButton::released, this, &gzGUIWidget::rom_btn_clicked);
    connect(og_patch_btn, &QPushButton::released, this, &gzGUIWidget::patch_btn_clicked);
    connect(og_wad_btn, &QPushButton::released, this, &gzGUIWidget::wad_btn_clicked);
    connect(output_btn, &QPushButton::released, this, &gzGUIWidget::output_btn_clicked);
    connect(generate_btn, &QPushButton::released, this, &gzGUIWidget::generate);
    connect(output_cbox, &QCheckBox::stateChanged, this, &gzGUIWidget::updateCheckBoxSetting);
}

void gzGUIWidget::initializeSettings(QString setting, SettingType type) {
    switch(type) {
        case SettingType::RomPath:
            defaultRomSearchPath = setting;
            break;
        case SettingType::WadPath:
            og_wad_path->setText(setting);
            break;
        case SettingType::OutputPath:
            output_path->setText(setting);
            break;
        case SettingType::OpenFolderWhenComplete:
            output_cbox->setChecked(setting == "true");
            break;
    }
}

void gzGUIWidget::appendToArgs(QString argument) {
    QString args_text = additional_args->text();
    args_text += " " + argument;
    args_text = args_text.trimmed();
    additional_args->setText(args_text);
}

void gzGUIWidget::clearArgs() {
    additional_args->setText("");
}

void gzGUIWidget::updateChannelId(QString channelId) {
    channel_id->setText(channelId);
}

void gzGUIWidget::updateChannelTitle(QString channelTitle) {
    channel_title->setText(channelTitle);
}

void gzGUIWidget::rom_btn_clicked() {
    QString romPath;
    if (defaultRomSearchPath.isEmpty())
        romPath = handleFileDialog("Open ROM", "N64 Files (*.z64 *.n64 *.v64)");
    else
        romPath = handleFileDialog("Open ROM", "N64 Files (*.z64 *.n64 *.v64)", defaultRomSearchPath);
    if (!romPath.isEmpty()) {
        og_rom_path->setText(romPath);
        QFileInfo rom_path(romPath);
        if (output_path->text().isEmpty()) {
            output_path->setText(rom_path.absoluteDir().absolutePath());
            gzinject_gui->defineSetting(rom_path.absoluteDir().absolutePath(), SettingType::OutputPath);
        }
        gzinject_gui->checkAutoLoad(rom_path);
        gzinject_gui->defineSetting(rom_path.absoluteDir().absolutePath(), SettingType::RomPath);
    }
}

void gzGUIWidget::patch_btn_clicked() {
    QString patchPath;
    patchPath = handleFileDialog("Open Patch", "XDelta3 Patch Files (*.xdelta)");
    if (!patchPath.isEmpty()) {
        og_patch_path->setText(patchPath);
    }
}

void gzGUIWidget::wad_btn_clicked() {
    QString wadPath;
    if (og_wad_path->text().isEmpty())
        wadPath = handleFileDialog("Open WAD", "WAD Files (*.wad)");
    else {
        QFileInfo wad_path(og_wad_path->text());
        wadPath = handleFileDialog("Open WAD", "WAD Files (*.wad)", wad_path.absoluteDir().absolutePath());
    }
    if (!wadPath.isEmpty()) {
        og_wad_path->setText(wadPath);
        gzinject_gui->defineSetting(wadPath, SettingType::WadPath);
    }
}

void gzGUIWidget::output_btn_clicked() {
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    if (!og_rom_path->text().isEmpty()) {
        QFileInfo rom_path(og_rom_path->text());
        defaultPath = rom_path.absoluteDir().absolutePath();
    }
    QString dirPath = QFileDialog::getExistingDirectory(this, "Open Directory", defaultPath);
    if (!dirPath.isEmpty()) {
        output_path->setText(dirPath);
        gzinject_gui->defineSetting(dirPath, SettingType::OutputPath);
    }
}

QString gzGUIWidget::handleFileDialog(QString windowTitle, QString fileType, QString defaultPath) {
    QFileDialog dialog(this);
    QString path = dialog.getOpenFileName(this, windowTitle, defaultPath, fileType + ";;All Files (*)");
    if (path.isEmpty()) { return nullptr; }
    return path;
}

void gzGUIWidget::updateCheckBoxSetting(int state) {
    if (state == Qt::Checked)
        gzinject_gui->defineSetting("true", SettingType::OpenFolderWhenComplete);
    else
        gzinject_gui->defineSetting("false", SettingType::OpenFolderWhenComplete);
}

void gzGUIWidget::generate() {
    QString channelTitle = nullptr;
    QString channelId = nullptr;
    QString additionalArgs = nullptr;
    if (!channel_title->text().isEmpty())
        channelTitle = channel_title->text();
    if (!channel_id->text().isEmpty())
        channelId = channel_id->text();
    if (!additional_args->text().isEmpty())
        additionalArgs = additional_args->text();
    gzinject_gui->patchROM(og_rom_path->text(), og_patch_path->text(), QCoreApplication::applicationDirPath() + QDir::separator() + "patched_rom.z64");
    gzinject_gui->injectWAD(QCoreApplication::applicationDirPath() + QDir::separator() + "patched_rom.z64", og_wad_path->text(), output_path->text(), output_cbox->isChecked(), channelTitle, channelId, additionalArgs);
    gzinject_gui->cleanup();
}
