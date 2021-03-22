#include "Database.hpp"

int Database::establishConnection() {
	try {
		sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
		this->con = driver->connect("tcp://database-1.c8qhphxyl0rn.eu-west-1.rds.amazonaws.com:3306", "root", "12345678");
		con->setSchema(DATABASESCHEME);
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	return 0;
}

long long Database::lastID() {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;
	if (this->con->isValid()) {
		try {
			pstmt = this->con->prepareStatement(LAST_INSERT_ID_SQL);
			res = pstmt->executeQuery();
			while (res->next()) {
				this->ID = res->getString("LAST_INSERT_ID()");
				delete pstmt;
				return res->getInt64(1);
			}

			//std::cout << pstmt->getResultSet()->getInt64("LAST_INSERT_ID()");
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			
			return -1;
		}
	}
	else {
		this->establishConnection();
		try {
			pstmt = this->con->prepareStatement(LAST_INSERT_ID_SQL);
			res = pstmt->executeQuery();
			while (res->next()) {
				this->ID = res->getString("LAST_INSERT_ID()");
				delete pstmt;
				return res->getInt64(1);
			}

			//std::cout << pstmt->getResultSet()->getInt64("LAST_INSERT_ID()");
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}

	
}

int Database::insertMeasuredData(float data[5], int num) {
	sql::PreparedStatement* pstmt;
	if (this->con->isValid()) {
		try {
			pstmt = this->con->prepareStatement(INSERT_MEASURED_DATA_SQL);
			pstmt->setBigInt(1, this->ID);
			for (int i = 0; i < num; i++) {
				pstmt->setDouble(i + 2,data[i]);
			}
			pstmt->executeQuery();
			delete pstmt;
			return 0;
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	else {
		this->establishConnection();
		try {
			pstmt = this->con->prepareStatement(INSERT_MEASURED_DATA_SQL);
			pstmt->setBigInt(1, this->ID);
			for (int i = 0; i < num; i++) {
				pstmt->setDouble(i + 2, data[i]);
			}
			pstmt->executeQuery();
			delete pstmt;
			return 0;
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}

}

int Database::insertDataInstance(int fkid, long long timestamp) {
	sql::PreparedStatement* pstmt;
	if (this->con->isValid()) {
		try {
			pstmt = this->con->prepareStatement(INSERT_DATA_INSTANCE_SQL);
			pstmt->setInt(1, fkid);
			pstmt->setInt64(2, timestamp);
			pstmt->executeQuery();

		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	else {
		this->establishConnection();
		try {
			pstmt = this->con->prepareStatement(INSERT_DATA_INSTANCE_SQL);
			pstmt->setInt(1, fkid);
			pstmt->setInt64(2, timestamp);
			pstmt->executeQuery();
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	delete pstmt;
	return 0;
}

int Database::insertMotor(int id, float ratedData[4], int num) {
	sql::PreparedStatement* pstmt;
	if (this->con->isValid()) {
		try {
			pstmt = this->con->prepareStatement(INSERT_MOTOR_SQL);
			pstmt->setInt(1, id);
			for (int i = 0; i < num; i++) {
				pstmt->setDouble(i+2, ratedData[i]);
			}
			pstmt->executeQuery();
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	else {
		this->establishConnection();
		try {
			pstmt = this->con->prepareStatement(INSERT_MOTOR_SQL);
			pstmt->setInt(1, id);
			for (int i = 0; i < num; i++) {
				pstmt->setDouble(i + 2, ratedData[i]);
			}
			pstmt->executeQuery();
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	delete pstmt;
	return 0;
}

int Database::insertTest() {
	sql::PreparedStatement* pstmt;
	float a = 2212;
	if (this->con->isValid()) {
		try {
			pstmt = this->con->prepareStatement(INSERT_TEST_SQL);
			pstmt->setDouble(1, a);
			//pstmt->executeUpdate();
			pstmt->executeQuery();
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	else {
		this->establishConnection();
		try {
			pstmt = this->con->prepareStatement("INSERT INTO Test VALUES (?)");
		    pstmt->setDouble(1, a);
			pstmt->executeQuery();
			//pstmt->executeUpdate();
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	delete pstmt;
	return 0;
}