#ifndef DECK_H
#define DECK_H

#include <QVector>
#include "card.h"

class Deck
{
public:
    Deck();

    void createDeck();          //generate 52 cards
    void shuffle();             //randomise
    Card dealCard();            //return top and remove it
    size_t cardsRemaining() const;
    void reset();               //reset and shuffle

private:
    QVector<Card> m_cards;
};

#endif // DECK_H
