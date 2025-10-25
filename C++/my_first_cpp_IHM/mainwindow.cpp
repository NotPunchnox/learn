#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    compteur1(0),
    timer (new QTimer)
{
    ui->setupUi(this);
    ui->lcdNumber_1->display(0);
    connect(ui->pushButton_1, &QPushButton::clicked, this, &MainWindow::monAction);

    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::actionTimer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::monAction() {
    compteur1++;
    ui->lcdNumber_1->display(compteur1);
}

void MainWindow::actionTimer() {
    static int compteur = 0;
    compteur++;
    ui->lcdNumber_3->display(compteur);

}
