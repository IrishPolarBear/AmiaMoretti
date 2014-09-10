// AmiaMoretti.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include "startup.h"

using namespace std;

class Display_Output {

	/*Purpose: Display output to the user*/

public: 
	Display_Output();
	string message;
	string display_Welcome(void);
};

string Display_Output::display_Welcome(){
	message = "Welcome to the Porn Optimizer!\nCurrently the only supported video types is AVI, MP3, and WMVs. Please stay tuned for more updates.\n";
	return message;
}
Display_Output::Display_Output(){
	message = "Not Yet Initialized!";
}

string Initialize(Start_Up access){
	string userIn, compare, strout, errorCheck = "None", *a;
	int usrIn;

	cout << "Yes or No:";
	cin >> userIn;
	compare = userIn.at(0);
	if (compare == "y" || compare == "Y") {
		cout << "Please Enter the Home Directory:";
		cin >> userIn;
		strout = userIn;
		errorCheck = access.initialize(userIn);
		if (errorCheck == "None"){
			cout << "Please enter how many additional categories you would like to add (Enter 0 for none. No more than 10!):";
			cin >> usrIn;
			while (usrIn > 10 || usrIn < 0){
				cout << "Please enter a valid number from 0 - 10:";
				cin >> usrIn;
			}
			a = new string[usrIn];
			if (usrIn > 0){
				for (int x = 0; x < usrIn; x++){
					cout << "Please enter category #" << x + 1 << ":";
					cin >> a[x];
				}
			}
			errorCheck = access.create_table("homeDir", a, usrIn);
			delete[] a;
			errorCheck = access.initial_scan("homeDir", strout);
		}
	}
	else {
		cout << "Closing Program!";
		errorCheck = "Closed!";
	}
	return errorCheck;
}

string DisplayOptions(int* value){
	int usrIn;

	cout << "Please select one of the following options:\n1) Add new directory\n2) Retrieve categories from a directory\n" \
		"3) Retrieve one row of data\n4) Retrieve all data from table\n5) Update category value for a video\n";
	cin >> usrIn;
	*value = usrIn + 2;
	return "None";
}

string AddNewDirectory(Start_Up access){
	string dirName, *a, errorCheck = "None", strout, dirLocation;
	int usrIn;

	cout << "Please enter the name of the new directory: ";
	cin >> dirName;
	cout << "Please enter the location of the directory:";
	cin >> dirLocation;
	strout = dirLocation;
	errorCheck = access.add_directory(dirName);
	if (errorCheck == "None") {
		cout << "Please enter how many additional categories you would like to add (Enter 0 for none. No more than 10!):";
		cin >> usrIn;
		while (usrIn > 10 || usrIn < 0) {
			cout << "Please enter a valid number from 0 - 10:";
			cin >> usrIn;
		}
		a = new string[usrIn];
		if (usrIn > 0) {
			for (int x = 0; x < usrIn; x++){
				cout << "Please enter category #" << x + 1 << ":";
				cin >> a[x];
			}
		}
		errorCheck = access.create_table(dirName, a, usrIn);
		delete[] a;
		errorCheck = access.initial_scan(dirName, strout);
	}
	return errorCheck;
}

string CreateArray(int arrayLength, string valueString, string returnArray[]){
	string compare, holder;
	int inc = 0;

	returnArray = new string[arrayLength + 1];
	cout << arrayLength << endl;
	cout << "xx"<< valueString << "XX";
	for (string::iterator it = valueString.begin(); it != valueString.end(); it++) {
		compare = *it;
		if (compare == "^"){
			cout << "Holder: " << holder << endl;
			returnArray[inc] = holder;
			inc = inc + 1;
			holder = "";
		}
		else {
			holder = holder + *it;
		}
	}
	returnArray[inc] = holder;
	return "None";
}
string GetCategories(Start_Up access, string* valueString){
	string outputString = "", outputString2 = "", errorCheck = "None";
	string *displayArray, *tableArray, compare, holder, categories;
	string transValue;
	int iterNum = 0, inc = 0, usrIn;

	errorCheck = access.get_directories(&outputString, &outputString2);
	for (int i = 0; i < outputString.length(); i++) {
		compare = outputString.at(i);
		if (compare == "^") {
			iterNum = iterNum + 1;
		}
	}
	displayArray = new string[iterNum + 1];
	tableArray = new string[iterNum + 1];
	for (string::iterator it = outputString.begin(); it != outputString.end(); it++){
		compare = *it;
		if (compare == "^"){
			displayArray[inc] = holder;
			inc = inc + 1;
			holder = "";
		}
		else {
			holder = holder + *it;
		}
	}
	displayArray[inc] = holder;
	inc = 0;
	holder = "";
	tableArray = new string[iterNum + 1];
	for (string::iterator it2 = outputString2.begin(); it2 != outputString2.end(); it2++){
		compare = *it2;
		if (compare == "^"){
			tableArray[inc] = holder;
			inc = inc + 1;
			holder = "";
		}
		else {
			holder = holder + *it2;
		}
	}
	tableArray[inc] = holder;
	cout << "Please select the table you want the categories from: " << endl;
	for (int j = 0; j < iterNum + 1; j++){
		cout << j + 1 << ": " << displayArray[j] << endl;
	}
	cin >> usrIn;
	transValue = tableArray[usrIn-1];

	errorCheck = access.get_categories(transValue, &categories);

	if (categories == "None"){
		cout << "File Name, File Type, Size, Date Created, Tags" << endl;
	}
	else{
		cout << "File Name, File Type, Size, Date Created, Tags," << categories << endl;
	}

	return "None";
}

string DisplayTable(Start_Up access){
	string tbleName, returnDir1, returnDir2;
	string errorCheck = "None";
	
	cout << "Please enter the table name:";
	cin >> tbleName;

	errorCheck = access.return_table(tbleName);

	return errorCheck;

}

string DisplayRow(Start_Up access){
	string tbleName, key;
	string errorCheck = "None", valueHolder;

	cout << "Please enter the table name:";
	cin >> tbleName;
	cout << "\nPlease enter the key:";
	cin >> key;

	errorCheck = access.return_row(tbleName, key);

	return errorCheck;

}

string UpdateCategory(Start_Up access){
	string tbleName, category, newValue, key;
	string errorCheck = "None";

	cout << "Please enter the table name:";
	cin >> tbleName;
	cout << "Please enter the key index: ";
	cin >> key;
	cout << "Please enter the category to update:";
	cin >> category;
	cout << "Please enter the new value:";
	cin >> newValue;
	errorCheck = access.update_value(tbleName, category, newValue, key);

	return errorCheck;

}
int main(int argc, char* argv[]){
	Display_Output dis;
	Start_Up init; 
	string userIn, errorCheck = "None", returnString = "";
	int wrkFlow = 0;

	string strout;
	cout << dis.display_Welcome();
	cout << init.get_output()<<endl;
	wrkFlow = init.proceduralCode;
	while (wrkFlow > -1) {
		switch (wrkFlow){
		case 1: {
					errorCheck = Initialize(init);
					if (errorCheck != "None") {
						cout << "Error - " + errorCheck;
					}
					else
						wrkFlow = 2;
					break;
		}
		case 2: {
					errorCheck = DisplayOptions(&wrkFlow);
					if (errorCheck != "None") {
						cout << "Error - " + errorCheck;
					}
					break;
		}
		case 3: {
					errorCheck = AddNewDirectory(init);
					if (errorCheck != "None") {
						cout << "Error - " + errorCheck;
					}
					else
						wrkFlow = 2;
					break;
		}
		case 4:	{
					returnString = "";
				   errorCheck = GetCategories(init, &returnString);
				   cout << returnString;
				   wrkFlow = 2;
				   break;
		}
		case 5:{
				   returnString = "";
				   errorCheck = DisplayRow(init);
				   wrkFlow = 2;
				   break;
		}
		case 6: {
					returnString = "";
					errorCheck = DisplayTable(init);
					wrkFlow = 2;
					break;
		}
		case 7: {
					errorCheck = UpdateCategory(init);
					wrkFlow = 2;
					break;
		}

		}
	}
	return 0;
}

