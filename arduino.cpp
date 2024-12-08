#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent), serial(nullptr)
{
    serial = new QSerialPort(this);  // Create a new QSerialPort object
    connect(serial, &QSerialPort::readyRead, this, &Arduino::readData);  // Connect the readyRead signal to the readData slot
}

bool Arduino::connectArduino()
{
    // Get a list of available serial ports
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Port found: " << info.portName();
        // Here, we can try to connect to a specific port or just use the first available one
        serial->setPort(info);
        serial->setBaudRate(QSerialPort::Baud9600);  // Set the baud rate (same as Arduino)
        serial->setDataBits(QSerialPort::Data8);     // 8 data bits
        serial->setParity(QSerialPort::NoParity);    // No parity
        serial->setStopBits(QSerialPort::OneStop);   // One stop bit
        serial->setFlowControl(QSerialPort::NoFlowControl); // No flow control

        if (serial->open(QIODevice::ReadWrite)) {
            qDebug() << "Connected to Arduino on port: " << info.portName();
            return true;  // Successfully connected
        }
    }
    qDebug() << "Failed to connect to Arduino.";
    return false;  // Connection failed
}

void Arduino::disconnectArduino()
{
    if (serial->isOpen()) {
        serial->close();  // Close the serial connection
        qDebug() << "Disconnected from Arduino.";
    }
}

void Arduino::writeData(const QByteArray &data)
{
    if (serial->isOpen()) {
        serial->write(data);  // Send data to Arduino
        qDebug() << "Data sent to Arduino: " << data;
    } else {
        qDebug() << "Serial port not open. Cannot send data.";
    }
}


void Arduino::readData()
{
    // Check if data is available to read from the serial port
    if (serial->canReadLine()) {
        QByteArray data = serial->readAll();  // Read the data from Arduino
        QString dataString = QString::fromUtf8(data);  // Convert to QString (assuming UTF-8 encoding)

        // Emit the dataReceived signal with the received data
        emit dataReceived(dataString);

        qDebug() << "Data received from Arduino: " << dataString;
    }
}
