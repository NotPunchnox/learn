mod visualisation;
use crate::visualisation::{visualize_leg, visualize_leg_top_view, MatrixPoint};

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

    // Créer la structure MatrixPoint avec toutes les coordonnées
    let points = MatrixPoint::new(
        (x_coxa, y_coxa, z_coxa),
        (x_femur, y_femur, z_femur),
        (x_tibia, y_tibia, z_tibia),
        (x_end, y_end, z_end),
    );

    // Afficher un résumé complet (coordonnées + longueurs calculées)
    points.print_summary();

    // Génération des visualisations 2D
    if let Err(e) = visualize_leg_top_view(points.clone()) {
        println!("Erreur lors de la génération de la vue du ciel : {}", e);
        return;
    }
    
    if let Err(e) = visualize_leg(points) {
        println!("Erreur lors de la génération de la visualisation : {}", e);
    }
}

fn main() {
    println!("=== Programme de calcul -> trygonométrie ===");

    let x = 20.0;
    let y = -5.0;
    let z = -5.0;

    calcul(x, y, z);
}