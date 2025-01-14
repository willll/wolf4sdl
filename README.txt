Wolf4SDL by Moritz "Ripper" Kroll (http://www.chaos-software.de.vu)
Original Wolfenstein 3D by id Software (http://www.idsoftware.com)
=============================================================================

Wolf4SDL is an open-source port of id Software's classic first-person shooter
Wolfenstein 3D to the cross-plattform multimedia library "Simple DirectMedia
Layer (SDL)" (http://www.libsdl.org). It is meant to keep the original feel
while taking advantage of some improvements mentioned in the list below.


Main features:

 - Cross-plattform:
      Supported operating systems are at least:
       - Windows 98, Windows ME, Windows 2000, Windows XP, Windows Vista
         (32 and 64 bit)
       - Linux
       - BSD variants
       - KallistiOS (used for Dreamcast)
      Only little endian platforms like x86, ARM and SH-4 are supported, yet.

 - AdLib sounds and music:
      This port includes the OPL2 emulator from MAME, so you can not only
      hear the AdLib sounds but also music without any AdLib-compatible
      soundcard in near to perfect quality!

 - Multichannel digitized sounds:
      Digitized sounds play on 8 channels! So in a fire fight you will
      always hear, when a guard opens the door behind you ;)

 - Higher screen resolutions:
      Aside from the original 320x200 resolution, Wolf4SDL currently
      supports any resolutions being multiples of 320x200 or 320x240,
      the default being 640x400.
      Unlike some other ports, Wolf4SDL does NOT apply any bilinear
      or similar filtering, so the graphics are NOT blurred but
      pixelated just as we love it.

 - Fully playable with only a game controller:
      Wolf4SDL can be played completely without a keyboard. At least two
      buttons are required (shoot/YES and open door/NO), but five or more
      are recommended (run, strafe, ESC).

Additional features:

 - Two additional view sizes:
      Wolf4SDL supports one view size using the full width of the screen
      and showing the status bar, like in Mac-enstein, and one view size
      filling the whole screen (press TAB to see the status bar).

 - (Nearly) unlimited sound and song lengths:
      Mod developers are not restricted to 64kB for digitized sounds and
      IMF songs anymore, so longer songs and digitized sounds with better
      quality are possible.

 - Resuming ingame music:
      When you come back to the game from the menu or load a save game, the
      music will be resumed where it was suspended rather than started from
      the beginning.

 - Freely movable pushwalls:
      Moving pushwalls can be viewed from all sides, allowing mod developers
      to place them with fewer restrictions. The player can also follow the
      pushwall directly instead of having to wait until the pushwall has left
      a whole tile.

 - Optional integrated features for mod developers:
      Wolf4SDL already contains the shading, directional 3D sprites,
      floor and ceiling textures, high resolution textures/sprites,
      parallax sky, cloud sky and outside atmosphere features, which
      can be easily activated in version.h.


The following versions of Wolfenstein 3D data files are currently supported
by the source code (choose the version by commenting/uncommenting lines in
version.h as described in that file):

 - Wolfenstein 3D v1.1 full Apogee
 - Wolfenstein 3D v1.4 full Apogee (not tested)
 - Wolfenstein 3D v1.4 full GT/ID/Activision
 - Wolfenstein 3D v1.0 shareware Apogee
 - Wolfenstein 3D v1.1 shareware Apogee
 - Wolfenstein 3D v1.2 shareware Apogee
 - Wolfenstein 3D v1.4 shareware
 - Spear of Destiny full
 - Spear of Destiny demo
 - Spear of Destiny - Mission 2: Return to Danger (not tested)
 - Spear of Destiny - Mission 3: Ultimate Challenge (not tested)


How to play:

To play Wolfenstein 3D with Wolf4SDL, you just have to copy the original data
files (e.g. *.WL6) into the same directory as the Wolf4SDL executable.
Please make sure, that you use the correct version of the executable with the
according data files version as the differences are hardcoded into the binary!

On Windows SDL.dll and SDL_mixer.dll must also be copied into this directory.
They are also available at http://www.chaos-software.de.vu

If you play in windowed mode (--windowed parameter), press SCROLLLOCK or F12
to grab the mouse. Press it again to release the mouse.


Usage:

Wolf4SDL supports the following command line options:
 --help                 This help page
 --tedlevel <level>     Starts the game in the given level
 --baby                 Sets the difficulty to baby for tedlevel
 --easy                 Sets the difficulty to easy for tedlevel
 --normal               Sets the difficulty to normal for tedlevel
 --hard                 Sets the difficulty to hard for tedlevel
 --nowait               Skips intro screens
 --windowed             Starts the game in a window
 --res <width> <height> Sets the screen resolution
                        (must be multiple of 320x200 or 320x240)
 --resf <w> <h>         Sets any screen resolution >= 320x200
                        (which may result in graphic errors)
 --bits <b>             Sets the screen color depth
                        (Use this when you have palette/fading problem
                        or perhaps to optimize speed on old systems.
                        Allowed: 8, 16, 24, 32, default: "best" depth)
 --joystick <index>     Use the index-th joystick if available
 --joystickhat <index>  Enables movement with the given coolie hat
 --samplerate <rate>    Sets the sound sample rate (given in Hz)
 --audiobuffer <size>   Sets the size of the audio buffer (-> sound latency)
                        (given in bytes)
 --ignorenumchunks      Ignores the number of chunks in VGAHEAD.*
                        (may be useful for some broken mods)

For Spear of Destiny the following additional options are available:
 --mission <mission>    Mission number to play (1-3)
 --goodtimes            Disable copy protection quiz


Compiling from source code:

The current version of the source code is available in the svn repository at:
   svn://tron.homeunix.org:3690/wolf3d/trunk

The following ways of compiling the source code are supported:
 - Makefile (for Linux, BSD variants and MinGW/MSYS)
 - Cmake
 ex :
  cmake .. -GNinja && ninja && ninja install

IMPORTANT: Do not forget to take care of version.h!
   By default it compiles for "Wolfenstein 3D v1.4 full GT/ID/Activision"!


TODOs:
 - Add PC speaker emulation
 - Center non-ingame screens for resolutions being a multiple of 320x240
 - Add support for any graphic resolution >= 320x200


Known bugs:
 - None! ;D


Troubleshooting:
 - If your frame rate is low, consider using the original screen resolution
   (--res 320 200) or lowering the sound quality (--samplerate 22050)


Credits:
 - Special thanks to id Software! Without the source code we would still have
   to pelt Wolfenstein 3D with hex editors and disassemblers ;D
 - Special thanks to the MAME developer team for providing the source code
   of the OPL2 emulator!
 - Many thanks to "Der Tron" for hosting the svn repository, making Wolf4SDL
   FreeBSD compatible, testing, bugfixing and cleaning up the code!
 - Thanks to Chris for his improvements on Wolf4GW, on which Wolf4SDL bases.
 - Thanks to Pickle for the GP2X support and help on 320x240 support
 - Thanks to fackue for the Dreamcast support
 - Thanks to Xilinx, Inc. for providing a list of maximum-length LFSR counters
   used for higher resolutions of fizzle fade


Licenses:
 - The original source code of Wolfenstein 3D: license-id.txt
 - The OPL2 emulator (fmopl.cpp): license-mame.txt
