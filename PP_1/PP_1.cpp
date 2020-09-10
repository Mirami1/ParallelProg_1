// PP_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include <iostream>
#include <string>
#include <omp.h>
#include "task2.h"
#include <Windows.h>

using namespace std;



int n = 10000;
void f1();
void f2();
void f3();
void f3_1(int*);
void f3_2(int*);
void f4();
void f5();

void printMatrix(int n, int** a) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%4i ", a[i][j]);
		printf("\n");
	}
}
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");
	printf("Задача - 1\n");
	//f1();
	printf("Задача - 2\n");
	f2();
	printf("Задача - 3\n");
	//f3();
	printf("Задача - 4\n");
	//f4();
	printf("Задача - 5\n");
	//f5();
}




/*1. Вывести параллельно “Hello World”  5 - 10 раз.*/
void f1() { 
	omp_set_num_threads(rand() % 3 + 5);
#pragma omp parallel
	{
		printf("Hello World, %i thread\n", omp_get_thread_num());
	}
	printf("\n");
}


/*2.Исследовать влияние настроек параметра shedule (не менее 4 опций ) парадигмы #omp parallel for.*/
	/*Результат вывести в виде таблице (4 х 3). Объяснить результат.*/
void f2() 
{
	int* a = new int[n];
	int* b = new int[n];
	int* c = new int[n];
	for (int i = 0; i < n; ++i)
	{
		a[i] = i;
		b[i] = i + 1;
		c[i] = 0;
	}


	printf("accuracy=%g\n", omp_get_wtick()); //точность
	printf("\t\t static \t dynamic \t guided \t runtime\n");

	int deg = 100; //число повторений опытов
	for (int k = 1; k < 4; k++) { // 3 типа итераций
		printf("Type - %d ", k);
		for (int i = 0; i < 4; i++) { //4 варианта опций task2.cpp
			double res = 0;
			for (int j = 0; j < deg; j++) {
				res += (funcs[i])(n, a, b, c,k);
			}
			res /= deg;
			printf("\t%g", res);
		}
		printf("\n");
	}
	delete a;
	delete b;
	delete c;

	printf("\n\n");
}

/**
 * Критическая секция сообщает, что в определенный момент времени она будет выполняться только одним потоком
 * То есть когда один поток находится в критической секции, все остальные потоки находятся в режиме ожидания
 * Reduction позволяет производить безопасное глобальное вычисление. В нашем случае с оператором сложения. Разбивает
 * участки массива на N/кол.-во потоков и поочередно выполняет операции с полученными диапазонами массива
 */

void f3() {
	int* a = new int[n];
	for (int i = 0; i < n; i++)
		a[i] = 1;
	f3_1(a);
	f3_2(a);

	delete a;
	printf("\n");
}

void f3_1(int* a) { //critical
	int i;
	int sum = 0;
	double t1 = omp_get_wtime();

#pragma omp parallel private(i)
	{
#pragma omp for
		for (i = 0; i < n; i++)
#pragma omp critical
		{
			sum += a[i];
		}
	}
	double t2 = omp_get_wtime();

	printf("critical sum=%i; time=%g\n", sum, t2 - t1);
}

void f3_2(int* a) { //reduction
	int i;
	int sum = 0;
	double t1 = omp_get_wtime();

#pragma omp parallel private(i), reduction(+:sum)
	{
#pragma omp for
		for (i = 0; i < n; i++)
			sum += a[i];
	}
	double t2 = omp_get_wtime();

	printf("reduction sum=%i; time=%g\n", sum, t2 - t1);

}

void f4() {

	printf("Without barrier: \n");
#pragma omp parallel
	{
		printf("Thread %i before barrier\n", omp_get_thread_num());

		printf("Thread %i after barrier\n", omp_get_thread_num());
	}

	printf("With barrier: \n");
#pragma omp parallel
	{
		printf("Thread %i before barrier\n", omp_get_thread_num());
#pragma omp barrier
		printf("Thread %i after barrier\n", omp_get_thread_num());
	}

	printf("\n");
	
}

void f5() {
	int** a = new int* [n];
	int** b = new int* [n];
	n = 1000;
	int step = 0;
	int step_1 = 0;
	for (int i = 0; i < n; ++i)
	{
		a[i] = new int[n];
		b[i] = new int[n];
		for (int j = 0; j < n; ++j) {
			a[i][j] = step++;
			b[i][j] = step_1++;
		}
	}
//	printMatrix(n, a);

	
	// Последовательное вычисление

	double t1 = omp_get_wtime();

	for (int i = 2; i < n; i++)
		for (int j = 2; j < n; j++) {
			a[i][j] = a[i - 2][j] + a[i][j - 2];
			Sleep(0.000001);
		}
	
	double t2 = omp_get_wtime();

	printf("Time: %fs\n", t2 - t1);
//	printMatrix(n, a);
	printf("\n");
	 
	
	// Параллельное вычисление

	t1 = omp_get_wtime();
	int diag_count = 2 * n - 1; //2n-2 - последняя диагональ из одного элемента
	for (int d = 4; d <diag_count; d++) { 
		int i;
		//определим диапазон i
		int maxi = d - 1 < n ? d - 1 : n;
#pragma omp parallel private(i)
		{
#pragma omp for
			for (i = d - (n + 1) > 0 ? 2 + d - (n + 1) : 2; i < maxi; ++i)
			{

				b[i][d - i] = b[i - 2][d - i] + b[i][d - i - 2];
				Sleep(0.000001);
				
			}
		}
		
	}
	t2 = omp_get_wtime();
	printf("Time: %fs\n", t2 - t1);
	//printMatrix(n, b);
	//printf("\n");
	

	for (int i = 0; i < n; ++i) {
		delete a[i];
		delete b[i];
	}
	delete a;
	delete b;
}