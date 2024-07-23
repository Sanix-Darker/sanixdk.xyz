![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/build-go-code-and-use-in-php-codebase/preview.jpg)

## BUILD GO CODE AND USE IT IN PHP CODEBASE
`2024-06-10 07:03AM` • 7 min read • **#php** **#go** **#performance**

Building Go code and integrating it into a PHP codebase can help increase performance, particularly for tasks that are computationally intensive. Here are the steps to achieve this:

1. **Write and Build Go Code**:
   - Write the Go code you want to use.
   - Build the Go code into a shared library.

2. **Use `cgo` to Generate Shared Library**:
   - Use `cgo` to compile the Go code into a shared object (`.so`) file that can be called from PHP.

3. **Call the Shared Library from PHP**:
   - Use PHP's `FFI` (Foreign Function Interface) extension to call functions from the shared library.

Here are detailed steps:

### WRITE AND BUILD GO CODE

First, write your Go code. For example, create a file called `example.go`:

```go
package main

import "C"

//export Add
func Add(a, b int) int {
    return a + b
}

func main() {}
```

### BUILD GO CODE INTO SHARED LIBRARY

Compile the Go code into a shared library:

```bash
$ go build -o libexample.so -buildmode=c-shared example.go
```

This will produce two files: `libexample.so` (the shared library) and `libexample.h` (the header file).

### INSTALL PHP FFI EXTENSION

Ensure that the PHP `FFI` extension is installed and enabled. You can install it via PECL if it’s not already available:

```bash
# install libffi if not available already
# This command is for .DEB distro
$ apt-get install libffi-dev -y
$ pecl install ffi

# On error, you may use a specific channel
```

Add the extension to your `php.ini`, depending on the php version you installed (for example for php8.2 -> `/etc/php/8.2/apache2/php.ini`):

```ini
extension=ffi.so
```

Then restart the apache2 service (if it's the webserver you're using).

```bash
$ sudo service apache2 restart
```

### USE THE SHARED LIBRARY IN PHP

Create a PHP script to use the shared library. For example, `use_go.php`:

```php
<?php

// Load the shared library
$ffi = FFI::cdef(
    "int Add(int a, int b);",
    "./libexample.so"
);

// Call the Add function from the Go library
$result = $ffi->Add(5, 7);
echo "The result is: " . $result . "\n";

?>
```

### RUN THE PHP SCRIPT

Run your PHP script:

```bash
$ php use_go.php
```

By following these steps, you can leverage Go’s performance advantages within a PHP codebase. This method is particularly useful for performance-critical sections of your application.

-----------
