// SimpleDisplay.cpp
#include "SimpleDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

SimpleDisplay::SimpleDisplay(Adafruit_ST7735 &tft, MenuItem rootItem)
    : tft(tft), rootItem(rootItem)
{
  textColor = ST7735_WHITE;
  textBackground = ST7735_BLACK;
  selectedTextColor = ST7735_BLACK;
  selectedTextBackground = ST7735_WHITE;
  lastScrollTime = 0;
  scrollOffset = 0;
  scrolling = false;
  dirty = std::vector<bool>(WINDOW_SIZE, true);
}

void SimpleDisplay::renderTextWithScroll(int i)
{
  String text = rootItem.subMenuItems[i].label;
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
    if (i >= rootItem.subMenuItems.size())
      break;

    if (!dirty[ctr])
    {
      ctr++;
      continue;
    }

    tft.setCursor(ITEM_OFFSET + 1, startY + ctr * 24); // 24 pixels = 16px for text height + 8px for spacing

    if (ctr == selectedIndex)
    {
      renderTextRow(startY + ctr * 24, rootItem.subMenuItems[i].label, selectedTextColor, selectedTextBackground);
    }
    else
    {
      renderTextRow(startY + ctr * 24, rootItem.subMenuItems[i].label, textColor, textBackground);
    }
    dirty[ctr] = false;
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
  Serial.print(rootItem.subMenuItems[displayWindowIndex].label);
  Serial.print(". Window end - ");
  Serial.println(rootItem.subMenuItems[displayWindowIndex + 5].label);
}

void SimpleDisplay::navigateUp()
{
  int newIndex = clampIndex(selectedIndex, -1);
  displayWindowIndex = handleWindow(selectedIndex - 1);
  selectedIndex = newIndex;
  renderText();
}

void SimpleDisplay::navigateDown()
{

  int newIndex = clampIndex(selectedIndex, 1);
  displayWindowIndex = handleWindow(selectedIndex + 1);
  selectedIndex = newIndex;
  renderText();
}

int SimpleDisplay::clampIndex(int index, int change)
{
  if (index + change < 0)
  {
    dirty[index] = true;
    dirty[0] = true;
    return 0;
  }
  else if (index + change >= WINDOW_SIZE)
  {
    dirty[index] = true;
    dirty[WINDOW_SIZE - 1] = true;
    return WINDOW_SIZE - 1;
  }
  else
  {
    dirty[index] = true;
    dirty[index + change] = true;
    return index + change;
  }
}

int SimpleDisplay::handleWindow(int index)
{
  if (index < 0 && displayWindowIndex != 0)
  {
    dirty = std::vector<bool>(WINDOW_SIZE, true);
    return displayWindowIndex - 1;
  }
  else if (index >= WINDOW_SIZE && displayWindowIndex + WINDOW_SIZE < rootItem.subMenuItems.size())
  {
    dirty = std::vector<bool>(WINDOW_SIZE, true);
    return displayWindowIndex + 1;
  }

  return displayWindowIndex;
}

void SimpleDisplay::renderTextRow(int y, String text, uint16_t textColor, uint16_t textBackground)
{
  tft.setTextColor(textColor);
  tft.fillRect(ITEM_OFFSET, y, 118, 16, textBackground);
  tft.print(text);
}