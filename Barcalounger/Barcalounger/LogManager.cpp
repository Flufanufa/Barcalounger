#include "LogManager.h"
//LogManager is a singleton class


LogManager* LogManager::logInstance = NULL;
LogManager::LogManager() {
	out = NULL;
}


LogManager::~LogManager() {
	closeFile();
}

void LogManager::openFile() {
	//calls openfile with the default log file name
	openFile("logFile.log");
}

void LogManager::openFile(std::string filename) {
	//closes the file that is currently open and then 
	//creates a new file to write to
	closeFile();
	out = new std::ofstream(filename.c_str());
}

void LogManager::closeFile() {
	//checks to make sure a file is open first
	if (out != NULL) {
		//closes the file and makes sure there are no
		//references to that particular output file
		out->close();
		delete out;
		out = NULL;
	}
}

LogManager *LogManager::getInstance() {
	//checks to see if there is already an instance of the logmanager
	//if there isn't, then it creates an instance then returns it
	if (logInstance == NULL) {
		logInstance = new LogManager;
	}
	return logInstance;
}

void LogManager::log(LMessageType level, std::string message) {
	//The magic happens here
	//The log manager only outputs errors if it is a high enough severity
	if (level <= logLevel && logLevel > LOG_NONE) {
		//makes sure there is a file open
		if (out == NULL) {
			openFile();
		}
		//The message is printed to the file
		(*out) << message << "\n";
		out->flush();
	}
}