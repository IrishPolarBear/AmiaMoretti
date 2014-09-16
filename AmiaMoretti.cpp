// AmiaMoretti.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include "startup.h"

using namespace std;

class Display_Output {

	/*Purpose: Display output to the user
	now seems to be pointless class, may need to be eliminated or better utilized*/

public: 
	Display_Output();
	string message;
	string display_Welcome(void);
};

string Display_Output::display_Welcome(){
	//welcome message
	message = "Welcome to the Porn Optimizer!\nCurrently the only supported video types is AVI, MP3, and WMVs. Please stay tuned for more updates.\n";
	return message;
}
Display_Output::Display_Output(){
	message = "Not Yet Initialized!";
}

string Initialize(Start_Up access){
	//the initializer function. It gets all of the necessary user data to initialize the program, home dir, create home database, etc
	string userIn, compare, strout, errorCheck = "None", *a;
	int usrIn;

	cout << "Yes or No:";
	cin >> userIn;
	compare = userIn.at(0);
	if (compare == "y" || compare == "Y") {
		//starts off by getting the new home directory
		cout << "Please Enter the Home Directory:";
		cin >> userIn;
		strout = userIn;
		errorCheck = access.initialize(userIn);
		if (errorCheck == "None"){
			//creates custom categories for home directory
			cout << "Please enter how many custom categories you would like to add (Enter 0 for none. No more than 10!):";
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
			errorCheck = access.create_table("homeDir", a, usrIn); //function that creates the table in the sql database
			delete[] a;
			errorCheck = access.initial_scan("homeDir", strout); //function that does an auto-scan of the database, gets videos details
		}
	}
	else {
		cout << "Closing Program!";
		errorCheck = "Closed!";
	}
	return errorCheck;
}

string DisplayOptions(int* value){
	//All that it does is display the options
	int usrIn;

	cout << "Please select one of the following options:\n1) Add new directory\n2) Retrieve categories from a directory\n" \
		"3) Retrieve one row of data\n4) Retrieve all data from table\n5) Update category value for a video\n";
	cin >> usrIn;
	*value = usrIn + 2;
	return "None";
}

string AddNewDirectory(Start_Up access){
	//driver function to add a new category
	string dirName, *a, errorCheck = "None", strout, dirLocation;
	int usrIn;

	cout << "Please enter the name of the new directory: ";
	cin >> dirName;
	cout << "Please enter the location of the directory:";
	cin >> dirLocation;
	strout = dirLocation;
	errorCheck = access.add_directory(dirName); //calls the actual function to add the directory
	//loop to get all of the ten custom categories and then creates them
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
		errorCheck = access.create_table(dirName, a, usrIn); //creates the sql database
		delete[] a;
		errorCheck = access.initial_scan(dirName, strout); //starts the initial file scan
	}
	return errorCheck;
}

string CreateArray(int arrayLength, string valueString, string returnArray[]){
	//function that creates a new array
	string compare, holder;
	int inc = 0;

	returnArray = new string[arrayLength + 1];
	//cout << arrayLength << endl;
	//cout << "xx"<< valueString << "XX";
	for (string::iterator it = valueString.begin(); it != valueString.end(); it++) {
		compare = *it;
		if (compare == "^"){
		//	cout << "Holder: " << holder << endl;
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
	//Takes the return value of the categories for a table and then parses it out into two usable arrays, one with the display values and the other one with the corresponding keys
	//Display values make it look nice while the keys are what it really needs to access and retrieve data
	string outputString = "", outputString2 = "", errorCheck = "None";
	string *displayArray, *tableArray, compare, holder, categories;
	string transValue;
	int iterNum = 0, inc = 0, usrIn;

	errorCheck = access.get_directories(&outputString, &outputString2); //calls the sql database for category values
	//just a bunch of parsing
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
	//resets and repeats for the key values arrays
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
	//Gets the user data it needs to display a complete table
	string tbleName, returnDir1, returnDir2;
	string errorCheck = "None";
	
	cout << "Please enter the table name:";
	cin >> tbleName;

	errorCheck = access.return_table(tbleName);

	return errorCheck;

}

string DisplayRow(Start_Up access){
	//focus of the function is to get needed user data to retrieve a specific row
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
	//Primary focus of the function is to retrieve the needed user data  to update a value for the function: table, row index, category, value.
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

//This is the main function that runs the program
int main(int argc, char* argv[]){
	//Declaration of variables
	Display_Output dis;
	Start_Up init; 
	string userIn, errorCheck = "None", returnString = "";
	int wrkFlow = 0;
	string strout;

	cout << dis.display_Welcome(); //simply displays a welcome message
//	cout << init.get_output()<<endl;
	wrkFlow = init.proceduralCode; //checks at what state the program is in, has it already been initialized or not. 
	while (wrkFlow > -1) {
		switch (wrkFlow){
		case 1: {
					//calls function that will initialize the database, including asking the user for information
					errorCheck = Initialize(init);
					if (errorCheck != "None") {
						cout << "Error - " + errorCheck;
					}
					else
						wrkFlow = 2;
					break;
		}
		case 2: {
					//Displays options for the user
					errorCheck = DisplayOptions(&wrkFlow);
					if (errorCheck != "None") {
						cout << "Error - " + errorCheck;
					}
					break;
		}
		case 3: {
				//Calls function that adds a new directory of videos
					errorCheck = AddNewDirectory(init);
					if (errorCheck != "None") {
						cout << "Error - " + errorCheck;
					}
					else
						wrkFlow = 2;
					break;
		}
		case 4:	{
				//Calls function tha returns all categories for a table
					returnString = "";
				   errorCheck = GetCategories(init, &returnString);
				   cout << returnString;
				   wrkFlow = 2;
				   break;
		}
		case 5:{
				//Calls function that displays details for a row
				   returnString = "";
				   errorCheck = DisplayRow(init);
				   wrkFlow = 2;
				   break;
		}
		case 6: {
				//Calls function that displays an entire table
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

