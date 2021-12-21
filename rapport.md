# Rapport: système solaire

## Introduction

Pour réaliser une simulation d'un système planétaire, plusieurs facteurs sont à prendre en compte. Puisque dans notre univers, tous les corps sont soumis à des forces. Nous allons donc utiliser pour cela les trois **lois de Newton**:

  1. Si un corps est immobile (ou en mouvement rectiligne uniforme), alors la somme des forces qu’il subit, appelée force résultante, est nulle. (F⃗ =0⃗ ).\
  2. La force résultante subit par un corps est égale à la masse de ce dernier multipliée par son accélération. (F⃗ =ma⃗ p).\
  3. Si un corps A subit une force de la part d’un corps B, alors le corps B subit une force de réaction de sens opposé et de même intensité (F⃗ BA=−F⃗ AB).\

## Réalisation de la simulation d'un système planétaire

### _Création d'une étoile(dans notre cas: le soleil)_

Le soleil sera représenté comme une planète donc aura la structure d'une planète mais son comportement sera différentes des autres planètes. Pour ce faire, on a tout d'abord défini la structure d'une planète:

```c
    typedef struct _planet
    {
        double mass; //masse de la planète
        int32_t color; //couleur sur la simulation
        double radius; //Grandeur de la planète 
        double semi_major_axis; //Distance par rapport au soleil
        double eccentricity; //excentricité
        vec2 pos;      // x(t)
        vec2 prec_pos; // x(t - dt)
    } planet_t;

```

Comme convenu, pour la création de notre étoile nous avons donc eu besoin d'implémenter une fonction qui permet de créer les planètes. Cette fonction contient également un champ qui permettra de préciser si cette planète créée est une étoile(*champ: is_star*):

```c
    planet_t create_planet(double mass, int color, double radius, double semi_major_axis, double eccentricity, bool is_star) {
        planet_t planet = { //attribution de tous les paramètres
            .mass = mass,
            .color = color,
            .radius = radius,
            .semi_major_axis = semi_major_axis,
            .eccentricity = eccentricity,
        };

        if (is_star) { //Si c'est une étoile on le place au milieu donc vecteur 0
            planet.prec_pos = planet.pos = vec2_create_zero();
        } else { //Sinon on la place par rapport à sa distance au soleil(étoile) et son excentricité
            planet.prec_pos = planet.pos = vec2_create(semi_major_axis * (1 - eccentricity), 0);
        } //La planète est aligné horizontalement au soleil
        return planet;
    }
```

### _Ajout de planètes autour de étoile(Soleil)_

Ici on a déjà la base pour créer chaque planète, donc on désire en créer chaque planète autour de l'étoile. Pour cela nous avons décidé de créer les planètes à partir d'un fichier _.csv_ pour ne pas avoir à modifier tout le code à chaque création de planètes. Notre fichier _.csv_ est composé comme ci-dessous:


   |  Name   |   Mass    | Semi major axis | Eccentricity | Radius |   Color    |
   | :-----: | :-------: | :-------------: | :----------: | :----: | :--------: |
   |   Sun   | 1.989e30  |        0        |      0       |   8    | 0x00FFFF00 |
   | Mercury | 3.285e23  |    57.909e9     |    0.2056    |  0.5   | 0x00E98B41 |
   |  Venus  | 4.867e24  |    108.209e9    |    0.0067    |   2    | 0x001AA89A |
   |  Earth  | 5.972e24  |    149.596e9    |    0.0167    |   1    | 0x00669EBF |
   |  Mars   | 6.417e23  |    227.923e9    |    0.0935    |  1.5   | 0x00E26642 |
   | Jupiter | 1.989e30  |    378.570e9    |    0.0489    |  2.5   | 0x00F5F5DC |
   | Saturn  | 568.32e27 |     1434e9      |    0.0565    |  2.25  | 0x00FFF2CC |

Pour créer chaque planète à partir de ce tableau, on a créer une fonction qui permettra de récupérer chaque élément du tableau et l'attribuer dans le bon champs de la planète.