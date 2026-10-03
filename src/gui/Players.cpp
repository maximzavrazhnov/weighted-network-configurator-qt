#include "Players.h"

#include <QFileDialog>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QHeaderView>
#include <QIntValidator>

#include <cmath>
#include <stdexcept>

namespace
{
constexpr double kPi = 3.14159265358979323846;
}

Players::Players(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    auto* validator = new QIntValidator(1, 99, this);

    fileEdit = new QLineEdit(this);
    fileEdit->setGeometry(30, 80, 300, 25);
    fileEdit->setEnabled(false);
    fileEdit->hide();

    numEdit = new QLineEdit(this);
    numEdit->setGeometry(30, 50, 30, 25);
    numEdit->setText("4");
    numEdit->setValidator(validator);

    fileButton = new QPushButton("Choose file", this);
    fileButton->setGeometry(370, 80, 80, 25);
    fileButton->hide();

    inputTable = new QTableWidget(this);
    inputTable->setGeometry(30, 80, 800, 200);
    inputTable->setColumnCount(5);
    inputTable->setRowCount(4);
    inputTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    inputTable->horizontalHeader()->hide();

    for (int i = 0; i < inputTable->columnCount(); ++i)
        inputTable->setColumnWidth(i, 50);

    for (int i = 0; i < 4; ++i)
    {
        inputTable->setItem(i, 0, new QTableWidgetItem("2"));

        for (int j = 1; j < 5; ++j)
        {
            auto* item = new QTableWidgetItem("0");
            inputTable->setItem(i, j, item);
            if (i + 1 >= j)
                item->setFlags(item->flags() &
                               ~Qt::ItemIsSelectable &
                               ~Qt::ItemIsEnabled);
        }
    }

    // Default symmetric weighted example from the original project.
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

    connect(ui.pushButton, &QPushButton::clicked, this, &Players::setText);
    connect(ui.comboBox, &QComboBox::currentIndexChanged, this, &Players::changeInput);
    connect(fileButton, &QPushButton::clicked, this, &Players::chooseFile);
    connect(numEdit, &QLineEdit::editingFinished, this, &Players::rebuildTable);
    connect(inputTable, &QTableWidget::cellChanged, this, &Players::copyItem);
}

System::PlayerInput Players::collectInput() const
{
    System::PlayerInput info;
    info.reserve(static_cast<std::size_t>(numOfPlayers));

    for (int i = 0; i < numOfPlayers; ++i)
    {
        const unsigned requiredLinks =
            inputTable->item(i, 0)->text().toUInt();

        std::vector<std::pair<unsigned, float>> possibleLinks;

        for (int j = 1; j < numOfPlayers + 1; ++j)
        {
            const float weight = inputTable->item(i, j)->text().toFloat();
            if (weight != 0.0F)
                possibleLinks.emplace_back(static_cast<unsigned>(j), weight);
        }

        info.emplace_back(requiredLinks, std::move(possibleLinks));
    }

    return info;
}

void Players::setText()
{
    if (ui.comboBox->currentIndex() == 1)
    {
        // File input existed as an unfinished branch in the original code.
        // The public package keeps the UI element for provenance but does not
        // pretend the parser is implemented.
        throw std::runtime_error(
            "File input is not implemented in the original master's project. "
            "Use manual input mode.");
    }

    system = std::make_unique<System>(collectInput());
    system->combinate();

    if (system->getConfigs().empty())
        return;

    pageIndex = 0;

    if (!scheme)
        scheme = new QGraphicsScene(this);

    if (!view)
    {
        view = new QGraphicsView(scheme);
        view->setWindowState(Qt::WindowMaximized);
    }

    drawScheme(system.get(), pageIndex);
}

void Players::changeInput()
{
    const bool manual = ui.comboBox->currentIndex() == 0;
    fileEdit->setVisible(!manual);
    fileButton->setVisible(!manual);
    inputTable->setVisible(manual);
}

void Players::chooseFile()
{
    fileEdit->setText(QFileDialog::getOpenFileName(this));
}

void Players::rebuildTable()
{
    if (numEdit->text().isEmpty())
        return;

    const int newSize = numEdit->text().toInt();
    if (newSize == numOfPlayers)
        return;

    inputTable->blockSignals(true);
    inputTable->setRowCount(newSize);
    inputTable->setColumnCount(newSize + 1);

    for (int i = 0; i < inputTable->columnCount(); ++i)
        inputTable->setColumnWidth(i, 50);

    for (int i = 0; i < newSize; ++i)
    {
        for (int j = 0; j < newSize + 1; ++j)
        {
            if (!inputTable->item(i, j))
                inputTable->setItem(i, j, new QTableWidgetItem("0"));

            auto* item = inputTable->item(i, j);
            if (j != 0 && i + 1 >= j)
                item->setFlags(item->flags() &
                               ~Qt::ItemIsSelectable &
                               ~Qt::ItemIsEnabled);
        }
    }

    numOfPlayers = newSize;
    inputTable->blockSignals(false);
}

void Players::drawScheme(System* sys, int index)
{
    const auto& configs = sys->getConfigs();
    if (configs.empty() || index < 0 || index >= static_cast<int>(configs.size()))
        return;

    scheme->clear();
    view->show();

    const auto& current = configs.at(static_cast<std::size_t>(index));
    const double sectorSize = 360.0 / static_cast<double>(current.players.size());

    for (std::size_t i = 0; i < current.players.size(); ++i)
    {
        const double angle = static_cast<double>(i) * sectorSize / 180.0 * kPi;
        const double x = 100.0 * std::sin(angle);
        const double y = 100.0 * std::cos(angle);

        auto* node = scheme->addEllipse(x, y, 30, 30, pen, brush);
        node->setScale(2.5);

        auto* label = new QGraphicsTextItem(
            QString::number(static_cast<int>(i + 1)), node);
        label->setPos(x + 8.0 + 7.0 * std::sin(angle),
                      y + 3.0 + 7.0 * std::cos(angle));

        for (const auto link : current.players.at(i).links)
        {
            if (link < i)
                continue;

            const double linkAngle =
                static_cast<double>(link) * sectorSize / 180.0 * kPi;
            const double x2 = 100.0 * std::sin(linkAngle);
            const double y2 = 100.0 * std::cos(linkAngle);

            auto* line = scheme->addLine(x + 15.0, y + 15.0,
                                         x2 + 15.0, y2 + 15.0, pen);
            line->setScale(2.5);
        }
    }

    auto* weightInfo = new QLineEdit;
    weightInfo->setEnabled(false);
    weightInfo->setText(
        QString("Weight = %1").arg(current.weight));
    weightInfo->setGeometry(QRect(-100, 500, 200, 30));
    scheme->addWidget(weightInfo);

    auto* buttonPrev = new QPushButton("Previous");
    buttonPrev->setGeometry(QRect(-500, 500, 80, 30));
    buttonPrev->setEnabled(index > 0);
    scheme->addWidget(buttonPrev);

    auto* buttonNext = new QPushButton("Next");
    buttonNext->setGeometry(QRect(500, 500, 80, 30));
    buttonNext->setEnabled(index + 1 < static_cast<int>(configs.size()));
    scheme->addWidget(buttonNext);

    connect(buttonPrev, &QPushButton::clicked, this, &Players::decreaseIndex);
    connect(buttonNext, &QPushButton::clicked, this, &Players::increaseIndex);
}

void Players::decreaseIndex()
{
    if (pageIndex > 0)
    {
        --pageIndex;
        drawScheme(system.get(), pageIndex);
    }
}

void Players::increaseIndex()
{
    if (system && pageIndex + 1 < static_cast<int>(system->getConfigs().size()))
    {
        ++pageIndex;
        drawScheme(system.get(), pageIndex);
    }
}

void Players::copyItem(int row, int column)
{
    if (column == 0 || row + 1 >= column)
        return;

    const int symmetricRow = column - 1;
    const int symmetricColumn = row + 1;

    if (!inputTable->item(symmetricRow, symmetricColumn))
        return;

    inputTable->blockSignals(true);
    inputTable->item(symmetricRow, symmetricColumn)
        ->setText(inputTable->item(row, column)->text());
    inputTable->blockSignals(false);
}
