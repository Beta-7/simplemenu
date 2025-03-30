#ifndef MENUITEM_H
#define MENUITEM_H

#include <Arduino.h>

class MenuItem {
  public:
    String label;

    MenuItem(String label);
};

#endif