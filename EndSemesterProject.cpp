#include <iostream>
using namespace std;
#include <fstream>
#include <string>
//library for generating random password
#include <cstdlib>
#include <ctime>

//global variables
string firstName,lastName,masterPassword,fileName;
string html_doc;
bool responseCheck();
bool checkPassword(string masterKey);
bool createLogin();
string generatePassword();
bool checkLogin();
void viewPasswords();
//check if html file is empty. 
bool fileIsEmpty(const string& filename) {
    ifstream file(filename);
    return file.peek() == ifstream::traits_type::eof(); // Checks if the file is empty
};
bool generateHtmlDoc(ofstream& myDoc);
bool setIdentity() {
	bool validMasterKey;
	bool response = false, fileExists = false;
	string first_name, last_name,master_password,file_name;
	cout<<"Create an account! : \n";
	while(true) {
		cout<<"Enter your first Name: (without spaces): ";
		cin>>first_name;
		cout<<"\nConfirm?: ";
		response = responseCheck();
		if (response == true) {
			break;
		}
	};
	while(true) {
		cout<<"Enter your last Name: (without spaces): ";
		cin>>last_name;
		cout<<"\nConfirm?: ";
		response = responseCheck();
		if (response == true) {
			break;
		}
	};
	while (true) {
		cout<<"\nEnter your MasterKey must be atleast 8 digits. ";
		cin>>master_password;
		validMasterKey = checkPassword(master_password);
		if(validMasterKey) {
			break;
		}
	}

	for (int i = 0 ; i<first_name.length(); i++) {
		first_name[i] = tolower(first_name[i]);
	}
	for (int i = 0 ; i<last_name.length(); i++) {
		last_name[i] = tolower(last_name[i]);
	}
	file_name = first_name+last_name+".txt";
	html_doc = first_name+last_name+".html";
	fileName = file_name;
	ofstream myFile(file_name);
	if (!myFile) {
		cout << "Error creating file.\n";
		return false;
	}
	myFile << first_name << " " << last_name << " " << master_password<<endl;
	myFile <<"Website\t"<<"Password"<<endl;
	myFile.close();
	cout<<"Your file has been created! ";
	createLogin();
	return true;

}
bool getIdentity() {
	bool response = false,fileExists = false;

	string first_name,last_name,master_password,file_name;
	cout<<"Sign In: \n";
	while(true) {
		cout<<"Enter your first Name: (without spaces): ";
		cin>>first_name;
		cout<<"\nConfirm?: ";
		response = responseCheck();
		if (response == true) {
			break;
		}
	};
	while(true) {
		cout<<"Enter your last Name: (without spaces): ";
		cin>>last_name;
		cout<<"\nConfirm?: ";
		response = responseCheck();
		if (response == true) {
			break;
		}
	};

	for (int i = 0 ; i<first_name.length(); i++) {
		first_name[i] = tolower(first_name[i]);
	}
	for (int i = 0 ; i<last_name.length(); i++) {
		last_name[i] = tolower(last_name[i]);
	}
	file_name = first_name+last_name+".txt";
	fileName = file_name;
	//	Store the data in the file if it exists:
	ifstream readFile(file_name);
	if (readFile.is_open()) {
		fileExists = true;
		readFile>>firstName>>lastName>>masterPassword;
		readFile.close();
		// validate with the userInput!
		cout<<firstName<<endl;
		//	if true authenticate user to view all passwords by returning true.
	} else {
		cout<<"File does not exist.\n";
	}
	return fileExists;

}

int main() {
	char input;
	bool existingUser;
	bool isAuthenticatedUser;
	bool response = false;
	cout<<"Are you an existing user: ";
	response = responseCheck();
	if(response == true) {
		existingUser = true;
		getIdentity();
	} else {
		existingUser = false;
		setIdentity();
	};
	existingUser = getIdentity();
	if (existingUser) {
		cout<<"Your file has been found!";
		cout<<endl;
		cout<<"Please enter your password: ";
		isAuthenticatedUser = checkLogin();
		if (isAuthenticatedUser) {
			// call function to show passwords.
			viewPasswords();
		}
	}
	while (true) {
		cout<<"\nPress Y to exit";
		if (input == 'Y') break;
		if (response == true) {
			getIdentity();
		}
		if (response !=true) {
			setIdentity();
		}
	}
	return 0;
};


bool createLogin() {
	bool response,html_response;
	string siteName,password;
	cout<<"Create logins: ";
	while(true) {
		cout<<"\nAdd a Login!\n";
		response = responseCheck();
		cout<<"\nEnter the siteName: ";
		cin>>siteName;
		cout<<"\nConfirmed siteName as: "<<siteName<<endl;

		password = generatePassword();
		if(response == true) {
			ofstream myFile(fileName,ios::app);
			myFile<<siteName<<" "<<password<<endl;
			myFile.close();
		}
		//	create an empty styled html page.
		cout<<"\nDo you want to generate an HTML file as well.\n";
		// check for user response here.
		html_response = responseCheck();
		if (html_response) {
			ofstream myDoc(html_doc, ios::app);
			if (fileIsEmpty(html_doc)) {
                generateHtmlDoc(myDoc);
            }
			myDoc << "<tr><td>" << siteName << "</td><td>" << password << "</td></tr>\n";
			myDoc.close();
		}
		if(response !=true) {
			break;
		}

	}
	return true;
};
bool responseCheck() {
	char response;
	do {
		cout<<"Enter 'Y' for Yes, 'N' for No, 'E' to exit: ";
		cin>>response;
		response = tolower(response);
		switch(response) {
			case 'y': {
				return true;
				break;
			}
			case 'n': {
				return false;
				break;
			}
			case 'e': {
				exit(0);
				break;
			}
			default: {
				cout<<"\nInvalid Input";
			}
		}
	} while(true);
};
bool checkPassword(string masterKey) {

	bool isValidKey = false;
	if (masterKey.length()>=8) {
		isValidKey = true;
	} else {
		isValidKey = false;
		cout<<"\nYour Password must be atleast 8 digits long";
	}
	return isValidKey;
};
string generatePassword() {
	string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+";
	string password = "";
	int n = characters.length();

	for (int i = 0; i < 12; ++i) {
		int randIndex = rand() % n;
		password += characters[randIndex];
	}

	return password;
}
bool checkLogin() {
	bool isAuthenticated = false,response;
	cout<<"Enter your masterKey: \n";
	string masterKey;
	while (true) {
		cout<<"Enter your Master Password: ";
		cin>>masterKey;
		cout<<"Confirm?\n";
		response = responseCheck();
		if(response == true) {
			break;
		}
	}
	if(masterKey == masterPassword) {
		isAuthenticated = true;
	}
	return isAuthenticated;
};
void viewPasswords() {
	// chatgpt code:
	cout << "\nThese are your logins: \n";
	ifstream readFile(fileName);
	string readString;

	if (readFile.is_open()) { // Simplified this check
		// Skip the first 3 lines
		for (int i = 0; i < 3; i++) {
			getline(readFile, readString);
		}

		// Read and display the rest of the lines
		while (getline(readFile, readString)) { // This will return false when end-of-file is reached
			cout << readString << endl;
		}

		readFile.close();
	} else {
		cout << "Error opening file." << endl;
	}
};
bool generateHtmlDoc(ofstream& myDoc) {
	bool isSuccessful = false;
	if (myDoc.is_open()) {
		myDoc << "<!DOCTYPE html>\n"
		      << "<html>\n"
		      << "<head>\n"
		      << "<title>Password Manager</title>\n"
		      << "<style>\n"
		      << "table { width: 50%; border-collapse: collapse; margin: 20px 0; }\n"
		      << "th, td { border: 1px solid #ddd; padding: 8px; text-align: center; }\n"
		      << "th { background-color: #f2f2f2; }\n"
		      << "tr:nth-child(even) { background-color: #f9f9f9; }\n"
		      << "</style>\n"
		      << "</head>\n"
		      << "<body>\n"
		      << "<h2>Stored Passwords</h2>\n"
		      << "<table>\n"
		      << "<tr><th>Site Name</th><th>Password</th></tr>\n";
		isSuccessful = true;
	}
	return isSuccessful;
}