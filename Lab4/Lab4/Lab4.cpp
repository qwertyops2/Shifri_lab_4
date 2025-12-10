#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <windows.h>
#include <limits>

using namespace std;

vector<bool> reshOfErato(unsigned long long n)
{
    vector<bool> isProst(n + 1, true);

    if (n < 2) return isProst;

    isProst[0] = isProst[1] = false;

    for (unsigned long long i = 2; i * i <= n; i++)
    {
        if (isProst[i])
        {
            for (unsigned long long j = i * i; j <= n; j += i)
                isProst[j] = false;
        }
    }
    return isProst;
}

vector<unsigned long long> getperf(unsigned long long n)
{
    vector<unsigned long long> perf;
    perf.push_back(1);

    for (unsigned long long i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            perf.push_back(i);
            if (i * i != n)
                perf.push_back(n / i);
        }
    }

    return perf;
}

bool isPerfect(unsigned long long n, unsigned long long& sum, vector<unsigned long long>& perf)
{
    if (n < 2) return false;

    perf = getperf(n);

    sum = 0;
    for (auto d : perf)
        sum += d;

    return sum == n;
}

void printPrimes(const vector<bool>& primes)
{
    cout << "Простые числа: ";
    for (size_t i = 2; i < primes.size(); i++)
        if (primes[i]) cout << i << " ";
    cout << "\n";
}

void printHelp()
{
    cout << "Использование:\n";
    cout << "  --mode resh   : Решето Эратосфена\n";
    cout << "  --mode perfect : Проверка числа на совершенность\n";
    cout << "  --number N     : Число\n";
    cout << "  --help         : Показать справку\n";
}

bool isValidMode(const string& mode)
{
    return mode == "resh" || mode == "perfect";
}

bool readUnsignedLongLong(const string& prompt, unsigned long long& value)
{
    cout << prompt;

    string input;
    getline(cin, input);

    if (input.empty())
        return false;

    try
    {
        size_t pos = 0;
        long long temp = stoll(input, &pos);

        if (pos != input.length())
        {
            cout << "Ошибка: введены посторонние символы\n";
            return false;
        }

        if (temp < 0)
        {
            cout << "Ошибка: число должно быть неотрицательным\n";
            return false;
        }

        value = static_cast<unsigned long long>(temp);
        return true;
    }
    catch (const invalid_argument&)
    {
        cout << "Ошибка: введите корректное число\n";
        return false;
    }
    catch (const out_of_range&)
    {
        cout << "Ошибка: число слишком большое\n";
        return false;
    }
}

void runMode(const string& mode, unsigned long long number)
{
    cout << "=============================================\n";
    cout << "              Результаты работы\n";
    cout << "=============================================\n";
    cout << "Режим: " << mode << "\n";
    cout << "Число: " << number << "\n\n";

    auto start = chrono::high_resolution_clock::now();

    if (mode == "resh")
    {
        vector<bool> primes = reshOfErato(number);
        auto end = chrono::high_resolution_clock::now();

        printPrimes(primes);

        auto time_us = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Время выполнения: " << time_us.count() << " мкс\n";
    }
    else if (mode == "perfect")
    {
        unsigned long long sum = 0;
        vector<unsigned long long> divisors;
        bool result = isPerfect(number, sum, divisors);

        auto end = chrono::high_resolution_clock::now();
        auto time_us = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << "Делители числа " << number << ":\n";

        for (size_t i = 0; i < divisors.size(); i++)
        {
            cout << divisors[i];
            if (i < divisors.size() - 1) cout << " + ";
        }

        cout << " = " << sum << "\n";
        cout << "Число " << number << (result ? " является" : " НЕ является") << " совершенным.\n";
        cout << "Время выполнения: " << time_us.count() << " мкс\n";
    }
    else
    {
        cerr << "Неизвестный режим: " << mode << "\n";
    }

    cout << "=============================================\n";
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string mode;
    unsigned long long number = 0;
    bool interactive = false;

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];

        if (arg == "--mode" && i + 1 < argc)
        {
            mode = argv[++i];
            if (!isValidMode(mode))
            {
                cerr << "Ошибка: недопустимый режим. Используйте 'resh' или 'perfect'\n";
                return 1;
            }
        }
        else if (arg == "--number" && i + 1 < argc)
        {
            try
            {
                number = stoull(argv[++i]);
            }
            catch (const exception& e)
            {
                cerr << "Ошибка: недопустимое число в аргументе --number\n";
                return 1;
            }
        }
        else if (arg == "--help")
        {
            printHelp();
            return 0;
        }
        else
        {
            cerr << "Ошибка: неизвестный аргумент: " << arg << "\n";
            printHelp();
            return 1;
        }
    }

    if (argc == 1)
    {
        interactive = true;
        cout << "===== Интерактивный режим =====\n";

        while (true)
        {
            cout << "Введите режим (resh / perfect): ";
            getline(cin, mode);

            if (isValidMode(mode))
                break;

            cout << "Ошибка: допустимые режимы: 'resh' или 'perfect'\n";
        }

        while (!readUnsignedLongLong("Введите число: ", number))
        {
            cout << "Попробуйте еще раз.\n";
        }
    }

    if (mode.empty())
    {
        cerr << "Ошибка: необходимо указать --mode\n";
        printHelp();
        return 1;
    }

    if (number == 0)
    {
        cerr << "Ошибка: необходимо указать корректное число (больше 0)\n";
        return 1;
    }

    runMode(mode, number);

    if (interactive)
    {
        cout << "\nНажмите Enter для выхода...";
        cin.ignore();
        cin.get();
    }

    return 0;
}