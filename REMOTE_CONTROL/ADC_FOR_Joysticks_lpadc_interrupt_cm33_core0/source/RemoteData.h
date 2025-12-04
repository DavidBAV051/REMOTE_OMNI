/*
 * RemoteData.h
 *
 *  Created on: Dec 2, 2025
 *      Author: diego
 */
/* RemoteData.h */
#ifndef REMOTE_DATA_H_
#define REMOTE_DATA_H_

#include <stdint.h>

#define REMOTE_PACKET_HEADER 0xC5

/* * Structure sent from Remote MCXN947 -> Remote ESP32
 * Size: 24 Bytes (Fits within ESP_SPI_TRANSFER_SIZE = 40)
 */
typedef struct __attribute__((packed)) {
    uint32_t header;        // 0xC5 + Packet Counter
    float vx;               // Linear Velocity X (m/s)
    float vy;               // Linear Velocity Y (m/s)
    float phi;              // Angular Velocity (rad/s)
    uint32_t buttons;       // Button states (bitmask)
    uint32_t timestamp;     // Time reference
} RemoteCommand_t;

#endif /* REMOTE_DATA_H_ */
