// *****************************************************************************************************************
// Filename    : OsFifo.c
// 
// OS          : OSEK 2.2.3  
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Circulair FIFO Library implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include "OsAPIs.h"

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
uint8_t osCircularFifoQueue_IsEmpty(OsCircularFifoQueueDataType* pfifo)
{
    return (pfifo->PushPtr == pfifo->PopPtr);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
uint8_t osCircularFifoQueue_IsFull(OsCircularFifoQueueDataType* pfifo)
{
    return ((pfifo->PushPtr + 1) % pfifo->size) == pfifo->PopPtr;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
uint32_t osCircularFifoQueue_GetAvailableSize(OsCircularFifoQueueDataType* pfifo)
{
    if (pfifo->PushPtr >= pfifo->PopPtr)
    {
        return (pfifo->size - (pfifo->PushPtr - pfifo->PopPtr) - 1);
    }
    else
    {
        return (pfifo->PopPtr - pfifo->PushPtr - 1);
    }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
uint32_t osCircularFifoQueue_GetCurrentConsumption(OsCircularFifoQueueDataType* pfifo)
{
    return (pfifo->size - osCircularFifoQueue_GetAvailableSize(pfifo) - 1);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
uint8_t osCircularFifoQueue_Push(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size)
{
    if (size > osCircularFifoQueue_GetAvailableSize(pfifo))
    {
        return 0; // Not enough space
    }

    for (uint32_t i = 0; i < size; ++i)
    {
        pfifo->buf[pfifo->PushPtr] = data[i];
        pfifo->PushPtr = (pfifo->PushPtr + 1) % pfifo->size;
    }

    return 1;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
uint8_t osCircularFifoQueue_Pop(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size)
{
    if (size > osCircularFifoQueue_GetCurrentConsumption(pfifo))
    {
        return 0; // Not enough data
    }

    for (uint32_t i = 0; i < size; ++i)
    {
        data[i] = pfifo->buf[pfifo->PopPtr];
        pfifo->PopPtr = (pfifo->PopPtr + 1) % pfifo->size;
    }

    return 1;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//------------------------------------------------------------------------------------------------------------------
uint8_t osCircularFifoQueue_ReadCopy(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size)
{
    if (size > osCircularFifoQueue_GetCurrentConsumption(pfifo))
    {
        return 0; // Not enough data
    }

    uint32_t ptr = pfifo->PopPtr;
    for (uint32_t i = 0; i < size; ++i)
    {
        data[i] = pfifo->buf[ptr];
        ptr = (ptr + 1) % pfifo->size;
    }

    return 1;
}
