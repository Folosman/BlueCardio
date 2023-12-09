#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtBluetooth>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>

#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void findClicked();
    // void powerOnClicked();
    void powerOffClicked();
    void clearBtnClicked();

    void discoveredDevice(const QBluetoothDeviceInfo &device);
    // void startDiscovery();
    // void blueListClicked(QListWidgetItem *item);
    void bluetoothListClicked(QListWidget *item);

private:
    Ui::MainWindow *ui;
    QBluetoothDeviceDiscoveryAgent *bluetoothAgent;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket;
    QList<QBluetoothDeviceInfo> discoveryAgentList;
    QString contentString;
};
#endif // MAINWINDOW_H
