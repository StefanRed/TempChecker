/* Was soll rein:
 * - SSID / PW
 * - Mess-Intervalle
 * - Ziel-Server oder Adresse
 * - Schwellenwerte
 * - BAtteriestatus
 */

// ----------------------------------------------------------------------------
// Temperature scanner defining
#define DHT22_PIN 4 // Replace 4 with your actual DHT pin number

// ----------------------------------------------------------------------------
// Set the delay between every Measurement here
int delayMeasurement = 10000; // in milliseconds, 60000 = 60 Seconds

// ----------------------------------------------------------------------------
// Set the baud rate if needed
int baudRate = 115200;

/*
//-----------------------------------------------------------------------------
// WiFi Scanner
#define SSID "RedNet"
#define PASS "xxxx"

//-----------------------------------------------------------------------------
// Send Data to the Server
#define SERVER "192.168.0.1"
#define PORT 8080
*/

// ----------------------------------------------------------------------------
// Wi-Fi configurations - change and add as needed

const char *ssidList[] = {
    "RedNet", "SSID_2"}; // Array with SSIDs, add the needed SSIDs here
const char *passwordList[] = {"xxx", "PASSWORD_2"}; // Array with the Passwords
const int numNetworks =
    sizeof(ssidList) /
    sizeof(ssidList[0]); // Calculating the ammount of Networks by deviding the
                         // amount of SSIDs by the the size of the first SSID in
                         // the array

// FIXE VERSION DER EINGABE, SCHWER ZU ERWEITERN
#define WIFI_SSID1 "RedNet"
#define WIFI_PASSWORD1 "xxx"
#define WIFI_IP1 "192.168.0.100"
#define WIFI_PORT1 8080

#define WIFI_SSID2 "SSID_2"
#define WIFI_PASSWORD2 "PASSWORD_2"
#define WIFI_IP2 ""
#define WIFI_PORT2 8080

// Number of Wi-Fi configurations
#define NUM_WIFI_CONFIGS                                                       \
  2 // After adding WIFIs above - change this number to the correct amount

// Number of attempts to connect per Wi-Fi configuration
#define MAX_ATTEMPTS 3
