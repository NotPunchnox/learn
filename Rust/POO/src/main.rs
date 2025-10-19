#[derive(Debug)]
struct Personne {
    nom: String,
    age: u32,
}

impl Personne {

    // Constructeur
    fn new(nom: String, age: u32) -> Self {
        Self { nom, age }
    }

    // Méthode pour afficher les infos
    fn afficher(&self) {
        println!("Je m'appelle {} et j'ai {} ans.", self.nom, self.age);
    }

    // Méthode qui modifie l'instance
    fn anniversaire(&mut self) {
        self.age += 1;
    }
}

fn main() {
    let mut personne = Personne::new(String::from("Punchnox"), 20);
    personne.afficher();
}
