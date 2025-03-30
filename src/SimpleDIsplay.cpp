// SimpleDisplay.cpp
#include "SimpleDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

SimpleDisplay::SimpleDisplay(Adafruit_ST7735 &tft, MenuItem menuItems[], int menuItemCount)
    : tft(tft), menuItems(menuItems), menuItemCount(menuItemCount), selectedIndex(0), displayWindowIndex(0)
{
  textColor = ST7735_WHITE;
  textBackground = ST7735_BLACK;
  selectedTextColor = ST7735_BLACK;
  selectedTextBackground = ST7735_WHITE;
  lastScrollTime = 0;
  scrollOffset = 0;
  scrolling = false;
}

void SimpleDisplay::renderTextWithScroll(int i)
{
  String text = menuItems[i].label;
  int16_t x, y;
  uint16_t textWidth, textHeight;
  tft.getTextBounds(text, ITEM_OFFSET, 40 + selectedIndex * 24, &x, &y, &textWidth, &textHeight);
  
  int displayWidth = 118;

  if (textWidth <= displayWidth)
  {
    return;
  }
  else
  {
    tft.fillRect(ITEM_OFFSET, 40 + selectedIndex * 24, 118, 16, selectedTextBackground);
    if (!scrolling)
    {
      scrolling = true;
      lastScrollTime = millis();
    }
    unsigned long currentTime = millis();
    if (currentTime - lastScrollTime > 50)
    {
      lastScrollTime = currentTime;
      scrollOffset -= 10;
      if (scrollOffset < -textWidth)
      {
        scrollOffset = displayWidth;
      }
      tft.setCursor(ITEM_OFFSET + scrollOffset, 40 + selectedIndex * 24);
      tft.setTextColor(selectedTextColor);

      tft.print(text);
    }
  }
}

void SimpleDisplay::renderText()
{
  int startY = 40;
  int ctr = 0;
  tft.setTextSize(2);
  tft.setTextWrap(false);

  for (int i = displayWindowIndex; i < displayWindowIndex + WINDOW_SIZE; i++)
  {
    if (i >= menuItemCount)
      break;

    tft.setCursor(ITEM_OFFSET+1, startY + ctr * 24); // 24 pixels = 16px for text height + 8px for spacing
    if (ctr == selectedIndex)
    {
      tft.setTextColor(selectedTextColor);
      tft.fillRect(ITEM_OFFSET, startY + ctr * 24, 118, 16, selectedTextBackground);
      tft.print(menuItems[i].label);
    }
    else
    {
      tft.setTextColor(textColor);
      tft.fillRect(ITEM_OFFSET, startY + ctr * 24, 118, 16, textBackground);
      tft.print(menuItems[i].label);
    }
    ctr++;
  }
  scrollOffset = 0;
}

void SimpleDisplay::renderTick()
{
  renderTextWithScroll(selectedIndex + displayWindowIndex);
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
