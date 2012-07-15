#pragma once

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTextCodec>
#include <QPoint>
#include <QMenu>
#include "network.h"
#include "telnetform.h"
#include "graphform.h"
#include "fileform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionConnect_activated();
    void onRedraw();

    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void runTelnet();
    void runGraph();
    void runFile();

private:
    Ui::MainWindow *ui;
    Network *network_;
    QMenu *modeMenu_;
};
