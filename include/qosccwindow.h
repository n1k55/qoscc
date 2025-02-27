#ifndef QOSCCWINDOW_H
#define QOSCCWINDOW_H

#include <QMainWindow>
#include "controllerclass.h"
#include <QLabel>

#define VERSION "0.41"

namespace Ui {
    class QOscCWindow;
}

class QOscCWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QOscCWindow(QWidget *parent = nullptr);
    ~QOscCWindow() override;

    void initActions();
    void initMenuBar();
    ControllerClass* controller;
public slots:  
    void slotFileOpen();  
    void slotFileQuit();
    void slotHelpAbout();
    void saveSettings();
    void addDevice();
    void addScope();
    void addTrace();
    void setStatus(const QString &);
    void update();
    void changeLabel(QWidget *, const QString &);
signals:
    void hasChanged();
private:
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *setMenu;

    QAction *fileOpen;
    QAction *fileQuit;
    QAction *helpAboutApp;

    QAction *setSave;
    QAction *setAddDevice;
    QAction *setAddScope;
    QAction *setAddTrace;
    QTabWidget *groups;

    QLabel *statusLabel;

    void createControls();
    void destroyControls();

    std::string conffilename;

    Ui::QOscCWindow *ui;
};
#endif // QOSCCWINDOW_H
