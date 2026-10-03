#include "Players.h"
#include "system.h"
#include <qfiledialog.h>
#include <QHeaderView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>



Players::Players(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QValidator* validator = new QIntValidator(1, 99, this);

    fileEdit = new QLineEdit(this);
    fileEdit->move(30, 80);
    fileEdit->resize(300, 25);
    fileEdit->setEnabled(false);
    fileEdit->hide();

    numEdit = new QLineEdit(this);
    numEdit->move(30, 50);
    numEdit->resize(30, 25);
    numEdit->setText("4");
    numEdit->setValidator(validator);
    numEdit->show();

    numOfPlayers = 4;

    fileButton = new QPushButton(this);
    fileButton->move(370, 80);
    fileButton->resize(80, 25);
    fileButton->setText("Choose file");
    fileButton->hide();

    inputTable = new QTableWidget(this);
    inputTable->move(30, 80);
    inputTable->resize(800, 200);
    inputTable->setColumnCount(5);
    inputTable->setRowCount(4);
    inputTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    inputTable->horizontalHeader()->hide();
    inputTable->show();
    for (int i = 0; i < inputTable->columnCount(); i++)
        inputTable->setColumnWidth(i, 50);
    
    for (int i = 0; i < 4; i++)
    {
        auto* item = new QTableWidgetItem();
        item->setText("2");
        inputTable->setItem(i, 0, item);
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 5; j++)
        {
            auto* item = new QTableWidgetItem();
            item->setText("0");
            inputTable->setItem(i, j, item);
            if (i + 1 >= j)
                item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);
        }
    }

    inputTable->item(0, 2)->setText("1");
    inputTable->item(0, 3)->setText("2");
    inputTable->item(0, 4)->setText("3");
    inputTable->item(1, 3)->setText("2");
    inputTable->item(1, 4)->setText("3");
    inputTable->item(2, 4)->setText("1");

    inputTable->item(1, 1)->setText("1");
    inputTable->item(2, 1)->setText("2");
    inputTable->item(3, 1)->setText("3");
    inputTable->item(2, 2)->setText("2");
    inputTable->item(3, 2)->setText("3");
    inputTable->item(3, 3)->setText("1");



 /*   auto* item0 = new QTableWidgetItem();
    item0->setText("2 3 4");
    inputTable->setItem(0, 1, item0);
    auto* item1 = new QTableWidgetItem();
    item1->setText("1 3 4");
    inputTable->setItem(1, 1, item1);
    auto* item2 = new QTableWidgetItem();
    item2->setText("1 2 4");
    inputTable->setItem(2, 1, item2);
    auto* item3 = new QTableWidgetItem();
    item3->setText("1 2 3");
    inputTable->setItem(3, 1, item3);*/



    pageIndex = 0;
    
    connect(ui.pushButton, &QPushButton::clicked, this, &Players::setText);
    connect(ui.comboBox, &QComboBox::currentIndexChanged, this, &Players::changeInput);
    connect(fileButton, &QPushButton::clicked, this, &Players::chooseFile);
    connect(numEdit, &QLineEdit::editingFinished, this, &Players::rebuildTable);
    connect(inputTable, &QTableWidget::cellChanged, this, &Players::copyItem);
}

void Players::setText()
{
    if (ui.comboBox->currentIndex() == 1)
    {
      /*  auto s = fileEdit->text();
        auto s2 = s.toUtf8();
        string ss(s2);
        system = new System(ss);*/
    }
    else
    {
        std::vector<std::pair<unsigned, std::vector<std::pair<unsigned, float>>>> info;

        for (int i = 0; i < numOfPlayers; i++)
        {
            unsigned numOfLinks = atoi(inputTable->item(i, 0)->text().toUtf8());
            std::vector<std::pair<unsigned, float>> second;

            for (int j = 1; j < numOfPlayers + 1; j++)
            {
                auto textToInt = atoi(inputTable->item(i, j)->text().toUtf8());

                if (textToInt != 0)
                {
                    second.emplace_back(std::make_pair(j, textToInt));
                }
            }

            info.emplace_back(std::make_pair(numOfLinks, second));
        }

        system = new System(info);
    }

    if (!system)
        throw std::runtime_error("Не инициализована system");

    system->combinate();
    /*auto textEdit = new QTextEdit();
    textEdit->setText(QString::fromUtf8(system->print().c_str()));
    textEdit->show();*/

    scheme = new QGraphicsScene();
    pen = new QPen();
    brush = new QBrush();
    view = new QGraphicsView(scheme);
    view->setWindowState(Qt::WindowMaximized);
    view->hide();
    drawScheme(system, pageIndex);
}

void Players::changeInput()
{
    if (ui.comboBox->currentIndex() == 0)
    {
        fileEdit->hide();
        fileButton->hide();
        inputTable->show();
    }
    else
    {
        fileEdit->show();
        fileButton->show();
        inputTable->hide();
    }
}

void Players::chooseFile()
{
    fileEdit->setText(QFileDialog::getOpenFileName(this));
}

void Players::rebuildTable()
{
    auto text = numEdit->text();
    if (text == "")
        return;
    auto newSize = text.toInt();
    if (newSize == numOfPlayers)
        return;
    
    inputTable->setRowCount(newSize);
    inputTable->setColumnCount(newSize + 1);
    for (int i = 0; i < inputTable->columnCount(); i++)
        inputTable->setColumnWidth(i, 50);
    for (int i = 0; i < numOfPlayers; i++)
    {
        for (int j = numOfPlayers + 1; j < newSize + 1; j++)
        {
            auto item = new QTableWidgetItem("0");
            inputTable->setItem(i, j, item);
            if (j != 0 && i + 1 >= j)
                item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);
        }
    }

    for (int i = numOfPlayers; i < newSize; i++)
    {
        for (int j = 0; j < newSize + 1; j++)
        {
            auto item = new QTableWidgetItem("0");
            inputTable->setItem(i, j, item);
            if (j != 0 && i + 1 >= j)
                item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);
        }
    }
    numOfPlayers = newSize;
}

void Players::drawScheme(System *sys, int index)
{
    auto items = scheme->items();
    for (int i = 0; i < items.size(); i++)
        scheme->removeItem(items[i]);
    view->show();


    auto configs = sys->getConfigs();
    auto first = configs.at(index);

    float sectorSize = 360.0 / first.players.size();

    for (int i = 0; i < first.players.size(); i++)
    {
        auto graphicsItem0 = new QGraphicsEllipseItem();
        auto angle = i * sectorSize / 180 * M_PI;
        int w = 100 * sin(angle);
        int h = 100 * cos(angle);

       // graphicsItem0->setPos(20 + i * 20, 20);
        graphicsItem0->setRect(w, h, 30, 30);
        graphicsItem0->setScale(2.5);
        graphicsItem0->setPen(*pen);
        graphicsItem0->setBrush(*brush);
        graphicsItem0->show();
        auto text = new QGraphicsTextItem(graphicsItem0);
        text->setPlainText(QString::number(i + 1));
        text->setPos(w + 8 + 7 * sin(angle), h + 3 + 7 * cos(angle));
        text->show();
        scheme->addItem(graphicsItem0);

        for (auto& link : first.players.at(i).links)
        {
            if (link < i)
                continue;

            auto line = new QGraphicsLineItem();
            int w2 = 100 * sin(link * sectorSize / 180 * M_PI);
            int h2 = 100 * cos(link * sectorSize / 180 * M_PI);

            auto x1 = w + 15;
            auto y1 = h + 15;
            auto x2 = w2 + 15;
            auto y2 = h2 + 15;

            if (w == w2)
            {

            }

            line->setLine(x1, y1, x2, y2);
            line->setPen(*pen);
            line->setScale(2.5);
            line->show();
            scheme->addItem(line);
        }

        string str = std::to_string(first.weight);
        QString weightText = "Weight = " + QString::fromUtf8(str.c_str());
        QLineEdit* weightInfo = new QLineEdit();
        weightInfo->setEnabled(false);
        weightInfo->setText(weightText);
        weightInfo->setGeometry(QRect(-100, 500, 200, 30));
        QGraphicsProxyWidget* proxyText = scheme->addWidget(weightInfo);

        QPushButton* buttonPrev = new QPushButton;
        buttonPrev->setGeometry(QRect(-500, 500, 80, 30));
        buttonPrev->setText("Previous");
        QGraphicsProxyWidget* proxy = scheme->addWidget(buttonPrev);

        QPushButton* buttonNext = new QPushButton;
        buttonNext->setGeometry(QRect(500, 500, 80, 30));
        buttonNext->setText("Next");
        QGraphicsProxyWidget* proxy2 = scheme->addWidget(buttonNext);

        if (index == 0)
            buttonPrev->setEnabled(false);
        if (index == configs.size() - 1)
            buttonNext->setEnabled(false);

        connect(buttonPrev, &QPushButton::clicked, this, &Players::decreaseIndex);
        connect(buttonNext, &QPushButton::clicked, this, &Players::increaseIndex);
    }
}

void Players::copyItem(int x, int y)
{
    if (y == 0)
        return;
    if (x > y)
        return;

    if (!inputTable->item(y - 1, x + 1))
        return;
    auto itemText = inputTable->item(x, y)->text();
    inputTable->item(y - 1, x + 1)->setText(itemText);
}

Players::~Players()
{}
