#pragma once
#include <string>
#include <forward_list> 
#include <ctime>

#ifdef _WIN32
	#include <windows.h>
	#define CLEAR_CONSOLE   system("cls")	
	#define SLEEP(n) Sleep(n)

#else
	#define CLEAR_CONSOLE printf("\033[H\033[J")  
	#include <unistd.h>
	#define SLEEP(n) sleep(static_cast<int>(n)/1000)

#endif

class SmartHomeDevice {

public:
	virtual void CommandLine() = 0;
	virtual void Reset() = 0;

protected:
	class MessagePoll {

	public:
		void NewMessage(const char *msg);
		void AppendMessage(const char *msg);
		void NewMessage(const std::string &msg);
		void AppendMessage(const std::string &msg);
		void DisplayMessages() const;
		void Clear();
		static void UpperCase(std::string &str);

	private:
		std::string msg;

	}msg_poll;

};

struct Time {

	Time(unsigned int _hrs_, unsigned int _minutes_);
	unsigned int hrs, minutes;
	bool operator >= (const Time &other_obj) const;
	bool operator <= (const Time &other_obj) const;
	operator char*() const;
	void show();

};

class Clock : public SmartHomeDevice{

public:
	Clock();
	void CommandLine();
	static Time CurrentTime();

private:
	std::string current_time;	//current date and time
	std::forward_list<unsigned int> timer_list; unsigned int list_size;

	std::string& GetCurrentTime();
	void AppendTimerList(unsigned int target_time);
	void ClearTimerList();
	void TraverseList();
	void Timer(const double target_time);
	void Reset();
	
};


class Light : public SmartHomeDevice {

public:
	Light();
	void CommandLine();

private:
	const Time default_AUTO_ON, default_AUTO_OFF;
	Time current_AUTO_ON, current_AUTO_OFF;
	void Set_AUTO_ON(unsigned int hrs, unsigned int minutes);
	void Set_AUTO_OFF(unsigned int hrs, unsigned int minutes);
	bool IsOn() const;
	void Reset();
};