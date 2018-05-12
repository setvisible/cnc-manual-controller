/* - CNC Manual Controller - Copyright (C) 2018 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "version.h"
#include "globals.h"

#include <Core/Engine>
#include <GUI/ControlWidget>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtCore/QStandardPaths>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#ifdef QT_DEBUG
#  include <QtCore/QDebug>
#endif


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_engine(new Engine(this))
  , m_dirty(false)
  , m_physicalFile(false)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("%0 v%1").arg(STR_APPLICATION_NAME).arg(STR_APPLICATION_VERSION));
    this->setWindowIcon(QIcon(":/icons/logo/Iconshow-Button-Design-Pack-05-CNC-Drill.ico"));
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setAcceptDrops(true);

    // Initialize widgets
    ui->speedSpinBox->setMaximum(1000);
    ui->speedSpinBox->setMinimum(1);

    // Connect widgets to engine
    connect(ui->connectButton, SIGNAL(toggled(bool)), m_engine, SLOT(setConnected(bool)));
    connect(ui->speedSpinBox, SIGNAL(valueChanged(int)), m_engine, SLOT(setTicksPerSecond(int)));

    connect(m_engine, SIGNAL(connectedChanged(bool)), this, SLOT(updateConnectButtonAndStatus(bool)));
    connect(m_engine, SIGNAL(ticksPerSecondChanged(int)), ui->speedSpinBox, SLOT(setValue(int)));

    m_engine->reset();


    createActions();
    createMenus();

    readSettings();

    newFile();
}


MainWindow::~MainWindow()
{
    delete ui;
}

/******************************************************************************
 ******************************************************************************/
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

/******************************************************************************
 ******************************************************************************/
void MainWindow::newFile()
{
    if (maybeSave()) {
        m_physicalFile = false;
        m_currentFile = QFileInfo();
        m_currentFile.setFile(QStringLiteral("untitled.gcode"));

        this->setClean();
    }
}

bool MainWindow::isExampleFile() const
{
    return m_currentFile.filePath().startsWith(":/");
}

bool MainWindow::isPhysicalFile() const
{
    return m_physicalFile
            && m_currentFile.isFile()
            && !m_currentFile.isSymLink()
            && !isExampleFile();
}

bool MainWindow::save()
{
    if ( isPhysicalFile() ){
        return saveFile(m_currentFile.absoluteFilePath());
    } else {
        return saveAs();
    }
}

bool MainWindow::saveAs()
{
    QString filePath = askSaveFileName(QStringLiteral("G-Code File (*.gcode)"),
                                       tr("G-Code File"));
    if (filePath.isEmpty()) {
        return false;
    }
    return saveFile(filePath);
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString filePath = askOpenFileName(tr("G-Code File (*.gcode);;All files (*.*)"));
        if (!filePath.isEmpty()) {
            if (loadFile(filePath)) {
                this->setClean();
            }
        }
    }
}

/******************************************************************************
 ******************************************************************************/
void MainWindow::showFileProperties()
{
    //PropertiesDialog dialog(m_sceneManager, this);
    //dialog.exec();
}

/******************************************************************************
 ******************************************************************************/
void MainWindow::showPreferences()
{
    //  SettingsDialog dialog(this);
    //  dialog.exec();
    //  m_mystery->readSettings();
}

/******************************************************************************
 ******************************************************************************/
void MainWindow::about()
{
    QMessageBox msgBox(QMessageBox::NoIcon, tr("About CNC Manual Controller"), aboutHtml());
    msgBox.exec();
}

/******************************************************************************
 ******************************************************************************/
bool MainWindow::maybeSave()
{
    if (m_dirty) {
        int ret = QMessageBox::warning(
                    this, tr("Unsaved changes"),
                    tr("The scene has not been saved.\n\n"
                       "Do you want to save your changes ?"),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return save();
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

QString MainWindow::niceFileName() const
{
    if ( isPhysicalFile() ){
        return QDir::toNativeSeparators(m_currentFile.canonicalFilePath());
    }else{
        return m_currentFile.fileName();
    }
}

void MainWindow::setDirty()
{
    if (!m_dirty) {
        m_dirty = true;
        this->setWindowTitle( niceFileName() + QStringLiteral("* - CNC Manual Controller"));
    }
}

void MainWindow::setClean()
{
    m_dirty = false;
    this->setWindowTitle( niceFileName() + QStringLiteral(" - CNC Manual Controller"));
}


/******************************************************************************
 ******************************************************************************/
QString MainWindow::askSaveFileName(const QString &fileFilter, const QString &title)
{
    QString suggestedPath;
    if ( isPhysicalFile() ) {
        suggestedPath = m_currentFile.canonicalFilePath();
    }else{
        const QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        suggestedPath = dir + QDir::separator() + m_currentFile.fileName();
        suggestedPath = QDir::toNativeSeparators(suggestedPath);
    }
    return QFileDialog::getSaveFileName(this, title, suggestedPath, fileFilter);
}

QString MainWindow::askOpenFileName(const QString &fileFilter, const QString &title)
{
    QString currentDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if ( isPhysicalFile() ){
        currentDir = m_currentFile.absolutePath();
    }
    return QFileDialog::getOpenFileName(this, title, currentDir, fileFilter);
}


/******************************************************************************
 ******************************************************************************/
void MainWindow::readSettings()
{
    QSettings settings;
    if ( !isMaximized() ) {
        this->move(settings.value("Position", QPoint(100, 100)).toPoint());
        this->resize(settings.value("Size", QSize(350, 350)).toSize());
    }
    this->setWindowState( (Qt::WindowStates)settings.value("WindowState", 0).toInt() );

}

void MainWindow::writeSettings()
{
    QSettings settings;

    if( !(isMaximized() | isFullScreen()) ) {
        settings.setValue("Position", this->pos());
        settings.setValue("Size", this->size());
    }
    settings.setValue("WindowState", (int)this->windowState()); // minimized, maximized, active, fullscreen...


    // --------------------------------------------------------------
    // Write also the current version of application in the settings,
    // because it might be used by 3rd-party update manager softwares like
    // FileHippo or Google Updater (aka gup).
    // --------------------------------------------------------------
    settings.setValue("Version", STR_APPLICATION_VERSION );
}

/******************************************************************************
 ******************************************************************************/
void MainWindow::createActions()
{
    //! [0] File
    ui->action_New->setShortcuts(QKeySequence::New);
    ui->action_New->setStatusTip(tr("Create a new G-Code File"));
    connect(ui->action_New, SIGNAL(triggered()), this, SLOT(newFile()));

    ui->action_Open->setShortcuts(QKeySequence::Open);
    ui->action_Open->setStatusTip(tr("Open a G-Code sequence from a file"));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));

    ui->action_Save->setShortcuts(QKeySequence::Save);
    ui->action_Save->setStatusTip(tr("Save the world to current G-Code file"));
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));

    ui->action_SaveAs->setShortcuts(QKeySequence::SaveAs);
    ui->action_SaveAs->setStatusTip(tr("Save the G-Code sequence to a different file"));
    connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

    ui->action_Properties->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F2));
    ui->action_Properties->setStatusTip(tr("Show the properties of the current file"));
    connect(ui->action_Properties, SIGNAL(triggered()), this, SLOT(showFileProperties()));

    ui->action_Exit->setShortcuts(QKeySequence::Quit);
    ui->action_Exit->setStatusTip(tr("Quit CNC Manual Controller"));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));
    //! [0]

    //! [1] Options

#ifdef Q_OS_WIN
    ui->action_Preferences->setShortcut(QKeySequence(/*Qt::CTRL +*/ Qt::Key_F10));
#else
    ui->action_Preferences->setShortcuts(QKeySequence::Preferences);
#endif
    ui->action_Preferences->setStatusTip(tr("Open settings"));
    connect(ui->action_Preferences, SIGNAL(triggered()), this, SLOT(showPreferences()));

    //! [1]

    //! [2] Help
    ui->action_About->setShortcuts(QKeySequence::HelpContents);
    ui->action_About->setStatusTip(tr("About %0").arg(STR_APPLICATION_NAME));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));

    ui->action_AboutQt->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F1));
    ui->action_AboutQt->setStatusTip(tr("About Qt"));
    connect(ui->action_AboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    //! [2]
}

void MainWindow::createMenus()
{

}


/******************************************************************************
 ******************************************************************************/
bool MainWindow::saveFile(const QString &path)
{
    QDir::setCurrent(path);
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        QMessageBox::warning(this, tr("Cannot save file"),
                             tr("Cannot write to file %1:\n%2.")
                             .arg(path)
                             .arg(file.errorString()));
        return false;
    }

    /// \todo implement this
    //    QByteArray ba;
    // ...
    //    file.write( ba );

    m_physicalFile = true;
    m_currentFile.setFile(path);
    this->statusBar()->showMessage(tr("File saved"), 2000);
    this->setClean();
    return true;
}

/******************************************************************************
 ******************************************************************************/
bool MainWindow::loadFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open file.");
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(path)
                             .arg(file.errorString()));
        return false;
    }
    bool ok = true;

    /// \todo implement this
    //     bool ok;
    //     QByteArray data = file.readAll();
    //     ...

    if (!ok) {
        qCritical("Couldn't parse the file.");
        QMessageBox::warning(this, tr("Error"),
                             tr("Error found in the file:\n"
                                "%1\n")
                             .arg(path));
    }

    m_physicalFile = true;
    m_currentFile = path;
    this->statusBar()->showMessage(tr("File loaded"), 5000);
    this->setClean();
    return true;
}

/******************************************************************************
 ******************************************************************************/
void MainWindow::updateConnectButtonAndStatus(bool connected)
{
    const QString status = connected ? tr("connected") : tr("disconnected");
    const QString button = connected ? tr("Disconnect") : tr("Connect");
    ui->connectStatus->setText(status);
    ui->connectButton->setText(button);
}
