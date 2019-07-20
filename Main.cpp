#include "Header.h"
#include <string>
#include <iostream>
#include <ctime>
#include <set>
#include <iterator>

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


int main() {

	Organizer org;
	org.CommandLine();

	return 0;
}