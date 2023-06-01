/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart';
import '../TypeHelpers.dart';
import '../../Bindings.dart';
import '../../Bindings_KDDWBindingsCore.dart' as KDDWBindingsCore;
import '../../Bindings_KDDWBindingsFlutter.dart' as KDDWBindingsFlutter;
import '../../LibraryLoader.dart';
import '../../FinalizerHelpers.dart';

var _dylib = Library.instance().dylib;

class TabBar extends KDDWBindingsCore.Controller {
  TabBar.fromCppPointer(var cppPointer, [var needsAutoDelete = false])
      : super.fromCppPointer(cppPointer, needsAutoDelete) {}
  TabBar.init() : super.init() {}
  factory TabBar.fromCache(var cppPointer, [needsAutoDelete = false]) {
    if (QObject.isCached(cppPointer)) {
      var instance = QObject.s_dartInstanceByCppPtr[cppPointer.address];
      if (instance != null) return instance as TabBar;
    }
    return TabBar.fromCppPointer(cppPointer, needsAutoDelete);
  } //TabBar(KDDockWidgets::Core::Stack * tabWidget)
  TabBar({required KDDWBindingsCore.Stack? tabWidget}) : super.init() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__constructor_Stack')
        .asFunction();
    thisCpp = func(tabWidget == null ? ffi.nullptr : tabWidget.thisCpp);
    QObject.s_dartInstanceByCppPtr[thisCpp.address] = this;
    registerCallbacks();
  } // countChanged()
  countChanged() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__countChanged')
        .asFunction();
    func(thisCpp);
  } // currentDockWidget() const

  KDDWBindingsCore.DockWidget currentDockWidget() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__currentDockWidget')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.DockWidget.fromCppPointer(result, false);
  } // currentDockWidgetChanged(KDDockWidgets::Core::DockWidget * arg__1)

  currentDockWidgetChanged(KDDWBindingsCore.DockWidget? arg__1) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__currentDockWidgetChanged_DockWidget')
        .asFunction();
    func(thisCpp, arg__1 == null ? ffi.nullptr : arg__1.thisCpp);
  } // currentIndex() const

  int currentIndex() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__currentIndex')
        .asFunction();
    return func(thisCpp);
  } // dockWidgetAt(QPoint localPos) const

  KDDWBindingsCore.DockWidget dockWidgetAt(QPoint localPos) {
    final voidstar_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__dockWidgetAt_QPoint')
        .asFunction();
    ffi.Pointer<void> result =
        func(thisCpp, localPos == null ? ffi.nullptr : localPos.thisCpp);
    return KDDWBindingsCore.DockWidget.fromCppPointer(result, false);
  } // dockWidgetAt(int index) const

  KDDWBindingsCore.DockWidget dockWidgetAt_2(int index) {
    final voidstar_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_ffi_Int32_FFI>>(
            'c_KDDockWidgets__Core__TabBar__dockWidgetAt_int')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp, index);
    return KDDWBindingsCore.DockWidget.fromCppPointer(result, false);
  } // dragCanStart(QPoint pressPos, QPoint pos) const

  bool dragCanStart(QPoint pressPos, QPoint pos) {
    final bool_Func_voidstar_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(1214))
        .asFunction();
    return func(thisCpp, pressPos == null ? ffi.nullptr : pressPos.thisCpp,
            pos == null ? ffi.nullptr : pos.thisCpp) !=
        0;
  }

  static int dragCanStart_calledFromC(ffi.Pointer<void> thisCpp,
      ffi.Pointer<void> pressPos, ffi.Pointer<void> pos) {
    var dartInstance =
        QObject.s_dartInstanceByCppPtr[thisCpp.address] as TabBar;
    if (dartInstance == null) {
      print(
          "Dart instance not found for TabBar::dragCanStart(QPoint pressPos, QPoint pos) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.dragCanStart(
        QPoint.fromCppPointer(pressPos), QPoint.fromCppPointer(pos));
    return result ? 1 : 0;
  } // group() const

  KDDWBindingsCore.Group group() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__group')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.Group.fromCppPointer(result, false);
  } // hasSingleDockWidget() const

  bool hasSingleDockWidget() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__hasSingleDockWidget')
        .asFunction();
    return func(thisCpp) != 0;
  } // indexOfDockWidget(const KDDockWidgets::Core::DockWidget * dw) const

  int indexOfDockWidget(KDDWBindingsCore.DockWidget? dw) {
    final int_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__indexOfDockWidget_DockWidget')
        .asFunction();
    return func(thisCpp, dw == null ? ffi.nullptr : dw.thisCpp);
  } // isMDI() const

  bool isMDI() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(1222))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isMDI_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        QObject.s_dartInstanceByCppPtr[thisCpp.address] as TabBar;
    if (dartInstance == null) {
      print(
          "Dart instance not found for TabBar::isMDI() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isMDI();
    return result ? 1 : 0;
  } // isWindow() const

  bool isWindow() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            cFunctionSymbolName(1224))
        .asFunction();
    return func(thisCpp) != 0;
  }

  static int isWindow_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        QObject.s_dartInstanceByCppPtr[thisCpp.address] as TabBar;
    if (dartInstance == null) {
      print(
          "Dart instance not found for TabBar::isWindow() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isWindow();
    return result ? 1 : 0;
  } // moveTabTo(int from, int to)

  moveTabTo(int from, int to) {
    final void_Func_voidstar_int_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>>(
            'c_KDDockWidgets__Core__TabBar__moveTabTo_int_int')
        .asFunction();
    func(thisCpp, from, to);
  } // numDockWidgets() const

  int numDockWidgets() {
    final int_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<int_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__numDockWidgets')
        .asFunction();
    return func(thisCpp);
  } // onMouseDoubleClick(QPoint localPos)

  onMouseDoubleClick(QPoint localPos) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__onMouseDoubleClick_QPoint')
        .asFunction();
    func(thisCpp, localPos == null ? ffi.nullptr : localPos.thisCpp);
  } // onMousePress(QPoint localPos)

  onMousePress(QPoint localPos) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__onMousePress_QPoint')
        .asFunction();
    func(thisCpp, localPos == null ? ffi.nullptr : localPos.thisCpp);
  } // rectForTab(int index) const

  QRect rectForTab(int index) {
    final voidstar_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_ffi_Int32_FFI>>(
            'c_KDDockWidgets__Core__TabBar__rectForTab_int')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp, index);
    return QRect.fromCppPointer(result, true);
  } // removeDockWidget(KDDockWidgets::Core::DockWidget * dw)

  removeDockWidget(KDDWBindingsCore.DockWidget? dw) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__removeDockWidget_DockWidget')
        .asFunction();
    func(thisCpp, dw == null ? ffi.nullptr : dw.thisCpp);
  } // renameTab(int index, const QString & arg__2)

  renameTab(int index, String? arg__2) {
    final void_Func_voidstar_int_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__renameTab_int_QString')
        .asFunction();
    func(thisCpp, index, arg__2?.toNativeUtf8() ?? ffi.nullptr);
  } // setCurrentDockWidget(KDDockWidgets::Core::DockWidget * dw)

  setCurrentDockWidget(KDDWBindingsCore.DockWidget? dw) {
    final void_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__setCurrentDockWidget_DockWidget')
        .asFunction();
    func(thisCpp, dw == null ? ffi.nullptr : dw.thisCpp);
  } // setCurrentIndex(int index)

  setCurrentIndex(int index) {
    final void_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int32_FFI>>(
            'c_KDDockWidgets__Core__TabBar__setCurrentIndex_int')
        .asFunction();
    func(thisCpp, index);
  }

  static void setParentView_impl_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? parent) {
    var dartInstance =
        QObject.s_dartInstanceByCppPtr[thisCpp.address] as TabBar;
    if (dartInstance == null) {
      print(
          "Dart instance not found for TabBar::setParentView_impl(KDDockWidgets::Core::View * parent)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setParentView_impl((parent == null || parent.address == 0)
        ? null
        : KDDWBindingsCore.View.fromCppPointer(parent));
  } // singleDockWidget() const

  KDDWBindingsCore.DockWidget singleDockWidget() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            cFunctionSymbolName(1241))
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.DockWidget.fromCppPointer(result, false);
  }

  static ffi.Pointer<void> singleDockWidget_calledFromC(
      ffi.Pointer<void> thisCpp) {
    var dartInstance =
        QObject.s_dartInstanceByCppPtr[thisCpp.address] as TabBar;
    if (dartInstance == null) {
      print(
          "Dart instance not found for TabBar::singleDockWidget() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.singleDockWidget();
    return result.thisCpp;
  } // stack() const

  KDDWBindingsCore.Stack stack() {
    final voidstar_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__stack')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp);
    return KDDWBindingsCore.Stack.fromCppPointer(result, false);
  } // tabsAreMovable() const

  bool tabsAreMovable() {
    final bool_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__tabsAreMovable')
        .asFunction();
    return func(thisCpp) != 0;
  } // text(int index) const

  QString text(int index) {
    final voidstar_Func_voidstar_int func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_ffi_Int32_FFI>>(
            'c_KDDockWidgets__Core__TabBar__text_int')
        .asFunction();
    ffi.Pointer<void> result = func(thisCpp, index);
    return QString.fromCppPointer(result, true);
  }

  static // tr(const char * s, const char * c, int n)
      QString tr(String? s, String? c, int n) {
    final voidstar_Func_string_string_int func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_string_string_ffi_Int32_FFI>>(
            'c_static_KDDockWidgets__Core__TabBar__tr_char_char_int')
        .asFunction();
    ffi.Pointer<void> result = func(
        s?.toNativeUtf8() ?? ffi.nullptr, c?.toNativeUtf8() ?? ffi.nullptr, n);
    return QString.fromCppPointer(result, true);
  }

  void release() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            'c_KDDockWidgets__Core__TabBar__destructor')
        .asFunction();
    func(thisCpp);
  }

  String cFunctionSymbolName(int methodId) {
    switch (methodId) {
      case 1214:
        return "c_KDDockWidgets__Core__TabBar__dragCanStart_QPoint_QPoint";
      case 1222:
        return "c_KDDockWidgets__Core__TabBar__isMDI";
      case 1224:
        return "c_KDDockWidgets__Core__TabBar__isWindow";
      case 911:
        return "c_KDDockWidgets__Core__TabBar__setParentView_impl_View";
      case 1241:
        return "c_KDDockWidgets__Core__TabBar__singleDockWidget";
    }
    return super.cFunctionSymbolName(methodId);
  }

  static String methodNameFromId(int methodId) {
    switch (methodId) {
      case 1214:
        return "dragCanStart";
      case 1222:
        return "isMDI";
      case 1224:
        return "isWindow";
      case 911:
        return "setParentView_impl";
      case 1241:
        return "singleDockWidget";
    }
    throw Error();
  }

  void registerCallbacks() {
    assert(thisCpp != null);
    final RegisterMethodIsReimplementedCallback registerCallback = _dylib
        .lookup<ffi.NativeFunction<RegisterMethodIsReimplementedCallback_FFI>>(
            'c_KDDockWidgets__Core__TabBar__registerVirtualMethodCallback')
        .asFunction();
    const callbackExcept1214 = 0;
    final callback1214 =
        ffi.Pointer.fromFunction<bool_Func_voidstar_voidstar_voidstar_FFI>(
            KDDWBindingsCore.TabBar.dragCanStart_calledFromC,
            callbackExcept1214);
    registerCallback(thisCpp, callback1214, 1214);
    const callbackExcept1222 = 0;
    final callback1222 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.TabBar.isMDI_calledFromC, callbackExcept1222);
    registerCallback(thisCpp, callback1222, 1222);
    const callbackExcept1224 = 0;
    final callback1224 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.TabBar.isWindow_calledFromC, callbackExcept1224);
    registerCallback(thisCpp, callback1224, 1224);
    final callback911 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsCore.Controller.setParentView_impl_calledFromC);
    registerCallback(thisCpp, callback911, 911);
    final callback1241 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsCore.TabBar.singleDockWidget_calledFromC);
    registerCallback(thisCpp, callback1241, 1241);
  }
}
