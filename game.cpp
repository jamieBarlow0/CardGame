#include "game.h"
#include <QMap>
#include <algorithm>

Game::Game()
    : m_player("Player"),
    m_computer("Computer"),
    m_round(0),
    m_maxRounds(5),
    m_lastRoundDraw(false)
{
}

void Game::startGame()
{
    m_deck.reset();
    m_player.resetScore();
    m_computer.resetScore();
    m_round = 0;
    m_lastRoundDraw = false;
    m_lastRoundText.clear();
}

bool Game::dealNextRound()
{
    if (m_deck.cardsRemaining() < 10)
        return false;

    ++m_round;

    m_player.hand().dealHand(m_deck);
    m_computer.hand().dealHand(m_deck);

    if (m_round <= 4)
        computerSwapLogic();

    int cmp = compareHands(m_player.hand(), m_computer.hand());
    if (cmp > 0)
    {
        m_player.incrementScore();
        m_lastRoundDraw = false;
        m_lastRoundText = "player wins this round";
    }
    else if (cmp < 0)
    {
        m_computer.incrementScore();
        m_lastRoundDraw = false;
        m_lastRoundText = "computer wins this round";
    }
    else
    {
        m_lastRoundDraw = true;
        m_lastRoundText = "this round is a draw";
    }

    return true;
}

Player const& Game::winnerOfRound() const
{
    if (m_player.getScore() >= m_computer.getScore())
        return m_player;
    else
        return m_computer;
}

Player const& Game::overallWinner() const
{
    if (m_player.getScore() >= m_computer.getScore())
        return m_player;
    else
        return m_computer;
}

int Game::currentRound() const
{
    return m_round;
}

void Game::resetGame()
{
    startGame();
}

Player &Game::player()
{
    return m_player;
}

Player &Game::computer()
{
    return m_computer;
}

const Player &Game::player() const
{
    return m_player;
}

const Player &Game::computer() const
{
    return m_computer;
}

QString Game::lastRoundResult() const
{
    return m_lastRoundText;
}

bool Game::isDrawRound() const
{
    return m_lastRoundDraw;
}

int Game::rankFromCode(const QString &code) const
{
    if (code == "ryfl") return 1;
    if (code == "stfl") return 2;
    if (code == "four") return 3;
    if (code == "full") return 4;
    if (code == "flsh") return 5;
    if (code == "strt") return 6;
    if (code == "trio") return 7;
    if (code == "twop") return 8;
    if (code == "pair") return 9;
    return 10;
}

int Game::compareHands(const Hand &h1, const Hand &h2) const
{
    QString c1 = h1.getBest();
    QString c2 = h2.getBest();

    int r1 = rankFromCode(c1);
    int r2 = rankFromCode(c2);

    if (r1 < r2)
        return 1;
    if (r1 > r2)
        return -1;

    QVector<Card> v1 = h1.cards();
    QVector<Card> v2 = h2.cards();

    std::sort(v1.begin(), v1.end(), [](const Card &a, const Card &b) {
        return a.getValue() > b.getValue();
    });
    std::sort(v2.begin(), v2.end(), [](const Card &a, const Card &b) {
        return a.getValue() > b.getValue();
    });

    for (int i = 0; i < v1.size() && i < v2.size(); ++i)
    {
        if (v1[i].getValue() > v2[i].getValue())
            return 1;
        if (v1[i].getValue() < v2[i].getValue())
            return -1;
    }

    return 0;
}

void Game::playerSwap(const QVector<int> &indices)
{
    if (m_round >= 1 && m_round <= 4)
        m_player.hand().swapCard(indices, m_deck);
}

void Game::computerSwapLogic()
{
    QString best = m_computer.hand().getBest();

    if (best == "ryfl" || best == "stfl" || best == "four" ||
        best == "full" || best == "flsh" || best == "strt")
    {
        return;
    }

    QMap<int,int> counts;
    for (const Card &c : m_computer.hand().cards())
        counts[c.getValue()] += 1;

    QVector<int> keepValues;
    int maxCount = 0;
    for (auto it = counts.begin(); it != counts.end(); ++it)
    {
        if (it.value() > maxCount)
        {
            maxCount = it.value();
            keepValues.clear();
            keepValues.push_back(it.key());
        }
        else if (it.value() == maxCount)
        {
            keepValues.push_back(it.key());
        }
    }

    QVector<int> indicesToSwap;

    if (maxCount >= 2)
    {
        const QVector<Card> &cards = m_computer.hand().cards();
        for (int i = 0; i < cards.size(); ++i)
        {
            if (!keepValues.contains(cards[i].getValue()))
                indicesToSwap.push_back(i + 1);
        }
    }
    else
    {
        QVector<Card> cardsCopy = m_computer.hand().cards();
        QVector<int> originalIndices;
        for (int i = 0; i < cardsCopy.size(); ++i)
            originalIndices.push_back(i);

        std::sort(originalIndices.begin(), originalIndices.end(), [cardsCopy](int a, int b) {
            return cardsCopy[a].getValue() < cardsCopy[b].getValue();
        });

        for (int i = 0; i < 3 && i < originalIndices.size(); ++i)
            indicesToSwap.push_back(originalIndices[i] + 1);
    }

    m_computer.hand().swapCard(indicesToSwap, m_deck);
}
