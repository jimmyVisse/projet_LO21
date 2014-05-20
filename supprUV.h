#ifndef SUPPRUV_H
#define SUPPRUV_H

#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "UTProfiler.h"

class SupprUV: public QWidget {
    Q_OBJECT
private:
    QLabel* texte;
    QLineEdit* chUV;
    QPushButton* bouton;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QVBoxLayout* couche;

public slots:
    void openChoix();

public:
    explicit SupprUV(QWidget* parent=0);
};

#endif // SUPPRUV_H
