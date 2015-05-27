/*
 * Classe gérant les solutions
*/

#ifndef SOLUTION_H
#define SOLUTION_H

#include "UTProfiler.h"
#include "uv.h"
#include "inscription.h"

class Solution {
private:
    Solution() {}
    Solution(unsigned int cs, unsigned int tm): inscription(), file("sol_dossier.xml"), nbCredCS(cs), nbCredTM(tm) {}
    QVector<Inscription*> inscription;
    QString file;
    unsigned int nbCredCS;
    unsigned int nbCredTM;
    friend class Dossier;
public:
    unsigned int getCredCS() const { return nbCredCS; }
    unsigned int getCredTM() const { return nbCredTM; }
    void ajoutInscription(const QString& uv, const QString& sem);
    Semestre plusGrandSemestre() const;
};


#endif // SOLUTION_H
