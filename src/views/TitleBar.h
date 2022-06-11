/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2020-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once


namespace KDDockWidgets {

namespace Views {


/// @brief The interface that TitleBar views should implement
class TitleBar
{
public:
    virtual void updateMaximizeButton() = 0;
#ifdef DOCKS_DEVELOPER_MODE
    virtual bool isFloatButtonVisible() const = 0;
    virtual bool isCloseButtonVisible() const = 0;
    virtual bool isCloseButtonEnabled() const = 0;
#endif
};

}

}
