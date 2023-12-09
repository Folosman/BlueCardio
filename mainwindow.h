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
    // void powerOffClicked();

    void discoveredDevice(const QBluetoothDeviceInfo &device);
    // void startDiscovery();
    // void blueListClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QBluetoothDeviceDiscoveryAgent *bluetoothAgent;
    QBluetoothSocket *socket;
    QString contentString;
};
#endif // MAINWINDOW_H
