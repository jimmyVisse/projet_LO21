/*
 * Fichier regroupant l'ensemble des classes utiles aux différentes classes et opérations de l'application
*/

#ifndef UT_PROFILER_h
#define UT_PROFILER_h

#include <QString>
#include <QTextStream>
#include <QMap>
#include <QVector>
#include <QFile>
#include <QTextCodec>
#include <QtXml>
using namespace std;

//Classe permettant de gérer les problèmes via des exceptions
class UTProfilerException{
public:
    UTProfilerException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

enum Categorie {
    /* Connaissances Scientifiques */ CS,  /* Techniques et Méthodes */ TM,
    /* Technologies et Sciences de l'Homme */ TSH, /* Stage et Projet */ SP,
    //first=CS, last=SP
};

QTextStream& operator<<(QTextStream& f, const Categorie& s);

Categorie StringToCategorie(const QString& s);
QString CategorieToString(Categorie c);
QTextStream& operator>>(QTextStream& f, Categorie& cat);

enum Note { A, B, C, D, E, F, FX, RES, ABS, /* en cours */ EC  };

//fonctions utilitaires pour transformer une note en QString
QString NoteToString(Note n);
Note StringToNote(const QString& s);

enum Saison { Automne, Printemps };
Saison StringToSaison(const QString& s);
inline QTextStream& operator<<(QTextStream& f, const Saison& s) { if (s==Automne) f<<"A"; else f<<"P"; return f;}

class Semestre {
    Saison saison;
    unsigned int annee;
public:
    Semestre(Saison s, unsigned int a):saison(s),annee(a){ if (annee<1972||annee>2099) throw UTProfilerException("annee non valide"); }
    Saison getSaison() const { return saison; }
    unsigned int getAnnee() const { return annee; }
    bool operator>(const Semestre& s);
    Semestre operator=(const Semestre& s);
    Semestre& operator++();
};

QString SemestreToString(const Semestre& sem);
Semestre StringToSemestre(const QString& s);

inline QTextStream& operator<<(QTextStream& f, const Semestre& s) { return f<<s.getSaison()<<s.getAnnee()%100; }

/* Classe abstraite Manager centralisant des opérations des gestionnaires d'UVs et de Cursus
Utilisation du design pattern Template Method pour le chargement et la sauvegarde */
class Manager {
protected:
    QString file;
    Manager(const QString& f="");
    virtual ~Manager();
public:
    virtual void load(const QString& f);
    virtual void lireFichier(QXmlStreamReader& xml) = 0;  //méthode abstraite redéfinie dans les classes filles
    virtual void save(const QString& f);
    virtual void ecrireFichier(QXmlStreamWriter* r) = 0;   //méthode abstraite redéfinie dans les classes filles
};

#endif
