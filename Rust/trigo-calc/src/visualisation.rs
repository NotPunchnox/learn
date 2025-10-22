use plotters::prelude::*;

pub fn visualize_leg(x: f64, y: f64, z: f64, coxa: f64, femur: f64, tibia: f64, theta1: f64, theta2: f64, theta4: f64) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("leg_visualization.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let mut chart = ChartBuilder::on(&root)
        .caption("Visualisation 2D de la patte robotique", ("sans-serif", 30))
        .margin(20)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(-10.0..25.0, -10.0..15.0)?;

    chart.configure_mesh().draw()?;

    // Points de la patte
    let origin = (0.0, 0.0);
    let coxa_end = (coxa, 0.0);
    let femur_end = (coxa + femur * theta4.cos(), femur * theta4.sin());
    let target = (x, z);

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

    // Afficher les longueurs des segments
    chart.draw_series(std::iter::once(Text::new(
        format!("Coxa: {:.2} cm", coxa),
        (coxa / 2.0, -1.0),
        ("sans-serif", 15)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Fémur: {:.2} cm", femur),
        ((coxa + femur_end.0) / 2.0, femur_end.1 / 2.0),
        ("sans-serif", 15)
    )))?;
    chart.draw_series(std::iter::once(Text::new(
        format!("Tibia: {:.2} cm", tibia),
        ((femur_end.0 + target.0) / 2.0, (femur_end.1 + target.1) / 2.0),
        ("sans-serif", 15)
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
    
    println!("Visualisation sauvegardée dans 'leg_visualization.png'");
    Ok(())
}

// Génère la vue du ciel (top view) de la patte robotique
pub fn visualize_leg_top_view(x: f64, y: f64, _z: f64, coxa: f64, femur: f64, _tibia: f64, theta1: f64, theta4: f64) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("leg_visualization_top.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let mut chart = ChartBuilder::on(&root)
        .caption("Vue du ciel de la patte robotique (plan X-Y)", ("sans-serif", 30))
        .margin(20)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(-5.0..15.0, -5.0..10.0)?;

    chart.configure_mesh().draw()?;

    // Projections horizontales des segments
    let origin = (0.0, 0.0);
    let coxa_end = (coxa * theta1.cos(), coxa * theta1.sin());
    
    // projection horizontale du fémur
    let f_prime = femur * theta4.cos();
    let femur_end = (coxa_end.0 + f_prime * theta1.cos(), coxa_end.1 + f_prime * theta1.sin());
    let target = (x, y);

    // Dessiner les segments projetés
    chart.draw_series(LineSeries::new(
        vec![origin, coxa_end], &RED
    ))?.label("Coxa").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &RED));

    chart.draw_series(LineSeries::new(
        vec![coxa_end, femur_end], &BLUE
    ))?.label("Fémur (proj.)").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &BLUE));

    chart.draw_series(LineSeries::new(
        vec![femur_end, target], &GREEN
    ))?.label("Tibia (proj.)").legend(|(x, y)| PathElement::new(vec![(x, y), (x + 10, y)], &GREEN));

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
        (1.0, 1.0),
        ("sans-serif", 15)
    )))?;

    chart.configure_series_labels().draw()?;
    root.present()?;
    
    println!("Vue du ciel sauvegardée dans 'leg_visualization_top.png'");
    Ok(())
}