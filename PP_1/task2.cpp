#include <omp.h>
#include "task2.h"
#include <Windows.h>

double c1(int, int*, int*, int*,int);
double c2(int, int*, int*, int*,int);
double c3(int, int*, int*, int*,int);
double c4(int, int*, int*, int*,int);

fn* funcs[] = {c1,c2,c3,c4};



#define cycle_1(n,a,b,c) for (i = 0; i< n ; i++ ){ Sleep(0.001); c[i] = a[i] * b[i];} //����������
#define cycle_2(n,a,b,c) for (i = 0; i < n; ++i){ if(i%2==1) Sleep(0.001); c[i] = a[i] * b[i]; } //�������������-�����
#define cycle_3(n,a,b,c) for (i = 0; i < n; ++i){ if(i<n/2) Sleep(0.001); c[i] = a[i] * b[i]; } //�������������-�������


double c1(int n, int* a, int* b, int* c, int type) {
	int i;
	double t1 = omp_get_wtime();


	if (type == 1) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(static)
			cycle_1(n, a, b, c);
		}
	}


	if (type == 2) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(static)
			cycle_2(n, a, b, c);
		}
	}


	if (type == 3) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(static)
			cycle_3(n, a, b, c);
		}
	}


	double t2 = omp_get_wtime();
	return t2 - t1;
}

double c2(int n, int* a, int* b, int* c, int type)
{
	int i;
	double t1 = omp_get_wtime();

	if (type == 1) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(dynamic)
			cycle_1(n, a, b, c);
		}
	}

	if (type == 2) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(dynamic)
			cycle_2(n, a, b, c);
		}
	}

	if (type == 3) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(dynamic)
			cycle_3(n, a, b, c);
		}
	}


	double t2 = omp_get_wtime();
	return t2 - t1;
}

double c3(int n, int* a, int* b, int* c, int type)
{
	int i;
	double t1 = omp_get_wtime();

	if (type == 1) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(guided)
			cycle_1(n, a, b, c);
		}
	}

	if (type == 2) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(guided)
			cycle_2(n, a, b, c);
		}
	}

	if (type == 3) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(guided)
			cycle_3(n, a, b, c);
		}
	}


	double t2 = omp_get_wtime();
	return t2 - t1;
}

double c4(int n, int* a, int* b, int* c, int type)
{
	int i;
	double t1 = omp_get_wtime();


	if (type == 1) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(runtime)
			cycle_1(n, a, b, c);
		}
	}


	if (type == 2) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(runtime)
			cycle_2(n, a, b, c);
		}
	}


	if (type == 3) {
#pragma omp parallel private(i)
		{
#pragma omp for schedule(runtime)
			cycle_3(n, a, b, c);
		}
	}


	double t2 = omp_get_wtime();
	return t2 - t1;
}

/**
 * ������ �������� ���������� schedule(type, chunk), type - ��� ��������, chunk - ������ ������, ������������ �����
 * ����������. ������������ ��, ��� ��� �������� ����� �������������� ����� ��������
 * Static -  �������� ���������� �������������� �� ������� �� ������� ������ chunk ��� �� ������� N/���.-�� �������
 * ����������� ������������� ������ ����������, ����� ����� ���������� �������� �����,
 * ��� �������������� �����. ���� ��� �� ���, �� ������� ������������ ��������� ��� ������������� �����:
 * Dynamic - �������� ����������� �� ������ ������ ������� chunk, �� ��� ����������� ��������������� � ��������������
 * ������
 * Guided - ��� �������� �� ������ ���������� ������������ �������� ����������� �������� �� ������ chunk � �����������
 * � ������ �������������� �����
 * Runtime - ��������� ����������� ���������� �������� ������ � �������� ���������� ���������
 * Auto - ���� ����������� ������ ������ ����� ��� ������ ������������
 */