//#include <Arduino.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 1
#endif

#include "debug.h"
#include <MemoryFree.h>
#include <SPI.h>
#include <Ethernet.h>


const unsigned long BAUD_RATE = 9600;

const byte BUFFER_SIZE = 32;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];

#ifndef SYSTEM_NAME
#define SYSTEM_NAME "test"
#endif

// see http://forum.arduino.cc/index.php?topic=47538.0
// for a discussion about why the following needs to be a string (double quotes)
// not char (sinlge quotes) for use with strcat
const char* MQTT_SEPARATOR = "/";

// strings
const char STRING_CONNECTED[] PROGMEM   = "CONNECTED";
const char STRING_OK[] PROGMEM          = "OK";
const char STRING_ERROR[] PROGMEM       = "ERROR";
const char STRING_START[] PROGMEM       = "START";
const char STRING_END[] PROGMEM         = "END";
const char STRING_SLEEP[] PROGMEM       = "SLEEP";
const char STRING_STATUS[] PROGMEM      = "status";
const char STRING_CONTROL[] PROGMEM     = "control";
const char STRING_MEASUREMENT[] PROGMEM = "measurement";

PGM_P const STRINGS[] PROGMEM = {
    STRING_CONNECTED,   // idx = 0
    STRING_OK,          // idx = 1
    STRING_ERROR,       // idx = 2
    STRING_START,       // idx = 3
    STRING_END,         // idx = 4
    STRING_SLEEP,       // idx = 5
    STRING_STATUS,      // idx = 6
    STRING_CONTROL,     // idx = 7
    STRING_MEASUREMENT, // idx = 8
};

/* MQTT_PAYLOADS indices, must match table above */
typedef enum {
  STRING_CONNECTED_IDX   = 0,
  STRING_OK_IDX          = 1,
  STRING_ERROR_IDX       = 2,
  STRING_START_IDX       = 3,
  STRING_END_IDX         = 4,
  STRING_SLEEP_IDX       = 5,
  STRING_STATUS_IDX      = 6,
  STRING_CONTROL_IDX     = 7,
  STRING_MEASUREMENT_IDX = 8,
} strings;

// item strings
const char MQTT_STATUS[] PROGMEM     = "mqtt";
const char INTERVAL_STATUS[] PROGMEM = "interval";
const char IP_ADDR_STATUS[] PROGMEM  = "ip_addr";
const char UPTIME_STATUS[] PROGMEM   = "uptime";
const char MEMORY_STATUS[] PROGMEM   = "memory";
const char TIME_STATUS[] PROGMEM     = "time";
const char ALARM_STATUS[] PROGMEM    = "alarm";
const char RELAY_STATUS[] PROGMEM    = "relay";

PGM_P const ITEM_STRINGS[] PROGMEM = {
    MQTT_STATUS,     // idx = 0
    INTERVAL_STATUS, // idx = 1
    IP_ADDR_STATUS,  // idx = 2
    UPTIME_STATUS,   // idx = 3
    MEMORY_STATUS,   // idx = 4
    TIME_STATUS,     // idx = 5
    ALARM_STATUS,    // idx = 6
    RELAY_STATUS,    // idx = 7
};

/* ITEM_STRINGS indices, must match table above */
typedef enum {
  MQTT_STATUS_IDX     = 0,
  INTERVAL_STATUS_IDX = 1,
  IP_ADDR_STATUS_IDX  = 2,
  UPTIME_STATUS_IDX   = 3,
  MEMORY_STATUS_IDX   = 4,
  TIME_STATUS_IDX     = 5,
  ALARM_STATUS_IDX    = 6,
  RELAY_STATUS_IDX    = 7,
} item_strings;

char* construct_topic(byte groupIdx, byte itemIdx) {
  // constructs an MQTT topic of form
  // <PROJECT_NAME>/<GROUP_IDX>/<ITEM_IDX>
  print_freememory();
  topicBuffer[0] = '\0';
  char buffer[BUFFER_SIZE];
  print_freememory();
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  strcat(topicBuffer, SYSTEM_NAME);
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  strcat(topicBuffer, MQTT_SEPARATOR);
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  buffer[0] = '\0';
  DEBUG_LOG(1, "buffer: ");
  DEBUG_LOG(1, buffer);
  strcpy_P(buffer,
           (char *)pgm_read_word(&(STRINGS[groupIdx])));
  DEBUG_LOG(1, "buffer: ");
  DEBUG_LOG(1, buffer);
  strcat(topicBuffer, buffer);
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  strcat(topicBuffer, MQTT_SEPARATOR);
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  buffer[0] = '\0';
  DEBUG_LOG(1, "buffer: ");
  DEBUG_LOG(1, buffer);
  strcpy_P(buffer,
           (char *)pgm_read_word(&(ITEM_STRINGS[itemIdx])));
           DEBUG_LOG(1, "buffer: ");
           DEBUG_LOG(1, buffer);
  strcat(topicBuffer, buffer);
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  print_freememory();
  free(buffer);
  print_freememory();
  return topicBuffer;
}

void publish_from_progmem() {
  Serial.print("From Progmem: ");
  print_freememory();
  payloadBuffer[0] = '\0';
  strcpy_P(payloadBuffer,
           (char *)pgm_read_word(&(STRINGS[STRING_CONNECTED_IDX])));
  print_freememory();
  Serial.println(payloadBuffer);
}

void publish_construct_from_progmem() {
  Serial.println("Construct from progmem: ");
  Serial.println(construct_topic(STRING_STATUS_IDX, MQTT_STATUS_IDX));
}

void compare_strings() {
  Serial.print("Compare testStr: ");
  const char* testStr = "another/status/mqtt";
  Serial.print(testStr);
  if (strcmp(construct_topic(STRING_STATUS_IDX, MQTT_STATUS_IDX), testStr) == 0) {
//  if (strcmp(topicBuffer, testStr) == 0) {
    Serial.println(" same");
  } else {
    Serial.println(" different");
  }
}

void publish_integer() {
  Serial.print("Integer: ");
  int i = 30000;
  payloadBuffer[0] = '\0';
  itoa(i, payloadBuffer, 10);
  Serial.println(payloadBuffer);
}

void publish_long() {
  Serial.print("Long: ");
  long speedOfLight = 186000L;
  payloadBuffer[0] = '\0';
  ltoa(speedOfLight, payloadBuffer, 10);
  Serial.println(payloadBuffer);
}

void publish_float(byte decimal_places) {
  Serial.print("Float: ");
  float pi = 3.14159;
  payloadBuffer[0] = '\0';
  dtostrf(pi,1,decimal_places, payloadBuffer);
  Serial.println(payloadBuffer);
}

void publish_values() {
  Serial.print("Values: ");
  int i = 30000;
  long speedOfLight = 186000L;
  float pi = 3.14159;
  char c = 'c';
  char* cs = "cs";
  sprintf(payloadBuffer, "%d,%d,%.2f,%c,%s", i, speedOfLight, pi, c, cs);
  Serial.println(payloadBuffer);
}

void publish_ip_address() {
  Serial.print("IP: ");
  IPAddress ip(192, 168, 1, 254);
  payloadBuffer[0] = '\0';
  sprintf(payloadBuffer, "%i%c%i%c%i%c%i", ip[0], '.', ip[1], '.', ip[2], '.',
          ip[3]);
  Serial.println(payloadBuffer);
}

void print_freememory() {
  Serial.print("Mem: ");
  Serial.println(getFreeMemory());
}

/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
  Serial.begin(BAUD_RATE);

  Serial.println(SYSTEM_NAME);

  print_freememory();

  publish_from_progmem();
  publish_construct_from_progmem();
  compare_strings();
  publish_integer();
  publish_long();
  publish_float(1);
  publish_float(4);
  publish_values();
  publish_ip_address();
}

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{}
