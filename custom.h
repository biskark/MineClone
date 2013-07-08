#ifndef _CUSTOM_H
#define _CUSTOM_H

#include <QWidget>
#include <QDialog>

class QGridLayout;
class QDialogButtonBox;
class QVBoxLayout;
class QSpinBox;
class QLabel;

class Custom : public QDialog {
    Q_OBJECT

    public:
        Custom(QWidget *parent=0);
        void setup_default(int rows, int cols, int mines);

    signals:
        void cancel_box();
        void ok_box(int, int, int);

    private slots:
        void confirmed();
        void rejected();
        void update_max_mines(int i);    

    private:
        void setup_grid();
        void setup_buttons();
        void setup_layout();
        QGridLayout *options;
        QVBoxLayout *layout;
        QDialogButtonBox *confirm;
        QSpinBox *num_rows;
        QSpinBox *num_cols;
        QSpinBox *num_mines;
};

#endif
