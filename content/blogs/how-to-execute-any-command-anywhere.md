![cover](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/uu6pc0ipayoog1316pli.jpg)

## HOW TO EXECUTE ANY COMMAND FROM ANYWHERE TO ANYWHERE IN YOUR LAPTOP !
-----------
`2023-12-14 02:03AM` **#bash** **#unix** **#terminal**

Sometimes i got in situations where i want to execute a specific command or multiple commands inside a given directory without moving there as quickly as i can !
This may sound obvious to just jump there, do the stuff and come back but as a lazy guy, i like doing that as quickly as possible and hit enter once !

![target](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/xxpojlupfhlk96l544mc.png)

So in this tip, you will need [zoxide](https://github.com/ajeetdsouza/zoxide), a nice tool that cache all your paths by their finals dirname, that mean wherever you are in your linux system `~/zig/zag/toum/yyyy`, you just have to type *cd xxxx* (which is here `~/boum/bam/xxx`), to go there !

So this is my tip:
- go to the target dir (`$1` as the given dir)
- execute the command (`{$@:2}` as all strings after the dir)
- get back from where i was (`cd -`)

![Image](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/ie9fgl7nm3okmqygtkpf.jpg)

Then add it inside your `~/.bash_aliases` and you're good to go !

You can copy the source code from [here](https://gist.github.com/Sanix-Darker/27a0cb536b9c2ec7f29f93c70b942785) !

-----------
