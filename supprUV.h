#ifndef SUPPRUV_H
#define SUPPRUV_H

#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include "UTProfiler.h"

class SupprUV: public QWidget {
    Q_OBJECT
private:
    QComboBox* choixCat;
    QLabel* texte;
    QPushButton* bouton;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QVBoxLayout* couche;

public slots:
    void openChoix();

public:
    explicit SupprUV(QWidget* parent=0);
};

class FenetreChoix: public QDialog {
    Q_OBJECT
private:
    int placeUV;
    QComboBox* choix;
    QLabel* texte;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QVBoxLayout* couche;
    QPushButton* ok;

public slots:
    void supprimer();

public:
    explicit FenetreChoix(int nb, QDialog* parent=0);
};

#endif // SUPPRUV_H
