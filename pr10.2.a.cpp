#define NOMINMAX
#include <windows.h>
#include "function.h"
#include <iostream>
#include <limits>

using namespace std;

namespace
{
	void ClearMenuBuffer()
	{
		cin.clear();
		cin.ignore((numeric_limits<streamsize>::max)(), '\n');
	}

	int ReadMenuItem()
	{
		int item;
		while (true)
		{
			cout << "Введіть значення: ";
			if (cin >> item)
			{
				ClearMenuBuffer();
				return item;
			}
			cout << "Помилка! Введіть ціле число — номер пункту меню." << endl;
			ClearMenuBuffer();
		}
	}

	int ReadPositiveCount()
	{
		int n;
		while (true)
		{
			cout << "Введіть кількість студентів: ";
			if (cin >> n && n > 0)
			{
				ClearMenuBuffer();
				return n;
			}
			cout << "Помилка! Кількість студентів має бути додатним цілим числом." << endl;
			ClearMenuBuffer();
		}
	}

	int ReadSearchInt(const string& prompt, int minVal, int maxVal)
	{
		int val;
		while (true)
		{
			cout << prompt;
			if (cin >> val && val >= minVal && val <= maxVal)
			{
				ClearMenuBuffer();
				return val;
			}
			cout << "Помилка! Введіть число в межах [" << minVal << "; " << maxVal << "]." << endl;
			ClearMenuBuffer();
		}
	}

	string ReadNonEmptyText(const string& prompt)
	{
		string text;
		while (true)
		{
			cout << prompt;
			if (getline(cin, text) && !text.empty())
			{
				return text;
			}
			cout << "Помилка! Рядок не може бути порожнім." << endl;
		}
	}
}

int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	int N = ReadPositiveCount();
	Student* p = new Student[N];
	bool isDataEntered = false;
	bool isPhysicallySorted = false;

	int menuItem;
	do
	{
		cout << endl;
		cout << "Виберіть дію:" << endl;
		cout << "  [1] - введення даних з клавіатури" << endl;
		cout << "  [2] - вивід даних на екран" << endl;
		cout << "  [3] - фізичне впорядкування даних" << endl;
		cout << "  [4] - індексне впорядкування та вивід даних" << endl;
		cout << "  [5] - бінарний пошук студента" << endl;
		cout << "  [0] - вихід та завершення роботи програми" << endl << endl;

		menuItem = ReadMenuItem();
		cout << endl;

		switch (menuItem)
		{
		case 1:
			Create(p, N);
			isDataEntered = true;
			isPhysicallySorted = false;
			break;

		case 2:
			if (!isDataEntered)
			{
				cout << "Спочатку введіть дані про студентів (пункт [1])!" << endl;
				break;
			}
			Print(p, N);
			break;

		case 3:
			if (!isDataEntered)
			{
				cout << "Спочатку введіть дані про студентів (пункт [1])!" << endl;
				break;
			}
			Sort(p, N);
			isPhysicallySorted = true;
			cout << "Масив фізично впорядковано." << endl;
			Print(p, N);
			break;

		case 4:
		{
			if (!isDataEntered)
			{
				cout << "Спочатку введіть дані про студентів (пункт [1])!" << endl;
				break;
			}
			int* idxArr = IndexSort(p, N);
			PrintIndexSorted(p, idxArr, N);
			delete[] idxArr;
			break;
		}

		case 5:
		{
			if (!isDataEntered)
			{
				cout << "Спочатку введіть дані про студентів (пункт [1])!" << endl;
				break;
			}
			if (!isPhysicallySorted)
			{
				cout << "Масив ще не був фізично впорядкований — виконуємо впорядкування перед пошуком..." << endl;
				Sort(p, N);
				isPhysicallySorted = true;
			}

			string prizv = ReadNonEmptyText("Введіть прізвище: ");
			int kurs = ReadSearchInt("Введіть курс (1-5): ", 1, 6);
			int infGrade = ReadSearchInt("Введіть оцінку з інформатики (2-5): ", 2, 5);

			int found = BinSearch(p, N, prizv, kurs, infGrade);
			if (found != -1)
				cout << "Студента знайдено на позиції " << found + 1 << "." << endl;
			else
				cout << "Студента не знайдено." << endl;
			break;
		}

		case 0:
			break;

		default:
			cout << "Помилкове значення! Слід ввести номер вибраного пункту меню." << endl;
			break;
		}
	} while (menuItem != 0);

	delete[] p;
	return 0;
}