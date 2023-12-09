#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QListWidgetItem>

#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bluetoothAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(bluetoothAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(discoveredDevice(QBluetoothDeviceInfo)));

    // bluetoothAgent->start();

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    connect(ui->m_findBtn,      &QPushButton::clicked, this, &MainWindow::findClicked);
    connect(ui->m_powerOffBtn,  &QPushButton::clicked, this, &MainWindow::powerOffClicked);
    connect(ui->m_clearBtn,     &QPushButton::clicked, this, &MainWindow::clearBtnClicked);
    connect(ui->m_blueList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(bluetoothListClicked(QListWidgetItem*)));
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

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
}

void MainWindow::powerOffClicked()
{
    bluetoothAgent->stop();
    discoveryAgentList = discoveryAgent->discoveredDevices();
}

void MainWindow::clearBtnClicked()
{
    ui->m_blueList->clear();
}

void MainWindow::discoveredDevice(const QBluetoothDeviceInfo &device)
{
    ui->m_blueList->addItem(device.address().toString() + " " + device.name());
    // socket->connectToService(device);
}

// void MainWindow::blueListClicked(QListWidgetItem *item)
// {
//     contentString = item->text();
//     ui->m_blueList->addItem(contentString);
// }

void MainWindow::bluetoothListClicked(QListWidget *item)
{
    // (discoveryAgentList.at(item->currentRow())).
    socket->connectToService(QBluetoothAddress((discoveryAgentList.at(item->currentRow())).address()), QBluetoothUuid(QBluetoothUuid::SerialPort));
    QByteArray recievedData = socket->readAll();
    QFile file("log.txt");
    file.open(QIODevice::WriteOnly);
    file.write(recievedData);
    file.close();

}
