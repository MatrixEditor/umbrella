# umbrellacxx

A compact Python library, written in C++, designed for inspecting static Objective-C runtime information.

> **Note**: This package is not currently available via PyPI; you will need to compile and install it manually using the installation steps provided below.

A pure Python implementation that supports both Objective-C and Swift metadata, please visit [umbrella-py](https://github.com/MatrixEditor/umbrella-py) (this package also has an installation candidate).

## Principles

This small and efficient Python package enables you to inspect Mach-O binaries that store Objective-C runtime information. It supports the following use cases:

- Examining the static runtime information of Objective-C binaries. This includes a detailed recreation of Objective-C header files storing classes, extensions, categories, and protocols.
- A complete and reliable Objective-C type decoder with support for properties, method signatures, and default types.

## Usage

Below is a small collection of use-cases for this library:
```python
import umbrellacxx

# 1. Parse the binary file
metadata = umbrellacxx.objc.parse("/path/to/binary")

# Dump all class declarations (same with protocols and categories)
for objc_class in metadata.classes:
    print(objc_class.get_decl())

# Get a protocol by its name
protocol = metadata.get_protocol("Foo")

# Decode an Objective-C encoded type description
desc = umbrellacxx.objc.typedesc('T@"NSArray",&,N,V_foo')
print(umbrellacxx.objc.decode(desc))
# prints '@property (retain, nonatomic, ) NSArray foo'

# Decode a method's signature
sig = umbrellacxx.objc.signature("foo:bar:", "q24@0:8@16")
print(sig) # '(double)foo:(id) bar:(id)'
```
For more detailed information about the structure of each Python class, please refer to [objc.pyi](/bindings/python/umbrellacxx/objc.pyi).

## Installation

To install the Python package you need a nanobind-compatible compiler architecture, CMake and LIEF to be
installed. The complete setup is listed below:

1. Clone the repository

    ```console
    git clone https://github.com/MatrixEditor/umbrella.git && cd umbrella
    ```

2. Setup all relevant files using CMake. This step is necessary to check whether all components are installed
   correctly:

    ```console
    cmake -S . -B build
    ```

3. We use pip to install the python library:

    ```console
    cd bindings/python && pip install .
    ```

4. Done! You now should be able to import the python package:

    ```python
    >>> import umbrellacxx
    >>> metadata = umbrellacxx.objc.parse("/path/to/binary")
    >>> cls = metadata.classes[0]
    >>> print(cls.get_decl())
    """
    @interface DebugSettingsViewController: AdvancedSettingsViewController
    // methods
    - (void)viewDidLoad // 0x100006fc4
    - (id)createSettingsForUser:(id) // 0x100007140
    @end
    """
    ```

### Building the documentation

In order to build the documentation locally, Doxygen must be installed. **(Make sure to setup the project using CMake before running Doxygen)**:

1. Setup and build documentation:

    ```console
    cmake -S . -B build && doxygen DoxyFile
    ```

## License

Distributed under the Apache 2.0 License. See LICENSE for more information.