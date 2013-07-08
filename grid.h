#ifndef _GRID_H
#define _GRID_H

#include <QWidget>
#include <QList>

class QGridLayout;
class Spot;
class vector;

class Grid : public QWidget {
    Q_OBJECT

    public:
        Grid(int x, int y, int mines, QWidget *parent = 0);

        // Randomnly chooses positions on the grid to make a mine.
        // Should be called after the first position is chosen so the it
        // will not make the clicked position a mine (and thus lose the game).
        void set_mines(Spot *spot = 0);
        int open_spots();

    signals:
        void lost_focus();
        void won();
        void lost();
        void start();
        void deactivate_board();
        void flag_update(int, int);

    public slots:
        void mouseMoveEvent(QMouseEvent *e);
        void mouse_pressed(QMouseEvent *e);
        void mouse_released(QMouseEvent *e);
        // Method for implementating a double click.
        // If the player has identified the appropriate number of flags,
        // the method clicks all the nearby pieces as a convenience.
        // Note that if the flags are mislabeled, can lose the game.
        // Also note that it still 'clicks' the flagged spots, so it assumes
        // that those pieces will resist these clicks.
        void show_nearby_flags(Spot *spot);
        void update_flags(bool);        
        void boom();

        // Function that returns the number of nearby spaces that have mines.
        // Additionally creates a recursive call for each one with no nearby 
        // mines. Occurs simulataneously with a Spot reveal
        void mines_nearby(Spot *spot);


    private:
        void left_click_spot();
        void right_click_spot();
        void double_click_spot();

        // Method when triggered increases count. Then checks if all possible
        // spots are revealed. If so, game won!
        void spot_revealed();

        // Gets the nearby pieces of the given Spot that are the given type.
        // Either 'flag' or 'mine'. Then clicks all those pieces if the criteria
        // matches the count.
        void reveal(Spot *spot, const char *type, int criteria);

        // Gets widget under cursor and stores it in focus
        void update_focus();
        void update_adjacent();

        QGridLayout *box;
        bool game_over;
        int rows;
        int cols;
        int mines;
        int revealed;
        int flags_marked;

        // Placeholders for communicating between spots, remembers if
        // mouse has been clicked
        bool left;
        bool right;
        bool doubled;

        // Used for 'pressing' down other spots when double clicked
        Spot *focus;
        QList<Spot *> adjacent;
};

#endif
