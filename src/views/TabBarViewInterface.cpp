/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2020-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "TabBarViewInterface.h"
#include "kddockwidgets/core/TabBar.h"


namespace KDDockWidgets::Views {

TabBarViewInterface::TabBarViewInterface(Core::TabBar *controller)
    : m_tabBar(controller)
{
}

TabBarViewInterface::~TabBarViewInterface() = default;

void TabBarViewInterface::setTabsAreMovable(bool)
{
    // Not implemented by default
}


} // namespace
