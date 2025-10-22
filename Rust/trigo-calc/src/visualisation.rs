use plotters::prelude::*;

pub fn visualize_leg(
    target_x: f64, target_y: f64, target_z: f64,
    coxa_x: f64, coxa_z: f64,
    femur_x: f64, femur_z: f64,
    tibia_x: f64, tibia_z: f64,
    theta1: f64, theta2: f64, theta4: f64
) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("leg_visualization.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let mut chart = ChartBuilder::on(&root)
        .caption("Visualisation 2D de la patte robotique (profil X-Z)", ("sans-serif", 30))
        .margin(20)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(-5.0..25.0, -5.0..15.0)?;

    chart.configure_mesh().draw()?;

    // Points de la patte avec les coordonnées réelles
    let origin = (coxa_x, coxa_z);
    let coxa_end = (femur_x, femur_z);
    let femur_end = (tibia_x, tibia_z);
    let target = (target_x, target_z);

    // Dessiner les segments
    chart.draw_series(LineSeries::new(
        vec![origin, coxa_end], &RED
    ))?.label("Coxa").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &RED));

    chart.draw_series(LineSeries::new(
        vec![coxa_end, femur_end], &BLUE
    ))?.label("Fémur").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &BLUE));

    chart.draw_series(LineSeries::new(
        vec![femur_end, target], &GREEN
    ))?.label("Tibia").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &GREEN));

    // Calculer les longueurs réelles des segments
    let coxa_length = ((coxa_end.0 - origin.0).powi(2) + (coxa_end.1 - origin.1).powi(2)).sqrt();
    let femur_length = ((femur_end.0 - coxa_end.0).powi(2) + (femur_end.1 - coxa_end.1).powi(2)).sqrt();
    let tibia_length = ((target.0 - femur_end.0).powi(2) + (target.1 - femur_end.1).powi(2)).sqrt();

    // Afficher les longueurs des segments
    chart.draw_series(std::iter::once(Text::new(
        format!("Coxa: {:.2} cm", coxa_length),
        ((origin.0 + coxa_end.0) / 2.0, (origin.1 + coxa_end.1) / 2.0 - 1.0),
        ("sans-serif", 15)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Fémur: {:.2} cm", femur_length),
        ((coxa_end.0 + femur_end.0) / 2.0, (coxa_end.1 + femur_end.1) / 2.0 + 1.0),
        ("sans-serif", 15)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Tibia: {:.2} cm", tibia_length),
        ((femur_end.0 + target.0) / 2.0, (femur_end.1 + target.1) / 2.0 + 1.0),
        ("sans-serif", 15)
    )))?;

    // Afficher les angles
    chart.draw_series(std::iter::once(Text::new(
        format!("θ1 = {:.1}°", theta1.to_degrees()),
        (1.0, 10.0),
        ("sans-serif", 15).into_font().color(&BLACK)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("θ2 = {:.1}°", theta2.to_degrees()),
        (1.0, 8.5),
        ("sans-serif", 15).into_font().color(&BLACK)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("θ4 = {:.1}°", theta4.to_degrees()),
        (1.0, 7.0),
        ("sans-serif", 15).into_font().color(&BLACK)
    )))?;

    // Dessiner les points
    chart.draw_series(PointSeries::of_element(
        vec![origin, coxa_end, femur_end, target],
        5,
        &BLACK,
        &|c, s, st| Circle::new(c, s, st.filled())
    ))?;

    chart.configure_series_labels().draw()?;
    root.present()?;
    
    println!("Visualisation profil sauvegardée dans 'leg_visualization.png'");
    Ok(())
}

// Génère la vue du ciel (top view) de la patte robotique
pub fn visualize_leg_top_view(
    target_x: f64, target_y: f64, _target_z: f64,
    coxa_x: f64, coxa_y: f64,
    femur_x: f64, femur_y: f64,
    tibia_x: f64, tibia_y: f64,
    theta1: f64
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

    // Points de la patte avec les coordonnées réelles
    let origin = (coxa_x, coxa_y);
    let coxa_end = (femur_x, femur_y);
    let femur_end = (tibia_x, tibia_y);
    let target = (target_x, target_y);

    // Dessiner les segments avec les coordonnées précises
    chart.draw_series(LineSeries::new(
        vec![origin, coxa_end], &RED
    ))?.label("Coxa").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &RED));

    chart.draw_series(LineSeries::new(
        vec![coxa_end, femur_end], &BLUE
    ))?.label("Fémur (proj.)").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &BLUE));

    chart.draw_series(LineSeries::new(
        vec![femur_end, target], &GREEN
    ))?.label("Tibia (proj.)").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &GREEN));

    // Calculer les longueurs réelles des projections
    let coxa_length = ((coxa_end.0 - origin.0).powi(2) + (coxa_end.1 - origin.1).powi(2)).sqrt();
    let femur_proj_length = ((femur_end.0 - coxa_end.0).powi(2) + (femur_end.1 - coxa_end.1).powi(2)).sqrt();
    let tibia_proj_length = ((target.0 - femur_end.0).powi(2) + (target.1 - femur_end.1).powi(2)).sqrt();

    // Afficher les longueurs des projections
    chart.draw_series(std::iter::once(Text::new(
        format!("Coxa: {:.2} cm", coxa_length),
        ((origin.0 + coxa_end.0) / 2.0, (origin.1 + coxa_end.1) / 2.0 - 1.0),
        ("sans-serif", 15)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Fémur proj.: {:.2} cm", femur_proj_length),
        ((coxa_end.0 + femur_end.0) / 2.0, (coxa_end.1 + femur_end.1) / 2.0 + 1.0),
        ("sans-serif", 15)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Tibia proj.: {:.2} cm", tibia_proj_length),
        ((femur_end.0 + target.0) / 2.0, (femur_end.1 + target.1) / 2.0 + 1.0),
        ("sans-serif", 15)
    )))?;

    // Dessiner les points
    chart.draw_series(PointSeries::of_element(
        vec![origin, coxa_end, femur_end, target],
        5,
        &BLACK,
        &|c, s, st| Circle::new(c, s, st.filled())
    ))?;

    // Ajouter des annotations pour les angles
    chart.draw_series(std::iter::once(Text::new(
        format!("θ1 = {:.1}°", theta1.to_degrees()),
        (1.0, 12.0),
        ("sans-serif", 15).into_font().color(&BLACK)
    )))?;

    // Dessiner une ligne pour indiquer la direction de θ1
    let angle_line_end = (2.0 * theta1.cos(), 2.0 * theta1.sin());
    chart.draw_series(LineSeries::new(
        vec![origin, angle_line_end], &MAGENTA
    ))?.label("Direction θ1").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &MAGENTA));

    chart.configure_series_labels().draw()?;
    root.present()?;
    
    println!("Vue du ciel sauvegardée dans 'leg_visualization_top.png'");
    Ok(())
}