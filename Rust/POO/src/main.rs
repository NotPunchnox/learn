// Importer la classe Personne
mod classes;
use classes::personne::Personne;

// Fonction principale
fn main() {
    let mut personne = Personne::new(String::from("Punchnox"), 20);
    personne.afficher();
}
