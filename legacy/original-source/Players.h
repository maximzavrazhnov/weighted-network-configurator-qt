#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Players.h"
#include <QString>
#include <QLineEdit>
#include <QTableWidget>
#include <QTextEdit>
#include <system.h>
#include <QGraphicsScene>

class Players : public QMainWindow
{
    Q_OBJECT

public:
    Players(QWidget *parent = nullptr);
    ~Players();
public slots:
    void setText();
    void changeInput();

private:
    Ui::PlayersClass ui;
    QString resText;
    QLineEdit* fileEdit, *numEdit;
    QPushButton* fileButton;
    QTableWidget* inputTable;
    QGraphicsScene* scheme;
    QGraphicsView* view;
    QPen* pen;
    QBrush* brush;
    int numOfPlayers;
    int pageIndex;
    void drawScheme(System *sys, int index);
    System* system;

private slots:
    void chooseFile();
    void rebuildTable();
    void decreaseIndex() { pageIndex--; drawScheme(system, pageIndex); }
    void increaseIndex() { pageIndex++; drawScheme(system, pageIndex); }
    void copyItem(int x, int y);
};
