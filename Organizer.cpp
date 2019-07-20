#include "Header.h"
#include <iostream>
#include <sstream>

using namespace std;

Organizer::List::List(vector<string> &list_of_items) : list_num(0) {
	
	/*get date in the correct format*/
	string time_format = Clock::CurrentTime(1);
	sscanf(time_format.c_str(), "%d/%d/%d/%d/%d/%d", &year, &month, &day, &hrs, &minutes, &seconds);

	int size = static_cast<int>(list_of_items.size());
	for (int i = 0; i < size; i++) 
		items.push_back(list_of_items[i]);
	
	/*save the date in the date_of_creation string with the required format*/
	char out[100];
	sprintf(out, "%.2d-%.2d-%.2d  %.2d:%.2d", day, month, year, hrs, minutes);
	date_of_creation = out;

}


bool Organizer::List::operator > (const List &other) const{

	if (this->year > other.year) return true;

	else if (this->year == other.year) {

		if (this->month > other.month) return true;	

		else if (this->month == other.month) {

			/*s_date1 and s_date2 represents how many seconds have passed since the beginning of the month*/
			unsigned long int s_date1 = this->seconds;
			s_date1 += this->minutes * 60;
			s_date1 += this->hrs * 60 * 60;
			s_date1 += this->day * 24 * 60 * 60;

			unsigned long int s_date2 = other.seconds;
			s_date2 += other.minutes * 60;
			s_date2 += other.hrs * 60 * 60;
			s_date2 += other.day * 24 * 60 * 60;
			
			if (s_date1 > s_date2) return true;

		}

	}


	return false;
}

bool Organizer::List::operator < (const List &other) const {

	if (this->year < other.year) return true;

	else if (this->year == other.year) {

		if (this->month < other.month) return true;

		else if (this->month == other.month) {

			/*s_date1 and s_date2 represents how many seconds have passed since the beginning of the month*/
			unsigned long int s_date1 = this->seconds;
			s_date1 += this->minutes * 60;
			s_date1 += this->hrs * 60 * 60;
			s_date1 += this->day * 24 * 60 * 60;

			unsigned long int s_date2 = other.seconds;
			s_date2 += other.minutes * 60;
			s_date2 += other.hrs * 60 * 60;
			s_date2 += other.day * 24 * 60 * 60;

			if (s_date1 < s_date2) return true;

		}

	}

	return false;
}

void Organizer::List::show(MessagePoll &msg_poll, bool append_msg) const {

	ostringstream out;
	out << "List number: " << this->list_num << "\n";
	
	switch (append_msg) {
		case false: { msg_poll.NewMessage(out.str()); break; }
		case true: { msg_poll.AppendMessage(out.str()); break; }
	}


	out.str(""); out.clear();
	
	char out_char[100];
	sprintf(out_char, "Date of creation: %s\n", date_of_creation.c_str());
	msg_poll.AppendMessage(out_char);

	msg_poll.AppendMessage("Items: ");
	int item_size = static_cast<int>(items.size());

	for (int i = 0; i < item_size - 1; i++) {
		string temp = items[i] + ", ";
		msg_poll.AppendMessage(temp);
	}

	msg_poll.AppendMessage(items[item_size - 1] + "\n\n");
		
}


bool Organizer::CompareList::operator() (List lhs, List rhs) const{ return lhs < rhs; }

void Organizer::DeleteList(int list_num) {

	multiset<List, CompareList>::iterator itr;
	for(itr = storing_lists.begin(); itr != storing_lists.end(); ++itr)
		if (itr->list_num == static_cast<int>(list_num)) { storing_lists.erase(itr); break; }

	msg_poll.NewMessage("Requested list has been deleted\n");

}

void Organizer::ShowAllLists() {
	
	if (storing_lists.size() == 0) { msg_poll.NewMessage("No list has been created\n"); return; }

	multiset<List, CompareList>::iterator itr;
	itr = storing_lists.begin();
	itr->show(msg_poll, false); ++itr; 
	if (itr != storing_lists.end()) itr->show(msg_poll, true);

}

bool compare_string(const char *str1, const char *str2) {

	for (int i = 0; i < (int)strlen(str2); i++)
		if (*(str1 + i) != '\n' &&  *(str1 + i) != *(str2 + i))	//ignore \n
			return false;

	return true;

}

/*checks if the user input is something like STORELIST[...]*/
static string CheckStoreListInput(string &user_input) {

	string cmd;	//if the user has typed STORELIST[ whatever there is here ] then save the command STORELIST in this string
	string items;	//the list of items should be here

	/*we are going to scan through teh user_input string and save all the characters in cmd string until we meet the first 
	square bracket '['*/
	string::iterator itr;
	bool first_bracket_encountered = false;

	for (itr = user_input.begin(); itr != user_input.end(); ++itr) {

		if (*itr == '[') { first_bracket_encountered = true; ++itr; break; }
		cmd += *itr;

	}
	
	//if the bracket has not been found then the user did not type STORELIST[items...], so just move on
	if (first_bracket_encountered == false) return string("_COMMAND_NOT_RECOGNIZED_");

	//now we need to check if the string before the square bracket matches to "STORELIST"
	//to make this check case insensitive, the strings should be uppercased
	string::iterator cmd_itr;
	for (cmd_itr = cmd.begin(); cmd_itr != cmd.end(); ++cmd_itr) 
		*cmd_itr = toupper(*cmd_itr);
	
	//now we check if cmd == "STORELIST"
	if(cmd != "STORELIST") return string("_COMMAND_NOT_RECOGNIZED_");

	/*so now we need to store all the items in one single string until we encounter the second 
	bracket*/

	bool second_bracket_encountered = false;

	/*obviously we dont tell the iterator to start from the beginning, but from it's last position*/
	for (; itr != user_input.end(); ++itr) {

		if (*itr == ']') { second_bracket_encountered = true; break; }
		items += *itr;

	}

	if (second_bracket_encountered == true) return items;	//']' has been found, so 

	return string("_COMMAND_NOT_RECOGNIZED_");

}


/*Once we make sure that the user has typed STORELIST[...items...] we parse the item string and put the individual items in a vector*/
static bool ParseItemsString(string &user_input, vector<string> &item_list) {

	string list_of_items = CheckStoreListInput(user_input);	//is user input STORELIST[...item...] ?
	if (list_of_items == "_COMMAND_NOT_RECOGNIZED_") return false;

	string itr_str;	//temporary string
	string::iterator itr;

	//parse the list_of_items string
	for (itr = list_of_items.begin(); itr != list_of_items.end(); ++itr) {

		/*Everytime we find a comma, we put the string we have copied so far in the vector and clear the itr_str string to
		copy the next item*/
		if (*itr != ',') { if (*itr != ' ' && *itr != '[' && *itr != ']') itr_str += *itr; }	//dont add commas and whitespaces
		else { item_list.push_back(itr_str); itr_str.clear(); }	//add the item and proceed to build the next string

	}

	/*the last item doesn't end with a comma, so the above if statement did not get executed, hence, the last item was not added
	to the vector. So we have to make sure toadd the last item as well*/
	item_list.push_back(itr_str);	

	return true;

}

bool Organizer::AddList(vector<string> &item_list) {

	/*first add the list*/
	int size = static_cast<int>(storing_lists.size());
	if (size >= 2) return false;
	storing_lists.insert(List(item_list));

	/*record the number of lists*/
	multiset<List, CompareList>::iterator itr;
	int i = 0;
	for (itr = storing_lists.begin(); itr != storing_lists.end(); ++i, ++itr)
		itr->list_num = (i + 1);

	return true;


}

void Organizer::CommandLine() {

	CLEAR_CONSOLE;

	while (1) {

		cout << " _________________ " << endl;
		cout << "|                 |" << endl;
		cout << "|    ORGANIZER    |" << endl;
		cout << "|_________________|" << endl;
		cout << "\n\n";
		
		msg_poll.DisplayMessages();

		cout << "cmd:";
		string command;
		cin >> command;
		cin.ignore();
		cout << endl;

		int list_num = -1;	//need this later
		vector<string> item_list;

		/*is user input STORELIST[ item1, item2... ] ? (check is case insensitive)*/
		if (ParseItemsString(command, item_list) == true) {

			msg_poll.Clear();
			if (AddList(item_list) == true) msg_poll.NewMessage("List added succesfully\n");
			else msg_poll.NewMessage("2 Lists already exist, type RETURNLISTS to see them\n");
			goto END;

		}

		MessagePoll::UpperCase(command);	//convert to upper case so that the input check is not case sensitive

		if (command == "RETURNLISTS") { msg_poll.Clear(); ShowAllLists(); goto END; }

		if (sscanf(command.c_str(), "DELETE_LIST_%d\n", &list_num) != EOF && list_num != -1) 
			DeleteList(list_num);
		
		END: {
			CLEAR_CONSOLE;
		}

	}


}


void Organizer::Reset() {

	storing_lists.clear();

}