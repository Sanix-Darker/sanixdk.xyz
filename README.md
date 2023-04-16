## Sanixdk.xyz

Since this is a personnal website i don't know why there should be a readme for it... but whatever...

There are multiple steps/stage, but you can run the website under docker and the Makefile itself has a lot of instructions from it.
You just need to be use to bash tweaks :).

```bash
$ make
 Choose a command:
  compile             Compile the app builder itself
  compile-resources   To polish all resources suchy as the css or all potential images etc... ?
  build               Build the webpage that is going to be serve
  polish              To concatenate header/footer components to all the pages
  serve               Serve the app (Not implemented/nor ready yet)
  docker-build        Docker build for the website
  docker-run          Docker run for the website
```

This has my website builder, some Dockerfile/nginx.
And the content is markdown based.
