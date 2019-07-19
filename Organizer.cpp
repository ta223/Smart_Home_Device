#include "Header.h"

using namespace std;

Organizer::List::List(string list_of_items) {

	/*get date in the correct format*/
	string time_format = Clock::CurrentTime(1);
	sscanf(time_format.c_str(), "%d/%d/%d/%d/%d/%d", &year, &month, &day, &hrs, &minutes, &seconds);

	/*Now we need to parse the list_of_items string*/
	string itr_str;
	string::iterator itr;
	
	for (itr = list_of_items.begin(); itr != list_of_items.end(); ++itr) {

		if (*itr != ',') if(*itr!=' ') itr_str += *itr;	//dont add whitespaces as well
		else { items.push_back(itr_str); itr_str.clear(); }	//add the item and proceed to build the next string

	}

	//add the last string, because the list_of_items string is not supposed to end with a comma, so we need to append the last string
	items.push_back(itr_str);

}

bool Organizer::List::operator > (const List &other) {

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

bool Organizer::CompareList::operator() (const List &lhs, List &rhs) {return rhs > lhs;}

void Organizer::AddList(std::string list_of_items) {

	/*first add the list*/
	int size = static_cast<int>(storing_lists.size());
	if(size < 2) storing_lists.insert(List( list_of_items));

	/*record the number of lists*/
	multiset<List, CompareList>::iterator itr;
	int i = 0;
	for (itr = storing_lists.begin(); i < size; ++i, ++itr) itr->list_num = static_cast<uint8_t>(i + 1);

}

void Organizer::DeleteList(enum list_number list_num) {

	multiset<List, CompareList>::iterator itr;
	for(itr = storing_lists.begin(); itr != storing_lists.end(); ++itr)
		if (itr->list_num == static_cast<int>(list_num)) { storing_lists.erase(itr); break; }

}

void Organizer::CommandLine() {


}


void Organizer::Reset() {

	storing_lists.clear();

}