/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/


#define DOCTEST_CONFIG_IMPLEMENT

#include "kddockwidgets/core/Platform.h"
#include "kddockwidgets/core/Window.h"

#include <QRect>
#include <QSize>
#include <QPoint>

#include <doctest/doctest.h>
#include <iostream>

std::ostream &operator<<(std::ostream &os, QSize size)
{
    os << QStringLiteral("QSize(%1x%2)").arg(size.width()).arg(size.height()).toStdString();
    return os;
}

std::ostream &operator<<(std::ostream &os, QPoint pt)
{
    os << QStringLiteral("QPoint(%1,%2)").arg(pt.x()).arg(pt.y()).toStdString();
    return os;
}

std::ostream &operator<<(std::ostream &os, QRect r)
{
    os << QStringLiteral("QRect(%1,%2 %3x%4)")
              .arg(r.x())
              .arg(r.y())
              .arg(r.width())
              .arg(r.height())
              .toStdString();
    return os;
}

using namespace KDDockWidgets;

inline Core::View *createViewAndWindow(Core::CreateViewOptions opts, Core::View *parent = nullptr)
{
    opts.createWindow = true;
    return Core::Platform::instance()->tests_createView(opts, parent);
}

#ifdef KDDW_FRONTEND_FLUTTER
#include "flutter_tests_embedder/tests_embedder.h"
#include "../src/flutter/Platform.h"
static int s_argc;
static char **s_argv;
int main(int argc, char **argv)
{
    s_argc = argc;
    s_argv = argv;

    KDDockWidgets::flutter::Platform::s_runTestsFunc = [] {
        doctest::Context ctx;
        ctx.setOption("abort-after", 4);
        ctx.applyCommandLine(s_argc, s_argv);
        ctx.setOption("no-breaks", true);
        return ctx.run();
    };

    KDDockWidgets::flutter::TestsEmbedder embedder(argc, argv);
    const int result = embedder.run();

    qDebug() << Q_FUNC_INFO << "tests ended with result=" << result;
    return result;
}


#else

int main(int argc, char **argv)
{
    int exitCode = 0;

    doctest::Context ctx;
    ctx.setOption("abort-after", 4);
    ctx.applyCommandLine(argc, argv);
    ctx.setOption("no-breaks", true);

    for (FrontendType type : Core::Platform::frontendTypes()) {

        Core::Platform::tests_initPlatform(argc, argv, type);
#ifndef KDDW_TESTS_NO_FATAL_WARNINGS
        Core::Platform::instance()->installMessageHandler();
#endif
        std::cout << "\nStarting tests for Platform" << Core::Platform::instance()->name() << "\n";

        int code = ctx.run();

        if (Core::Platform::instance()->m_numWarningsEmitted > 0) {
            std::cout << "ABORTING! Test caused a warning.\n";
            code = 1;
        }

        if (code != 0)
            exitCode = code;

        Core::Platform::instance()->uninstallMessageHandler();
        Core::Platform::tests_deinitPlatform();
    }

    return exitCode;
}
#endif
