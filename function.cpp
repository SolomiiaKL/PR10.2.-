#include "function.h"
#include <iostream>
#include <limits>

using namespace std;

namespace
{
	string SpecToStr(Spec spec)
	{
		switch (spec)
		{
		case KN:
			return "Комп'ютерні науки";
		case INF:
			return "Інформатика";
		case ME:
			return "Мат. та економіка";
		case FI:
			return "Фіз. та інформатика";
		case TN:
			return "Трудове навчання";
		default:
			return "Невідомо";
		}
	}

	int Utf8Len(const string& text)
	{
		int len = 0;
		for (unsigned char c : text)
		{
			if ((c & 0xC0) != 0x80)
			{
				len++;
			}
		}
		return len;
	}

	string PadCol(const string& text, int width, bool rightAlign)
	{
		int len = Utf8Len(text);
		if (len >= width)
		{
			return text;
		}
		string sp(width - len, ' ');
		return rightAlign ? (sp + text) : (text + sp);
	}

	void ClearInputBuffer()
	{
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	int ReadIntInRange(const string& prompt, int minVal, int maxVal)
	{
		int val;
		while (true)
		{
			cout << prompt;
			if (cin >> val && val >= minVal && val <= maxVal)
			{
				ClearInputBuffer();
				return val;
			}
			cout << "Помилка! Введіть ціле число в межах ["
				<< minVal << "; " << maxVal << "]." << endl;
			ClearInputBuffer();
		}
	}

	string ReadNonEmptyLine(const string& prompt)
	{
		string line;
		while (true)
		{
			cout << prompt;
			if (getline(cin, line) && !line.empty())
			{
				return line;
			}
			cout << "Помилка! Рядок не може бути порожнім." << endl;
		}
	}

	void PrintTableRows(const Student* p, const int* indexMap, const int N)
	{
		const int w1 = 3;  // №
		const int w2 = 16; // Прізвище
		const int w3 = 5;  // Курс
		const int w4 = 21; // Спеціальність
		const int w5 = 7;  // Фізика
		const int w6 = 10; // Математика
		const int w7 = 11; // Інформатика

		string header =
			"| " + PadCol("№", w1, true) +
			" | " + PadCol("Прізвище", w2, false) +
			" | " + PadCol("Курс", w3, true) +
			" | " + PadCol("Спеціальність", w4, false) +
			" | " + PadCol("Фізика", w5, true) +
			" | " + PadCol("Математика", w6, true) +
			" | " + PadCol("Інформатика", w7, true) +
			" |";

		int lineLen = Utf8Len(header);
		string line(lineLen, '-');

		string title = (indexMap == nullptr)
			? "Таблиця успішності студентів"
			: "Індексно впорядкована таблиця успішності студентів";
		int titleLen = Utf8Len(title);
		int leftPad = (lineLen - 2 - titleLen) / 2;
		if (leftPad < 0) leftPad = 0;
		int rightPad = (lineLen - 2 - titleLen) - leftPad;
		if (rightPad < 0) rightPad = 0;

		cout << line << endl;
		cout << "|" << string(leftPad, ' ') << title << string(rightPad, ' ') << "|" << endl;
		cout << line << endl;
		cout << header << endl;
		cout << line << endl;

		for (int i = 0; i < N; i++)
		{
			int idx = (indexMap != nullptr) ? indexMap[i] : i;
			const Student& st = p[idx];

			cout << "| " << PadCol(to_string(i + 1), w1, true)
				<< " | " << PadCol(st.prizv, w2, false)
				<< " | " << PadCol(to_string(st.kurs), w3, true)
				<< " | " << PadCol(SpecToStr(st.spec), w4, false)
				<< " | " << PadCol(to_string(st.fizyka), w5, true)
				<< " | " << PadCol(to_string(st.matematyka), w6, true)
				<< " | " << PadCol(to_string(st.informatyka), w7, true)
				<< " |" << endl;
		}

		cout << line << endl << endl;
	}

	// Варіант 14, п. 1 (фізичне впорядкування):
	// 1) оцінка з 3-го предмета (informatyka) за зростанням
	// 2) курс за зростанням
	// 3) прізвище за спаданням (у зворотному до алфавітного порядку)
	bool ShouldSwapPhysical(const Student& a, const Student& b)
	{
		if (a.informatyka != b.informatyka)
			return a.informatyka > b.informatyka;
		if (a.kurs != b.kurs)
			return a.kurs > b.kurs;
		return a.prizv < b.prizv;
	}

	// Варіант 14, п. 2 (індексне впорядкування):
	// 1) оцінка з 3-го предмета (informatyka) за зростанням
	// 2) курс за зростанням
	// 3) прізвище за зростанням (в алфавітному порядку)
	bool ShouldShiftIndex(const Student& leftSt, const Student& valSt)
	{
		if (leftSt.informatyka != valSt.informatyka)
			return leftSt.informatyka > valSt.informatyka;
		if (leftSt.kurs != valSt.kurs)
			return leftSt.kurs > valSt.kurs;
		return leftSt.prizv > valSt.prizv;
	}
}

void Create(Student* p, const int N)
{
	for (int i = 0; i < N; i++)
	{
		cout << "Студент № " << i + 1 << ":" << endl;
		p[i].prizv = ReadNonEmptyLine("  Прізвище: ");
		p[i].kurs = ReadIntInRange("  Курс (1-5): ", 1, 5);

		int specCode = ReadIntInRange(
			"  Спеціальність (0 - КН, 1 - ІНФ, 2 - МЕ, 3 - ФІ, 4 - ТН): ", 0, 4);
		p[i].spec = static_cast<Spec>(specCode);

		p[i].fizyka = ReadIntInRange("  Оцінка з фізики (2-5): ", 2, 5);
		p[i].matematyka = ReadIntInRange("  Оцінка з математики (2-5): ", 2, 5);
		p[i].informatyka = ReadIntInRange("  Оцінка з інформатики (2-5): ", 2, 5);
		cout << endl;
	}
}

void Print(const Student* p, const int N)
{
	PrintTableRows(p, nullptr, N);
}

void Sort(Student* p, const int N)
{
	for (int i = 0; i < N - 1; i++)
	{
		for (int j = 0; j < N - i - 1; j++)
		{
			if (ShouldSwapPhysical(p[j], p[j + 1]))
			{
				Student tmp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = tmp;
			}
		}
	}
}

int* IndexSort(const Student* p, const int N)
{
	int* I = new int[N];
	for (int i = 0; i < N; i++)
	{
		I[i] = i;
	}

	for (int i = 1; i < N; i++)
	{
		int value = I[i];
		int j = i - 1;
		while (j >= 0 && ShouldShiftIndex(p[I[j]], p[value]))
		{
			I[j + 1] = I[j];
			j--;
		}
		I[j + 1] = value;
	}

	return I;
}

void PrintIndexSorted(const Student* p, const int* I, const int N)
{
	PrintTableRows(p, I, N);
}

// Варіант 14, п. 3: Бінарний пошук у фізично впорядкованому масиві
// за ключами (оцінка з інформатики за зростанням, курс за зростанням, прізвище за спаданням)
int BinSearch(const Student* p, const int N, const string& prizv,
	const int kurs, const int informatyka)
{
	int L = 0;
	int R = N - 1;

	while (L <= R)
	{
		int m = L + (R - L) / 2;

		if (p[m].informatyka == informatyka && p[m].kurs == kurs && p[m].prizv == prizv)
		{
			return m;
		}

		bool goRight =
			(p[m].informatyka < informatyka) ||
			(p[m].informatyka == informatyka && p[m].kurs < kurs) ||
			(p[m].informatyka == informatyka && p[m].kurs == kurs && p[m].prizv > prizv);

		if (goRight)
		{
			L = m + 1;
		}
		else
		{
			R = m - 1;
		}
	}

	return -1;
}