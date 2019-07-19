#include "Header.h"
#include <iostream>
#include <sstream>

using namespace std;

Light::Light() : current_AUTO_ON(18, 0), current_AUTO_OFF(0,0), default_AUTO_ON(18, 0), default_AUTO_OFF(0,0) {}

void Light::Set_AUTO_ON(unsigned int hrs, unsigned int minutes) {

	current_AUTO_ON.hrs = hrs; current_AUTO_ON.minutes = minutes;

}

void Light::Set_AUTO_OFF(unsigned int hrs, unsigned int minutes) {
	current_AUTO_OFF.hrs = hrs; current_AUTO_OFF.minutes = minutes;
}

bool Light::IsOn() const{

	Time current_time = Clock::CurrentTime();
	if (current_time >= current_AUTO_ON) return true;
}

void Light::CommandLine() {

	while (1) {

		cout << " _________________ " << endl;
		cout << "|                 |" << endl;
		cout << "| LIGHT CONTROLS  |" << endl;
		cout << "|_________________|" << endl;

		cout << "\n\n\n";

		msg_poll.DisplayMessages();

		cout << "\ncmd:";
		string command;
		cin >> command;
		cin.ignore();
		cout << endl;

		MessagePoll::UpperCase(command);	//convert to upper case so that the input check is not case sensitive

		int hrs = -1, minutes = -1;

		if (command == "RETURN_AUTO_OFF") {

			ostringstream out;
			char *temp = (char*)current_AUTO_OFF;
			out << "Current AUTO_OFF time: " << temp << "\n";
			msg_poll.NewMessage(out.str());
			delete temp;
			goto END;
		
		}

		if (command == "RETURN_AUTO_ON") {

			ostringstream out;
			char *temp = (char*)current_AUTO_ON;
			out << "Current AUTO_ON time: " << temp << "\n";
			msg_poll.NewMessage(out.str());
			delete temp;
			goto END;

		}
			
		if (command == "LIGHT_STATUS") {

			switch (IsOn()) {

			case true: msg_poll.NewMessage("Light is currently ON\n"); break;
			case false: msg_poll.NewMessage("Light is currently OFF\n"); break;

			}

		}

		if (sscanf(command.c_str(), "CHANGE_AUTO_ON_%d:%d",&hrs, &minutes )!= EOF && hrs != -1 && minutes != -1) { 

			current_AUTO_ON.hrs = hrs; current_AUTO_ON.minutes = minutes;
			char *temp = (char*)current_AUTO_ON;
			ostringstream out;
			out << "Current AUTO_ON time changed to: " << temp << "\n";
			msg_poll.NewMessage(out.str());
			CLEAR_CONSOLE;
			continue;

		}

		if (sscanf(command.c_str(), "CHANGE_AUTO_OFF_%d:%d", &hrs, &minutes) != EOF && hrs != -1 && minutes != -1) {

			current_AUTO_OFF.hrs = hrs; current_AUTO_OFF.minutes = minutes;
			char *temp = (char*)current_AUTO_OFF;
			ostringstream out;
			out << "Current AUTO_OFF time changed to: " << temp << "\n";
			msg_poll.NewMessage(out.str());

		}

		END: {
			CLEAR_CONSOLE;
		}
	}

}

void Light::Reset() {

	current_AUTO_ON = default_AUTO_ON;
	current_AUTO_OFF = default_AUTO_OFF;

}