use plotters::prelude::*;

pub fn visualize_leg(
    coxa_pos: (f64, f64), 
    femur_pos: (f64, f64), 
    tibia_pos: (f64, f64), 
    target_pos: (f64, f64)
) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("leg_visualization.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let mut chart = ChartBuilder::on(&root)
        .caption("Visualisation 2D de la patte robotique (vue de profil X-Z)", ("sans-serif", 30))
        .margin(20)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(-5.0..25.0, -10.0..15.0)?;

    chart.configure_mesh().draw()?;

    // Dessiner les segments en utilisant les coordonnées réelles des articulations
    chart.draw_series(LineSeries::new(
        vec![coxa_pos, femur_pos], &RED
    ))?.label("Coxa").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &RED));

    chart.draw_series(LineSeries::new(
        vec![femur_pos, tibia_pos], &BLUE
    ))?.label("Fémur").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &BLUE));

    chart.draw_series(LineSeries::new(
        vec![tibia_pos, target_pos], &GREEN
    ))?.label("Tibia").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &GREEN));

    // Calculer les longueurs des segments
    let coxa_length = ((femur_pos.0 - coxa_pos.0).powi(2) + (femur_pos.1 - coxa_pos.1).powi(2)).sqrt();
    let femur_length = ((tibia_pos.0 - femur_pos.0).powi(2) + (tibia_pos.1 - femur_pos.1).powi(2)).sqrt();
    let tibia_length = ((target_pos.0 - tibia_pos.0).powi(2) + (target_pos.1 - tibia_pos.1).powi(2)).sqrt();

    // Afficher les longueurs des segments
    chart.draw_series(std::iter::once(Text::new(
        format!("Coxa: {:.2} cm", coxa_length),
        ((coxa_pos.0 + femur_pos.0) / 2.0, (coxa_pos.1 + femur_pos.1) / 2.0 - 1.0),
        ("sans-serif", 12)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Fémur: {:.2} cm", femur_length),
        ((femur_pos.0 + tibia_pos.0) / 2.0, (femur_pos.1 + tibia_pos.1) / 2.0 + 1.0),
        ("sans-serif", 12)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Tibia: {:.2} cm", tibia_length),
        ((tibia_pos.0 + target_pos.0) / 2.0, (tibia_pos.1 + target_pos.1) / 2.0 + 1.0),
        ("sans-serif", 12)
    )))?;

    // Dessiner les points des articulations
    chart.draw_series(PointSeries::of_element(
        vec![coxa_pos, femur_pos, tibia_pos, target_pos],
        5,
        &BLACK,
        &|c, s, st| Circle::new(c, s, st.filled())
    ))?;


    chart.configure_series_labels().draw()?;
    root.present()?;
    
    println!("Visualisation sauvegardée dans 'leg_visualization.png'");
    Ok(())
}

// Génère la vue du ciel (top view) de la patte robotique
pub fn visualize_leg_top_view(
    coxa_pos: (f64, f64), 
    femur_pos: (f64, f64), 
    tibia_pos: (f64, f64), 
    target_pos: (f64, f64)
) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("leg_visualization_top.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let mut chart = ChartBuilder::on(&root)
        .caption("Vue du ciel de la patte robotique (plan X-Y)", ("sans-serif", 30))
        .margin(20)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(-5.0..25.0, -5.0..15.0)?;

    chart.configure_mesh().draw()?;

    // Dessiner les segments en utilisant les coordonnées réelles des articulations
    chart.draw_series(LineSeries::new(
        vec![coxa_pos, femur_pos], &RED
    ))?.label("Coxa").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &RED));

    chart.draw_series(LineSeries::new(
        vec![femur_pos, tibia_pos], &BLUE
    ))?.label("Fémur (proj.)").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &BLUE));

    chart.draw_series(LineSeries::new(
        vec![tibia_pos, target_pos], &GREEN
    ))?.label("Tibia (proj.)").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &GREEN));

    // Calculer les longueurs des projections
    let coxa_proj = ((femur_pos.0 - coxa_pos.0).powi(2) + (femur_pos.1 - coxa_pos.1).powi(2)).sqrt();
    let femur_proj = ((tibia_pos.0 - femur_pos.0).powi(2) + (tibia_pos.1 - femur_pos.1).powi(2)).sqrt();
    let tibia_proj = ((target_pos.0 - tibia_pos.0).powi(2) + (target_pos.1 - tibia_pos.1).powi(2)).sqrt();

    // Afficher les longueurs des projections
    chart.draw_series(std::iter::once(Text::new(
        format!("Coxa proj: {:.2} cm", coxa_proj),
        ((coxa_pos.0 + femur_pos.0) / 2.0, (coxa_pos.1 + femur_pos.1) / 2.0 - 0.5),
        ("sans-serif", 12)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Fémur proj: {:.2} cm", femur_proj),
        ((femur_pos.0 + tibia_pos.0) / 2.0, (femur_pos.1 + tibia_pos.1) / 2.0 + 0.5),
        ("sans-serif", 12)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Tibia proj: {:.2} cm", tibia_proj),
        ((tibia_pos.0 + target_pos.0) / 2.0, (tibia_pos.1 + target_pos.1) / 2.0 + 0.5),
        ("sans-serif", 12)
    )))?;

    // Dessiner les points des articulations
    chart.draw_series(PointSeries::of_element(
        vec![coxa_pos, femur_pos, tibia_pos, target_pos],
        5,
        &BLACK,
        &|c, s, st| Circle::new(c, s, st.filled())
    ))?;

    // Ajouter des labels pour les points
    chart.draw_series(std::iter::once(Text::new(
        "Origine",
        (coxa_pos.0 - 1.0, coxa_pos.1 - 1.0),
        ("sans-serif", 10)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        "Fémur",
        (femur_pos.0 - 1.0, femur_pos.1 - 1.0),
        ("sans-serif", 10)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        "Tibia",
        (tibia_pos.0 - 1.0, tibia_pos.1 - 1.0),
        ("sans-serif", 10)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        "Cible",
        (target_pos.0 - 1.0, target_pos.1 - 1.0),
        ("sans-serif", 10)
    )))?;

    // Calculer et afficher l'angle theta1 
    let theta1 = femur_pos.1.atan2(femur_pos.0);
    chart.draw_series(std::iter::once(Text::new(
        format!("θ1 = {:.1}°", theta1.to_degrees()),
        (1.0, 1.0),
        ("sans-serif", 15)
    )))?;

    chart.configure_series_labels().draw()?;
    root.present()?;
    
    println!("Vue du ciel sauvegardée dans 'leg_visualization_top.png'");
    Ok(())
}