# Rendering Sandbox

# Build

```bash
cmake -DQt5_DIR:PATH=<path/to/Qt5>
cmake --build . [-j <threads>]
```

# Build Documentation

```bash
cmake -DQt5_DIR:PATH=<path/to/Qt5> -DBUILD_DOCS:BOOL=ON
cmake --build . [-j <threads>]
cmake --build . -t doxygen
```
