#include "strategie.h"

//algorithme de complétion du TC
void StrategieTC::completer(Solution *s, unsigned int nbSemestre, QVector<UV *>& uvs) {
    unsigned int credCS = s->getCredCS();
    unsigned int credTM = s->getCredTM();

    //On récupère le semestre actuel
    Semestre semActuel = s->plusGrandSemestre();
    ++semActuel;

    QVector<UV*>::iterator it = uvs.begin();
    /*Tant que les conditions de validation du TC en terme de crédits ne sont pas validées
     ou qu'il reste des UVs à parcourir,  on fait le traitement  */
    while(it!=uvs.end() && credCS<48 && credTM<24) {
        nbSemestre++;
        unsigned int credCSSemestre = 0;
        unsigned int credTMSemestre = 0;
        unsigned int nbUVs = 0;
        while(nbSemestre<=6 && nbUVs<4 && credCSSemestre+credTMSemestre<35) {
            if((*it)->getCode() == "MT90") {
                s->ajoutInscription((*it)->getCode(), SemestreToString(semActuel));
                credCS += UVManager::getInstance().getUV("MT90").getNbCredits();
                credCSSemestre += UVManager::getInstance().getUV("MT90").getNbCredits();
                nbUVs++;
            }
            if ((*it)->getCode() == "PS90") {
                s->ajoutInscription((*it)->getCode(), SemestreToString(semActuel));
                credTM += UVManager::getInstance().getUV("PS90").getNbCredits();
                credTMSemestre += UVManager::getInstance().getUV("PS90").getNbCredits();
                nbUVs++;
            }
            if((*it)->getPreference()!=(-1)) {
                if(semActuel.getSaison() == Automne) {
                    if((*it)->ouvertureAutomne()) {
                        if((*it)->getCategorie() == CS) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credCS += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credCSSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                        if((*it)->getCategorie() == TM) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credTM += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credTMSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                    }
                }
                else if(semActuel.getSaison() == Printemps) {
                    if((*it)->ouverturePrintemps()) {
                        if((*it)->getCategorie() == CS) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credCS += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credCSSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                        if((*it)->getCategorie() == TM) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credTM += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credTMSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                    }
                }
            }
            else {
                it++;
                break;
            }
            it++;
        }
        ++semActuel;
    }
    if(credCS<48 || credTM<24) {
        throw UTProfilerException("Erreur, pas de solution pour vos criteres");
    }
}

//algorithme de complétion du GX
void StrategieGX::completer(Solution *s, unsigned int nbSemestre, QVector<UV *>& uvs) {
    unsigned int credCS = s->getCredCS();
    unsigned int credTM = s->getCredTM();

    Semestre semActuel = s->plusGrandSemestre();
    ++semActuel;

    QVector<UV*>::iterator it = uvs.begin();
    /*Tant que les conditions de validation du GX en terme de crédits ne sont pas validées
     ou qu'il reste des UVs à parcourir,  on fait le traitement  */
    while( (credCS<30 && credTM<30) && credCS+credTM<84 && it!=uvs.end()) {
        nbSemestre++;
        unsigned int credCSSemestre = 0;
        unsigned int credTMSemestre = 0;
        unsigned int nbUVs = 0;
        while(nbSemestre<=9 && nbUVs<5 && credCSSemestre+credTMSemestre<35) {
            if((*it)->getPreference()!=(-1)) {
                if(semActuel.getSaison() == Automne) {
                    if((*it)->ouvertureAutomne()) {
                        if((*it)->getCategorie() == CS) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credCS += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credCSSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                        if((*it)->getCategorie() == TM) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credTM += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credTMSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                    }
                }
                else if(semActuel.getSaison() == Printemps) {
                    if((*it)->ouverturePrintemps()) {
                        if((*it)->getCategorie() == CS) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credCS += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credCSSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                        if((*it)->getCategorie() == TM) {
                            QString codeUV = (*it)->getCode();
                            s->ajoutInscription(codeUV, SemestreToString(semActuel));
                            credTM += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            credTMSemestre += UVManager::getInstance().getUV(codeUV).getNbCredits();
                            nbUVs++;
                        }
                    }
                }
            }
            else {
                it++;
                break;
            }
            it++;
        }
        ++semActuel;
    }

    if(credCS<30 && credTM<30) {
        throw UTProfilerException("Erreur, pas de solution pour vos criteres");
    }
}

//algorithme de complétion du HuTech
void StrategieHuTech::completer(Solution *s, unsigned int nbSemestre, QVector<UV *> &uvs) {
}
