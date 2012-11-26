# Kinect playground

This project is my playground for experimenting with the Kinect depth
camera. Right now I'm interested in how to stream depth data with
minimal overhead so it can be consumed by higher-level languages.

If you're looking for an easy way to get started with the Kinect and
[OpenKinect](http://openkinect.org/)/libfreenect in C, or if you're
curious about depth image processing in JavaScript, the code here
might be a good starting point for you.


## What you need to get started

1. A [Microsoft Kinect](http://www.xbox.com/KINECT). You can buy one
at any major electronics retailer. It's sold as an Xbox accessory, but
it comes with a USB adapter that will plug right into your computer.

2. A C compiler. OS X users can install Xcode for this.

3. [libfreenect](https://github.com/OpenKinect/libfreenect) and
[libpng](http://www.libpng.org/). OS X users can install both
libraries using [Homebrew](http://mxcl.github.com/homebrew/).

4. A recent version of [Node.js](http://nodejs.org/) (>= 0.8) if you'd
like to run the browser example.

To compile the example programs, check out this repository and run
`make`. You may need to edit the `Makefile` to adjust the path to
libfreenect's header files.


## Example: Watching depth data from the command line

The `kinect-depth-ascii` program was largely accidental. I needed a
way to make sure I was reading the Kinect's depth data properly, and
the simplest way to debug it was just to write it out to the
terminal.

<img src="http://i.sstephenson.us/ascii_kinect.gif" width="570" height="438">

Source code:
[`kinect-depth-ascii.c`](https://github.com/sstephenson/kinect/blob/master/kinect-depth-ascii.c)


## Example: Streaming depth PNGs to another program

PNG is a good format for encoding Kinect depth data because it's
lossless, universally readable, and compresses reasonably well. The
`kinect-depth-png` program encodes the Kinect's depth images in
320x240@8bpp PNG images and streams them over stdout using the
[netstrings](http://cr.yp.to/proto/netstrings.txt) format.

Source code:
[`kinect-depth-png.c`](https://github.com/sstephenson/kinect/blob/master/kinect-depth-png.c)


## Example: Watching depth PNGs in the browser

You can pipe `kinect-depth-png`'s output to the `browser/server`
program, a Node.js web server that in turn streams the PNGs as
Base64-encoded `data:` URIs using  [HTML5 Server-Sent
Events](http://www.w3.org/TR/eventsource/).

The example client page simply sets an `<img>` tag's `src` attribute
to the URI after each frame is received without further processing.

:movie_camera: [Watch video: Depth images streamed simultaneously to
multiple browsers](http://f.cl.ly/items/1z1R100K1O3R2I1I3F43/kinect%20png%20stream.mov)

Usage: Run `./kinect-depth-png | browser/server` in a terminal, then
visit `http://localhost:5600/` in a browser.

Source code:
[`browser/server`](https://github.com/sstephenson/kinect/blob/master/browser/server),
[`browser/client.html`](https://github.com/sstephenson/kinect/blob/master/browser/client.html)


## Example: Depth edge detection in the browser

Building on the previous example, the `edge.html` example page
demonstrates how to perform live edge detection of Kinect depth images
in the browser.

The streamed depth images are drawn to a `<canvas>` element using the
[`drawImage`](http://www.whatwg.org/specs/web-apps/current-work/multipage/the-canvas-element.html#dom-context-2d-drawimage)
function. The depth image is then extracted into a typed array using
[`getImageData`](http://www.whatwg.org/specs/web-apps/current-work/multipage/the-canvas-element.html#dom-context-2d-getimagedata)
and passed to a [Web
worker](http://www.whatwg.org/specs/web-apps/current-work/multipage/workers.html),
where it is processed by the [Sobel
operator](http://en.wikipedia.org/wiki/Sobel_operator) to detect
edges. Then the computed edge image is returned back to the page and
composited atop the depth image.

:movie_camera: [Watch video: Live edge detection of Kinect depth
images in the browser](http://f.cl.ly/items/3N21382j1Z1u2K2U3V1x/kinect%20edge%20detection.mov)

Usage: As above, but visit `http://localhost:5600/edge.html`.

Source code:
[`browser/edge.html`](https://github.com/sstephenson/kinect/blob/master/browser/edge.html)


-----

Copyright &copy; 2012 Sam Stephenson.

Released under the MIT License.
