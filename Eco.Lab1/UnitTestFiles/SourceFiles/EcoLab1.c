/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"

/* Включение */
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/* Тесты */
#include "time.h"
#include "string.h"


int16_t __cdecl compare_int16(const void* a, const void* b) {
	int16_t arg1 = *(const int16_t*)a;
    int16_t arg2 = *(const int16_t*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int16_t __cdecl compare_int32(const void* a, const void* b) {
	int32_t arg1 = *(const int32_t*)a;
    int32_t arg2 = *(const int32_t*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int16_t __cdecl compare_double(const void* a, const void* b) {
	double_t arg1 = *(const double_t*)a;
    double_t arg2 = *(const double_t*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int16_t __cdecl compare_float(const void* a, const void* b) {
	float_t arg1 = *(const float_t*)a;
    float_t arg2 = *(const float_t*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int16_t __cdecl compare_char(const void* a, const void* b) {
	char_t arg1 = *(const char_t*)a;
    char_t arg2 = *(const char_t*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int16_t __cdecl compare_string(const void* a, const void* b) {
	int32_t result = strcmp(*(const char**)a, *(const char**)b);
    if (result < 0) return -1;
    if (result > 0) return 1;
    return 0;
}


/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    char_t* copyName = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;
	/* Указатель на включаемый интерфейс */
	IEcoCalculatorX* pIEcoCalculatorX = 0;
	/* Указатель на включаемый интерфейс */
    IEcoCalculatorY* pIEcoCalculatorY = 0;
	
	/* Переменные для сортировки */
	int32_t i;
	int32_t size;
	
	int16_t* arr16;
	int32_t* arr32;
	double_t* arr_double;
	float_t* arr_float;
	char_t** string_arr;
	int32_t string_size;

	/* Переменные для замера времени работы */
	clock_t start, end;
	double_t sorting_time = 0;
	int16_t j;

	/* Переменные для проверки включения */
	int16_t x = 0, y = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Выделение блока памяти */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);

    /* Заполнение блока памяти */
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }


	/* Проверка включения */
	printf("Input x:\n");
    scanf_s("%d", &x);
	printf("Input y:\n");
    scanf_s("%d", &y);

	printf("Get component X from EcoLab1\n");
	result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIEcoCalculatorX);
    printf("Addition result: %d\n", pIEcoCalculatorX->pVTbl->Addition(pIEcoCalculatorX, x, y));
	printf("Subtraction result: %d\n", pIEcoCalculatorX->pVTbl->Subtraction(pIEcoCalculatorX, x, y));

	printf("Get component Y from EcoLab1\n");
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIEcoCalculatorY);
    printf("Multiplication result: %d\n", pIEcoCalculatorY->pVTbl->Multiplication(pIEcoCalculatorY, x, y));
    printf("Division result: %d\n", pIEcoCalculatorY->pVTbl->Division(pIEcoCalculatorY, x, y));

	printf("Get component X from Y\n");
	result = pIEcoCalculatorY->pVTbl->QueryInterface(pIEcoCalculatorY, &IID_IEcoCalculatorX, (void**)&pIEcoCalculatorX);
	printf("Addition result: %d\n", pIEcoCalculatorX->pVTbl->Addition(pIEcoCalculatorX, x, y));
	printf("Subtraction result: %d\n", pIEcoCalculatorX->pVTbl->Subtraction(pIEcoCalculatorX, x, y));
	
	printf("Get component Y from X\n");
    result = pIEcoCalculatorX->pVTbl->QueryInterface(pIEcoCalculatorX, &IID_IEcoCalculatorY, (void**)&pIEcoCalculatorY);
    printf("Multiplication result: %d\n", pIEcoCalculatorY->pVTbl->Multiplication(pIEcoCalculatorY, x, y));
    printf("Division result: %d\n", pIEcoCalculatorY->pVTbl->Division(pIEcoCalculatorY, x, y));

	printf("Get component EcoLab1 from X\n");
    result = pIEcoCalculatorX->pVTbl->QueryInterface(pIEcoCalculatorX, &IID_IEcoLab1, (void**)&pIEcoLab1);

	/* Проверка сортировки */

	srand((unsigned int)time(NULL));

	printf("Input an array size: \n");
    scanf_s("%d", &size);

	
	/*СТРОКОВЫЙ ПРИМЕР*/

	/* Генерация массива строк */
	//string_arr = (char_t **) pIMem->pVTbl->Alloc(pIMem, size * sizeof(char_t*));
	//for(i = 0; i < size; i++) {
	//	string_arr[i] = (char_t*) pIMem->pVTbl->Alloc(pIMem, 15 * sizeof(char_t));
	//	string_size = rand() % 10 + 2;
	//	for (j = 0; j < string_size; ++j) {
	//		string_arr[i][j] = (char_t) (rand() % ('z' - 'a' + 1) + 'a');
	//  }
	//  string_arr[i][string_size] = 0;
	//}

	/* Печать неотсортированного массива */
	//for(j = 0; j < size; j++) {
	//	printf("%s\n", string_arr[j]);
	//}
	//printf("-----------------------------\n");

	/* Сортировка */
	//result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, size, sizeof(char_t*), string_arr, compare_string);

	/* Печать отсортированного массива */
	//for(j = 0; j < size; j++) {
	//	printf("%s\n", string_arr[j]);
	//}


	/* ЧИСЛОВЫЕ ПРИМЕРЫ */
	
	/* Выделение памяти для массивов */
	//arr16 = (int16_t *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(int16_t));
	arr32 = (int32_t *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(int32_t));
	//arr_double = (double_t *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(double_t));
	//arr_float = (float_t *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(float_t));
	//arr_double = (double_t *) pIMem->pVTbl->Alloc(pIMem, size * sizeof(double_t));
    

	/* Ввод элементов массива вручную */
	//printf("Input the array elements: \n");
	//for (i = 0; i < size; i++) {
	//	//scanf_s("%d", &arr16[i]);
	//    scanf_s("%d", &arr32[i]);
	//	//scanf_s("%f", &arr_double[i]);
	//	//scanf_s("%f", &arr_float[i]);
	//}
	
	//Выполнение нескольких сортировок для получения среднего времени выполнения
	//for(j = 0; j < 10; j++) {
		/* Заполнение массива рандомными числами */
		for(i = 0; i < size; i++) {
			//arr16[i] = rand() % 10;
			//arr32[i] = size - i;
			arr32[i] = rand() % 1000 * (i % 2 == 0 ? 1 : -1);
			//arr_double[i] = rand() % 100 * 0.0001 * (rand() % 100) * (i % 2 == 0 ? 1 : -1);
			//arr_float[i] = rand() % 100 + 0.1 * (rand() % 100) * (i % 2 == 0 ? 1 : -1);
		}

		/* Печать неотсортированного массива */
		//printf("Unsorted array:\n");
		//for (i = 0; i < size; i++) {
		//	//printf("%d ", arr16[i]);
		//	printf("%d ", arr32[i]);
		//	//printf("%f ", arr_double[i]);
		//	//printf("%f ", arr_float[i]);
		//}
		//printf("\n");

		start = clock();

		/* Сортировка */
		//result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, size, sizeof(int16_t), arr16, compare_int16);
		result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, size, sizeof(int32_t), arr32, compare_int32);
		//result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, size, sizeof(double_t), arr_double, compare_double);
		//result = pIEcoLab1->pVTbl->qsort(pIEcoLab1, size, sizeof(float_t), arr_float, compare_float);
		//qsort(arr32, size, sizeof(int32_t), compare_int32);
	
		end = clock();
		sorting_time += (double_t)(end - start) * 1000.0 / CLOCKS_PER_SEC;
	//}
	//sorting_time /= 10;
	printf("Sorting time: %f milli-seconds\n", sorting_time);

	/* Печать отсортированного массива */
	printf("Sorted array:\n");
	for (i = 0; i < size; i++) {
		//printf("%d ", arr16[i]);
		printf("%d ", arr32[i]);
		//printf("%f ", arr_double[i]);
		//printf("%f ", arr_float[i]);
	}
    printf("\n");

    /* Освлбождение блока памяти */
	//pIMem->pVTbl->Free(pIMem, arr16);
	pIMem->pVTbl->Free(pIMem, arr32);
	//pIMem->pVTbl->Free(pIMem, arr_double);
	//pIMem->pVTbl->Free(pIMem, arr_float);
    pIMem->pVTbl->Free(pIMem, name);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }

	/* Освобождение тестируемого интерфейса */
    if (pIEcoCalculatorX != 0) {
        pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);
    }

	/* Освобождение тестируемого интерфейса */
    if (pIEcoCalculatorY != 0) {
        pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

