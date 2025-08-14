#include <iostream>
#include <string>
#include <cctype>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;
const string FileName = "Clients.txt.";

enum enChoices
{
	ShowClientLists = 1,
	AddNewClient = 2,
	DeleteClient = 3,
	UpdateClientInfo = 4,
	FindClient = 5,
	Exit = 6
};

struct sClient
{
	string AccountNumber;
	string Pincode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;


};

short UserChoice()
{
	short Choice = 0;
	cin >> Choice;
	return Choice;

}

void ShowHeaderScreen(string Message) //الهيدر بتاع اي بروسيس
{
	cout << "\n\n----------------------------------------------------\n";
	cout << "                  " << Message << "                      \n";      
	cout << "\n----------------------------------------------------\n";

}

vector <string > SplitString(string S1, string Delim) //  دي بتاخد السطر من الفايل و بتحذف الفواصل و تحط البيانات ف فيكتور , الفتكور ده ف بينانات عميل واحد 
{
	vector <string> vData;
	int Pos = 0;
	string sWord = "";

	while ((Pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, Pos);
		if (sWord != "")
		{
			vData.push_back(sWord);
		}

		S1.erase(0, Pos + Delim.length());

	}

	if (S1 != "")
	{
		vData.push_back(S1);
	}


	return vData;


}

sClient ConvertLineToRecord(string Line, string Seperator = "#//#") // دا بقا بياخد اللاين بيعمله فيكتور ومن الفيكتور يملي بيانات استراكشر لعميل واحد
{
	sClient Client;
	vector <string> vClientData;
	vClientData = SplitString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.Pincode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);


	return Client;


}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#") // لو عاوزه تضيف البيانات لفايل ف بتاخد ستراكشر و بتحوله ل لاين 
{
	string Line = "";

	Line = Client.AccountNumber + Seperator;
	Line += Client.Pincode + Seperator;
	Line += Client.Name + Seperator;
	Line += Client.Phone + Seperator;
	Line += to_string(Client.AccountBalance);

	return Line;


}

vector <sClient> LoadDataFromFile(string FileName) // فيكتور من نوع استراكت فيه العملاء كلهم
{
	vector <sClient> vClients;

	fstream MyFile;

	MyFile.open(FileName, ios::in); //Read Mode

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);  // دي بيرجعلي استراكشر مليان ف اخزنه ف فيكتور العملاء
			vClients.push_back(Client);

		}

		MyFile.close();

	}

	return vClients;

}

void PrintClientRecord(sClient Client) // بطبع بيانات عميل واحد ف الليست وانا بستدعيها لكل واحد بقا
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.Pincode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;


}

void PrintAllClientsData(vector <sClient> &vClients) // Client List
{
	// (العنوان (بداية الليست
	cout << "\n\t\t\t\t Client List (" << vClients.size() << ") Client(s).";
	cout << "\n\n--------------------------------------------------------------------------------";
	cout << "--------------------------\n\n";

	// تقسيم جدول الليست

	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(10) << left << "Pin Code";
	cout << "| " << setw(40) << left << "Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance";

	cout << "\n\n--------------------------------------------------------------------------------";
	cout << "--------------------------\n";


	// بيانات العملاء بقا

	for (sClient C : vClients)
	{
		PrintClientRecord(C);
		cout << endl;

	}


}

string ReadAccountNumber()
{
	string AccountNumber;
	cout << "Please enter Account Number ?  ";
	cin >> AccountNumber;
	return AccountNumber;

}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> & vClients , sClient &Client) // اشوف رقم الحساب موجود عندنا ولا لا
{
	for (sClient &C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;  // لو موجود باخد بياناته
			return true;
		}
		
	}
	return false;

}

sClient NewClientData(string AccountNumber) // باخد بيانات العميل الجديد
{
	sClient	NewClient;

	NewClient.AccountNumber = AccountNumber; // رقم الحساب مش بغيره , بغير بيانات العميل بس

	cout << "\nEnter Pin Code ? ";
	getline(cin >> ws, NewClient.Pincode);

	cout << "\nEnter Name ? ";
	getline(cin, NewClient.Name);

	cout << "\nEnter Phone ? ";
	getline(cin, NewClient.Phone);

	cout << "\nEnter Account Balance ? ";
	cin >> NewClient.AccountBalance;



	return NewClient;

}

void SaveClientsToFile(string FileName, vector <sClient> &vClients) // بحفظ التغيرات اللي عملتها ف الفايل , ضيفت عميل جديد
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (sClient &C : vClients)
		{
			//  هتكون ب ترواو لو عدلت علي عميل ف نفس الكلامMake for delete ال  by default الخطوه دي عصفورين بحجر , لو الاضافه عميل جديد ف اوريدي   
			//بفولس ف م هيتحط ف الفايل Make for delete تاني حاجه ف جزء مسح عميل هتكون 

			if (C.MarkForDelete == false)  
			{
				MyFile << ConvertRecordToLine(C) << endl;
			}
			
		}

		MyFile.close();

	}


}

void AddNewClientScreen(vector <sClient>& vClients)
{
 // ال format 

	ShowHeaderScreen("Add New Client");
	cout << "Adding New Client : \n\n";


	// اخد الاكونت نمبر علشان اتشيك عليه

	sClient Client;
	string AccountNumber = ReadAccountNumber();

	// اتشيك بقا 
	// لو موجود يدخل رقم تاني , لو م موجود اخد منه باقي البيانات

	while (FindClientByAccountNumber(AccountNumber, vClients , Client))
	{
		cout << "Client with [" << AccountNumber << "] already exists , Enter another Account Number ? ";
		cin >> AccountNumber;
	}

	Client = NewClientData(AccountNumber);
	vClients.push_back(Client);    // اضيف العميل الجديد للفيكتور 

	SaveClientsToFile(FileName, vClients); // احفظ التغييرات دي بقا 

	char Answer;
	cout << "\nDo you want to add another client? (Y/N): ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		system("cls");
		AddNewClientScreen(vClients); // لو عاوز يضيف عميل تاني
	}


}

void PrintClientCard(sClient Client)
{
	cout << "\n\nThe following are the client detials: \n";
	cout << "---------------------------------------\n";

	cout << "Account Number : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.Pincode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n---------------------------------------\n";

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber , vector <sClient>& vClients) // بتاعته ب تروMake for delete العميل اللي عاوزه امسحه اخلي ال 
{

	for (sClient &C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;

		}

	}
	return false;

}

void DeleteClientByAccountNumber(vector<sClient> &vClients)
{
	ShowHeaderScreen("Delete Client Screen");
	char Answer = 'n';
	sClient Client;
	string AccountNumber = ReadAccountNumber(); // هاخد الاكونت نمبر

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client); // اطبع بيانات العميل ده قبل ما امسحه

		cout << "\n\nAre you sure you want to delete this client ? y/n ?  "; // التاكيد علي خطوة المسح
		cin >> Answer;


		if (Answer == 'Y' || Answer == 'y')   // لو متأكد انه هيسمح
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients); // بتاعته ب تروMake for delete اخلي ال 
			SaveClientsToFile(FileName, vClients);                       // امسحه من الفايل
			vClients = LoadDataFromFile(FileName);                       // اعدل علي الفيكتور , امسحه منه بردو
			cout << "\n\nClient Deleted Successfully.\n";
		

		}


	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n"; // لو الاكونت نمبر م موجود
	}

}

void UpdateClientData(vector<sClient>& vClients)  // اعدل علي بيانات عميل موجود بالفعل
{
	ShowHeaderScreen("Update Client Info Screen");

	char Answer = 'n';
	sClient Client;
	string AccountNumber = ReadAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) // اتشيك لو موجود من خلال الاكونت نمبر
	{
		PrintClientCard(Client);      // اطبع بياناته

		cout << "\n\nAre you sure you want to update client ? y/n ? ";  // التاكيد علي التعديل
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')  // لو متاكد انه هيعدل 
		{
			for (sClient& C : vClients) // انا عدلت هنا علي الفيكتور لان عامله باي ريف
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = NewClientData(AccountNumber); // اخد البيانات الجديده و عدلتها
					break;
				}

			}

			SaveClientsToFile(FileName, vClients); // اعدل الفايل 
			

			cout << "\n\nClient Updated Successfully\n";
		}


	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
	}

}

void SearchForClient(vector<sClient>& vClients)
{
	ShowHeaderScreen("Find Client Screen");

	sClient Client;
	string AccountNumber = ReadAccountNumber(); // اخد الاكونت نمبر

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))  // اتشيك 
	{
		PrintClientCard(Client);      // اطبعه لو موجود 

	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n"; // لو مش موجود 
	}


}

void ShowEndScreen()
{
	system("cls");
	ShowHeaderScreen("Program Ends :-)");

}


void ShowMainMenueScreen()
{
	system("cls");
	
	cout << "==============================================================\n";
	cout << "                   Main Menue Screen\n";
	cout << "==============================================================\n";
	cout << "                   [1] Show Client List.\n";
	cout << "                   [2] Add New Client.\n";
	cout << "                   [3] Delete Client.\n";
	cout << "                   [4] Update Client Info\n";
	cout << "                   [5] Find Client.\n";
	cout << "                   [6] Exit.\n";
	cout << "===============================================================";
	cout << "\nChoose what do you want to do ? [ 1 to 6 ] ?  ";

}

void BackToMainScreen()
{
	cout << "\n\nPress any Key to go back to Main Menue....";
	system("pause>0");
	
}

void StartBankProgram()
{
	vector <sClient> vClients = LoadDataFromFile(FileName);
	short Choice;

	do
	{
		ShowMainMenueScreen();
		Choice = UserChoice();

		switch (Choice)
		{
		case(enChoices::ShowClientLists):
			system("cls");
			PrintAllClientsData(vClients);
			BackToMainScreen();
			break;

		case(enChoices::AddNewClient):
			system("cls");
			AddNewClientScreen(vClients);
			BackToMainScreen();
			break;

		case(enChoices::DeleteClient):
			system("cls");
			DeleteClientByAccountNumber(vClients);
			BackToMainScreen();
			break;

		case(enChoices::UpdateClientInfo):
			system("cls");
			UpdateClientData(vClients);
			BackToMainScreen();
			break;

		case(enChoices::FindClient):
			system("cls");
			SearchForClient(vClients);
			BackToMainScreen();
			break;

		case(enChoices::Exit):
			ShowEndScreen();
			break;
		}

	} while (Choice != enChoices::Exit);

}

int main()
{
	StartBankProgram();
	

}