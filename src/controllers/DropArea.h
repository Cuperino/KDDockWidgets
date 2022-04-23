/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

/**
 * @file
 * @brief A MultiSplitter with support for drop indicators when hovering over.
 *
 * @author Sérgio Martins \<sergio.martins@kdab.com\>
 */

#ifndef KD_DROP_AREA_P_H
#define KD_DROP_AREA_P_H
#pragma once

#include "kddockwidgets/docks_export.h"
#include "kddockwidgets/KDDockWidgets.h"

#include "controllers/Frame.h"
#include "private/MultiSplitter_p.h"
#include "private/DropIndicatorOverlayInterface_p.h"

class TestDocks;

namespace KDDockWidgets {

class Draggable;
struct WindowBeingDragged;

namespace Controllers {
class Frame;
class DockWidget;

/**
 * @brief A MultiSplitter with support for drop indicators when hovering over.
 */
class DOCKS_EXPORT DropArea : public MultiSplitter
{
    Q_OBJECT
public:
    explicit DropArea(View *parent, MainWindowOptions options, bool isMDIWrapper = false);
    ~DropArea();

    void removeHover();
    DropLocation hover(WindowBeingDragged *draggedWindow, QPoint globalPos);
    ///@brief Called when a user drops a widget via DND
    bool drop(WindowBeingDragged *droppedWindow, QPoint globalPos);
    Controllers::Frame::List frames() const;

    Layouting::Item *centralFrame() const;
    DropIndicatorOverlayInterface *dropIndicatorOverlay() const
    {
        return m_dropIndicatorOverlay;
    }
    void addDockWidget(DockWidget *, KDDockWidgets::Location location,
                       DockWidget *relativeTo, InitialOption = {});

    bool containsDockWidget(DockWidget *) const;

    /// Returns whether this layout has a single dock widget which is floating
    /// Implies it's in a FloatingWindow and that it has only one dock widget
    bool hasSingleFloatingFrame() const;

    /// Returns whether this drop area has only 1 frame.
    /// See further explanation in FloatingWindow::hasSingleFrame()
    bool hasSingleFrame() const;

    QStringList affinities() const;
    void layoutParentContainerEqually(DockWidget *);

    /// When DockWidget::Option_MDINestable is used, docked MDI dock widgets will be wrapped inside a DropArea, so they accept drops
    /// This DropArea is created implicitly while docking, and this function will return true
    bool isMDIWrapper() const;

    /// Returns the helper dock widget for implementing DockWidget::Option_MDINestable.
    Controllers::DockWidget *mdiDockWidgetWrapper() const;

    static Controllers::Frame *createCentralFrame(MainWindowOptions options);

private:
    Q_DISABLE_COPY(DropArea)
    friend class Controllers::MainWindow;
    friend class Controllers::Frame;
    friend class Controllers::FloatingWindow;
    friend class ::TestDocks;
    friend class ::TestQtWidgets;
    friend class DropIndicatorOverlayInterface;
    friend class AnimatedIndicators;

    template<typename T>
    bool validateAffinity(T *, Controllers::Frame *acceptingFrame = nullptr) const;
    bool drop(WindowBeingDragged *draggedWindow, Controllers::Frame *acceptingFrame, DropLocation);
    bool drop(View *droppedwindow, KDDockWidgets::Location location, Controllers::Frame *relativeTo);
    Controllers::Frame *frameContainingPos(QPoint globalPos) const;
    void updateFloatingActions();

    bool m_inDestructor = false;
    const bool m_isMDIWrapper;
    QString m_affinityName;
    DropIndicatorOverlayInterface *m_dropIndicatorOverlay = nullptr;
    Controllers::Frame *const m_centralFrame = nullptr;
};
}
}

#endif