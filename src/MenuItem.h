#ifndef MENUITEM_H
#define MENUITEM_H

#include <Arduino.h>
#include <vector>

class MenuItem {
  public:
    String label;
    std::vector<MenuItem> subMenuItems;

    int menuItemCount;

    MenuItem(String label);

    MenuItem(String label, const std::vector<MenuItem>& subMenuItems);

    void addSubMenu(const std::vector<MenuItem>& items);
};

#endif
