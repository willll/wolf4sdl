//
//	ID Engine
//	ID_US_1.c - User Manager - General routines
//	v1.1d1
//	By Jason Blochowiak
//	Hacked up for Catacomb 3D
//

//
//	This module handles dealing with user input & feedback
//
//	Depends on: Input Mgr, View Mgr, some variables from the Sound, Caching,
//		and Refresh Mgrs, Memory Mgr for background save/restore
//
//	Globals:
//		ingame - Flag set by game indicating if a game is in progress
//		loadedgame - Flag set if a game was loaded
//		PrintX, PrintY - Where the User Mgr will print (global coords)
//		WindowX,WindowY,WindowW,WindowH - The dimensions of the current
//			window
//
#include "wl_def.h"
#include <stdlib.h>
//#include <string.h>
char 	*strdup (const char *) __malloc_like __result_use_check;

//	Global variables
		word		PrintX,PrintY;
		word		WindowX,WindowY,WindowW,WindowH;

//	Internal variables
#define	ConfigVersion	1

static	boolean		US_Started;

		void		(*USL_MeasureString)(const char *,word *,word *) = VW_MeasurePropString;
		void		(*USL_DrawString)(const char *) = VWB_DrawPropString;

//		SaveGame	Games[MaxSaveGames];
		HighScore	Scores[MaxScores] =
					{
						{"id software-'92",10000,1},
						{"Adrian Carmack",10000,1},
						{"John Carmack",10000,1},
						{"Kevin Cloud",10000,1},
						{"Tom Hall",10000,1},
						{"John Romero",10000,1},
						{"Jay Wilbur",10000,1},
					};

int rndindex = 0;

static byte rndtable[] = {
      0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
	 74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
	 95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
	 52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
	149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
	145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0,
	175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
	 25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
	 94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
	136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
	135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
	 80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
	 24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
	145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
	 28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
	 71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
	 17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
	197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
	120, 163, 236, 249 };

//	Internal routines
// SOurce : https://stackoverflow.com/a/37132824/420446
char *ft_strdup(char *src)
{
    char *str;
    char *p;
    int len = 0;

    while (src[len])
        len++;
    str = malloc(len + 1);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}


//	Public routines

///////////////////////////////////////////////////////////////////////////
//
//	US_Startup() - Starts the User Mgr
//
///////////////////////////////////////////////////////////////////////////
void US_Startup()
{
	if (US_Started)
		return;

	US_InitRndT(true);		// Initialize the random number generator

	US_Started = true;
}


///////////////////////////////////////////////////////////////////////////
//
//	US_Shutdown() - Shuts down the User Mgr
//
///////////////////////////////////////////////////////////////////////////
void
US_Shutdown(void)
{
	if (!US_Started)
		return;

	US_Started = false;
}

///////////////////////////////////////////////////////////////////////////
//
//	US_Print() - Prints a string in the current window. Newlines are
//		supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_Print(const char *sorg)
{
	char c;
	char *sstart = ft_strdup(sorg);
	char *s = sstart;
	char *se;
	word w,h;
//	slPrint((char *)sorg,slLocate(2,10));
//#if 0
	while (*s)
	{
		se = s;
		while ((c = *se)!=0 && (c != '\n'))
			se++;
		*se = '\0';

		USL_MeasureString(s,&w,&h);
		px = PrintX;
		py = PrintY;
		USL_DrawString(s);

		s = se;
		if (c)
		{
			*se = c;
			s++;

			PrintX = WindowX;
			PrintY += h;
		}
		else
			PrintX += w;
	}
	free(sstart);
	sstart = NULL;
//#endif
}

///////////////////////////////////////////////////////////////////////////
//
//	US_CPrintLine() - Prints a string centered on the current line and
//		advances to the next line. Newlines are not supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_CPrintLine(const char *s)
{
	word	w,h;

	USL_MeasureString(s,&w,&h);

	if (w > WindowW)
		Quit("US_CPrintLine() - String exceeds width");
	px = WindowX + ((WindowW - w) / 2);
	py = PrintY;
	USL_DrawString(s);
	PrintY += h;
}

///////////////////////////////////////////////////////////////////////////
//
//  US_CPrint() - Prints a string centered in the current window.
//      Newlines are supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_CPrint(const char *sorg)
{
	char	c;
	char *sstart = ft_strdup(sorg);
	char *s = sstart;
	char *se;

	while (*s)
	{
		se = s;
		while ((c = *se)!=0 && (c != '\n'))
			se++;
		*se = '\0';

		US_CPrintLine(s);

		s = se;
		if (c)
		{
			*se = c;
			s++;
		}
	}
	free(sstart);
	sstart = NULL;
}

///////////////////////////////////////////////////////////////////////////
//
//	US_ClearWindow() - Clears the current window to white and homes the
//		cursor
//
///////////////////////////////////////////////////////////////////////////
void
US_ClearWindow(void)
{
	VWB_Bar(WindowX,WindowY,WindowW,WindowH,WHITE);
	PrintX = WindowX;
	PrintY = WindowY;
}

///////////////////////////////////////////////////////////////////////////
//
//	US_DrawWindow() - Draws a frame and sets the current window parms
//
///////////////////////////////////////////////////////////////////////////
void
US_DrawWindow(word x,word y,word w,word h)
{
	word	i,
			sx,sy,sw,sh;

	WindowX = x * 8;
	WindowY = y * 8;
	WindowW = w * 8;
	WindowH = h * 8;

	PrintX = WindowX;
	PrintY = WindowY;

	sx = (x - 1) * 8;
	sy = (y - 1) * 8;
	sw = (w + 1) * 8;
	sh = (h + 1) * 8;

	US_ClearWindow();

	VWB_DrawTile8(sx,sy,0),VWB_DrawTile8(sx,sy + sh,5);
	for (i = sx + 8;i <= sx + sw - 8;i += 8)
		VWB_DrawTile8(i,sy,1),VWB_DrawTile8(i,sy + sh,6);
	VWB_DrawTile8(i,sy,2),VWB_DrawTile8(i,sy + sh,7);

	for (i = sy + 8;i <= sy + sh - 8;i += 8)
		VWB_DrawTile8(sx,i,3),VWB_DrawTile8(sx + sw,i,4);
}

//	Input routines

///////////////////////////////////////////////////////////////////////////
//
//	USL_XORICursor() - XORs the I-bar text cursor. Used by US_LineInput()
//
///////////////////////////////////////////////////////////////////////////
static void
USL_XORICursor(int x,int y,const char *s,word cursor)
{
	static	boolean	status;		// VGA doesn't XOR...
	char	buf[MaxString];
	int		temp;
	word	w,h;

	strcpy(buf,s);
	buf[cursor] = '\0';
	USL_MeasureString(buf,&w,&h);

	px = x + w - 1;
	py = y;
	if (status^=1)
		USL_DrawString("\x80");
	else
	{
		temp = fontcolor;
		fontcolor = backcolor;
		USL_DrawString("\x80");
		fontcolor = temp;
	}
}

char USL_RotateChar(char ch, int dir)
{
    static const char charSet[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ.,-!?0123456789";
    const int numChars = sizeof(charSet) / sizeof(char) - 1;
    int i;
    for(i = 0; i < numChars; i++)
    {
        if(ch == charSet[i]) break;
    }

    if(i == numChars) i = 0;

    i += dir;
    if(i < 0) i = numChars - 1;
    else if(i >= numChars) i = 0;
    return charSet[i];
}

///////////////////////////////////////////////////////////////////////////
//
// US_InitRndT - Initializes the pseudo random number generator.
//      If randomize is true, the seed will be initialized depending on the
//      current time
//
///////////////////////////////////////////////////////////////////////////
void US_InitRndT(int randomize)
{
    if(randomize)
        rndindex = (SDL_GetTicks() >> 4) & 0xff;
    else
        rndindex = 0;
}

///////////////////////////////////////////////////////////////////////////
//
// US_RndT - Returns the next 8-bit pseudo random number
//
///////////////////////////////////////////////////////////////////////////
int US_RndT()
{
    rndindex = (rndindex+1)&0xff;
    return rndtable[rndindex];
}
