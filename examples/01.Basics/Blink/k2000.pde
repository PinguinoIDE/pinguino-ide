int compteur;									// definit une variable entiere pour le compteur
int i;											// variable servant a l'initialisation

void setup(void)
{
	for (i=0;i<8;i++) 
	{
		pinMode(i,OUTPUT);						// definit les broches 0 a 7 en sortie
		digitalWrite(i,LOW);					// fixe un niveau 0 sur les sorties
	}
}



void loop(void)
{
	for (compteur=0;compteur<8;compteur++)		// pour compteur variant de 0 a 7
	{
		digitalWrite(compteur,HIGH);			// allume la led compteur
		if (compteur==0)
			digitalWrite(7,LOW);				// si led courante=0 eteindre la led 7
		else
			digitalWrite(compteur-1,LOW);		// sinon eteindre la led d'indice -1
		delay(50);								// attendre 50 milli-Secondes
	}
}