#include "Header.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <algorithm>

using namespace std;

Time::Time(unsigned int _hrs_, unsigned int _minutes_) : hrs(_hrs_), minutes(_minutes_) {

	if (minutes >= 60) minutes = 0;
	if (hrs >= 24) hrs = 0;

}

bool Time::operator >= (const Time &other_obj) const{

	if (this->hrs > other_obj.hrs)  return true;
	if (this->hrs == other_obj.hrs && this->minutes >= other_obj.minutes) return true;
	return false;
}

bool Time::operator <= (const Time &other_obj) const{

	if (this->hrs < other_obj.hrs)  return true;
	if (this->hrs == other_obj.hrs && this->minutes <= other_obj.minutes) return true;
	return false;

}

Time::operator char*() const {

	char *str = new char[10];
	sprintf(str, "%.2d:%.2d", hrs, minutes);
	return str;

}

void Time::show() { cout << hrs << ":" << minutes << endl; }

Clock::Clock():list_size(0) {}

string& Clock::GetCurrentTime() {

	time_t t = time(0);
	current_time = ctime(&t);
	return current_time;

}

void Clock::AppendTimerList(unsigned int target_time) { if (list_size < 10) timer_list.push_front(target_time); ++list_size; }

void Clock::ClearTimerList() {timer_list.clear(); list_size = 0;}

void Clock::TraverseList() {

	if (list_size == 0) { msg_poll.NewMessage("Stop watch has not been used recently"); return; }

	forward_list<unsigned int>::iterator itr;
	unsigned int counter = 0;
	ostringstream out;

	for (itr = timer_list.begin(); counter < list_size - 1 || itr != timer_list.end(); ++itr, ++counter) {

		out.str(""); out.clear();
		out << *itr << "s, ";
		msg_poll.AppendMessage(out.str());

	}

	if (itr != timer_list.end()) {

		out.str(""); out.clear();
		out << *itr;
		msg_poll.AppendMessage(out.str());

	}

}

void Clock::Timer(const double target_time){

	
	clock_t start_time = clock();

	cout << "\n\n\n";

	do{

		cout << "\rStop Watch: " << 
			target_time - (static_cast<double>(clock() - start_time) / static_cast<double>(CLOCKS_PER_SEC))
			<< "s ";

	} while (static_cast<double>(clock() - start_time) / static_cast<double>(CLOCKS_PER_SEC) < target_time);

	msg_poll.NewMessage("\a\nTime out\n");

	CLEAR_CONSOLE;

}


void Clock::CommandLine() {

	while (1) {

		cout << " ________________ " << endl;
		cout << "|                |" << endl;
		cout << "| CLOCK CONTROLS |" << endl;
		cout << "|________________|" << endl;

		cout << "\n\n\n";

		msg_poll.DisplayMessages();

		cout << "\ncmd:";
		string command;
		cin >> command;
		cin.ignore();
		cout << endl;

		MessagePoll::UpperCase(command);	//convert to upper case so that the input check is not case sensitive

		uint32_t target_time = -1;

		if (command == "CURRENTDT") {

			GetCurrentTime();
			msg_poll.NewMessage("current time: " + current_time);
			goto END;

		}

		
		if (sscanf(command.c_str(), "STOP_W_%d", &target_time) != EOF && target_time != -1) {

			msg_poll.Clear();
			Timer(target_time);
			AppendTimerList(target_time);
			continue;

		}

		if (command == "LASTOPWTIMERS") {msg_poll.NewMessage("Last timers:"); TraverseList();}

		END: {
			CLEAR_CONSOLE;
		}

	}

}

void Clock::Reset() {

	ClearTimerList();

}

Time Clock::CurrentTime() {

	char s[80];
	time_t t = time(0);
	strftime(s, 80, "%H:%M", localtime(&t));
	int hrs = -1, minutes = -1;
	sscanf(s, "%d:%d", &hrs, &minutes);
	return Time(hrs, minutes);
}

string Clock::CurrentTime(int ignore_this) {

	char s[80];
	time_t t = time(0);
	strftime(s, 80, "%Y/%m/%d/%H/%M/%S", localtime(&t));	//year/month/day/hour/minute/sec
	cout << s << endl;
	return string(s);
}

