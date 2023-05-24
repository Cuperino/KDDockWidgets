/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "TitleBar_c.h"


#include <QDebug>


namespace Dartagnan {

typedef int (*CleanupCallback)(void *thisPtr);
static CleanupCallback s_cleanupCallback = nullptr;

template<typename T>
struct ValueWrapper
{
    T value;
};

}
namespace KDDockWidgetsBindings_wrappersNS {
namespace KDDWBindingsCore {
TitleBar_wrapper::TitleBar_wrapper(KDDockWidgets::Core::FloatingWindow *parent)
    : ::KDDockWidgets::Core::TitleBar(parent)
{
}
TitleBar_wrapper::TitleBar_wrapper(KDDockWidgets::Core::Group *parent)
    : ::KDDockWidgets::Core::TitleBar(parent)
{
}
bool TitleBar_wrapper::closeButtonEnabled() const
{
    return ::KDDockWidgets::Core::TitleBar::closeButtonEnabled();
}
void TitleBar_wrapper::closeButtonEnabledChanged(bool arg__1)
{
    ::KDDockWidgets::Core::TitleBar::closeButtonEnabledChanged(arg__1);
}
QString TitleBar_wrapper::floatButtonToolTip() const
{
    return ::KDDockWidgets::Core::TitleBar::floatButtonToolTip();
}
void TitleBar_wrapper::floatButtonToolTipChanged(const QString &arg__1)
{
    ::KDDockWidgets::Core::TitleBar::floatButtonToolTipChanged(arg__1);
}
bool TitleBar_wrapper::floatButtonVisible() const
{
    return ::KDDockWidgets::Core::TitleBar::floatButtonVisible();
}
void TitleBar_wrapper::floatButtonVisibleChanged(bool arg__1)
{
    ::KDDockWidgets::Core::TitleBar::floatButtonVisibleChanged(arg__1);
}
KDDockWidgets::Core::FloatingWindow *TitleBar_wrapper::floatingWindow() const
{
    return ::KDDockWidgets::Core::TitleBar::floatingWindow();
}
KDDockWidgets::Core::Group *TitleBar_wrapper::group() const
{
    return ::KDDockWidgets::Core::TitleBar::group();
}
bool TitleBar_wrapper::hasIcon() const
{
    return ::KDDockWidgets::Core::TitleBar::hasIcon();
}
void TitleBar_wrapper::iconChanged()
{
    ::KDDockWidgets::Core::TitleBar::iconChanged();
}
bool TitleBar_wrapper::isCloseButtonEnabled() const
{
    return ::KDDockWidgets::Core::TitleBar::isCloseButtonEnabled();
}
bool TitleBar_wrapper::isCloseButtonVisible() const
{
    return ::KDDockWidgets::Core::TitleBar::isCloseButtonVisible();
}
bool TitleBar_wrapper::isFloatButtonVisible() const
{
    return ::KDDockWidgets::Core::TitleBar::isFloatButtonVisible();
}
bool TitleBar_wrapper::isFloating() const
{
    return ::KDDockWidgets::Core::TitleBar::isFloating();
}
bool TitleBar_wrapper::isFocused() const
{
    return ::KDDockWidgets::Core::TitleBar::isFocused();
}
void TitleBar_wrapper::isFocusedChanged()
{
    ::KDDockWidgets::Core::TitleBar::isFocusedChanged();
}
bool TitleBar_wrapper::isMDI() const
{
    if (m_isMDICallback) {
        const void *thisPtr = this;
        return m_isMDICallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::Core::TitleBar::isMDI();
    }
}
bool TitleBar_wrapper::isMDI_nocallback() const
{
    return ::KDDockWidgets::Core::TitleBar::isMDI();
}
bool TitleBar_wrapper::isOverlayed() const
{
    return ::KDDockWidgets::Core::TitleBar::isOverlayed();
}
bool TitleBar_wrapper::isWindow() const
{
    if (m_isWindowCallback) {
        const void *thisPtr = this;
        return m_isWindowCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::Core::TitleBar::isWindow();
    }
}
bool TitleBar_wrapper::isWindow_nocallback() const
{
    return ::KDDockWidgets::Core::TitleBar::isWindow();
}
KDDockWidgets::Core::MainWindow *TitleBar_wrapper::mainWindow() const
{
    return ::KDDockWidgets::Core::TitleBar::mainWindow();
}
bool TitleBar_wrapper::maximizeButtonVisible() const
{
    return ::KDDockWidgets::Core::TitleBar::maximizeButtonVisible();
}
void TitleBar_wrapper::minimizeButtonChanged(bool visible, bool enabled)
{
    ::KDDockWidgets::Core::TitleBar::minimizeButtonChanged(visible, enabled);
}
void TitleBar_wrapper::numDockWidgetsChanged()
{
    ::KDDockWidgets::Core::TitleBar::numDockWidgetsChanged();
}
void TitleBar_wrapper::onAutoHideClicked()
{
    ::KDDockWidgets::Core::TitleBar::onAutoHideClicked();
}
void TitleBar_wrapper::onCloseClicked()
{
    ::KDDockWidgets::Core::TitleBar::onCloseClicked();
}
bool TitleBar_wrapper::onDoubleClicked()
{
    return ::KDDockWidgets::Core::TitleBar::onDoubleClicked();
}
void TitleBar_wrapper::onFloatClicked()
{
    ::KDDockWidgets::Core::TitleBar::onFloatClicked();
}
void TitleBar_wrapper::onMaximizeClicked()
{
    ::KDDockWidgets::Core::TitleBar::onMaximizeClicked();
}
void TitleBar_wrapper::onMinimizeClicked()
{
    ::KDDockWidgets::Core::TitleBar::onMinimizeClicked();
}
void TitleBar_wrapper::setCloseButtonEnabled(bool arg__1)
{
    ::KDDockWidgets::Core::TitleBar::setCloseButtonEnabled(arg__1);
}
void TitleBar_wrapper::setFloatButtonVisible(bool arg__1)
{
    ::KDDockWidgets::Core::TitleBar::setFloatButtonVisible(arg__1);
}
void TitleBar_wrapper::setParentView_impl(KDDockWidgets::Core::View *parent)
{
    if (m_setParentView_implCallback) {
        const void *thisPtr = this;
        m_setParentView_implCallback(const_cast<void *>(thisPtr), parent);
    } else {
        ::KDDockWidgets::Core::TitleBar::setParentView_impl(parent);
    }
}
void TitleBar_wrapper::setParentView_impl_nocallback(KDDockWidgets::Core::View *parent)
{
    ::KDDockWidgets::Core::TitleBar::setParentView_impl(parent);
}
void TitleBar_wrapper::setTitle(const QString &title)
{
    ::KDDockWidgets::Core::TitleBar::setTitle(title);
}
KDDockWidgets::Core::DockWidget *TitleBar_wrapper::singleDockWidget() const
{
    if (m_singleDockWidgetCallback) {
        const void *thisPtr = this;
        return m_singleDockWidgetCallback(const_cast<void *>(thisPtr));
    } else {
        return ::KDDockWidgets::Core::TitleBar::singleDockWidget();
    }
}
KDDockWidgets::Core::DockWidget *TitleBar_wrapper::singleDockWidget_nocallback() const
{
    return ::KDDockWidgets::Core::TitleBar::singleDockWidget();
}
bool TitleBar_wrapper::supportsAutoHideButton() const
{
    return ::KDDockWidgets::Core::TitleBar::supportsAutoHideButton();
}
bool TitleBar_wrapper::supportsFloatingButton() const
{
    return ::KDDockWidgets::Core::TitleBar::supportsFloatingButton();
}
bool TitleBar_wrapper::supportsMaximizeButton() const
{
    return ::KDDockWidgets::Core::TitleBar::supportsMaximizeButton();
}
bool TitleBar_wrapper::supportsMinimizeButton() const
{
    return ::KDDockWidgets::Core::TitleBar::supportsMinimizeButton();
}
KDDockWidgets::Core::TabBar *TitleBar_wrapper::tabBar() const
{
    return ::KDDockWidgets::Core::TitleBar::tabBar();
}
QString TitleBar_wrapper::title() const
{
    return ::KDDockWidgets::Core::TitleBar::title();
}
bool TitleBar_wrapper::titleBarIsFocusable() const
{
    return ::KDDockWidgets::Core::TitleBar::titleBarIsFocusable();
}
void TitleBar_wrapper::titleChanged()
{
    ::KDDockWidgets::Core::TitleBar::titleChanged();
}
void TitleBar_wrapper::toggleMaximized()
{
    ::KDDockWidgets::Core::TitleBar::toggleMaximized();
}
QString TitleBar_wrapper::tr(const char *s, const char *c, int n)
{
    return ::KDDockWidgets::Core::TitleBar::tr(s, c, n);
}
void TitleBar_wrapper::updateButtons()
{
    ::KDDockWidgets::Core::TitleBar::updateButtons();
}
TitleBar_wrapper::~TitleBar_wrapper()
{
}

}
}
static KDDockWidgets::Core::TitleBar *fromPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgets::Core::TitleBar *>(ptr);
}
static KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper *fromWrapperPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper *>(ptr);
}
extern "C" {
void c_KDDockWidgets__Core__TitleBar_Finalizer(void *, void *cppObj, void *)
{
    delete reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper *>(cppObj);
}
void *c_KDDockWidgets__Core__TitleBar__constructor_FloatingWindow(void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::FloatingWindow *>(parent_);
    auto ptr = new KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper(parent);
    return reinterpret_cast<void *>(ptr);
}
void *c_KDDockWidgets__Core__TitleBar__constructor_Group(void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::Group *>(parent_);
    auto ptr = new KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper(parent);
    return reinterpret_cast<void *>(ptr);
}
// closeButtonEnabled() const
bool c_KDDockWidgets__Core__TitleBar__closeButtonEnabled(void *thisObj)
{
    return fromPtr(thisObj)->closeButtonEnabled();
}
// closeButtonEnabledChanged(bool arg__1)
void c_KDDockWidgets__Core__TitleBar__closeButtonEnabledChanged_bool(void *thisObj, bool arg__1)
{
    fromPtr(thisObj)->closeButtonEnabledChanged(arg__1);
}
// floatButtonToolTip() const
void *c_KDDockWidgets__Core__TitleBar__floatButtonToolTip(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QString> { fromPtr(thisObj)->floatButtonToolTip() };
}
// floatButtonToolTipChanged(const QString & arg__1)
void c_KDDockWidgets__Core__TitleBar__floatButtonToolTipChanged_QString(void *thisObj, const char *arg__1_)
{
    const auto arg__1 = QString::fromUtf8(arg__1_);
    fromPtr(thisObj)->floatButtonToolTipChanged(arg__1);
}
// floatButtonVisible() const
bool c_KDDockWidgets__Core__TitleBar__floatButtonVisible(void *thisObj)
{
    return fromPtr(thisObj)->floatButtonVisible();
}
// floatButtonVisibleChanged(bool arg__1)
void c_KDDockWidgets__Core__TitleBar__floatButtonVisibleChanged_bool(void *thisObj, bool arg__1)
{
    fromPtr(thisObj)->floatButtonVisibleChanged(arg__1);
}
// floatingWindow() const
void *c_KDDockWidgets__Core__TitleBar__floatingWindow(void *thisObj)
{
    return fromPtr(thisObj)->floatingWindow();
}
// group() const
void *c_KDDockWidgets__Core__TitleBar__group(void *thisObj)
{
    return fromPtr(thisObj)->group();
}
// hasIcon() const
bool c_KDDockWidgets__Core__TitleBar__hasIcon(void *thisObj)
{
    return fromPtr(thisObj)->hasIcon();
}
// iconChanged()
void c_KDDockWidgets__Core__TitleBar__iconChanged(void *thisObj)
{
    fromPtr(thisObj)->iconChanged();
}
// isCloseButtonEnabled() const
bool c_KDDockWidgets__Core__TitleBar__isCloseButtonEnabled(void *thisObj)
{
    return fromPtr(thisObj)->isCloseButtonEnabled();
}
// isCloseButtonVisible() const
bool c_KDDockWidgets__Core__TitleBar__isCloseButtonVisible(void *thisObj)
{
    return fromPtr(thisObj)->isCloseButtonVisible();
}
// isFloatButtonVisible() const
bool c_KDDockWidgets__Core__TitleBar__isFloatButtonVisible(void *thisObj)
{
    return fromPtr(thisObj)->isFloatButtonVisible();
}
// isFloating() const
bool c_KDDockWidgets__Core__TitleBar__isFloating(void *thisObj)
{
    return fromPtr(thisObj)->isFloating();
}
// isFocused() const
bool c_KDDockWidgets__Core__TitleBar__isFocused(void *thisObj)
{
    return fromPtr(thisObj)->isFocused();
}
// isFocusedChanged()
void c_KDDockWidgets__Core__TitleBar__isFocusedChanged(void *thisObj)
{
    fromPtr(thisObj)->isFocusedChanged();
}
// isMDI() const
bool c_KDDockWidgets__Core__TitleBar__isMDI(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isMDI_nocallback();} else {    return targetPtr->isMDI();} }();
}
// isOverlayed() const
bool c_KDDockWidgets__Core__TitleBar__isOverlayed(void *thisObj)
{
    return fromWrapperPtr(thisObj)->isOverlayed();
}
// isWindow() const
bool c_KDDockWidgets__Core__TitleBar__isWindow(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->isWindow_nocallback();} else {    return targetPtr->isWindow();} }();
}
// mainWindow() const
void *c_KDDockWidgets__Core__TitleBar__mainWindow(void *thisObj)
{
    return fromPtr(thisObj)->mainWindow();
}
// maximizeButtonVisible() const
bool c_KDDockWidgets__Core__TitleBar__maximizeButtonVisible(void *thisObj)
{
    return fromPtr(thisObj)->maximizeButtonVisible();
}
// minimizeButtonChanged(bool visible, bool enabled)
void c_KDDockWidgets__Core__TitleBar__minimizeButtonChanged_bool_bool(void *thisObj, bool visible, bool enabled)
{
    fromPtr(thisObj)->minimizeButtonChanged(visible, enabled);
}
// numDockWidgetsChanged()
void c_KDDockWidgets__Core__TitleBar__numDockWidgetsChanged(void *thisObj)
{
    fromPtr(thisObj)->numDockWidgetsChanged();
}
// onAutoHideClicked()
void c_KDDockWidgets__Core__TitleBar__onAutoHideClicked(void *thisObj)
{
    fromPtr(thisObj)->onAutoHideClicked();
}
// onCloseClicked()
void c_KDDockWidgets__Core__TitleBar__onCloseClicked(void *thisObj)
{
    fromPtr(thisObj)->onCloseClicked();
}
// onDoubleClicked()
bool c_KDDockWidgets__Core__TitleBar__onDoubleClicked(void *thisObj)
{
    return fromPtr(thisObj)->onDoubleClicked();
}
// onFloatClicked()
void c_KDDockWidgets__Core__TitleBar__onFloatClicked(void *thisObj)
{
    fromPtr(thisObj)->onFloatClicked();
}
// onMaximizeClicked()
void c_KDDockWidgets__Core__TitleBar__onMaximizeClicked(void *thisObj)
{
    fromPtr(thisObj)->onMaximizeClicked();
}
// onMinimizeClicked()
void c_KDDockWidgets__Core__TitleBar__onMinimizeClicked(void *thisObj)
{
    fromPtr(thisObj)->onMinimizeClicked();
}
// setCloseButtonEnabled(bool arg__1)
void c_KDDockWidgets__Core__TitleBar__setCloseButtonEnabled_bool(void *thisObj, bool arg__1)
{
    fromPtr(thisObj)->setCloseButtonEnabled(arg__1);
}
// setFloatButtonVisible(bool arg__1)
void c_KDDockWidgets__Core__TitleBar__setFloatButtonVisible_bool(void *thisObj, bool arg__1)
{
    fromPtr(thisObj)->setFloatButtonVisible(arg__1);
}
// setParentView_impl(KDDockWidgets::Core::View * parent)
void c_KDDockWidgets__Core__TitleBar__setParentView_impl_View(void *thisObj, void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    fromWrapperPtr(thisObj)->setParentView_impl_nocallback(parent);
}
// setTitle(const QString & title)
void c_KDDockWidgets__Core__TitleBar__setTitle_QString(void *thisObj, const char *title_)
{
    const auto title = QString::fromUtf8(title_);
    fromPtr(thisObj)->setTitle(title);
}
// singleDockWidget() const
void *c_KDDockWidgets__Core__TitleBar__singleDockWidget(void *thisObj)
{
    return [&] {auto targetPtr = fromPtr(thisObj);auto wrapperPtr = dynamic_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper*>(targetPtr);if (wrapperPtr) {    return wrapperPtr->singleDockWidget_nocallback();} else {    return targetPtr->singleDockWidget();} }();
}
// supportsAutoHideButton() const
bool c_KDDockWidgets__Core__TitleBar__supportsAutoHideButton(void *thisObj)
{
    return fromPtr(thisObj)->supportsAutoHideButton();
}
// supportsFloatingButton() const
bool c_KDDockWidgets__Core__TitleBar__supportsFloatingButton(void *thisObj)
{
    return fromPtr(thisObj)->supportsFloatingButton();
}
// supportsMaximizeButton() const
bool c_KDDockWidgets__Core__TitleBar__supportsMaximizeButton(void *thisObj)
{
    return fromPtr(thisObj)->supportsMaximizeButton();
}
// supportsMinimizeButton() const
bool c_KDDockWidgets__Core__TitleBar__supportsMinimizeButton(void *thisObj)
{
    return fromPtr(thisObj)->supportsMinimizeButton();
}
// tabBar() const
void *c_KDDockWidgets__Core__TitleBar__tabBar(void *thisObj)
{
    return fromPtr(thisObj)->tabBar();
}
// title() const
void *c_KDDockWidgets__Core__TitleBar__title(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QString> { fromPtr(thisObj)->title() };
}
// titleBarIsFocusable() const
bool c_KDDockWidgets__Core__TitleBar__titleBarIsFocusable(void *thisObj)
{
    return fromPtr(thisObj)->titleBarIsFocusable();
}
// titleChanged()
void c_KDDockWidgets__Core__TitleBar__titleChanged(void *thisObj)
{
    fromPtr(thisObj)->titleChanged();
}
// toggleMaximized()
void c_KDDockWidgets__Core__TitleBar__toggleMaximized(void *thisObj)
{
    fromPtr(thisObj)->toggleMaximized();
}
// tr(const char * s, const char * c, int n)
void *c_static_KDDockWidgets__Core__TitleBar__tr_char_char_int(const char *s, const char *c, int n)
{
    return new Dartagnan::ValueWrapper<QString> { KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper::tr(s, c, n) };
}
// updateButtons()
void c_KDDockWidgets__Core__TitleBar__updateButtons(void *thisObj)
{
    fromPtr(thisObj)->updateButtons();
}
void c_KDDockWidgets__Core__TitleBar__destructor(void *thisObj)
{
    delete fromPtr(thisObj);
}
void c_KDDockWidgets__Core__TitleBar__registerVirtualMethodCallback(void *ptr, void *callback, int methodId)
{
    auto wrapper = fromWrapperPtr(ptr);
    switch (methodId) {
    case 1150:
        wrapper->m_isMDICallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper::Callback_isMDI>(callback);
        break;
    case 1153:
        wrapper->m_isWindowCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper::Callback_isWindow>(callback);
        break;
    case 906:
        wrapper->m_setParentView_implCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper::Callback_setParentView_impl>(callback);
        break;
    case 1175:
        wrapper->m_singleDockWidgetCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::TitleBar_wrapper::Callback_singleDockWidget>(callback);
        break;
    }
}
}
