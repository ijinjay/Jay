#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString code = ui->codeInput->toPlainText();
    QString wordResult = "Word Result\n";
    wordResult.append(code);
    ui->word->setPlainText(wordResult);
}
