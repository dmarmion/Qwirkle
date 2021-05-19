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
    std::string stringRepr = colour + std::to_string(shape);
    return stringRepr;
}

std::string Tile::toColourString() {
    std::string stringRepr = colour + std::to_string(shape);
    return getColourEscape() + stringRepr + RESET_TEXT;
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

    if (getColour() == RED) {
        returnVal = RED_TEXT;
    } else if (getColour() == ORANGE) {
        returnVal = ORANGE_TEXT;
    } else if (getColour() == YELLOW) {
        returnVal = YELLOW_TEXT;
    } else if (getColour() == GREEN) {
        returnVal = GREEN_TEXT;
    } else if (getColour() == BLUE) {
        returnVal = BLUE_TEXT;
    } else if (getColour() == PURPLE) {
        returnVal = PURPLE_TEXT;
    }

    return returnVal;
}