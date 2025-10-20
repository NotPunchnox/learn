pub struct Personne {
    nom: String,
    age: u32,
}

impl Personne {

    // Constructeur
    pub fn new(nom: String, age: u32) -> Self {
        Self { nom, age }
    }

    // Méthode pour afficher les infos
    pub fn afficher(&self) {
        println!("Je m'appelle {} et j'ai {} ans.", self.nom, self.age);
    }

    // Méthode qui modifie l'instance
    pub fn anniversaire(&mut self) {
        self.age += 1;
    }

}