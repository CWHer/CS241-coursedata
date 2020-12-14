#include "gridselector.h"

GridSelector::GridSelector(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Select grid");
    setupLayouts();
    setupConnects();
}

void GridSelector::setupLayouts() {
    QFont font("consolas", 10);
    auto outer_layer = new QVBoxLayout();

    button_group.resize(GRID_NUM);
    for (auto &button : button_group)
        button = new QCheckBox();

    // grid selection begin
    auto select_part = new QVBoxLayout();
    for (int i = 0; i < GRID_PER; ++i) // row
    {
        auto button_line = new QHBoxLayout();
        for (int j = 0; j < GRID_PER; ++j) // column
            button_line->addWidget(button_group[i * GRID_PER + j]);
        select_part->addLayout(button_line);
    }
    outer_layer->addLayout(select_part);
    // grid selection end

    // apply button
    auto button_part = new QHBoxLayout();
    auto spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred);
    button_part->addItem(spacer);
    apply_button = new QPushButton();
    apply_button->setText("Apply");
    apply_button->setFont(font);
    button_part->addWidget(apply_button);
    outer_layer->addLayout(button_part);

    setLayout(outer_layer);
}
void GridSelector::setupConnects() {
    // apply -> accepted
    connect(apply_button, &QPushButton::clicked, this, &GridSelector::accept);
}

void GridSelector::selectGrid() {
    if (this->exec() == GridSelector::Accepted) {
        grid_id.clear();
        for (int i = 0; i < GRID_NUM; ++i)
            if (button_group[i]->checkState() == Qt::Checked) grid_id.push_back(i);
    }
}