#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_playerHasSwappedThisRound(false)
{
    ui->setupUi(this);

    ui->buttonNextRound->setEnabled(false);
    ui->buttonSwap->setEnabled(false);

    ui->labelResult->setText("click start to begin");
    ui->labelSwapInfo->setText("enter up to 3 card indices (1-5) separated by spaces, then press swap");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonStart_clicked()
{
    m_game.startGame();
    m_playerHasSwappedThisRound = false;

    bool ok = m_game.dealNextRound();
    if (!ok)
    {
        ui->labelResult->setText("not enough cards to start");
        return;
    }

    ui->buttonNextRound->setEnabled(true);
    ui->buttonSwap->setEnabled(true);

    updateUi();
}

void MainWindow::on_buttonNextRound_clicked()
{
    if (m_game.currentRound() >= 5 || !m_game.dealNextRound())
    {
        QString finalText;
        if (m_game.player().getScore() > m_game.computer().getScore())
            finalText = "game over: player wins overall";
        else if (m_game.player().getScore() < m_game.computer().getScore())
            finalText = "game over: computer wins overall";
        else
            finalText = "game over: overall draw";

        ui->labelResult->setText(finalText);
        ui->buttonNextRound->setEnabled(false);
        ui->buttonSwap->setEnabled(false);
    }
    else
    {
        m_playerHasSwappedThisRound = false;
        ui->buttonSwap->setEnabled(true);
        updateUi();
    }
}

void MainWindow::on_buttonSwap_clicked()
{
    if (m_game.currentRound() >= 5)
    {
        ui->labelResult->setText("no swapping allowed in round 5");
        return;
    }

    if (m_playerHasSwappedThisRound)
    {
        ui->labelResult->setText("you can only swap once per round");
        return;
    }

    QString text = ui->editSwapIndices->text().trimmed();
    if (text.isEmpty())
    {
        ui->labelResult->setText("no indices entered");
        return;
    }

    QStringList parts = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QVector<int> indices;
    for (const QString &p : parts)
    {
        bool ok = false;
        int val = p.toInt(&ok);
        if (ok && val >= 1 && val <= 5)
        {
            if (!indices.contains(val) && indices.size() < 3)
                indices.push_back(val);
        }
    }

    if (indices.isEmpty())
    {
        ui->labelResult->setText("no valid indices (1-5) found");
        return;
    }

    m_game.playerSwap(indices);
    m_playerHasSwappedThisRound = true;

    ui->labelResult->setText("cards swapped");
    updateUi();
}

QString MainWindow::handToString(const Hand &hand) const
{
    QStringList parts;
    for (const Card &c : hand.cards())
    {
        QString v;
        int val = c.getValue();
        if (val == 11) v = "J";
        else if (val == 12) v = "Q";
        else if (val == 13) v = "K";
        else if (val == 14) v = "A";
        else v = QString::number(val);

        QString suitSymbol;
        QString suit = c.getSuit();
        if (suit == "Hearts") suitSymbol = "♥";
        else if (suit == "Diamonds") suitSymbol = "♦";
        else if (suit == "Clubs") suitSymbol = "♣";
        else suitSymbol = "♠";

        parts << (v + suitSymbol);
    }
    return parts.join(" ");
}

QString MainWindow::categoryToText(const QString &code) const
{
    if (code == "ryfl") return "Royal Flush";
    if (code == "stfl") return "Straight Flush";
    if (code == "four") return "Four of a Kind";
    if (code == "full") return "Full House";
    if (code == "flsh") return "Flush";
    if (code == "strt") return "Straight";
    if (code == "trio") return "Three of a Kind";
    if (code == "twop") return "Two Pair";
    if (code == "pair") return "One Pair";
    return "High Card";
}

void MainWindow::updateUi()
{
    ui->labelPlayerHand->setText("player: " + handToString(m_game.player().hand()));
    ui->labelComputerHand->setText("computer: " + handToString(m_game.computer().hand()));

    ui->labelPlayerCategory->setText("player: " + categoryToText(m_game.player().hand().getBest()));
    ui->labelComputerCategory->setText("computer: " + categoryToText(m_game.computer().hand().getBest()));

    ui->labelScore->setText(
        QString("score - player: %1  computer: %2")
            .arg(m_game.player().getScore())
            .arg(m_game.computer().getScore())
        );

    ui->labelRound->setText(QString("round %1").arg(m_game.currentRound()));

    ui->labelResult->setText(m_game.lastRoundResult());

    if (m_game.currentRound() >= 5)
        ui->buttonNextRound->setText("finish game");
    else
        ui->buttonNextRound->setText("next round");
}
