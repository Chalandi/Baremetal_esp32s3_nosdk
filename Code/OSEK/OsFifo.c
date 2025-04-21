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
  return((pfifo->PopPtr == pfifo->PushPtr) ? 1 : 0);
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
  uint8_t result=0;

  if((pfifo->PopPtr == 0 && (pfifo->PushPtr + 1) == pfifo->size) || (pfifo->PopPtr == (pfifo->PushPtr +1)))
  {
    result = 1;
  }
  return(result);
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
  if(pfifo->PopPtr < pfifo->PushPtr)
  {
    return((uint32_t)(pfifo->size - pfifo->PushPtr + pfifo->PopPtr));
  }
  else if(pfifo->PopPtr > pfifo->PushPtr)
  {
    return((uint32_t)(pfifo->PopPtr - pfifo->PushPtr));
  }
  else
  {
    return((uint32_t)(pfifo->size));
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
  return((uint32_t)(pfifo->size - osCircularFifoQueue_GetAvailableSize(pfifo)));
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
  if(size <= osCircularFifoQueue_GetAvailableSize(pfifo))
  {
    for(uint32_t cpt=0;cpt < size; cpt++)
    {
      if(pfifo->PushPtr == pfifo->size)
      {
        pfifo->PushPtr = 0;
      }
      pfifo->buf[pfifo->PushPtr] = data[cpt];
      pfifo->PushPtr++;
    }
    return(1);
  }
  else
  {
    /* No more space in FIFO */
    return(0);
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
uint8_t osCircularFifoQueue_Pop(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size)
{
  if(size <= osCircularFifoQueue_GetCurrentConsumption(pfifo))
  {
    /* Available data */
    for(uint32_t cpt=0;cpt < size; cpt++)
    {
      if(pfifo->PopPtr == pfifo->size)
      {
        pfifo->PopPtr = 0;
      }
      data[cpt] = pfifo->buf[pfifo->PopPtr];
      pfifo->PopPtr++;
    }
    return(1);
  }
  else
  {
    /* Not available data size */
    return(0);
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
uint8_t osCircularFifoQueue_ReadCopy(OsCircularFifoQueueDataType* pfifo, uint8_t* data, uint32_t size)
{
  if(size <= osCircularFifoQueue_GetCurrentConsumption(pfifo))
  {
    /* Available data */
    uint32_t ptr = pfifo->PopPtr;
    for(uint32_t cpt=0;cpt < size; cpt++)
    {
      if(ptr == pfifo->size)
      {
        ptr = 0;
      }
      data[cpt] = pfifo->buf[ptr];
      ptr++;
    }
    return(1);
  }
  else
  {
    /* Not available data size */
    return(0);
  }
}
