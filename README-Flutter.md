# KDDockWidgets for Flutter

> ⚠️ This document is intended for KDDW developers.
> ⚠️ The flutter port is not ready for the general public.

## Requirements

- Flutter >= 3.10 && Dart 3

- Qt 6.5 (Only QtCore. To be removed soon)

- CMake, Ninja

## Build instructions

### Linux

Adjust paths as needed.

Preparatives:

```bash
export PATH=/home/user/Qt/6.5.1/gcc_64/bin/:$PATH
export KDDW_SOURCE_DIR=/home/user/somewhere/kddockwidgets/
```

Release build (Recommended):

```bash
export DARTAGNAN_BINDINGSLIB_PATH=$KDDW_SOURCE_DIR/build-release-flutter/lib
cd $KDDW_SOURCE_DIR
cmake --preset=release-flutter
cmake --build build-release-flutter
```

If you need to run tests, then you need a developer build, which is a bit more involved.<br>
You'll need to point `FLUTTER_ENGINE_FOLDER` to the folder containing a vanilla
flutter engine along with `flutter_embedder.h`.

Developer build:

```bash
export DARTAGNAN_BINDINGSLIB_PATH=$KDDW_SOURCE_DIR/build-dev-flutter/lib
export FLUTTER_ENGINE_FOLDER=/home/user/somewhere/flutter-embedder/
cd $KDDW_SOURCE_DIR
cmake --preset=dev-flutter
cmake --build build-dev-flutter
```

## Running the example

```bash
cd $KDDW_SOURCE_DIR/examples/flutter
flutter run -d linux # Or mac/windows, or nothing if you only have 1 flutter "device"
```

## Windows && macOS

To be tested. Probably won't be too different.

## TODO

- Styling

- Support flutter multi-window

- Make more tests pass

- Don't depend on QtCore

- Package