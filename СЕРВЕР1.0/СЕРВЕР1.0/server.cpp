#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <winsock2.h>
#include <winsock.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#pragma comment(lib,"Wsock32.lib")
using namespace std;
int main();
class MyException
{
private:
	std::string m_error;

public:
	MyException(std::string error): m_error(error){}

	const char* getError() { return m_error.c_str(); }
	const char* newHandler() {
		throw bad_alloc();
		m_error = "ошибка выделения памяти";
		return m_error.c_str();
	}
};

template<typename T>
class Container
{
	T *marks;
	int num;
public:
	class Iterator;
	Container(int n = 0) :num(n) { marks = new T[n]; }
	T& operator[] (const int& n)
	{
		if (n > 0 && n < num)
			return marks[n];
		return marks[0];

	}
	void push_back(T data)
	{
		try
		{
			T *_result = new T[this->num++];
		for (int index = 0; index < this->num; index++)
		{
			if (index != this->num - 1)
			{
				_result[index] = this->marks[index];
			}
			else
			{
				_result[index] = data;
				break;
			}
		}
		//delete[] this->marks;
		this->marks = _result;
		}
		catch (MyException &exception)
		{
			cout << "Вызвано исключение (" << exception.getError() << ")" << endl;
		}
	}
	const T &operator[](int nIndex) const
	{
		if(nIndex<0||size()<=nIndex ) 
			throw MyException("ошибка создания контейнера");
		return marks[nIndex];
	}
	int &size() { return num; }
	Iterator begin() { return marks; }
	Iterator end() { return marks + num; }
	friend ostream &operator<<(ostream &stream, const Container<T>& obj)
	{
		for (int i = 0; i < obj.num; i++)
		{
			stream << obj.marks[i] << " ";
		}
		//stream << Container<T>::marks[obj];
		return stream;
	}
	class Iterator
	{
		T* cur;
	public:
		Iterator(T* first) :cur(first) {};

		T& operator+ (int n) { return *(cur + n); }
		T& operator- (int n) { return *(cur - n); }

		T& operator++ (int) { return *cur++; }
		T& operator-- (int) { return *cur--; }
		T& operator++ () { return *++cur; }
		T& operator-- () { return *--cur; }

		bool operator!= (const Iterator& it) { return cur != it.cur; }
		bool operator== (const Iterator& it) { return cur == it.cur; }

		T& operator* () { return *cur; }
	};
};

long getFileSize(fstream *base) {
	long fileSizeBytes;
	base->seekg(0, ios::end);
	fileSizeBytes = base->tellg();
	base->seekg(0, ios::beg);
	return fileSizeBytes;
}

class SServer
{
public:
	SServer() {};
	~SServer() {};

	void startServer();
	unsigned short port;
	SOCKET s;//
	WSAData wData;
	string receive(SOCKET s2)
	{
		char buf[100];
		int result = 0;
		result = (recv(s2, buf, sizeof(buf), 0));
		if (result < 0)
		{
			closesocket(s2);
			main();
		}
		else
			buf[strlen(buf)] = '\0';
		return buf;
	}
	void send_int(SOCKET s2, int k)
	{
		try {
			if (sizeof(k) > 100)
				throw length_error("Ошибка отправки данных");
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
			if (sizeof(s) > 100)
				throw length_error("Ошибка отправки данных");
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
void SServer::startServer() {
	if (WSAStartup(MAKEWORD(2, 2), &wData) != 0)
	{
		//printf("WSA Startup succes\n");
		return;
	}
	s = socket(AF_INET, SOCK_STREAM, NULL);
	sockaddr_in local_addr;//
	local_addr.sin_family = AF_INET;//
	local_addr.sin_port = htons(1280);//
	local_addr.sin_addr.s_addr = 0;//

	if (s == SOCKET_ERROR) {
		printf("Socket not created\n");
		return;
	}

	if (bind(s, (struct sockaddr*)&local_addr, sizeof(local_addr)) != SOCKET_ERROR) {
		//printf("Socket succed binded\n");
	}

	if (listen(s, SOMAXCONN) != SOCKET_ERROR) {
		//printf("Start listenin at port%u\n", ntohs(local_addr.sin_port));
	}
}

class InfoService
{
protected:
	string name;
	float price;
	string profit;
	int kol_training;
	int duration_hours;
	int duration_minute;
public:
	string getname() { return name; }
	string getprofit() { return profit; }
	int gethours() { return duration_hours; }
	int getminute() { return duration_minute; }
	int getkol() { return kol_training; }
	string getprice() {
		char buff[100];
		sprintf(buff, "%g", price);
		return buff;
	}

	void receive_serv(SOCKET s2, string str)
	{
		SServer server;
		name = str;
		profit = server.receive(s2).c_str();
		duration_hours = atoi(server.receive(s2).c_str());
		duration_minute = atoi(server.receive(s2).c_str());
		kol_training = atoi(server.receive(s2).c_str());
		price = atof(server.receive(s2).c_str());
	}
	void send_serv(SOCKET s2)
	{
		char buff[100];
		SServer server;

		strcpy(buff, name.c_str());
		server.send_str(s2, buff);
		strcpy(buff, profit.c_str());
		server.send_str(s2, buff);
		server.send_int(s2, duration_hours);
		server.send_int(s2, duration_minute);
		server.send_int(s2, kol_training);
		sprintf(buff, "%g", price);
		server.send_str(s2, buff);
		return;
	}

	int write_serv(SOCKET);
	map<int, InfoService> show_serv(SOCKET);
	map<int, InfoService> read_serv(SOCKET);
	void edit_serv(SOCKET s2)
	{
		InfoService serv;
		SServer server;
		map<int, InfoService> m = serv.show_serv(s2);
		if (!m.empty())
		{
			int id = atoi(server.receive(s2).c_str());
			map<int, InfoService>::iterator mapPtr;
			mapPtr = m.find(id);
			if (mapPtr != m.end())
			{
				string name = server.receive(s2).c_str();
				mapPtr->second.receive_serv(s2, name);
				fstream file;
				file.open("InfoService.txt", ios::out | ios::binary);
				for (mapPtr = m.begin(); mapPtr != m.end(); mapPtr++)
				{
					file << (*mapPtr).first << "." << (*mapPtr).second << endl;
				}
				file.close();
				file.clear();
			}
		}
	}


	friend ostream &operator<<(ostream &stream, const InfoService& obj)
	{
		stream << obj.name << ".";
		stream << obj.profit << ".";
		stream << obj.duration_hours << ".";
		stream << obj.duration_minute << ".";
		stream << obj.kol_training << ".";
		stream << obj.price << ".";
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
		obj.price = atoi(buff);
		return stream;
	}
};

map<int, InfoService> InfoService::show_serv(SOCKET s2)
{
	SServer server;
	map<int, InfoService> m;
	fstream file;
	file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		server.send_int(s2, 0);
		return m;
	}
	else
	{
		if (getFileSize(&file) == 0)
		{
			server.send_int(s2, 0);
			return m;
		}
		else
		{
			server.send_int(s2, 1);
			InfoService *obj;
			int key = 0, count = 0, i = 0;
			char s[256];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			int n = i;
			//cout << "COUNT " << count << endl;
			file.close();
			obj = new InfoService[count];

			file.clear();
			char buff[100];
			file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
			server.send_int(s2, n);
			for (int i = 0; i < count; i++)
			{
				file.getline(buff, 100, '.');
				server.send_str(s2, buff);
				key = atoi(buff);
				file >> obj[i];
				obj[i].send_serv(s2);
				m.insert(pair<int, InfoService>(key, obj[i]));
			}
			return m;
		}
	}
}
map<int, InfoService> InfoService::read_serv(SOCKET s2)
{
	SServer server;
	map<int, InfoService> m;
	fstream file;
	file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
	InfoService *obj;
	int key = 0, count = 0, i = 0;
	char s[256];
	file.seekg(0, ios::beg);
	while (1)
	{
		file.getline(s, 1024, '\n');
		if (file.fail()) break;
		i++;
	}
	count = i;
	int n = i;
	file.close();
	obj = new InfoService[count];

	file.clear();
	char buff[100];
	file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
	//server.send_int(s2, n);
	for (int i = 0; i < count; i++)
	{
		file.getline(buff, 100, '.');
		//server.send_str(s2, buff);
		key = atoi(buff);
		file >> obj[i];
		//obj[i].send_serv(s2);
		m.insert(pair<int, InfoService>(key, obj[i]));
	}
	map<int, InfoService>::iterator mapPtr;
	//for (mapPtr = m.begin(); mapPtr != m.end(); mapPtr++)
		//cout << mapPtr->first << " " << mapPtr->second << endl;
	//cout << "m.size " << m.size() << endl;
	return m;


}
int InfoService::write_serv(SOCKET s2)
{
	fstream file;
	SServer server;
	string name;
	file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		return 1;
	}
	else
	{
		char buf[100];
		long file_size;
		file.seekg(0, ios::end);
		file_size = file.tellg();
		int count = 0;
		name = server.receive(s2).c_str();
		if (file_size != 0)
		{
			map<string, InfoService> m2;
			InfoService *obj;
			int  i = 0, key = 0;
			char s[256];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			file.close();
			try {
				obj = new InfoService[count];
				file.clear();
				char buff[100];
				file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
				for (int i = 0; i < count; i++)
				{
					file.getline(buff, 100, '.');
					key = atoi(buff);
					file >> obj[i];
					m2.insert(pair<string, InfoService>(obj[i].getname(), obj[i]));
				}
			}
			catch (MyException &exception)
			{
				cout << "Вызвано исключение (" << exception.newHandler() << ")" << endl;
			}

			map<string, InfoService>::iterator mapPtr2;
			while (true)
			{
				mapPtr2 = m2.find(name);
				if (mapPtr2 != m2.end())
				{
					server.send_int(s2, -4);
					name = server.receive(s2).c_str();
				}
				else {
					server.send_int(s2, 4);	break;
				}
			}
		}
		else server.send_int(s2, 4);
		file.close();
		file.clear();
		map<int, InfoService> m;
		InfoService data;
		data.receive_serv(s2, name);
		file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
		file.seekg(0, ios::end);
		file << count + 1 << "." << data << endl;
		return 0;
	}
}

class FitnessCenterWork
{
protected:
	InfoService *serv;
	string view;
	int num_aim;
	int scale;
	int *office;
public:
	InfoService* getobj() { return serv; }
	int getnum_aim() { return num_aim; }
	int getoffice( int num ) {
		return office[num];
	}
	
	void receive_aim(SOCKET s2)
	{
		SServer server;
		view = server.receive(s2).c_str();
		num_aim = atoi(server.receive(s2).c_str());
		scale = atoi(server.receive(s2).c_str());
	}
	int write_aim(SOCKET);

	friend ostream &operator<<(ostream &stream, const FitnessCenterWork& obj)
	{
		stream << obj.view << ".";
		stream << obj.num_aim << ".";
		stream << obj.scale << ".";
		return stream;
	}
	friend istream &operator>>(istream &stream, FitnessCenterWork& obj)
	{
		char buff[256];
		stream.getline(buff, 256, '.');
		obj.view = buff;
		stream.getline(buff, 256, '.');
		obj.num_aim = atoi(buff);
		stream.getline(buff, 256, '.');
		obj.scale = atoi(buff);

		//stream.getline(buff, 256, '\n');
		return stream;
	}

	map<int, FitnessCenterWork> send_aim(SOCKET);
};

int FitnessCenterWork::write_aim(SOCKET s2)
{
	fstream file;
	file.open("Fitness.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		return 1;
	}
	else
	{
		char buf[100];
		long file_size;
		file.seekg(0, ios::end);
		file_size = file.tellg();
		long id = rand() % 8999 + 10000;
		if (file_size != 0)
		{
			map<int, FitnessCenterWork> m2;
			FitnessCenterWork *obj;
			int count = 0, i = 0, key = 0;
			char s[256];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			file.close();
			obj = new FitnessCenterWork[count];
			file.clear();
			char buff[100];
			file.open("Fitness.txt", ios::out | ios::in | ios::app | ios::binary);
			for (int i = 0; i < count; i++)
			{
				file.getline(buff, 100, '.');
				key = atoi(buff);
				file >> obj[i];
				m2.insert(pair<int, FitnessCenterWork>(key, obj[i]));
			}
			map<int, FitnessCenterWork>::iterator mapPtr2;
			while (true)
			{
				mapPtr2 = m2.find(id);
				if (mapPtr2 != m2.end())
				{
					id = rand() % 8999 + 10000;
				}
				else break;
			}
		}
		file.close();
		file.clear();
		InfoService serv;
		map<int, InfoService> m = serv.show_serv(s2);
		if (!m.empty())
		{
			map<int, FitnessCenterWork> m;
			FitnessCenterWork data;
			SServer server;
			data.receive_aim(s2);
			int *aim = new int[data.getnum_aim()];
			file.open("Fitness.txt", ios::out | ios::in | ios::app | ios::binary);
			file.seekg(0, ios::end);
			//cout << id << "." << data;
			file << id << "." << data;
			for (int i = 0; i < data.getnum_aim(); i++)
			{
				aim[i] = atoi(server.receive(s2).c_str());
				//cout << "aim[" << i + 1 << "] " << aim[i] << endl;
				file << aim[i] << ".";
			}
			file << endl;
		}
		return 0;
	}
}
map<int, FitnessCenterWork> FitnessCenterWork::send_aim(SOCKET s2)
{
	SServer server;
	map<int, FitnessCenterWork> m;
	fstream file;
	file.open("Fitness.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		server.send_int(s2, 0);
		return m;
	}
	else
	{
		if (getFileSize(&file) == 0)
		{
			server.send_int(s2, 0);
			return m;
		}
		else
		{
			server.send_int(s2, 1);
			FitnessCenterWork *obj;
			int key = 0, count = 0, i = 0;
			char s[256];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			int n = i;
			file.close();
			obj = new FitnessCenterWork[count];

			file.clear();
			char buff[100];
			InfoService serv1;
			map<int, InfoService> ms = serv1.read_serv(s2);
			//cout << "ms.size() " << ms.size() << endl;
			map<int, InfoService> ::iterator maPs;
			//for (maPs = ms.begin(); maPs != ms.end(); maPs++)
			//	cout << maPs->first << " " << maPs->second << endl;
			file.close();
			file.clear();
			file.open("Fitness.txt", ios::out | ios::in | ios::app | ios::binary);
			server.send_int(s2, n);
			for (int i = 0; i < count; i++)
			{
				file.getline(buff, 100, '.');
				server.send_str(s2, buff);
				key = atoi(buff);
				file >> obj[i];
				//cout << "OBJ[I] " << obj[i] << endl;
				strcpy(buff, obj[i].view.c_str());
				server.send_str(s2, buff);
				server.send_int(s2, obj[i].num_aim);
				server.send_int(s2, obj[i].scale);
				obj[i].serv = new InfoService[obj[i].num_aim];
				obj[i].office = new int[obj[i].num_aim];
				map<int, InfoService> ::iterator maPs;
				for (int j = 0; j < obj[i].num_aim; j++)
				{
					file.getline(buff, 100, '.');
					obj[i].office[j] = atoi(buff);
					//cout << "buf " << buff;
					//cout <<maPs->first<< " maPs->second " << maPs->second << endl;
					maPs = ms.find(obj[i].office[j]);
					if (maPs != ms.end())
					{
						obj[i].serv[j] = maPs->second;
						//cout << " maPs->second " << maPs->second << endl;
						//cout << "obj[i].serv[j] " << obj[i].serv[j] << endl;
					}
					obj[i].serv[j].send_serv(s2);
				}
				m.insert(pair<int, FitnessCenterWork>(key, obj[i]));
			}
			file.close();
			file.clear();
			return m;
		}
	}
}

class FitService :public FitnessCenterWork
{
	Container<int> marks;
	int size;
	string log;
public:

	Container<int> getmarks() { return this->marks; }
	string getlog() { return log; }

	int count(SOCKET, int);
	void receive_mas(SOCKET s2)
	{
		SServer server;
		log = server.receive(s2).c_str();

		num_aim = atoi(server.receive(s2).c_str());
		scale = atoi(server.receive(s2).c_str());
		for (int i = 0; i < (num_aim* num_aim); i++)
		{
			marks.push_back(atoi(server.receive(s2).c_str()));
			//cout << "marks " << marks << endl;
		}
	}
	int send_mas(SOCKET, int);

	int del_mas(SOCKET, int);
	int write_mas(SOCKET s2, int key)
	{
		SServer server;
		fstream file;
		file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
		if (!file.is_open())
		{
			return 1;
		}
		else
		{
			map<int, FitService> m;
			FitService data;
			data.receive_mas(s2);
			m.insert(pair<int, FitService>(key, data));
			map<int, FitService>::iterator mapPtr;
			for (mapPtr = m.begin(); mapPtr != m.end(); mapPtr++)
			{
				file << endl << (*mapPtr).first << "." << (*mapPtr).second;
			}
			return 0;
		}
	}

	multimap<int, FitService> read_mas();

	friend ostream &operator<<(ostream &stream, FitService& obj)
	{
		stream << obj.log << ".";
		int size1 = obj.marks.size();
		stream << obj.marks.size() << ".";
		Container<int>::Iterator iter(obj.marks.begin());
		//stream << iter << ".";
		for (int i = 0; i < obj.marks.size(); i++)
		{
			//stream << obj.marks[i] << ".";
			stream << iter++ << ".";
		}
		//stream << endl;
		return stream;
	}
	friend istream &operator>>(istream &stream, FitService& obj)
	{
		char buff[256];
		stream.getline(buff, 256, '.');
		obj.log = buff;
		//cout << "log " << obj.log << endl;
		stream.getline(buff, 256, '.');
		obj.size = atoi(buff);
		//cout << "obj.size " << obj.size << endl;
		for (int i = 0; i < obj.size; i++)
		{
			stream.getline(buff, 256, '.');
			int ar = atoi(buff);
			obj.marks.push_back(ar);
			//cout << "ar"<<ar << endl;
		}
		//cout << obj.marks << endl;
		stream.getline(buff, 256, '\n');
		return stream;
	}
};

int FitService::send_mas(SOCKET s2, int keyad)
{
	SServer server;
	fstream file;
	file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		server.send_int(s2, 0);
		return 1;
	}
	else
	{
		if (getFileSize(&file) == 0)
		{
			server.send_int(s2, 0);
			return 1;
		}
		else
		{
			server.send_int(s2, 1);
			FitService *obj;
			SServer server;
			multimap<int, FitService> multi;
			int key = 0, count = 0, i = 0;
			char s[256], buff[100];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			file.close();
			file.clear();

			obj = new FitService[count];
			file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
			for (int i = 0; i < count; i++)
			{
				file.getline(buff, 100, '.');
				key = atoi(buff);
			//	cout << "Key" << key << endl;
				file >> obj[i];
				multi.insert(pair<int, FitService>(key, obj[i]));
			}
			file.close();
			file.clear();
			multimap<int, FitService>::iterator mapPtr;
			multimap<int, int>::size_type kol;
			kol = multi.count(keyad);
			server.send_int(s2, kol);
			mapPtr = multi.find(keyad);
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (keyad == (*mapPtr).first)
				{
					for (int i = 0; i < (*mapPtr).second.marks.size(); i++)
					{
						server.send_int(s2, (*mapPtr).second.marks[i]);
					}
					///cout << endl;
				}
			}
			return 0;
		}
	}
}
int FitService::del_mas(SOCKET s2, int keyad)
{
	SServer server;
	fstream file;
	file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		server.send_int(s2, 0);
		return 1;
	}
	else
	{
		if (getFileSize(&file) == 0)
		{
			server.send_int(s2, 0);
			return 1;
		}
		else
		{
			server.send_int(s2, 1);
			FitService *obj;
			SServer server;
			multimap<int, FitService> multi;
			int key = 0, count = 0, i = 0;
			char s[256], buff[100];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			file.close();
			file.clear();

			obj = new FitService[count];
			file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
			for (int i = 0; i < count; i++)
			{
				file.getline(buff, 100, '.');
				key = atoi(buff);
				file >> obj[i];
				multi.insert(pair<int, FitService>(key, obj[i]));
			}
			multimap<int, FitService>::iterator mapPtr;
			char buf[100];
			multimap<int, int>::size_type kol;
			multi.erase(keyad);
			file.close();
			file.clear();
			file.open("FitService.txt", ios::out | ios::binary);
			file.seekg(0, ios::beg);
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				file << (*mapPtr).first << "." << (*mapPtr).second;
				//cout << (*mapPtr).first << "." << (*mapPtr).second;
			}
			file.close();
			file.clear();
			return 0;
		}
	}
}
int FitService::count(SOCKET s2, int keyad)
{
	SServer server;
	fstream file;
	file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		//cout << "Файл не открыт!\n";
		server.send_int(s2, 0);
		return 1;
	}
	else
	{
		if (getFileSize(&file) == 0)
		{
			//cout << "Файл пуст!\n";
			server.send_int(s2, 0);
			return 1;
		}
		else
		{
			server.send_int(s2, 1);
			FitService *obj;
			SServer server;
			multimap<int, FitService> multi;
			int key = 0, count = 0, i = 0;
			char s[256], buff[100];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			file.close();
			file.clear();

			obj = new FitService[count];
			file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
			for (int i = 0; i < count; i++)
			{
				file.getline(buff, 100, '.');
				key = atoi(buff);
				file >> obj[i];
				multi.insert(pair<int, FitService>(key, obj[i]));
			}
			multimap<int, FitService>::iterator mapPtr;
			char buf[100];
			multimap<int, int>::size_type ex;
			ex = multi.count(keyad);
			//cout << "KOL " << ex << endl;
			mapPtr = multi.find(keyad);
			int numar = mapPtr->second.marks.size();
			int num = sqrt(numar);
			int ***Z = new int**[ex];
			for (int i = 0; i < ex; i++) {
				Z[i] = new int*[num];

				for (int j = 0; j < num; j++) {
					Z[i][j] = new int[num];
				}
			}
			//server.send_int(s2, ex);/////////////////
			mapPtr = multi.find(keyad);
			int e = 0;
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (keyad == (*mapPtr).first)
				{
					int k = 0;
					for (i = 0; i < num; i++)
					{
						for (int j = 0; j < num; j++)
						{
							//cout << "Z[" << e << "][" << i << "][" << j << "] = " << "mapPtr->second.vect" << "[" << k << "]= " << Z[e][i][j] << " = " << mapPtr->second.marks[k] << endl;
							Z[e][i][j] = mapPtr->second.marks[k];
							k++;
						}
					}
					e++;
				}
			}
			int Sum = 0;
			for (int e = 0; e < ex; e++)
			{
				//cout << "_______Z" << 1 << "________" << endl;
				for (i = 0; i < num; i++)
				{
					for (int j = 0; j < num; j++)
					{
						//cout << Z[e][i][j];
						Sum += Z[e][i][j];
					}
					//cout << endl;
				}
			}
			//cout << "Sum " << Sum << endl;
			double **f = new double*[ex];
			for (int i = 0; i < ex; i++)
				f[i] = new double[num];

			for (int e = 0; e < ex; e++)
			{
				for (i = 0; i < num; i++)
				{
					f[e][i] = 0;
				}
			}

			for (int e = 0; e < ex; e++)
			{
				for (i = 0; i < num; i++)
				{
					for (int j = 0; j < num; j++)
					{
						f[e][i] += Z[e][i][j];
						//cout << "f[" << e + 1 << "][" << i + 1 << "] = " << f[e][i] << endl;
					}
				}
			}

			double *Q = new double[num];
			for (int j = 0; j < num; j++)
			{
				Q[j] = 0;
			}
			for (int e = 0; e < ex; e++)
			{
				for (i = 0; i < num; i++)
				{
					Q[i] += f[e][i];
					//cout << "Q[" << i + 1 << "] = " << Q[i] << endl;
				}
			}
			//char buf[10];
			double *W = new double[num];
			for (i = 0; i < num; i++)
			{
				W[i] = Q[i] / Sum;
				sprintf(buf, "%g", W[i]);
				send(s2, buf, sizeof(buf), 0);
				//cout << "W[" << i + 1 << "] = " << W[i] << " = Q[" << i + 1 << "] / Sum = " << Q[i] << " / " << Sum << " = " << Q[i] / Sum << endl;
			}
			double max = W[0];
			int maxi = 0;

			for (int i = 0; i < num; ++i) {
				if (W[i] > max) {
					max = W[i];
					maxi = i;
				}
			}
			server.send_int(s2, maxi);
			//cout << "MAXI= " << maxi + 1 << " MAX= " << max << endl;
			return 0;
		}
	}
}
multimap<int, FitService> FitService::read_mas()
{
	SServer server;
	fstream file;
	multimap<int, FitService> multi;
	file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		//cout << "Файл не открыт!\n";
		//server.send_int(s2, 0);
		return multi;
	}
	else
	{
		if (getFileSize(&file) == 0)
		{
			//cout << "Файл пуст!\n";
			//server.send_int(s2, 0);
			return multi;
		}
		else
		{
			//server.send_int(s2, 1);
			FitService *obj;
			int key = 0, count = 0, i = 0;
			char s[256], buff[100];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			file.close();
			file.clear();

			obj = new FitService[count];
			file.open("FitService.txt", ios::out | ios::in | ios::app | ios::binary);
			for (int i = 0; i < count; i++)
			{
				file.getline(buff, 100, '.');
				key = atoi(buff);
				//cout << "Key" << key << endl;
				file >> obj[i];
				multi.insert(pair<int, FitService>(key, obj[i]));
			}
			file.close();
			file.clear();
			return multi;
		}
	}
}

class Order
{
	string log;
	int id;
	InfoService *serv;
public:
	string getlog() { return log; }
	int getid() { return id; }
	void receive_ord(SOCKET s2)
	{
		SServer server;
		id = atoi(server.receive(s2).c_str());
		log = server.receive(s2).c_str();
	}
	int send_mas(SOCKET, int);
	multimap<int, Order> read_ord();
	
	void top(SOCKET);
	void order(SOCKET);
	void show_ord(SOCKET);

	friend ostream &operator<<(ostream &stream, Order& obj)
	{
		stream << obj.id << ".";
		stream << obj.log << ".";
		return stream;
	}
	friend istream &operator>>(istream &stream, Order& obj)
	{
		char buff[256];
		stream.getline(buff, 256, '.');
		obj.id = atoi(buff);
		stream.getline(buff, 256, '.');
		obj.log = buff;
		//stream.getline(buff, 256, '\n');
		obj.serv = new InfoService[1];
		stream >> obj.serv[0];
		return stream;
	}
};

multimap<int, Order> Order::read_ord()
{
	SServer server;
	fstream file;
	multimap<int, Order> multi;
	file.open("Order.txt", ios::out | ios::in | ios::app | ios::binary);
	if (!file.is_open())
	{
		return multi;
	}
	else
	{
		if (getFileSize(&file) == 0)
		{
			return multi;
		}
		else
		{
			Order *obj;
			int count = 0, i = 0;
			char s[256], buff[100];
			file.seekg(0, ios::beg);
			while (1)
			{
				file.getline(s, 1024, '\n');
				if (file.fail()) break;
				i++;
			}
			count = i;
			file.close();
			file.clear();

			obj = new Order[count];
			file.open("Order.txt", ios::out | ios::in | ios::app | ios::binary);
			for (int i = 0; i < count; i++)
			{
				file >> obj[i];
				multi.insert(pair<int, Order>(obj[i].id, obj[i]));
			}
			file.close();
			file.clear();
			return multi;
		}
	}
}
void Order::order(SOCKET s2)
{
	InfoService serv;
	SServer server;
	map<int, InfoService> m = serv.show_serv(s2);
	if (!m.empty())
	{
		Order ord;
		ord.receive_ord(s2);
		multimap<int, Order> multi = ord.read_ord();
		if (!multi.empty())
		{
			multimap<int, Order>::iterator mapPtr;

			int vb = 0;
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (mapPtr->first == ord.id && mapPtr->second.getlog() == ord.log)
				{
					vb++;
				}
			}
			if (vb > 0)
				server.send_int(s2, 2);
			else {
				server.send_int(s2, 1);
				map<int, InfoService>::iterator Ptr = m.find(ord.id);
				fstream file;
				file.open("Order.txt", ios::out | ios::in | ios::app | ios::binary);
				file << ord << Ptr->second << endl;
				file.clear();
				file.close();
			}
		}
		else {
			server.send_int(s2, 1);
			map<int, InfoService>::iterator Ptr = m.find(ord.id);
			fstream file;
			file.open("Order.txt", ios::out | ios::in | ios::app | ios::binary);
			file << ord << Ptr->second << endl;
			file.clear();
			file.close();
		}
	}
}
void Order::show_ord(SOCKET s2)
{
	SServer server;
	Order ord;
	multimap<int, Order> multi = ord.read_ord();
	if (!multi.empty())
	{
		server.send_int(s2, 1);
		ord.log = server.receive(s2).c_str();
		multimap<int, Order>::iterator mapPtr;
		int vb = 0;
		for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
		{
			if (mapPtr->second.getlog() == ord.log)
			{
				vb++;
			}
		}
		if (vb < 1)server.send_int(s2, 0);
		else {
			server.send_int(s2, vb);
			InfoService serv;
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (mapPtr->second.getlog() == ord.log)
				{
					mapPtr->second.serv->send_serv(s2);

				}
			}
		}
	}
	else {
		server.send_int(s2, 0);
	}
}
void Order::top(SOCKET s2)
{
	SServer server;
	Order ord;
	multimap<int, Order> multi = ord.read_ord();
	if (!multi.empty())
	{
		server.send_int(s2, multi.size());
		fstream file;
		file.open("InfoService.txt", ios::out | ios::in | ios::app | ios::binary);
		int count = 0, i = 0;
		char s[256];
		file.seekg(0, ios::beg);
		while (1)
		{
			file.getline(s, 1024, '\n');
			if (file.fail()) break;
			i++;
		}
		count = i;

		int *x = new int[count+1];
		for (int i = 0; i <= count; i++)
			x[i] = 0;
		multimap<int, Order>::iterator mapPtr;
		for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
		{
			x[mapPtr->first]++;
		}

		for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
		{
			int max = -1;
			int maxi = 0;
			for (int i = 1; i <= count; ++i) {
				if (x[i] > max) {
					max = x[i];
					maxi = i;
				}
			}
			multimap<int, Order>::iterator pos;
			pos = multi.find(maxi);
			server.send_int(s2, x[maxi]);
			if (pos != multi.end())
			{
				server.send_int(s2, maxi);
				pos->second.serv->send_serv(s2);
			}
			x[maxi] = 0;
		}
	}
	else {
		server.send_int(s2, 0);
	}
}

class Method
{
	FitnessCenterWork *data;
	FitService *marks;
public:
	void show(SOCKET);
	void del(SOCKET);
	void add(SOCKET);
	void edit(SOCKET);
	void show_mas(SOCKET);
	void del_mas(SOCKET);
	void edit_mas(SOCKET);
	void solution(SOCKET);
};

void Method::show(SOCKET s2)
{
	FitService data;
	SServer server;
	map<int, FitnessCenterWork> m;
	m = data.send_aim(s2);
	if (!m.empty())
	{
		int key = atoi(server.receive(s2).c_str());
		//cout << "Key" << key << endl;
		map<int, FitnessCenterWork>::iterator mapPtr;
		mapPtr = m.find(key);
		if (mapPtr != m.end())
		{
			//cout << "Key" << key << endl;
			data.send_mas(s2, (*mapPtr).first);
		}
	}
}
void Method::del(SOCKET s2)
{
	FitService marks;
	SServer server;
	fstream file;
	map<int, FitnessCenterWork> m;
	m = marks.send_aim(s2);
	int key = atoi(server.receive(s2).c_str());
	map<int, FitnessCenterWork>::iterator pos;
	pos = m.find(key);
	if (pos != m.end())
	{
		m.erase(key);
		marks.del_mas(s2, key);
	}
	file.open("Fitness.txt", ios::out | ios::binary);
	for (pos = m.begin(); pos != m.end(); pos++)
	{
		file << (*pos).first << "." << (*pos).second;
		for (int i = 0; i < pos->second.getnum_aim(); i++)
		{
			file << pos->second.getoffice(i) << ".";
		}
		file << endl;
	}
	file.close();
	file.clear();
}
void Method::add(SOCKET s2)
{
	FitService data;
	SServer server;
	map<int, FitnessCenterWork> m = data.send_aim(s2);
	if (!m.empty())
	{
		int key = atoi(server.receive(s2).c_str());
		string login = server.receive(s2).c_str();
		multimap<int, FitService> multi = marks->read_mas();
		if (!multi.empty())
		{
			multimap<int, FitService>::iterator mapPtr;

			int vb = 0;
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (mapPtr->first == key && mapPtr->second.getlog() == login)
				{
					vb++;
				}
			}
			if (vb > 0)
				server.send_int(s2, 2);
			else {
				server.send_int(s2, 1);
				data.write_mas(s2, key);
			}
		}
		else {
			server.send_int(s2, 1);
			data.write_mas(s2, key);
		}
	}
}
void Method::edit(SOCKET s2)
{
	FitService marks;
	SServer server;
	fstream file;
	string name;
	map<int, FitnessCenterWork> m;
	m = marks.send_aim(s2);
	int key = atoi(server.receive(s2).c_str());
	map<int, FitnessCenterWork>::iterator mapPtr;
	mapPtr = m.find(key);
	if (mapPtr != m.end())
	{
		int id = atoi(server.receive(s2).c_str());
		mapPtr->second.getobj()[id - 1].receive_serv(s2, name);
	}
	file.open("Fitness.txt", ios::out | ios::binary);
	for (mapPtr = m.begin(); mapPtr != m.end(); mapPtr++)
	{
		file << endl << (*mapPtr).first << "." << (*mapPtr).second << endl;
	}
	file.close();
	file.clear();
}
void Method::show_mas(SOCKET s2)
{
	FitService data;
	SServer server;
	map<int, FitnessCenterWork> m = data.send_aim(s2);
	if (!m.empty())
	{
		int key = atoi(server.receive(s2).c_str());
		string login = server.receive(s2).c_str();
		multimap<int, FitService> multi = marks->read_mas();
		if (!multi.empty())
		{
			multimap<int, FitService>::iterator mapPtr;
			int vb = 0;
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (mapPtr->first == key && mapPtr->second.getlog() == login)
				{
					server.send_int(s2, 1);
					//cout << "(*mapPtr).second.getmarks().size()= " << (*mapPtr).second.getmarks().size() << endl;
					for (int i = 0; i < (*mapPtr).second.getmarks().size(); i++)
					{
						//cout << "[" << i + 1 << "]= " << (*mapPtr).second.getmarks()[i];
						server.send_int(s2, (*mapPtr).second.getmarks()[i]);
					}
					vb++;
				}
			}
			if (vb == 0)
				server.send_int(s2, 0);
		}
		else server.send_int(s2, 0);
	}
}
void Method::del_mas(SOCKET s2)
{
	FitService data;
	SServer server;
	map<int, FitnessCenterWork> m = data.send_aim(s2);
	if (!m.empty())
	{
		int key = atoi(server.receive(s2).c_str());
		string login = server.receive(s2).c_str();
		multimap<int, FitService> multi = marks->read_mas();
		if (!multi.empty())
		{
			multimap<int, FitService>::iterator mapPtr;
			int vb = 0;
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (mapPtr->first == key && mapPtr->second.getlog() == login)
				{
					multi.erase(mapPtr);
					vb++;
					break;
				}
			}
			fstream file;
			file.open("FitService.txt", ios::out | ios::binary);
			file.seekg(0, ios::beg);
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				//cout << mapPtr->first << " " << mapPtr->second << endl;
				file << (*mapPtr).first << "." << (*mapPtr).second;
			}
			if (vb == 0) server.send_int(s2, 0);
			else server.send_int(s2, 1);
		}
		else server.send_int(s2, 0);
	}

}
void Method::edit_mas(SOCKET s2)
{
	FitService data;
	SServer server;
	map<int, FitnessCenterWork> m = data.send_aim(s2);
	if (!m.empty())
	{
		int key = atoi(server.receive(s2).c_str());
		//char buf[100];
		string login = server.receive(s2).c_str();
		multimap<int, FitService> multi = marks->read_mas();
		if (!multi.empty())
		{
			multimap<int, FitService>::iterator mapPtr;
			int vb = 0;
			for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
			{
				if (mapPtr->first == key && mapPtr->second.getlog() == login)
				{
					multi.erase(mapPtr);
					vb++;
					break;
				}
			}
			if (vb == 0) server.send_int(s2, 0);
			else {
				server.send_int(s2, 1);
				data.receive_mas(s2);
				multi.emplace(key, data);
				fstream file;
				file.open("FitService.txt", ios::out | ios::binary);
				file.seekg(0, ios::beg);
				for (mapPtr = multi.begin(); mapPtr != multi.end(); mapPtr++)
				{
					//cout << mapPtr->first << " " << mapPtr->second << endl;
					file << (*mapPtr).first << "." << (*mapPtr).second << endl;
				}
			}
		}
		else server.send_int(s2, 0);
	}
}
void Method::solution(SOCKET s2)
{
	FitService data;
	SServer server;
	map<int, FitnessCenterWork> m;
	m = data.send_aim(s2);
	if (!m.empty())
	{
		int key = atoi(server.receive(s2).c_str());
		map<int, FitnessCenterWork>::iterator mapPtr;
		mapPtr = m.find(key);
		if (mapPtr != m.end())
		{
			char buff[100];
			for (int i = 0; i < mapPtr->second.getnum_aim(); i++)
			{
				strcpy(buff, mapPtr->second.getobj()[i].getname().c_str());
				server.send_str(s2, buff);
			}
			data.count(s2, (*mapPtr).first);
		}
	}
}


int main()
{
	WSACleanup();
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SServer server;
	server.startServer();
	SOCKET s2;
	SOCKADDR_IN addr_c;
	int addrlen = sizeof(addr_c);
	while (true)
	{
		if ((s2 = accept(server.s, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
			//printf("send\n");
			printf("Соединение установлено с  клиентом  %u.%u.%u.%u:%u\n",
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b1,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b2,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b3,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b4,
				ntohs(addr_c.sin_port));
			char buf[100], buf1[100], s;
			int result = 0;
			FitnessCenterWork data;
			InfoService serv;
			Method meth;
			Order ord;
			do
			{
				result = recv(s2, buf, sizeof(buf), 0);
				//cout << "RESULT " << result << endl;
				if (result != 0)
				{
					//cout << "RECV " << buf << endl;
					if (strcmp(buf, "create") == 0)
						data.write_aim(s2);
					if (strcmp(buf, "show") == 0)
						meth.show(s2);
					if (strcmp(buf, "solution") == 0)
						meth.solution(s2);
					if (strcmp(buf, "del") == 0)
						meth.del(s2);
					if (strcmp(buf, "edit") == 0)/////////
						meth.edit(s2);
					if (strcmp(buf, "show_mas") == 0)//////////
						meth.show_mas(s2);
					if (strcmp(buf, "add") == 0)////////
						meth.add(s2);
					if (strcmp(buf, "edit_mas") == 0)///////////
						meth.edit_mas(s2);
					if (strcmp(buf, "del_mas") == 0)////////////
						meth.del_mas(s2);
					if (strcmp(buf, "addserv") == 0)
						serv.write_serv(s2);
					if (strcmp(buf, "showserv") == 0)
						serv.show_serv(s2);
					if (strcmp(buf, "editserv") == 0)
						serv.edit_serv(s2);
					if (strcmp(buf, "order") == 0)
						ord.order(s2);
					if (strcmp(buf, "showord") == 0)
						ord.show_ord(s2);
					if (strcmp(buf, "top") == 0)
						ord.top(s2);
				}


			} while (result > 0);
		}
		closesocket(s2);
		Sleep(50);
	}
	system("pause");
	return 0;
} 