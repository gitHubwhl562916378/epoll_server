#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHBoxLayout>
#include "EpollServer/epollserver.h"
#include "EpollServer/epollloop.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
    QHBoxLayout *mainLay = new QHBoxLayout;

    btn_ = new QPushButton(tr("确定"));
    textEdit_ = new QTextEdit;
    mainLay->addWidget(textEdit_);
    mainLay->addWidget(btn_);
    setLayout(mainLay);

    server_ = new EpollServer();
    server_->Listen("0.0.0.0", 8000, 20);

    connect(btn_, SIGNAL(clicked()), this, SLOT(slotBtnClicked()));
}

MainWindow::~MainWindow()
{
    delete server_;
}

void MainWindow::slotBtnClicked()
{
    server_->Send(textEdit_->toPlainText().toStdString());
}
