#include <iostream>
#include <string>
using namespace std;

#ifndef NUMBER
#define NUMBER 100
#endif
class Student
{
public:
	Student(string name = "jinjay",
			float grade = 0,
			float classGrade = 0,
			char isCadre = 'N',
			char isWest = 'N',
			int paperNum = 0){
		this->name = name;
		this->grade = grade;
		this->classGrade = classGrade;
		this->isCadre = isCadre;
		this->isWest = isWest;
		this->paperNum = paperNum;
	};
	Student();
	~Student();

public:
	string name;
	float grade;
	float classGrade;
	char isCadre;
	char isWest;
	int paperNum;
	// award
	int award;
public:
	int getAward() {
		int Money = 0;
		if (grade > 80 && paperNum > 0) {
			Money += 8000;
		}
		if (grade > 85 && classGrade > 80) {
			Money += 4000;
		}
		if (grade > 90) {
			Money += 2000;
		}
		if (grade > 85 && isWest == 'Y') {
			Money += 1000;
		}
		if (grade > 80 && isCadre == 'Y') {
			Money += 850;
		}
		return (this->award = Money);
	}
};

typedef Student* SS;

void acm(){
	int x;cin>>x;
	int maxMoney = 0;
	int totalMoney = 0;
	Student* ss[NUMBER];
	for (int i = 0; i < x; ++i)
	{
		string name;float grade;float classGrade;char isCadre;char isWest;int paperNum;
		cin>>name;
		cin>>grade;
		cin>>classGrade;
		cin>>isCadre;
		cin>>isWest;
		cin>>paperNum;
		ss[i] = new Student(name,grade,classGrade,isCadre,isWest,paperNum);
	}
	int index = 0;
	for (int i = 0; i < x; ++i)
	{
		int iAward = ss[i]->getAward();
		if (maxMoney < iAward) {
			maxMoney = iAward;
			index = i;
		}
		totalMoney += iAward;
	}
	cout<<ss[index]->name<<endl<<ss[index]->award<<endl<<totalMoney;
}

int main(int argc, char const *argv[])
{
	int n;
	cin>>n;
	for (int i = 0; i < n; ++i) {
		acm();		
	}
	cin>>n;
	return 0;
}