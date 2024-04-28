/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1Sink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1Sink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"

#define BACKGROUND_ORANGE 0x0060

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_QueryInterface(/* in */ struct IEcoLab1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    if ( IsEqualUGUID(riid, &IID_IEcoLab1Events ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }

    *ppv = 0;

    return -1;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_AddRef(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    pCMe->m_cRef++;
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_Release(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1Sink((IEcoLab1Events*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция OnMyCallback
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnMyCallback(/* in */ struct IEcoLab1Events* me, /* in */ char_t* Name) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

	printf("Test callback\n");

    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_OnPrintDividingType(/* in */ struct IEcoLab1Events* me, /* in */ int16_t type){
	CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	// Настройка вывода в консоль
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    if (me == 0 ) {
        return -1;
    }

	// Сохранение текущих атрибутов
    GetConsoleScreenBufferInfo(handle, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(handle, BACKGROUND_ORANGE);

	if(type == 0){
		printf("Shell's ");
	}
	else if(type == 1){
		printf("Sedgwick's ");
	}
	printf("dividing\n");

	// Восстановление атрибутов
	SetConsoleTextAttribute(handle, saved_attributes);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnNewStepSize
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnNewStepSize(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t step) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

	printf("Step size %d:\n", step);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnPrintIntArray
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnPrintIntArray(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[]){
	CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	size_t i = 0;
	// Настройка вывода в консоль
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

	if (me == 0 ) {
        return -1;
    }

	// Сохранение текущих атрибутов
    GetConsoleScreenBufferInfo(handle, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    //SetConsoleTextAttribute(handle, BACKGROUND_BLUE/* | BACKGROUND_RED | BACKGROUND_GREEN*/ | BACKGROUND_INTENSITY);
	for(i = 0; i != arr_size; ++i){
		printf("%d ", arr[i]);
	}
	printf("\n");

	// Восстановление атрибутов
	SetConsoleTextAttribute(handle, saved_attributes);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnPrintIntArrayBeforeSorting
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnPrintIntArrayBeforeSorting(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[]){
	CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	size_t i = 0;
	// Настройка вывода в консоль
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

	if (me == 0 ) {
        return -1;
    }

	// Сохранение текущих атрибутов
    GetConsoleScreenBufferInfo(handle, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(handle, BACKGROUND_RED | BACKGROUND_INTENSITY);
	printf("Unsorted array:\n");

	// Восстановление атрибутов
	SetConsoleTextAttribute(handle, saved_attributes);

	for(i = 0; i != arr_size; ++i){
		printf("%d ", arr[i]);
	}
	printf("\n");

	// Восстановление атрибутов
	SetConsoleTextAttribute(handle, saved_attributes);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnPrintIntArrayAfterSorting
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnPrintIntArrayAfterSorting(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[]){
	CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	size_t i = 0;
	// Настройка вывода в консоль
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

	if (me == 0 ) {
        return -1;
    }

	// Сохранение текущих атрибутов
    GetConsoleScreenBufferInfo(handle, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(handle, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	printf("Sorted array:\n");

	// Восстановление атрибутов
	SetConsoleTextAttribute(handle, saved_attributes);

	for(i = 0; i != arr_size; ++i){
		printf("%d ", arr[i]);
	}
	printf("\n");

	// Восстановление атрибутов
	SetConsoleTextAttribute(handle, saved_attributes);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnPrintColoredIntArray
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnPrintColoredIntArray(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[],
	/* in */ uint32_t idx_to_color_size, /* in */ int32_t* idx_to_color[], int16_t is_first_print){
	CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;
	size_t i = 0, j = 0;
	// Настройка вывода в консоль
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

	if (me == 0 ) {
        return -1;
    }

	// Сохранение текущих атрибутов
    GetConsoleScreenBufferInfo(handle, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

	if(is_first_print != 1){
		printf("\r");
	}

	for(i = 0; i != arr_size; ++i){
		if(j < idx_to_color_size && i == (size_t)idx_to_color[j]){
			SetConsoleTextAttribute(handle, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
			printf("%d", arr[i]);
			SetConsoleTextAttribute(handle, saved_attributes);
			printf(" ");
			++j;
		}
		else{
			printf("%d ", arr[i]);
		}
	}
	//printf("\n");

	// Восстановление атрибутов
	SetConsoleTextAttribute(handle, saved_attributes);
	Sleep(1500);
    return 0;
}

/*
 *
 * <сводка>
 *   Функция Advise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Advise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint* pCP = 0;
    int16_t result = 0;

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);

    if (result == 0 && pCPC != 0) {
        result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
        pCPC->pVTbl->Release(pCPC);
        pCPC = 0;
        if (result == 0 && pCP != 0) {

            result = pCP->pVTbl->Advise(pCP, (IEcoUnknown*)me, &me->m_cCookie);
            pCP->pVTbl->Release(pCP);
            pCP = 0;
        }
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция Unadvise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Unadvise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint * pCP = 0;
    int16_t result = 0;

    if (me->m_cCookie) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);
        if (result == 0) {
            result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
            pCPC->pVTbl->Release(pCPC);
            pCPC = 0;
            if (result == 0) {
                result = pCP->pVTbl->Unadvise(pCP, me->m_cCookie);
                pCP->pVTbl->Release(pCP);
                pCP = 0;
            }
        }
    }
    return result;
}

/* Create Virtual Table IEcoLab1VTblEvents */
IEcoLab1VTblEvents g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents = {
    CEcoLab1Sink_QueryInterface,
    CEcoLab1Sink_AddRef,
    CEcoLab1Sink_Release,
    CEcoLab1Sink_OnMyCallback,
	CEcoLab1Sink_OnPrintDividingType,
	CEcoLab1Sink_OnNewStepSize,
    CEcoLab1Sink_OnPrintIntArray,
	CEcoLab1Sink_OnPrintIntArrayBeforeSorting,
	CEcoLab1Sink_OnPrintIntArrayAfterSorting,
	CEcoLab1Sink_OnPrintColoredIntArray
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
int16_t ECOCALLMETHOD createCEcoLab1Sink(/* in */ IEcoMemoryAllocator1* pIMem, /* out */ IEcoLab1Events** ppIEcoLab1Events) {
    int16_t result = -1;
    CEcoLab1Sink* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoLab1Events == 0 || pIMem == 0 ) {
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1Sink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Sink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;
    pCMe->m_pIMem->pVTbl->AddRef(pCMe->m_pIMem);

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoP2PEvents */
    pCMe->m_pVTblIEcoLab1Events = &g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents;

    *ppIEcoLab1Events = (IEcoLab1Events*)pCMe;

    return 0;
};

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
void ECOCALLMETHOD deleteCEcoLab1Sink(IEcoLab1Events* pIEcoLab1Events) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)pIEcoLab1Events;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1Events != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
};
