/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/


#pragma once

#include "View.h"
#include "core/views/StackViewInterface.h"

#include <QTabWidget>

#include "kdbindings/signal.h"

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QHBoxLayout;
QT_END_NAMESPACE

namespace KDDockWidgets {
namespace qtwidgets {

class DOCKS_EXPORT Stack : public View_qtwidgets<QTabWidget>, public Core::StackViewInterface
{
    Q_OBJECT
public:
    explicit Stack(Core::Stack *controller, QWidget *parent = nullptr);
    ~Stack();

    /// @brief Returns the QTabBar associated with this QTabWidget
    QTabBar *tabBar() const;

    bool isPositionDraggable(QPoint p) const override;
    void init() override;
    void setDocumentMode(bool) override;

    /// @brief Returns the controller
    Core::Stack *stack() const;

public: // TODOm3: make protected
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

    /// @brief Shows the context menu. Override to implement your own context menu.
    /// By default it's used to honour Config::Flag_AllowSwitchingTabsViaMenu
    virtual void showContextMenu(QPoint pos);

private:
    void updateMargins();
    void setupTabBarButtons();

    QHBoxLayout *m_cornerWidgetLayout = nullptr;
    QAbstractButton *m_floatButton = nullptr;
    QAbstractButton *m_closeButton = nullptr;
    KDBindings::ScopedConnection m_tabBarAutoHideChanged;

    Q_DISABLE_COPY(Stack)
};

}
}