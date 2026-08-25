// mod gui;
mod output;
// use cpal::traits::{DeviceTrait, HostTrait, StreamTrait};
// use cpal::{Device, FromSample, SampleFormat, SizedSample, StreamConfig};
// use fundsp::prelude::{
//     hammond_hz, multipass, reverb_stereo, sine, sine_hz, soft_saw_hz, square_hz, wave, AudioUnit,
//     Wave,
// };
// use output::run_output;
// use std::sync::Arc;

// /// This is the main function that is the entry point when we launch the
// /// binary, either directly or with `cargo run`.
// fn main() {
//     run_output();
// }
use eframe::egui;
use egui_file_dialog::FileDialog;
use std::path::PathBuf;

fn main() -> eframe::Result {
    // env_logger::init(); // Log to stderr (if you run with `RUST_LOG=debug`).
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([320.0, 240.0]),
        ..Default::default()
    };
    eframe::run_native(
        "My egui App",
        options,
        Box::new(|cc| {
            // This gives us image support:
            egui_extras::install_image_loaders(&cc.egui_ctx);

            Ok(Box::<MyApp>::default())
        }),
    )
}

struct MyApp {
    name: String,
    age: u32,
    file_dialog: FileDialog,
    picked_file: Option<PathBuf>,
}

impl Default for MyApp {
    fn default() -> Self {
        Self {
            name: "Arthur".to_owned(),
            age: 42,
            file_dialog: FileDialog::new()
                .show_new_folder_button(false)
                .show_search(false)
                .show_path_edit_button(false)
                .show_left_panel(false)
                .show_top_panel(true)
                .show_menu_button(false)
                .show_back_button(false)
                .show_forward_button(false)
                .show_parent_button(false)
                .show_current_path(true),
            picked_file: None,
        }
    }
}

impl eframe::App for MyApp {
    fn ui(&mut self, ui: &mut egui::Ui, _frame: &mut eframe::Frame) {
        egui::CentralPanel::default().show_inside(ui, |ui| {
            // File picker
            if ui.button("Picked file").clicked() {
                self.file_dialog.pick_file();
            }
            ui.label(format!("Picked file: {:?}", self.picked_file));

            if let Some(path) = self.file_dialog.update(ui).picked() {
                self.picked_file = Some(path.to_path_buf());
            }

            ui.heading("My egui Application");
            ui.horizontal(|ui| {
                let name_label = ui.label("Your name: ");
                ui.text_edit_singleline(&mut self.name)
                    .labelled_by(name_label.id);
            });
            ui.add(egui::Slider::new(&mut self.age, 0..=120).text("age"));
            if ui.button("Increment").clicked() {
                self.age += 1;
            }
            ui.label(format!("Hello '{}', age {}", self.name, self.age));
        });
    }
}
