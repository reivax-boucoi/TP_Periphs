#ifndef MOTEUR_H
#define MOTEUR_H

#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 

void MoteurPWM_Conf(TIM_TypeDef * Timer, uint32_t Channel);

void DefSensPlateau(int sens);

//réglage de la vitesse du plateau
void PourcentageRotation(TIM_TypeDef * Timer, int vitesse);

void CommandeMoteur(TIM_TypeDef * Timer, int remote);

#endif
