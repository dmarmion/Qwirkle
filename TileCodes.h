
#ifndef ASSIGN1_TILECODES_H
#define ASSIGN1_TILECODES_H

// Colours
#define RED       'R'
#define ORANGE    'O'
#define YELLOW    'Y'
#define GREEN     'G'
#define BLUE      'B'
#define PURPLE    'P'
#define NO_COLOUR 'X'

#define NUM_COLOURS 6

// Shapes
#define CIRCLE    1
#define STAR_4    2
#define DIAMOND   3
#define SQUARE    4
#define STAR_6    5
#define CLOVER    6
#define NO_SHAPE  0

#define NUM_SHAPES 6

// Text Colour Codes
// See https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
#define RESET_TEXT  "\e[0m"
#define RED_TEXT    "\e[0;31m"
#define ORANGE_TEXT "\e[38;5;202m"
#define YELLOW_TEXT "\e[0;33m"
#define GREEN_TEXT  "\e[0;32m"
#define BLUE_TEXT   "\e[0;34m"
#define PURPLE_TEXT "\e[0;35m"

// Shape Emojis
// See https://www.unicode-search.net/unicode-namesearch.pl
#define CIRCLE_SYMBOL  "●"
#define STAR_4_SYMBOL  "✦"
#define DIAMOND_SYMBOL "◆"
#define SQUARE_SYMBOL  "■"
#define STAR_6_SYMBOL  "✶"
#define CLOVER_SYMBOL  "🟀"

#endif // ASSIGN1_TILECODES_H
