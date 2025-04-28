// ファイル: ./samples/esp32/ble_motor_control.ino

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// モータ制御用ピン定義（仮の例）
const int motorPin1 = 18; // 正転用
const int motorPin2 = 19; // 逆転用

// BLEサービス・キャラクタリスティックUUID（仮）
#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID "6e400002-b5a3-f393-e0a9-e50e24dcca9e"

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

// モータ制御関数
void stopMotor()
{
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
}

void rotateForward()
{
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
}

void rotateReverse()
{
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
}

class CommandCallback : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0)
        {
            uint8_t command = rxValue[0];
            Serial.print("受信コマンド: ");
            Serial.println(command);

            switch (command)
            {
            case 0x00: // 停止
                stopMotor();
                break;

            case 0x01: // 正転
                rotateForward();
                delay(500); // 0.5秒回転
                stopMotor();
                break;

            case 0x02: // 逆転
                rotateReverse();
                delay(500); // 0.5秒回転
                stopMotor();
                break;

            default:
                Serial.println("不明なコマンド");
                stopMotor();
                break;
            }
        }
    }
};

void setup()
{
    Serial.begin(115200);

    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    stopMotor();

    BLEDevice::init("AutoTunerESP32");
    pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setCallbacks(new CommandCallback());
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("BLEサーバー起動 完了");
}

void loop()
{
    delay(1000);
}
