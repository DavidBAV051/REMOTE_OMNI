/* source/RobotTelemetry.h */
#ifndef ROBOT_TELEMETRY_H_
#define ROBOT_TELEMETRY_H_

#include <stdint.h>

/* Packet Headers */
#define TELEMETRY_PACKET_ID 0xA1  // Robot -> Remote
#define REMOTE_PACKET_HEADER 0xC5 // Remote -> Robot

/* * Telemetry Structure (Robot -> Remote)
 * Size: 36 bytes (Fits in 40-byte SPI buffer)
 */
typedef struct __attribute__((packed)) {
    uint32_t packet_header;     // [ID (8b) | Counter (24b)]

    /* Motor Speeds (rad/s) */
    float speed_m1;
    float speed_m2;
    float speed_m3;
    float speed_m4;

    /* Motor Currents (ADC Raw Values) */
    uint16_t adc_m1;
    uint16_t adc_m2;
    uint16_t adc_m3;
    uint16_t adc_m4;

    /* Extra Telemetry */
    uint32_t reserved_flags;
    uint32_t timestamp;         // Software tick
} RobotTelemetry_t;

/* * Command Structure (Remote -> Robot)
 * Size: 24 Bytes
 */
typedef struct __attribute__((packed)) {
    uint32_t header;        // 0xC5 + Packet Counter
    float vx;               // Linear Velocity X (m/s)
    float vy;               // Linear Velocity Y (m/s)
    float phi;              // Angular Velocity (rad/s)
    uint32_t buttons;       // Button states
    uint32_t timestamp;     // Time reference
} RemoteCommand_t;

/* Public API */
void Robot_SendTelemetry(void);

#endif /* ROBOT_TELEMETRY_H_ */
