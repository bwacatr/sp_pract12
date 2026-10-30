

#include <iostream>
#include <locale>
#include <string>
#include <exception>
#include <Windows.h>
#include <cwchar>

struct ProcessInfo // нужно создать массив этой структуры и сделать функцию, которая удаляет из массива и добавляет процессы в него
{
    std::wstring name;
    HANDLE hProcess;
    DWORD dwProcessId;
};

class ProcessInfoArray
{
    int size = 0;
public:

    ~ProcessInfoArray()
    {
        delete[] array;
    }
    
    ProcessInfo* array = nullptr;

    int GetSize()
    {
        return size;
    }

    void AddProcess(ProcessInfo& process)
    {
        //если указатель массива пуст(nullptr), то выделяем память под новый элемент массива ProcessInfo
        if (array == nullptr)
        {
            array = new ProcessInfo[1];
            array[0] = process;
            
        }
        else // иначе создаем новый массив с увеличенным размером
        {
            ProcessInfo* newArray = new ProcessInfo[size+1];

            for (int i = 0; i < size; i++)
            {
                newArray[i] = array[i];
            }

            newArray[size] = process;

            delete[] array;
            array = newArray;
            
        }
        size++;
    }

    void RemoveProcess(int index)
    {
        if (array == nullptr || index < 0 || index >= size)
        {

            return;
        }
        else if (size == 1)
        {
            CloseHandle(array[index].hProcess);
            delete[] array;

            array = nullptr;
            size = 0;

            return;
        }
        else
        {
            CloseHandle(array[index].hProcess);

            ProcessInfo* newArray = new ProcessInfo[size - 1];

            int newArrayIndex = 0;

            for (int i = 0; i < size; i++)
            {
                if (i != index)
                {
                    newArray[newArrayIndex] = array[i];
                    newArrayIndex++;
                }
            }

            size--;

            delete[] array;
            array = newArray;
        }
        
    }
};







int main()
{
    setlocale(LC_ALL, "Russian");
    std::wcout.imbue(std::locale(""));

    ProcessInfoArray processArray;

    bool skipMenu = false;

    while (skipMenu == false)
    {
        try
        {
            
            std::wstring lpCL;
            std::wstring executableName;;

            ProcessInfo process;

            std::string params;

            
        
            std::wcout << "\nМеню \n"
             << "1. Создание процесса.\n" 
             << "2. Список процессов.\n" 
             << "3. Завершение процесса.\n" 
             << "4. Количество работающих процессов.\n" 
             << "5. Выход. \n" <<  std::endl;

            int choiceNum;

            std::string str;
            std::cin >> str;
        
            try
            {
            
                choiceNum = std::stoi(str);

                if (choiceNum > 5 || choiceNum < 1)
                {
                    throw std::invalid_argument("Должно быть введено целое число от 1 до 5 включительно");
                }
            }
            catch (const std::exception& e)
            {
                throw std::invalid_argument("Должно быть введено целое число от 1 до 5 включительно");
            }

            switch (choiceNum)
            {
            case 1:
                std::wcout << "\n\n1. taskmgr.exe\n"
                    << "2. ping.exe\n"
                    << "3. task4.exe" << std::endl;

                
                std::cin >> str;

                int choiceNum1;

                try
                {

                    choiceNum1 = std::stoi(str);

                    if (choiceNum1 > 3 || choiceNum1 < 1)
                    {
                        throw std::invalid_argument("Должно быть введено целое число от 1 до 3 включительно");
                    }
                }
                catch (const std::exception& e)
                {
                    throw std::invalid_argument("Должно быть введено целое число от 1 до 3 включительно");
                }

                executableName = L"";

                params = "";

                switch (choiceNum1)
                {
                case 1:
                    executableName = L"taskmgr.exe";
                    break;
                case 2:
                    executableName = L"ping.exe";

                    std::wcout << "Введите параметры через пробел \n(Пример: [IP-address] | -n [N] [IP-address] | -t [IP-address])" << std::endl;
                    std::cin.ignore();
                    std::getline(std::cin, params);
                    
                    break;
                case 3:
                    executableName = L"task4.exe";

                    std::wcout << "Введите параметры через пробел \n(Пример: 0 405)" << std::endl;
                    std::cin.ignore();
                    std::getline(std::cin, params);
                    break;
                }

                if (params.empty() == false)
                {
                    std::wstring wParams(params.begin(), params.end());
                    executableName += L" " + wParams;
                }

                STARTUPINFO startUpInfo;
                PROCESS_INFORMATION piApp;
                ZeroMemory(&startUpInfo, sizeof(STARTUPINFO));
                startUpInfo.cb = sizeof(STARTUPINFO);

                lpCL = executableName;
                

                if (CreateProcess(NULL,&lpCL[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startUpInfo, &piApp) == false)
                {
                    throw std::runtime_error("Ошибка запуска процесса: " + std::to_string(GetLastError()));
                }

                process = { executableName, piApp.hProcess, piApp.dwProcessId };
                processArray.AddProcess(process);

                CloseHandle(piApp.hThread);
            
                break; // конец case 1

            case 2:
                std::wcout << "\nСписок процессов\n" << std::endl;

                if (processArray.array == nullptr)
                {
                    std::wcout << "Пустой список" << std::endl;
                }
                else
                {
                    for (int i = 0; i < processArray.GetSize(); i++)
                    {
                        DWORD exitCode = 0;

                        GetExitCodeProcess(processArray.array[i].hProcess, &exitCode);

                        if (exitCode != STILL_ACTIVE)
                        {
                            processArray.RemoveProcess(i);
                            i--;
                        }
                        else
                        {
                            std::wcout << i << " | " << processArray.array[i].name << " | ProcessID: " << processArray.array[i].dwProcessId << std::endl;
                        }
                    }
                    
                }
                break;

            case 3:
                std::wcout << "\nЗавершение процессов\n" << std::endl;

                if (processArray.array == nullptr)
                {
                    std::wcout << "Запущенные процессы отсутствуют" << std::endl;
                }
                else
                {
                    


                    for (int i = 0; i < processArray.GetSize(); i++)
                    {
                        DWORD exitCode = 0;

                        GetExitCodeProcess(processArray.array[i].hProcess, &exitCode);

                        if (exitCode != STILL_ACTIVE)
                        {
                            processArray.RemoveProcess(i);
                            i--;
                        }
                        else
                        {
                            std::wcout << i << " | " << processArray.array[i].name << " | ProcessID: " << processArray.array[i].dwProcessId << std::endl;
                        }
                    }

                    std::wcout << "Введите номер процесса\n" << std::endl;

                    std::cin >> str;
                    int index;

                    try
                    {
                        index = std::stoi(str);
                    }
                    catch (const std::exception&)
                    {
                        throw std::invalid_argument("Ошибка: введен сторонний символ");
                    }

                    if (index >= 0 && index < processArray.GetSize())
                    {
                        if (TerminateProcess(processArray.array[index].hProcess, 0))
                        {
                            processArray.RemoveProcess(index);
                            std::wcout << "Процесс завершен" << std::endl;
                        }
                        else
                        {
                            std::wcout << "Процесс не был завершен" << std::endl;
                        }
                    }
                    else
                    {
                        std::wcout << "Некорректный индекс" << std::endl;
                    }

                }
                break;

            case 4:
                std::wcout << "Количество процессов: " << processArray.GetSize() << std::endl;
                break;

            case 5:
                skipMenu = true;
                break;

            default:
                std::wcout << "Некорректный пункт меню" << std::endl;
                break;

            }

            

        }
        catch (const std::exception& e) 
        {
            std::wcout.flush();
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }

    for (int i = 0; i < processArray.GetSize(); i++)
    {
        DWORD exitCode = 0;

        GetExitCodeProcess(processArray.array[i].hProcess, &exitCode);

        if (exitCode != STILL_ACTIVE)
        {
            processArray.RemoveProcess(i);
            i--;
        }
    }

    

}


