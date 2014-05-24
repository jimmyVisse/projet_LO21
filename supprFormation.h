#ifndef SUPPRFORMATION_H
#define SUPPRFORMATION_H

#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "UTProfiler.h"

class SupprimerFormation: public QWidget {
    Q_OBJECT
private:
    QLineEdit* choixCursus;
    QPushButton* valider;
    QLabel* texte;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QVBoxLayout* couche;

public:
    explicit SupprimerFormation(QWidget* parent=0);

public slots:
    void supprimer();
};


#endif // SUPPRFORMATION_H
