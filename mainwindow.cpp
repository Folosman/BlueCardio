#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bluetoothAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(bluetoothAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(discoveredDevice(QBluetoothDeviceInfo)));

    // bluetoothAgent->start();

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    connect(ui->m_findBtn, &QPushButton::clicked, this, &MainWindow::findClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::findClicked()
{
    ui->m_blueList->clear();
    // bluetoothAgent->stop();
    bluetoothAgent->start();
}

void MainWindow::discoveredDevice(const QBluetoothDeviceInfo &device)
{
    ui->m_blueList->addItem(device.address().toString() + " " + device.name());
}

// void MainWindow::blueListClicked(QListWidgetItem *item)
// {
//     contentString = item->text();
//     ui->m_blueList->addItem(contentString);
// }
