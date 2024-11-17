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
		existingUser = getIdentity();
	} else {
		existingUser = false;
		setIdentity();
	};
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
			myDoc << "<tr><td><a href='https://" << siteName
			      << "' target='_blank'><i class='fas fa-external-link-alt'></i> "
			      << siteName << "</a></td><td>"
			      << password << "</td></tr>\n";
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
		      << "<html lang=\"en\">\n"
		      << "<head>\n"
		      << "<meta charset=\"UTF-8\">\n"
		      << "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
		      << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		      << "<title>Password Manager</title>\n"
		      << "<!-- Bootstrap CSS -->\n"
		      << "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n"
		      << "<!-- Font Awesome for icons -->\n"
		      << "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css\">\n"
		      << "<style>\n"
		      << "body {\n"
		      << "    background-color: #1e1e2f;\n"
		      << "    color: #e0e0e0;\n"
		      << "    display: flex;\n"
		      << "    justify-content: center;\n"
		      << "    align-items: center;\n"
		      << "    min-height: 100vh;\n"
		      << "    margin: 0;\n"
		      << "    font-family: 'Arial', sans-serif;\n"
		      << "}\n"
		      << "table {\n"
		      << "    width: 70%;\n"
		      << "    border-collapse: collapse;\n"
		      << "    background-color: #2a2a3b;\n"
		      << "    border-radius: 8px;\n"
		      << "    overflow: hidden;\n"
		      << "    animation: fadeIn 1s ease-in-out;\n"
		      << "    box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3);\n"
		      << "}\n"
		      << "th, td {\n"
		      << "    padding: 15px 20px;\n"
		      << "    text-align: left;\n"
		      << "}\n"
		      << "th {\n"
		      << "    background-color: #3b3b52;\n"
		      << "    color: #ffffff;\n"
		      << "    text-transform: uppercase;\n"
		      << "    font-weight: bold;\n"
		      << "}\n"
		      << "tr:nth-child(even) {\n"
		      << "    background-color: #33334d;\n"
		      << "}\n"
		      << "tr:hover {\n"
		      << "    background-color: #444466;\n"
		      << "    cursor: pointer;\n"
		      << "    transition: background-color 0.3s;\n"
		      << "}\n"
		      << "a {\n"
		      << "    color: #5dade2;\n"
		      << "    text-decoration: none;\n"
		      << "    font-weight: 600;\n"
		      << "    display: flex;\n"
		      << "    align-items: center;\n"
		      << "}\n"
		      << "a:hover {\n"
		      << "    color: #76c7f2;\n"
		      << "}\n"
		      << "i {\n"
		      << "    margin-right: 8px;\n"
		      << "}\n"
		      << "td:last-child {\n"
		      << "    font-family: 'Courier New', Courier, monospace;\n"
		      << "    font-weight: 500;\n"
		      << "}\n"
		      << "@keyframes fadeIn {\n"
		      << "    from { opacity: 0; }\n"
		      << "    to { opacity: 1; }\n"
		      << "}\n"
		      << "</style>\n"
		      << "</head>\n"
		      << "<body>\n"
		      << "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js\"></script>\n"
		      << "<table>\n"
		      << "<thead>\n"
		      << "<tr><th>Site Name</th><th>Password</th></tr>\n"
		      << "</thead>\n";

		isSuccessful = true;
	}
	return isSuccessful;
}