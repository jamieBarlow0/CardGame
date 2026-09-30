#include "deck.h"
#include <QRandomGenerator>

Deck::Deck()
{
    createDeck();
    shuffle();
}

void Deck::createDeck()
{
    m_cards.clear();

    QString suits[4] = { "Clubs", "Diamonds", "Hearts", "Spades" };

    for (int s = 0; s < 4; ++s)
    {
        for (int v = 2; v <= 14; ++v)
        {
            int vv = v;
            if (v == 14)
                vv = 1;

            int number = (s + 1) * 100 + vv; //svv
            Card c(v, suits[s], number);
            m_cards.push_back(c);
        }
    }
}

void Deck::shuffle()
{
    for (int i = m_cards.size() - 1; i > 0; --i)
    {
        int j = QRandomGenerator::global()->bounded(i + 1);
        m_cards.swapItemsAt(i, j);
    }
}

Card Deck::dealCard()
{
    if (m_cards.isEmpty())
        return Card();

    Card c = m_cards.back();
    m_cards.pop_back();
    return c;
}

size_t Deck::cardsRemaining() const
{
    return static_cast<size_t>(m_cards.size());
}

void Deck::reset()
{
    createDeck();
    shuffle();
}
