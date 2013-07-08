#include <QtGui>

#include <iostream>
#include "spot.h"

Spot::Spot(int row, int col, QWidget *parent)
                : QPushButton(parent), x(row), y(col) {
    QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setSizePolicy(policy);

    unset_mine();
    connect(this, SIGNAL(left_clicked()),
            this, SLOT(reveal()));
    connect(this, SIGNAL(right_clicked()),
            this, SLOT(toggle_flag()));
    connect(this, SIGNAL(double_clicked()),
            this, SLOT(check_surroundings()));
    adjustSize();
}

// Essentially an over-ridden clicked method to emit coords as well
void Spot::reveal() {
    if (not property("revealed").toBool() and
            not property("flag").toBool()) {
        update_property("revealed", true);
        if (is_mine()) {
            show_mine();
            emit hit_mine();
        }
        else {
            emit reveal_nearby_spots(this);
        }
    }
}

void Spot::show_nearby(int mines) {
    if (mines > 0) {
        setText(QString::number(mines));
        update_property("nearby", mines);
    }
}

/* // When called, acts as if it has been clicked, used for revealing blank */
/* // spaces */

void Spot::show_mine() {
    setText("M");
    update_property("boom", true);
}

bool Spot::is_mine() const {
    return mine;
}

void Spot::set_mine() {
    mine = true;
}

void Spot::unset_mine() {
    mine = false;
}

void Spot::toggle_flag() {
    if (not property("revealed").toBool()) {
        if (property("flag").toBool()) {
            update_property("flag", false);
            flag_toggled(false);
        }
        else {
            update_property("flag", true);
            flag_toggled(true);
        }
    }
}

void Spot::focus_gone() {
    update_property("pressed", false);
}

void Spot::check_surroundings() {
    if (property("revealed").toBool()) {
        emit reveal_nearby_flags(this);
    }
    else {
        /* emit clicked(); */
    }
}

void Spot::turn_off() {
    setMouseTracking(false);
    setDisabled(this);
    update_property("over", true);
}

void Spot::update_property(const char *prop, bool status) {
    setProperty(prop, status);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void Spot::update_property(const char *prop, int status) {
    setProperty(prop, status);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void Spot::mousePressEvent(QMouseEvent *e) {
    emit pressed(e);
}

void Spot::mouseReleaseEvent(QMouseEvent *e) {
    emit released(e);
}

QSize Spot::sizeHint() const {
    return QSize(25, 25);
}
