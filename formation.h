/*
 * Ensemble de classes pour la création et gestion des cursus
*/

#ifndef FORMATION_H
#define FORMATION_H

#include "UTProfiler.h"
#include "uv.h"

//Classe formation
class Formation {
private:
    QString nom;
    UV** uvs;
    unsigned int nb_Uvs;
    unsigned int nbMaxUvs;
    friend class CursusManager;

public:
    Formation(const QString& n);
    virtual ~Formation();
    QString getNom() const { return nom; }
    UV& getUV(const QString &code) const;
    void ajoutUV(UV& uv);
    void supprimerUV(UV& uv);
    //Itérateur public pour le parcours des UVs de la formation
    class Iterateur {
    private:
        UV** current;
        unsigned int nbRestant;
        friend class Formation;
        Iterateur(UV** u, unsigned int nb): current(u), nbRestant(nb) {}
    public:
        Iterateur(): current(0), nbRestant(0) {}
        bool isDone() const { return nbRestant==0; }
        UV& currentItem() const { return **current; }
        void next() { current++; nbRestant--; }
    };
    Iterateur begin() { return Iterateur(uvs, nb_Uvs); }
};

//Classe centralisant la gestion des formations
class CursusManager: public Manager {
    QMap<QString, Formation*> cursus;
    CursusManager(const QString& f);
    virtual ~CursusManager();
    Formation* trouverCursus(const QString& f) const;
    friend struct Handler;
    struct Handler{
        CursusManager* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; instance=0; }
    };
    static Handler handler;

public:
    static CursusManager& getInstance();
    static void libererInstance();
    void lireFichier(QXmlStreamReader& xml);  //Redéfinition des méthodes du Template Method
    void ecrireFichier(QXmlStreamWriter* r);
    void supprimerCursus(const QString& c);
    void ajouterCursus(const QString& n);
    void ajouterUVCursus(const QString& n, const QString& uv);
    void supprimerUVCursus(const QString& n, const QString& uv);
    Formation& getFormation(const QString& f);
};


#endif // FORMATION_H
