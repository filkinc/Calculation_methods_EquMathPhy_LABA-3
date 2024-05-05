// Calculation_methods_EquMathPhy_LABA-3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>

#include "ConditionsStringOscillation.h"

using namespace std;

const double pi = acos(-1);

const string INPUT_TASK_PARAMS = "C:\\Users\\filki\\Documents\\Учеба в МГТУ\\6 семестр\\Численные методы МФ\\Лаба №3 ЧМ\\Calculation_methods_EquMathPhy_LABA-3\\file_task_params.txt";
const string INPUT_METHOD_PARAMS = "C:\\Users\\filki\\Documents\\Учеба в МГТУ\\6 семестр\\Численные методы МФ\\Лаба №3 ЧМ\\Calculation_methods_EquMathPhy_LABA-3\\file_method_params.txt";
const string OUTPUT_FILE = "C:\\Users\\filki\\Documents\\Учеба в МГТУ\\6 семестр\\Численные методы МФ\\Лаба №3 ЧМ\\Calculation_methods_EquMathPhy_LABA-3\\out_put_file.txt";

template<typename Type>
OscilCondTaskParams<Type> inputTaskParams(const string& nameFile) {
    OscilCondTaskParams<Type> taskParams;
    Type L, T, rho, a;
    int initCondType, initCondSpeedType, leftCondType, rightCondType;
    bool DDinitCondType;

    ifstream inFile(nameFile);
    inFile >> L >> T >> rho >> a >> initCondType >> DDinitCondType >> initCondSpeedType >> leftCondType >> rightCondType;
    inFile.close();

    taskParams.L = L;
    taskParams.T = T;
    taskParams.rho = rho;
    taskParams.a = a;

    //Выбор значения второй производной
    taskParams.DDinitCondType = DDinitCondType;

    //Начальное расположение каждой точки
    if (initCondType == 1) {
        taskParams.initCond = [](Type x) {return sin(pi * x); };
        taskParams.DDinitCond = [](Type x) {return pi * cos(pi * x); };
    }
    else if (initCondType == 2) {
        taskParams.initCond = [](Type x) {return x * (1 - x); };
        taskParams.DDinitCond = [](Type x) {return 1 - 2 * x; };
    }
    else if (initCondType == 3) {
        taskParams.initCond = [](Type x) {return 2 * x * (x + 1) + 0.3; };
        taskParams.DDinitCond = [](Type x) {return 4 * x + 2; };
    }

    //Начальная скорость каждой точки
    if (initCondSpeedType == 1) {
        taskParams.initCondSpeed = [](Type) {return 0.; };
    }
    else if (initCondSpeedType == 2) {
        taskParams.initCondSpeed = [](Type x) {return 2 * sin(x); };
    }

    //Левое граничное условие
    if (leftCondType == 1) {
        taskParams.leftCond = [](Type) {return 0.; };
    }
    else if (leftCondType == 2) {
        taskParams.leftCond = [](Type) {return 0.3; };
    }

    //Правое граничное условие
    if (rightCondType == 1) {
        taskParams.rightCond = [](Type) {return 0.; };
    }
    else if (rightCondType == 2) {
        taskParams.rightCond = [](Type t) {return 4.3 + t; };
    }

    return taskParams;
}

template<typename T>
OscilCondMethodParams<T> inputMethodParams(const string& nameFile) {
    OscilCondMethodParams<T> methodParams;
    T n, m;
    
    ifstream inFile(nameFile);
    inFile >> n >> m;
    inFile.close();

    methodParams.n = n;
    methodParams.m = m;

    return methodParams;
}

int main()
{
    auto taskParams = inputTaskParams<double>(INPUT_TASK_PARAMS);
    auto methodParams = inputMethodParams<double>(INPUT_METHOD_PARAMS);

    oscilStringCheme(taskParams, methodParams, OUTPUT_FILE);

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
