#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <QMainWindow>

class QTimer;
class QTime;
class QLabel;
class QVBoxLayout;
class QPushButton;
class QStatusBar;
class QWidget;
class QMenuBar;
class QMenu;
class QAction;
class Custom;
class Grid;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();

    public slots:
        void custom_difficulty_ok(int, int, int);
        void custom_difficulty_cancel();
        void flags_changed(int, int);

    private slots:
        void new_game();
        void quit_game();
        void update_timer();
        void game_started();
        void game_lost();
        void game_won();
        void about();
        void stats();
        void change_difficulty();
        void easy_update();
        void medium_update();
        void hard_update();

    private:
        // Private function for handling stuff for every game_over
        void game_over();
        void set_defaults();
        void make_new_grid();
        void make_status_bar();
        void make_menu_bar();
        void make_new_game_button();
        void make_actions();
        void save_times();
        void load_times();
        void update_times(QString *t);
        void check_times();
        int compare_times(QString *current, QString *saved);

        QString title;
        enum {EASY, MEDIUM, HARD, CUSTOM};
        enum {TIME, NAME};
        int difficulty;  // Current Difficulty
        QString best[4][2];
        QWidget *main;
        QMenuBar *menu_bar;
        QMenu *file_menu;
        QMenu *options_menu;
        QMenu *help_menu;
        QVBoxLayout *layout;
        QPushButton *new_game_button;
        QStatusBar *status_bar;
        Grid *grid;
        QTimer *timer;
        QTime *time;
        QLabel *time_label;
        QLabel *found_label;
        QAction *new_action;
        QAction *quit_action;
        QAction *stats_action;
        QAction *about_action;
        QAction *easy_action;
        QAction *medium_action;
        QAction *hard_action;
        QAction *custom_action;
        Custom *custom;
        int num_rows;
        int num_cols;
        int num_mines;
        bool first;
};

#endif
