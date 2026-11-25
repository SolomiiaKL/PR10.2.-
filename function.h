#pragma once
#include <string>

enum Spec
{
	KN, // Комп'ютерні науки
	INF, // Інформатика
	ME, // Математика та економіка
	FI, // Фізика та інформатика
	TN // Трудове навчання
};

struct Student
{
	std::string prizv;
	int kurs;
	Spec spec;
	int fizyka;
	int matematyka;
	int informatyka;
};

void Create(Student* p, const int N);
void Print(const Student* p, const int N);
void Sort(Student* p, const int N);
int* IndexSort(const Student* p, const int N);
void PrintIndexSorted(const Student* p, const int* I, const int N);
int BinSearch(const Student* p, const int N, const std::string& prizv,
	const int kurs, const int informatyka);