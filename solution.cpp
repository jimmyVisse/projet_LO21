#include "solution.h"

//Détermination du plusGrandSemestre de la solution, pour acquérir le semestre actuel
Semestre Solution::plusGrandSemestre() const {
    Semestre plusGrand = inscription[0]->getSemestre();
    for(int i=1; i<inscription.size(); i++) {
        if(inscription[i]->getSemestre()>plusGrand) {
            plusGrand = inscription[i]->getSemestre();
        }
    }
    return plusGrand;
}

//Ajout d'une inscription à une solution
void Solution::ajoutInscription(const QString &uv, const QString &sem) {
    UV& u = UVManager::getInstance().getUV(uv);
    Semestre s = StringToSemestre(sem);
    inscription.push_back(new Inscription(u, s));
}
