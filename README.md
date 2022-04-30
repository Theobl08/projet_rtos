# projet_rtos
Le but de ce projet est de créer 5 tache que nous allons téléverser sur une carte Arduino Mega
# Tache 1
Récupère une valeur analogique sur l’entrée A0 qui est branchée avec un potentiomètre puis l’envoie à la tâche 3 (valeur entre 0 et 1023) 
# Tache 2
Récupère une valeur numérique qui est la résultante de l'addition des deux valeurs des deux entrées numérique 3 et 4 qui sont branchées avec des boutons poussoirs en montage pull down, puis envoie cette valeur numérique à la tâche 3 (valeur entre 0 et 2) 
# Tache 3
Reçoit les deux valeurs des tâches 1 et 2 puis les mets dans une structure en ajoutant la valeur de la fonction millis()
# Tache 4
Cette tâche reçoit la valeur de la structure et utilise le port série pour l’afficher, ensuite envoie cette structure à la tâche 5.
# Tache 5
Cette tâche doit transformer la valeur du temps dans la structure en minutes, ensuite elle doit afficher cette nouvelle structure à travers le port série.
