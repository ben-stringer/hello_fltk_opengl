use crate::geom::object::Object;
use std::error::Error;
use std::path::PathBuf;

#[derive(Default)]
pub struct ViewState {
    draw_axis: bool,
    draw_bounds: bool,
    obj: Option<Object>,
}

impl ViewState {
    pub fn reset(&mut self) {
        self.draw_axis = false;
        self.draw_bounds = false;
    }

    pub fn open(&mut self, file: &PathBuf) {
        self.obj = match Object::open(file.to_owned()) {
            Ok(obj) => Some(obj),
            Err(e) => {
                log::warn!("Could not open file {:?} due to error: {}", file, e);
                None
            }
        };
    }

    pub fn draw(&self) {
        log::debug!("ViewState::draw()");
        unsafe {
            glu_sys::glClearColor(0., 0., 0., 0.);
            glu_sys::glEnable(glu_sys::GL_DEPTH_TEST);
            glu_sys::glEnable(glu_sys::GL_NORMALIZE);
            glu_sys::glLineWidth(2.);
            glu_sys::glLightModelf(glu_sys::GL_LIGHT_MODEL_AMBIENT, 0.);
            glu_sys::glLightModeli(glu_sys::GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
        }

        if self.draw_axis {
            log::debug!("ViewState::draw().draw_axis");
            self.draw_axis();
        }

        if let Some(obj) = &self.obj {
            if self.draw_bounds {
                log::debug!("ViewState::draw().draw_bounds");
                obj.bbox().draw();
            }

            obj.draw();
        }
        log::debug!("ViewState::draw() exiting");
    }

    fn draw_axis(&self) {
        unsafe {
            glu_sys::glBegin(glu_sys::GL_LINES);
            glu_sys::glColor3f(1., 0., 0.);
            glu_sys::glVertex3f(-10., 0., 0.);
            glu_sys::glVertex3f(10., 0., 0.);
            glu_sys::glColor3f(0., 1., 0.);
            glu_sys::glVertex3f(0., -10., 0.);
            glu_sys::glVertex3f(0., 10., 0.);
            glu_sys::glColor3f(0., 0., 1.);
            glu_sys::glVertex3f(0., 0., -10.);
            glu_sys::glVertex3f(0., 0., 10.);
            //Draw a unit vector
            glu_sys::glColor3f(1., 1., 0.);
            glu_sys::glVertex3f(0., 0., 0.);
            glu_sys::glVertex3f(1., 0., 0.);
            glu_sys::glEnd();
        }
    }
}
