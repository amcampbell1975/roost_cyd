

#include "graphics.h"
#include "generated/banana.h"
#include "generated/rgb.h"

extern TFT_eSPI tft;

void showHelloWorld() {
  tft.fillScreen(TFT_BLACK);     // Clear screen
  //tft.setTextSize(2);
  //tft.setFreeFont(&FreeSans12pt7b);
  //tft.setFreeFont(&FreeSans9pt7b.h);
  
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(40, 20);        // Center-ish position
  tft.print("RED TEXT");
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.setCursor(40, 40);        // Center-ish position
  tft.print("BLUE TEXT");

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(40, 60);        // Center-ish position
  tft.print("GREEN TEXT");


  tft.pushImage(50, 150, rgb_width, rgb_height, rgb_img);
  tft.pushImage(150, 150, banana_width, banana_height, banana_img);

  sleep(100);



  delay(4000);
}


// ---------------------------
// DRAW GRAPH
// ---------------------------
void drawGraph_old(bool bufferFilled,int voltageBuffer[GRAPH_W],int bufferIndex) {
  // Clear graph area (inside border)
  tft.fillRect(GRAPH_X + 1, GRAPH_Y + 1, GRAPH_W - 2, GRAPH_H - 2, TFT_BLACK);

  int lastY = -1;
  int count = bufferFilled ? GRAPH_W : bufferIndex;

  for (int i = 0; i < count; i++) {
    int voltage = voltageBuffer[i];

    // Map 220–250 V to graph height
    int y = map(voltage, 220, 250, GRAPH_H - 2, 1);

    if (lastY >= 0) {
      tft.drawLine(GRAPH_X + i - 1, GRAPH_Y + lastY,
                   GRAPH_X + i,     GRAPH_Y + y,
                   TFT_GREEN);
    }

    lastY = y;
  }
}

void drawGraph(bool bufferFilled,int voltageBuffer[GRAPH_W],int bufferIndex) {
  // Clear graph area (inside border)
  tft.fillRect(GRAPH_X + 1, GRAPH_Y + 1, GRAPH_W - 2, GRAPH_H - 2, TFT_BLACK);

  // ---------------------------
  // Horizontal grid lines (5 V)
  // ---------------------------
  for (int v = 220; v <= 250; v += 5) {
    int y = map(v, 220, 250, GRAPH_H - 2, 1);
    tft.drawLine(GRAPH_X + 1, GRAPH_Y + y, GRAPH_X + GRAPH_W - 2, GRAPH_Y + y, TFT_DARKGREY);

    // Label voltage
    tft.setTextColor(TFT_WHITE, TFT_BLACK );
    // tft.setTextSize(2);
    //tft.setCursor(GRAPH_X + 3, GRAPH_Y + y - 3);
    tft.drawNumber(v,GRAPH_X + 3, GRAPH_Y + y - 3);
    Serial.printf("Test %d\n",v);
    //tft.print(v);
  }

    // ---------------------------
    // Vertical time markers
    // Every 10 samples
    // ---------------------------
    int count = bufferFilled ? GRAPH_W : bufferIndex;

    for (int i = 0; i < GRAPH_W; i++) {
        if (i % 20 == 0) {
            tft.drawLine(
                GRAPH_X + i,           // X position
                GRAPH_Y + 1,           // top of graph
                GRAPH_X + i,           // same X
                GRAPH_Y + GRAPH_H - 2, // bottom of graph
                TFT_DARKGREY               // bright visible color
            );
        }
    }


  // ---------------------------
  // Draw voltage line
  // ---------------------------
  int lastY = -1;

  for (int i = 0; i < count; i++) {
    int voltage = voltageBuffer[i];
    int y = map(voltage, 220, 250, GRAPH_H - 2, 1);

    if (lastY >= 0) {
      tft.drawLine(GRAPH_X + i - 1, GRAPH_Y + lastY,
                   GRAPH_X + i,     GRAPH_Y + y,
                   TFT_GREEN);
    }

    lastY = y;
  }

}


// ---------------------------
// DISPLAY TEXT VALUE
// ---------------------------
void showValue(const String& value) {
  tft.fillRect(0, 0, 320, 30, TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
//   tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Voltage: ");
  tft.print(value);
}