#include <cctype>

#include "Tile.h"
#include "TileCodes.h"

Tile::Tile(Colour colour, Shape shape) :
    colour(std::toupper(colour)),
    shape(shape)
{}

// This is a stupid bodge
Tile::Tile(std::string tileSTR)
{
    colour = tileSTR.at(0);
    shape = std::stoi(tileSTR.substr(1,1));
}

Tile::Tile(Tile& other) :
    colour(other.colour),
    shape(other.shape)
{}

Tile::Tile(Tile&& other) :
    colour(other.colour),
    shape(other.shape)
{}

Tile::~Tile() {
    // No manual deletes necessary
}

Colour Tile::getColour() {
    return colour;
}

void Tile::setColour(Colour newColour) {
    colour = std::toupper(newColour);
}

Shape Tile::getShape() {
    return shape;
}

void Tile::setShape(Shape newShape)  {
    shape = newShape;
}

std::string Tile::toString() {
    return toString(false, false);
}

std::string Tile::toString(bool useColour, bool useSymbols) {
    std::string stringRepr = "";
    stringRepr += colour;

    if (useSymbols) {
        stringRepr += getSymbol();
    } else {
        stringRepr += std::to_string(shape);
    }

    if (useColour) {
        stringRepr = getColourEscape() + stringRepr + RESET_TEXT;
    }

    return stringRepr;
}

bool Tile::equals(Tile t) {
    return t.getColour() == colour && t.getShape() == shape;
}

bool Tile::hasBlankValue() {
    return colour == NO_COLOUR || shape == NO_SHAPE;
}

std::string Tile::getColourEscape() {
    // By default, return the default colour
    std::string returnVal = RESET_TEXT;

    if (colour == RED) {
        returnVal = RED_TEXT;
    } else if (colour == ORANGE) {
        returnVal = ORANGE_TEXT;
    } else if (colour == YELLOW) {
        returnVal = YELLOW_TEXT;
    } else if (colour == GREEN) {
        returnVal = GREEN_TEXT;
    } else if (colour == BLUE) {
        returnVal = BLUE_TEXT;
    } else if (colour == PURPLE) {
        returnVal = PURPLE_TEXT;
    }

    return returnVal;
}

std::string Tile::getSymbol() {
    // If not using a #defined colour, use "?"
    std::string returnVal = "?";

    if (shape == CIRCLE) {
        returnVal = CIRCLE_SYMBOL;
    } else if (shape == STAR_4) {
        returnVal = STAR_4_SYMBOL;
    } else if (shape == DIAMOND) {
        returnVal = DIAMOND_SYMBOL;
    } else if (shape == SQUARE) {
        returnVal = SQUARE_SYMBOL;
    } else if (shape == STAR_6) {
        returnVal = STAR_6_SYMBOL;
    } else if (shape == CLOVER) {
        returnVal = CLOVER_SYMBOL;
    }

    return returnVal;
}