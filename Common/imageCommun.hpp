/**
 * @file imageCommun.hpp
 * @brief Common header file for constants used in the client-server program.
 * @author Shayan Eram
 */

#ifndef IMAGE_COMMUN_H
#define IMAGE_COMMUN_H

#include <cstdint>

// Server
constexpr uint32_t OK = 0x00000001;          ///< Client message to continue
constexpr uint32_t QUIT = 0x00000000;        ///< Client message to quit
constexpr uint32_t RES1_OK = 0x00000003;     ///< Resolution 1
constexpr uint32_t RES2_OK = 0x00000007;     ///< Resolution 2
constexpr uint32_t READY = 0x00000008;       ///< Light on
constexpr uint32_t IDOWN = 0x00000010;       ///< Light off
constexpr uint32_t PUSHB = 0x00000018;       ///< Light on and button on

// Client
constexpr uint16_t PORT = 4099;              ///< Port number for communication
constexpr uint32_t DELAY = 30;               ///< Delay in milliseconds for displaying the image
constexpr uint32_t RESO1 = 0x00000002;       ///< Constant for resolution 1
constexpr uint32_t RESO2 = 0x00000006;       ///< Constant for resolution 2
constexpr uint32_t LEARN = 0x00000040;       ///< Constant for learning mode
constexpr uint32_t KNOWN = 0x00000080;       ///< Constant for known face
constexpr uint32_t UNKNOWN = 0x000000c0;     ///< Constant for unknown face

// Keys
constexpr uint8_t ESC = 27;                  ///< Value of the ESC key
constexpr uint8_t K1 = 49;                   ///< Value of the key 1
constexpr uint8_t K2 = 50;                   ///< Value of the key 2
constexpr uint8_t K5 = 53;                   ///< Value of the key 5
constexpr uint8_t K6 = 54;                   ///< Value of the key 6

#endif  // IMAGE_COMMUN_H
