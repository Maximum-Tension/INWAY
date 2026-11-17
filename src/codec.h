/******************************************************************************\
# H - CODEC                                      #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# :  License - GPL-3.0  :: Update - 2026/11/17 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/
/******************************************************************************\
|* __________________________________________________________________________ *|
|*|                                  CODEC                                   |*|
|*|__________________________________________________________________________|*|
|*|             ________                                                     |*|
|*| A TINY LOSSLESS IMAGE PACKER FOR ONE SCREEN TILE, IN THE SPIRIT OF QOI:  |*|
|*| RUNS OF THE SAME COLOUR, A 64 COLOUR CACHE, SMALL DIFFERENCES TO THE     |*|
|*| LAST PIXEL, OR THE FULL RGB COLOUR. ONLY RGB IS SENT, ALPHA IS DROPPED.  |*|
|*| WHILE THE SCREEN MOVES (A WINDOW OPENS, A VIDEO PLAYS) THE CHANGED PART  |*|
|*| IS SENT AS JPEG INSTEAD, WITH THE JPEG CODER BUILT INTO WINDOWS (WIC).   |*|
|*| IT IS MANY TIMES SMALLER, SO THE PICTURE STAYS SMOOTH. A MOMENT AFTER    |*|
|*| THE SCREEN STOPS MOVING, THE SAME PART IS SENT AGAIN LOSSLESS, SO TEXT   |*|
|*| ENDS UP SHARP. EVERY THREAD THAT USES JPEG OPENS ITS OWN STRUCT S_JPEG.  |*|
|*| ENCODE_TILE() NEEDS AN OUTPUT OF AT LEAST W * H * 4 BYTES. IT RETURNS    |*|
|*| THE PACKED SIZE. DECODE_TILE() RETURNS 0 WHEN THE DATA IS BROKEN.        |*|
|*|__________________________________________________________________________|*|
|*'..........................................................................'*|
|* :                                                                        : *|
|* :   v [C LANGUAGE]                                                       : *|
|* :  1|                                                                    : *|
|* :  2| size_t ENCODE_TILE(const uint32_t *SOURCE, int STRIDE, int W,      : *|
|* :  3|                    int H,                                          : *|
|* :  4|                    uint8_t *OUT);                                  : *|
|* :  5| int    DECODE_TILE(const uint8_t *IN, size_t SIZE,                 : *|
|* :  6|                    uint32_t *TARGET,                               : *|
|* :  7|                    int STRIDE, int W, int H);                      : *|
|* :  8| int    JPEG_OPEN(struct S_JPEG *J);                                : *|
|* :  9| void   JPEG_CLOSE(struct S_JPEG *J);                               : *|
|* : 10| int    JPEG_ENCODE(struct S_JPEG *J, const uint32_t *SOURCE,       : *|
|* : 11|                    int STRIDE,                                     : *|
|* : 12|                    int W, int H, struct S_TEXT *OUT);              : *|
|* : 13| int    JPEG_DECODE(struct S_JPEG *J, const uint8_t *IN,            : *|
|* : 14|                    size_t SIZE,                                    : *|
|* : 15|                    uint32_t *TARGET, int STRIDE, int W, int H);    : *|
|* : 16|                                                                    : *|
|* :                                                                        : *|
|* '........................................................................' *|
\******************************************************************************/

#ifndef CODEC_H
#define CODEC_H

#define JPEG_QUALITY	0.72F	/* 0 = TINY AND UGLY, 1 = BIG AND PERFECT */

struct	S_JPEG
{
	void		*FACTORY;	/* IWICIMAGINGFACTORY, NULL = NO JPEG */
	uint8_t		*TEMP;		/* 24 BIT COPY OF THE PIXELS FOR THE CODER */
	size_t		TEMP_CAP;
	int		COM;		/* 1 WHEN THIS THREAD STARTED COM */
};

extern size_t	ENCODE_TILE(
	const uint32_t *SOURCE,
	int STRIDE,
	int W,
	int H,
	uint8_t *OUTPUT
);
extern int	DECODE_TILE(
	const uint8_t *BYTES,
	size_t SIZE,
	uint32_t *TARGET,
	int STRIDE,
	int W,
	int H
);

extern int	JPEG_OPEN(struct S_JPEG *J);
extern void	JPEG_CLOSE(struct S_JPEG *J);
extern int	JPEG_ENCODE(
	struct S_JPEG *J,
	const uint32_t *SOURCE,
	int STRIDE,
	int W,
	int H,
	struct S_TEXT *OUTPUT
);
extern int	JPEG_DECODE(
	struct S_JPEG *J,
	const uint8_t *BYTES,
	size_t SIZE,
	uint32_t *TARGET,
	int STRIDE,
	int W,
	int H
);

#endif /* !CODEC_H */
