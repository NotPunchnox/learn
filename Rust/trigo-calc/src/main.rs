mod visualisation;
use crate::visualisation::{visualize_leg, visualize_leg_top_view};

const COXA: f64 = 5.0;
const FEMUR: f64 = 6.3;
const TIBIA: f64 = 13.0;

fn calcul(x: f64, y: f64, z: f64) {
    let tpatte = (x * x + y * y).sqrt();

    println!(
        "Coordonnées de la cible : X: {:.2} cm | Y: {:.2} cm | Z: {:.2} cm",
        x, y, z
    );
    println!("Longueurs des segments de la patte (Constantes) : Coxa: {:.2} cm | Fémur: {:.2} cm | Tibia: {:.2} cm", COXA, FEMUR, TIBIA);
    println!(
        "Taille de la patte (projection vu de haut) : {:.2} cm\n",
        tpatte
    );

    // Theta 1: angle azimuthal du coxa (rotation autour de Z)
    let theta1 = y.atan2(x);
    
    // Distance horizontale réelle après rotation du coxa
    let horizontal_dist = tpatte - COXA;
    let h = (z * z + horizontal_dist * horizontal_dist).sqrt();
    
    // Application de la loi des cosinus pour trouver l'angle d'articulation entre fémur et tibia
    let cos_theta5 = (FEMUR * FEMUR + TIBIA * TIBIA - h * h) / (2.0 * FEMUR * TIBIA);
    
    // Vérifier que le calcul est valide
    if cos_theta5 > 1.0 || cos_theta5 < -1.0 {
        println!("Erreur: position cible non atteignable !");
        return;
    }
    
    // angle entre fémur et tibia
    let theta5 = cos_theta5.acos();
    
    // (coxa position -> target) à (coxa articulation -> femur -> target)
    // cos(angle_au_coxa) = (FEMUR^2 + h^2 - TIBIA^2) / (2 * FEMUR * h)
    let cos_angle_au_coxa = (FEMUR * FEMUR + h * h - TIBIA * TIBIA) / (2.0 * FEMUR * h);
    let angle_au_coxa = cos_angle_au_coxa.acos();
    
    // L'angle d'élévation du vecteur cible par rapport au coxa
    let target_horiz_dist = tpatte - COXA;
    let elev_angle = z.atan2(target_horiz_dist);
    
    let theta4 = elev_angle + angle_au_coxa;

    // Afficher les angles calculés
    println!("Angles calculés (en degrés) :");
    println!("Theta 1 (Coxa)  : {:.2}°", theta1.to_degrees());
    println!("Theta 4 (Fémur) : {:.2}°", theta4.to_degrees());
    println!("Theta 5 (Tibia) : {:.2}°", theta5.to_degrees());
    println!("Angle d'élévation : {:.2}°", elev_angle.to_degrees());
    println!("Angle au coxa : {:.2}°", angle_au_coxa.to_degrees());

    // Projections horizontales des segments pour vue de dessus
    let c_prime = COXA;
    let f_prime = FEMUR * theta4.cos();
    let t_prime = horizontal_dist - f_prime;

    println!("\nLongueurs des projections des segments de la patte (vue du ciel) :");
    println!("Coxa (c')  : {:.2} cm", c_prime);
    println!("Fémur (f') : {:.2} cm", f_prime);
    println!("Tibia (t') : {:.2} cm", t_prime);

    // Projections verticales pour vue de profil
    println!("\nLongeurs des projections de la patte (vue de profil) :");
    let _c_prime_prime = COXA;
    let f_double_prime = FEMUR * theta4.sin();
    let t_double_prime = (z * z + t_prime * t_prime).sqrt();

    println!("Fémur (f'') : {:.2} cm", f_double_prime);
    println!("Tibia (t'') : {:.2} cm", t_double_prime);

    // Coordonnées 3D de chaque articulation
    
    // Origine: base du coxa
    let x_coxa: f64 = 0.0;
    let y_coxa: f64 = 0.0;
    let z_coxa: f64 = 0.0;

    // Extrémité du coxa après rotation theta1
    let x_femur: f64 = COXA * theta1.cos();
    let y_femur: f64 = COXA * theta1.sin();
    let z_femur: f64 = 0.0;

    // Extrémité du fémur: extension dans le plan vertical orienté par theta1
    let x_tibia: f64 = x_femur + FEMUR * theta1.cos() * theta4.cos();
    let y_tibia: f64 = y_femur + FEMUR * theta1.sin() * theta4.cos();
    let z_tibia: f64 = z_femur + FEMUR * theta4.sin();

    // Position finale: extension du tibia vers la cible
    // Normalisation du vecteur tibia->cible pour respecter la longueur TIBIA
    let to_target_x = x - x_tibia;
    let to_target_y = y - y_tibia;
    let to_target_z = z - z_tibia;
    let dist_to_target = (to_target_x * to_target_x + to_target_y * to_target_y + to_target_z * to_target_z).sqrt();
    
    // Vecteur tibia normalisé à la longueur exacte
    let tibia_extension_x = TIBIA * to_target_x / dist_to_target;
    let tibia_extension_y = TIBIA * to_target_y / dist_to_target;
    let tibia_extension_z = TIBIA * to_target_z / dist_to_target;
    
    let x_end: f64 = x_tibia + tibia_extension_x;
    let y_end: f64 = y_tibia + tibia_extension_y;
    let z_end: f64 = z_tibia + tibia_extension_z;

    println!("\nCoordonnées des articulations :");
    println!(
        "Coxa   : X: {:.2} | Y: {:.2} | Z: {:.2}",
        x_coxa, y_coxa, z_coxa
    );
    println!(
        "Fémur  : X: {:.2} | Y: {:.2} | Z: {:.2}",
        x_femur, y_femur, z_femur
    );
    println!(
        "Tibia  : X: {:.2} | Y: {:.2} | Z: {:.2}",
        x_tibia, y_tibia, z_tibia
    );
    println!(
        "Cible calculée : X: {:.2} | Y: {:.2} | Z: {:.2}",
        x_end, y_end, z_end
    );
    println!("Cible visée    : X: {:.2} | Y: {:.2} | Z: {:.2}", x, y, z);


    // Vérification: distances calculées vs longueurs théoriques des segments
    println!("\nComparatif des mesures (coordonnées vs longueurs réelles) :");
    println!(
        "Coxa   : Coordonnées (X-Y): {:.2} cm | Longueur réelle c'': {:.2} cm",
        ((x_femur - x_coxa).powi(2) + (y_femur - y_coxa).powi(2) + (z_femur - z_coxa).powi(2)).sqrt(),
        COXA
    );
    println!(
        "Fémur  : Coordonnées (X-Y): {:.2} cm | Longueur réelle f'': {:.2} cm",
        ((x_tibia - x_femur).powi(2) + (y_tibia - y_femur).powi(2) + (z_tibia - z_femur).powi(2)).sqrt(),
        FEMUR
    );
    println!(
        "Tibia  : Coordonnées (X-Y): {:.2} cm | Longueur réelle t'': {:.2} cm",
        ((x_end - x_tibia).powi(2) + (y_end - y_tibia).powi(2) + (z_end - z_tibia).powi(2)).sqrt(),
        TIBIA
    );
    println!(
        "Cible  : Coordonnées (X-Y-Z): {:.2} cm; {:.2} cm; {:.2} cm | Cible réelle (X-Y-Z): {} cm; {} cm; {} cm",
        x_end, y_end, z_end,
        x, y, z
    );

    // Génération des visualisations 2D
    if let Err(e) = visualize_leg_top_view(
        (x_coxa, y_coxa),
        (x_femur, y_femur),
        (x_tibia, y_tibia),
        (x_end, y_end),
    ) {
        println!("Erreur lors de la génération de la vue du ciel : {}", e);
    }
    
    if let Err(e) = visualize_leg(
        (x_coxa, z_coxa),
        (x_femur, z_femur),
        (x_tibia, z_tibia),
        (x_end, z_end),
    ) {
        println!("Erreur lors de la génération de la visualisation : {}", e);
    }
}

fn main() {
    println!("=== Programme de calcul -> trygonométrie ===");

    let x = 20.0;
    let y = 0.0;
    let z = -4.0;

    calcul(x, y, z);
}