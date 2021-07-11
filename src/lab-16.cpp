/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/ianp/Documents/iot2021/labs/lab-16/src/lab-16.ino"
#include "oled-wing-adafruit.h"

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);
void setup();
void loop();
#line 3 "/home/ianp/Documents/iot2021/labs/lab-16/src/lab-16.ino"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

OledWingAdafruit display;
const size_t UART_TX_BUF_SIZE = 20;

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

bool JustConnected = false;
bool Disconnect = false;
//BleAdvertisingData data;

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  if ((char)data[0] == '1')
  {
    display.println("got a 1");
    display.display();
  }
  else if ((char)data[0] == '0')
  {
    Disconnect = true;
  }
}

void setup()
{
  pinMode(D7, OUTPUT);

  display.setup();
  display.clearDisplay();
  display.display();

  BLE.on();
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);
}

void loop()
{
  if (BLE.connected())
  {
    if (!JustConnected)
    {
      JustConnected = true;

      digitalWrite(D7, HIGH);
      delay(3000);

      uint8_t txBuf[UART_TX_BUF_SIZE];
      String message = "Hello, BlueFruit!\n";
      message.toCharArray((char *)txBuf, message.length() + 1);
      txCharacteristic.setValue(txBuf, message.length() + 1);
    }
    digitalWrite(D7, HIGH);
    if (Disconnect)
    {
      BLE.disconnect();
      Disconnect = false;
    }
  }
  else
  {
    JustConnected = false;
    digitalWrite(D7, LOW);
  }
}