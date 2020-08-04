#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getFileData(QString fileName);

public slots:
    void loadFileName();

private:
    QLineEdit * edit;
    QTextEdit * results;
};
#endif // MAINWINDOW_H
