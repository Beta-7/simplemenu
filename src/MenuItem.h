#ifndef MENUITEM_H
#define MENUITEM_H

#include <Arduino.h>
#include <vector>

class MenuItem {
  public:
    String label;
    std::vector<MenuItem> subMenuItems;
    std::function<void()> onSelect;
    bool dirty = true;
    int menuItemCount;

    MenuItem(String label);

    MenuItem(String label, const std::vector<MenuItem>& subMenuItems);
    MenuItem(String label, std::function<void()> onSelect);

    void addSubMenu(const std::vector<MenuItem>& items);
};

#endif
