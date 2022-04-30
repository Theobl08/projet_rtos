#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

struct valeurCapteurs {
    int analogique;
    int numérique;
    double tempsEnMillisecondes;
};

QueueHandle_t Queue1;
QueueHandle_t Queue2;
QueueHandle_t Queue3;
QueueHandle_t Queue4;

#define bouton1 3
#define bouton2 4
#define anaologique A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Initialisation de port serie
  
  //Attente de connexion au port serie
  while (!Serial);
  
  if ( xSerialSemaphore == NULL )  // On verifie que le Semaphore de port serie n'a pas encore été créé.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // On créé un mutex que nous allons utiliser pour gérer le port serie
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // On rend le port série disponible, en "donnant" le Semaphore.
  }
  //Mise en entrée des bouttons et du potentiometre analogique
  pinMode(analogique, INPUT);
  pinMode(bouton1, INPUT);
  pinMode(bouton2, INPUT);

  //Creation des queues associées aux taches
  Queue1 = xQueueCreate(8, sizeof(int));
  Queue2 = xQueueCreate(8, sizeof(int));
  Queue3 = xQueueCreate(8, sizeof(valeurCapteurs));
  Queue4 = xQueueCreate(8, sizeof(valeurCapteurs));
  
  //Creation des 5 taches
  xTaskCreate(Tache1, "Potentiomètre", 255, NULL, 0, NULL);
  xTaskCreate(Tache2, "Recupère la valeur numérique de l'addition des deux boutons", 255, NULL, 0, NULL);
  xTaskCreate(Tache3, "Initialisation de la structure avec les valeurs des taches 1 et 2 et de la fonction millis()", 255, NULL, 0, NULL);
  xTaskCreate(Tache4, "Affichage de la structure avec le temps en milliseconde", 255, NULL, 0, NULL);
  xTaskCreate(Tache5, "Affichage de la structure avec le temps en minute", 255, NULL, 0, NULL);

void Tache1(void* pvParameters) 
{
  int potentiometre;
  while (1) 
  {
    potentiometre = analogRead(analogique);//Recupération de la valeur lu par le potentiometre
    xQueueSend(Queue1, &potentiometre,0);//on l'envoie dans la queue associé a la tache 1
    vTaskDelay( 1000 ); // attente de 1 seconde
  }
}

void Tache2(void* pvParameters)
{
  int addition;
  while(1)
  {
    addition = digitalRead(bouton1) + digitalRead(bouton2); //On additionne les valeur lues par les bouttons (0 ou 1 pour chaque boutton), ce qui donne un resultat compris entre 0 et 2
    xQueueSend(Queue2, &addition, 0); //Envoie vers queue de la tache 2
    vTaskDelay(1000);
  }
}

void Tache3(void *pvParameters)
{
  int t1;//Valeur du potentiometre obtenue à la tache 1
  int t2;//valeur des deux boutons additionné obtenue à la tache 2
  valeurCapteurs c;
  while(1)
  {
    xQueueReceive(Queue1, &t1, 0);//On reçoit le resultat de la tache 1
    xQueueReceive(Queue2, &t2, 0);//On reçoit le résultat de la tache 2
    c.analogique = t1;
    c.numerique = t2;
    c.tempsEnMillisecondes = millis();//on récupère le resultat de la fonctin millis
    xQueueSend(Queue3, &c, 0);//Envoie Queue tache 3
    vTaskDelay(1000);
  }
}

void Tache4(void *pvParameters)
{
  valeurCapteurs c;
  while(1)
  {
    xQueueReceive(Queue3, &c, 0);
    Serial.print("Valeur analogique lue par le potentimetre : ");
    Serial.println(c.analogique);//Affichage du champ c.analogique
    Serial.print("Valeur numerique lue par les 2 boutons : ");
    Serial.println(c.numerique);//affichage du champ c.numerique
    Serial.print("Temps écoulé (en ms) : ");
    Serial.println(c.tempsEnMillisecondes);//affichage du champ c.tempsEnMillisecondes
    xQueueSend(Queue4, &c, 0);
    vTaskDelay(1000);
  }
}

void Tache5(void *pvParameters)
{
  valeurCapteurs c;
  while(1)
  {
    xQueueReceive(Queue4, &c, 0);
    Serial.print("Valeur analogique lue par le potentimetre : ");
    Serial.println(c.analogique);
    Serial.print("Valeur numerique lue par les 2 boutons : ");
    Serial.println(c.numerique);
    Serial.print("Temps écoulé (en minute) : ");
    Serial.println(c.tempsEnMillisecondes/60000);//Affichage du champ c.tempsEnMillisecondes converti en minute (1ms = 1/1000 secondes, ou 0.001 seconde, et 1 seconde = 1/60 minute)
    vTaskDelay(1000);
  }
}