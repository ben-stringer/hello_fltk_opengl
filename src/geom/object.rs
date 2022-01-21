use crate::geom::bbox::BoundingBox;
use crate::geom::face::Face;
use crate::geom::point::Point;
use crate::geom::vector::Vector;
use crate::geom::vertex::Vertex;
use scan_fmt::parse::ScanError;
use scan_fmt::scan_fmt;
use std::error::Error;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::path::Path;

pub struct Object {
    vertices: Vec<Vertex>,
    faces: Vec<Face>,
    bbox: BoundingBox,
}

impl Object {
    pub fn open<P>(path: P) -> Result<Self, Box<dyn Error>>
    where
        P: AsRef<Path>,
    {
        let file = BufReader::new(File::open(path)?);

        let mut vertices = Vec::new();
        let mut faces = Vec::new();

        for line in file.lines() {
            if let Ok(text) = line {
                if let Some(ch) = text.chars().nth(0) {
                    match ch {
                        '#' => {}
                        'V' => match Object::parse_vertex(&text) {
                            Ok(v) => vertices.push(v),
                            Err(e) => log::warn!(
                                "Failed to parse vertex text '{}' due to error {}.",
                                &text,
                                e
                            ),
                        },
                        'F' => match Object::parse_face(&text) {
                            Ok(f) => faces.push(f),
                            Err(e) => log::warn!(
                                "Failed to parse face text '{}' due to error: {}.",
                                &text,
                                e
                            ),
                        },
                        _ => log::warn!("Unrecognized line in input file: {}", text),
                    }
                }
            }
        }

        let bbox = BoundingBox::bounding(&vertices);

        log::info!("File opened, num faces: {}", faces.len());

        Ok(Self {
            vertices,
            faces,
            bbox,
        })
    }

    pub fn bbox(&self) -> &BoundingBox {
        &self.bbox
    }

    fn parse_vertex(text: &str) -> Result<Vertex, ScanError> {
        scan_fmt!(
            text,
            "Vertex {}  {} {} {} {{normal=({} {} {})}}",
            usize,
            f32,
            f32,
            f32,
            f32,
            f32,
            f32
        )
        .map(|(id, px, py, pz, nx, ny, nz)| Vertex {
            point: Point::new(px, py, pz, 0.0),
            normal: Vector::new(nx, ny, nz, 0.0),
        })
    }

    fn parse_face(text: &str) -> Result<Face, ScanError> {
        scan_fmt!(text, "Face {} {} {} {}", usize, usize, usize, usize)
            .map(|(id, v1, v2, v3)| (v1-1, v2-1, v3-1))
            .map(|(v1, v2, v3)| Face { v1, v2, v3 })
    }

    pub fn draw(&self) {
        log::debug!("Object::draw");
        unsafe {
            glu_sys::glBegin(glu_sys::GL_TRIANGLES);
            for face in &self.faces {
                let v1 = &self.vertices[face.v1];
                let v2 = &self.vertices[face.v2];
                let v3 = &self.vertices[face.v3];

                glu_sys::glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
                glu_sys::glVertex3f(v1.point.x, v1.point.y, v1.point.z);
                glu_sys::glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
                glu_sys::glVertex3f(v2.point.x, v2.point.y, v2.point.z);
                glu_sys::glNormal3f(v3.normal.x, v3.normal.y, v3.normal.z);
                glu_sys::glVertex3f(v3.point.x, v3.point.y, v3.point.z);
            }
            glu_sys::glEnd();
        }
        log::debug!("Object::draw exiting");
    }
}
