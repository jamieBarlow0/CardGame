#include "player.h"

Player::Player()
    : m_name("Player"),
    m_score(0)
{
}

Player::Player(const QString &name)
    : m_name(name),
    m_score(0)
{
}

void Player::resetScore()
{
    m_score = 0;
}

void Player::incrementScore()
{
    ++m_score;
}

QString Player::getName() const
{
    return m_name;
}

int Player::getScore() const
{
    return m_score;
}

Hand &Player::hand()
{
    return m_hand;
}

const Hand &Player::hand() const
{
    return m_hand;
}
