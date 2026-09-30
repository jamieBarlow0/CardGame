#ifndef HAND_H
#define HAND_H

#include <QVector>
#include <QString>
#include <QMap>
#include "card.h"
#include "deck.h"

class Hand
{
public:
    Hand();

    void dealHand(Deck &deck);
    void sortValue();
    void sortGroup();

    QString getBest() const; //ryfl, stfl, four, full, flsh, strt, trio, twop, pair, high

    void setHand(const QVector<int> &cardValues);
    void swapCard(const QVector<int> &cardIndices, Deck &deck);

    const QVector<Card> &cards() const;
    QVector<Card> &cards();

private:
    QVector<Card> m_cards;

    bool isFlush() const;
    bool isStraight(int &highValue, bool &isRoyal) const;
    void valueCounts(QMap<int,int> &counts) const;
};

#endif // HAND_H
