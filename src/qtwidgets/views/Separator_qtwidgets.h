/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once

#include "View_qtwidgets.h"
#include "kddockwidgets/Qt5Qt6Compat_p.h"


namespace KDDockWidgets::Core {
class Separator;
}

namespace KDDockWidgets::Views {

class DOCKS_EXPORT Separator_qtwidgets : public View_qtwidgets<QWidget>
{
    Q_OBJECT
public:
    explicit Separator_qtwidgets(Core::Separator *controller, View *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(KDDockWidgets::Qt5Qt6Compat::QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;

private:
    Core::Separator *const m_controller;
};

}
