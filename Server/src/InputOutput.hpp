/**
 * @file InputOutput.hpp
 * @brief Header file for InputOutput class.
 * This class provides methods to read input from sensors and buttons.
 * @author Shayan Eram
 */

#ifndef INPUTOUTPUT_HPP
#define INPUTOUTPUT_HPP

#include "serverFuncs.hpp"

/**
 * @struct dataBuz
 * @brief Structure representing the data associated with the buzzer.
 */
struct dataBuz
{
    uint32_t frequency; /**< Buzzer frequency */
    uint32_t length;    /**< Buzzer length */
    uint32_t delay;     /**< Delay between buzzers */
};

class InputOutput 
{
public:

    static InputOutput& getInstance();

    /**
     * @brief Function to read the light sensor value.
     * This function reads the light sensor value and returns true if the light is on, false otherwise.
     * @return True if the light is on, false otherwise.
     */
    bool readADC();

    /**
     * @brief Function to read the button input value.
     * This function reads the button input value and returns true if the button is pressed, false otherwise.
     * @return True if the button is pressed, false otherwise.
     */
    bool readGPIO();

    /**
     * @brief Reads data from the text file and stores it in a vector.
     * 
     * @param filename The name of the text file.
     * @return A vector containing the data from the text file.
     */
    std::vector<dataBuz> readFile(std::string filename);

    /**
     * @brief Controls the buzzer based on the given frequency and length.
     * 
     * @param frequency Buzzer frequency.
     * @param length Buzzer length.
     */
    void Buzzer(uint32_t frequency, uint32_t length);

    /**
     * @brief Plays the buzzers based on the data stored in the vector.
     * 
     * @param dataTxt Vector containing data from the text file.
     */
    void playBuzzer(const std::vector<dataBuz> &dataTxt);

private:
    InputOutput() = default;
    InputOutput(const InputOutput&) = delete;
    InputOutput& operator=(const InputOutput&) = delete;

    static constexpr int LEARN = 0x00000040; /**< Message type for learning */
    static constexpr int KNOWN = 0x00000080; /**< Message type for known data */
    static constexpr int UNKOWN = 0x000000c0; /**< Message type for unknown data */
};

#endif // INPUTOUTPUT_HPP