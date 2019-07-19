#include "Header.h"
#include <string>
#include <iostream>
#include <ctime>

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

void foo(string list_of_items) {

	string itr_str;
	string::iterator itr;

	vector<string> items;

	for (itr = list_of_items.begin(); itr != list_of_items.end(); ++itr) {

		if (*itr != ',') if (*itr != ' ') itr_str += *itr;

		else { items.push_back(itr_str); itr_str.clear(); }	//add the item and proceed to build the next string

	}

	int size = (int)items.size();
	for (int i = 0; i < size; i++)
		cout << items[i].c_str() << endl;

}

int main() {

	//class Clock clock;
	//clock.CommandLine();
	foo("Jesus, Moses, Noah, Jacob, Joseph");

	Light light;
	light.CommandLine();

	return 0;
}