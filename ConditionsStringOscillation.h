#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

template<typename T>
using OscilCondBorderFunc = T(*)(T);

template<class Type>
struct OscilCondTaskParams {
	Type L;
	Type T;

	Type rho;

	OscilCondBorderFunc<Type> initCond;
	OscilCondBorderFunc<Type> DDinitCond;

	OscilCondBorderFunc<Type> initCondSpeed;

	OscilCondBorderFunc<Type> leftBorderType;
	OscilCondBorderFunc<Type> rightBorseType;

	Type a;
};

template<class Type>
struct OscilCondMethodParams {
	int n; // отрезки по пространству
	int m; // отрезки по времени
};

template<class T>
void oscilStringCheme(const OscilCondTaskParams<T>& taskParams, const OscilCondMethodParams<T>& methodParams, const string& nameFile) {
	T N = methodParams.n;
	T M = methodParams.m;

	T h = taskParams.L / N;
	T tau = taskParams.T / M;
	T a = taskParams.a;

	vector<T> t(M + 1);
	t[0] = 0;
	for (int i = 1; i < M + 1; ++i) {
		t[i] = t[0] + i * tau;
	}

	vector<T> x(N + 1);
	x[0] = 0.;
	for (int i = 1; i < N + 1; ++i) {
		x[i] = x[0] + i * h;
	}

	auto leftCond = taskParams.leftCond;
	auto rightCond = taskParams.rightCond;

	vector<T> prev_y(N + 1);
	vector<T> cur_y(N + 1);
	vector<T> next_y(N + 1);

	ofstream outFile(nameFile);

	for (int i = 0; i < N + 1; ++i) { // начальное условие расположения точек струны
		prev_y[i] = taskParams.initCond(x[i]);
	}

	for (int i = 0; i < N + 1; ++i) { // вывод в файл нулевого временного слоя
		outFile << prev_y[i] << " ";
	}
	outFile << endl;

	for (int i = 0; i < N + 1; ++i) { // начальное условие скоростей каждой точки струны
		cur_y[i] = prev_y[i] + tau * tau * taskParams.initCondSpeed(x[i]) + (a * a * tau * tau) / 2 * taskParams.DDinitCond(x[i]);
	}

	for (int i = 0; i < N + 1; ++i) { // вывод в файл первого временного слоя
		outFile << cur_y[i] << " ";
	}
	outFile << endl;

	for (int j = 1; j < M; ++j) { 
		
		next_y[0] = leftCond(t[j]);
		next_y[N] = rightCond(t[j]);

		for (int i = 1; i < N; ++i) {
			next_y[i] = (a * a * tau * tau) / (h * h) * (cur_y[i + 1] - 2 * cur_y[i] + cur_y[i - 1]) + 2 * cur_y[i] - prev_y[i - 1];
		}
		
		for (int k = 0; k < N + 1; ++k) {
			outFile << next_y[k] << " ";
		}
		outFile << endl;
	}
	outFile.close();
}