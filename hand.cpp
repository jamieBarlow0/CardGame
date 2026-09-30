#include "hand.h"
#include <algorithm>

Hand::Hand()
{
}

const QVector<Card> &Hand::cards() const
{
    return m_cards;
}

QVector<Card> &Hand::cards()
{
    return m_cards;
}

void Hand::dealHand(Deck &deck)
{
    m_cards.clear();
    for (int i = 0; i < 5; ++i)
        m_cards.push_back(deck.dealCard());
}

void Hand::sortValue()
{
    std::sort(m_cards.begin(), m_cards.end(), [](const Card &a, const Card &b) {
        return a.getValue() < b.getValue();
    });
}

void Hand::sortGroup()
{
    QMap<int,int> counts;
    for (const Card &c : m_cards)
        counts[c.getValue()] += 1;

    std::sort(m_cards.begin(), m_cards.end(), [counts](const Card &a, const Card &b) {
        int ca = counts[a.getValue()];
        int cb = counts[b.getValue()];
        if (ca != cb)
            return ca > cb; // more frequent first
        return a.getValue() < b.getValue();
    });
}

bool Hand::isFlush() const
{
    if (m_cards.isEmpty())
        return false;

    QString suit = m_cards[0].getSuit();
    for (int i = 1; i < m_cards.size(); ++i)
    {
        if (m_cards[i].getSuit() != suit)
            return false;
    }
    return true;
}

bool Hand::isStraight(int &highValue, bool &isRoyal) const
{
    if (m_cards.size() != 5)
        return false;

    QVector<int> vals;
    for (const Card &c : m_cards)
        vals.push_back(c.getValue());

    std::sort(vals.begin(), vals.end());

    // low ace straight (a,2,3,4,5)
    if (vals[0] == 2 && vals[1] == 3 && vals[2] == 4 && vals[3] == 5 && vals[4] == 14)
    {
        highValue = 5;
        isRoyal = false;
        return true;
    }

    for (int i = 0; i < 4; ++i)
    {
        if (vals[i+1] != vals[i] + 1)
            return false;
    }

    highValue = vals[4];
    isRoyal = (vals[0] == 10 && vals[1] == 11 && vals[2] == 12 && vals[3] == 13 && vals[4] == 14);
    return true;
}

void Hand::valueCounts(QMap<int,int> &counts) const
{
    counts.clear();
    for (const Card &c : m_cards)
        counts[c.getValue()] += 1;
}

QString Hand::getBest() const
{
    bool flush = isFlush();
    int straightHigh = 0;
    bool isRoyal = false;
    bool straight = isStraight(straightHigh, isRoyal);

    QMap<int,int> counts;
    valueCounts(counts);

    int maxCount = 0;
    int secondCount = 0;
    for (auto it = counts.begin(); it != counts.end(); ++it)
    {
        int c = it.value();
        if (c > maxCount)
        {
            secondCount = maxCount;
            maxCount = c;
        }
        else if (c > secondCount)
        {
            secondCount = c;
        }
    }

    if (flush && straight && isRoyal)
        return "ryfl";
    if (flush && straight)
        return "stfl";
    if (maxCount == 4)
        return "four";
    if (maxCount == 3 && secondCount == 2)
        return "full";
    if (flush)
        return "flsh";
    if (straight)
        return "strt";
    if (maxCount == 3)
        return "trio";
    if (maxCount == 2 && secondCount == 2)
        return "twop";
    if (maxCount == 2)
        return "pair";
    return "high";
}

void Hand::setHand(const QVector<int> &cardValues)
{
    m_cards.clear();
    if (cardValues.size() != 5)
        return;

    for (int n : cardValues)
    {
        int suitCode = n / 100;
        int vv = n % 100;

        QString suit;
        if (suitCode == 1) suit = "Clubs";
        else if (suitCode == 2) suit = "Diamonds";
        else if (suitCode == 3) suit = "Hearts";
        else suit = "Spades";

        int value;
        if (vv == 1)
            value = 14; //ace 14 by default
        else
            value = vv;

        Card c(value, suit, n);
        m_cards.push_back(c);
    }
}

void Hand::swapCard(const QVector<int> &cardIndices, Deck &deck)
{
    if (cardIndices.isEmpty())
        return;

    QVector<int> unique;
    for (int idx : cardIndices)
    {
        if (!unique.contains(idx) && unique.size() < 3)
            unique.push_back(idx);
    }

    for (int idx : unique)
    {
        int realIndex = idx - 1; //1-based to 0-based
        if (realIndex >= 0 && realIndex < m_cards.size())
            m_cards[realIndex] = deck.dealCard();
    }
}
