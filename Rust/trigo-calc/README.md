Le code de ce dossier à pour objectif de résoudre la **cinématique inverse** d'une patte à 3 degrées de libertés de robot hexapode, plus exactement du robot [Aash](https://github.com/notpunchnox/hexa), tout est visualisé dans un environnement 2D avec la librairie `Plotters`.
La version 3D est disponible également sur mon github [Aash-Simulator](https://github.com/notpunchnox/Aash-Simulator).

### Usage
Pour lancer le programme il vous suffira d'executer la commande suivante:

```sh
cargo run
```

Exemple de sortie:
```sh
$ cargo run
   Compiling trigo-calc v0.1.0 (/home/punchnox/Bureau/dev/learn/Rust/trigo-calc)
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.25s
     Running `target/debug/trigo-calc`

=== Programme de calcul -> trygonométrie ===
Coordonnées de la cible : X: 20.00 cm | Y: 0.00 cm | Z: -4.00 cm
Longueurs des segments de la patte (Constantes) : Coxa: 5.00 cm | Fémur: 6.30 cm | Tibia: 13.00 cm
Taille de la patte (projection vu de haut) : 20.00 cm

Angles calculés (en degrés) :
Theta 1 (Coxa)  : 0.00°
Theta 4 (Fémur) : 40.25°
Theta 5 (Tibia) : 101.38°
Angle d'élévation : -14.93°
Angle au coxa : 55.18°

Longueurs des projections des segments de la patte (vue du ciel) :
Coxa (c')  : 5.00 cm
Fémur (f') : 4.81 cm
Tibia (t') : 10.19 cm

Longeurs des projections de la patte (vue de profil) :
Fémur (f'') : 4.07 cm
Tibia (t'') : 10.95 cm

Coordonnées des articulations :
Coxa   : X: 0.00 | Y: 0.00 | Z: 0.00
Fémur  : X: 5.00 | Y: 0.00 | Z: 0.00
Tibia  : X: 9.81 | Y: 0.00 | Z: 4.07
Cible calculée : X: 20.00 | Y: 0.00 | Z: -4.00
Cible visée    : X: 20.00 | Y: 0.00 | Z: -4.00

Comparatif des mesures (coordonnées vs longueurs réelles) :
Coxa   : Coordonnées (X-Y): 5.00 cm | Longueur réelle c'': 5.00 cm
Fémur  : Coordonnées (X-Y): 6.30 cm | Longueur réelle f'': 6.30 cm
Tibia  : Coordonnées (X-Y): 13.00 cm | Longueur réelle t'': 13.00 cm
Cible  : Coordonnées (X-Y-Z): 20.00 cm; 0.00 cm; -4.00 cm | Cible réelle (X-Y-Z): 20 cm; 0 cm; -4 cm

Vue du ciel sauvegardée dans 'leg_visualization_top.png'
Visualisation sauvegardée dans 'leg_visualization.png'
```
---

### **Constantes**
- Longueur du coxa : $( l_c = 5.0 \, \text{cm} )$
- Longueur du fémur : $( l_f = 6.3 \, \text{cm} )$
- Longueur du tibia : $( l_t = 13.0 \, \text{cm} )$

### **1. Projection horizontale (vue de dessus)**
La distance horizontale de la cible par rapport à l'origine (projection dans le plan $(xy)$) :
$t_{\text{patte}} = \sqrt{x^2 + y^2}$

### **2. Distance dans le plan vertical**
Distance entre l'extrémité du coxa et la cible dans le plan vertical (incluant $(z)$) :
$(h = \sqrt{z^2 + (t_{\text{patte}} - l_c)^2})$
où $( t_{\text{patte}} - l_c )$ est la distance horizontale entre l'extrémité du coxa et la projection de la cible.

### **3. Calcul des angles**
#### **Theta 1 ($(\theta_1)$) : Angle azimuthal du coxa**
Angle de rotation autour de l'axe $(z)$ (dans le plan $(xy)$) :
$(\theta_1 = \arctan2(y, x))$

#### **Theta 5 ($(\theta_5)$) : Angle entre fémur et tibia**
Utilisation de la loi des cosinus pour déterminer l'angle entre le fémur et le tibia :
$(\cos(\theta_5) = \frac{l_f^2 + l_t^2 - h^2}{2 \cdot l_f \cdot l_t})$
$(\theta_5 = \arccos(\cos(\theta_5)))$
*Note* : Si $(\cos(\theta_5) > 1)$ ou $(\cos(\theta_5) < -1)$, la position cible est inatteignable.

#### **Angle au coxa**
Angle entre le fémur et la droite reliant l'extrémité du coxa à la cible :
$(\cos(\theta_{\text{coxa}}) = \frac{l_f^2 + h^2 - l_t^2}{2 \cdot l_f \cdot h})$
$(\theta_{\text{coxa}} = \arccos(\cos(\theta_{\text{coxa}})))$

#### **Angle d'élévation**
Angle d'élévation du vecteur cible par rapport à l'extrémité du coxa :
$(\theta_{\text{elev}} = \arctan2(z, t_{\text{patte}} - l_c))$

#### **Theta 4 ($(\theta_4)$) : Angle du fémur**
Angle total du fémur par rapport à l'horizontale :
$(\theta_4 = \theta_{\text{elev}} + \theta_{\text{coxa}})$

### **4. Projections des segments**
#### **Vue de dessus (plan $(xy)$)**
- Coxa : $( c' = l_c )$
- Fémur : $( f' = l_f \cdot \cos(\theta_4) )$
- Tibia : $( t' = t_{\text{patte}} - (c' + f') )$

#### **Vue de profil (plan vertical)**
- Fémur : $( f'' = l_f \cdot \sin(\theta_4) )$
- Tibia : $( t'' = \sqrt{z^2 + t'^2} )$

### **5. Coordonnées des articulations**
#### **Base du coxa**
$((x_{\text{coxa}}, y_{\text{coxa}}, z_{\text{coxa}}) = (0, 0, 0))$

#### **Extrémité du coxa (base du fémur)**
$(x_{\text{femur}} = l_c \cdot \cos(\theta_1))$
$(y_{\text{femur}} = l_c \cdot \sin(\theta_1))$
$(z_{\text{femur}} = 0)$

#### **Extrémité du fémur (base du tibia)**
$(x_{\text{tibia}} = x_{\text{femur}} + l_f \cdot \cos(\theta_1) \cdot \cos(\theta_4))$
$(y_{\text{tibia}} = y_{\text{femur}} + l_f \cdot \sin(\theta_1) \cdot \cos(\theta_4))$
$(z_{\text{tibia}} = z_{\text{femur}} + l_f \cdot \sin(\theta_4))$

#### **Position finale (extrémité du tibia)**
Vecteur de l'extrémité du tibia à la cible :
$(\Delta x = x - x_{\text{tibia}}, \quad \Delta y = y - y_{\text{tibia}}, \quad \Delta z = z - z_{\text{tibia}})$

Distance à la cible :
$(d_{\text{target}} = \sqrt{\Delta x^2 + \Delta y^2 + \Delta z^2})$

Coordonnées normalisées pour respecter la longueur du tibia :
$(x_{\text{end}} = x_{\text{tibia}} + l_t \cdot \frac{\Delta x}{d_{\text{target}}})$
$(y_{\text{end}} = y_{\text{tibia}} + l_t \cdot \frac{\Delta y}{d_{\text{target}}})$
$(z_{\text{end}} = z_{\text{tibia}} + l_t \cdot \frac{\Delta z}{d_{\text{target}}})$

### **6. Vérification des longueurs**
- Longueur du coxa (calculée vs réelle) :
$(\sqrt{(x_{\text{femur}} - x_{\text{coxa}})^2 + (y_{\text{femur}} - y_{\text{coxa}})^2 + (z_{\text{femur}} - z_{\text{coxa}})^2} \quad \text{vs} \quad l_c)$
- Longueur du fémur :
$(\sqrt{(x_{\text{tibia}} - x_{\text{femur}})^2 + (y_{\text{tibia}} - y_{\text{femur}})^2 + (z_{\text{tibia}} - z_{\text{femur}})^2} \quad \text{vs} \quad l_f)$
- Longueur du tibia :
$(\sqrt{(x_{\text{end}} - x_{\text{tibia}})^2 + (y_{\text{end}} - y_{\text{tibia}})^2 + (z_{\text{end}} - z_{\text{tibia}})^2} \quad \text{vs} \quad l_t)$
