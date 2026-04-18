#pragma once

#include <TFT_eSPI.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>


#define GFXFF 1

// ---------------------------
// GRAPH CONFIG
// ---------------------------
const int GRAPH_X = 10;
const int GRAPH_Y = 40;
const int GRAPH_W = 280;
const int GRAPH_H = 180;

void drawGraph(bool bufferFilled,int voltageBuffer[GRAPH_W],int bufferIndex);
void showValue(const String& value);
void showHelloWorld();
