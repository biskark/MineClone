#include <QtGui> 
#include <iostream>

#include "mainwindow.h"
#include "grid.h"
#include "custom.h"

MainWindow::MainWindow() {
    title = "MineClone";
    main = new QWidget();
    layout = new QVBoxLayout();
    first = true;
    set_defaults();
    make_actions();
    make_menu_bar();
    make_status_bar();
    make_new_game_button();
    make_new_grid();
    load_times();
    main->setLayout(layout);
    setCentralWidget(main);
    connect(new_game_button, SIGNAL(clicked()),
            this, SLOT(new_game()));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setWindowTitle(title);
}

void MainWindow::make_menu_bar() {
    menu_bar = new QMenuBar();
    file_menu = menu_bar->addMenu(tr("&File"));
    file_menu->addAction(new_action);
    file_menu->addSeparator();
    file_menu->addAction(quit_action);

    options_menu = menu_bar->addMenu(tr("&Options"));
    options_menu->addAction(easy_action);
    options_menu->addAction(medium_action);
    options_menu->addAction(hard_action);
    options_menu->addAction(custom_action);
    options_menu->addSeparator();
    options_menu->addAction(stats_action);

    help_menu = menu_bar->addMenu(tr("&Help"));
    help_menu->addAction(about_action);
    setMenuBar(menu_bar);
}

void MainWindow::make_actions() {
    new_action = new QAction(tr("&New"), this);
    new_action->setShortcut(QKeySequence::New);
    new_action->setStatusTip(tr("Start New Game"));
    connect(new_action, SIGNAL(triggered()),
            this, SLOT(new_game()));
    quit_action = new QAction(tr("&Quit"), this);
    quit_action->setShortcut(QKeySequence::Quit);
    quit_action->setStatusTip(tr("Quit Game"));
    connect(quit_action, SIGNAL(triggered()),
            this, SLOT(quit_game()));

    easy_action   = new QAction(tr("&Easy   - 9x9,   10 Mines"), this);
    medium_action = new QAction(tr("&Medium - 16x16, 40 Mines"), this);
    hard_action   = new QAction(tr("&Hard   - 30x16, 99 Mines"), this);
    custom_action = new QAction(tr("&Custom"), this);
    connect(easy_action, SIGNAL(triggered()),
            this, SLOT(easy_update()));
    connect(medium_action, SIGNAL(triggered()),
            this, SLOT(medium_update()));
    connect(hard_action, SIGNAL(triggered()),
            this, SLOT(hard_update()));
    connect(custom_action, SIGNAL(triggered()),
            this, SLOT(change_difficulty()));

    stats_action = new QAction(tr("&Statistics"), this);
    stats_action->setStatusTip(tr("View Best Times"));
    connect(stats_action, SIGNAL(triggered()),
            this, SLOT(stats()));

    about_action = new QAction(tr("&About"), this);
    about_action->setStatusTip(tr("Shows Information on Mineclone"));
    connect(about_action, SIGNAL(triggered()),
            this, SLOT(about()));
}

void MainWindow::make_new_game_button() {
    new_game_button = new QPushButton("New");
    layout->addWidget(new_game_button, 0, Qt::AlignCenter);
}

void MainWindow::make_new_grid() {
    if (first) {
        first = false;
    }
    else {
        layout->removeWidget(grid);
        delete grid;
    }
    adjustSize();
    grid = new Grid(num_rows, num_cols, num_mines);
    connect(grid, SIGNAL(won()),
            this, SLOT(game_won()));
    connect(grid, SIGNAL(lost()),
            this, SLOT(game_lost()));
    connect(grid, SIGNAL(start()),
            this, SLOT(game_started()));
    connect(grid, SIGNAL(flag_update(int, int)),
            this, SLOT(flags_changed(int, int)));
    layout->addWidget(grid, 1, Qt::AlignCenter);
}

void MainWindow::make_status_bar() {
    status_bar = new QStatusBar();
    time = new QTime();
    timer = new QTimer();
    time_label = new QLabel();
    time_label->setText("00:00:00");
    found_label = new QLabel();
    found_label->setText("");
    status_bar->addWidget(time_label);
    status_bar->addWidget(found_label);
    status_bar->setSizeGripEnabled(false);
    setStatusBar(status_bar);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(update_timer()));
}

void MainWindow::update_timer() {
    int secs = time->elapsed() / 1000;
    int mins = (secs / 60) % 60;
    int hours = (secs / 3600);
    secs = secs % 60;
    time_label->setText(QString("%1:%2:%3")
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0')) );
}

void MainWindow::game_started() {
    time_label->setText("00:00:00");
    time->restart();
    timer->start(1000);
}

void MainWindow::game_lost() {
    game_over();
    QString done = time_label->text();
    done.push_back("   Lost!");
    time_label->setText(done);
}

void MainWindow::game_won() {
    game_over();
    QString done = time_label->text();
    done.push_back("   Won!");
    time_label->setText(done);
    check_times();
}

void MainWindow::game_over() {
    timer->stop();
    found_label->setText("");
}

void MainWindow::new_game() {
    game_over();
    make_new_grid();
}

void MainWindow::quit_game() {
    game_over();
    QCoreApplication::exit();
}

void MainWindow::easy_update() {
    num_rows = 9;
    num_cols = 9;
    num_mines = 10;
    difficulty = EASY;
    new_game();
}

void MainWindow::medium_update() {
    num_rows = 16;
    num_cols = 16;
    num_mines = 40;
    difficulty = MEDIUM;
    new_game();
}

void MainWindow::hard_update() {
    num_rows = 16;
    num_cols = 30;
    num_mines = 99;
    difficulty = HARD;
    new_game();
}

void MainWindow::change_difficulty() {
    custom = new Custom();
    custom->setWindowTitle(title);
    custom->setup_default(num_rows, num_cols, num_mines);
    connect(custom, SIGNAL(cancel_box()),
            this, SLOT(custom_difficulty_cancel()));
    connect(custom, SIGNAL(ok_box(int, int, int)),
            this, SLOT(custom_difficulty_ok(int, int, int)));
    custom->setModal(true);
    custom->show();
}

void MainWindow::about() {
    QMessageBox about;
    about.setWindowTitle(title);
    about.setText("This minesweeper clone was created entirely in C++ and Qt "
                  "4.8 by Kevin Biskar.");
    about.exec();
}

void MainWindow::stats() {
    QMessageBox stats_window;
    stats_window.setWindowTitle(title);
    QString text("Best Times\n");
    for (int i = 0; i < CUSTOM; ++i) {
        if (i == EASY) {
            text.append("Easy\t");
        }
        else if (i == MEDIUM) {
            text.append("Medium\t");
        }
        else if (i == HARD) {
            text.append("Hard\t");
        }
        text.append(best[i][TIME]);
        text.append("\t");
        text.append(best[i][NAME]);
        text.append("\n");
    }
    stats_window.setText(text);
    stats_window.exec();
}

void MainWindow::set_defaults() {
    num_rows = 9;
    num_cols = 9;
    num_mines = 10;
    difficulty = EASY;
}

void MainWindow::custom_difficulty_ok(int x, int y, int z) {
    num_rows = x;
    num_cols = y;
    num_mines = z;
    delete custom;
    difficulty = CUSTOM;
    new_game();
}

void MainWindow::custom_difficulty_cancel() {
    delete custom;
}

void MainWindow::flags_changed(int flags, int mines) {
    QString text;
    if (flags == 1) {
        text = QString("%1 mine found, %2 to go.").arg(
                        flags).arg(mines - flags);
    }
    else {
        text = QString("%1 mines found, %2 to go.").arg(
                        flags).arg(mines - flags);
    }
    found_label->setText(text);
}

void MainWindow::save_times() {
    QFile file(QDir::homePath() + QDir::separator() + ".mineclone.sav");
    if (! file.open(QIODevice::WriteOnly)) {
        std::cerr << "Cannot open file for saving times\n";
        return;
    }
    QTextStream out(&file);
    QList<int> fields;
    fields.append(EASY);
    fields.append(MEDIUM);
    fields.append(HARD);

    /* time_label->setText(QString("%1:%2:%3") */
    foreach (int diff, fields) {
        QString strdiff;
        switch (diff) {
            case EASY:   strdiff = "Easy";
                         break;
            case MEDIUM: strdiff = "Medium";
                         break;
            case HARD:   strdiff = "Hard";
                         break;
        }
        QString line = QString("%1\t%2\t%3\n")
                        .arg(strdiff)
                        .arg(best[diff][TIME])
                        .arg(best[diff][NAME]);
        out << line;
    }
}

void MainWindow::load_times() {
    QFile file(QDir::homePath() + QDir::separator() + ".mineclone.sav");
    if (! file.exists()) {
        file.setFileName(":best_times.txt");
    }
    if (! file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file 'best_times' for reading\n";
        return;
    }
    QTextStream in(&file);
    while (! in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split('\t');
        if (fields[0] == "Easy") {
            best[EASY][TIME] = fields[1];
            best[EASY][NAME] = fields[2];
        }
        else if (fields[0] == "Medium") {
            best[MEDIUM][TIME] = fields[1];
            best[MEDIUM][NAME] = fields[2];
        }
        else if (fields[0] == "Hard") {
            best[HARD][TIME] = fields[1];
            best[HARD][NAME] = fields[2];
        }
        else if (fields[0] == "Custom") {
        }
    }
}

void MainWindow::update_times(QString *t) {
    QString name = QInputDialog::getText(this, tr("Input Name"),
                        tr("You got the fastest time!\nEnter your name"));
    if (name.isEmpty()) {
        name = "Anonymous";
    }
    best[difficulty][TIME] = *t;
    best[difficulty][NAME] = name;
    save_times();
}

void MainWindow::check_times() {
    QString t = time_label->text().split(QRegExp("\\s+"))[0];
    if (difficulty == CUSTOM) {
        return;
    }
    if (compare_times(&t, &best[difficulty][TIME]) == -1) {
        update_times(&t);
    }
}

int MainWindow::compare_times(QString *current, QString *saved) {
    const int FIELDS = 3;
    QStringList cur = current->split(':');
    QStringList save = saved->split(':');
    for (int i = 0; i < FIELDS; ++i) {
        if (cur[i].toInt() < save[i].toInt()) {
            return -1;
        }
        if (cur[i].toInt() > save[i].toInt()) {
            return 1;
        }
    }
    return 0;
}
