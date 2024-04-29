## sanixdk.xyz

Hi.
since this is my custom builder for my personnal website
i don't know why there should be a readme for it but whatever...

There are multiple steps/stage, but you can run the website under docker and the Makefile itself has a lot of instructions from it.
You just need to be use to bash tweaks :).

```console
$ make
 Choose a command...
  compile        Compile the app builder itself
  build          Build the webpage that is going to be serve
  serve          Serve the app (Not implemented/nor ready yet)
  docker-build   Docker build for the website
  docker-run     Docker run for the website
  up             To build and deploy the website
  down           To get down the service
```

This has my website builder, some Dockerfile/nginx.
And the content is markdown based.

to start everything up, you just need to run :
```console
make compose
```
