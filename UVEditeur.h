#ifndef UVEDITEUR_H
#define UVEDITEUR_H

#include <QApplication>
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include "UTProfiler.h"

class UVEditeur : public QWidget
{
    Q_OBJECT
private:
    UV& uv;
    QVBoxLayout* couche;
    QHBoxLayout* coucheH1;
    QHBoxLayout* coucheH2;
    QHBoxLayout* coucheH3;
    QHBoxLayout* coucheH4;
    QLineEdit* code;
    QTextEdit* titre;
    QLabel *codeLabel, *titreLabel, *ouverture, *catLabel, *creditLabel;
    QCheckBox *automne, *printemps;
    QPushButton *annuler, *sauver;
    QSpinBox *cred;
    QComboBox *catego;

    public slots:
    void sauverUV();

private slots:
    void activerSauver(QString str="");

public:
    explicit UVEditeur(UV& uvToEdit, QWidget *parent=0);
};


#endif // UVEDITEUR_H
