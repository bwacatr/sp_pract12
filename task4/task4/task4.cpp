// task4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <math.h>
#include <conio.h>

int reverseNum(int num)
{
    int newNum = 0;
    while (num != 0)
    {
        newNum = newNum * 10;
        newNum = newNum + num % 10;
        num = num / 10;
    }
    return newNum;
}

int main(int argc,  char* argv[])
{
    setlocale(LC_ALL, "Russian");

    if (argc < 3)
    {
        return 0;
    }

    int a = 0;
    int b = 5000;

    try
    {
        a = std::stoi(argv[1]);
        b = std::stoi(argv[2]);
    }
    catch (const std::exception&)
    {
        std::cout << "Ошибка: были введены не целые числа";
    }
    

    int num1 = std::min(a, b);
    int num2 = std::max(a,b);

    

    int count = 0;

    for (int i = num1; i <= num2; i++)
    {
        if (i == reverseNum(i))
        {
            count++;
            std::cout << "\n" << i << std::endl;
        }
    }

    std::cout << "\n\nКол-во чисел-палиндромов: " << count << std::endl;

    std::cout << "Нажмите любую клавишу" << std::endl;

    std::cin.get();
    
}


