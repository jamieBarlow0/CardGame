#ifndef CARD_H
#define CARD_H

#include <QString>

class Card
{
public:
    Card();
    Card(int value, const QString &suit, int number);

    int getValue() const;      //11 = j, 12 = q, 13 = k, 14 = a
    QString getSuit() const;   //clubs, diamonds, hearts, spades
    QString getName() const;   //jack of clubs, two of hearts
    int getNumber() const;     //encoded as svv

    void setValue(int v);
    void setSuit(const QString &s);
    void setNumber(int n);

private:
    int m_value;
    QString m_suit;
    int m_number;
};

#endif // CARD_H
