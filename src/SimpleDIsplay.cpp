// SimpleDisplay.cpp
#include "SimpleDisplay.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

SimpleDisplay::SimpleDisplay(Adafruit_ST7735 &tft, MenuItem menu)
    : tft(tft), menu(menu)
{
  textColor = ST7735_WHITE;
  textBackground = ST7735_BLACK;
  selectedTextColor = ST7735_BLACK;
  selectedTextBackground = ST7735_WHITE;
  lastScrollTime = 0;
  scrollOffset = 0;
  scrolling = false;
  shownMenu = &this->menu;
  menuPath.push_back(shownMenu);
}

void SimpleDisplay::renderTextWithScroll(int i)
{
  String text = (*shownMenu).subMenuItems[i].label;
  int16_t x, y;
  uint16_t textWidth, textHeight;
  tft.getTextBounds(text, ITEM_OFFSET, 40 + selectedIndex * 24, &x, &y, &textWidth, &textHeight);

  if (textWidth <= DISPLAY_WIDTH)
    return;

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
      scrollOffset = DISPLAY_WIDTH;
    }
    tft.setCursor(ITEM_OFFSET + scrollOffset, 40 + selectedIndex * 24);
    tft.setTextColor(selectedTextColor);

    tft.print(text);
  }
}

void SimpleDisplay::renderText()
{
  tft.setTextWrap(false);

  // Breadcrumb
  tft.setTextSize(1);
  tft.fillRect(0, 0, DISPLAY_WIDTH, 16, textBackground);
  if (menuPath.size() > 1)
  {
    String parentLabel = "< " + menuPath[menuPath.size() - 2]->label;
    tft.setCursor(ITEM_OFFSET, 4);
    tft.setTextColor(textColor);
    tft.print(parentLabel);
  }

  // Title
  tft.setTextSize(2);
  tft.fillRect(0, 16, DISPLAY_WIDTH, 20, textBackground);
  tft.setCursor(ITEM_OFFSET, 18);
  tft.setTextColor(textColor);
  tft.print(shownMenu->label);

  // Divider
  tft.drawFastHLine(0, 38, DISPLAY_WIDTH, textColor);

  // Items
  int startY = 40;
  int ctr = 0;
  tft.setTextSize(2);

  for (int i = 0; i < WINDOW_SIZE; i++)
  {
    int menuIndex = displayWindowIndex + i;

    bool isInBounds = menuIndex < shownMenu->subMenuItems.size();
    MenuItem* item = isInBounds ? &shownMenu->subMenuItems[menuIndex] : nullptr;

    if (item && !item->dirty)
    {
      ctr++;
      continue;
    }

    uint16_t backgroundColor = (i == selectedIndex) ? selectedTextBackground : textBackground;
    uint16_t textColor2 = (i == selectedIndex) ? selectedTextColor : textColor;

    String label = isInBounds ? item->label : "";

    tft.setCursor(ITEM_OFFSET + 1, startY + i * 24);
    renderTextRow(startY + i * 24, label, textColor2, backgroundColor);

    if (item) item->dirty = false;
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
  int actualIndex = displayWindowIndex + selectedIndex;

  if (actualIndex < 0 || actualIndex >= shownMenu->subMenuItems.size()) {
    Serial.println("navigateRight: Invalid menu index");
    return;
  }

  MenuItem &selectedItem = shownMenu->subMenuItems[actualIndex];

  if (!selectedItem.subMenuItems.empty()) {
    shownMenu = &selectedItem;
    menuPath.push_back(shownMenu);
    selectedIndex = 0;
    displayWindowIndex = 0;
    markAllDirty();
    renderText();
  } else if (selectedItem.onSelect) {
    Serial.print("navigateRight: Triggering action for ");
    Serial.println(selectedItem.label);
    selectedItem.onSelect();
  } else {
    Serial.println("navigateRight: No submenu or action");
  }
}

void SimpleDisplay::navigateLeft()
{
  if (menuPath.size() > 1)
  {
    menuPath.pop_back();
    shownMenu = menuPath.back();
    selectedIndex = 0;
    displayWindowIndex = 0;
    markAllDirty();
    renderText();
  }
  else
  {
    Serial.println("Already at top-level menu!");
  }
}

void SimpleDisplay::navigateUp()
{
  int newIndex = clampIndex(selectedIndex, -1);
  displayWindowIndex = handleWindow(selectedIndex - 1);
  selectedIndex = newIndex;
  markItemDirty(selectedIndex);
  renderText();
}

void SimpleDisplay::navigateDown()
{
  int newIndex = clampIndex(selectedIndex, 1);
  displayWindowIndex = handleWindow(selectedIndex + 1);
  selectedIndex = newIndex;
  markItemDirty(selectedIndex);
  renderText();
}

int SimpleDisplay::clampIndex(int index, int change)
{
  int newIndex = index + change;
  if (newIndex < 0 || newIndex >= shownMenu->subMenuItems.size()) {
    markItemDirty(index);
    return index;
  } else {
    markItemDirty(index);
    markItemDirty(newIndex);
    return newIndex;
  }
}

int SimpleDisplay::handleWindow(int index)
{
  if (index < 0 && displayWindowIndex != 0) {
    markAllDirty();
    return displayWindowIndex - 1;
  } else if (index >= WINDOW_SIZE && displayWindowIndex + WINDOW_SIZE < shownMenu->subMenuItems.size()) {
    markAllDirty();
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

void SimpleDisplay::markAllDirty() {
  for (auto& item : shownMenu->subMenuItems) {
    item.dirty = true;
  }
}

void SimpleDisplay::markItemDirty(int relativeIndex) {
  int actualIndex = displayWindowIndex + relativeIndex;
  if (actualIndex >= 0 && actualIndex < shownMenu->subMenuItems.size()) {
    shownMenu->subMenuItems[actualIndex].dirty = true;
  }
}