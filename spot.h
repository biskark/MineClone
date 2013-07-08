#ifndef _SPOT_H
#define _SPOT_H

#include <QPushButton>

#include "grid.h"

class Spot : public QPushButton {
    Q_OBJECT

    public:
        Spot(int row, int col, QWidget *parent=0);
        bool is_mine() const;
        void set_mine();
        void unset_mine();
        int row() const {return x;};
        int col() const {return y;};
        virtual QSize sizeHint() const;
        friend class Grid;

    signals:
        void flag_toggled(bool);
        void hit_mine() const;
        
        // Executed when "double clicked" on a revealed piece
        void reveal_nearby_flags(Spot *) const;

        // Executed when left clicked on an unrevealed piece
        void reveal_nearby_spots(Spot *) const;

        void pressed(QMouseEvent *e) const;
        void released(QMouseEvent *e) const;
        void left_clicked() const;
        void right_clicked() const;
        void double_clicked() const;

    public slots:
        void turn_off();
        void focus_gone();
        // Catch mouse events, then send off to Grid class for handling
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);

    private slots:
        void toggle_flag();
        void reveal();
        void check_surroundings();

    private:
        void show_nearby(int mines);
        void show_mine();
        // Helper method that calls others
        void update_property(const char *prop, bool status);
        void update_property(const char *prop, int status);

        int x, y;
        bool mine;
};

#endif
