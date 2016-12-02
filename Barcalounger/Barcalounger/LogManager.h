#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <fstream>
#include <string>




class LogManager
{
public:
	enum LMessageType { LOG_NONE = 0, LOG_ERROR, LOG_WARN, LOG_TRACE, LOG_INFO };
	static LogManager *getInstance();
	~LogManager();
	//Closes the file that is currently open - never needs to be 
	//called by the user, but can be if they so choose
	void closeFile();
	//opens a specific file that the user sends through or uses the default if
	//nothing is passed through
	void openFile();
	void openFile(std::string fileName);

	//set/get the level for what messages will be written to the file
	void setLogLevel(LMessageType level) { logLevel = level; }
	LMessageType getLogLevel() { return logLevel; }

	void log(LMessageType level, std::string message);
	//each calls the log function if the user does not want to type what kind of message
	//is being sent
	void error(std::string s) { log(LOG_ERROR, s); }
	void warn(std::string s) { log(LOG_WARN, s); }
	void trace(std::string s) { log(LOG_TRACE, s); }
	void info(std::string s) { log(LOG_INFO, s); }

private:
	LMessageType logLevel;
	// ONLY ONE INSTANCE WILL BE CREATED
	static LogManager	*logInstance;
	std::ofstream		*out;
	LogManager();
};

#endif // !LOG_MANAGER_H

