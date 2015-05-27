/*
 * Fichier contenant la classe de gestion d'un dossier
*/

#ifndef DOSSIER_H
#define DOSSIER_H

#include "UTProfiler.h"
#include "formation.h"
#include "inscription.h"
#include "strategie.h"
#include "solution.h"
#include "uv.h"

//Classe gérant le dossier d'un étudiant
class Dossier {
private:
    QString nomEtu;
    QString prenomEtu;
    QVector<Inscription*> inscriptions;
    Formation cursus;
    unsigned int nb_CreditsCS;
    unsigned int nb_CreditsTM;
    unsigned int nbSemestre;
    QString file;
    friend struct Handler;
    //Singleton
    struct Handler{
        Dossier* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;
    virtual ~Dossier();
    Dossier(const QString &n, const QString& p);
    Strategie* strat;
    QVector<Solution*> sol;

public:
    static Dossier& getInstance();
    static void libererInstance();
    void chargerDossier(const QString& f);
    void sauverDossier(const QString& f);
    const QString getNom() const { return nomEtu; }
    const QString getPrenom() const { return prenomEtu; }
    unsigned int getNbCredits() const { return nb_CreditsCS+nb_CreditsTM; }
    Formation getCursus() const { return cursus; }
    void setCursus(const Formation& f);
    void setNom(const QString& n) { nomEtu=n; }
    void setPrenom(const QString& p) { prenomEtu=p; }
    void setFile(const QString& f) { file=f; }
    void setNbSemestre(unsigned int nb) { nbSemestre=nb; }
    void addCredits();
    void ajouterEquivalence(unsigned int creditsCS, unsigned int creditsTM);
    void ajoutInscription(const QString& uv, const QString& res, const QString& sem);
    void enregistrerSolution(Solution* s);
    void completerCursus();
    void lireSolution(const QString& f);
    void ajoutInscSolution(const QString& uv, const QString& s);

    //Méthode permettant d'accéder "publiquement" à l'itérateur du vecteur d'inscription
    QVector<Inscription*>::iterator begin_ins() { return inscriptions.begin(); }
    QVector<Inscription*>::iterator end_ins() { return inscriptions.end(); }
};

#endif // DOSSIER_H
