#include "MenuItem.h"

MenuItem::MenuItem(String label) : label(label) {}

MenuItem::MenuItem(String label, const std::vector<MenuItem>& subMenuItems)
    : label(label), subMenuItems(subMenuItems) {}

void MenuItem::addSubMenu(const std::vector<MenuItem>& items) {
    subMenuItems = items;
}
