#include <iostream>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include "EpollServer/epollserver.h"
#include "EpollServer/epollloop.h"
#include "HandDataSink.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
    QVBoxLayout *mainLay = new QVBoxLayout;

    lineEdit_ = new QLineEdit;
    btn_ = new QPushButton(tr("发送"));
    textEdit_ = new QTextEdit;
    // textEdit_->setEnabled(false);
    mainLay->addWidget(textEdit_);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(lineEdit_);
    hlay->addWidget(btn_);
    mainLay->addLayout(hlay);
    setLayout(mainLay);

    server_ = new EpollServer();
    sink_ = new HandDataSink;
    sink_->SetDataCallBack([&](const std::string &msg, const Socket*){
        QMetaObject::invokeMethod(textEdit_, "insertPlainText", Q_ARG(QString, QString::fromStdString(msg)));
    });
    server_->SetNewConnectCB([&](std::shared_ptr<BasicStream> stream){
        stream->SetDataSink(sink_);
    });
    server_->Listen("0.0.0.0", 8000, 20);

    connect(btn_, SIGNAL(clicked()), this, SLOT(slotBtnClicked()));
}

MainWindow::~MainWindow()
{
    delete server_;
    server_ = nullptr;
    delete sink_;
    sink_ = nullptr;
}

void MainWindow::slotBtnClicked()
{
    server_->Send(lineEdit_->text().toStdString());
}
