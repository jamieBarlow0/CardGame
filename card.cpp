#include "card.h"

Card::Card()
    : m_value(0),
    m_suit(""),
    m_number(0)
{
    //empty card
}

Card::Card(int value, const QString &suit, int number)
    : m_value(value),
    m_suit(suit),
    m_number(number)
{
}

int Card::getValue() const
{
    return m_value;
}

QString Card::getSuit() const
{
    return m_suit;
}

QString Card::getName() const
{
    QString name;

    if (m_value == 11)
        name = "Jack";
    else if (m_value == 12)
        name = "Queen";
    else if (m_value == 13)
        name = "King";
    else if (m_value == 14)
        name = "Ace";
    else
        name = QString::number(m_value);

    return name + " of " + m_suit;
}

int Card::getNumber() const
{
    return m_number;
}

void Card::setValue(int v)
{
    m_value = v;
}

void Card::setSuit(const QString &s)
{
    m_suit = s;
}

void Card::setNumber(int n)
{
    m_number = n;
}
