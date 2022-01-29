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
  PI_WKUP,
  PWR_CTL,
  CHRG_LVL_1,
  CHRG_LVL_2,
  CHRG_LVL_3,
  CHRG_LVL_4,
  PWR_MODE,
  CHRG_OK,
  AC_OK,
  IN_DG,
  IO_CNT,
};

GPIO_PinState HAL_GPIO_ReadPin_Rev(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  const GPIO_PinState res = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
  return (res == GPIO_PIN_RESET) ? GPIO_PIN_SET : GPIO_PIN_RESET;
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
    .io_id = PI_WKUP,
    .pin  = PI_WKUP_Pin,
    .port = PI_WKUP_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
  },
  {
    .io_id = PWR_CTL,
    .pin = PWR_CTL_Pin,
    .port = PWR_CTL_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin_Rev,
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
    .io_id = PWR_MODE,
    .pin = PWR_MODE_Pin,
    .port = PWR_MODE_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin,
  },
  {
    .io_id = CHRG_OK,
    .pin = CHRG_OK_Pin,
    .port = CHRG_OK_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin,
  },
  {
    .io_id = AC_OK,
    .pin = AC_OK_Pin,
    .port = AC_OK_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin,
  },
  {
    .io_id = IN_DG,
    .pin = IN_DG_Pin,
    .port = IN_DG_GPIO_Port,
    .pin_get = HAL_GPIO_ReadPin,
  },
};

void io_scan(void)
{
  static char io_str[IO_CNT + 1] = "";
  bool must_upd = false;

  for (uint8_t i = 0; i < IO_CNT; i++)
  {
    const char sts_ch[] = "-+";

    GPIO_PinState tmp_sts = io_list[i].pin_get(io_list[i].port, io_list[i].pin);
    char tmp_sts_ch = (tmp_sts == GPIO_PIN_RESET) ? sts_ch[0] : sts_ch[1];
    if (io_str[i] != tmp_sts_ch)
    {
      io_str[i] = tmp_sts_ch;
      must_upd = true;
    }
  }

  if (must_upd)
    printf("%s\n", io_str);
};
