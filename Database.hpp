#pragma once
#include <mysql/jdbc.h>

#define DATABASESCHEME "Test"

class Database
{
private:
	sql::Connection* con;
public:
	Database() {};

	int establishConnection();

	int insertTest(const char* a);

	void closeConnection() {
		delete this->con;
	}

	int insertData(float data[6]);
};

