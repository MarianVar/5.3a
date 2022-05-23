#include <iostream>
#include <string>
#include <iomanip>
#include <locale>
#include <fstream>
#include <Windows.h> 

using namespace std;
enum Specialty { KN, INF, ME, PI, TD };

string specialtyStr[] = { "Комп'ютерні науки", "Інформатика", "Матема. та екн.", " Фізика та інфорю", "Трудове навчання" };

struct Student {
	char prizv[64];
	unsigned cours;
	Specialty spec;
	unsigned physic, math, informatic;
};

void Create(const char* fName);
void Print(const char* fName);
void Sort(const char* fName);
void fWrite(fstream& f, const int i, Student s);
Student fRead(fstream& f, const int i);
void fChange(fstream& f, const int i, const int j);
int BinSearch(const char* fName, const char* prizv, const int cours, const int spec);
void IndexSort(const char* fName , const char* tName);
void PrintIndexSorted(const char* fName, const char* tName);

int main()
{

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char t[8] = "tmp.dat";
	char fname[100];
	cout << "Введіть назву файлу: ";
	cin >> fname;

	char prizv[64];
	int cours, spec, binNum;

	int menuItem;

	do {
		cout << endl << endl << endl;
		cout << "Виберіть дію:" << endl << endl;
		cout << " [1] - введення даних з клавіатури" << endl;
		cout << " [2] - вивід даних на екран" << endl;
		cout << " [3] - фізичне впорядкування даних" << endl;
		cout << " [4] - бінарний пошук студент за прізвищем, курсом і спеціальністю" << endl;
		cout << " [5] - індексне впорядкування та вивід даних" << endl << endl;
		cout << " [0] - вихід та завершення роботи програми" << endl << endl;
		cout << "Введіть значення: "; cin >> menuItem;
		cout << endl << endl << endl;


		switch (menuItem)
		{
		case 1:
			Create(fname);
			break;
		case 2:
			Print(fname);
			break;
		case 3:
			Sort(fname);
			break;
		case 4:
			cout << "Введіть ключі пошуку:" << endl;

			cout << "Введіть прізвище: "; cin >> prizv;
			cout << "Введіть курс: "; cin >> cours;
			cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та екноміка, 3 - Фізика та інформатика, 4 - Трудове навчання): " << endl;
			cout << "Введіть спеціальеність: "; cin >> spec;
			binNum = BinSearch(fname, prizv, cours, spec);
			if (binNum == -1) 
			{
				cout << "Не знайдено студента з такими характеристиками!!!" << endl;
			}
			else 
			{
				cout << "Номер цього студента в списку: " << binNum << endl;
			}
			break;
		case 5:
			IndexSort(fname, t);
			PrintIndexSorted(fname, t);
			break;
		case 0:
			break;
		default:
			cout << "Ви ввели помилкове значення! "
				"Слід ввести число - номер вибраного пункту меню" << endl;
		}
	} while (menuItem != 0);
	return 0;
}

void Create(const char* fName)
{

	ofstream f(fName, ios::binary);
	Student s;
	char ch;
	int spec;
	int i = 0;
	do {
		cin.get();
		cin.sync();

		cout << " Прізвище: "; cin >> s.prizv;
		cout << " Курс: "; cin >> s.cours;

		cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та екноміка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
		cin >> spec;
		s.spec = (Specialty)spec;
		cout << " Оцінка з фізики: "; cin >> s.physic;
		cout << " Оцінка з математики: "; cin >> s.math;
		cout << " Оцінка з інформатики: "; cin >> s.informatic;
		cout << endl;
		if (!f.write((char*)&s, sizeof(Student))) {
			cerr << "Error writing file." << endl;
		}
		cout << "Continue?(Y/N)"; cin >> ch;
	} while (ch == 'y' || ch == 'Y');
}

void Print(const char* fName)
{

	ifstream f(fName, ios::binary);
	Student s;
	int i = 0;

	cout << "========================================================================="
		<< endl;
	cout << "| № | Прізвище | Курс | Спеціальність | Фізика | Математика | Інформатика | "
		<< endl;
	cout << "-------------------------------------------------------------------------"
		<< endl;
	while (f.read((char*)&s, sizeof(Student)))
	{
		cout << "| " << setw(2) << right << i + 1 << " ";
		cout << "| " << setw(9) << left << s.prizv
			<< "| " << setw(4) << right << s.cours << " "
			<< "| " << setw(11) << left << specialtyStr[s.spec] << " "
			<< "| " << setw(6) << right << s.physic << " "
			<< "| " << setw(6) << left << s.math << " "
			<< "| " << setw(7) << right << s.informatic << endl;
	}
	cout << "========================================================================="
		<< endl;
	cout << endl;
}

void fWrite(fstream& f, const int i, Student s) {
	f.seekp(i * (long)sizeof(Student));
	f.write((char*)&s, sizeof(Student));
}

Student fRead(fstream& f, const int i) {
	Student s;
	f.seekg(i * (long)sizeof(Student));
	f.read((char*)&s, sizeof(Student));
	return s;
}

void fChange(fstream& f, const int i, const int j) {
	Student x = fRead(f, i);
	Student y = fRead(f, j);
	fWrite(f, i, y);
	fWrite(f, j, x);
}

void Sort(const char* fName) {
	fstream f(fName, ios::binary | ios::in | ios::out);

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	for (int i = 1; i < size; i++) 
	{
		for (int j = 0; j < size - i; j++) 
		{
			Student a = fRead(f, j);
			Student b = fRead(f, j + 1);
			if ((a.spec > b.spec)
				||
				(a.spec == b.spec &&
					a.cours > b.cours)
				||
				(a.spec == b.spec &&
					a.cours == b.cours &&
					a.prizv > b.prizv))
			{
				fChange(f, j, j + 1);
			}
		}
	}
}

int BinSearch(const char* fName, const char* prizv, const int cours, const int spec)
{
	fstream f(fName, ios::binary | ios::in | ios::out);
	int L = 0, m;
	f.seekg(0, ios::end);
	int R = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);
	do {
		m = (L + R) / 2;
		Student a = fRead(f, m);
		if (strcmp(a.prizv, prizv) == 0 && a.cours == cours && a.spec == spec ) {
			return m + 1;
		}
		else if ((a.prizv > prizv) ||
			(a.cours == cours && a.cours > cours) ||
			(a.prizv == prizv && a.cours == cours && a.spec > spec)) {
			R = m - 1;
		}
		else {
			L = m + 1;
		}
	} while (L <= R);
	return -1;
}

void IndexSort(const char* fName, const char* tName)
{
	fstream t(tName, ios::binary | ios::in | ios::out | ios::trunc);
	fstream f(fName, ios::binary | ios::in | ios::out);

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	for (int i = 0; i < size; i++)
		t.write((char*)&i, sizeof(int));
	t.seekg(0, ios::beg);

	int value1, value2, tmp;
	for (int i = 1; i < size; i++) {
		for (int j = 0; j < size - i; j++) {
			;
			t.seekg(j * (long)sizeof(int));
			t.read((char*)&value1, sizeof(int));
			t.read((char*)&value2, sizeof(int));
			Student a = fRead(f, value1);
			Student b = fRead(f, value2);
			if ((a.spec > b.spec) ||
				(a.spec == b.spec && a.cours > b.cours) ||
				(a.spec == b.spec && a.cours == b.cours && a.prizv > b.prizv)) {
				int tmp1;
				int tmp2;
				t.seekg(j * (long)sizeof(int));
				t.read((char*)&tmp1, sizeof(int));
				t.seekg((j + 1) * (long)sizeof(int));
				t.read((char*)&tmp2, sizeof(int));
				t.seekg(j * (long)sizeof(int));
				t.write((char*)&tmp2, sizeof(int));
				t.seekg((j + 1) * (long)sizeof(int));
				t.write((char*)&tmp1, sizeof(int));
			}
		}
	}

	t.seekg(0, ios::beg);
	f.seekp(0, ios::end);
	f.close();
	t.close();

	
}

void PrintIndexSorted(const char* fName, const char* tName)
{

	cout << "========================================================================="
		<< endl;
	cout << "| № | Прізвище | Курс | Спеціальність | Фізика | Математика | Інформатика | "
		<< endl;
	cout << "-------------------------------------------------------------------------"
		<< endl;
	ifstream t(tName, ios::binary);
	fstream f(fName, ios::binary | ios::in | ios::out);
	Student s;

	f.seekg(0, ios::end);
	int size = f.tellg() / sizeof(Student);
	f.seekg(0, ios::beg);

	int ind, i = 1;

	while (t.read((char*)&ind, sizeof(int))) {
		f.seekp(ind * (long)sizeof(Student));
		f.read((char*)&s, sizeof(Student));
		cout << "| " << setw(2) << right << i + 1 << " ";
		cout << "| " << setw(9) << left << s.prizv
			<< "| " << setw(4) << right << s.cours << " "
			<< "| " << setw(11) << left << specialtyStr[s.spec] << " "
			<< "| " << setw(6) << right << s.physic << " "
			<< "| " << setw(6) << left << s.math << " "
			<< "| " << setw(7) << right << s.informatic << endl;
	}
	cout << "========================================================================="
		<< endl;
	cout << endl;
}
