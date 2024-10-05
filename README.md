# GrizzlyOne
Chess project for AVR 

## History
This project is a long term wish to be completed at any time. It is born in the good old 80s 
when chess computers had 8-bit CPUs and play awful chess. <br>
The goal is to build a chess computer with 8-bit hardware which plays at least equal to those old
machines in times where men won nearly all games.<br>
<br>

The code is not complete. <br>

- the chess rules are implemented
- it hangs when thinking a little deeper when one move
- evaluation is not good

## Two souls
<b>AVR</b>
The chess program is mainly developed to run on an AVR on a self made hardware.
To find out more visit www.slengo.de and look for GrizzlyOne.
The hardware is open source. It is a little chess computer with baord based on an AVR.
<br><br>
<b>Computer</b> 
It will be possible to test the chess engine on Linux, Windows or anywhere else with gcc and make tools.
The enigine will get a winboard interface to be tested with some chess frontends.
<br><br>
But it is not finished yet. So just wait and keep it watching.

## Platforms
### Grizzly One Board
GrizzlyOne is meant to run on the GrizzlyOne board. But because of limited debugging resources a second version has been made.
### GrizzlyOne makefile project
While sharing one engine the UI is seperated for AVR and desktop. The desktop part is meant for develope and debug the engine. It will have not many features in the beginning.

It will support WindBoard and / or UCI protocol maybe later to use it as competing version for development of other Grizzly Engines.
