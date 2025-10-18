fn main() {
    // test de println simple
    let name = "punchnox";
    println!("Hello, world!\n");
    println!("Nom: {}", name);

    // Test de variable (let x = constante)
    let mut test = "salut";
    println!("Variable test: {}", test);

    test = "salut ça va ?";
    println!("Variable test après modification: {}", test);

    const AGE: f64 = 25.60;
    println!("Age: {}", AGE);

    let name2: &str = "salut";
    println!("Nom2: {}", name2);

    // Opérateurs
    let mut x = 10;
    println!("Start: {}", x);

    x += 5;
    println!("After += 5: {}", x);

    x -= 2;
    println!("After -= 2: {}", x);

    x *= 2;
    println!("After *= 2: {}", x);

    x /= 3;
    println!("After /= 3: {}", x);

    x %= 4;
    println!("After %= 4: {}", x);

    // Conditions
    let mut y: i32 = 2;

    if y >= x {
        println!("{} <= {}", y, x);
    } else {
        y = x+1;
    }


    let test3 = if y > x {
        "y > x"
    } else {
        "y < x"
    };

    println!("résultat test3: {}", test3);
}
