#ifndef IMAGE_COMMUN_H
#define IMAGE_COMMUN_H
/**
 * @file imageCommun.h
 * @brief Common header file for constants used in the client-server program.
 * @author Shayan Eram
 */
// Server
#define OK 0x00000001            ///< Client message to continue
#define QUIT 0x00000000          ///< Client message to quit
#define RES1_OK 0x00000003       ///< Resolution 1
#define RES2_OK 0x00000007       ///< Resolution 2
#define READY 0x00000008         ///< Light on
#define IDOWN 0x00000010         ///< Light off
#define PUSHB 0x00000018         ///< Light on and button on

// Client
#define PORT 4099               ///< Port number for communication
#define DELAY 30                ///< Delay in milliseconds for displaying the image
#define RESO1 0x00000002        ///< Constant for resolution 1
#define RESO2 0x00000006        ///< Constant for resolution 2
#define LEARN 0x00000040        ///< Constant for learning mode
#define KNOWN 0x00000080        ///< Constant for known face
#define UNKNOWN 0x000000c0      ///< Constant for unknown face

#define ESC 27                  ///< Value of the ESC key
#define K1 49                   ///< Value of the key 1
#define K2 50                   ///< Value of the key 2
#define K5 53                   ///< Value of the key 5
#define K6 54                   ///< Value of the key 6

#endif  // IMAGE_COMMUN_H
