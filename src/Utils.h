#pragma once

#include "ofMain.h"

#ifdef __cplusplus
	extern "C" {
#endif

void setFrameRateAsWindowTitle();
void setupOF(int fps = 60); // 60 FPS, vertical sync, dark background
#ifdef __cplusplus
	}
#endif