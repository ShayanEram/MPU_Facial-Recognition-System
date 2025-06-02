/**
 * @file InputOutput.cpp
 * @brief Implementation file for InputOutput class.
 * @author Shayan Eram
 */

#include "InputOutput.hpp"

static InputOutput& InputOutput::getInstance()
{
	static InputOutput instance;
	return instance;
}

bool InputOutput::readADC()
{
	std::string lightPath = "/sys/class/saradc/ch0";
	std::string lightValue;

	ifstream lightFile;
	lightFile.open(lightPath.c_str());

	if(lightFile.fail()){
		cout << "Failed to open lightFile";
		exit(-1);
	}

	lightFile >> lightValue;
	cout << lightValue << endl;

	lightFile.close();
	
	//inverse logique
	if (stoi(lightValue) < 1000)
		return true;
	else
		return false;
}

bool InputOutput::readGPIO()
{
	std::string exportPath = "/sys/class/gpio/export";
	std::string directionPath = "/sys/class/gpio/gpio228/direction";
	std::string btnPath = "/sys/class/gpio/gpio228/value";

	std::string btnValue;

	std::ofstream exportFile;
	std::ofstream directionFile;
	std::ifstream btnFile;

	exportFile.open(exportPath.c_str(),std::ofstream::out | std::ios::trunc);
	directionFile.open(directionPath.c_str(),std::ofstream::out | std::ios::trunc);

	exportFile << "228";
	directionFile << "in";

	exportFile.close();
	directionFile.close();

	btnFile.open(btnPath.c_str());
	if(btnFile.fail()){
		cout << "failed to open btnFile" << endl;
		exit(-1);
	}

	btnFile >> btnValue;

	btnFile.close();

	// inverse logique
	if(stoi(btnValue) == 0)
		return true;
	else
		return false;
}

std::vector<dataBuz> InputOutput::readFile(std::string filename)
{
    std::vector<dataBuz> dataTxt;

    dataBuz c;

    std::ifstream file(filename.c_str(), std::ifstream::in);
    if (file.fail())
    {
        std::cout << "Failed to open " + filename << std::endl;
        exit(EXIT_FAILURE);
    }
    while (!file.eof())
    {
        std::string data1, data2, data3, data4, data5, frequency, length, delay;

        file >> data1 >> data2 >> data3;
        file.ignore(1);

        file >> data4 >> data5;
        file.ignore(1);

        frequency.assign(data2, 10, 3);
        length.assign(data3, 7, 3);
        delay.assign(data5, 0, 3);

        c.frequency = atoi(frequency.c_str());
        c.delay = atoi(delay.c_str());
        c.length = atoi(length.c_str());

        dataTxt.push_back(c);
    }
    return dataTxt;
}

void InputOutput::Buzzer(uint32_t frequency, uint32_t length)
{
    std::string frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
    std::string enablePath = "/sys/devices/pwm-ctrl.42/enable0";

    // Write the file to activate it
    ofstream freqFile;
    freqFile.open(frequencyPath.c_str());
    freqFile << frequency;

    ofstream enableFile;
    enableFile.open(enablePath.c_str());
    enableFile << 1;

    freqFile.close();
    enableFile.close();

    // Play for the given time in ms
    usleep(length * 1000);

    // Write the file to close it
    enableFile.open(enablePath.c_str());
    enableFile << 0;
    enableFile.close();
}

void InputOutput::playBuzzer(const std::vector<dataBuz> &dataTxt)
{
    for (const auto &it : dataTxt)
    {
        Buzzer(it.frequency, it.length);
        usleep((it.delay - it.length) * 1000);
    }
}