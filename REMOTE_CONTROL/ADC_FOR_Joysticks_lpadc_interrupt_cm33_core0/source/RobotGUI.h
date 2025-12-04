/*
 * source/RobotGUI.h
 * Professional GUI for MCXN947 Remote Control
 */

#ifndef ROBOT_GUI_H_
#define ROBOT_GUI_H_

#include "lvgl.h"

/* Initialize the GUI (Create screens, charts, labels) */
void RobotGUI_Init(void);

/* Update the GUI with new telemetry data
 * vx, vy: Linear speeds in m/s
 * phi: Angular speed in rad/s
 */
void RobotGUI_Update(float vx, float vy, float phi);

#endif /* ROBOT_GUI_H_ */
