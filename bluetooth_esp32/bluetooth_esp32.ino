
/**
 * @board Wemos D1 R32 - (wemos D1 mini esp32)
 */

/* Debug defines -------------------------------------------------------------*/

#define DEBUG 1

#if DEBUG == 1
#define LOG(x) Serial.print(x)
#define LOGX(x) Serial.print(x, HEX)
#define LOGS(s, x)          \
    {                       \
        Serial.print(F(s)); \
        Serial.print(" ");  \
        Serial.print(x);    \
    }
#define LOGSX(s, x)           \
    {                         \
        Serial.print(F(s));   \
        Serial.print(" ");    \
        Serial.print(x, HEX); \
    }
#define LOGSN(s, x) \
    {               \
        LOGS(s, x); \
        LOGF("\n"); \
    }
#define LOGF(s) Serial.print(F(s))
#define PRINTF(s, ...) Serial.printf(PSTR(s), __VA_ARGS__)
#else
#define LOG(x)
#define LOGX(x)
#define LOGS(s, x)
#define LOGSX(s, x)
#define LOGSN(s, x)
#define LOGF(s)
#define PRINTF(s, ...)
#endif

/* Includes ------------------------------------------------------------------*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <sstream>


/* BLE Setup -----------------------------------------------------------------*/
#define SCAN_TIME (1)
const std::string search_name = "SPG0-05:C9:EE:32";

BLEScan *pBLEScan;

struct BleContext{
    bool start_next_scan = true;
    bool scan_done = false;
    bool device_found = false;
    int16_t rssi;
};

BleContext ble_ctx;

/* Callback class ------------------------------------------------------------*/
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        if (advertisedDevice.haveName())
        {
            std::string name = advertisedDevice.getName(); 
            if (name == search_name)
            {
                // PRINTF("Advertised Device (%d dBm): %s \n", advertisedDevice.getRSSI(), advertisedDevice.toString().c_str());
                ble_ctx.rssi = advertisedDevice.getRSSI();
                ble_ctx.device_found = true;
            }
        }
    }
};


/**
 * Callback to scan complete
*/
void scan_complete_cb(BLEScanResults results)
{
    ble_ctx.scan_done = true;
    pBLEScan->clearResults();
}

/* Setup ---------------------------------------------------------------------*/
void setup()
{
#if (DEBUG == 1)
    Serial.begin(115200);
    delay(2000);
    LOGF("\n\n-----------------------------\n"
         "Begin:\n" __FILE__
         "\n");
#endif

    LOGF("Starting Scan...\n");
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); // create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99); // less or equal setInterval value
}

/* Loop ----------------------------------------------------------------------*/
void loop()
{

    if (ble_ctx.start_next_scan)
    {

        ble_ctx.start_next_scan = false;
        ble_ctx.scan_done = false;
        ble_ctx.device_found = false;
        pBLEScan->start(SCAN_TIME, scan_complete_cb, false);
    }

    if (ble_ctx.scan_done)
    {
        ble_ctx.scan_done = false;
        ble_ctx.start_next_scan = true;
        if (ble_ctx.device_found)
        {
            PRINTF("Device RSSI: %d\n", ble_ctx.rssi);
        }
    }
}