#include "MenuItem.h"

MenuItem::MenuItem(String label) : label(label) {}

MenuItem::MenuItem(String label, const std::vector<MenuItem> &subMenuItems)
    : label(label), subMenuItems(subMenuItems) {}
    
MenuItem::MenuItem(String label, std::function<void()> onSelect)
    : label(label), onSelect(onSelect), menuItemCount(0) {}

void MenuItem::addSubMenu(const std::vector<MenuItem> &items)
{
    subMenuItems = items;
}
