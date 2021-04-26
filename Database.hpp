#pragma once
#include <mysql/jdbc.h>

#define DATABASESCHEME "Data"
#define INSERT_MOTOR_SQL "INSERT INTO Motor VALUES(?,?,?,?,?)" //id, rated v, rated c, rated p, rated f
#define INSERT_DATA_INSTANCE_SQL "INSERT INTO DataInstance VALUES(NULL,?,?)" //id, fkid, timestamp
#define INSERT_MEASURED_DATA_SQL "INSERT INTO MeasuredData VALUES(?,?,?,?,?,?,?)" //id, v, c, tc, s, f
#define LAST_INSERT_ID_SQL "SELECT LAST_INSERT_ID()"
#define INSERT_TEST_SQL "INSERT INTO Test VALUES (?)"

class Database
{
private:
	sql::Connection* con;
	sql::SQLString ID;
public:
	Database() {};

	long long lastID();

	int establishConnection();

	int insertTest();

	int insertDataInstance(int fkid, long long timestamp);

	int insertMotor(int id, float ratedData[4], int num);

	void closeConnection() {
		delete this->con;
	}

	int insertMeasuredData(float data[5], int num);

	int insertData(float data[6]);
};

