#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <string>


using namespace std;

class student {
	string name;
	int sj1, sj2, sum;
	double avg;
public:
	void putgrade(int x, int y) {
		sj1 = x;
		sj2 = y;
		sum = x + y;
		avg = sum / 2;
	}
	void putName(string h) { name = h; }
	string getName() { return name; }
	double getAvg() { return avg; }
	int getSum() { return sum; }
	int getSj1() { return sj1; }
	int getSj2() { return sj2; }
};

int main() {
	sqlite3* db;
	int rc;
	int i = 0;
	int subsum1, subsum2;
	double subavg1, subavg2;
	vector <student> stu(3);

	rc = sqlite3_open("sj.db", &db);
	if (rc != SQLITE_OK) {
		cerr << "데이터베이스 열기 실패: " << sqlite3_errmsg(db) << endl;
		return 1;
	}
	const char* sql = "SELECT * FROM student;";
	sqlite3_stmt* stmt;

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cerr << "쿼리 준비 실패: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
		return 1;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		
		string name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
		int sj1 = sqlite3_column_int(stmt, 1);
		int sj2 = sqlite3_column_int(stmt, 2);

		stu[i].putgrade(sj1, sj2);
		stu[i].putName(name);
		i++;
		
		cout << "이름: " << name << ", 과목1 : " << sj1 << ", 과목2 :" << sj2 << endl;
	}

	if (rc != SQLITE_DONE) {
		cerr << "쿼리 실행 실패: " << sqlite3_errmsg(db) << endl;
	}

	for (int i = 0; i < 3; i++) {
		cout << stu[i].getName() << "총점 : " << stu[i].getSum() << endl;
		cout << stu[i].getName() << "평균 : " << stu[i].getAvg() << endl;
		subsum1 =+ stu[i].getSj1();
		subsum2 = +stu[2].getSj2();
	}

	subavg1 = (double)subsum1 / 2;
	subavg2 = (double)subsum2 / 2;

	cout << "과목1 총점: " << subsum1 << endl;
	cout << "과목1 평균: " << subavg1 << endl;
	cout << "과목2 총점: " << subsum2 << endl;
	cout << "과목2 평균: " << subavg2 << endl;

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return 0;

}