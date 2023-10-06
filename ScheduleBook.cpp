#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <windows.h>
#include <ctime>

using namespace std;

void show();
void all();
void sort();

char event[100][100] = {};
char t_event[100][100] = {};
char range[100] = "";
int s_time = 0, e_time = 0, timecount = 0, eventcount = 0, daynum = 0;

class check // functions of checking format
{
public:
	//check format and range of date
	int chkdate(char date[100]) {
		int day = 0, month = 0, year = 0, i = 0;
		char d[3] = "", m[3] = "", y[5] = "";
		for (i = 0; i < 10; i++) {
			if (i < 2)
				d[i] = date[i];
			else if (i > 2 && i < 5)
				m[i - 3] = date[i];
			else if (i > 5 && i < 10)
				y[i - 6] = date[i];
		}
		if (d[1] == '-' || m[1] == '-' || date[2] != '-' || date[5] != '-')
			return 2;
		day = atoi(d);
		month = atoi(m);
		year = atoi(y);
		if (date[10] != '\0')
			return 2;
		else if (year >= 1000 && year <= 9999)
		{
			if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 0 && day <= 31) {
				daynum = 31;
				return 1;
			}
			else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 0 && day <= 30) {
				daynum = 30;
				return 1;
			}
			else if (month == 2) {
				if ((year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) && day > 0 && day <= 29) {
					daynum = 29;
					return 1;
				}
				else if (day > 0 && day <= 28) {
					daynum = 28;
					return 1;
				}
				else
					return 0;
			}
			else
				return 0;
		}
		else
			return 0;
	}
	//check format and range of time
	int chktime(char time[100]) {
		int hour = 0, minute = 0, i = 0;
		char h[3] = "", m[3] = "";
		for (i = 0; i < 5; i++) {
			if (i < 2)
				h[i] = time[i];
			else if (i > 2 && i <= 4)
				m[i - 3] = time[i];
		}
		if (h[1] == '-' || m[1] < 48 || m[1] > 57 || time[2] != ':')
			return 2;
		hour = atoi(h);
		minute = atoi(m);
		if (hour < 24 && hour >= 0 && minute < 60 && minute >= 0) {
			if (timecount == 0)
				s_time = hour * 100 + minute;
			else if (timecount == 1)
				e_time = hour * 100 + minute;
			return 1;
		}
		else
			return 0;
	}
	//check time conflict or not
	int chkconflict(char temp[100])
	{
		int newshour = 0, newsmin = 0, newehour = 0, newemin = 0, oldshour = 0, oldsmin = 0, oldehour = 0, oldemin = 0, newstime = 0, newetime, oldstime = 0, oldetime = 0, i = 0, j = 0, c = 1;
		char newsh[3] = "", newsm[3] = "", neweh[3] = "", newem[3] = "", oldsh[3] = "", oldsm[3] = "", oldeh[3] = "", oldem[3] = "";
		for (i = 0; i < 100; i++) {
			for (j = 0; j < 100; j++) {
				if (j > 10 && j < 13) {
					newsh[j - 11] = temp[j];
					oldsh[j - 11] = event[i][j];
				}
				else if (j > 13 && j < 16) {
					newsm[j - 14] = temp[j];
					oldsm[j - 14] = event[i][j];
				}
				else if (j > 16 && j < 19) {
					neweh[j - 17] = temp[j];
					oldeh[j - 17] = event[i][j];
				}
				else if (j > 19 && j < 22) {
					newem[j - 20] = temp[j];
					oldem[j - 20] = event[i][j];
				}
			}
			newshour = atoi(newsh);
			newsmin = atoi(newsm);
			newehour = atoi(neweh);
			newemin = atoi(newem);
			oldshour = atoi(oldsh);
			oldsmin = atoi(oldsm);
			oldehour = atoi(oldeh);
			oldemin = atoi(oldem);
			newstime = newshour * 100 + newsmin;
			newetime = newehour * 100 + newemin;
			oldstime = oldshour * 100 + oldsmin;
			oldetime = oldehour * 100 + oldemin;
			if (strncmp(temp, event[i], 10) == 0) {
				if ((newstime < oldstime && newetime <= oldstime) || (newstime >= oldetime && newetime > oldetime))
					c = 1;
				else
					return 0;
			}
		}
	}
};

class printing //printing with coordinate
{
public:
	void gotoxy(int xpos, int ypos)
	{
		COORD scrn;
		HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
		scrn.X = xpos; scrn.Y = ypos;
		SetConsoleCursorPosition(hOuput, scrn);
	}
	int wherex()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}
	int wherey()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
};

//changing text color
void apicolor(int color)
{//Windows API(Copied)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//the output when inputing invalid data
void invalid()
{
	apicolor(12);
	cout << "Invalid value! Please try again." << endl;
	apicolor(7);
}
//swap the events 
void swap(char array[100][100], int i) {
	char temp[100] = "";
	strcpy_s(temp, array[i]);
	strcpy_s(array[i], array[i + 1]);
	strcpy_s(array[i + 1], temp);
}
//bubble sorting the event to ascending
void sort() {
	int i = 0, j = 0;
	char temp[6] = "";
	char temp1[6] = "";
	for (i = 0; i < 100; i++) {
		for (j = 0; j < strlen(event[i]); j++) {
			if (j < 4)
				t_event[i][j] = event[i][6 + j];
			else if (j == 4)
				t_event[i][j] = event[i][5];
			else if (j > 4 && j < 7)
				t_event[i][j] = event[i][j - 2];
			else if (j == 7)
				t_event[i][j] = event[i][2];
			else if (j > 7 && j < 10)
				t_event[i][j] = event[i][j - 8];
			else if (j >= 10)
				t_event[i][j] = event[i][j];
		}
	}
	do {
		for (i = 0; i < eventcount - 1; i++) {
			if (strncmp(t_event[i], t_event[i + 1], 16) > 0) {
				swap(t_event, i);
				swap(event, i);
			}
		}
		j++;
	} while (j != eventcount);
}

void add()//add event function 
{
	check c;
	int count = 0, type = 0, erd = 0, erst = 0, eret = 0, stet = 0, con = 0, ert = 0;
	char temp[100] = {};
	char temp1[100] = {};
	char input[100] = {};
	char d[100] = "", st[100] = "", et[100] = "", e[100] = "", t[100] = "";
	timecount = 0;
	system("cls");
	cout << "Add one event : \n" << endl;
	do {
		erd = 0;
		erst = 0;
		eret = 0;
		stet = 0;
		con = 0;
		ert = 0;
		memset(input, '\0', sizeof(input));
		memset(temp, '\0', sizeof(temp));
		memset(temp1, '\0', sizeof(temp1));
		memset(d, '\0', sizeof(d));
		memset(st, '\0', sizeof(st));
		memset(et, '\0', sizeof(et));
		memset(e, '\0', sizeof(e));
		memset(t, '\0', sizeof(t));
		cout << "Please Input The Event \n";
		cout << "(DD-MM-YYYY/HH:MM/HH:MM/EVENT TITLE/TYPE(1: Home 2: Work 3: Study 4: Play))  \n";
		cout << "E.g. 24-03-2020/15:30/16:30/SEHH2042 Lesson/3: \n";

		if (count == 0)
			cin.ignore();
		cin.getline(d, sizeof(d), '/');
		cin.getline(st, sizeof(st), '/');
		cin.getline(et, sizeof(et), '/');
		cin.getline(e, sizeof(e), '/');
		cin.getline(t, sizeof(t), '\n');
		strcat_s(temp1, e);
		strcat_s(temp1, "/");
		memset(input, '\0', sizeof(input));

		if (c.chkdate(d) == 1) {
			strcat_s(temp, d);
			strcat_s(temp, "/");
		}
		else
			erd = 1;
		memset(input, '\0', sizeof(input));
		if (c.chktime(st) == 1) {
			strcat_s(temp, st);
			strcat_s(temp, "/");
		}
		else
			erst = 1;
		memset(input, '\0', sizeof(input));
		timecount++;
		if (c.chktime(et) == 1 && e_time >= s_time) {
			strcat_s(temp, et);
			strcat_s(temp, "/");
		}
		else
		{
			eret = 1;
			if (e_time <= s_time)
				stet = 1;
		}
		strcat_s(temp, temp1);
		memset(input, '\0', sizeof(input));
		if (c.chkconflict(temp) == 0)
			con = 1;
		type = atoi(t);
		if (type >= 1 && type < 5)
			strcat_s(temp, t);
		else
			ert = 1;
		if (erd != 0 || erst != 0 || eret != 0 || stet != 0 || con != 0 || ert != 0) {
			if (erd != 0)
				cout << "Invalid date!" << endl;
			if (erst != 0)
				cout << "Invalid starting time!" << endl;
			if (eret != 0)
				cout << "Invalid ending time!" << endl;
			if (stet != 0 && erst == 0 && eret == 0)
				cout << "Starting time is larger Ending time!" << endl;
			if (con != 0 && erst == 0 && eret == 0)
				cout << "There is time conflict!" << endl;
			if (ert != 0)
				cout << "Invalid type!" << endl;
			count++;
			invalid();
			cout << "Remains " << 3 - count << endl;
			if (count == 3) {
				system("pause");
				return;
			}
		}
	} while (erd != 0 || erst != 0 || eret != 0 || stet != 0 || con != 0 || ert != 0);
	//delect event function  
	strcpy_s(event[eventcount], temp);
	cout << "\n\nAdded: " << event[eventcount];
	eventcount++;
	sort();
	cout << "\n\n";
	system("pause");
}

void del()//delet one event 
{
	check c;
	char input[100];
	char temp[100][100] = {};
	int count = 0, j = 0, record = 0, choice = 0, check = 0;
	do {
		cout << "Please Input Event Date (DD-MM-YYYY): ";
		cin >> input;
		if (c.chkdate(input) == 1) {
			break;
		}
		else if (c.chkdate(input) == 2) {
			invalid();
			apicolor(12);
			cout << "Wrong format!" << endl;
			apicolor(7);
			count++;
			cout << "Remains " << 3 - count << endl;
			if (count == 3) {
				system("pause");
				return;
			}
		}
		else {
			invalid();
			count++;
			cout << "Remains " << 3 - count << endl;
			if (count == 3) {
				system("pause");
				return;
			}
		}
	} while (c.chkdate(input) == 0 && count < 4 || c.chkdate(input) == 2);
	for (int i = 0; i <= eventcount - 1; i++) {
		if (strncmp(input, event[i], 10) != 0)
			check++;
	}
	if (check == eventcount) {
		cout << "\n\n";
		invalid();
		cout << "No event on ";
		apicolor(12);
		cout << input;
		apicolor(7);
		cout << "." << endl;
		system("pause");
		return;
	}

	cout << left << setw(7) << "No." << setw(25) << "Date and Time" << setw(45) << "Title" << setw(5) << "Type\n";
	for (int i = 0; i <= eventcount - 1; i++) {
		if (strncmp(input, event[i], 10) == 0) {
			record++;
			setfill(' ');
			cout << right << setw(3) << record << ".   ";
			strcpy_s(temp[record], event[i]);
			for (j = 0; j < 10; j++) {
				cout << event[i][j];
			}
			cout << ',';
			for (j = 11; j < 16; j++) {
				cout << event[i][j];
			}
			cout << '-';
			for (j = 17; j < 22; j++) {
				cout << event[i][j];
			}
			cout << "   ";
			for (j = 23; j < strlen(event[i]) - 2; j++) {
				if (j == strlen(event[i]) - 3)
					cout << left << setw(45 - (strlen(event[i]) - 23)) << event[i][j];
				else
					cout << event[i][j];
			}
			cout << "   ";
			for (j = strlen(event[i]) - 1; j < strlen(event[i]); j++) {
				cout << event[i][j];
			}
			cout << endl;
		}
	}

	cout << "Please input the number to delete : ";
	cin >> choice;
	if (choice <1 || choice >record) {
		invalid();
		apicolor(12);
		cout << "No such number can be delete.\n" << endl;
		apicolor(7);
		cout << " Press Enter back to menu.";
		system("pause");
		return;
	}
	else {
		for (int i = 0; i <= eventcount - 1; i++) {
			if (strcmp(temp[choice], event[i]) == 0) {
				memset(event[i], '~', 16);
				sort();
				eventcount--;
				memset(event[eventcount], '\0', sizeof(event[i]));
				cout << temp[choice] << " removed.";
				cout << " Press Enter back to menu.";
				system("pause");
				return;
			}
		}
	}
}

void addevent()//add events in batch 
{
	check c;
	string input[100];
	int a = 0, b = 0, error = 0, count = 0;
	char type[100] = {};
	do {
		getline(cin, input[count]);
		count++;
	} while (input[count - 1] != "0");
	for (a = 0; a <= count - 2; a++)
	{
		input[a].copy(type, 23, 0);
		if (c.chkconflict(type) == 0)
			error++;
		else
		{
			input[a].copy(event[eventcount], input[a].length(), 0);
			eventcount++;
			sort();
			b++;
		}
	}
	system("cls");
	cout << "Total " << count - 2 << " events in batch" << endl;
	cout << b << " event(s) addded" << endl;
	cout << error - 1 << " event(s) not addded due to time conflict" << endl;
	system("pause");
}

void all() //show all events
{
	int i = 0, j = 0;
	cout << left << setw(25) << "Date and Time" << setw(45) << "Title" << setw(5) << "Type\n";
	if (event[0][0] == '\0') {
		apicolor(12);
		cout << "No Value" << endl;
		apicolor(7);
		cout << "\n\n" << endl;
		system("pause");
		system("cls");
		return;
	}
	for (i = 0; i < eventcount; i++) {
		for (j = 0; j < 10; j++) {
			cout << event[i][j];
		}
		cout << ',';
		for (j = 11; j < 16; j++) {
			cout << event[i][j];
		}
		cout << '-';
		for (j = 17; j < 22; j++) {
			cout << event[i][j];
		}
		cout << "   ";
		for (j = 23; j < strlen(event[i]) - 2; j++) {
			if (j == strlen(event[i]) - 3)
				cout << left << setw(45 - (strlen(event[i]) - 23)) << event[i][j];
			else
				cout << event[i][j];
		}
		cout << "   ";
		for (j = strlen(event[i]) - 1; j < strlen(event[i]); j++) {
			if (event[i][j] == '1')
				cout << "Home";
			else if (event[i][j] == '2')
				cout << "Work";
			else if (event[i][j] == '3')
				cout << "Study";
			else if (event[i][j] == '4')
				cout << "Play";
		}
		cout << endl;
	}
}
//check the range of the latest day 
void chkrange(int n)
{
	int day = 0, month = 0, year = 0, i = 0, j = 0;
	string y1, m1, d1;
	time_t tt;
	time(&tt);
	tm TM = *localtime(&tt);
	year = TM.tm_year + 1900;
	month = TM.tm_mon;
	day = TM.tm_mday;
	month += 1;
	if (year >= 1000 && year <= 9999)
	{
		if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 0 && day <= 31) {
			if ((day + n) > 31) {
				day = day + n - 31;
				month = month + 1;
			}
			else
				day = day + n;
		}
		else if (month == 4 || month == 6 || month == 9 || month == 11 && day > 0 && day <= 30) {
			if ((day + n) > 30) {
				day = day + n - 30;
				month = month + 1;
			}
			else
				day = day + n;
		}
		else if (month == 2) {
			if ((year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) && day > 0 && day <= 29) {
				if ((day + n) > 29) {
					day = day + n - 29;
					month = month + 1;
				}
				else
					day = day + n;
			}
		}
		else if (day > 0 && day <= 28) {
			if ((day + n) > 28) {
				day = day + n - 28;
				month = month + 1;
			}
			else
				day = day + n;
		}
	}
	if (month > 12)
		year = year + 1;
	y1 = to_string(year);
	m1 = to_string(month);
	d1 = to_string(day);
	strcat_s(range, y1.c_str());
	strcat_s(range, "-");
	if (month < 10)
		strcat_s(range, "0");
	strcat_s(range, m1.c_str());
	strcat_s(range, "-");
	if (day < 10)
		strcat_s(range, "0");
	strcat_s(range, d1.c_str());
}

void list()//show the data by list format
{
	int  n, i, j;
	char event_type[2] = {}, current[11] = {};
	string y1, m1, d1;
	int year, month, day;
	time_t tt;
	time(&tt);
	tm TM = *localtime(&tt);
	year = TM.tm_year + 1900;
	month = TM.tm_mon;
	day = TM.tm_mday;
	month += 1;
	y1 = to_string(year);
	m1 = to_string(month);
	d1 = to_string(day);
	strcat_s(current, y1.c_str());
	strcat_s(current, "-");
	if (month < 10)
		strcat_s(current, "0");
	strcat_s(current, m1.c_str());
	strcat_s(current, "-");
	if (day < 10)
		strcat_s(current, "0");
	strcat_s(current, d1.c_str());
	for (i = 0; i < 100; i++) {
		for (j = 0; j < strlen(event[i]); j++) {
			if (j < 4)
				t_event[i][j] = event[i][6 + j];
			else if (j == 4)
				t_event[i][j] = event[i][5];
			else if (j > 4 && j < 7)
				t_event[i][j] = event[i][j - 2];
			else if (j == 7)
				t_event[i][j] = event[i][2];
			else if (j > 7 && j < 10)
				t_event[i][j] = event[i][j - 8];
			else if (j >= 10)
				t_event[i][j] = event[i][j];
		}
	}
	cout << left << setw(40) << "Please Input Event Type " << endl;
	cout << setw(40) << "(1)Home (2)Work (3)Study (4)Play : ";
	cin >> event_type;
	cout << setw(40) << "Please Input coming days : ";
	cin >> n;
	chkrange(n-1);
	cout << left << setw(25) << "Date and Time" << setw(45) << "Title" << setw(5) << "Type\n";
	if ((event_type[0] >= 49 && event_type[0] <= 52) && (n >= 1 && n <= 7)) {
		for (int i = 0; i <= eventcount - 1; i++) {  
			if ((event_type[0] == t_event[i][strlen(t_event[i]) - 1]) && (((strncmp(range, t_event[i], 10) >= 0) && (strncmp(current, t_event[i], 10) < 0)) || (strncmp(current, t_event[i], 10) == 0))) {

				for (j = 0; j < 10; j++) {
					cout << event[i][j];
				}
				cout << ',';
				for (j = 11; j < 16; j++) {
					cout << event[i][j];
				}
				cout << '-';
				for (j = 17; j < 22; j++) {
					cout << event[i][j];
				}
				cout << "   ";
				for (j = 23; j < strlen(event[i]) - 2; j++) {
					if (j == strlen(event[i]) - 3)
						cout << left << setw(45 - (strlen(event[i]) - 23)) << event[i][j];
					else
						cout << event[i][j];
				}
				cout << "   ";
				for (j = strlen(event[i]) - 1; j < strlen(event[i]); j++) {
					if (event[i][j] == '1')
						cout << "Home";
					else if (event[i][j] == '2')
						cout << "Work";
					else if (event[i][j] == '3')
						cout << "Study";
					else if (event[i][j] == '4')
						cout << "Play";
				}
				cout << endl;
			}
		}
		system("pause");
		system("cls");
	}
	else {
		invalid();
		apicolor(12);
		cout << "Wrong format!" << endl;
		apicolor(7);
		system("pause");
	}
}
// find week
int week(char date[100]) {
	int day = 0, month = 0, year = 0, i = 0, w = 0, c = 0;
	char d[3] = "", m[3] = "", y[5] = "";
	for (i = 0; i < 10; i++) {
		if (i < 2)
			d[i] = date[i];
		else if (i > 2 && i < 5)
			m[i - 3] = date[i];
		else if (i > 5 && i < 10)
			y[i - 6] = date[i];
	}
	day = atoi(d);
	month = atoi(m);
	year = atoi(y);
	if (month == 1) {
		month = 13;
		year--;
	}
	else if (month == 2) {
		month = 14;
		year--;
	}
	c = year / 100;
	year = year - c * 100;
	w = (year + year / 4 + c / 4 - 2 * c + (26 * (month + 1)) / 10 + day - 1) % 7;
	return w;
}
//calendar
void calendar()
{
	check c;
	printing p;
	char input[100] = "01-", temp[10], t_input[100] = "";
	int day = 0, month = 0, year = 0, i = 0, j = 0, w = 0, cx = 0, cy = 0, bx = 0, by = 0, count = 0;
	char d[3] = "", m[3] = "", y[5] = "";
	string da("");
	cout << "Please Input Month and Year (MM-YYYY)  :";
	cin.ignore();
	cin.getline(temp, '\n');
	strcat_s(input, temp);
	if (c.chkdate(input) == 1) {
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
		cout << left << setw(32) << "Sun|";
		cout << setw(32) << "Mon|";
		cout << setw(32) << "Tue|";
		cout << setw(32) << "Wed|";
		cout << setw(32) << "Thu|";
		cout << setw(32) << "Fri|";
		cout << setw(32) << "Sat|" << endl;
		for (j = 0; j < daynum; j++) {
			count = 0;
			w = week(input);
			if (w == 0) {
				cout << "\n\n\n\n\n\n\n";
			}
			for (i = 0; i < 10; i++) {
				if (i < 2)
					d[i] = input[i];
				else if (i > 2 && i < 5)
					m[i - 3] = input[i];
				else if (i > 5 && i < 10)
					y[i - 6] = input[i];
			}
			day = atoi(d);
			if (j == 0) {
				for (i = 0; i < w; i++) {
					cout << setw(32) << "";
				}
			}
			if (w == 0) {
				apicolor(12);
				cout << left << setw(32) << day;
				apicolor(7);
			}
			else
				cout << left << setw(32) << day;
			cx = p.wherex();
			cy = p.wherey();
			for (i = 0; i < eventcount; i++) {
				if (strncmp(event[i], input, 10) == 0 && count < 6) {
					if (count == 0) {
						apicolor(10);
					}
					else
						apicolor(10 + count);
					p.gotoxy(cx - 32, cy + count + 1);
					for (int k = 11; k < 16; k++) {
						cout << event[i][k];
					}
					cout << "-";
					for (int k = 17; k < 22; k++) {
						cout << event[i][k];
					}
					cout << " ";
					if (strlen(event[i]) - 27 >= 19) {
						for (int k = 23; k < 40; k++) {
							cout << event[i][k];
						}
						cout << "...";
					}
					else {
						for (int k = 23; k <= strlen(event[i]) - 3; k++) {
							cout << event[i][k];
						}
					}
					count++;
					apicolor(7);
				}
			}
			if (count != 0)
				p.gotoxy(cx, cy);
			day++;
			if (day < 10)
				strcat_s(t_input, "0");
			da = to_string(day);;
			strcat_s(t_input, da.c_str());
			strcat_s(t_input, "-");
			strcat_s(t_input, m);
			strcat_s(t_input, "-");
			strcat_s(t_input, y);
			strcpy_s(input, t_input);
			strcpy_s(t_input, "");
		}
	}
	else {
		invalid();
		apicolor(12);
		cout << "Wrong format!" << endl;
		apicolor(7);
		system("pause");
	}
	cout << "\n\n\n\n\n\n\n";
	system("pause");
	system("cls");
}
//the event menu
void show()
{
	system("cls");
	char option;
	do {
		cout << "\n\n";
		cout << "***** Show Event Menu *****" << endl;
		cout << "[1] Show all events" << endl;
		cout << "[2] Show events (list view)" << endl;
		cout << "[3] Show events (calendar view)" << endl;
		cout << "[4] Return to Main Menu" << endl;
		cout << "***************************" << endl;
		cout << "Option (1 - 4) :";
		cin >> option;
		switch (option) {
		case '1': all(); break;
		case '2': list(); break;
		case '3': calendar(); break;
		case '4': break;
		}
	} while (option != '4');
}
//show the information of groupmate 
void credit()
{
	system("cls");
	cout << "\n\n";
	cout << "Credits: \n" << endl;
	cout << left << setw(18) << "Name" << setw(12) << "Student ID" << setw(5) << "Class" << endl;
	
	cout << "\n\n";
	system("pause");
}
//exit the program
void exit()
{
	char choice;
	do {
		cout << "Exit (Y / N) : ";
		cin >> choice;
		if (choice == 'Y' || choice == 'y') {
			cout << "Program terminates. Good bye!" << endl;
			exit(0);
		}
		else if (choice == 'N' || choice == 'n')
			system("cls");
		else {
			system("cls");
			apicolor(12);
			cout << "Invalid value! Please try again." << endl;
			apicolor(7);
		}

	} while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');
}
//main menu
int main()
{
	time_t result = time(NULL);
	char str[26];
	char prog_choice;
	for (;;)
	{
		system("cls");
		cout << "Today's date and time : ";
		ctime_s(str, sizeof str, &result);
		cout << str;
		cout << "\n\n";
		cout << "=====Welcome=====\n" << endl;
		cout << "*** Main Menu ***" << endl;
		cout << "[1] Add one event" << endl;
		cout << "[2] Delete one event" << endl;
		cout << "[3] Add events in batch" << endl;
		cout << "[4] Show events" << endl;
		cout << "[5] Credits" << endl;
		cout << "[6] Exit" << endl;
		cout << "*****************" << endl;
		cout << "Option (1 - 6) :";
		cin >> prog_choice;

		switch (prog_choice) {
		case '1': add(); break;
		case '2': del(); break;
		case '3': addevent(); break;
		case '4': show(); break;
		case '5': credit(); break;
		case '6': exit(); break;

		}
	}
	return 0;
}