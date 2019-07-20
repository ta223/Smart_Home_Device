#include "Header.h"
#include <iostream>

using namespace std;

void SmartHomeDevice::MessagePoll::NewMessage(const char *msg) { this->msg = msg; }

void SmartHomeDevice::MessagePoll::AppendMessage(const char *msg) { this->msg += msg; }

void SmartHomeDevice::MessagePoll::DisplayMessages() const { if (!msg.empty()) cout << msg << endl; }

void SmartHomeDevice::MessagePoll::NewMessage(const std::string &msg) { this->msg = msg; }

void SmartHomeDevice::MessagePoll::AppendMessage(const std::string &msg) { this->msg += msg; }

void SmartHomeDevice::MessagePoll::Clear() { msg.clear(); }

void SmartHomeDevice::MessagePoll::UpperCase(std::string &str) {

	int size = static_cast<int>(str.size());

	for (int i = 0; i < size; i++) str[i] = toupper(str[i]);

}

/*if SmartHomeDevice::pointer == this then this becomes a recursive function*/
void SmartHomeDevice::CommandLine() {

	START: {
		CLEAR_CONSOLE;
	}

	cout << "		____________________________" << endl;
	cout << "		||                        ||" << endl;
	cout << "		|| SMART HOME CONTROLLER  ||" << endl;
	cout << "		||       MAIN MENU        ||" << endl;
	cout << "		||________________________||" << endl;
	cout << "\n\n\n";

	cout << "Type CLOCK, ORGANIZER, LIGHTS, RESET to access the respective controls\n\n";

	msg_poll.DisplayMessages();

	cout << "cmd:";
	string command;
	cin >> command;
	cin.ignore();
	cout << endl;

	MessagePoll::UpperCase(command);

	if (command == "CLOCK") { pointer = derived_classes[1]; goto END; }

	if (command == "ORGANIZER") { pointer = derived_classes[2]; goto END; }

	if (command == "LIGHTS") { pointer = derived_classes[3]; goto END; }

	if (command == "RESET") pointer->Reset();

	if (command == "EXIT") return;

	END: {
		CLEAR_CONSOLE;
	}

	 pointer->CommandLine();
	 goto START;

}

void SmartHomeDevice::Reset() {
	/*nothing to see here*/
}


int main() {

	SmartHomeDevice smart_home_device;
	smart_home_device.pointer = &smart_home_device;

	smart_home_device.derived_classes[0] = &smart_home_device;
	smart_home_device.derived_classes[1] = new Clock;
	smart_home_device.derived_classes[2] = new Organizer;
	smart_home_device.derived_classes[3] = new Light;
	smart_home_device.CommandLine();

	delete smart_home_device.derived_classes[1];
	delete smart_home_device.derived_classes[2];
	delete smart_home_device.derived_classes[3];

	return 0;
}