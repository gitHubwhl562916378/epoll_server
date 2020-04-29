#pragma once
#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(EpollServer)
QT_FORWARD_DECLARE_CLASS(HandDataSink)
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *btn_;
    QTextEdit *textEdit_;
    QLineEdit *lineEdit_;
    EpollServer *server_;
    HandDataSink *sink_;

private slots:
    void slotBtnClicked();
};