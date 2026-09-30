#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "deck.h"

class Game
{
public:
    Game();

    void startGame();                 //resets deck, score and round
    bool dealNextRound();             //false if < 10
    Player const& winnerOfRound() const;
    Player const& overallWinner() const;
    int currentRound() const;
    void resetGame();

    Player &player();
    Player &computer();
    const Player &player() const;
    const Player &computer() const;

    QString lastRoundResult() const;
    bool isDrawRound() const;

    void playerSwap(const QVector<int> &indices); //player swap

private:
    Player m_player;
    Player m_computer;
    Deck m_deck;
    int m_round;
    int m_maxRounds;

    bool m_lastRoundDraw;
    QString m_lastRoundText;

    int compareHands(const Hand &h1, const Hand &h2) const;
    int rankFromCode(const QString &code) const;

    void computerSwapLogic();
};

#endif // GAME_H
