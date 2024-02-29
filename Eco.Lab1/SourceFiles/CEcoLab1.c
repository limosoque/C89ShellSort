/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Calculate Sedgewick Steps
 * </сводка>
 *
 * <описание>
 *   Функция вычисления длин промежутков для сортировки Шелла
 * </описание>
 *
 */
int16_t CalculateSedgewickSteps(/* in */ uint32_t steps[], /* in */ uint32_t size) {
	int16_t p1 = 1, p2 = 1, p3 = 1, i = -1;
	do {
		if (++i % 2 == 0) {
			steps[i] = 9 * p1 - 9 * p3 + 1;
			p2 *= 2;
			p3 *= 2;
		}
		else {
			steps[i] = 8 * p1 - 6 * p2 + 1;
		}
		p1 *= 2;
	} while (3 * steps[i] < size);

	//Возвращаем размер массива
	return i > 0 ? --i : 0;
}

/*
 *
 * <сводка>
 *   Функция Insertion sort
 * </сводка>
 *
 * <описание>
 *   Функция сортировки вставками
 *   Для сортировки всего массива этой функцией нужно задать step = 1
 * </описание>
 *
 */
void InsertionSort(
	/* in */ CEcoLab1* pCMe,
	/* in */ void* arr[],
	/* in */ uint32_t arr_size,
	/* in */ size_t type_size,
	/* in */ uint32_t step,
	/* in */ void* __cdecl compare(const void*, const void*)
	) {
	size_t i, j;
	char_t* temp = (char_t*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, type_size * sizeof(char_t));

    for (i = step; i < arr_size; i++) {
		memcpy(temp, (char_t*)arr + (i * type_size), type_size);
        
        for (j = i; j >= step && (int16_t)compare((char_t*)arr + (j - step) * type_size, temp) > 0; j -= step) {
			memcpy((char_t*)arr + j * type_size, (char_t*)arr + (j - step) * type_size, type_size);
        }
        
		memcpy((char_t*)arr + j * type_size, temp, type_size);
    }

	pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, temp);
}

/*
 *
 * <сводка>
 *   Функция Qsort
 * </сводка>
 *
 * <описание>
 *   Функция сортировки Шелла
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_qsort(
	/* in */ struct IEcoLab1* me,
	/* in */ uint32_t arr_size,
	/* in */ size_t type_size,
	/* in */ void* arr[],
	/* in */ void* __cdecl compare(const void*, const void*)
	) {
	 CEcoLab1* pCMe = (CEcoLab1*)me;
	
	int16_t i, j, step, steps_index;

	/* Создание массива для разбиения Сэджвика, 
	20 хватит для размера массива в несколько миллионов элементов */
	uint32_t steps[20];
	for(i = 0; i < 20; i++)
		steps[i] = -1;

	steps_index = CalculateSedgewickSteps(steps, arr_size);

	printf("Steps array:\n");
    for (i = 0; i <= steps_index; i++)
        printf("%d ", steps[i]);
    printf("\n");
	printf("Steps array:\n");
    for (i = 0; i < 20; i++)
        printf("%d ", steps[i]);
    printf("\n");

	/* Сортировка разбиением Сэджвика */
	for (i = steps_index; i >= 0; i--) {
		InsertionSort(pCMe, arr, arr_size, type_size, steps[i], compare);
	}

	/* Сортировка разбиением Шелла */
    for (step = arr_size / 2; step > 0; step /= 2) {
		InsertionSort(pCMe, arr, arr_size, type_size, step, compare);
	}

	/* Вывод отсортированного массива */
	//printf("Sorted array:\n");
	//for (i = 0; i < arr_size; i++)
	//	printf("%d ", arr[i]);
	//printf("\n");

    return 0;
}

//int16_t ECOCALLMETHOD CEcoLab1_qsort(/* in */ struct IEcoLab1* me, /* in */ uint32_t size, /* in */ int32_t arr[]) {
//    CEcoLab1* pCMe = (CEcoLab1*)me;
//	
//	int16_t i, j, step, steps_index;
//	//int16_t* steps = (int16_t*) malloc(size);
//	//for (i = 0; i < size; i++)
//	//	steps[i] = -1;
//	//steps = (int16_t*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size * sizeof(int16_t));
//	uint32_t steps[40];
//	for(i = 0; i < 40; i++)
//		steps[i] = -1;
//
//	//printf("Steps size: %d\n", size);
//	//printf("Steps array:\n");
// //   for (i = 0; i < size; i++)
// //       printf("%d: %d ", i, steps[i]);
// //   printf("\n");
//
//	steps_index = CalculateSedgewickSteps(steps, size);
//
//	printf("Steps array:\n");
//    for (i = 0; i <= steps_index; i++)
//        printf("%d ", steps[i]);
//    printf("\n");
//	printf("Steps array:\n");
//    for (i = 0; i < 40; i++)
//        printf("%d ", steps[i]);
//    printf("\n");
//
//	for (i = steps_index; i >= 0; i--) {
//		InsertionSort(arr, size, steps[i]);
//	}
//
// //   for (step = size / 2; step > 0; step /= 2) {
//	//	InsertionSort(arr, size, step);
//	//}
//
//	pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, steps);
//    return 0;
//}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;



    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);
	
    return result;
}

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_qsort
};




/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;
	
    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

	/* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
