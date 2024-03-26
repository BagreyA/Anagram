#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>//подключен для работы с широкими символами

#define MAX_LENGTH 100

void swap(wchar_t* a, wchar_t* b) 
{
    wchar_t temp = *a;
    *a = *b;
    *b = temp;
    }

void find_anagrams(wchar_t* word, int start, int end, FILE* output)//находим все анаграммы для заданного слова(использует рекурсивный подход)
{
    if (start == end) 
    {
        fwprintf(output, L"%ls\n", word);
        wprintf(L"%ls\n", word);
        return;
    }

    for (int i = start; i <= end; i++) 
    {
        swap(&word[start], &word[i]);
        find_anagrams(word, start + 1, end, output);
        swap(&word[start], &word[i]);
    }
}

void find_anagrams_in_file(wchar_t* filename)//находим все анаграммы для слова в файле
{
    FILE* input = _wfopen(filename, L"r, ccs=UTF-8");
    if (input == NULL) 
    {
        wprintf(L"Ошибка при открытии файла %ls!\n", filename);
        return;
    }
    
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    rewind(input);
    
    wchar_t* word = (wchar_t*) malloc((file_size + 1) * sizeof(wchar_t));
    if (word == NULL) 
    {
        wprintf(L"Ошибка при выделении памяти!\n");
        fclose(input);
        return;
    }
    fgetws(word, file_size, input);
    fclose(input);

    word[file_size] = L'\0';
    if (word[file_size - 1] == L'\n') //проверка последнего символа, если он равен символу новой строки 
    {                                 //то он заменяется на символ окончания строки
        word[file_size - 1] = L'\0';
    }

    int len = wcslen(word);
    if (len == 0)//выводится когда файл пуст 
    {
        wprintf(L"Файл %ls пуст!\n", filename);
        free(word);
        return;
    }
    
    for (int i = 0; i < len; i++)//выводится когда в файле есть символы помимо букв и пробелов 
    {
        if (!iswalpha(word[i])) 
        {
            wprintf(L"Файл %ls содержит недопустимые символы!\n", filename);
            free(word);
            return;
        }
    }

    FILE* output = _wfopen(L"anagrams.txt", L"w, ccs=UTF-8");
    if (output == NULL) 
    {
        wprintf(L"Ошибка при открытии выходного файла!\n");
        free(word);
        return;
    }

    find_anagrams(word, 0, len - 1, output);

    fclose(output);
    printf("Найденные анаграммы(включая изначальное слово) записаны в файл \"anagrams.txt\"!\n");
    free(word);
}

int main(int argc, char* argv[]) 
{
    setlocale(0, "");
    wchar_t* filename_w;//объявление указателя на символы для хранения имени файла


//проверяется наличие аргумента командной строки с индексом 1 с помощью if (argc > 1)
//если аргумент существует, то имя файла извлекается из argv[1]
//иначе имя файла запрашивается у пользователя с помощью scanf()
    if (argc > 1) {
        int len = mbstowcs(NULL, argv[1], 0) + 1; 
        filename_w = (wchar_t*) malloc(len * sizeof(wchar_t)); 
        mbstowcs(filename_w, argv[1], len); 
    } else {
        printf("Введите имя файла: "); 
        char filename[MAX_LENGTH]; 
        scanf("%s", filename); 

        int len = mbstowcs(NULL, filename, 0) + 1; 
        filename_w = (wchar_t*) malloc(len * sizeof(wchar_t)); 
        mbstowcs(filename_w, filename, len); 
    }
    find_anagrams_in_file(filename_w); 
    free(filename_w); 
 
    return 0; 
}
