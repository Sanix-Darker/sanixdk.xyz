![image](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/jldl9kw57wv3z0be076q.jpg)

## TEST YOUR ENTIRE PROJECT(PHP, JS, PYTHON, RUBY) WITH JUST COMMENTS.
`2023-11-16 11:03PM` • 9 min read • **#cli** **#test**

Hey there, today am presenting you something really special i made about tests, i hope you're going to like the way i build this solution !
Now, let's jump into it !

### Introduction

Let's face it frankly, tests are certainly important but they are sometimes extremely boring to write, when you couple that with the fact that each language or each framework has its own way of handling unit tests, it does not help them. things !

![Boring](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/h72carv0ynad6ea928d3.jpg)

So I asked myself a fairly simple question, is it possible to manage unit tests in a universal way whatever the programming language "script" or the framework !

![solution](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/cxyf4p6apcay5uk6te25.gif)


### What do all code projects have in common?

The answer is quite simple, the **comments**; they are the only elements which are present in almost any project, whether it is with a scripting language or a compiled language.

![Alt Text](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/chvjhuof79i1u2cgn8h9.png)


### How can we transform simple comments to unit tests ?

Here we are trying to create a system that will do unit tests in a file or a project based on comments, so it was a question for us to define a kind of uniform syntax whatever the programming language in which we are, then create a pseudo test environment like a file in which we put all the elements of testa class and we test the corresponding assertion !

Behind all of this we would have a cli program that will browse and fetch all tests cases and test them to generate a final report.

For a simple assertion we end up with something like this :

```python
# ::testa_start::
# ::case_start::
# >> testa.isEqual(2, 2)
# << true
# ::case_end::
# ::testa_end::
```
In this example, we use `::testa_start::` and `::testa_end::` to surround our pseudo test environment, we can now define a test case with `::case_start::` and `::case_end::`, then using `>>` and `<<` to define the expected inputs and outputs, here *testa.isEqual* just checks if 2 is equal to 2, and we expect a true in output, we also have *testa.isSup*, etc ...

With this structure, we can therefore structure several test cases in a single test pseudo environment in the code :
```python
# Inside a python code

# ::testa_start::
# ::case_start::
# >> testa.isEqual(2, 2)
# << true
# ::case_end::

# ::case_start::
# >> testa.isSup(3, 2)
# << true
# ::case_end:

# ::testa_end::
```

What should be remembered here is that if I want to do the same tests on javascript for example, I just have to replace the comments because, on javascript, the comments per line are `//` and not `#`; so we are going to have :

```javascript
// Inside a javascript code

// ::testa_start::

// ::case_start::
// >> testa.isEqual(2, 2)
// << true
// ::case_end::

// ::case_start::
// >> testa.isSup(3, 2)
// << true
// ::case_end:

// ::testa_end::
```

### How to encapsulate a method and define the correct output relative to a precise input ?

Just like the assertion of a simple test case, we define a precise syntax to surround our method !
```python
# ::testa_start::
# ::case_start::
# >> addition(2, 2)
# << 4
# ::case_end::
# ::code_start::
def addition(a, b):
    """ This method will return the sum
    of a and b """

    return a+b
# ::code_end::
# ::testa_end::
```
Unlike the simple case tests, here we have `::code_start::` and `::code_end::` which define the pieces of code that must be taken into account in the pseudo test environment.

After the execution of the tests in a file/project, testa we generate a test report parraport to the tests which are in success and those which are in failed !


### How to manage imports from external libraries !

In some cases we often have functions that call external libs or other functions in other files, to solve this problem, I added to the syntax of testa `::import_start::` and `::import_end::`, this part in a file includes all the imports which will be necessary for the tests in each pseudo environment (files with assertion).

```python
# ::import_start::
Your imports here
# ::import_end::
```


### How to standardize these tests with only one command line.

For this last problem I used the configuration file model, a bit like the `package.json` with *npm* or *yarn*, in this case `tesla.json`, but a fairly simple file which describes which directory (file or project) to test, from which language it is and what is the interpreter that must be used, so we have this semblance of structure:

- An example on `PHP`:
```json
{
    "module": "ta-php",
    "path": ["./square.php"],
    "extensions": [".php"],
    "launcher": "php"
}
```
- An example with `python`:
```json
{
    "module": "ta-py",
    "path": ["./sha256.py", "./square.py"],
    "extensions": [".py"],
    "launcher": "python"
}
```
- An example on `Ruby`:
```json
{
    "module": "ta-rb",
    "path": ["./my_ruby_app/", "./square.rb"],
    "extensions": [".rb"],
    "launcher": "ruby"
}
```


### Conclusion

In conclusion, we can say that we have made a kind of library which works on the CLI allowing to do unit tests whatever the scripting programming language is and to set it up, it's quite simple.

![Installation](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/gjrsd1fvk4zgz5q832wa.png)

Testa is a free and open source project, this is some link !
- Source code : [On github](https://github.com/Sanix-Darker/testa),
- The demo : [VIDEO-DEMO](https://www.loom.com/share/6d691436356a48ad9ac3ba768459f883)

-----------
