#include <QtGui>
#include "custom.h"

Custom::Custom(QWidget *parent) : QDialog(parent) {
    setup_grid();
    setup_buttons();
    setup_layout();
}

void Custom::setup_grid() {
    options = new QGridLayout();
    QLabel *one = new QLabel;
    QLabel *two = new QLabel;
    QLabel *three = new QLabel;
    one->setText("Rows");
    two->setText("Columns");
    three->setText("Mines");

    num_rows = new QSpinBox();
    num_cols = new QSpinBox();
    num_mines = new QSpinBox();
    num_rows->setMinimum(3);
    num_cols->setMinimum(3);
    num_mines->setMinimum(1);
    
    connect(num_rows, SIGNAL(valueChanged(int)),
            this, SLOT(update_max_mines(int)));
    connect(num_cols, SIGNAL(valueChanged(int)),
            this, SLOT(update_max_mines(int)));
    options->addWidget(one, 0, 0, Qt::AlignHCenter);
    options->addWidget(two, 0, 1, Qt::AlignHCenter);
    options->addWidget(three, 0, 2, Qt::AlignHCenter);

    options->addWidget(num_rows, 1, 0);
    options->addWidget(num_cols, 1, 1);
    options->addWidget(num_mines, 1, 2);
}

void Custom::setup_buttons() {
    confirm = new QDialogButtonBox(QDialogButtonBox::Ok |
                                   QDialogButtonBox::Cancel);
    connect(confirm, SIGNAL(accepted()),
            this, SLOT(confirmed()));
    connect(confirm, SIGNAL(rejected()),
            this, SLOT(rejected()));
}

void Custom::setup_layout() {
    layout = new QVBoxLayout();
    layout->addLayout(options);
    layout->addWidget(confirm);
    setLayout(layout);
}

void Custom::setup_default(int rows, int cols, int mines) {
    num_rows->setValue(rows);
    num_cols->setValue(cols);
    num_mines->setValue(mines);
    num_mines->setMaximum(num_rows->value() * num_cols->value() - 1);
}

void Custom::confirmed() {
    emit ok_box(num_rows->value(), num_cols->value(), num_mines->value());
}

void Custom::rejected() {
    emit cancel_box();
}

void Custom::update_max_mines(int i) {
    num_mines->setMaximum(num_rows->value() * num_cols->value() - 1);
}
