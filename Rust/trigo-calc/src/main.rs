mod visualisation;
use crate::visualisation::{visualize_leg, visualize_leg_top_view};

fn calcul (x : f64, y: f64, z: f64) {

    let coxa: f64 = 5.0;
    let femur: f64 = 6.3;
    let tibia: f64 = 13.0;

    let tpatte = (x*x + y*y).sqrt();

    println!("Coordonnées de la cible : X: {:.2} cm | Y: {:.2} cm | Z: {:.2} cm", x, y, z);
    println!("Longueurs des segments de la patte : Coxa: {:.2} cm | Fémur: {:.2} cm | Tibia: {:.2} cm", coxa, femur, tibia);
    println!("Taille de la patte (projection vu de haut) : {:.2} cm\n", tpatte);

    let h = (z*z + (x-coxa) * (x-coxa)).sqrt();

    let theta1 = y.atan2(x);
    let theta2 = (tpatte - coxa).atan2(z);
    let theta3_theta4 = ((femur*femur + h*h - tibia*tibia) / (2.0 * femur * h)).acos();

    // Séparer theta3 et theta4 en soustrayant theta2 et theta3-4 de 180 pour obtenir theta7
    // Obtenir theta4 en retirant theta7 de 90 degrés
    let theta7 = 180.0_f64.to_radians() - (theta3_theta4 + theta2);
    let theta4 = 90.0_f64.to_radians() - theta7;
    let theta3 = theta3_theta4 - theta4;
    let theta5 = ((femur*femur + tibia*tibia - h*h) / (2.0 * femur * tibia)).acos();

    // Afficher les angles calculés
    println!("Angles calculés (en degrés) :");
    println!("Theta 1 (Coxa)  : {:.2}°", theta1.to_degrees());
    println!("Theta 2 (Fémur)  : {:.2}°", theta2.to_degrees());
    println!("Theta 3 (Fémur) : {:.2}°", theta3.to_degrees());
    println!("Theta 4 (Tibia) : {:.2}°", theta4.to_degrees());
    println!("Theta 5 (Tibia) : {:.2}°", theta5.to_degrees());

    // Calcul des longueurs des vecteurs ( vue du ciel ) (c', f', t')
    let c_prime = coxa;
    let f_prime = femur * theta4.to_radians().cos();
    let t_prime = x - (c_prime + f_prime);

    println!("\nLongueurs des projections des segments de la patte (vue du ciel) :");
    println!("Coxa (c')  : {:.2} cm", c_prime);
    println!("Fémur (f') : {:.2} cm", f_prime);
    println!("Tibia (t') : {:.2} cm", t_prime);

    // Calcul des coordonnées des articulations
    let x_coxa = 0.0;
    let y_coxa = 0.0;
    let z_coxa = 0.0;

    let x_femur = coxa * theta1.cos();
    let y_femur = coxa * theta1.sin();
    let z_femur = 0.0;

    let x_tibia = x_femur + femur * theta4.cos() * theta1.cos();
    let y_tibia = y_femur + femur * theta4.cos() * theta1.sin();
    let z_tibia = femur * theta4.sin();

    println!("\nCoordonnées des articulations :");
    println!("Coxa    : ({:.2}, {:.2}, {:.2})", x_coxa, y_coxa, z_coxa);
    println!("Fémur   : ({:.2}, {:.2}, {:.2})", x_femur, y_femur, z_femur);
    println!("Tibia   : ({:.2}, {:.2}, {:.2})", x_tibia, y_tibia, z_tibia);
    println!("Cible   : ({:.2}, {:.2}, {:.2})", x, y, z);

    // Générer la vue du ciel (x;y) avec les coordonnées précises
    if let Err(e) = visualize_leg_top_view(
        x, y, z, 
        x_coxa, y_coxa, 
        x_femur, y_femur, 
        x_tibia, y_tibia, 
        theta1
    ) {
        println!("Erreur lors de la génération de la vue du ciel : {}", e);
    }
    
    // Générer la visualisation de profile (x;z) avec les coordonnées précises
    if let Err(e) = visualize_leg(
        x, y, z, 
        x_coxa, z_coxa, 
        x_femur, z_femur, 
        x_tibia, z_tibia, 
        theta1, theta2, theta4
    ) {
        println!("Erreur lors de la génération de la visualisation : {}", e);
    }

}

fn main() {
    println!("=== Programme de calcul -> trygonométrie ===");

    let x = 20.0;
    let y = 4.0;
    let z = -8.0;

    calcul(x, y, z);
}