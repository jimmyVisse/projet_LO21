/*
 * Déclaration des éléments de l'interface graphique pour actionner la complétion du dossier
*/

#ifndef COMPLETER_H
#define COMPLETER_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "uv.h"
#include "dossier.h"

class Completer: public QWidget {
    Q_OBJECT
private:
    QLabel* texte;
    QLabel* uv_label;
    QLabel* pref_label;
    QLineEdit* uv;
    QSpinBox* preference;
    QPushButton* ajouter;
    QPushButton* valider;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QHBoxLayout* coucheH3;
    QHBoxLayout* coucheH4;
    QVBoxLayout* couche;
public:
    explicit Completer(QWidget* parent=0);
public slots:
    void ajouterPreference();       // slot pour ajouter une préférence à une UV
    void validerChoix();            // slot pour actionner la complétion
};

#endif // COMPLETER_H
