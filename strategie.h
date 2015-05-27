/*
 * Fichier gérant les classes du design pattern Strategie
*/

#ifndef STRATEGIE_H
#define STRATEGIE_H

#include "UTProfiler.h"
#include "uv.h"
#include "solution.h"

//utilisation du design pattern Strategie pour l'algorithme de complétion du cursus
class Strategie {
public:
    virtual void completer(Solution* s, unsigned int nbSemestre, QVector<UV*>& uvs) = 0;
};

//Algorithme concret pour le TC
class StrategieTC: public Strategie {
public:
    void completer(Solution* s, unsigned int nbSemestre, QVector<UV *> &uvs);
};

//Algorithme concret pour le GX
class StrategieGX: public Strategie {
public:
    void completer(Solution *s, unsigned int nbSemestre, QVector<UV*>& uvs);
};

//Algorithme concret pour le HuTech
class StrategieHuTech: public Strategie {
public:
    void completer(Solution *s, unsigned int nbSemestre, QVector<UV*>& uvs);
};

#endif // STRATEGIE_H
