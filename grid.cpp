#include <QtGui>
#include <QList>
#include <QWidget>
#include <QGridLayout>
#include <QSharedPointer>
#include <QEvent>

#include <stdlib.h>
#include <cstring>
#include <iostream>

#include "grid.h"
#include "spot.h"

Grid::Grid(int x, int y, int m, QWidget *parent) 
    : QWidget(parent), rows(x), cols(y) {
    // Creates space for the layout and background data matrix,
    // where each has a grid of pointers to Spot Buttons
    setMouseTracking(true);
    mines = m;
    flags_marked = revealed = 0;
    game_over = true;  // Toggled to false when the first Spot is clicked
    left = right = false;
    box = new QGridLayout;
    box->setSpacing(0);
    // Sets up rows and columns for grid and connects the buttons
    QList<Spot *> all;
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            Spot *button = new Spot(i, j);
            all << button;
            button->setFocusPolicy(Qt::NoFocus);
            connect(button, SIGNAL(hit_mine()),
                    this, SLOT(boom()));
            connect(this, SIGNAL(deactivate_board()),
                    button, SLOT(turn_off()));
            connect(this, SIGNAL(lost_focus()),
                    button, SLOT(focus_gone()));
            connect(button, SIGNAL(reveal_nearby_flags(Spot *)),
                    this, SLOT(show_nearby_flags(Spot *)));
            connect(button, SIGNAL(reveal_nearby_spots(Spot *)),
                    this, SLOT(mines_nearby(Spot *)));
            connect(button, SIGNAL(pressed(QMouseEvent *)),
                    this, SLOT(mouse_pressed(QMouseEvent *)));
            connect(button, SIGNAL(released(QMouseEvent *)),
                    this, SLOT(mouse_released(QMouseEvent *)));
            connect(button, SIGNAL(flag_toggled(bool)),
                    this, SLOT(update_flags(bool)));
            box->addWidget(button, i, j);
        }
    }
    setLayout(box);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void Grid::show_nearby_flags(Spot *spot) {
    int expected = spot->property("nearby").toInt();
    reveal(spot, "flag", expected);
}

void Grid::mines_nearby(Spot *spot) {
    // If new game (first click)
    if (game_over) {
        set_mines(spot);
        game_over = false;
        emit start();
    }
    spot_revealed();  // Adds to a count and checks if all spots are revealed
    reveal(spot, "mine", 0);
}

void Grid::reveal(Spot *spot, const char *type, int criteria) {
    int count = 0;
    int x = spot->row();
    int y = spot->col();
    QList<Spot *> nearby;
    for (int i = x - 1; i <= x + 1; ++i) {  // Go through rows
        // Skip OB rows
        if (i < 0 or i >= rows) {
            continue;
        }
        for (int j = y - 1; j <= y + 1; ++j) { // Go through columns in row
            // Skip OB columns
            if (j < 0 or j >= cols) {
                continue;
            }
            // Get ptr to spot at position i, j and store it
            Spot *temp = static_cast<Spot *>(
                    box->itemAtPosition(i, j)->widget());
            nearby << temp;
            if (std::strcmp(type, "mine") == 0 and temp and temp->is_mine()) {
                ++count;
            }
            // temp is a correctly flagged mine
            else if (std::strcmp(type, "flag") == 0 and temp and temp->property("flag").toBool()) {
                ++count;
            }
        }
    }
    if (std::strcmp(type, "mine") == 0) {
        spot->show_nearby(count);
    }
    if (count == criteria) {
        foreach (Spot *s, nearby) {
            s->left_clicked();
        }
    }
}

void Grid::set_mines(Spot *spot) {
    int max = rows * cols - 1;  // Number of total spaces - 1
    if (mines >= max) {
        mines = max;
    }
    int num_mines = mines;
    int x, y;
    while (num_mines > 0) {
        x = rand() % rows;
        y = rand() % cols;
        Spot *temp = dynamic_cast<Spot *>(box->itemAtPosition(x, y)->widget());
        if (! temp->is_mine() and temp != spot) {
            temp->set_mine();
            --num_mines;
        }
    }
}

void Grid::spot_revealed() {
    ++revealed;
    if (revealed == open_spots()) {
        game_over = true;
        emit deactivate_board();
        emit won();
    }
}

int Grid::open_spots() {
    return rows * cols - mines;
}

void Grid::boom() {
    game_over = true;
    emit deactivate_board();
    emit lost();
}

void Grid::update_focus() {
    emit lost_focus();
    focus = dynamic_cast<Spot *>(
            QApplication::widgetAt(QCursor::pos()));
    if (focus and doubled) {
        adjacent = QList<Spot *>();
        int x = focus->row();
        int y = focus->col();
        for (int i = x - 1; i <= x + 1; ++i) {
            if (i < 0 or i >= rows) {
                continue;
            }
            for (int j = y - 1; j <= y + 1; ++j) {
                if (j < 0 or j >= cols or
                        (i == x and j == y)) {
                    continue;
                }
                Spot *temp = dynamic_cast<Spot *>(
                        box->itemAtPosition(i, j)->widget());
                adjacent << temp;
            }
        }
        foreach (Spot *ad, adjacent) {
            if (not ad->property("revealed").toBool() and
                    not ad->property("flag").toBool()) {
                ad->update_property("pressed", true);
            }
        }
        if (not focus->property("revealed").toBool() and
                not focus->property("flag").toBool()) {
            focus->update_property("pressed", true);
        }
    }
}

void Grid::left_click_spot() {
    update_focus();
    if (focus) {
        focus->left_clicked();
    }
}

void Grid::right_click_spot() {
    update_focus();
    if (focus) {
        focus->right_clicked();
    }
}

void Grid::double_click_spot() {
    update_focus();
    if (focus) {
        focus->double_clicked();
    }
}

void Grid::mouse_pressed(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        left = true;
        // rest of this if statement is for styling purposes only
        update_focus();
        if (focus and not doubled 
                  and not focus->property("revealed").toBool()
                  and not focus->property("flag").toBool()) {
            focus->update_property("pressed", true);
        }
    }
    else if (e->button() == Qt::RightButton) {
        right = true;
    }
    if (left and right) {
        doubled = true;
        update_focus();
    }
    // Immediately send a right click signal if left isn't pressed
    else if (right) {
        right_click_spot();
    }
}

void Grid::mouse_released(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        left = false;
        if (not doubled) {
            left_click_spot(); }
    }
    else if (e->button() == Qt::RightButton) {
        right = false;
    }
    // Double click activates once both buttons are released
    if (doubled and not left and not right) {
        doubled = false;
        double_click_spot();
    }
}

void Grid::mouseMoveEvent(QMouseEvent *e) {
    update_focus();
    if (focus and left and not doubled) {
        if (not focus->property("revealed").toBool()
                and not focus->property("flag").toBool()) {
            focus->update_property("pressed", true);
        }
    }
}

void Grid::update_flags(bool status) {
    if (status) {
        ++flags_marked;
    }
    else {
        --flags_marked;
    }
    emit flag_update(flags_marked, mines);
}
