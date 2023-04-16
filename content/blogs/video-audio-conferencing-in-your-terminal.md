![Video-Conf](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/ic32yegr54gnjvok9ezy.png)

## Video and Audio Conferencing in your terminal
-----------
`2022/05/01 11:03:10 AM` **#cli** **#terminal**

Yeah i know, what you guys are thinking "Why ?", "Why do you do that ?", well "just for fun ?" and a way to mastering "real-time parallelism", this is also my first POST ever made!

Let's jump into it !
First of all, a videoconferencing system is a real-time application which consists in broadcasting to each person connected the audio and video stream of all the other participants with the least possible latency !


As I had to work on the terminal, I only had characters and "bytes" for data entry!

### How to transform an image into a perceptible string of characters on a terminal ?

My algorithm is very simple, I first use OpenCv to retrieve the image that the camera /dev/0 device gives me, after that, i convert to black on white using the cvtColor method from OpenCV; then i cut this image into a sort of grid which gives me the color contrast by squares ranging from 0 to 255, I have a sort array equivalence of each pixel! By setting another array of characters ranging from "" to "B", I have a match against the occupancy rate in the pixel !
To complicated ?

Let me explain again, more a pixel is dark, the less dense the character will be, so we will have either a "." or a "," with the color value `10` or `12` unlike "B" or "K" which is closer to `250` or `255` From there, I manage to have my transcription of a color image in characters on a terminal !
![ASCII](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/6pap4pv0adhcnb84dt08.gif)

### How to retrieve the audio stream and transmit it ?

To do this I use PyAudio which connects to the microphone to retrieve the audio stream, then transmit it in the form of bytes per sequences, at the reception, i do a remodeling according to bit rate !


### How to send and retrieve the stream(video and audio) for each session ?

Here I am using a byte formatting system in json by adding elements allowing me to identify the one sending and in which session it is sending it, when a client retrieves the data, analyzes it, then retrieves the contents of the package
![Sessions](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/yggp0fgsivn2pezlmtz3.png)

### How does security work in such a system ?

I have implemented two types of data encryption, asymmetric encryption and symmetric encryption, for both systems there is key sharing that needs to be done first, but in the version being tested I have disabled all encryption, as this has an impact on the final rendering quality!

#### AES (Symmetric encryption)

![AES](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/1gunnuj99qup5euyey3m.png)

#### RSA (Asymmetric encryption)

![RSA](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/4klq1h54rmu6wj2urpg2.gif)

To use s2c, just install it via pip !
```shell
pip install s2c
```

Start the server :
```shell
s2c_server -p 1122
```

Then start the client :
```shell
s2c -s session1 -c darker -i 127.0.0.1 -p 1122
```

Or you can use help to see the manual :
![Manual](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/nf830zhk0e177ktq00es.png)

Of course, all this article would be useless without a DEMO: [WATCH ME](https://www.loom.com/share/3692b64a140a4b8e87d2a0319f8ae3d5)

s2c is Open Source and free !

- Source Code : [s2c github](https://github.com/sanix-darker/s2c)
- Pypi-Package : [s2c pypi](https://pypi.org/project/s2c)

PS : Any comments or constructive advice are welcome !

<script src="https://giscus.app/client.js"
        data-repo="sanix-darker/sanixdk.xyz"
        data-repo-id="video-audio-conf"
        data-category="blog"
        data-category-id="b"
        data-mapping="pathname"
        data-strict="0"
        data-reactions-enabled="1"
        data-emit-metadata="0"
        data-input-position="bottom"
        data-theme="dark"
        data-lang="en"
        data-loading="lazy"
        crossorigin="anonymous"
        async>
</script>
