## HOW TO HACK and SECURE YOUR TERMBIN content
`2025-01-01 13:12AM` **#bash** **#unix** **#terminal**

### WHAT THE HECK IS TERMBIN ?

**termbin.com** is a variation of **pastebin.com** but right on your terminal,
it is a really great tool if you're a [`tty`](https://en.wikipedia.org/wiki/Tty_(Unix)) fan like me, and you want to share text content accross the internet, whether it's the standard output/err of commands to someone or programmatically to another service or anything else (as text),

All you need is to pipe the output to a netcat command line to achieve this, so `netcat` need to be installed.
```bash
$ echo "hello WaZabi" | nc termbin.com 9999
https://termbin.com/h9id
```
And just like that, you now have a public link to access your text content.

NOTE: For the rest of this blog post, we're going to use an alias for this termbin pipe,
`alias tb='nc termbin.com 9999'`

### PRACTICAL USES CASES

#### FROM ASSETS/CDN STORAGE

Imagine you're building a web app and you need to store assets, js/css files somewhere, with termbin, it's extremly easy to do so, **HOWEVER**, you need to be aware that your content will be automatically wipe after one week, following what is said on the website :
```
7. Duration of Data Retention

The lifespan of any data or content posted on the service is limited to one week. After this period, such data will be automatically deleted.
```

#### FROM COMMAND LINE

How can this tool be usefull from command line ? you may ask;
Imagine, you have an error on compile-time of a C program,

```c
#include <stdio.h>

#define ARRAY_SIZE 10
#if ARRAY_SIZE < 130
#error "Array index out of bounds!"
#endif

int main() {
    return 0;
}
```
And you want to compile it and share the output with a different service or let say a collegue in your team :

```bash
$ gcc -Wall -Wextra -Wpedantic -std=c11 -o out in.c
in.c:5:2: error: #error "Array index out of bounds!"
    5 | #error "Array index out of bounds!"
      |  ^~~~~
```

The easiest way is to pipe the output of the command to the termbin :

**BUT WAIT**, this is a command that will surelly fail, and if it fails, the pipe to termbin will not happens.
We can "patch" that with `2>&1`, and here we're doing 2 things:
   - Redirects file descriptor 2 (stderr) to file descriptor 1 (stdout), combining them.
   - Ensures both standard output and error messages are piped into the next command (termbin).

The final result looks like :
```bash
$ gcc -Wall -Wextra -Wpedantic -std=c11 -o out in.c 2>&1 | tb # remember the alias we defined up there.
https://termbin.com/x190
```

and if you check the content of https://termbin.com/x190, it will output the actual error message :

```bash
$ curl -fSsl https://termbin.com/x190
in.c:5:2: error: #error "Array index out of bounds!"
    5 | #error "Array index out of bounds!"
      |  ^~~~~
```

##### BONUS

Imagine, you want to send the stdout on termbin... but you also want to get it on your terminal after running the command, you can use `tee` :

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -o out in.c 2>&1 | tee >(nc termbin.com 9999)
```
The `tee` command reads from standard input and writes to standard output and files, effectively allowing you to duplicate the output stream.

### WHAT IS MISSING (THE PROBLEM)

Even though I found termbin really amazing... I think it's a lack of "security"... how ?
These generated links are publis and do not require authentication nor authorization to access them... this means that anyone can randomly click on a termbin link and get someone's saved content;

**FOR EDUCATIONAL PURPOSES ONLY** i will demonstrate you that by creating this small python program that can loop over a randomly selected link and if the content seems valid it is saved to a file :

```python
import requests
import random
import string
import time

valid_tracks = set()
random_tracks = set()
start = time.time()
def get_unique_track():
    while True:
        track = ''.join(
            random.choices(string.ascii_letters + string.digits, k=4)
        )
        if track not in random_tracks:
            random_tracks.add(track)
            return track

def loop_get_bin():
    while True:
        time.sleep(0.1) # we wait a litle to not get ban

        random_choose = get_unique_track()
        url = f"https://termbin.com/{random_choose}"
        r = requests.get(url)
        if r.status_code != 200:
            continue

        valid_tracks.add(random_choose)
        print(f"Generated {len(random_tracks)}")
        print(f">Valid track found {url}!")
        elapsed = time.time() - start
        print(f"Time elapsed: {elapsed}")
        with open("valid_termbin", "a+") as f:
            f.write(url)

if __name__ == "__main__":
    print("fetch started...")
    loop_get_bin()
```

And just like that, it's easy to fetch what people saved there :

```bash
$ python3 ./tmp.py
fetch started...
Generated 150
>Valid track found https://termbin.com/7hls!
Time elapsed: 28.98163866996765 s
Generated 316
>Valid track found https://termbin.com/vn05!
Time elapsed: 61.754194498062134 s
Generated 388
>Valid track found https://termbin.com/xqnx!
Time elapsed: 75.26441860198975 s
Generated 617
>Valid track found https://termbin.com/gv8a!
Time elapsed: 119.45438385009766 s
Generated 697
>Valid track found https://termbin.com/767l!
Time elapsed: 134.81792902946472 s
Generated 841
>Valid track found https://termbin.com/458n!
Time elapsed: 162.68385577201843 s
Generated 991
>Valid track found https://termbin.com/ik02!
Time elapsed: 191.86978006362915 s
```

The most scary part of this is how people can share extreme sensitive informations there,
i can go further by adding a filtering on content fetched to try to extract each line containing 'password' or 'token'
keyword... but that's not the purpose of this blog post.

And from the website you can read this :
```
5. Liability Disclaimer

The service owner is not responsible for any wrongdoing, illegal activity, or any harm caused by the use of this service. Users are solely responsible for their actions and the content they post on our platform.
```

### MY SOLUTION

Use encryption of your content before sending to termbin,
In my case i use `gpg` for all the content am publishing there,
You can install `gnupg` depending on your OS with :

```bash
# For Debian/Ubuntu
sudo apt-get install gnupg

# For Red Hat/CentOS
sudo yum install gnupg

# For Fedora
sudo dnf install gnupg

# For Arch Linux
sudo pacman -S gnupg

# For openSUSE
sudo zypper install gpg2
```

and with these simple bash methods below (each steps explained) you can effectively secure everything you share across termbin:
Even if it's a file or just a text.

```bash
# NOTE: you can change the key to put anything else you want,
# you will need it for encryption and decryption :
export AES_KEY_EN_DE="0123456789ABCDEF0123456789ABCDEF"

# Encrypt function: Takes key first, then input (file or string).
encrypt() {
    local key=$1     # Encryption key
    local input=$2   # File path or raw string

    if [[ -f "$input" ]]; then
        # Encrypt a file and encode in Base64
        gpg --symmetric --cipher-algo AES256 --passphrase "$key" --batch --yes --output - "$input" | base64
    else
        # Encrypt a string and encode in Base64
        echo -n "$input" | gpg --symmetric --cipher-algo AES256 --passphrase "$key" --batch --yes --output - | base64
    fi
}

# Decrypt function: Takes key first, then Base64 input.
decrypt() {
    local key=$1     # Decryption key
    local input=$2   # Base64-encoded string

    # Decode Base64 and decrypt the data
    echo -n "$input" | base64 --decode | gpg --quiet --decrypt --passphrase "$key" --batch --yes
}

# to host anything
# Usage : cat index.html | tb
tb(){
    # To read from the pipe:
    local input="$(cat)"
    echo "$input" | nc termbin.com 9999
}

tsend(){
    encrypt "$AES_KEY_EN_DE" "$@" | tb
}
tget(){
    local eBin=$(curl -LSs "https://termbin.com/$@")
    decrypt "$AES_KEY_EN_DE" "$eBin"
}
```

I modified a little bit our `tb` alias to take from `stdin`, added an `encrypt` and `decrypt` methods.

#### EXAMPLE USE CASES

Now, let's try out **tsend** and **tget**,

```bash
$ tsend "Hello there, this is neo"
https://termbin.com/tfg4
```

Now if anyone try to get the content, it will looks like :
```bash
$ curl -fSsL https://termbin.com/tfg4
jA0ECQMCPUYnVfJfB87/0koB/zhVM17HprE8kztungEwJ4BvP4NgNrLzRK+1Hv4k7A/MgRwCK7Ar
h1mAS84dcmv/Zxg8TOz1M/24o38UEIrPlqlL0Fpjm+wLOw==
```

But, if i use the tget, that know what is my key (exported in the bash script up there) :
```bash
$ tget tfg4
Hello there, this is neo
```

And that, is how easy you can add a small layer of security on your text content shared over the internet.

**NOTE:** This is available for any type of provider on which you want to share, in this case it's termbin, but that can be easily replaced by something else.

-----------
