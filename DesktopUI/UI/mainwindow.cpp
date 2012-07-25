#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Protocols/graphics.h"

MainWindow::MainWindow(delta3::Network *net, QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	if(net)
		setNetwork(net);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QAction *act;
    modeMenu_ = new QMenu(this);

	act = new QAction(tr("Telnet mode"),this);
	connect(act,SIGNAL(triggered()),this,SLOT(runTelnet()));
	modeMenu_->addAction(act);

	act = new QAction(tr("File mode"),this);
    //connect(act,SIGNAL(triggered()),this,SLOT(runFile()));
	modeMenu_->addAction(act);

	act = new QAction(tr("Graphics mode"),this);
	connect(act,SIGNAL(triggered()),this,SLOT(runGraph()));
	modeMenu_->addAction(act);

	modeMenu_->addSeparator();

	act = new QAction(tr("Options"),this);
	connect(act,SIGNAL(triggered()),this,SLOT(runOptions()));
	modeMenu_->addAction(act);

    createTrayIcon();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createTrayIcon()
{
    QAction *action;
    this->setWindowIcon(QIcon(":/icon.ico"));

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icon.ico"));
    trayIcon->show();

    QMenu *menu = new QMenu(this);

    action = new QAction(QString::fromLocal8Bit("��������"),this);http://www.lektorium.tv/lecture/?id=13406
    connect(action,SIGNAL(triggered()),this,SLOT(showNormal()));
    menu->addAction(action);

    menu->addSeparator();

    menu->addAction(QString::fromLocal8Bit("O ��������� Delta3"));
    menu->addSeparator();

    action = new QAction(QString::fromLocal8Bit("����� �� Delta3"),this);
    connect(action,SIGNAL(triggered()),this,SLOT(close()));
    menu->addAction(action);

    trayIcon->setContextMenu(menu);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {

    //���� ��� ����� ������ ������

    case QEvent::WindowStateChange:
    {
        if(isMinimized())
        {
            this->hide();
            e->ignore();
        }
        else
        {
            //setFocus();
            e->accept ();
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::setNetwork(delta3::Network *net)
{
	if(network_)
		disconnect(this);

	network_ = net;

	connect(network_,SIGNAL(listUpdated()),
			this,SLOT(onRedraw()));

}

void MainWindow::on_actionConnect_activated()
{
    if(network_ == NULL)
		return;
    qDebug() << "connecting to server";
    network_->connectToServer();
}

void MainWindow::onRedraw()
{
    qDebug() << "onRedraw()";
    QListWidget *list=ui->listWidget;
    QListWidgetItem *item;
    list->clear();
    qDebug() << "clients: " << network_->getClients().size();
	for(auto i = network_->getClients().begin();
			 i != network_->getClients().end(); i++)
    {
        item = new QListWidgetItem(i.value()->getOs() );
        item->setWhatsThis(QString("%1")
                        .arg( i.value()->getId() ));
        list->addItem(item);
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "Double Click!";
	runTelnet();
}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "Context Menu!";
    if (ui->listWidget->selectedItems().size()==0)
        return;
   // modeMenu_->raise();
    modeMenu_->move(QCursor::pos());
	modeMenu_->show();
}

void MainWindow::runTelnet()
{
    QListWidgetItem *item = NULL;

    if ((item = ui->listWidget->selectedItems()[0]) == NULL)
		return;

    static delta3::Telnet *tel = NULL;
    static TelnetForm *form = NULL;

//	if(tel) {
//		delete tel;
//		delete form;
//	}

	tel = new delta3::Telnet(network_, item->whatsThis().toInt());
	form = new TelnetForm(tel);
	tel->setParent(form);
	form->show();
}


void MainWindow::runGraph()
{
    QListWidgetItem *item = NULL;

    if ((item = ui->listWidget->selectedItems()[0]) == NULL)
        return;

    static delta3::Graphics *graph = NULL;
    static GraphForm *form = NULL;

//	if(graph)
//		delete graph;

	graph = new delta3::Graphics(network_,item->whatsThis().toInt());
	form = new GraphForm(graph);
	graph->setParent(form);
	form->show();
}

/*
void MainWindow::runFile()
{
    if (ui->listWidget->selectedItems().size()==0)
        return;
    QListWidgetItem *item=ui->listWidget->selectedItems()[0];
    FileForm *file = new FileForm(
                network_,
                item->whatsThis().toInt());
            // REVIEW: potential memory leak?
    file->show();
}
*/

void MainWindow::runOptions()
{
    QListWidgetItem *item = NULL;

    if ((item = ui->listWidget->selectedItems()[0]) == NULL)
		return;

	ClientInfoDialog dialog(item->whatsThis().toInt(), network_);
	if (dialog.exec() != ClientInfoDialog::Rejected)
    {
		network_->setClientCaption(item->whatsThis().toInt(), dialog.getCaption());
    }
}


