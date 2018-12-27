#include <iostream>
#include <ctime>
#include <windows.h>
#include <psapi.h>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#pragma comment (lib, "Psapi.lib")

typedef boost::variate_generator<boost::mt19937, boost::uniform_int<>> v_gen;

int* createRandomValues(int c);
int rand(int low, int high);
int* copy(int* a, int count);
int randomizedSelect(int* a, int p, int r, int i);
int randomizedPartition(int* a, int p, int r, int k);
int median_Wirth(int* a, int n, int k);

int partition(int* a, int p, int r);
void quickSort(int* a, int p, int r);

void main()
{
	int* _arrayMedian;
	int* _arraySort;
	int _count;
	int _element;
	int _m;
	clock_t _total_time;
	clock_t _lokal_time;

	PROCESS_MEMORY_COUNTERS pmc;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, GetCurrentProcessId());

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
		std::cout << "Speicherverbrauch: " << pmc.PagefileUsage << " Bytes" << std::endl;
	}

	std::cout << "START" << std::endl;
	std::cout << "Wie viele Zufallszahlen sollen generiert werden? --> ";
	std::cin >> _count;
	std::cout << std::endl << "Das wievielt kleinste Element soll gesucht werden? (1-" << _count << ") --> ";
	std::cin >> _element;

	_total_time = clock();
	_lokal_time = clock();

	// generate random numbers
	int* _array;
	_array = createRandomValues(_count);
	_arraySort = copy(_array, _count);
	_arrayMedian = copy(_array, _count);

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
		std::cout << "Speicherverbrauch: " << pmc.PagefileUsage << " Bytes" << std::endl;
	}

	_lokal_time = clock() - _lokal_time;
	std::cout << std::endl << "Zufallszahlen in " << _lokal_time << " ms erstellt." << std::endl;

	// first median search
	_lokal_time = clock();
	_m = randomizedSelect(_array, 0, _count - 1, _element);
	_lokal_time = clock() - _lokal_time;
	std::cout << "Das " << _element << " Element ist: " << _m << std::endl;
	std::cout << "1. Median: " << _element << ". Element nach " << _lokal_time << " ms gefunden." << std::endl;

	// second median search
	_lokal_time = clock();
	_m = median_Wirth(_arrayMedian, _count, _element);
	_lokal_time = clock() - _lokal_time;
	std::cout << "2. Median: " << _element << ". Element nach " << _lokal_time << " ms gefunden." << std::endl;

	// sort array
	_lokal_time = clock();
	quickSort(_arraySort, 0, _count - 1);
	_lokal_time = clock() - _lokal_time;
	std::cout << "Array sortiert nach " << _lokal_time << " ms." << std::endl;

	_total_time = clock() - _total_time;
	std::cout << std::endl << "Programm nach " << _total_time << " ms beendet." << std::endl;
	std::cout << "ENDE" << std::endl;

	delete[] _array;
	delete[] _arraySort;
	delete[] _arrayMedian;

	// show memory leaks
	_CrtDumpMemoryLeaks();
}

int median_Wirth(int* a, int n, int k)
{
	int i, j, l, m;
	int x;

	l = 0;
	m = n - 1;
	while (l < m)
	{
		x = a[k];
		i = l;
		j = m;
		do {
			while (a[i] < x) { i++; }
			while (x < a[j]) { j--; }
			if (i <= j)
			{
				int help = a[i];
				a[i] = a[j];
				a[j] = help;
				i++;
				j--;
			}
		} while (i <= j);
		if (j < k)
			l = i;
		if (k < i)
			m = j;
	}
	return a[k - 1];
}

int randomizedPartition(int* a, int p, int r, int k)
{
	k = rand(p, r);
	int pivot = a[k];
	int help = 0;
	help = a[k];
	a[k] = a[r];
	a[r] = help;
	int storeIndex = p;

	for (int j = p; j < r; ++j)
	{
		if (a[j] < pivot)
		{
			help = a[storeIndex];
			a[storeIndex] = a[j];
			a[j] = help;
			++storeIndex;
		}
	}
	help = a[r];
	a[r] = a[storeIndex];
	a[storeIndex] = help;

	return storeIndex;
}

int randomizedSelect(int* a, int p, int r, int i)
{
	if (p == r)
		return a[p];
	int q = randomizedPartition(a, p, r, i);
	int k = q - p + 1;
	if (i == k)
		return a[p];
	else
	{
		if (i < k)
			return randomizedSelect(a, p, q - 1, i);
		else
			return randomizedSelect(a, q + 1, r, i - k);
	}
}

int* createRandomValues(int c)
{
	// create new array
	int* a = new int[c];

	// random variables
	boost::mt19937 gen(std::time(0));
	boost::uniform_int<> dist(0, 1000000);

	v_gen vargen(gen, dist);

	boost::generator_iterator<v_gen> zahlen(&vargen);

	for (int i = 0; i < c; ++i)
		a[i] = *zahlen++;

	return a;
}

int* copy(int* a, int count)
{
	int* a_neu = new int[count];
	for (int i = 0; i < count; ++i)
		a_neu[i] = a[i];
	return a_neu;
}

int rand(int low, int high)
{
	boost::mt19937 gen(std::time(0));
	boost::uniform_int<> dist(low, high);
	v_gen vargen(gen, dist);
	boost::generator_iterator<v_gen> zahl(&vargen);

	return *zahl;
}

int partition(int* a, int p, int r) {
	int x = a[r];
	int j = p - 1;
	for (int i = p; i < r; i++) {

		if (x <= a[i]) {
			j = j + 1;
			int temp = a[j];
			a[j] = a[i];
			a[i] = temp;
		}
	}
	a[r] = a[j + 1];
	a[j + 1] = x;

	return (j + 1);
}

void quickSort(int* a, int p, int r) {
	if (p < r) {
		int q = partition(a, p, r);
		quickSort(a, p, q - 1);
		quickSort(a, q + 1, r);
	}
}