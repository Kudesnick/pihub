/*
 * io_tester.c
 *
 *  Created on: 28 янв. 2022 г.
 *      Author: kudes
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "main.h"

enum io_id_t
{
  IN_DG,      // Input. 1 - on, 0 - off, internal OD 0 - error (overcurrent, overtemperature etc)
  PWR_MODE,   // Input. 0 - charge, 1 - discharge
  PI_WKUP,    // OD and push button. Wakeup raspberry board
  PWR_CTL,    // Input push button. Short push - discharge on, push  longer than 2.5s - discharge off
  CHRG_LVL_1, // OD LED Low lever
  CHRG_LVL_2, // OD LED
  CHRG_LVL_3, // OD LED
  CHRG_LVL_4, // OD LED Hi level
  CHRG_OK,    // OD LED Charge in progress (1s blinking - NTC or timer fault)
  AC_OK,      // OD LED Input voltage is recognized as a good source (1 discharging)
  IO_CNT,
};

GPIO_PinState HAL_GPIO_ReadPin_Rev(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  const GPIO_PinState PinState = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
  return (PinState == GPIO_PIN_RESET) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

void HAL_GPIO_WritePin_Rev(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  PinState = (PinState == GPIO_PIN_RESET) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

void HAL_GPIO_WritePin_PU(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  GPIO_InitTypeDef GPIO_InitStruct =
  {
    .Pin = GPIO_Pin,
    .Mode = GPIO_MODE_INPUT,
    .Pull = (PinState == GPIO_PIN_RESET) ? GPIO_PULLDOWN : GPIO_PULLUP,
  };

  HAL_GPIO_Init(IN_DG_GPIO_Port, &GPIO_InitStruct);
}

struct io_ctl_t
{
  const enum io_id_t        io_id;
  const uint16_t            pin;
  GPIO_TypeDef *const port;
  GPIO_PinState (* const    pin_get)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
  void (* const             pin_set)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
} io_list[IO_CNT] =
{
  {
    .io_id = IN_DG,
    .pin = IN_DG_Pin,
    .port = IN_DG_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin,
    .pin_set = HAL_GPIO_WritePin_PU,
  },
  {
    .io_id = PWR_MODE,
    .pin = PWR_MODE_Pin,
    .port = PWR_MODE_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin,
    .pin_set = HAL_GPIO_WritePin,
  },
  {
    .io_id = PI_WKUP,
    .pin  = PI_WKUP_Pin,
    .port = PI_WKUP_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
    .pin_set = HAL_GPIO_WritePin_Rev,
  },
  {
    .io_id = PWR_CTL,
    .pin = PWR_CTL_Pin,
    .port = PWR_CTL_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
    .pin_set = HAL_GPIO_WritePin_Rev,
  },
  {
    .io_id = CHRG_LVL_1,
    .pin = CHRG_LVL_1_Pin,
    .port = CHRG_LVL_1_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
  },
  {
    .io_id = CHRG_LVL_2,
    .pin = CHRG_LVL_2_Pin,
    .port = CHRG_LVL_2_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
  },
  {
    .io_id = CHRG_LVL_3,
    .pin = CHRG_LVL_3_Pin,
    .port = CHRG_LVL_3_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
  },
  {
    .io_id = CHRG_LVL_4,
    .pin = CHRG_LVL_4_Pin,
    .port = CHRG_LVL_4_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
  },
  {
    .io_id = CHRG_OK,
    .pin = CHRG_OK_Pin,
    .port = CHRG_OK_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
  },
  {
    .io_id = AC_OK,
    .pin = AC_OK_Pin,
    .port = AC_OK_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
  },
};

GPIO_PinState io_list_get(const enum io_id_t id)
{
  return (io_list[id].pin_get != NULL) ?
    io_list[id].pin_get(io_list[id].port, io_list[id].pin) :
    GPIO_PIN_RESET;
}

void io_list_set(const enum io_id_t id, const GPIO_PinState PinState)
{
  if (io_list[id].pin_set != NULL)
  {
    io_list[id].pin_set(io_list[id].port, io_list[id].pin, PinState);
  }
}

void io_ctl(void)
{
  const char in_ch = getc(stdin);
  switch (in_ch)
  {
    // Charging mode
    case 'c':
    case 'C':
      io_list_set(PWR_MODE, GPIO_PIN_RESET);
      break;

    // Disharging mode
    case 'd':
    case 'D':
      io_list_set(PWR_MODE, GPIO_PIN_SET);
      break;

    // Load on
    case 's':
    case 'S':
      io_list_set(IN_DG, GPIO_PIN_SET);
      break;

    // Load off
    case 'x':
    case 'X':
      io_list_set(IN_DG, GPIO_PIN_RESET);
      break;

    default:
      break;
  }
}

void io_observe(void)
{
  static char io_str[IO_CNT + 1] = "";
  bool must_upd = false;

  for (uint8_t i = 0; i < IO_CNT; i++)
  {
    const char sts_ch[] = "-+";

    GPIO_PinState tmp_sts = io_list_get(i);
    char tmp_sts_ch = (tmp_sts == GPIO_PIN_RESET) ? sts_ch[0] : sts_ch[1];
    if (io_str[i] != tmp_sts_ch)
    {
      io_str[i] = tmp_sts_ch;
      must_upd = true;
    }
  }

  if (must_upd)
    printf("\n%s", io_str);
}

void io_scan(void)
{
  io_ctl();
  io_observe();
};
