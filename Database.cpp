#include "Database.hpp"

int Database::establishConnection() {
	try {
		sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
		this->con = driver->connect("tcp://192.168.1.7:4002", "root", "rootpassword");
		con->setSchema(DATABASESCHEME);
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	return 0;
}

int Database::insertTest(const char* a) {
	sql::PreparedStatement* pstmt;
	if (this->con->isValid()) {
		try {
			pstmt = this->con->prepareStatement("INSERT INTO StableStock VALUES (?)");
			pstmt->setString(1, a);
			pstmt->executeUpdate();
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	else {
		this->establishConnection();
		try {
			pstmt = this->con->prepareStatement("INSERT INTO StableStock VALUES (?)");
			pstmt->setString(1, a);
			pstmt->executeUpdate();
		}
		catch (sql::SQLException& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	delete pstmt;
	return 0;
}