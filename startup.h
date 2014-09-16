#define __STARTUP_H__
#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <Windows.h>
#include <ctime>

#include <tchar.h>
#include <strsafe.h>
#include <tchar.h>
#pragma comment(lib,"User32.lib")


using namespace std;
using namespace System;
using namespace System::IO;


class Start_Up {

	/*Started off just to initalize the program but morphed into the class that does the sql interfacing*/
	//class public variables
	string initialized;
	sqlite3 *sqldb;
	int rc;

public:
	Start_Up(); //initializer function
	int get_rc(); //returns an error catch
	string get_output(); 
	string output, errorMessage;
	int proceduralCode;
	string initialize(string homedir); //creates the home directory
	int display_table(string table); //calls sql function to display the entire table (most likely no in use)
	string create_table(string table, string categories[], int num); //calls the sql function to create a table
	string initial_scan(string table, string dir); //function that scans the directory and populates the database
	string add_directory(string dirName); //adds a new category to the database
	string get_directories(string* returnValue, string* returnValue2); //returns a list of all of the sql databaes
	string get_categories(string tableName, string* returnValue); //returns the categories in the sql tble
	string return_row(string tableName, string key); //returns a row value, so all data for single video
	string return_table(string tableName); //returns the whole table values
	string update_value(string tableName, string category, string newValue, string key); //updates specific value in a certain table
private:
	static int callback(void *NotUsed, int argc, char **argb, char **azColName); //runs sql commands, primary one
	string run_sql(string expression); //runs sql commands, single command
	string findFileType(string original); //gets the file type  of a video
	string findFileName(string original); //ges the file name of a video
	string getFileDate(HANDLE hObj, LPTSTR fileDate, DWORD dwEror); //gets the data added to the hard drive
	string getFileSize(WIN32_FIND_DATA file); //gets the file size in kbs
	string filePathConvert(string original); //converts the windows path to programming path so it can be used
	string get_singleValue(string expression, string* returnValue); //returns a single value
	string run_multipleSql(string expression); //used to call multiple sql lines
	string separate_Values(string masterString, string pattern, string* leftValue, string* rightValue); //separates values, method used to store multiple data points
	string get_timeDate(); //gets current time and date, or moves it
	string get_rowValue(string expression, string *rowValue); //returns a database row
};

int Start_Up::get_rc(){
	//returns the rc function, needs to be eliminated
	return rc;
}

string Start_Up::get_output(void){
	//returns error code for latest sql error. Very useful for debugging
	errorMessage = sqlite3_errmsg(sqldb);
	return output;
}

int Start_Up::callback(void *NotUsed, int argc, char **argv, char **azColName){
	//sql call back function
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int Start_Up::display_table(string table){
	//pretty sure that this can be removed
	char *sqlcommand, *zErrMsg = 0, *convert;
	sqlite3_stmt *statement;
	string expression, closeExpression = "')'",data = "teststring";
	int wrk = 1;

	expression = "SELECT * FROM " + table;
	convert = new char[expression.length() + 1];
	strcpy_s(convert, 32, expression.c_str());
	sqlcommand = convert;
	delete[] convert;
	if (sqlite3_prepare(sqldb, sqlcommand, -1, &statement, 0) == SQLITE_OK){
		int ctotal = sqlite3_column_count(statement);
		int res = 0;
		res = sqlite3_step(statement);
		if (res == SQLITE_ROW){
			string s = (char*)sqlite3_column_text(statement, 0);
			data = s + "\n";
		}
		data = res;
		/*while (wrk = 1){
		res = sqlite3_step(statement);
		if (res == SQLITE_ROW){
		for (int i = 0; i < ctotal; i++){
		string s = (char*)sqlite3_column_text(statement, i);
		data = data + s + "\n";
		}
		}
		if (res == SQLITE_DONE || res == SQLITE_ERROR){
		data = data + "DONE/ERROR";
		wrk = 0;
		}
		}*/
		return res;
	}
	else
		return sqlite3_prepare(sqldb, sqlcommand, -1, &statement, 0);
}


string Start_Up::getFileDate(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
	//function that gets the last modified windows date of the file
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	// Build a string showing the date and time.
	dwRet = StringCchPrintf(lpszString, dwSize,
		TEXT("%02d/%02d/%d  %02d:%02d"),
		stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute);

	if (S_OK == dwRet)
		return "WORKED";
	else return "NOPE!";
}

string Start_Up::getFileSize(WIN32_FIND_DATA data){
	//function gets the size of the video
	LARGE_INTEGER size;
	
	size.LowPart = data.nFileSizeLow;
	size.HighPart = data.nFileSizeHigh;

	return to_string(size.QuadPart);
}

string Start_Up::get_timeDate(){
	//basically just gets the current date/time and puts it in a nice format
	string timeString, holder, timeArray[5], compare, returnString, month, holdValue;
	string monthCompare[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	string monthNum[12] = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12" };
	time_t rawtime;
	int inc = 0;

	time(&rawtime);
	timeString = ctime(&rawtime);
	for (string::iterator it = timeString.begin(); it != timeString.end(); it++){
		compare = *it;
		if (compare == " ") {
			timeArray[inc] = holder;
			inc = inc + 1;
			holder = "";
		}
		else
			holder = holder + compare;
	}
	timeArray[inc] = holder;
	compare = timeArray[1];
	for (int i = 0; i < 12; i++){
		if (compare == monthCompare[i]){
			month = monthNum[i];
		}
	}
	inc = 0;
	holder = "";
	for (string::iterator ch = timeArray[4].begin(); ch != timeArray[4].end(); ch++){
		if (inc <= 3){
			holder = holder + *ch;
			inc = inc + 1;
		}
	}
	returnString = month + "/" + timeArray[2] + "/" + holder + " " + timeArray[3];
	return returnString;
}

string Start_Up::separate_Values(string masterString, string pattern, string* leftValue, string* rightValue){
	//function that sepearates database row information, these rows have multiple vaues that are separated by a key and this parses it all
	string errorCheck = "None", compare = "";
	char convert;
	int key = 0, lenString;

	*leftValue = "";
	*rightValue = "";
	lenString = masterString.length();
	for (int i = 0; i < (lenString - 2); i++){
		compare = "";
		for (int k = i; k < (i + 3); k++){
			convert = masterString.at(k);
			compare = compare + convert;
		}
		if (pattern == compare){
			key = i;
			break;
		}
	}

	if (key > 0){
		for (int j = 0; j < masterString.length(); j++){
			if (j < key){
				*leftValue = *leftValue + masterString.at(j);
			}
			if (j >(key + 2)) {
				*rightValue = *rightValue + masterString.at(j);
			}
		}
		return "None";
	}
	else
		return "NULL";
}

string Start_Up::get_directories(string* returnValue, string* returnValue2){
	//function that gets all of the sql different directories
	string expression, errorCheck = "None",dirString,dirTable,directories,leftString="";
	string getValue = "None", formatString, endString = "", transString, tables;
	int inSomething;

	expression = "SELECT options FROM options WHERE id = 'extraDir';";
	errorCheck = get_singleValue(expression, &getValue); //runs sql command to get stored values
	directories = "homedir";  //initalizing shit
	tables = "homedir";
	if (errorCheck == "None"){
		errorCheck = separate_Values(getValue, "#$#", &leftString, &endString);
		//while loop that runs to parse the string into two different strings, one is a key the other is display
		while ((errorCheck == "None") || (endString != "")){
			errorCheck = separate_Values(leftString, "#-#", &dirTable, &dirString);
			directories = directories + "^" + dirString;
			tables = tables + "^" + dirTable;
			transString = endString;
			errorCheck = separate_Values(transString, "#$#", &leftString, &endString);
		}
		if (errorCheck == "None"){
			dirTable = "";
			dirString = "";
			errorCheck = separate_Values(leftString, "#-#", &dirTable, &dirString);
			directories = directories + "^" + dirString;
			tables = tables + "^" + dirTable;
		}
	}
	//returns the two strings into usable strings
	*returnValue = directories;
	*returnValue2 = tables;
	return errorCheck;
}

string Start_Up::get_categories(string tableName, string* returnValue){
	//gets the categories from the sql database, list of categories stored in a row
	string errorCheck = "None", expression, stringValue = "None";
	string leftString = "", endString = "", holderString = "", transString;

	expression = "SELECT tags FROM " + tableName + " WHERE type='categories';";
	errorCheck = get_singleValue(expression, &stringValue);
	if ((errorCheck == "None") && (stringValue != "None")){
		errorCheck = separate_Values(stringValue, "#-#", &leftString, &endString);
		while ((errorCheck == "None") || (endString != "")) {
			holderString = holderString + leftString + ", ";
			transString = endString;
			errorCheck = separate_Values(transString, "#-#", &leftString, &endString);
		}
		holderString = holderString + leftString;
		*returnValue = holderString;
	}
	else
		*returnValue = stringValue;

	return errorCheck;
}

string Start_Up::add_directory(string dirName){
	//adds new directory to the sql database
	string expression, errorCheck = "None", strInformation;
	string returnValue = "None";

	expression = "SELECT options FROM options WHERE id='extraDir'"; //adds the new directory to the options table, so program can use it

	errorCheck = get_singleValue(expression, &returnValue);
	//either updates or creates a new row for a new directory
	if ((returnValue != "None")){
		strInformation = returnValue + dirName + "#-#" + dirName + "#$#";
		expression = "UPDATE options SET options='" + strInformation + "' WHERE id='extraDir';";
		errorCheck = run_sql(expression);
	}
	else
	{
		errorCheck = "None";
		strInformation = dirName + "#-#" + dirName + "#$#";
		expression = "INSERT INTO options(id,options) VALUES ('extraDir','" + strInformation + "');";
		errorCheck = run_sql(expression);

	}
	return errorCheck;
}

string Start_Up::return_row(string tableName, string key){
	//function that returns a row
	string expression, errorCheck = "None";

	expression = "SELECT * FROM " + tableName + " WHERE ID =" + key;
	errorCheck = run_multipleSql(expression);
	return errorCheck;

}

string Start_Up::update_value(string tableName, string category, string newValue, string key){
	//updates a single value in sql database
	string expression, errorCheck = "None";

	expression = "UPDATE " + tableName + " SET " + category + "='" + newValue + "' WHERE ID=" + key;
	errorCheck = run_sql(expression);
	return errorCheck;
}

string Start_Up::return_table(string tableName){
	string expression, errorCheck = "None";

	expression = "SELECT * FROM " + tableName;
	errorCheck = run_multipleSql(expression);
	return errorCheck;
}

/*string Start_Up::get_rowValue(string expression, string* returnValue){
	char *sqlcommand;
	sqlite3_stmt *statement;
	int res, col = 0;
	string valueHolder, valueHolder2;

	char *convert = new char[expression.length() + 1];
	strcpy(convert, expression.c_str());
	sqlcommand = convert;
	res = sqlite3_prepare(sqldb, sqlcommand, -1, &statement, 0);
	if (res == SQLITE_OK){
		res = sqlite3_step(statement);
	}
	if (res == SQLITE_ROW){
		valueHolder = (char*)sqlite3_column_text(statement, col);
	}
	valueHolder2 = valueHolder;
	valueHolder2 = valueHolder2 + "#-#" + (char*)sqlite3_column_text(statement, 2)+ "#-#" + (char*)sqlite3_column_text(statement,3);
	//while (valueHolder != ""){
	//	col = col + 1;
	//	valueHolder = (char*)sqlite3_column_text(statement, col);
	//	valueHolder2 = valueHolder2 + "#-#" + valueHolder;
	//}
	delete[] convert;
	sqlite3_finalize(statement);
	*returnValue = valueHolder2;
	if (res == SQLITE_ERROR){
		return sqlite3_errmsg(sqldb);
	}
	else
		return "None";
}*/
string Start_Up::get_singleValue(string expression, string* returnValue){
	//gets a single value
	char *sqlcommand;
	sqlite3_stmt *statement;
	int res;

	//changes command to char* from string
	char *convert = new char[expression.length() + 1];
	strcpy(convert, expression.c_str());
	sqlcommand = convert;
	res = sqlite3_prepare(sqldb, sqlcommand, -1, &statement, 0);
	//runs sql command
	if (res == SQLITE_OK){
		res = sqlite3_step(statement);
	}
	if (res == SQLITE_ROW){
		*returnValue = (char*)sqlite3_column_text(statement, 0);
	}
	delete[] convert;
	sqlite3_finalize(statement);
	//returns error, if one exists
	if (res == SQLITE_ERROR){
		return sqlite3_errmsg(sqldb);
	}
	else {
		return "None";
	}

}
string Start_Up::run_sql(string expression){
	//runs the sql command
	char *sqlcommand;
	sqlite3_stmt *statement;
	int res;
	
	char* convert = new char[expression.length() + 1];
	strcpy(convert, expression.c_str());
	sqlcommand = convert;
	res = sqlite3_prepare(sqldb, sqlcommand, -1, &statement, 0);
	if (res == SQLITE_OK){
		while (true){
			res = sqlite3_step(statement);
			if (res == SQLITE_DONE || res == SQLITE_ERROR){
				break;
			}
		}
	}
	delete[] convert;
	sqlite3_finalize(statement);
	if (res == SQLITE_ERROR){
		return sqlite3_errmsg(sqldb);
	}
	else{
		return "None";
	}
}

string Start_Up::create_table(string table, string categories[], int num){
	//creates new directory table
	string expression, closeExpression = ");";
	string errorCheck, categoryString = "None";

	expression = "CREATE TABLE " + table + " (ID INTEGER PRIMARY KEY, Type varchar(3), Size varchar(32), Tags varchar(255), Date varchar(32), DateAccessed varchar(32), DateAdded varchar(32), Title varchar(32), gif varchar(255)";
	if (num > 0){
		categoryString = "";
		for (int x = 0; x < num; x++){
			expression = expression + ", " + categories[x] + " varchar(255)";
			categoryString = categoryString + categories[x] + "#-#";
		}
	}
	expression = expression + closeExpression;
	//return expression;
	errorCheck =run_sql(expression);
	if (errorCheck == "None"){
		expression = "INSERT INTO " + table + " (type,tags) VALUES ('categories','" + categoryString + "'"+ closeExpression; 
		errorCheck = run_sql(expression);
	}
	return errorCheck;
}


string Start_Up::initialize(string homedir){
	string expression, closeExpression = "')";
	string errorCheck;

	expression = "CREATE TABLE options (id varchar(10), options varchar(32));INSERT INTO options (id, options) VALUES ('initialized', '1');";
	expression = expression + "INSERT INTO options (id, options) VALUES ('homedir','" + homedir + closeExpression;
	errorCheck = run_multipleSql(expression);
	return errorCheck;
}

string Start_Up::findFileName(string original){
	//gets the file name
	int length = original.length(), pos;
	char compare;
	string nwString;

	for (int i = 0; i < length-1; i++){
		compare = original.at(i);
		if (compare == *".") {
			pos = i;
			i = length;
		}
	}

	nwString = "";
	
	for (int j = 0; j < pos; j++){
		nwString = nwString + original.at(j);
	}
	return nwString;

}

string Start_Up::findFileType(string original){
	//gets the file type
	int length = original.length(), pos;
	char compare;
	string nwString;

	for (int i = 0; i < length-1; i++){
		compare = original.at(i);
		if (compare == *"."){
			pos = i;
			i = length;
		}
	}

	nwString = "";
	for (int j = pos + 1; j < length; j++){
		nwString = nwString + original.at(j);
	}
	return nwString;
	
}

string Start_Up::filePathConvert(string originalPath){
	//converts the path from user friendly to computer friendl
	string newPath, compare;

	for (int i = 0; i < originalPath.length(); i++){
		compare = originalPath.at(i);
		if (compare == "\\"){
			newPath = newPath + "\\";
		}
		newPath = newPath + originalPath.at(i);
	}
	return newPath;
}
string Start_Up::initial_scan(string table, string dir){
	//scans the directories for files and then it adds them to the sql database
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE, hDate = INVALID_HANDLE_VALUE;
	DWORD dwFileSize;
	string tbool, convertDate;
	string expression, errorCheck, fType, fName, dateStr,newDir = "";
	string compare;
	LPTSTR lpFileTime =0;
	size_t tFileTime = 64;
	TCHAR szDir[MAX_PATH];
	const DWORD size = 100 + 1;
	TCHAR testp[MAX_PATH];
	string dateFilePath, nonDateFilePath = "c:", curDate;

	//initializes
	newDir = filePathConvert(dir);
	TCHAR *param = new TCHAR[newDir.size() + 1];
	param[newDir.size()] = 0;
	copy(newDir.begin(), newDir.end(), param);
	//converting the file path from string to programming usable
	StringCchCopy(szDir, MAX_PATH, param);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &ffd); //searches first file
	if (INVALID_HANDLE_VALUE == hFind){
		FindClose(hFind);
		return "ERROR!";
	}
	
	curDate = get_timeDate(); //gets current system time
	while (FindNextFile(hFind, &ffd) != 0){ //loop that just grabs files and file data
		wstring wfile (ffd.cFileName);
		string file(wfile.begin(), wfile.end());
		if (file.length() > 4){
			fType = findFileType(file); //gets file type
			fName = findFileName(file); //gets file name
			dwFileSize = GetFileSize(hFind, NULL); //gets file size
			//set of steps for full file and converst string
			newDir = dir + "\\" + file; 
			dateFilePath = filePathConvert(newDir);
			TCHAR *param = new TCHAR[dateFilePath.size() + 1];
			param[dateFilePath.size()] = 0;
			copy(dateFilePath.begin(), dateFilePath.end(), param);
			//starts the steps to get the file date
			hDate = CreateFile(param, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (INVALID_HANDLE_VALUE == hDate){
				FindClose(hFind);
				return "ERROR!" + dir + " || " + newDir + " || " + file;
			}
			tbool = getFileDate(hDate, testp, MAX_PATH); //gets file date
			//converts it from date type to string
			wstring wTime(testp);
			string dateStr(wTime.begin(), wTime.end());
			convertDate = dateStr;
			string size = getFileSize(ffd);
			expression = expression+ "INSERT INTO " + table + " (type, size, date, dateAdded, title) VALUES ('";
			expression = expression + fType + "', '" + size + "', '" + dateStr + "', '" + curDate + "', '" + fName + "');"; //sql expression to update database
			CloseHandle(hDate);
			errorCheck = run_sql(expression);
			if (errorCheck != "None")
				expression = errorCheck+expression;
			delete[] param;
			expression = "";
		}  
	}
	FindClose(hFind);
	return errorCheck;
}

string Start_Up::run_multipleSql(string expression){
	//runs multiple sql commands
	char *sqlcommand;
	char *zErrMsg = 0;
	int res;
	string errorCheck = "None";

	char* convert = new char[expression.length() + 1];
	strcpy(convert, expression.c_str());
	sqlcommand = convert;
	res = sqlite3_exec(sqldb, sqlcommand, callback, 0, &zErrMsg);

	if (res == SQLITE_ERROR) {
		errorCheck = string(zErrMsg);
	}
	return errorCheck;
}
Start_Up::Start_Up(){
	//initializes the class and checks for a sql database, if not one it calls program for it to be initialized
	char *sqlcommand; 
	char *zErrMsg = 0;
	string errorCheck = "None";

	rc = sqlite3_open("video.db", &sqldb);
	sqlcommand = "SELECT * FROM homedir";
	if (rc != SQLITE_OK){
		output = "ERROR: UNABLE TO OPEN DATABASE";
		return;
	}
	else
		output = "Opened Database Successfully!";
	//rc = sqlite3_exec(sqldb, sqlcommand, callback, 0, &zErrMsg);
	errorCheck = run_sql(sqlcommand);
	if (errorCheck != "None"){
		output = "Database is not yet initialized. Would you like to initialize it?";
		proceduralCode = 1;
		return;
	}
	else {
		proceduralCode = 2;
	}
	
}
