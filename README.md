# 2025_PSE_B6_gsailly_bjoumier

### Choix Projet :

Dans le cadre de ce cours de Premier Système Embarqué, nous avons pour objectif de créer un système embarqué en ayant pour seule consigne d'y intégrer un AVR ATmega32u4 AT90USB1287 et une alimentation hybride :
- par un port USB, méthode utilisée pour la programmation, les tests et la configuration
- par une batterie Lithium, en mode autonome.

Notre binôme a donc choisi de réaliser une montre connectée qui a pour objectif principal d'afficher l'heure et de capturer les BPM de l'utilisateur. (D'autres fonctions pourront être ajoutées si elles sont réalisables)

---

### Réalisation Projet :

#### Partie Matériel :

Pour réaliser cette montre connectée, nous avons dans un premier temps utilisé le logiciel KiCad, pour réaliser le schéma et le PCB. *[(Voir /Materiel)](https://gitea.plil.fr/gsailly/2025_PSE_B6_gsailly_bjoumier/src/branch/main/Materiel)*

Suite à cette réalisation, nous avons :
- soudé le µC, le crystal et leurs capacités/résistances associées
- testé le bon fonctionnement de la carte via l'ICSP, (Comme nous n'avions pas encore le régulateur de tension et le cavalier, nous avons court-circuité la pate +5V du connecteur J8 avec R1 afin d'alimenter le µC)
- soudé les LEDs, puis testé ces dernières via un simple code pour les faire clignoter :

```c
	#define F_CPU 8000000UL
		
	#include <avr/io.h>
	#include <util/delay.h>
	
	void delay(volatile uint32_t nCount) {
	    while(nCount--) {}
	}
	
	int main(void) {
	
		CLKSEL0 = 0b00010101;   // sélection de l'horloge externe
		CLKSEL1 = 0b00001111;   // minimum de 8Mhz
		CLKPR = 0b10000000;     // modification du diviseur d'horloge (CLKPCE=1)
		CLKPR = 0;              // 0 pour pas de diviseur (diviseur de 1)
	
	    DDRB |= (1 << PB4) | (1 << PB5);
	
	    while(1) {
	         // LED sur PB5 ON
	        PORTB |= (1 << PB5);
	
	        // LED sur PB4 OFF
	        PORTB &= ~(1 << PB4);
	
	        _delay_ms(500);
	
	        // Inverser
	        PORTB &= ~(1 << PB5);
	        PORTB |= (1 << PB4);
	
	        _delay_ms(500);
	    }
	}
```
- soudé le capteur de BPM MAX30102 (que nous avons probablement grillé en le soudant puis en branchant la carte avant de souder le régulateur)
- soudé tous les connecteurs manquants ainsi que le régulateur de tension et l'inductance (ce qui nous a permis de brancher l'écran I2C et le module Bluetooth HC-05)

Enfin, à la dernière séance, nous avons soudé le MAX1811 qui nous a permis de faire fonctionner la montre via le chargeur LIPO en branchant la batterie.

#### Partie Logiciel :

Pour réaliser la partie logicielle de la montre, nous avons développé le programme embarqué en C pour le microcontrôleur ATmega32U4. *[(Voir /Logiciel)](https://gitea.plil.fr/gsailly/2025_PSE_B6_gsailly_bjoumier/src/branch/main/Logiciel)*

Nous avons utilisé plusieurs bibliothèques afin de faciliter la gestion des différents périphériques :

- **U8G2** : gestion de l’écran OLED SSD1306
- **LUFA** : gestion de la communication USB
- pilotes personnalisés pour :
  - le Bluetooth HC-05
  - le capteur MAX30102
  - l’I2C
  - les timers
  - la gestion de l’heure

##### Structure du projet

```text
Logiciel/
├──	montre_connectee/
│	├── include/
│	│   └── LUFA/
│	├── lib/
│	│   └── U8g2/
│	├── src/
│	│   ├── main.c
│	│   ├── display.c
│	│   ├── display.h
│	│   ├── i2c.c
│	│   ├── i2c.h
│	│   ├── max30102.c
│	│   ├── max30102.h
│	│   ├── timekeeper.c
│	│   ├── timekeeper.h
│	│   ├── timer.c
│	│   ├── timer.h
│	│   ├── uart.c
│	│   ├── uart.h
│	│   ├── usb_comm.c
│	│   └── usb_comm.h
│	└── Makefile
```

##### Fonctionnement général

Le programme initialise :
- l’écran OLED,
- la communication Bluetooth UART,
- les timers,
- la gestion du temps,
- ainsi que le capteur cardiaque MAX30102.

L’heure peut être synchronisée via Bluetooth grâce à l’application **Serial Bluetooth Terminal** sur tablette ou smartphone.

Un message au format suivant est envoyé :
```text
T14:30:00
```

Le microcontrôleur lit ensuite cette trame et met à jour l’horloge affichée à l’écran.

##### Affichage

L’écran affiche :
- l’heure actuelle,
- le rythme cardiaque en BPM (le capteur étant grillé, nous avons chois d'afficher une valeur de 80 BPM fixe),
- ainsi qu’une interface graphique réalisée avec la bibliothèque U8G2.

##### Communication Bluetooth

Le module **HC-05** communique avec l’ATmega32U4 via l’UART à 9600 bauds.

La réception Bluetooth est gérée directement dans la boucle principale du programme.

##### Capteur cardiaque

Le capteur **MAX30102** communique via le protocole I2C.

Nous avons développé un pilote permettant :
- l’initialisation du capteur,
- la lecture des valeurs IR,
- ainsi que le calcul du rythme cardiaque.

##### Compilation

Le projet est compilé avec `avr-gcc` via un `Makefile`.

Effacer :
```bash
make clean
```

Compilation :
```bash
make all
```

Appuer sur les boutons NHWB (maintenir) et RESET (1 pression) puis relâcher NHWB pour pouvoir re-flasher la carte.

Téléversement :
```bash
make upload
```

Débrancher puis Rebrancher la carte.


### Carte en fontionnement (USB et LIPO)

![carte USB](/Documentation/carte_fonctionnement_usb.jpeg "Photo de la carte en fonctionnement alimentée via l'USB")

![carte LIPO](/Documentation/carte_fonctionnement_lipo.jpeg "Photo de la carte en fonctionnement alimentée via le LIPO")


---

### Erreurs / Points d'amélioration :

#### Erreurs : 

- Mauvaise empreinte pour le crystal (empreinte légèrement trop petite).
- Nous avons voulu tester le capteur de BPM  avant d'avoir le régulateur de tension, ce qui a pu le griller (en plus de l'avoir soudé à une température trop élevée).

![BPM cramé](/Documentation/capteur_bpm_grille.jpeg "Photo du Capteur de BPM grillé")

- Nous avions utilisé un mauvais baudrate (115200 au lieu de 9600), ce qui nous a fait perdre entre 30 et 45 minutes.
- Mauvais paramétrage de l'horloge qui prenait 8 secondes à réaliser une instruction d'1 seconde, ce qui nous a fait perdre entre 15 et 30 minutes. Nous avons résolu ce problème grâce aux 4 lignes de code fournies dans le cours de Mr Redon ainsi qu'en remplaçant le chiffre 16 (MHz) par 8 (MHz) dans le code ```uart.c``` : 

```c
CLKSEL0 = 0b00010101;   // sélection de l'horloge externe
CLKSEL1 = 0b00001111;   // minimum de 8Mhz
CLKPR = 0b10000000;     // modification du diviseur d'horloge (CLKPCE=1)
CLKPR = 0;              // 0 pour pas de diviseur (diviseur de 1)
```

```c
uint16_t ubrr = (F_CPU / 8 / baud) - 1;
```


#### Points d'amélioration :

- Utilisation d'un levier/interrupteur pour passer de l'alimentation USB à l'alimentaion sur batterie.
- Allumer la LED D4 qui indique le passage de la carte à l'alimentaion sur batterie.
- Ajouter des boutons sur la carte qui auraient permis de régler l'heure directement dessus (1 pression rajoute +1 à l'heure ou aux minutes ou aux secondes).
- Ajouter un sorte de "joystick" qui aurait pu servir à modifier l'heure (déplacement gauche pour reculer, déplacement droit pour avancer, déplacement haut pour passer à l'unité suivante, déplacement vers le bas pour passer à l'unité précédente).
- Ajouter une molette pour régler l'heure.

---

Premier Système Embarqué - IoT 3 - B6 - **[Notre Wiki](https://projets-se.plil.fr/mediawiki/index.php/SE3Groupe2025-6)** - *[Accès à notre machine de TP (zabeth16)](https://vnc.plil.fr/ssh/?16)*
