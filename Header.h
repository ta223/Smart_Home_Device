#pragma once
#include <string>
#include <forward_list> 
#include <ctime>
#include <vector>
#include <set>

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

	//the parameter is just there to allow overloading
	static std::string CurrentTime(int);	// returns current with format yyyy/mm/dd/hrs/min/sec

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

class Organizer : public SmartHomeDevice {

public:
	void CommandLine();
	void Reset();

private:

	struct List {

		List(std::vector<std::string> &list_of_items);
		bool operator > (const List &other_list) const;
		bool operator < (const List &other_list) const;
		mutable int list_num;
		std::vector<std::string> items;

		/*these are used for operator overloading*/
		int day, month, year;
		int hrs, minutes, seconds;

		/*this is used to print on screen in a different format*/
		std::string date_of_creation;

		void show(MessagePoll &msg_poll, bool append_msg) const;

	};

	struct CompareList {
		bool operator() (List lhs, List rhs) const;
	};

	std::multiset<List, CompareList> storing_lists;	//with multisets the lists are ordered with respect to their date of creation
	bool AddList(std::vector<std::string> &item_list);
	void DeleteList(int list_num);
	void ShowAllLists();

};