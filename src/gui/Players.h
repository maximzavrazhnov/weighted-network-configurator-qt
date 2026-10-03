#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_Players.h"
#include "system.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPen>
#include <QPushButton>
#include <QTableWidget>

#include <memory>

class Players : public QMainWindow
{
    Q_OBJECT

public:
    explicit Players(QWidget* parent = nullptr);
    ~Players() override = default;

public slots:
    void setText();
    void changeInput();

private slots:
    void chooseFile();
    void rebuildTable();
    void decreaseIndex();
    void increaseIndex();
    void copyItem(int row, int column);

private:
    Ui::PlayersClass ui;

    QLineEdit* fileEdit{nullptr};
    QLineEdit* numEdit{nullptr};
    QPushButton* fileButton{nullptr};
    QTableWidget* inputTable{nullptr};

    QGraphicsScene* scheme{nullptr};
    QGraphicsView* view{nullptr};

    QPen pen;
    QBrush brush;

    int numOfPlayers{4};
    int pageIndex{0};

    std::unique_ptr<System> system;

    void drawScheme(System* sys, int index);
    System::PlayerInput collectInput() const;
};
