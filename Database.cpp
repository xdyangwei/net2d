#include "Database.h"
#include<string>
#include<iostream>
std::string read_file(std::string fname) {
	HANDLE hFile = CreateFile(fname.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		puts("failed to read file ");
		puts(fname.c_str());
		return "";
	}
	DWORD dwSize = GetFileSize(hFile, NULL);	
	char* cBuf = new char[dwSize+1];
	RtlZeroMemory(cBuf, sizeof(char)*(dwSize));
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	DWORD haveReadByte;
	ReadFile(hFile, cBuf, dwSize, &haveReadByte, 0);
	cBuf[dwSize] = 0;
	std::cout << "read bytes " << haveReadByte << std::endl;
	auto result = std::string(cBuf);
	puts(cBuf);
	free(cBuf);
	return result;
}


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int build_point(void* point_vector, int argc, char** argv, char ** col_name) {
	auto v = (std::vector<POINT_MESSAGE>*)(point_vector);
	POINT_MESSAGE m;
	m.x = atof(argv[3]);
	m.y = atof(argv[4]);
	v->push_back(m);
}



Database::Database(const char * filename)
{
	dbname = "";
	int rc = sqlite3_open(filename, &this->sqlite);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(this->sqlite));
		sqlite3_close(this->sqlite);
		this->sqlite = NULL;
	}
	char* err;
	rc = sqlite3_exec(this->sqlite, read_file("./sql/create_tables.sql").c_str(), callback, 0, &err);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err);
		sqlite3_free(err);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
		dbname = filename;
	}

	sqlite3_close(this->sqlite);
}

Database::~Database()
{
}

std::vector<POINT_MESSAGE> Database::GetData()
{
	int rc = sqlite3_open(this->dbname.c_str(), &this->sqlite);
	char* err;
	rc = sqlite3_exec(this->sqlite, read_file("./sql/readall.sql").c_str(), callback, 0, &err);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err);
		sqlite3_free(err);
	}
	sqlite3_close(this->sqlite);
	return std::vector<POINT_MESSAGE>();
}
