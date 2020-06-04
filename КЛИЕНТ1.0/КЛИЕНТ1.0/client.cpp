#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <iostream>
#include <iomanip>
#include <WinSock2.h>
#include <winsock.h>
#include <sstream>
#include <vector>
#include <map>
#include <conio.h>
using namespace std;
int main();
//////////////////////////////////////////////////////////////
template<class T>
class entrance
{
	T pass[255];

public:
	static T log[255];
	T login[255];
	//friend class token;
	void getlog(T str[]) { strcpy(log, str); }
	static char* crypt(T pass[255]);
	static int openuser();
	static int deleteuser()
	{
		FILE* fp;
		entrance* mas, temp;
		char check;
		int k = 0, zapis = 0;

		fopen_s(&fp, "loginuser.txt", "r");
		if (fp != NULL)
		{
			if (fscanf_s(fp, "%c", &check, 1) != EOF)
			{
				fseek(fp, -1, SEEK_CUR);
				while (!feof(fp))
				{
					fscanf_s(fp, "%s", temp.login, sizeof(temp.login));
					fscanf_s(fp, "%s\n", temp.pass, sizeof(temp.pass));
					++k;
				}
				rewind(fp);
				mas = new entrance[k];
				for (int i = 0; i < k; i++)
				{
					fscanf_s(fp, "%s", mas[i].login, sizeof(mas[i].login));
					fscanf_s(fp, "%s\n", mas[i].pass, sizeof(mas[i].pass));
				}
				fclose(fp);
				cout << "Введите номер записи для удаления: " << endl;
				scanf_s("%d", &zapis);
				if (zapis < 1 || zapis > k)
				{
					return 1;
				}
				fopen_s(&fp, "loginuser.txt", "w");
				for (int i = 0; i < k; i++)
				{
					if (i + 1 == zapis) continue;
					fprintf(fp, "%s %s\n", mas[i].login, mas[i].pass);
				}
				rewind(fp);
				delete[] mas;
				fclose(fp);
				system("pause");
				return 0;
			}
			else
			{
				system("pause");
				fclose(fp);
				return 1;
			}
		}
		else { cout << "error of opening!" << endl; }
	}
	static int adduser()
	{
		FILE* fp;
		entrance temp;
		int k = 0, i = 0, j = 0, size = 0;
		cout << "Введите логин:\n";
		scanf_s("%s", temp.login, 255);
		_getch();
		printf_s("Введите пароль:\n");
		while (1)
		{
			temp.pass[i] = _getch();
			if (temp.pass[i] == '\r') break;
			if (temp.pass[i] == '\b')
			{
				printf("%s", "\b \b");
				--i;
			}
			else
			{
				printf("%c", '*');
				++i;
			}
		}
		temp.pass[i] = '\0';
		printf("\n");
		if (i - 1 < 1)
		{
			_getch();
			return 1;
		}
		strcpy_s(temp.pass, crypt(temp.pass));
		fopen_s(&fp, "loginuser.txt", "a");
		fprintf(fp, "%s %s\n", temp.login, temp.pass);
		fclose(fp);

		_getch();
		return 0;
	}
	static int addadmin()
	{
		FILE* fp;
		entrance temp;
		int k = 0, i = 0, j = 0, size = 0;
		cout << "Введите логин:\n";
		scanf_s("%s", temp.login, 255);
		_getch();
		printf_s("Введите пароль:\n");
		while (1)
		{
			temp.pass[i] = _getch();
			if (temp.pass[i] == '\r') break;
			if (temp.pass[i] == '\b')
			{
				printf("%s", "\b \b");
				--i;
			}
			else
			{
				printf("%c", '*');
				++i;
			}
		}
		temp.pass[i] = '\0';
		printf("\n");
		if (i - 1 < 1)
		{
			_getch();
			return 1;
		}
		strcpy_s(temp.pass, crypt(temp.pass));
		fopen_s(&fp, "loginadmin.txt", "a");
		fprintf(fp, "%s %s\n", temp.login, temp.pass);
		fclose(fp);

		_getch();
		return 0;
	}
	static int loginadmin()
	{
		errno_t err;
		FILE* fr;
		entrance temp;
		char login[255], pass[255];
		int k = 0, i = 0;

		printf("Введите логин:\n");
		scanf_s("%s", login, 255);
		getchar();
		printf_s("Введите пароль от двух символов:\n");
		while (1)
		{
			pass[i] = _getch();
			if (pass[i] == '\r') break;
			if (pass[i] == '\b')
			{
				printf("%s", "\b \b");
				--i;
			}
			else
			{
				printf("%c", '*');
				++i;
			}
		}
		pass[i] = '\0';
		printf("\n");
		if (i - 1 < 1)
		{
			return 1;
		}

		strcpy_s(pass, crypt(pass));
		err = fopen_s(&fr, "loginadmin.txt", "r");
		if (fr != NULL)
		{
			while (!feof(fr))
			{
				fscanf_s(fr, "%s\n", temp.login, sizeof(temp.login));
				fscanf_s(fr, "%s\n", temp.pass, sizeof(temp.pass));
				if (strcmp(login, temp.login) == 0 && strcmp(pass, temp.pass) == 0)
				{
					fclose(fr);
					return 0;
				}
			}
			fclose(fr);
			return 1;

		}

	}
	int loginuser()
	{
		errno_t err;
		FILE* fp;
		entrance temp;
		char login[255], pass[255];
		int k = 0, i = 0;

		printf("Введите логин:\n");
		scanf_s("%s", login, 255);
		temp.getlog(login);
		printf_s("Введите пароль от двух символов:\n");
		while (1)
		{
			pass[i] = _getch();
			if (pass[i] == '\r') break;
			if (pass[i] == '\b')
			{
				printf("%s", "\b \b");
				--i;
			}
			else
			{
				printf("%c", '*');
				++i;
			}
		}
		pass[i] = '\0';
		printf("\n");
		if (i - 1 < 1)
		{
			return 1;
		}
		strcpy_s(pass, crypt(pass));
		err = fopen_s(&fp, "loginuser.txt", "r");
		while (!feof(fp))
		{
			fscanf_s(fp, "%s\n", temp.login, sizeof(temp.login));
			fscanf_s(fp, "%s\n", temp.pass, sizeof(temp.pass));
			if (strcmp(login, temp.login) == 0 && strcmp(pass, temp.pass) == 0)
			{
				fclose(fp);
				return 0;
			}
		}
		fclose(fp);
		return 1;
	}
};
char entrance<char>::log[255];
template<class T>
char* entrance<T>::crypt(T pass[255]) {
	char temp;

	if (strlen(pass) % 2 == 0)
	{
		for (int i = 0; i < strlen(pass); i = i + 2)
		{

			temp = pass[i];
			pass[i] = pass[i + 1] + 2;
			pass[i + 1] = temp + 2;
		}
	}
	else
	{
		for (int i = 0; i < strlen(pass); i = i + 2)
		{
			if (i + 1 == strlen(pass)) break;
			temp = pass[i];
			pass[i] = pass[i + 1] + 2;
			pass[i + 1] = temp + 2;
		}
	}
	return pass;
}
template<class T>
int entrance<T>::openuser() {
	FILE* fp;
	entrance temp, *mas;
	char check;
	int k = 0;

	fopen_s(&fp, "loginuser.txt", "r");
	if (fp != NULL)
	{
		if (fscanf_s(fp, "%c", &check, 1) != EOF)
		{
			fseek(fp, -1, SEEK_CUR);
			while (!feof(fp))
			{
				fscanf_s(fp, "%s", temp.login, sizeof(temp.login));
				fscanf_s(fp, "%s\n", temp.pass, sizeof(temp.pass));
				++k;
			}
			rewind(fp);
			mas = new entrance[k];
			for (int i = 0; i < k; i++)
			{
				fscanf_s(fp, "%s", mas[i].login, sizeof(mas[i].login));
				fscanf_s(fp, "%s\n", mas[i].pass, sizeof(mas[i].pass));
			}
			rewind(fp);
			fclose(fp);
			printf("Пользователи:\n");
			printf("--------------------------------\n");
			printf("| Логин пользователя |\n");
			printf("--------------------------------\n");
			for (int i = 0; i < k; i++)
			{
				printf("|%24s|\n", mas[i].login);
				printf("--------------------------------\n");
			}
			system("pause");
			return 0;
		}
		else
		{
			fclose(fp);
			system("pause");
			return 1;
		}
	}
	else { cout << "error of opening!" << endl; }

}

int edituser()
{
	int k = 0;
	entrance<char> temp;
	while (1)
	{
		system("cls");
		printf("1.Добавить пользователя\n");
		printf("2.Удалить пользователя\n");
		printf("3.Показать всех пользователей\n");
		printf("0.Выход\n");
		cin >> k;
		switch (k)
		{
		case 1:
			system("cls");
			if (temp.adduser() == 0)
			{
				printf("Добавление успешно!\n");
			}
			else
			{
				printf("Ошибка!\n");
			}
			getchar();
			break;
		case 2:
			system("cls");
			temp.openuser();
			if (temp.deleteuser() == 0)
			{
				printf("Удаление успешно!\n");
			}
			else
			{
				printf("Ошибка!\n");
			}
			getchar();
			break;
		case 3:
			system("cls");
			if (temp.openuser() == 0)
			{
				printf("Чтение успешно!\n");
			}
			else
			{
				printf("Файл пуст!\n");
			}
			getchar();
			break;
		case 0: return 0;
		default: printf("Ошибка!\n"); getchar();
		}
	}
}

int editadmin()
{
	int k = 0;
	entrance<char> temp;
	while (1)
	{
		system("cls");
		printf("1.Добавить пользователя\n");
		printf("0.Выход\n");
		cin >> k;
		switch (k)
		{
		case 1:
			system("cls");
			if (temp.addadmin() == 0)
			{
				printf("Добавление успешно!\n");
			}
			else
			{
				printf("Ошибка!\n");
			}
			getchar();
			break;
		case 0: return 0;
		default: printf("Ошибка!\n"); getchar();
		}
	}
}
/////////////////////////////////////////////////////////
class SClient
{
private:
	sockaddr_in dest_addr;
	WSAData wData;
public:
	SOCKET s;
	SClient() {}
	~SClient() {}
	SOCKET start();
	string receive(SOCKET s2)
	{
		char buf[100];
		if (recv(s2, buf, sizeof(buf), 0) != 0)
		{
			buf[strlen(buf)] = '\0';
			//cout << "recv " << buf << endl;
		}
		//cout << "recv " << buf << endl;
		return buf;
	}
	string receive_int(SOCKET s2)
	{
		char buf[100];
		if (recv(s2, buf, sizeof(buf), 0) != 0)
		{
			buf[strlen(buf)] = '\0';
			//cout << "recv " << buf << endl;
		}
		//cout << "recv " << buf << endl;
		return buf;
	}
	void send_int(SOCKET s2, int k)
	{
		try {
			if (sizeof(k) > 100)
				throw length_error("Вызвано исключение(превышен размер строки)");
			else {
				char buf[100];
				itoa(k, buf, 10);
				send(s2, buf, sizeof(buf), 0);
			}
		}
		catch (length_error &er) {
			cout << er.what() << endl;
			getchar();
			main();
		}
	}
	void send_str(SOCKET s2, char s[])
	{
		try {
			if (strlen(s) > 100)
				throw length_error("Вызвано исключение(превышен размер строки)");
			else {
				char str[100];
				strcpy(str, s);
				send(s2, str, sizeof(str), 0);
			}
		}
		catch (length_error &er) {
			cout << er.what() << endl;
			getchar();
			main();
		}
	}
};
SOCKET SClient::start()
{
	if (WSAStartup(MAKEWORD(2, 2), &wData) != 0)
	{
		printf("WSA Startup succes\n");
		return 1;
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(1280);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s = socket(AF_INET, SOCK_STREAM, 0);

	connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr));
	printf("Client created\n");
	return s;
}

class InfoService
{
	string name;
	float price;
	string profit;
	int kol_training;
	int duration_hours;
	int duration_minute;
public:
	string getname() { return name; }
	void receive_serv(SOCKET s2)
	{
		SClient client;
		char buf[100];
		name = client.receive(s2).c_str();
		profit = client.receive(s2).c_str();
		duration_hours = atoi(client.receive_int(s2).c_str());
		duration_minute = atoi(client.receive_int(s2).c_str());
		kol_training = atoi(client.receive_int(s2).c_str());
		if (recv(s2, buf, sizeof(buf), 0) != 0) {
			price = atof(buf);
		}
	}
	void send_serv(SOCKET s2, InfoService& obj, int flag)
	{
		char buff[100];
		SClient client;
		if (flag == 1)
		{
			while (true)
			{
				cout << "Введите название услуги "; getline(cin, name);
				strcpy(buff, obj.name.c_str());
				client.send_str(s2, buff);
				int ans = atoi(client.receive_int(s2).c_str());
				if (ans == 4) break;
			}
			obj.add_serv();
		}
		else if (flag == 0)
		{
			strcpy(buff, obj.name.c_str());
			client.send_str(s2, buff);
		}
		strcpy(buff, obj.profit.c_str());
		client.send_str(s2, buff);
		client.send_int(s2, obj.duration_hours);
		client.send_int(s2, obj.duration_minute);
		client.send_int(s2, obj.kol_training);
		client.send_int(s2, obj.price);
	}
	void edit_serv()
	{
		while (1)
		{
			int flag = 0;
			cout << "Выберите поле для изменения" << endl;
			cout << "1. Полезные свойства" << endl;
			cout << "2. Продолжительность" << endl;
			cout << "3. Количество занятий" << endl;
			cout << "4. Цена" << endl;
			cout << "0. Выход\n";
			while (true)
			{
				cin.unsetf(ios::skipws);
				cin >> flag;
				if (flag >= 5 || flag < 0) {
					cout << "Неверный ввод. Попробуйте еще раз\n";
					cin.clear(ios::failbit);
				}
				if (cin.good()) {
					cin.ignore(10, '\n');
					break;
				}
				cin.clear();
				cin.ignore(10, '\n');
			}
			char buf[100];
			SClient client;
			switch (flag) {
			case 1:
				while (true)
				{
					cout << "Введите полезные свойства ";
					cin.unsetf(ios::skipws);
					getline(cin, profit);
					if (cin.good()) {
						cin.ignore(10, '\n');
						break;
					}
					cin.clear();
					cout << "Неправильный ввод данных\n";
					cin.ignore(10, '\n');
				}
				break;
			case 2:
			{
				while (true)
				{
					cout << "Введите продолжительность(часы) ";
					cin.unsetf(ios::skipws);
					cin >> duration_hours;
					if (duration_hours > 10 || duration_hours < 0) {
						cout << "Неправильный ввод данных\n";
						cin.clear(ios::failbit);
					}
					if (cin.good()) {
						cin.ignore(10, '\n');
						break;
					}
					cin.clear();
					cin.ignore(10, '\n');
				}
				while (true)
				{
					cout << "Введите продолжительность(минуты) ";
					cin.unsetf(ios::skipws);
					cin >> duration_minute;
					if (duration_minute > 60 || duration_minute < 0) {
						cout << "Неправильный ввод данных\n";
						cin.clear(ios::failbit);
					}
					if (cin.good()) {
						cin.ignore(10, '\n');
						break;
					}
					cin.clear();
					cin.ignore(10, '\n');
				}
			}
				break;
			case 3:
				while (true)
				{
					cout << "Введите  количество занятий ";
					cin.unsetf(ios::skipws);
					cin >> kol_training;
					if (kol_training < 1) {
						cout << "Неправильный ввод данных\n";
						cin.clear(ios::failbit);
					}
					if (cin.good()) {
						cin.ignore(10, '\n');
						break;
					}
					cin.clear();
					cin.ignore(10, '\n');
				}
				break;
			case 4:
				while (true) {
					cout << "Введие цену ";
					cin.unsetf(ios::skipws);
					cin >> price;
					if (price >= 999.0 || price < 0.0) {
						cin.clear(ios::failbit);
					}
					if (cin.good()) {
						cin.ignore(10, '\n');
						break;
					}
					cin.clear();
					cout << "Неправильный ввод данных\n";
					cin.ignore(10, '\n');
				}
				break;
			case 0: return;
			}
		}
	}
	void add_serv()
	{
		//cout << "Введите название услуги "; getline(cin, name);
		//cin.ignore(10, '\n');
		cout << "Введите полезные свойства "; getline(cin, profit);
		//cin.ignore(10, '\n');
		while (true)
		{
			cout << "Введите продолжительность(часы) ";
			cin.unsetf(ios::skipws);
			cin >> duration_hours;
			if (duration_hours > 10 || duration_hours < 0) {
				cout << "Неправильный ввод данных\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		while (true)
		{
			cout << "Введите продолжительность(минуты) ";
			cin.unsetf(ios::skipws);
			cin >> duration_minute;
			if (duration_minute > 60 || duration_minute < 0) {
				cout << "Неправильный ввод данных\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		while (true)
		{
			cout << "Введите  количество занятий ";
			cin.unsetf(ios::skipws);
			cin >> kol_training;
			if (kol_training < 1) {
				cout << "Неправильный ввод данных\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		while (true) {
			cout << "Введие цену "; 
			cin.unsetf(ios::skipws);
			cin >> price;
			if (price >= 999.0 || price < 0.0) {
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cout << "Неправильный ввод данных\n";
			cin.ignore(10, '\n');
		}
	}
	map<int, InfoService> read_serv(SOCKET s2);
	void show_serv(SOCKET s2)
	{
		SClient client;
		InfoService serv;
		int count, key, ans;
		ans = atoi(client.receive(s2).c_str());
		if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
		else serv.read_serv(s2);
	}

	friend ostream &operator<<(ostream &stream, const InfoService& obj)
	{
		stream << "|" << setw(28) << setfill(' ') << obj.name << "|" << setw(27) << obj.profit << "|" << setw(13) << obj.duration_hours*60 + obj.duration_minute << "мин |" << setw(10) << obj.kol_training << "|" << setw(14) << obj.price;
		return stream;
	}
	friend istream &operator>>(istream &stream, InfoService& obj)
	{
		char buff[256];

		stream.getline(buff, 256, '.');
		obj.name = buff;
		stream.getline(buff, 256, '.');
		obj.profit = buff;
		stream.getline(buff, 256, '.');
		obj.duration_hours = atoi(buff);
		stream.getline(buff, 256, '.');
		obj.duration_minute = atoi(buff);
		stream.getline(buff, 256, '.');
		obj.kol_training = atoi(buff);
		stream.getline(buff, 256, '.');
		obj.price = atof(buff);
		return stream;
	}
};
map<int, InfoService> InfoService::read_serv(SOCKET s2)
{
	InfoService serv;
	SClient client;
	map<int, InfoService> m;
	char buf[100], str[100];
	int key;
	int count = atoi(client.receive(s2).c_str());
	for (int i = 0; i < count; i++)
	{
		key = atoi(client.receive(s2).c_str());
		serv.receive_serv(s2);
		m.insert(pair<int, InfoService>(key, serv));
	}
	map<int, InfoService>::iterator mapPtr;
	cout << " __________________________________________________________________________________________________________" << endl;
	cout << "|     |                            |                           |Продолжительность|Количество|   Стоимость  |" << endl;
	cout << "|  №  |          Название          |     Полезные свойства     | (одно занятие)  | занятий  | (в бел. руб) |" << endl;
	cout << "|_____|____________________________|___________________________|_________________|__________|______________|" << endl;

	for (mapPtr = m.begin(); mapPtr != m.end(); mapPtr++)
	{
		cout << "|" << setw(5) << setfill(' ') << mapPtr->first << mapPtr->second << "|" << endl;
	}
	cout << "|_____|____________________________|___________________________|_________________|__________|______________|" << endl;
	return m;
}

class FIT
{
	InfoService *serv = new InfoService;
	entrance<char> user;
	SClient client;
	string view;
	int num_aim;
	int scale;
	vector<int> marks;
public:
	void display(int flag, int key) {
		if (flag == 0)
		{
			cout << " _________________________________________________________________________________________________________________________________________" << endl;
			cout << "|        |                           |                            |                           |Продолжительность|Количество|   Стоимость  |" << endl;
			cout << "|  Ключ  |        Направление        |          Название          |     Полезные свойства     | (одно занятие)  | занятий  | (в бел. руб) |" << endl;
			cout << "|________|___________________________|____________________________|___________________________|_________________|__________|______________|" << endl;
		}
		else {
			cout << "|" << setw(7) << setfill(' ') << key << " |" << setw(27) << view << serv[0] << "|" << endl;
			for (int i = 1; i < num_aim; i++)
			{
				cout << "|" << setw(9) << setfill(' ') << " |" << setw(28) << serv[i] << "|" << endl;
			}
			cout << "|________|___________________________|____________________________|___________________________|_________________|__________|______________|" << endl;
		}
	}
	void displaymas()
	{
		for (int i = 0; i < (num_aim + 1); i++)
			cout << " ______";
		cout << endl;
		for (int i = 0; i < (num_aim + 1); i++)
		{
			if (i == 0)cout << "|      |";
			else cout << setw(4) << setfill(' ') << "Z" << i << " |";
		}
		cout << endl;
		for (int i = 0; i < (num_aim + 1); i++)
			cout << "|______";
		cout << "|" << endl;

		for (int i = 0; i < (num_aim*num_aim); i++)
		{
			if (i == 0 || (i % num_aim == 0))
				cout << "|" << setw(5) << setfill(' ') << "Z" << (i / num_aim) + 1;

			cout << "|" << setw(3) << setfill(' ') << marks[i] << "/" << scale;

			int k = i + 1;
			if (k % num_aim == 0)
			{
				cout << "|" << endl;
				for (int j = 0; j < (num_aim + 1); j++)
					cout << "|______";
				cout << "|" << endl;
			}
		}
	}

	void send_aim(SOCKET s2)
	{
		SClient client;
		InfoService serv;
		int num_aim, sc;
		char buf[100];
		int ans = -1;
		ans = atoi(client.receive(s2).c_str());
		if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
		else if (ans == 1)
		{
			map<int, InfoService> m = serv.read_serv(s2);
			map<int, InfoService>::iterator mapPtr;
			cout << "Введите данные о новом исследовании." << endl;
			cout << "Введите направление исследования ";
			getline(cin, view);
			strcpy(buf, view.c_str());
			client.send_str(s2, buf);
			//cin.ignore(10, '\n');
			while (true)
			{
				cout << "Введите количество целей ";
				cin.unsetf(ios::skipws);
				cin >> num_aim;
				if (num_aim > m.size() || num_aim <= 1) {
					if (num_aim <= 1)
					cout << "Количество целей не может быть меньше двух\n";
					else cout << "Неправильный ввод данных\n";
					cin.clear(ios::failbit);
				}
				if (cin.good()) {
					cin.ignore(10, '\n');
					break;
				}
				cin.clear();
				cin.ignore(10, '\n');
			}
			client.send_int(s2, num_aim);
			while (true)
			{
				cout << "Введите шкалу для оценок ";
				cin.unsetf(ios::skipws);
				cin >> sc;
				if (sc < 3 || sc > 100) {
					cout << "Неправильный ввод данных\n";
					cin.clear(ios::failbit);
				}
				if (cin.good()) {
					cin.ignore(10, '\n');
					break;
				}
				cin.clear();
				cin.ignore(10, '\n');
			}
			client.send_int(s2, sc);
			//int key;
			int *aim = new int[num_aim];
			for (int i = 0; i < num_aim; i++)
			{
				int id = 0;
				while (true)
				{
					cout << "Введите № ";
					cin.unsetf(ios::skipws);
					cin >> aim[i];
					mapPtr = m.find(aim[i]);
					if (!(mapPtr != m.end()))
					{
						cout << "Такого № нету. Попробуйте еще раз\n";
						cin.clear(ios::failbit);
					}
					else
					{
						for (int j = 0; j < i; j++)
						{
							if (aim[i] == aim[j])
							{
								cout << "Эта услуга уже добавлена в исследование. Попробуйте еще раз\n";
								cin.clear(ios::failbit);
							}
						}
					}
					if (cin.good()) {
						cin.ignore(10, '\n');
						break;
					}
					cin.clear();
					cin.ignore(10, '\n');
				}
			}
			for (int i = 0; i < num_aim; i++)
			{
				//cout << "aim[" << i + 1 << "] " << aim[i] << endl;
				client.send_int(s2, aim[i]);
			}
		}
	}
	void receive_aim(SOCKET s2)
	{
		SClient client;
		char buf[100];
		view = client.receive(s2);
		//cout << "VIEW " << view << endl;
		num_aim = atoi(client.receive_int(s2).c_str());
		//cout << "NUM_AIM " << num_aim << endl;
		scale = atoi(client.receive_int(s2).c_str());
		//cout << "SCALE " << scale << endl;
		serv = new InfoService[num_aim];
		for (int i = 0; i < num_aim; i++)
		{
			serv[i].receive_serv(s2);
		}
	}
	map<int, FIT> show_aim(SOCKET s2);

	void send_mas(SOCKET s2)
	{
		SClient client;
		vector<vector<int>> vect(num_aim);
		cout << "Оцените насколько: " << endl;
		for (int i = 0; i < num_aim; i++)
		{
			vect[i].resize(num_aim);
			for (int j = 0; j < num_aim; j++)
			{
				if (i < j)
				{
					//cin.ignore(10, '\n');
					while (true)
					{
						cout << serv[i].getname() << " предпочтительнее " << serv[j].getname() << "(от 0 до " << scale << ") ";
						cin.unsetf(ios::skipws);
						cin >> vect[i][j];
						if (vect[i][j]<0 || vect[i][j]>scale)
						{
							cout << "Оценка не должна выходить из заданного диапозона. Попробуйте еще раз\n";
							cin.clear(ios::failbit);
						}
						if (cin.good()) {
							cin.ignore(10, '\n');
							break;
						}
						cin.clear();
						cin.ignore(10, '\n');
					}

					//cout << "AIM_" << i << " ->aim_" << j;
					//int x;
					//cin >> vect[i][j];
				}
				else if (i > j) vect[i][j] = scale - vect[j][i];
				if (i == j) vect[i][j] = 0;
			}
		}
		client.send_str(s2, user.log);
		client.send_int(s2, num_aim);
		client.send_int(s2, scale);
		for (int i = 0; i < num_aim; i++)
		{
			for (int j = 0; j < num_aim; j++)
			{
				client.send_int(s2, vect[i][j]);
			}
		}
	}
	void receive_mas(SOCKET s2)
	{
		SClient client;
		marks.clear();
		for (int i = 0; i < (num_aim * num_aim); i++)
			marks.push_back(atoi(client.receive_int(s2).c_str()));
	}

	void search_key(SOCKET s2)
	{
		FIT fit;
		SClient client;
		int key;
		map<int, FIT> m = fit.show_aim(s2);
		map<int, FIT>::iterator mapPtr;
		while (true)
		{
			cout << "Введите ключ ";
			cin.unsetf(ios::skipws);
			cin >> key;  //cout << "Key" << key << endl;
			mapPtr = m.find(key);
			if (!(mapPtr != m.end()))
			{
				cout << "Такого ключа нету. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		} //cout << "Key" << key << endl;
		mapPtr = m.find(key);
		if (mapPtr != m.end())
		{
			client.send_int(s2, (*mapPtr).first);
			//cout << "Key" << key << endl;
			num_aim = (*mapPtr).second.num_aim;
			scale = (*mapPtr).second.scale;
		}
	}
	void show(SOCKET);
	void del(SOCKET);
	void add(SOCKET);
	void edit(SOCKET);
	void show_mas(SOCKET);
	void del_mas(SOCKET);
	void edit_mas(SOCKET);
	void solution(SOCKET);
	void order(SOCKET);
	void show_ord(SOCKET);
	void top(SOCKET);
};

map<int, FIT> FIT::show_aim(SOCKET s2)
{
	FIT fit;
	SClient client;
	map<int, FIT> m;
	char buf[100], str[100];
	int key;
	int count = atoi(client.receive(s2).c_str());
	for (int i = 0; i < count; i++)
	{
		key = atoi(client.receive(s2).c_str());
		fit.receive_aim(s2);
		m.insert(pair<int, FIT>(key, fit));
	}
	map<int, FIT>::iterator mapPtr;
	fit.display(0, 0);
	for (mapPtr = m.begin(); mapPtr != m.end(); mapPtr++)
	{
		(*mapPtr).second.display(1, (*mapPtr).first);
	}
	return m;
}
void FIT::show(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		fit.search_key(s2);
		ans = -1;
		ans = atoi(client.receive(s2).c_str());
		if (ans == 0) cout << "Оценок в файле не обнаружено." << endl;
		else if (ans == 1)
		{
			count = atoi(client.receive(s2).c_str());
			if (count == 0) cout << "Оценок по данным не обнаружено." << endl;
			for (int j = 0; j < count; j++)
			{
				fit.receive_mas(s2);
				fit.displaymas();
			}
		}

	}
}
void FIT::del(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		fit.search_key(s2);
		cout << "Удаление прошло успешно." << endl;
		ans = atoi(client.receive(s2).c_str());
	}
}
void FIT::add(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		char buf[100];
		SClient client;
		map<int, FIT> m = fit.show_aim(s2);
		map<int, FIT>::iterator mapPtr;
		while (true)
		{
			cout << "Введите ключ ";
			cin.unsetf(ios::skipws);
			cin >> key;// cout << "Key" << key << endl;
			mapPtr = m.find(key);
			if (!(mapPtr != m.end()))
			{
				cout << "Такого ключа нету. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		mapPtr = m.find(key);
		if (mapPtr != m.end())
		{
			client.send_int(s2, (*mapPtr).first);
			// << "Key" << key << endl;
			client.send_str(s2, user.log);
			ans = -1;
			ans = atoi(client.receive(s2).c_str());
			//cout << "ANS= " << ans << endl;
			if (ans == 2) cout << "Вы уже добавляли оценки по этому исследованию." << endl;
			else if (ans == 1)
				(*mapPtr).second.send_mas(s2);
		}
	}
}
void FIT::edit(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		InfoService serv;
		SClient client;
		map<int, InfoService> m = serv.read_serv(s2);
		map<int, InfoService>::iterator mapPtr;
		int id = 0;
		while (true)
		{
			cout << "Введите № ";
			cin.unsetf(ios::skipws);
			cin >> id;
			mapPtr = m.find(id);
			if (!(mapPtr != m.end()))
			{
				cout << "Такого № нету. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}	
		mapPtr = m.find(id);
		if (mapPtr != m.end())
		{
			client.send_int(s2, (*mapPtr).first);
			mapPtr->second.edit_serv();
			mapPtr->second.send_serv(s2, mapPtr->second, 0);
		}
		
		cout << "Изменение прошло успешно." << endl;
	}
}
void FIT::show_mas(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		char buf[100];
		SClient client;
		map<int, FIT> m = fit.show_aim(s2);
		map<int, FIT>::iterator mapPtr;
		while (true)
		{
			cout << "Введите ключ ";
			cin.unsetf(ios::skipws);
			cin >> key;
			mapPtr = m.find(key);
			if (!(mapPtr != m.end()))
			{
				cout << "Такого ключа нету. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		mapPtr = m.find(key);
		if (mapPtr != m.end())
		{
			client.send_int(s2, (*mapPtr).first);
			client.send_str(s2, user.log);
			ans = -1;
			ans = atoi(client.receive(s2).c_str());
			if (ans == 0) cout << "Вы не добавляли оценки по этому исследованию." << endl;
			else if (ans == 1)
			{
				(*mapPtr).second.receive_mas(s2);
				(*mapPtr).second.displaymas();
			}
		}
	}
}
void FIT::del_mas(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		char buf[100];
		SClient client;
		map<int, FIT> m = fit.show_aim(s2);
		map<int, FIT>::iterator mapPtr;
		while (true)
		{
			cout << "Введите ключ ";
			cin.unsetf(ios::skipws);
			cin >> key;
			mapPtr = m.find(key);
			if (!(mapPtr != m.end()))
			{
				cout << "Такого ключа нету. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		mapPtr = m.find(key);
		if (mapPtr != m.end())
		{
			client.send_int(s2, (*mapPtr).first);
			client.send_str(s2, user.log);
			ans = -1;
			ans = atoi(client.receive(s2).c_str());
			if (ans == 0) cout << "Вы не добавляли оценки по этому исследованию." << endl;
			else if (ans == 1)
			{
				cout << "Удаление прошло успешно." << endl;
			}
		}
	}
}
void FIT::edit_mas(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		char buf[100];
		SClient client;
		map<int, FIT> m = fit.show_aim(s2);
		map<int, FIT>::iterator mapPtr;
		while (true)
		{
			cout << "Введите ключ ";
			cin.unsetf(ios::skipws);
			cin >> key;
			mapPtr = m.find(key);
			if (!(mapPtr != m.end()))
			{
				cout << "Такого ключа нету. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		mapPtr = m.find(key);
		if (mapPtr != m.end())
		{
			client.send_int(s2, (*mapPtr).first);
			client.send_str(s2, user.log);
			ans = -1;
			ans = atoi(client.receive(s2).c_str());
			if (ans == 0) cout << "Вы не добавляли оценки по этому исследованию." << endl;
			else if (ans == 1)
			{
				mapPtr->second.send_mas(s2);
				cout << "Редактирование прошло успешно." << endl;
			}
		}
	}
}
void FIT::solution(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		fit.search_key(s2);
		string *aim = new string[fit.num_aim];
		for (int i = 0; i < fit.num_aim; i++)
		{
			char str[100];
			if (recv(s2, str, sizeof(str), 0) != 0)
			{
				aim[i] = str;
			}
		}
		count = atoi(client.receive(s2).c_str());
		if (count == 0) cout << "Оценок по данным не обнаружено." << endl;
		else if (count == 1)
		{
			string *W = new string[fit.num_aim];
			int retval;
			for (int i = 0; i < fit.num_aim; i++)
			{
				char str[100];
				if (recv(s2, str, sizeof(str), 0) != 0)
				{
					W[i] = str;
				}
				cout << aim[i] << " --- " << W[i] << endl;
			}
			int maxi = atoi(client.receive(s2).c_str());
			cout << "ИТОГО: " << aim[maxi] << " - " << W[maxi] << endl;
		}
	}
}
void FIT::order(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if (ans == 0) cout << "Данных в файле не обнаружено." << endl;
	else if (ans == 1)
	{
		InfoService serv;
		SClient client;
		map<int, InfoService> m = serv.read_serv(s2);
		map<int, InfoService>::iterator mapPtr;
		int id = 0;
		while (true)
		{
			cout << "Введите № ";
			cin.unsetf(ios::skipws);
			cin >> id;
			mapPtr = m.find(id);
			if (!(mapPtr != m.end()))
			{
				cout << "Такого № нету. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		mapPtr = m.find(id);
		if (mapPtr != m.end())
		{
			client.send_int(s2, (*mapPtr).first);
			client.send_str(s2, user.log);
			ans = -1;
			ans = atoi(client.receive(s2).c_str());
			if (ans == 2) cout << "Вы уже заказывали эту услугу." << endl;
		}
	}
}
void FIT::show_ord(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	if(ans==0) cout << "Вы не заказывали услуги." << endl;
	else if (ans == 1)
	{
		client.send_str(s2, user.log);
		ans = -1;
		ans = atoi(client.receive(s2).c_str());
		if (ans == 0) cout << "Вы не заказывали услуги." << endl; 
		else 
		{
			cout << "Услуги, которые вы заказывали" << endl;
			cout << " ____________________________________________________________________________________________________" << endl;
			cout << "|                            |                           |Продолжительность|Количество|   Стоимость  |" << endl;
			cout << "|          Название          |     Полезные свойства     | (одно занятие)  | занятий  | (в бел. руб) |" << endl;
			cout << "|____________________________|___________________________|_________________|__________|______________|" << endl;

			InfoService serv;
			for (int i = 0; i < ans; i++)
			{
				serv.receive_serv(s2);
				cout  << serv << "|" << endl;
			}
			cout << "|____________________________|___________________________|_________________|__________|______________|" << endl;

		}
	}
}
void FIT::top(SOCKET s2)
{
	FIT fit;
	int count, key, ans;
	ans = atoi(client.receive(s2).c_str());
	//cout << "ANS " << ans << endl;
	if (ans == 0) cout << "Заказов нет." << endl;
	else 
	{
		cout << "Услуги, которые заказывают чаще всего" << endl;
		cout << " _____________________________________________________________________________________________________________________" << endl;
		cout << "|     |                            |                           |Продолжительность|Количество|   Стоимость  |Количество|" << endl;
		cout << "|  №  |          Название          |     Полезные свойства     | (одно занятие)  | занятий  | (в бел. руб) | заказов  |" << endl;
		cout << "|_____|____________________________|___________________________|_________________|__________|______________|__________|" << endl;

		InfoService serv;
		for (int i = 0; i < ans; i++)
		{
			int num = atoi(client.receive(s2).c_str());
			int id = 0;
			if (num > 0)
			{
			    id = atoi(client.receive(s2).c_str());
				serv.receive_serv(s2);
			}
			if (num > 0 && i < 5)
			{
				cout << "|" << setw(5) << setfill(' ') << id << serv << "|" << setw(10) << num << "|" << endl;
				cout << "|_____|____________________________|___________________________|_________________|__________|______________|__________|" << endl;
			}
		}
	}
}

int menu_admin(SOCKET s2)
{
	int flag = 0;
	FIT menu;
	InfoService serv;
	SClient client;
	while (1) {
		system("cls");
		cout << "_______________АДМИНИСТРАТОР________________" << endl;
		cout << "1. Просмотр всех услуг\n";
		cout << "2. Добавить услугу\n";
		cout << "3. Изменить характеристики услуги\n";
		cout << "4. Выбор услуг для исследования\n";
		cout << "5. Просмотр существующих исследований\n";
		cout << "6. Удалить исследование\n";
		cout << "7. Сгенерировать отчет об исследовании\n";
		cout << "8. Просмотр наиболее популярных услуг\n";
		cout << "9. Управление аккаунтами пользователей\n";
		cout << "0. Выход\n";
		//scanf_s("%d", &flag);
		while (true)
		{
			cin.unsetf(ios::skipws);
			cin >> flag;
			if (flag >= 10 || flag < 0) {
				cout << "Неверный ввод. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		char buf[100];
		switch (flag) {
		case 1:
			system("cls");
			strcpy(buf, "showserv");
			send(s2, buf, sizeof(buf), 0);
			serv.show_serv(s2);
			getchar();
			break;
		case 2:
			system("cls");
			strcpy(buf, "addserv");
			send(s2, buf, sizeof(buf), 0);
			serv.send_serv(s2, serv, 1);
			getchar();
			break;
		case 3:
			system("cls");
			strcpy(buf, "editserv");
			send(s2, buf, sizeof(buf), 0);
			menu.edit(s2);
			getchar();
			break;
		case 4:
			system("cls");
			strcpy(buf, "create");
			send(s2, buf, sizeof(buf), 0);
			menu.send_aim(s2);
			getchar();
			break;
		case 5:
			system("cls");
			strcpy(buf, "show");
			send(s2, buf, sizeof(buf), 0);
			menu.show(s2);
			getchar();
			break;
		case 6:
			system("cls");
			strcpy(buf, "del");
			send(s2, buf, sizeof(buf), 0);
			menu.del(s2);
			getchar();
			break;
		case 7:
			system("cls");
			strcpy(buf, "solution");
			send(s2, buf, sizeof(buf), 0);
			menu.solution(s2);
			getchar();
			break;
		case 8:
			system("cls");
			strcpy(buf, "top");
			send(s2, buf, sizeof(buf), 0);
			menu.top(s2);
			getchar();
			break;
		case 9:
			edituser();
			break;
		case 0: return 0;
		}
	}
}
int menu_user(SOCKET s2)
{
	int flag = 0;
	FIT menu;
	SClient client;
	InfoService serv;
	while (1) {
		system("cls");
		cout << "____________ПОЛЬЗОВАТЕЛЬ___________" << endl;
		cout << "1. Просмотр всех услуг\n";
		cout << "2. Выстасить оценки по исследованию\n";
		cout << "3. Просмотр всех своих оценок\n";
		cout << "4. Редактирование своих оценок\n";
		cout << "5. Удаление своих оценок\n";
		cout << "6. Заказать услугу\n";
		cout << "7. Просмотр всех своих заказов\n";
		cout << "0. Выход\n";
		//cin >> flag;
		//scanf_s("%d", &flag); 
		while (true)
		{
			cin.unsetf(ios::skipws);
			cin >> flag;
			if (flag >=8 || flag < 0) {
				cout << "Неверный ввод. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		char buf[100];
		switch (flag) {
		case 1:
			system("cls");
			strcpy(buf, "showserv");
			send(s2, buf, sizeof(buf), 0);
			serv.show_serv(s2);
			getchar();
			break;
		case 2:
			system("cls");
			strcpy(buf, "add");////////////
			send(s2, buf, sizeof(buf), 0);
			menu.add(s2);
			getchar();
			break;
		case 3:
			system("cls");
			{
				strcpy(buf, "show_mas");///////////
				send(s2, buf, sizeof(buf), 0);
				menu.show_mas(s2);
			}
			getchar();
			break;
		case 4:
			system("cls");
			strcpy(buf, "edit_mas");/////////////
			send(s2, buf, sizeof(buf), 0);
			menu.edit_mas(s2);
			getchar();
			break;
		case 5:
			system("cls");
			strcpy(buf, "del_mas");//////////
			send(s2, buf, sizeof(buf), 0);
			menu.del_mas(s2);
			getchar();
			break;
		case 6:
			system("cls");
			strcpy(buf, "order");//////////
			send(s2, buf, sizeof(buf), 0);
			menu.order(s2);
			getchar();
			break;
		case 7:
			system("cls");
			strcpy(buf, "showord");//////////
			send(s2, buf, sizeof(buf), 0);
			menu.show_ord(s2);
			getchar();
			break;
		case 0: return 0;
		}
	}
}

int menu()
{
	int flag = 0;
	FIT menu;
	entrance<char> user;
	SClient client;
	while (1) {
		system("cls");
		cout << "1. Вход под именем администратора\n";
		cout << "2. Вход под именем пользователя\n";
		cout << "3. Зарегестрироваться\n";
		cout << "0. Выход\n";
		//cin >> flag;
		//scanf_s("%d", &flag);
		while (true)
		{
			cin.unsetf(ios::skipws);
			cin >> flag;
			if (flag >= 4 || flag < 0) {
				cout << "Неверный ввод. Попробуйте еще раз\n";
				cin.clear(ios::failbit);
			}
			if (cin.good()) {
				cin.ignore(10, '\n');
				break;
			}
			cin.clear();
			cin.ignore(10, '\n');
		}
		char buf[100];
		switch (flag) {
		case 1:
		{
			if (user.loginadmin() == 0)
			{
				SClient client;
				SOCKET s2 = client.start();
				menu_admin(s2);
				closesocket(s2);
			}
			else
			{
				cout << "Неверный логин или пароль!\n";
				getchar();
			}
		}
		//menu_admin(s2);
		break;
		case 2:
		{
			if (user.loginuser() == 0)
			{
				//user.getlog();
				SClient client;
				SOCKET s2 = client.start();
				menu_user(s2);
				closesocket(s2);
			}
			else
			{
				cout << "Неверный логин или пароль!\n";
				getchar();
			}
		}
		//menu_user(s2);
		break;
		case 3:
			system("cls");
			entrance<char> temp;
			if (temp.adduser() == 0)
			{
				printf("Регистрация прошла успешна!\n");
			}
			else
			{
				printf("Ошибка!\n");
			}
			getchar();
			break;
			break;
		case 0: return 0;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	menu();
	WSACleanup();
	system("pause");
	return 0;
}