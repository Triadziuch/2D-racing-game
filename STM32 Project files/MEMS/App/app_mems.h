/**
  ******************************************************************************
  * File Name          : app_mems.h
  * Description        : This file provides code for the configuration
  *                      of the STMicroelectronics.X-CUBE-MEMS1.10.0.0 instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_MEMS_H
#define __APP_MEMS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported defines ----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void MX_MEMS_Init(void);
void MX_MEMS_Process(void);
int get_pomiar_elapsed_time_us(void);
int get_pomiar_time_stamp(void);
int get_pomiar_gettick(void);
double *get_pomiar_gyro(void);
double *get_pomiar_acc(void);
double *get_pomiar_mag(void);
double *get_pomiar_quaternion(void);
double *get_pomiar_rotation(void);
double *get_pomiar_gravity(void);
double *get_pomiar_linear_acceleration(void);
double get_pomiar_heading(void);
double get_pomiar_headingErr(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_MEMS_H */
