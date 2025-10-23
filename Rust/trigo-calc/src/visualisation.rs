use plotters::prelude::*;

#[derive(Clone, Debug)]
pub struct MatrixPoint {
    pub coxa: (f64, f64, f64),
    pub femur: (f64, f64, f64),
    pub tibia: (f64, f64, f64),
    pub end: (f64, f64, f64),
}

impl MatrixPoint {
    /// MatrixPoint avec toutes les coordonnées
    pub fn new(
        coxa: (f64, f64, f64),
        femur: (f64, f64, f64),
        tibia: (f64, f64, f64),
        end: (f64, f64, f64),
    ) -> Self {
        Self {
            coxa,
            femur,
            tibia,
            end,
        }
    }

    /// Calcule la longueur du segment Coxa (de coxa à femur)
    pub fn coxa_length(&self) -> f64 {
        let dx = self.femur.0 - self.coxa.0;
        let dy = self.femur.1 - self.coxa.1;
        let dz = self.femur.2 - self.coxa.2;
        (dx * dx + dy * dy + dz * dz).sqrt()
    }

    /// Calcule la longueur du segment Fémur (de femur à tibia)
    pub fn femur_length(&self) -> f64 {
        let dx = self.tibia.0 - self.femur.0;
        let dy = self.tibia.1 - self.femur.1;
        let dz = self.tibia.2 - self.femur.2;
        (dx * dx + dy * dy + dz * dz).sqrt()
    }

    /// Calcule la longueur du segment Tibia (de tibia à end)
    pub fn tibia_length(&self) -> f64 {
        let dx = self.end.0 - self.tibia.0;
        let dy = self.end.1 - self.tibia.1;
        let dz = self.end.2 - self.tibia.2;
        (dx * dx + dy * dy + dz * dz).sqrt()
    }

    /// Calcule la longueur totale de la patte
    pub fn total_length(&self) -> f64 {
        self.coxa_length() + self.femur_length() + self.tibia_length()
    }

    /// Affiche toutes les coordonnées des articulations
    pub fn print_coordinates(&self) {
        println!("\n=== Coordonnées des articulations (MatrixPoint) ===");
        println!("Coxa   : X: {:.2} | Y: {:.2} | Z: {:.2}", 
                 self.coxa.0, self.coxa.1, self.coxa.2);
        println!("Fémur  : X: {:.2} | Y: {:.2} | Z: {:.2}", 
                 self.femur.0, self.femur.1, self.femur.2);
        println!("Tibia  : X: {:.2} | Y: {:.2} | Z: {:.2}", 
                 self.tibia.0, self.tibia.1, self.tibia.2);
        println!("Fin    : X: {:.2} | Y: {:.2} | Z: {:.2}", 
                 self.end.0, self.end.1, self.end.2);
    }

    /// Affiche un résumé des longueurs des segments
    pub fn print_lengths(&self) {
        println!("\n=== Longueurs des segments calculées ===");
        println!("Coxa   : {:.2} cm", self.coxa_length());
        println!("Fémur  : {:.2} cm", self.femur_length());
        println!("Tibia  : {:.2} cm", self.tibia_length());
        println!("Total  : {:.2} cm", self.total_length());
    }

    /// Affiche un résumé complet (coordonnées + longueurs)
    pub fn print_summary(&self) {
        self.print_coordinates();
        self.print_lengths();
    }
}

pub fn visualize_leg(points: MatrixPoint) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("leg_visualization.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;

    // Extraction des coordonnées pour la vue de profil (plan X-Z)
    let coxa_pos = (points.coxa.0, points.coxa.2);
    let femur_pos = (points.femur.0, points.femur.2);
    let tibia_pos = (points.tibia.0, points.tibia.2);
    let target_pos = (points.end.0, points.end.2);

    let mut chart = ChartBuilder::on(&root)
        .caption(
            "Visualisation 2D de la patte robotique (vue de profil X-Z)",
            ("sans-serif", 30),
        )
        .margin(20)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(-5.0..25.0, -10.0..15.0)?;

    chart.configure_mesh().draw()?;

    // Dessiner les segments en utilisant les coordonnées réelles des articulations
    chart
        .draw_series(LineSeries::new(vec![coxa_pos, femur_pos], &RED))?
        .label("Coxa")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &RED));

    chart
        .draw_series(LineSeries::new(vec![femur_pos, tibia_pos], &BLUE))?
        .label("Fémur")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &BLUE));

    chart
        .draw_series(LineSeries::new(vec![tibia_pos, target_pos], &GREEN))?
        .label("Tibia")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &GREEN));

    // Calculer les longueurs des segments
    let coxa_length = points.coxa_length();
    let femur_length = points.femur_length();
    let tibia_length = points.tibia_length();

    // Afficher les longueurs des segments
    chart.draw_series(std::iter::once(Text::new(
        format!("Coxa: {:.2} cm", coxa_length),
        (
            (coxa_pos.0 + femur_pos.0) / 2.0,
            (coxa_pos.1 + femur_pos.1) / 2.0 - 1.0,
        ),
        ("sans-serif", 12),
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Fémur: {:.2} cm", femur_length),
        (
            (femur_pos.0 + tibia_pos.0) / 2.0,
            (femur_pos.1 + tibia_pos.1) / 2.0 + 1.0,
        ),
        ("sans-serif", 12),
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Tibia: {:.2} cm", tibia_length),
        (
            (tibia_pos.0 + target_pos.0) / 2.0,
            (tibia_pos.1 + target_pos.1) / 2.0 + 1.0,
        ),
        ("sans-serif", 12),
    )))?;

    // Dessiner les points des articulations
    chart.draw_series(PointSeries::of_element(
        vec![coxa_pos, femur_pos, tibia_pos, target_pos],
        5,
        &BLACK,
        &|c, s, st| Circle::new(c, s, st.filled()),
    ))?;

    chart.configure_series_labels().draw()?;
    root.present()?;

    println!("Visualisation sauvegardée dans 'leg_visualization.png'");
    Ok(())
}

// Génère la vue du ciel (top view) de la patte robotique
pub fn visualize_leg_top_view(
    points: MatrixPoint
) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("leg_visualization_top.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;

    // Extraction des coordonnées pour la vue du ciel (plan X-Y)
    let coxa_pos = (points.coxa.0, points.coxa.1);
    let femur_pos = (points.femur.0, points.femur.1);
    let tibia_pos = (points.tibia.0, points.tibia.1);
    let target_pos = (points.end.0, points.end.1);

    let mut chart = ChartBuilder::on(&root)
        .caption(
            "Vue du ciel de la patte robotique (plan X-Y)",
            ("sans-serif", 30),
        )
        .margin(20)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(0.0..25.0, -10.0..15.0)?;

    chart.configure_mesh().draw()?;

    // Dessiner les segments en utilisant les coordonnées réelles des articulations
    chart
        .draw_series(LineSeries::new(vec![coxa_pos, femur_pos], &RED))?
        .label("Coxa")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &RED));

    chart
        .draw_series(LineSeries::new(vec![femur_pos, tibia_pos], &BLUE))?
        .label("Fémur (proj.)")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &BLUE));

    chart
        .draw_series(LineSeries::new(vec![tibia_pos, target_pos], &GREEN))?
        .label("Tibia (proj.)")
        .legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &GREEN));

    // Calculer les longueurs des projections (XY plane)
    let coxa_proj =
        ((femur_pos.0 - coxa_pos.0).powi(2) + (femur_pos.1 - coxa_pos.1).powi(2)).sqrt();
    let femur_proj =
        ((tibia_pos.0 - femur_pos.0).powi(2) + (tibia_pos.1 - femur_pos.1).powi(2)).sqrt();
    let tibia_proj =
        ((target_pos.0 - tibia_pos.0).powi(2) + (target_pos.1 - tibia_pos.1).powi(2)).sqrt();

    // Calculer les longueurs réelles 3D
    let coxa_length = points.coxa_length();
    let femur_length = points.femur_length();
    let tibia_length = points.tibia_length();

    // Afficher les longueurs des projections et réelles
    chart.draw_series(std::iter::once(Text::new(
        format!("Coxa proj: {:.2} cm (3D: {:.2} cm)", coxa_proj, coxa_length),
        (
            (coxa_pos.0 + femur_pos.0) / 2.0,
            (coxa_pos.1 + femur_pos.1) / 2.0 - 0.5,
        ),
        ("sans-serif", 12),
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Fémur proj: {:.2} cm (3D: {:.2} cm)", femur_proj, femur_length),
        (
            (femur_pos.0 + tibia_pos.0) / 2.0,
            (femur_pos.1 + tibia_pos.1) / 2.0 + 0.5,
        ),
        ("sans-serif", 12),
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Tibia proj: {:.2} cm (3D: {:.2} cm)", tibia_proj, tibia_length),
        (
            (tibia_pos.0 + target_pos.0) / 2.0,
            (tibia_pos.1 + target_pos.1) / 2.0 + 0.5,
        ),
        ("sans-serif", 12),
    )))?;

    // Dessiner les points des articulations
    chart.draw_series(PointSeries::of_element(
        vec![coxa_pos, femur_pos, tibia_pos, target_pos],
        5,
        &BLACK,
        &|c, s, st| Circle::new(c, s, st.filled()),
    ))?;

    // Ajouter des labels pour les points
    chart.draw_series(std::iter::once(Text::new(
        "Origine",
        (coxa_pos.0 - 1.0, coxa_pos.1 - 1.0),
        ("sans-serif", 10),
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        "Fémur",
        (femur_pos.0 - 1.0, femur_pos.1 - 1.0),
        ("sans-serif", 10),
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        "Tibia",
        (tibia_pos.0 - 1.0, tibia_pos.1 - 1.0),
        ("sans-serif", 10),
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        "Cible",
        (target_pos.0 - 1.0, target_pos.1 - 1.0),
        ("sans-serif", 10),
    )))?;

    // Calculer et afficher l'angle theta1
    let theta1 = femur_pos.1.atan2(femur_pos.0);
    chart.draw_series(std::iter::once(Text::new(
        format!("θ1 = {:.1}°", theta1.to_degrees()),
        (1.0, 1.0),
        ("sans-serif", 15),
    )))?;

    chart.configure_series_labels().draw()?;
    root.present()?;

    println!("Vue du ciel sauvegardée dans 'leg_visualization_top.png'");
    Ok(())
}
