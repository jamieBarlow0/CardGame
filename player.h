#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "hand.h"

class Player
{
public:
    Player();
    Player(const QString &name);

    void resetScore();
    void incrementScore();

    QString getName() const;
    int getScore() const;

    Hand &hand();
    const Hand &hand() const;

private:
    QString m_name;
    int m_score;
    Hand m_hand;
};

#endif // PLAYER_H
