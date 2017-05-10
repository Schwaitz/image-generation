# Image Generation Test

### Command Line Parameters

usage: ./gen.exe -w [width] -h [height] -s [spread] -p [passes] -r [red] -g [green] -b [blue]

modifiers:

	-w	width of the generated image

	-h	height of the generated image

	-s	spread (randomness) of the generated image

	-p	passes that the generation will go through

	-r	starting RGB red value

	-g	starting RGB green value

	-b	starting RGB blue value

### Examples

512x512 image with a spread of 18 and 1 pass

![512x512, spread: 18, passes: 1](http://i.imgur.com/lEzhFX4.png)

2048x1024 image with a spread of 10 and 1 pass

![2048x1024, spread: 10, passes: 1](http://i.imgur.com/ZkhNOtI.png)

2048x1024 image with a spread of 10 and 10 passes

![2048x1024, spread: 10, passes: 10](http://i.imgur.com/NmaDUPL.png)

1024x1024 image with a spread of 30 and 1 pass

![1024x1024, spread: 30, passes: 1](http://i.imgur.com/22Gh9ii.png)
