#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <random>
#include <cctype>
// Struct containing fullName of user and MasterPassword.
struct userDetails
{
	string firstName;
	string lastName;
	string masterPassword;
};
userDetails currentUser;
// global variables
string fileName;
string html_doc;
bool responseCheck();
bool checkPassword(const string &masterKey);
bool createLogin();
string generatePassword();
bool checkLogin();
void viewPasswords();
// check if html file is empty.
bool fileIsEmpty(const string &filename)
{
	ifstream file(filename);
	return file.peek() == ifstream::traits_type::eof(); // Checks if the file is empty
};
bool generateHtmlDoc(ofstream &myDoc);
bool setIdentity()
{
	userDetails fileUser;
	string file_name;
	bool validMasterKey;
	bool response = false, fileExists = false;
	cout << "Create an account! : \n";
	while (true)
	{
		cout << "Enter your first Name: (without spaces): ";
		cin >> fileUser.firstName;
		cout << "\nConfirm?: ";
		response = responseCheck();
		if (response == true)
		{
			break;
		}
	};
	while (true)
	{
		cout << "Enter your last Name: (without spaces): ";
		cin >> fileUser.lastName;
		cout << "\nConfirm?: ";
		response = responseCheck();
		if (response == true)
		{
			break;
		}
	};
	while (true)
	{
		cout << "\nEnter your MasterKey must be atleast 8 digits. ";
		cin >> fileUser.masterPassword;
		validMasterKey = checkPassword(fileUser.masterPassword);
		if (validMasterKey)
		{
			break;
		}
	}

	for (int i = 0; i < fileUser.firstName.length(); i++)
	{
		fileUser.firstName[i] = tolower(fileUser.firstName[i]);
	}
	for (int i = 0; i < fileUser.lastName.length(); i++)
	{
		fileUser.lastName[i] = tolower(fileUser.lastName[i]);
	}
	// variables storng filenames of html document and .txt document that will be created to store the passwords in the file.
	file_name = fileUser.firstName + fileUser.lastName + ".txt";
	html_doc = fileUser.firstName + fileUser.lastName + ".html";
	fileName = file_name;
	// ofstream to allow read write operations in file.
	ofstream myFile(file_name);
	// check if there is no file. (Proper error handling, function will return if there is any error while creating the file.)
	if (!myFile)
	{
		cout << "Error creating file.\n";
		return false;
	}
	// Write into the file. The first name, last name and the master_password.
	myFile << fileUser.firstName << " " << fileUser.lastName << " " << fileUser.masterPassword << endl;
	myFile << "Website\t" << "Password" << endl;
	myFile.close();
	cout << "Your file has been created! ";
	createLogin();
	return true;
}
bool getIdentity()
{
	bool response = false, fileExists = false;
	string file_name;
	userDetails fileUser;
	cout << "Sign In: \n";
	while (true)
	{
		cout << "Enter your first Name: (without spaces): ";
		cin >> fileUser.firstName;
		cout << "\nConfirm?: ";
		response = responseCheck();
		if (response == true)
		{
			break;
		}
	};
	while (true)
	{
		cout << "Enter your last Name: (without spaces): ";
		cin >> fileUser.lastName;
		cout << "\nConfirm?: ";
		response = responseCheck();
		if (response == true)
		{
			break;
		}
	};

	for (int i = 0; i < fileUser.firstName.length(); i++)
	{
		fileUser.firstName[i] = tolower(fileUser.firstName[i]);
	}
	for (int i = 0; i < fileUser.lastName.length(); i++)
	{
		fileUser.lastName[i] = tolower(fileUser.lastName[i]);
	}
	file_name = fileUser.firstName + fileUser.lastName + ".txt";
	fileName = file_name;
	ifstream readFile(file_name);

	//	Read the first line in the file using ifstream if the file is opened.
	if (readFile.is_open())
	{
		fileExists = true;
		readFile >> currentUser.firstName >> currentUser.lastName >> currentUser.masterPassword;
		readFile.close();
	}
	else
	{
		// if the file does not exist return false.
		cout << "File does not exist.\n";
		fileExists = false;
	}
	return fileExists;
}

int main()
{
	bool existingUser;
	bool isAuthenticatedUser;
	bool response = false;
	cout << "Are you an existing user: ";
	response = responseCheck();
	if (response == true)
	{
		existingUser = getIdentity();
	}
	else
	{
		existingUser = false;
		setIdentity();
	};
	if (existingUser)
	{
		cout << "Your file has been found!";
		cout << endl;
		isAuthenticatedUser = checkLogin();
		if (isAuthenticatedUser)
		{
			viewPasswords();
		}
	}
	return 0;
};

bool createLogin()
{
	bool response, html_response;
	string siteName, password;
	cout << "Create logins: ";
	while (true)
	{
		cout << "\nAdd a Login!\n";
		response = responseCheck();
		cout << "\nEnter the siteName: ";
		cin >> siteName;
		cout << "\nConfirmed siteName as: " << siteName << endl;

		password = generatePassword();

		if (response == true)
		{
			ofstream myFile(fileName, ios::app);
			myFile << siteName << " " << password << endl;
			myFile.close();
		}
		//	create an empty styled html page.
		cout << "\nDo you want to generate an HTML file as well.\n";
		// check for user response here.
		html_response = responseCheck();
		if (html_response)
		{
			ofstream myDoc(html_doc, ios::app);
			// fileIsEmpty is a function that returns true if the file is empty and false if the file is not empty.
			if (fileIsEmpty(html_doc))
			{
				generateHtmlDoc(myDoc);
			}
			// append passwords in the form of html tables to save an html document for users.
			myDoc << "<tr>"
				  << "<td><a href='https://" << siteName
				  << "' target='_blank'><i class='fas fa-external-link-alt'></i> "
				  << siteName << "</a></td>"
				  << "<td>"
				  << "<span class='password' style='display: none;'>" << password << "</span>"
				  << "<input type='password' value='" << password
				  << "' class='form-control d-inline-block password-input' readonly style='width: 70%;'>"
				  << "<button class='btn btn-sm btn-secondary toggle-password'>"
				  << "<i class='fas fa-eye'></i></button>"
				  << "<button class='btn btn-sm btn-primary copy-password'>"
				  << "<i class='fas fa-copy'></i></button>"
				  << "</td>"
				  << "</tr>\n";
				  myDoc.close();
		}
		if (response != true)
		{
			break;
		}
	}
	return true;
};
bool responseCheck()
{
	char response;
	do
	{
		cout << "Enter 'Y' for Yes, 'N' for No, 'E' to exit: ";
		cin >> response;
		response = tolower(response);
		// lowered the response entered by user to match the case.
		switch (response)
		{
		case 'y':
		{
			return true;
			break;
		}
		case 'n':
		{
			return false;
			break;
		}
		case 'e':
		{
			exit(0);
			break;
		}
		default:
		{
			cout << "\nInvalid Input";
		}
		}
	} while (true);
};
bool checkPassword(const string &masterKey)
{
	if (masterKey.length() < 8)
	{
		cout << "\nYour password must be at least 8 characters long.\n";
		return false;
	}
	bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
	for (char ch : masterKey)
	{
		if (isupper(ch))
			hasUpper = true;
		else if (islower(ch))
			hasLower = true;
		else if (isdigit(ch))
			hasDigit = true;
		else
			hasSpecial = true;
	}
	if (!hasUpper)
		cout << "\nYour password must contain at least one uppercase letter.\n";
	if (!hasLower)
		cout << "\nYour password must contain at least one lowercase letter.\n";
	if (!hasDigit)
		cout << "\nYour password must contain at least one digit.\n";
	if (!hasSpecial)
		cout << "\nYour password must contain at least one special symbol (e.g., @, #, $, etc.).\n";

	return hasUpper && hasLower && hasDigit && hasSpecial;
}
bool isSecurePassword(const string &password)
{
	if (password.length() < 8)
		return false;

	bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

	for (char ch : password)
	{
		if (isupper(ch))
			hasUpper = true;
		else if (islower(ch))
			hasLower = true;
		else if (isdigit(ch))
			hasDigit = true;
		else
			hasSpecial = true;
	}

	return hasUpper && hasLower && hasDigit && hasSpecial;
}

string generatePassword()
{
	//	predefined strucutre of passwords.
	const string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+";
	// create random password based on the time.
	srand(static_cast<unsigned>(time(0)));
	string password;
	while (true)
	{
		password.clear(); // Clear any previous attempt.

		for (int i = 0; i < 12; i++)
		{
			password += characters[rand() % characters.size()];
		}

		// Check if the generated password meets security requirements.
		if (isSecurePassword(password))
		{
			cout << "Generated Secure Password: " << password << endl;
			return password;
		}
	}
}

bool checkLogin()
{
	bool isAuthenticated = false;
	string masterKey;
	while (true)
	{
		cout << "Enter your Master Password: ";
		cin >> masterKey;
		if (masterKey == currentUser.masterPassword)
		{
			isAuthenticated = true;
			cout << "Authentication successful!\n";
			break; // Exit the loop on successful authentication
		}
		else
		{
			cout << "Incorrect password. Try again.\n";
		}

		bool response = responseCheck();
		if (!response)
		{
			cout << "Exiting login attempt.\n";
			break; // Exit the loop if the user chooses not to retry
		}
	}
	return isAuthenticated;
}

void viewPasswords()
{
	cout << "\nThese are your logins: \n";
	ifstream readFile(fileName);
	string readString;

	if (readFile.is_open())
	{
		// Skip only the header line
		getline(readFile, readString); // Skip the header
		while (getline(readFile, readString))
		{ // Read the remaining lines
			cout << readString << endl;
		}
		readFile.close();
	}
	else
	{
		cout << "Error opening file." << endl;
	}
	cout << "\n-------------------\nAdd Logins to your file\n";
	bool result = responseCheck();
	if (result)
	{
		createLogin();
	}
}

bool generateHtmlDoc(ofstream &myDoc)
{
    bool isSuccessful = false;

    if (myDoc.is_open())
    {
        myDoc << "<!DOCTYPE html>\n"
              << "<html lang=\"en\">\n"
              << "<head>\n"
              << "<meta charset=\"UTF-8\">\n"
              << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
              << "<title>Password Manager</title>\n"
              << "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n"
              << "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css\">\n"
              << "<style>\n"
              << "body {\n"
              << "    background-color: #f8f9fa;\n"
              << "    color: #212529;\n"
              << "    display: flex;\n"
              << "    justify-content: center;\n"
              << "    align-items: start;\n"
              << "    min-height: 100vh;\n"
              << "    margin-top: 10vh;\n"
              << "    font-family: 'Arial', sans-serif;\n"
              << "}\n"
              << "table {\n"
              << "    width: 80%;\n"
              << "    border-collapse: collapse;\n"
              << "    background-color: #ffffff;\n"
              << "    border: 1px solid #dee2e6;\n"
              << "    border-radius: 8px;\n"
              << "    overflow: hidden;\n"
              << "    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);\n"
              << "}\n"
              << "th, td {\n"
              << "    padding: 10px 15px;\n"
              << "    text-align: center;\n"
              << "}\n"
              << "th {\n"
              << "    background-color: #343a40;\n"
              << "    color: #ffffff;\n"
              << "    font-weight: bold;\n"
              << "}\n"
              << "tr:nth-child(even) {\n"
              << "    background-color: #f1f3f5;\n"
              << "}\n"
              << "tr:hover {\n"
              << "    background-color: #e9ecef;\n"
              << "    cursor: pointer;\n"
              << "}\n"
              << ".toast-container {\n"
              << "    position: fixed;\n"
              << "    bottom: 1rem;\n"
              << "    right: 1rem;\n"
              << "}\n"
              << "</style>\n"
              << "</head>\n"
              << "<body>\n"
              << "<div class='toast-container'>\n"
              << "    <div id='copy-toast' class='toast' role='alert' aria-live='assertive' aria-atomic='true'>\n"
              << "        <div class='toast-header'>\n"
              << "            <strong class='me-auto'>Notification</strong>\n"
              << "            <small>Just now</small>\n"
              << "            <button type='button' class='btn-close' data-bs-dismiss='toast' aria-label='Close'></button>\n"
              << "        </div>\n"
              << "        <div class='toast-body'>\n"
              << "        </div>\n"
              << "    </div>\n"
              << "</div>\n"
              << "<table>\n"
              << "<thead>\n"
              << "<tr><th>Site Name</th><th>Password</th><th>Actions</th></tr>\n"
              << "</thead>\n";

        myDoc << "<script>\n"
              << "document.addEventListener('DOMContentLoaded', () => {\n"
              << "    document.querySelectorAll('.toggle-password').forEach(button => {\n"
              << "        button.addEventListener('click', () => {\n"
              << "            const row = button.closest('tr');\n"
              << "            const input = row.querySelector('.password-input');\n"
              << "            if (input) {\n"
              << "                if (input.type === 'password') {\n"
              << "                    input.type = 'text';\n"
              << "                    button.innerHTML = \"<i class='fas fa-eye-slash'></i>\";\n"
              << "                } else {\n"
              << "                    input.type = 'password';\n"
              << "                    button.innerHTML = \"<i class='fas fa-eye'></i>\";\n"
              << "                }\n"
              << "            }\n"
              << "        });\n"
              << "    });\n"
              << "\n"
              << "    document.querySelectorAll('.copy-password').forEach(button => {\n"
              << "        button.addEventListener('click', () => {\n"
              << "            const row = button.closest('tr');\n"
              << "            const input = row.querySelector('.password-input');\n"
              << "            if (input) {\n"
              << "                navigator.clipboard.writeText(input.value).then(() => {\n"
              << "                    const toast = new bootstrap.Toast(document.getElementById('copy-toast'));\n"
              << "                    document.querySelector('#copy-toast .toast-body').textContent = 'Password copied!';\n"
              << "                    toast.show();\n"
              << "                }).catch(err => console.error('Clipboard copy failed', err));\n"
              << "            }\n"
              << "        });\n"
              << "    });\n"
              << "});\n"
              << "</script>\n";

        isSuccessful = true;
    }

    return isSuccessful;
}
