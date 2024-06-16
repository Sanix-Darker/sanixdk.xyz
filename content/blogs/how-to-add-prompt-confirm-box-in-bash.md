![bash-confirm](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-add-prompt-confirm-box-in-bash/preview.jpg)

## HOW TO ADD PROMPT BOX CONFIRMATION IN BASH
`2024-06-15 03:30PM` • 12 min read • **#cli** **#bash** **#linux**

Yo, what's up ? It's me again, with a #uselesstech linux/bash tips for you.

Creating a prompt confirm feature in Bash(or shell scripting in general) is essential for scripts that require user confirmation before executing 'critical' actions.
This feature can help prevent accidental commands that might disrupt systems or cause data loss (since most of them ar irreversible).
In this tutorial, we'll build a `_confirm` function in Bash that prompts the user for confirmation and executes a command based on their response. We'll also explain each part of the function step by step and provide examples of how to use it.


### INTRODUCTION TO *my `_confirm`* FUNCTION

The `_confirm` function is a method i made myself and i use it everytime when composing conditionals operations, it takes a message as input and, based on the user's response, executes a command. It also includes a non-interactive mode for automated scripts. Here is the complete `_confirm` function, followed by a detailed explanation of each part:

```bash
# Usage :
# Ex : _confirm "Message" echo "test"
_confirm(){
    args=("${@}")
    if [[ $NOTINTERACTIVE = "1" ]]; then
        echo "[+] ${args[0]} "
        callback=${args[@]:1}
        $callback
    else
        echo "[-] ${args[0]} "
        read -p "[?] (Y/y): " -n 1 -r
        if [[ $REPLY =~ ^[Yy]$ ]]
        then
            callback=${args[@]:1}
            $callback
            echo "-------------------------------------------"
            return 0
        fi
        return 1
    fi
    echo
}
```

### DETAILED EXPLANATIONS

#### STEP 1: DEFINE THE FUNCTION AND HANDLE ARGUMENTS

```bash
$ args=("${@}")
```

The `_confirm` function starts by capturing all arguments passed to it using `"${@}"`, storing them in an array called `args`.

#### STEP 2: CHECK FOR NON-INTERACTIVE MODE

```bash
$ if [[ $NOTINTERACTIVE = "1" ]]; then
```

This line checks if the `NOTINTERACTIVE` variable is set to `1`. This variable is used to bypass user confirmation, allowing the function to run non-interactively, which is useful for automated scripts.

#### STEP 3: EXECUTE THE COMMAND IN NON-INTERACTIVE MODE

```bash
$ callback=${args[@]:1}
$ $callback
```

Here, the function extracts the command to be executed from the arguments, starting from the second argument (`${args[@]:1}`), and stores it in the `callback` variable. It then executes the command stored in `callback`.

#### STEP 4: READ USER INPUT FOR CONFIRMATION

```bash
$ read -p "[?] (Y/y): " -n 1 -r
```

The `read` command prompts the user for input with the message `"[?] (Y/y): "` and waits for a single character input (`-n 1`). The user's input is stored in the `REPLY` variable.

#### STEP 5: CHECK USER RESPONSE AND EXECUTE COMMAND

```bash
$ if [[ $REPLY =~ ^[Yy]$ ]]; then
    callback=${args[@]:1}
    $callback
    echo "-------------------------------------------"
    return 0
```

If the user's input matches `Y` or `y` (using a regular expression `[[ $REPLY =~ ^[Yy]$ ]]`), the function extracts the command from the arguments and executes it. It then prints a separator line and returns `0` indicating success.

#### STEP 6: HANDLE NEGATIVE RESPONSE

```bash
$  return 1
```

If the user's input does not match `Y` or `y`, the function returns `1`, indicating that the command was not executed.

### EXAMPLES OF USING THE `_confirm` FUNCTION

Here are some examples of how you can use the `_confirm` function in your Bash scripts:

#### EXAMPLE 1: DOWNLOADING A FILE

```bash
$ filename="life.zip"
$ selected_link="http://internet.com/life.zip"
$ _confirm "> Download $filename?" wget $selected_link
```

In this example, the `_confirm` function prompts the user with the message "> Download example_file.txt?" If the user confirms by typing `Y` or `y`, the script executes the `wget` command to download the file from the specified link.

#### EXAMPLE 2: INSTALLING A PACKAGE

```bash
$ package_name="curl"
$ _confirm "> Install $package_name?" sudo apt-get install -y $package_name
```

Here, the function prompts the user with the message "> Install curl?" If the user confirms, the script runs the command to install the `curl` package using `apt-get`.

#### EXAMPLE 3: DELETING A DIRECTORY

```bash
$ directory="/path/to/directory"
$ _confirm "> Delete directory $directory?" rm -rf $directory
```

In this example, the function prompts the user with the message `"> Delete directory /path/to/directory?"` If the user confirms, the script deletes the specified directory using `rm -rf`.

### DEMO

![demo-confirm](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-add-prompt-confirm-box-in-bash/confirm-demo.gif)

### CONCLUSION

The `_confirm` function in Bash is a versatile tool i made myself for adding confirmation prompts on scripts, that you can adapt for your needs.
It handles both interactive and non-interactive modes, ensuring that user confirmation is sought only when needed. By following the step-by-step breakdown, you can customize and enhance this function to fit your specific requirements, adding a layer of safety to your Bash scripts.

With this guide, you should now be able to implement a prompt confirm feature in your own Bash scripts, making them more robust and user-friendly.

Don't forget to leave a reaction or a comment if you have any question or suggestion.
Thanks for reading and Happy scripting :)


-----------
