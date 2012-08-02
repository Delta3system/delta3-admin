#pragma once

#include <QWidget>
#include "network.h"
#include "defines.h"
#include "Protocols/graphics.h"

namespace Ui {
class GraphForm;
}

class GraphForm : public QWidget
{
    Q_OBJECT
    
public:
	explicit GraphForm(delta3::Graphics* graph, QWidget* parent = 0);
    ~GraphForm();

signals:
	void ready(QByteArray &arr);
    void mMove(qint16, qint16);
    void mClick(qint16, qint16, delta3::GMCLICK);
    void keyPress(int key);

private slots:
	void onDataReceived(QImage &img);
    void onReady(int clW, int clH);

protected:
	void paintEvent(QPaintEvent *);
	bool eventFilter(QObject* _o, QEvent* _e);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

    void keyPressEvent(QKeyEvent *);

private:
	delta3::Graphics	*graph_;
	QImage				image_; // mb it's a not very well idea ;|
	Ui::GraphForm		*ui;

private:
    qint16 getClientMousePosX(int mPos) {
        return 1.0 * mPos / width() * graph_->clientWidth(); }

    qint16 getClientMousePosY(int mPos) {
        return 1.0 * mPos / height() * graph_->clientHeight(); }
};
