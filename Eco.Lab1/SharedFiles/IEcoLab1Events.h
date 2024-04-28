﻿/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IEcoLab1
 * </описание>
 *
 * <ссылка>
 *
 * </ссылка>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECOLAB1EVENTS_H__
#define __I_ECOLAB1EVENTS_H__

#include "IEcoBase1.h"

/* IEcoLab1Events IID = {B6D38661-6827-46D9-9135-FD4B204A68D7} */
#ifndef __IID_IEcoLab1Events
static const UGUID IID_IEcoLab1Events = {0x01, 0x10, 0xB6, 0xD3, 0x86, 0x61, 0x68, 0x27, 0x46, 0xD9, 0x91, 0x35, 0xFD, 0x4B, 0x20, 0x4A, 0x68, 0xD7};
#endif /* __IID_IEcoLab1Events */

/* Обратный интерфейс */
typedef struct IEcoLab1VTblEvents {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface )(/* in */ struct IEcoLab1Events* me, /* in */ const UGUID* riid, /* out */ void **ppv);
    uint32_t (ECOCALLMETHOD *AddRef )(/* in */ struct IEcoLab1Events* me);
    uint32_t (ECOCALLMETHOD *Release )(/* in */ struct IEcoLab1Events* me);

    /* IEcoLab1Events */
    int16_t (ECOCALLMETHOD *OnMyCallback)(/* in */ struct IEcoLab1Events* me, /* in */ char_t* Name);
    
	int16_t (ECOCALLMETHOD *OnPrintDividingType)(/* in */ struct IEcoLab1Events* me, /* in */ int16_t type);
	int16_t (ECOCALLMETHOD *OnNewStepSize)(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t step);
	int16_t (ECOCALLMETHOD *OnPrintIntArray)(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[]);
	int16_t (ECOCALLMETHOD *OnPrintIntArrayBeforeSorting)(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[]);
	int16_t (ECOCALLMETHOD *OnPrintIntArrayAfterSorting)(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[]);
	int16_t (ECOCALLMETHOD *OnPrintColoredIntArray)(/* in */ struct IEcoLab1Events* me, /* in */ uint32_t arr_size, /* in */ void* arr[],
		/* in */ uint32_t idx_to_color_size, /* in */ int32_t* idx_to_color[], int16_t is_first_print);

} IEcoLab1VTblEvents, *IEcoLab1VTblEventsPtr;

interface IEcoLab1Events {
    struct IEcoLab1VTblEvents *pVTbl;
} IEcoLab1Events;


#endif /* __I_ECOLAB1EVENTS_H__ */