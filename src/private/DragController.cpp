/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "DragController_p.h"
#include "DockRegistry.h"
#include "Logging_p.h"
#include "Qt5Qt6Compat_p.h"
#include "Utils_p.h"
#include "WidgetResizeHandler_p.h"
#include "Config.h"
#include "WindowZOrder_x11_p.h"
#include "Platform.h"
#include "Window.h"

#include "kddockwidgets/controllers/MDILayout.h"
#include "kddockwidgets/controllers/DropArea.h"
#include "kddockwidgets/controllers/TitleBar.h"
#include "kddockwidgets/controllers/Group.h"
#include "kddockwidgets/controllers/FloatingWindow.h"
#include "kddockwidgets/controllers/DockWidget_p.h"

#include <QMouseEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QWindow>
#include <QDrag>
#include <QScopedValueRollback>

#ifdef KDDW_FRONTEND_QTWIDGETS
#include <QWidget>
#include <QApplication>
#endif

#if defined(Q_OS_WIN)
#include <windows.h>
#endif

using namespace KDDockWidgets;
using namespace KDDockWidgets::Controllers;

namespace KDDockWidgets {
///@brief Custom mouse grabber, for platforms that don't support grabbing the mouse
class FallbackMouseGrabber : public QObject /// clazy:exclude=missing-qobject-macro
{
public:
    FallbackMouseGrabber(QObject *parent)
        : QObject(parent)
    {
    }

    ~FallbackMouseGrabber() override;

    void grabMouse(View *target)
    {
        m_target = target;
        m_guard = target->asQObject();
        qGuiApp->installEventFilter(this);
    }

    void releaseMouse()
    {
        // Ungrab harder if QtQuick.
        // QtQuick has the habit og grabbing the MouseArea internally, then doesn't ungrab it since
        // we're consuming the events. So explicitly ungrab if any QQuickWindow::mouseGrabberItem()
        // is still set. Done via platform now, so it's generic. Should be a no-op for QtWidgets.
        Platform::instance()->ungrabMouse();

        m_target = nullptr;
        m_guard.clear();
        qGuiApp->removeEventFilter(this);
    }

    bool eventFilter(QObject *, QEvent *ev) override
    {
        if (m_reentrancyGuard || !m_guard)
            return false;

        if (QMouseEvent *me = mouseEvent(ev)) {
            m_reentrancyGuard = true;
            Platform::instance()->sendEvent(m_target, me);
            m_reentrancyGuard = false;
            return true;
        }

        return false;
    }

    bool m_reentrancyGuard = false;
    View *m_target = nullptr;
    QPointer<QObject> m_guard;
};

FallbackMouseGrabber::~FallbackMouseGrabber()
{
}

}

State::State(MinimalStateMachine *parent)
    : QObject(parent)
    , m_machine(parent)
{
}

State::~State() = default;

bool State::isCurrentState() const
{
    return m_machine->currentState() == this;
}

MinimalStateMachine::MinimalStateMachine(QObject *parent)
    : QObject(parent)
{
}

template<typename Obj, typename Signal>
void State::addTransition(Obj *obj, Signal signal, State *dest)
{
    connect(obj, signal, this, [this, dest] {
        if (isCurrentState()) {
            m_machine->setCurrentState(dest);
        }
    });
}


State *MinimalStateMachine::currentState() const
{
    return m_currentState;
}

void MinimalStateMachine::setCurrentState(State *state)
{
    if (state != m_currentState) {
        if (m_currentState)
            m_currentState->onExit();

        m_currentState = state;

        if (state)
            state->onEntry();

        Q_EMIT currentStateChanged();
    }
}

StateBase::StateBase(DragController *parent)
    : State(parent)
    , q(parent)
{
}

bool StateBase::isActiveState() const
{
    return q->activeState() == this;
}

StateBase::~StateBase() = default;

StateNone::StateNone(DragController *parent)
    : StateBase(parent)
{
}

void StateNone::onEntry()
{
    qCDebug(state) << "StateNone entered";
    q->m_pressPos = QPoint();
    q->m_offset = QPoint();
    q->m_draggable = nullptr;
    q->m_draggableGuard.clear();
    q->m_windowBeingDragged.reset();
    WidgetResizeHandler::s_disableAllHandlers = false; // Re-enable resize handlers

    q->m_nonClientDrag = false;
    if (q->m_currentDropArea) {
        q->m_currentDropArea->removeHover();
        q->m_currentDropArea = nullptr;
    }

    Q_EMIT q->isDraggingChanged();
}

bool StateNone::handleMouseButtonPress(Draggable *draggable, QPoint globalPos, QPoint pos)
{
    qCDebug(state) << "StateNone::handleMouseButtonPress: draggable"
                   << draggable << "; globalPos" << globalPos;

    if (!draggable->isPositionDraggable(pos))
        return false;

    q->m_draggable = draggable;
    q->m_draggableGuard = draggable->asView()->asQObject();
    q->m_pressPos = globalPos;
    q->m_offset = draggable->mapToWindow(pos);
    Q_EMIT q->mousePressed();
    return false;
}

StateNone::~StateNone() = default;


StatePreDrag::StatePreDrag(DragController *parent)
    : StateBase(parent)
{
}

StatePreDrag::~StatePreDrag() = default;

void StatePreDrag::onEntry()
{
    qCDebug(state) << "StatePreDrag entered" << q->m_draggableGuard.data();
    WidgetResizeHandler::s_disableAllHandlers = true; // Disable the resize handler during dragging
}

bool StatePreDrag::handleMouseMove(QPoint globalPos)
{
    if (!q->m_draggableGuard) {
        qWarning() << Q_FUNC_INFO << "Draggable was destroyed, canceling the drag";
        Q_EMIT q->dragCanceled();
        return false;
    }

    if (q->m_draggable->dragCanStart(q->m_pressPos, globalPos)) {
        if (q->m_draggable->isMDI())
            Q_EMIT q->manhattanLengthMoveMDI();
        else
            Q_EMIT q->manhattanLengthMove();
        return true;
    }
    return false;
}

bool StatePreDrag::handleMouseButtonRelease(QPoint)
{
    Q_EMIT q->dragCanceled();
    return false;
}

bool StatePreDrag::handleMouseDoubleClick()
{
    // This is only needed for QtQuick.
    // With QtQuick, when double clicking, we get: Press, Release, Press, Double-click. and never
    // receive the last Release event.
    Q_EMIT q->dragCanceled();
    return false;
}

StateDragging::StateDragging(DragController *parent)
    : StateBase(parent)
{
#if defined(Q_OS_WIN) && !defined(DOCKS_DEVELOPER_MODE)
    m_maybeCancelDrag.setInterval(100);
    QObject::connect(&m_maybeCancelDrag, &QTimer::timeout, this, [this] {
        // Workaround bug #166 , where Qt doesn't agree with Window's mouse button state.
        // Looking in the Qt bug tracker there's many hits, so do a quick workaround here:

        const bool mouseButtonIsReallyDown = (GetKeyState(VK_LBUTTON) & 0x8000);
        if (!mouseButtonIsReallyDown && isLeftButtonPressed()) {
            qCDebug(state) << "Canceling drag, Qt thinks mouse button is pressed"
                           << "but Windows knows it's not";
            Q_EMIT q->dragCanceled();
        }
    });
#endif
}

StateDragging::~StateDragging() = default;

void StateDragging::onEntry()
{
    m_maybeCancelDrag.start();

    if (Controllers::DockWidget *dw = q->m_draggable->singleDockWidget()) {
        // When we start to drag a floating window which has a single dock widget, we save the position
        if (dw->isFloating())
            dw->d->saveLastFloatingGeometry();
    }

    const bool needsUndocking = !q->m_draggable->isWindow();
    q->m_windowBeingDragged = q->m_draggable->makeWindow();
    if (q->m_windowBeingDragged) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0) && defined(Q_OS_WIN)
        if (!q->m_nonClientDrag && KDDockWidgets::usesNativeDraggingAndResizing()) {
            // Started as a client move, as the dock widget was docked,
            // but now that we're dragging it as a floating window, switch to native drag, so we can still get aero-snap
            FloatingWindow *fw = q->m_windowBeingDragged->floatingWindow();
            q->m_nonClientDrag = true;
            q->m_windowBeingDragged.reset();
            q->m_windowBeingDragged = fw->makeWindow();

            Window::Ptr window = fw->view()->window();

            if (needsUndocking) {
                // Position the window before the drag start, otherwise if you move mouse too fast there will be an offset
                // Only required when we've undocked/detached a window.
                window->setPosition(QCursor::pos() - q->m_offset);
            }

            // Start the native move
            window->startSystemMove();
        }
#else
        Q_UNUSED(needsUndocking);
#endif

        qCDebug(state) << "StateDragging entered. m_draggable=" << q->m_draggable
                       << "; m_windowBeingDragged=" << q->m_windowBeingDragged->floatingWindow();

        auto fw = q->m_windowBeingDragged->floatingWindow();
#ifdef Q_OS_LINUX
        if (fw->isMaximizedOverride()) {
            // When dragging a maximized window on linux we need to restore its normal size
            // On Windows this works already. On macOS I don't see this feature at all

            const QRect normalGeometry = fw->normalGeometry();

            // distance to the left edge of the window:
            const int leftOffset = q->m_offset.x();

            // distance to the right edge of the window:
            const int rightOffset = fw->width() - q->m_offset.x();

            const bool leftEdgeIsNearest = leftOffset <= rightOffset;

            fw->showNormal();

            if (!normalGeometry.contains(q->m_pressPos)) {
                if ((leftEdgeIsNearest && leftOffset > normalGeometry.width()) || (!leftEdgeIsNearest && rightOffset > normalGeometry.width())) {
                    // Case #1: The window isn't under the cursor anymore
                    // Let's just put its middle under the cursor
                    q->m_offset.setX(normalGeometry.width() / 2);
                } else if (!leftEdgeIsNearest) {
                    // Case #2: The new geometry is still under the cursor, but instead of moving its right edge left
                    // we'll move the left edge right, since initially the press position was closer to the right edge
                    q->m_offset.setX(normalGeometry.width() - rightOffset);
                }
            }
        } else
#endif

            if (!fw->geometry().contains(q->m_pressPos)) {
            // The window shrunk when the drag started, this can happen if it has max-size constraints
            // we make the floating window smaller. Has the downside that it might not be under the mouse
            // cursor anymore, so make the change
            if (fw->width() < q->m_offset.x()) { // make sure it shrunk
                q->m_offset.setX(fw->width() / 2);
            }
        }
    } else {
        // Shouldn't happen
        qWarning() << Q_FUNC_INFO << "No window being dragged for " << q->m_draggable;
        Q_EMIT q->dragCanceled();
    }

    Q_EMIT q->isDraggingChanged();
}

void StateDragging::onExit()
{
    m_maybeCancelDrag.stop();
}

bool StateDragging::handleMouseButtonRelease(QPoint globalPos)
{
    qCDebug(state) << "StateDragging: handleMouseButtonRelease";

    FloatingWindow *floatingWindow = q->m_windowBeingDragged->floatingWindow();
    if (!floatingWindow) {
        // It was deleted externally
        qCDebug(state) << "StateDragging: Bailling out, deleted externally";
        Q_EMIT q->dragCanceled();
        return true;
    }

    if (floatingWindow->anyNonDockable()) {
        qCDebug(state) << "StateDragging: Ignoring floating window with non dockable widgets";
        Q_EMIT q->dragCanceled();
        return true;
    }

    if (q->m_currentDropArea) {
        if (q->m_currentDropArea->drop(q->m_windowBeingDragged.get(), globalPos)) {
            Q_EMIT q->dropped();
        } else {
            qCDebug(state) << "StateDragging: Bailling out, drop not accepted";
            Q_EMIT q->dragCanceled();
        }
    } else {
        qCDebug(state) << "StateDragging: Bailling out, not over a drop area";
        Q_EMIT q->dragCanceled();
    }
    return true;
}

bool StateDragging::handleMouseMove(QPoint globalPos)
{
    FloatingWindow *fw = q->m_windowBeingDragged->floatingWindow();
    if (!fw) {
        qCDebug(state) << "Canceling drag, window was deleted";
        Q_EMIT q->dragCanceled();
        return true;
    }

    if (fw->beingDeleted()) {
        // Ignore, we're in the middle of recurrency. We're inside StateDragging::handleMouseButtonRelease too
        return true;
    }

#ifdef Q_OS_LINUX
    if (fw->lastWindowManagerState() == Qt::WindowMaximized) {
        // The window was maximized, we dragged it, which triggers a show normal.
        // But we can only start moving the window *after* the (async) window manager acknowledges.
        // See QTBUG-102430.
        // Since #286 was only implemented and needed on Linux, then this counter-part is also ifdefed for Linux,
        // Probably the ifdef could be removed, but don't want to be testing N platforms, who's undocumented behaviour
        // can change between releases, so narrow the scope and workaround for linux only.
        return true;
    }
#endif

    if (!q->m_nonClientDrag)
        fw->view()->window()->setFramePosition(globalPos - q->m_offset);

    if (fw->anyNonDockable()) {
        qCDebug(state) << "StateDragging: Ignoring non dockable floating window";
        return true;
    }

    DropArea *dropArea = q->dropAreaUnderCursor();
    if (q->m_currentDropArea && dropArea != q->m_currentDropArea)
        q->m_currentDropArea->removeHover();

    if (dropArea) {
        if (FloatingWindow *targetFw = dropArea->floatingWindow()) {
            if (targetFw->anyNonDockable()) {
                qCDebug(state) << "StateDragging: Ignoring non dockable target floating window";
                return false;
            }
        }

        dropArea->hover(q->m_windowBeingDragged.get(), globalPos);
    }

    q->m_currentDropArea = dropArea;

    return true;
}

bool StateDragging::handleMouseDoubleClick()
{
    // See comment in StatePreDrag::handleMouseDoubleClick().
    // Very unlikely that we're in this state though, due to manhattan length
    Q_EMIT q->dragCanceled();
    return false;
}

StateInternalMDIDragging::StateInternalMDIDragging(DragController *parent)
    : StateBase(parent)
{
}

StateInternalMDIDragging::~StateInternalMDIDragging()
{
}

void StateInternalMDIDragging::onEntry()
{
    qCDebug(state) << "StateInternalMDIDragging entered. draggable="
                   << q->m_draggable;

    // Raise the dock widget being dragged
    if (auto tb = q->m_draggable->asView()->asTitleBarController()) {
        if (Controllers::Group *f = tb->group())
            f->view()->raise();
    }

    Q_EMIT q->isDraggingChanged();
}

bool StateInternalMDIDragging::handleMouseButtonRelease(QPoint)
{
    Q_EMIT q->dragCanceled();
    return false;
}

bool StateInternalMDIDragging::handleMouseMove(QPoint globalPos)
{
    // for MDI we only support dragging via the title bar, other cases don't make sense conceptually
    auto tb = q->m_draggable->asView()->asTitleBarController();
    if (!tb) {
        qWarning() << Q_FUNC_INFO << "expected a title bar, not" << q->m_draggable;
        Q_EMIT q->dragCanceled();
        return false;
    }

    Controllers::Group *group = tb->group();
    if (!group) {
        // Doesn't happen.
        qWarning() << Q_FUNC_INFO << "null group.";
        Q_EMIT q->dragCanceled();
        return false;
    }

    const QSize parentSize = group->view()->parentSize();
    const QPoint oldPos = group->mapToGlobal(QPoint(0, 0));
    const QPoint delta = globalPos - oldPos;
    const QPoint newLocalPos = group->pos() + delta - q->m_offset;

    // Let's not allow the MDI window to go outside of its parent

    QPoint newLocalPosBounded = { qMax(0, newLocalPos.x()), qMax(0, newLocalPos.y()) };
    newLocalPosBounded.setX(qMin(newLocalPosBounded.x(), parentSize.width() - group->width()));
    newLocalPosBounded.setY(qMin(newLocalPosBounded.y(), parentSize.height() - group->height()));

    auto layout = group->mdiLayoutWidget();
    Q_ASSERT(layout);
    layout->moveDockWidget(group, newLocalPosBounded);

    // Check if we need to pop out the MDI window (make it float)
    // If we drag the window against an edge, and move behind the edge some threshold, we float it
    const int threshold = Config::self().mdiPopupThreshold();
    if (threshold != -1) {
        const QPoint overflow = newLocalPosBounded - newLocalPos;
        if (qAbs(overflow.x()) > threshold || qAbs(overflow.y()) > threshold)
            Q_EMIT q->mdiPopOut();
    }

    return false;
}

bool StateInternalMDIDragging::handleMouseDoubleClick()
{
    Q_EMIT q->dragCanceled();
    return false;
}

StateDraggingWayland::StateDraggingWayland(DragController *parent)
    : StateDragging(parent)
{
}

StateDraggingWayland::~StateDraggingWayland()
{
}

void StateDraggingWayland::onEntry()
{
    qCDebug(state) << "StateDragging entered";

    if (m_inQDrag) {
        // Maybe we can exit the state due to the nested event loop of QDrag::Exec();
        qWarning() << Q_FUNC_INFO << "Impossible!";
        return;
    }

    QScopedValueRollback<bool> guard(m_inQDrag, true);
    q->m_windowBeingDragged = std::unique_ptr<WindowBeingDragged>(new WindowBeingDraggedWayland(q->m_draggable));

    auto mimeData = new WaylandMimeData();
    QDrag drag(this);
    drag.setMimeData(mimeData);
    drag.setPixmap(q->m_windowBeingDragged->pixmap());

    qGuiApp->installEventFilter(q);
    const Qt::DropAction result = drag.exec();
    qGuiApp->removeEventFilter(q);
    if (result == Qt::IgnoreAction)
        Q_EMIT q->dragCanceled();
}

bool StateDraggingWayland::handleMouseButtonRelease(QPoint /*globalPos*/)
{
    qCDebug(state) << Q_FUNC_INFO;
    Q_EMIT q->dragCanceled();
    return true;
}

bool StateDraggingWayland::handleDragEnter(QDragEnterEvent *ev, DropArea *dropArea)
{
    auto mimeData = qobject_cast<const WaylandMimeData *>(ev->mimeData());
    if (!mimeData || !q->m_windowBeingDragged)
        return false; // Not for us, some other user drag.

    if (q->m_windowBeingDragged->contains(dropArea)) {
        ev->ignore();
        return true;
    }

    dropArea->hover(q->m_windowBeingDragged.get(), dropArea->mapToGlobal(Qt5Qt6Compat::eventPos(ev)));

    ev->accept();
    return true;
}

bool StateDraggingWayland::handleDragLeave(DropArea *dropArea)
{
    dropArea->removeHover();
    return true;
}

bool StateDraggingWayland::handleDrop(QDropEvent *ev, DropArea *dropArea)
{
    auto mimeData = qobject_cast<const WaylandMimeData *>(ev->mimeData());
    if (!mimeData || !q->m_windowBeingDragged)
        return false; // Not for us, some other user drag.

    if (dropArea->drop(q->m_windowBeingDragged.get(), dropArea->mapToGlobal(Qt5Qt6Compat::eventPos(ev)))) {
        ev->setDropAction(Qt::MoveAction);
        ev->accept();
        Q_EMIT q->dropped();
    } else {
        Q_EMIT q->dragCanceled();
    }

    dropArea->removeHover();
    return true;
}

bool StateDraggingWayland::handleDragMove(QDragMoveEvent *ev, DropArea *dropArea)
{
    auto mimeData = qobject_cast<const WaylandMimeData *>(ev->mimeData());
    if (!mimeData || !q->m_windowBeingDragged)
        return false; // Not for us, some other user drag.

    dropArea->hover(q->m_windowBeingDragged.get(), dropArea->mapToGlobal(Qt5Qt6Compat::eventPos(ev)));

    return true;
}

DragController::DragController(QObject *parent)
    : MinimalStateMachine(parent)
{
    qCDebug(creation) << "DragController()";

    m_stateNone = new StateNone(this);
    auto statepreDrag = new StatePreDrag(this);
    auto stateDragging = isWayland() ? new StateDraggingWayland(this)
                                     : new StateDragging(this);
    m_stateDraggingMDI = new StateInternalMDIDragging(this);

    m_stateNone->addTransition(this, &DragController::mousePressed, statepreDrag);
    statepreDrag->addTransition(this, &DragController::dragCanceled, m_stateNone);
    statepreDrag->addTransition(this, &DragController::manhattanLengthMove, stateDragging);
    statepreDrag->addTransition(this, &DragController::manhattanLengthMoveMDI, m_stateDraggingMDI);
    stateDragging->addTransition(this, &DragController::dragCanceled, m_stateNone);
    stateDragging->addTransition(this, &DragController::dropped, m_stateNone);

    m_stateDraggingMDI->addTransition(this, &DragController::dragCanceled, m_stateNone);
    m_stateDraggingMDI->addTransition(this, &DragController::mdiPopOut, stateDragging);

    if (Platform::instance()->usesFallbackMouseGrabber())
        enableFallbackMouseGrabber();

    setCurrentState(m_stateNone);
}

DragController *DragController::instance()
{
    static DragController dragController;
    return &dragController;
}

void DragController::registerDraggable(Draggable *drg)
{
    m_draggables << drg;
    drg->asView()->asQObject()->installEventFilter(this);
}

void DragController::unregisterDraggable(Draggable *drg)
{
    m_draggables.removeOne(drg);
    drg->asView()->asQObject()->removeEventFilter(this);
}

bool DragController::isDragging() const
{
    return m_windowBeingDragged != nullptr || activeState() == m_stateDraggingMDI;
}

bool DragController::isInNonClientDrag() const
{
    return isDragging() && m_nonClientDrag;
}

bool DragController::isInClientDrag() const
{
    return isDragging() && !m_nonClientDrag;
}

bool DragController::isIdle() const
{
    return activeState() == m_stateNone;
}

void DragController::grabMouseFor(View *target)
{
    if (isWayland())
        return; // No grabbing supported on wayland

    if (m_fallbackMouseGrabber) {
        m_fallbackMouseGrabber->grabMouse(target);
    } else {
        target->grabMouse();
    }
}

void DragController::releaseMouse(View *target)
{
    if (isWayland())
        return; // No grabbing supported on wayland

    if (m_fallbackMouseGrabber) {
        m_fallbackMouseGrabber->releaseMouse();
    } else {
        target->releaseMouse();
    }
}

FloatingWindow *DragController::floatingWindowBeingDragged() const
{
    return m_windowBeingDragged ? m_windowBeingDragged->floatingWindow()
                                : nullptr;
}

void DragController::enableFallbackMouseGrabber()
{
    if (!m_fallbackMouseGrabber)
        m_fallbackMouseGrabber = new FallbackMouseGrabber(this);
}

WindowBeingDragged *DragController::windowBeingDragged() const
{
    return m_windowBeingDragged.get();
}

bool DragController::eventFilter(QObject *o, QEvent *e)
{
    if (m_nonClientDrag && e->type() == QEvent::Move) {
        // On Windows, non-client mouse moves are only sent at the end, so we must fake it:
        qCDebug(mouseevents) << "DragController::eventFilter e=" << e->type() << "; o=" << o;
        activeState()->handleMouseMove(QCursor::pos());
        return MinimalStateMachine::eventFilter(o, e);
    }

    if (isWayland()) {
        // Wayland is very different. It uses QDrag for the dragging of a window.
        if (auto view = Platform::instance()->qobjectAsView(o)) {
            if (auto dropArea = view->asDropAreaController()) {
                switch (int(e->type())) {
                case QEvent::DragEnter:
                    if (activeState()->handleDragEnter(static_cast<QDragEnterEvent *>(e), dropArea))
                        return true;
                    break;
                case QEvent::DragLeave:
                    if (activeState()->handleDragLeave(dropArea))
                        return true;
                    break;
                case QEvent::DragMove:
                    if (activeState()->handleDragMove(static_cast<QDragMoveEvent *>(e), dropArea))
                        return true;
                    break;
                case QEvent::Drop:
                    if (activeState()->handleDrop(static_cast<QDropEvent *>(e), dropArea))
                        return true;
                    break;
                }
            }
        }
    }

    QMouseEvent *me = mouseEvent(e);
    if (!me)
        return MinimalStateMachine::eventFilter(o, e);

    auto w = Platform::instance()->qobjectAsView(o);
    if (!w)
        return MinimalStateMachine::eventFilter(o, e);

    qCDebug(mouseevents) << "DragController::eventFilter e=" << e->type() << "; o=" << o
                         << "; m_nonClientDrag=" << m_nonClientDrag;

    switch (e->type()) {
    case QEvent::NonClientAreaMouseButtonPress: {
        if (auto fw = w->asFloatingWindowController()) {
            if (KDDockWidgets::usesNativeTitleBar() || fw->isInDragArea(Qt5Qt6Compat::eventGlobalPos(me))) {
                m_nonClientDrag = true;
                return activeState()->handleMouseButtonPress(draggableForQObject(o), Qt5Qt6Compat::eventGlobalPos(me), me->pos());
            }
        }
        return MinimalStateMachine::eventFilter(o, e);
    }
    case QEvent::MouseButtonPress:
        // For top-level windows that support native dragging all goes through the NonClient* events.
        // This also forbids dragging a FloatingWindow simply by pressing outside of the title area, in the background
        if (!KDDockWidgets::usesNativeDraggingAndResizing() || !w->isRootView()) {
            Q_ASSERT(activeState());
            return activeState()->handleMouseButtonPress(draggableForQObject(o), Qt5Qt6Compat::eventGlobalPos(me), me->pos());
        } else
            break;
    case QEvent::MouseButtonRelease:
    case QEvent::NonClientAreaMouseButtonRelease:
        return activeState()->handleMouseButtonRelease(Qt5Qt6Compat::eventGlobalPos(me));
    case QEvent::NonClientAreaMouseMove:
    case QEvent::MouseMove:
        return activeState()->handleMouseMove(Qt5Qt6Compat::eventGlobalPos(me));
    case QEvent::MouseButtonDblClick:
    case QEvent::NonClientAreaMouseButtonDblClick:
        return activeState()->handleMouseDoubleClick();
    default:
        break;
    }

    return MinimalStateMachine::eventFilter(o, e);
}

StateBase *DragController::activeState() const
{
    return static_cast<StateBase *>(currentState());
}

#if defined(Q_OS_WIN)
static std::shared_ptr<ViewWrapper> qtTopLevelForHWND(HWND hwnd)
{
    const Window::List windows = Platform::instance()->windows();
    for (Window::Ptr window : windows) {
        if (!window->isVisible())
            continue;

        if (hwnd == ( HWND )window->handle()) {
            if (auto result = window->rootView())
                return result;
#ifdef KDDW_FRONTEND_QTWIDGETS
            if (Platform::instance()->isQtWidgets()) {
                // It's not a KDDW window, but we still return something, as the KDDW main window
                // might be embedded into another non-kddw QMainWindow
                // Case not supported for QtQuick.
                const QWidgetList widgets = qApp->topLevelWidgets();
                for (QWidget *widget : widgets) {
                    if (!widget->window()) {
                        // Don't call winId on windows that don't have it, as that will force all its children to have it,
                        // and that's not very stable. a top level might not have one because it's being destroyed, or because
                        // it's a top-level just because it has't been reparented I guess.
                        continue;
                    }
                    if (hwnd == ( HWND )widget->winId()) {
                        return Platform::instance()->qobjectAsView(widget);
                    }
                }
            }
#endif
        }
    }

    qCDebug(toplevels) << Q_FUNC_INFO << "Couldn't find hwnd for top-level" << hwnd;
    return nullptr;
}

#endif

static ViewWrapper::Ptr qtTopLevelUnderCursor_impl(QPoint globalPos, const Window::List &windows,
                                                   View *rootViewBeingDragged)
{
    for (auto i = windows.size() - 1; i >= 0; --i) {
        Window::Ptr window = windows.at(i);
        auto tl = window->rootView();

        if (!tl->isVisible() || tl->equals(rootViewBeingDragged) || tl->isMinimized())
            continue;

        if (rootViewBeingDragged && rootViewBeingDragged->window()->equals(window))
            continue;

        if (window->geometry().contains(globalPos)) {
            qCDebug(toplevels) << Q_FUNC_INFO << "Found top-level" << tl.get();
            return tl;
        }
    }

    return nullptr;
}

ViewWrapper::Ptr DragController::qtTopLevelUnderCursor() const
{
    QPoint globalPos = QCursor::pos();

    if (qGuiApp->platformName() == QLatin1String("windows")) { // So -platform offscreen on Windows doesn't use this
#if defined(Q_OS_WIN)
        POINT globalNativePos;
        if (!GetCursorPos(&globalNativePos))
            return nullptr;

        // There might be windows that don't belong to our app in between, so use win32 to travel by z-order.
        // Another solution is to set a parent on all top-levels. But this code is orthogonal.
        HWND hwnd = HWND(m_windowBeingDragged->floatingWindow()->view()->window()->handle());
        while (hwnd) {
            hwnd = GetWindow(hwnd, GW_HWNDNEXT);
            RECT r;
            if (!GetWindowRect(hwnd, &r) || !IsWindowVisible(hwnd))
                continue;

            if (!PtInRect(&r, globalNativePos)) // Check if window is under cursor
                continue;

            if (auto tl = qtTopLevelForHWND(hwnd)) {
                const QRect windowGeometry = tl->windowGeometry();

                if (windowGeometry.contains(globalPos) && tl->objectName() != QStringLiteral("_docks_IndicatorWindow_Overlay")) {
                    qCDebug(toplevels) << Q_FUNC_INFO << "Found top-level" << tl->asQObject();
                    return tl;
                }
            } else {
#ifdef KDDW_FRONTEND_QTWIDGETS
                if (Platform::instance()->isQtWidgets()) {
                    // Maybe it's embedded in a QWinWidget:
                    auto topLevels = qApp->topLevelWidgets();
                    for (auto topLevel : topLevels) {
                        if (QLatin1String(topLevel->metaObject()->className()) == QLatin1String("QWinWidget")) {
                            if (hwnd == GetParent(HWND(topLevel->window()->winId()))) {
                                if (topLevel->rect().contains(topLevel->mapFromGlobal(globalPos)) && topLevel->objectName() != QStringLiteral("_docks_IndicatorWindow_Overlay")) {
                                    qCDebug(toplevels) << Q_FUNC_INFO << "Found top-level" << topLevel;
                                    return Platform::instance()->qobjectAsView(topLevel);
                                }
                            }
                        }
                    }
                }
#endif // QtWidgets A window belonging to another app is below the cursor
                qCDebug(toplevels) << Q_FUNC_INFO << "Window from another app is under cursor" << hwnd;
                return nullptr;
            }
        }
#endif // Q_OS_WIN
    } else if (linksToXLib() && isXCB()) {
        bool ok = false;
        const Window::List orderedWindows = KDDockWidgets::orderedWindows(ok);
        FloatingWindow *tlwBeingDragged = m_windowBeingDragged->floatingWindow();
        if (auto tl = qtTopLevelUnderCursor_impl(globalPos, orderedWindows, tlwBeingDragged->view()))
            return tl;

        if (!ok) {
            qCDebug(toplevels) << Q_FUNC_INFO << "No top-level found. Some windows weren't seen by XLib";
        }
    } else {
        // !Windows: Linux, macOS, offscreen (offscreen on Windows too), etc.

        // On Linux we don't have API to check the z-order of top-levels. So first check the floating windows
        // and check the MainWindow last, as the MainWindow will have lower z-order as it's a parent (TODO: How will it work with multiple MainWindows ?)
        // The floating window list is sorted by z-order, as we catch QEvent::Expose and move it to last of the list

        View *tlwBeingDragged = m_windowBeingDragged->floatingWindow()->view();
        if (auto tl = qtTopLevelUnderCursor_impl(globalPos, DockRegistry::self()->floatingQWindows(), tlwBeingDragged))
            return tl;

        return qtTopLevelUnderCursor_impl(globalPos,
                                          DockRegistry::self()->topLevels(/*excludeFloating=*/true),
                                          tlwBeingDragged);
    }

    qCDebug(toplevels) << Q_FUNC_INFO << "No top-level found";
    return nullptr;
}

static DropArea *deepestDropAreaInTopLevel(ViewWrapper::Ptr topLevel, QPoint globalPos,
                                           const QStringList &affinities)
{
    const auto localPos = topLevel->mapFromGlobal(globalPos);
    ViewWrapper::Ptr view = topLevel->childViewAt(localPos);

    while (view) {
        if (auto dt = view->asDropAreaController()) {
            if (DockRegistry::self()->affinitiesMatch(dt->affinities(), affinities))
                return dt;
        }
        view = view->parentView();
    }

    return nullptr;
}

DropArea *DragController::dropAreaUnderCursor() const
{
    ViewWrapper::Ptr topLevel = qtTopLevelUnderCursor();
    if (!topLevel)
        return nullptr;

    const QStringList affinities = m_windowBeingDragged->floatingWindow()->affinities();

    if (auto fw = topLevel->asFloatingWindowController()) {
        if (DockRegistry::self()->affinitiesMatch(fw->affinities(), affinities))
            return fw->dropArea();
    }

    if (topLevel->objectName() == QStringLiteral("_docks_IndicatorWindow")) {
        qWarning() << "Indicator window should be hidden " << topLevel.get() << topLevel->isVisible();
        Q_ASSERT(false);
    }

    if (auto dt = deepestDropAreaInTopLevel(topLevel, QCursor::pos(), affinities)) {
        return dt;
    }

    qCDebug(state) << "DragController::dropAreaUnderCursor: null2";
    return nullptr;
}

Draggable *DragController::draggableForQObject(QObject *o) const
{
    for (auto draggable : m_draggables)
        if (draggable->asView()->asQObject() == o) {
            return draggable;
        }

    return nullptr;
}
