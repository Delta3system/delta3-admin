#pragma once

#include <QDialog>
#include <QTextCursor>
#include <QColor>
#include <QPalette>
#include "network.h"
#include "defines.h"

namespace Ui {
class TelnetForm;
}

class TelnetForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit TelnetForm(
            Network *network,
            qint32 clientId,
            QWidget* parent = 0);
    ~TelnetForm();
    
private slots:
    void on_lineEdit_returnPressed();
    void onDataReceived();

private:
    Network *network_;
    qint32 clientId_;
    Ui::TelnetForm *ui;
};

