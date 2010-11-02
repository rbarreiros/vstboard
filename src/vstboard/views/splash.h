#ifndef SPLASH_H
#define SPLASH_H

#include <QDialog>

namespace Ui {
    class Splash;
}

class Splash : public QDialog
{
    Q_OBJECT

public:
    explicit Splash(QWidget *parent = 0);
    ~Splash();

private:
    Ui::Splash *ui;
};

#endif // SPLASH_H
