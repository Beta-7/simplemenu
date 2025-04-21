#include "MenuItem.h"

MenuItem::MenuItem(String label)
    : label(label), menuItemCount(0), dirty(true) {}

MenuItem::MenuItem(String label, const std::vector<MenuItem>& subMenuItems)
    : label(label), subMenuItems(subMenuItems), menuItemCount(subMenuItems.size()), dirty(true) {}

MenuItem::MenuItem(String label, std::function<void(Adafruit_ST7735&)> onSelect)
    : label(label), onSelect(onSelect), menuItemCount(0), dirty(true) {}

void MenuItem::addSubMenu(const std::vector<MenuItem>& items) {
    subMenuItems = items;
    menuItemCount = items.size();
    dirty = true;
}
