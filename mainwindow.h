#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonStart_clicked();
    void on_buttonNextRound_clicked();
    void on_buttonSwap_clicked();

private:
    Ui::MainWindow *ui;
    Game m_game;
    bool m_playerHasSwappedThisRound;

    void updateUi();
    QString handToString(const Hand &hand) const;
    QString categoryToText(const QString &code) const;
};

#endif // MAINWINDOW_H
