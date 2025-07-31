<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-optimize-python-code-with-golang/preview.jpg" />

## HOW TO OPTIMIZE YOUR PYTHON CODE WITH GOLANG: A COMPREHENSIVE GUIDE
`2024-06-20 08:12AM` • 15 min read • **#go** **#python** **#perf** **#speed**

---

Python is renowned for its simplicity and ease of use, making it a favorite for many developers(yes... even for me lol).
However, when it comes to heavy CPU or RAM processing tasks, Python's performance can sometimes fall short(if not MOST OF THE TIMES ? :wink 'for loops').
This is where Golang (Go) comes into play. By leveraging Go’s performance capabilities through bindings, we can significantly optimize our Python code. This small guide will explore three methods to achieve this: **using C parameters**, **Protocol Buffers (protobuf)**, and finally **Gopy**.

### WHY USE GOLANG WITH PYTHON?

Combining the strengths of Python and Go allows us to write high-performance applications. Python handles the high-level logic and user interface, while Go processes the performance-intensive tasks. This approach can lead to significant performance improvements, especially for applications involving complex calculations or data processing.

### GOPY

Gopy generates Python bindings for Go packages, allowing you to call Go functions directly from Python.

**Repository**: [gopy](https://github.com/go-python/gopy)

**Usage**: Suitable for seamless integration with Python, simplifying the process of using Go code within Python.

#### INSTALL DEPENDENCIES

First, we need to install the necessary dependencies.

For the python side :
```bash
$ pip install --upgrade setuptools wheel pybindgen
```
and on golang side :
```bash
$ go install golang.org/x/tools/cmd/goimports@latest
$ go install github.com/go-python/gopy@latest
```

#### BUILD THE SHARED LIBRARY

Use Gopy to build the shared library and generate the Python bindings.
Some go code :

```go
// main.go
package go_bind

import "fmt"

type TEstData struct {
    Data string
    Len  int
}

// NOTE: this is the function we're exporting
// the only syntax we need to add is 'export FunctionName'
//export HelloThere
func HelloThere(ex TEstData) TEstData {
    ex.Data = "Hello there from GO code '" + ex.Data + "';"
    ex.Len = len(ex.Data)
    return ex
}

func main() {
    fmt.Printf("Hello world!\n")
}
```

Then run :
```bash
$ gopy build -output=build -vm=python3 github.com/sanix-darker/gopy
```

#### RUN THE PYTHON TEST SCRIPT

Finally, run the Python test script to verify the integration:

```bash
# don't mind the 3.10, you can actually use whatever version you want atchuaaally
$ python3.10 ./test_go_bind.py
```

From :

```python
from build.go_bind import TEstData, HelloThere

ex = TEstData(Data="test", Len=0)
pp = HelloThere(ex)

print(f" >> {ex.Data=} :: {ex.Len}")
print(f" >> {pp.Data=} :: {pp.Len}")

# output
# >> ex.Data=test :: 0
# >> ex.Data=Hello there from GO code 'test'; :: 32
```

And BADABOUM, we're all good, now we're able to export a function AND a type defined in golang but use it
in our python code.

### 'C' PARAMETERS

Uses C parameters to create a C-shared library from Go code, which can then be called from Python.

**Repository**: Example provided in a custom repository.

**Usage**: Ideal for performance-critical applications, offering more control and efficiency.

#### INSTALL PYTHON DEPENDENCIES

Install the required Python packages (in this case, only cffi is needed):

```bash
$ pip install cffi
```

#### BUILD THE SHARED LIBRARY

Given the main.go code as follow :

```go
//main.go
package main

/*
#cgo pkg-config: python3
#define Py_LIMITED_API
#include <Python.h>

typedef struct {
    int xxx;
    char yyy[100];
} GoBindStruct;

*/
import "C"
import (
    "fmt"
    "unsafe"
)

type GoBindStruct struct {
    xxx C.int
    yyy *C.char
}

//export PrintThis
func PrintThis(a C.int, b *C.char) C.int {
    aa := GoBindStruct{
        a, b,
    }
    fmt.Printf("> input :: %d\n", aa.xxx)
    fmt.Printf("< output :: %s\n", C.GoString(aa.yyy))
    aa.xxx = aa.xxx + C.int(len(C.GoString(aa.yyy)))
    return aa.xxx
}

func main() {
    var strr *C.char = C.CString("dk")
    defer C.free(unsafe.Pointer(strr))
    PrintThis(1, strr)
}
```

Build the shared library from the Go code:

```bash
# > Building go_bind...
$ go build -buildmode=c-shared -o go_bind.so .

# OPTIONAL :
# > List exported funcs from go_bind...
$ nm -gC go_bind.so
```

#### BUILD PYTHON BINDINGS AND RUN THE PYTHON TEST SCRIPT

To build the python bindings, you just have to run :

```bash
$ python3 ./go_bind_builder.py
```

Then, run the Python test script to ensure everything is working correctly:

```bash
$ python3 ./test_go_bind.py
```

From :

```python
from go_bind_py import lib

print("=", lib.PrintThis(4, b"tintin"))
print(f">> go_bind_py :: export {dir(lib)}")

# output:
# =
# > input :: 4
# < output :: 'tintin'

# >> go_bind_py :: export ['PrintThis']
```

And as you can see, you can even list all available methods from the exported lib.


### USING PROTOCOL BUFFERS

Utilizes Protocol Buffers to serialize data, enabling efficient data interchange between Go and Python.

**Repository**: Requires setup with `protoc` and corresponding Go and Python protobuf libraries.

**Usage**: Best for applications needing efficient data serialization and language-agnostic data interchange, suitable for scalable projects.

#### INSTALL PYTHON AND GO DEPENDENCIES

On python side, we are going to need only those two :

```bash
$ pip install protobuf cffi grpcio-tools
```

We're going to need `protobuf/protoc-gen-go` installed to convert our .proto file into an exploitable go code.

```bash
$ go install github.com/golang/protobuf/protoc-gen-go@latest
```

#### GENERATE PROTOBUF FILES

Given this .proto file :

```go
// go_bind.proto
syntax = "proto3";
option go_package = "./";

package go_bind;

message Pack {
    string data = 1;
    int32 len = 2;
}
```

And the `main.go` file :

```go
// main.go
package main

/*
#cgo pkg-config: python3
#define Py_LIMITED_API
#include <Python.h>

*/
import "C"

import (
    "fmt"
    "unsafe"

    "github.com/golang/protobuf/proto"
)

//export GetDataInfo
func GetDataInfo(pb *C.char) *C.char {
    bytes := C.GoBytes(unsafe.Pointer(pb), C.int(C.strlen(pb)))
    data := &Pack{}
    err := proto.Unmarshal(bytes, data)
    if err != nil {
        fmt.Printf("Error unmarshaling protobuf: %v", err)
        return nil
    }
    fmt.Printf("Data: %s, Length: %d\n", data.GetData(), data.GetLen())
    data.Data = "Hi from GO, your input was '" + data.GetData() + "'"
    data.Len = int32(len(data.GetData()))
    modifiedBytes, err := proto.Marshal(data)
    if err != nil {
        fmt.Printf("Error marshaling protobuf: %v", err)
        return nil
    }
    return C.CString(string(modifiedBytes))
}

func main() {}
```

Generate some Go and Python code from protobuf files:

```bash
# All code in the Example section
$ protoc --go_out=. go_bind.proto
$ protoc --python_out=. go_bind.proto
$ python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. go_bind.proto
```

#### BUILD THE SHARED LIBRARY

Build the shared library from the Go code:

```bash
# > Building go_bind...
$ go build -buildmode=c-shared -o go_bind.so .

# OPTIONAL
# > List exported funcs from go_bind...
$ nm -gC go_bind.so
```

The `.so` file will be then used on the python side.

#### RUN THE PYTHON TEST SCRIPT

Run the Python test script to ensure everything is working correctly:

```bash
$ python3 ./test_go_bind.py
```

From python code as follow :

```python
from ctypes import c_char_p, CDLL
import pathlib
from go_bind_pb2 import Pack as PackMessage

libpath = pathlib.Path().absolute() / "go_bind.so"
lib = CDLL(str(libpath))

get_data_info = lib.GetDataInfo
get_data_info.argtypes = [c_char_p]
get_data_info.restype = c_char_p

pk = PackMessage(data="ok doki", len=101)
data = pk.SerializeToString()

result = get_data_info(data)

pko = PackMessage()
pko.ParseFromString(result)
print(f">> {pko.data=}")
print(f">> {pko.len=}")
```

### ADVANTAGES AND DISAVANTAGES

### WITH GOPY

**Advantages:**
- **Ease of Use**: Gopy simplifies the process of generating Python bindings for Go packages, handling much of the complexity for you.
- **Data Structures**: Go data structures are exported in a way that makes them easy to use in Python.
- **Integration**: Seamlessly integrates with Python, making it easier to call Go functions directly from Python.

**Disadvantages:**
- **Python Version Compatibility**: Works best with specific versions of Python (e.g., Python 3.10), which might require you to manage multiple Python environments.
- **Limited Customization**: Less flexibility in customizing the bindings compared to manual methods.

### WITH C PARAMETERS

**Advantages:**
- **Performance**: Directly calling C functions can be very fast, making this method suitable for performance-critical applications.
- **Control**: Offers more control over how the data is passed between Python and Go, allowing for optimization based on specific needs.

**Disadvantages:**
- **Complexity**: Requires a deeper understanding of both C and Go, as well as how to manage memory manually.
- **Error-Prone**: More prone to errors such as memory leaks or segmentation faults if not handled correctly.

### WITH PROTOCOL BUFFERS

**Advantages:**
- **Serialization**: Efficiently serializes data, making it easy to transfer complex data structures between Python and Go.
- **Language-Agnostic**: Protocol Buffers can be used with many languages, not just Python and Go, providing flexibility for multi-language projects.
- **Scalability**: Well-suited for applications that need to scale, as it provides a robust way to handle data interchange.

**Disadvantages:**
- **Setup Complexity**: Requires setting up Protocol Buffers and understanding the .proto file syntax, which adds to the initial setup complexity.
- **Performance Overhead**: Serialization and deserialization add some performance overhead compared to direct function calls.


### CONCLUSION

Integrating Golang with Python can significantly enhance the performance of your Python applications, especially for tasks that are computationally intensive. Whether you use C parameters, Protocol Buffers, or Gopy, each method has its advantages and can be chosen based on the specific needs of your project. By following the steps outlined in this guide, you can effectively combine the simplicity of Python with the performance of Go, creating powerful and efficient applications.

-----------
