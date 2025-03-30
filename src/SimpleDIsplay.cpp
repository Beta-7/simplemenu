// SimpleDisplay.cpp
#include "SimpleDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

SimpleDisplay::SimpleDisplay(Adafruit_ST7735& tft, MenuItem menuItems[], int menuItemCount)
  : tft(tft), menuItems(menuItems), menuItemCount(menuItemCount), selectedIndex(0), displayWindowIndex(0) {
  textColor = ST7735_WHITE;
  textBackground = ST7735_BLACK;
  selectedTextColor = ST7735_BLACK;
  selectedTextBackground = ST7735_WHITE;
  lastScrollTime = 0;
  scrollOffset = 0;
  scrolling = false;
}

void SimpleDisplay::renderTextWithScroll(int i) {
  String text = menuItems[i].label;
  // Get the bounding box of the text
  int16_t x, y;
  uint16_t textWidth, textHeight;
  tft.getTextBounds(text, 10, 40 + selectedIndex * 24, &x, &y, &textWidth, &textHeight); // Get text width and height

  int displayWidth = 128;  // Adjust based on your screen width
  
  if (textWidth > displayWidth) {
    if (!scrolling) {
      scrolling = true;
      lastScrollTime = millis();  // Start the scroll when the item is selected
    }
    unsigned long currentTime = millis();
    if (currentTime - lastScrollTime > 1) {  // Delay for smooth scrolling (adjust speed)
      lastScrollTime = currentTime;
      scrollOffset--;
      if (scrollOffset < -textWidth) {
        scrollOffset = displayWidth;
      }
      tft.setCursor(10 + scrollOffset, 40 + selectedIndex * 24);
      tft.print(text);
    }
  } else {
    tft.print(text);  // No scroll needed
  }
}

void SimpleDisplay::renderText() {
  int startY = 40;
  int ctr = 0;
  tft.setTextSize(2);
  tft.setTextWrap(false);

  for (int i = displayWindowIndex; i < displayWindowIndex + WINDOW_SIZE; i++) {
    if (i >= menuItemCount) break;  // Don't render beyond the array length
    
    tft.setCursor(10, startY + ctr * 24); // 24 pixels = 16px for text height + 8px for spacing
    if (ctr == selectedIndex) {
      tft.setTextColor(selectedTextColor);
      tft.fillRect(10, startY + ctr * 24, 118, 16, selectedTextBackground);
      renderTextWithScroll(i);  // Render the selected item with scrolling effect
    } else {
      tft.setTextColor(textColor);
      tft.fillRect(10, startY + ctr * 24, 118, 16, textBackground);
      tft.print(menuItems[i].label);
    }
    ctr++;
  }
}

void SimpleDisplay::navigateRight()
{
}

void SimpleDisplay::navigateLeft()
{
  Serial.print("Selected index - ");
  Serial.println(selectedIndex);
  Serial.print("Window start - ");
  Serial.print(menuItems[displayWindowIndex].label);
  Serial.print(". Window end - ");
  Serial.println(menuItems[displayWindowIndex + 5].label);
}

void SimpleDisplay::navigateUp()
{
  selectedIndex = clampIndex(--selectedIndex);
  renderText();
}

void SimpleDisplay::navigateDown()
{
  selectedIndex = clampIndex(++selectedIndex);
  renderText();
}

int SimpleDisplay::clampIndex(int index)
{
  if (index < 0)
  {
    if (displayWindowIndex != 0)
    {
      displayWindowIndex--;
    }
    return 0;
  }
  else if (index >= WINDOW_SIZE)
  {
    if (displayWindowIndex + WINDOW_SIZE < menuItemCount)
    {
      displayWindowIndex++;
    }
    return WINDOW_SIZE - 1;
  }
  else
  {
    return index;
  }
}
