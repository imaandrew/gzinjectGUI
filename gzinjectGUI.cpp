#include "gzinjectGUI.h"

#define _STR(x) #x
#define STRINGIFY(x)  _STR(x)

gzinjectGUI::gzinjectGUI(QWidget *parent) : QMainWindow(parent) {
    settings = new QSettings("Mini's Applications", "gzinjectGUI");
    resize(300, 500);
    gzinjectPath = initializeGzPath();

    aboutwindow.setText("About gzinjectGUI");
    remakeAboutWindow();
    pathnotfound.setText("gzinject not found");
    pathnotfound.setInformativeText("gzinject could not be found, either the executable was not included in the download, or the path set by the user no longer exists. This can be corrected by either putting gzinject in \"tools/gzinject\", or manually setting a path in File -> Set gzinject Path.");
    pathnotfound.setIcon(QMessageBox::Critical);
    
    guiwidget = new gzGUIWidget(this);

    QString openFolderSetting = "";
    if (settings->value("file/openFolderWhenComplete").toBool())
        openFolderSetting = "true";

    guiwidget->initializeSettings(settings->value("file/romPath").toString(), SettingType::RomPath);
    guiwidget->initializeSettings(settings->value("file/wadPath").toString(), SettingType::WadPath);
    guiwidget->initializeSettings(settings->value("file/outputPath").toString(), SettingType::OutputPath);
    guiwidget->initializeSettings(openFolderSetting, SettingType::OpenFolderWhenComplete);

    auto *quit = new QAction("&Quit", this);
    quit->setShortcut(QKeySequence(QKeySequence::Quit));
    auto *setgzpath = new QAction("Set gzinject Path", this);
    auto *about_item = new QAction("About", this);
    about_item->setShortcut(QKeySequence(QKeySequence::HelpContents));
    auto *apply_patch = new QAction("Apply Patch File", this);

    QMenu *file = menuBar()->addMenu("&File");
    presets_menu = menuBar()->addMenu("&Presets");
    initializePresets();
    QMenu *help = menuBar()->addMenu("&Help");

    file->addAction(setgzpath);
    file->addAction(apply_patch);
    file->addSeparator();
    file->addAction(quit);

    help->addAction(about_item);

    setCentralWidget(guiwidget);
    setWindowTitle("gzinjectGUI");

    if (gzinjectPath.isNull())
        pathnotfound.exec();

    connect(quit, SIGNAL(triggered()), this, SLOT(quitMainWindow()));
    connect(setgzpath, SIGNAL(triggered()), this, SLOT(setGzPath()));
    connect(about_item, SIGNAL(triggered()), this, SLOT(showAboutWindow()));
    connect(apply_patch, SIGNAL(triggered()), this, SLOT(applyPatch()));
}

bool gzinjectGUI::isGzIncluded() {
    QString extension = "";
    #if defined(Q_OS_WIN)
    extension = ".exe";
    #endif
    QFileInfo included_gz(QCoreApplication::applicationDirPath() + QDir::separator() + "tools/gzinject" + extension);
    return included_gz.exists();
}

QString gzinjectGUI::initializeGzPath() {
    QString path = settings->value("file/gzpath").toString();
    if (!path.isEmpty() && QFileInfo(path).exists())
        return path;
    else if (isGzIncluded())
        return QCoreApplication::applicationDirPath() + QDir::separator() + "tools/gzinject";
    else
        return nullptr;
}

void gzinjectGUI::initializePresets() {
    presets_menu->clear();
    QList<QAction*> presets;
    QList<QAction*> gzi_files;
    QDir directory(QCoreApplication::applicationDirPath() + QDir::separator() + "presets");
    QStringList files = directory.entryList(QStringList() << "*.gzi" << "*.json", QDir::Files);
    foreach (QString filename, files) {
        QString filePath = directory.absolutePath() + QDir::separator() + filename;
        if (filePath.endsWith(".json")) {
            auto preset = Preset(filePath);
            if (!preset.getName().isNull()) {
                auto *action = new QAction(preset.getName());
                connect(action, &QAction::triggered, this, [this, preset]{ applyPreset(preset); });
                presets.append(action);
            }
        } else {
            auto *action = new QAction(QFileInfo(filePath).fileName());
            connect(action, &QAction::triggered, this, [this, filePath]{ applyPatch(filePath); });
            gzi_files.append(action);
        }
    }
    foreach(QAction *action, presets) {
        presets_menu->addAction(action);
    }
    presets_menu->addSeparator();
    foreach (QAction *action, gzi_files) {
        presets_menu->addAction(action);
    }
}

CommandOutput gzinjectGUI::executeCommand(QStringList arguments, bool isCommonKey) {
    if (gzinjectPath.isEmpty()) { return CommandOutput(-1, nullptr, nullptr); }
    QProcess process;
    process.setWorkingDirectory(QCoreApplication::applicationDirPath());
    process.start(gzinjectPath, arguments);
    if (isCommonKey)
        process.write("45e\n");
    process.waitForFinished(-1);
    return CommandOutput(process.exitCode(), process.readAllStandardOutput(), process.readAllStandardError());
}

void gzinjectGUI::quitMainWindow() {
    this->close();
}

void gzinjectGUI::showAboutWindow() {
    aboutwindow.exec();
}

QString gzinjectGUI::gzinjectVersion() {
    CommandOutput version = executeCommand({"--version"});
    QStringList output = version.getStandardOut().split(" ");
    int version_text = output.lastIndexOf("Version");
    if (output.length() > version_text + 1)
        return output[version_text + 1].trimmed();
    else
        return "";
}

void gzinjectGUI::remakeAboutWindow() {
    aboutwindow.setInformativeText("gzinject Version: " + gzinjectVersion() + "\ngzinject Path: " + gzinjectPath + "\n\nGit Commit: " + QString(STRINGIFY(GIT_VERSION)) + "\nOS: " + QSysInfo::prettyProductName() + " (" + QSysInfo::currentCpuArchitecture() + ")\n\nMade by Mini / Amy\ngzinject made by krimtonz");
}

void gzinjectGUI::setGzPath() {
    QString path = QFileDialog::getOpenFileName(this, "Open File", QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if (path.isEmpty()) { return; }
    settings->setValue("file/gzpath", path);
    initializeGzPath();
    remakeAboutWindow();
}

void gzinjectGUI::applyPreset(Preset preset) {
    if (!preset.getChannelId().isEmpty())
        guiwidget->updateChannelId(preset.getChannelId());
    if (!preset.getChannelTitle().isEmpty())
        guiwidget->updateChannelTitle(preset.getChannelTitle());
    foreach(QString patch, preset.getPatches()) {
        applyPatch(patch);
    }
}

void gzinjectGUI::applyPatch(QString filePath) {
    QString path = filePath.replace(" ", "\\ ");
    guiwidget->appendToArgs("-p " + path);
}

void gzinjectGUI::applyPatch() {
    QString path = QFileDialog::getOpenFileName(this, "Open File", QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "GZI Files (*.gzi);;All Files (*)");
    if (path.isEmpty()) { return; }
    applyPatch(path);
}

void gzinjectGUI::openFileLocation(QString path) {
    /* https://stackoverflow.com/a/46019091
     Doesn't have Linux support because file browsers are funny. That final catch at the end is "good enough" for Linux users.
     TODO: add something in for GNOME (nautilus) or KDE (dolphin).
     */
    if (path.isEmpty()) { return; }
    QFileInfo info(path);
    #if defined(Q_OS_WIN)
        QStringList args;
        if (!info.isDir())
            args << "/select,";
        args << QDir::toNativeSeparators(path);
        if (QProcess::startDetached("explorer", args))
            return;
    #elif defined(Q_OS_MAC)
        QStringList args;
        args << "-e";
        args << "tell application \"Finder\"";
        args << "-e";
        args << "activate";
        args << "-e";
        args << "select POSIX file \"" + path + "\"";
        args << "-e";
        args << "end tell";
        args << "-e";
        args << "return";
        if (!QProcess::execute("/usr/bin/osascript", args))
            return;
    #endif
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir()? path : info.path()));
}

void gzinjectGUI::defineSetting(QString setting, SettingType type) {
    switch(type) {
        case SettingType::RomPath:
            settings->setValue("file/romPath", setting);
            break;
        case SettingType::WadPath:
            settings->setValue("file/wadPath", setting);
            break;
        case SettingType::OutputPath:
            settings->setValue("file/outputPath", setting);
            break;
        case SettingType::OpenFolderWhenComplete:
            settings->setValue("file/openFolderWhenComplete", setting == "true");
            break;
    }
}

void gzinjectGUI::injectWAD(QString romPath, QString wadPath, QString outputPath, bool openFolderWhenComplete, QString title, QString channel_id, QString additional_args) {
    QFileInfo common_key(QCoreApplication::applicationDirPath() + QDir::separator() +  "common-key.bin");
    if (!common_key.exists())
        executeCommand({"-a", "genkey"}, true);
    QString wadFileName;
    QFileInfo rom_file(romPath);
    wadFileName = rom_file.fileName().split(".")[0] + ".wad";

    QStringList arguments;
    arguments << "-a" << "inject" << "-m" << romPath << "-w" << wadPath << "-o" << outputPath + QDir::separator() + wadFileName;
    if (!title.isNull())
        arguments << "-t" << title;
    if (!channel_id.isNull())
        arguments << "-i" << channel_id;
    if (!additional_args.isEmpty()) {
        QStringList stringList = additional_args.split(" ");
        for (int i = 0; i < stringList.length(); i++) {
            QString stringItem = stringList[i];
            while (stringItem.endsWith("\\") && i < stringList.length() - 1) {
                stringItem.chop(1);
                stringItem += " " + stringList[i+1];
                i++;
            }
            arguments << stringItem;
        }
    }
    arguments.append("--cleanup");
    CommandOutput output = executeCommand(arguments);
    if (output.getExitCode() != 0) {
        QMessageBox failedCommand;
        failedCommand.setText("Command failed with exit code " + QString::number(output.getExitCode()));
        failedCommand.setInformativeText(output.getStandardOut() + "\n\n" + output.getStandardErr());
        failedCommand.setIcon(QMessageBox::Critical);
        failedCommand.exec();
    }
    else {
        if (openFolderWhenComplete)
            openFileLocation(outputPath + QDir::separator() + wadFileName);
    }
}
