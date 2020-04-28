#pragma once
#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(EpollServer)
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *btn_;
    QTextEdit *textEdit_;
    EpollServer *server_;

private slots:
    void slotBtnClicked();
};