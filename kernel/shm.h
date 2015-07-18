#ifndef __SHM_H__
#define __SHM_H__

/* 
 * Initialise une liste des pages disponibles
 */
void init_shm();

/* 
 * Demande la création d'une page paratagée de 4 Ko identifiée
 * par la chaine key
 *
 * Retourne l'addresse de la page si l'allocation et le mappage 
 * de celle ci ont été effectués avec succès
 * Retourne NULL sinon
 */
void *shm_create(const char *key);

/* 
 * Permet d'obtenir une référence sur la page partagée identifiée
 * par key
 *
 * Retourne l'adresse si celle ci est disponible et a été correctement 
 * mappée
 * Retourne NULL sinon
 */
void *shm_acquire(const char *key);

/*
 * Permet de relacher la référence sur la page identifiée par key.
 * Si la page a été effectivement acquise précédemment et mappée
 * celle ci est démappée sinon l'appel est sans effet.
 * Si cet appel ammène à relacher la dernière référence sur cette page
 * celle ci est libérée
 */
void shm_release(const char *key);

#endif
