use fltk::{prelude::*, *};
use std::cell::RefCell;
use std::rc::Rc;

const W: i32 = 600;
const H: i32 = 400;

pub fn main() {
    let app = app::App::default();
    let mut wind = window::GlWindow::new(100, 100, W, H, "Rotate me!");

    wind.end();
    wind.show();

    let rotangle = Rc::from(RefCell::from(0.0));
    let rotangle_rc = rotangle.clone();

    wind.draw(move |_| draw_triangle(&rotangle_rc.borrow()));

    let (s, r) = app::channel::<(i32, i32)>();

    wind.handle(move |_, ev| match ev {
        enums::Event::Drag => {
            s.send(app::event_coords());
            true
        }
        _ => false,
    });

    while app.wait() {
        match r.recv() {
            Some(coords) => {
                let rand: f32 = ((coords.0 - W / 2) * (coords.1 - H / 2) / 360) as f32;
                *rotangle.borrow_mut() += rand;
                wind.redraw();
            }
            None => (),
        }
    }
}

fn draw_triangle(rotangle: &f32) {
    unsafe {
        glu_sys::glEnable(glu_sys::GL_DEPTH_TEST);
        glu_sys::glDepthFunc(glu_sys::GL_ALWAYS);
        glu_sys::glDepthFunc(glu_sys::GL_LEQUAL);
        glu_sys::glDepthRange(0.0, 1.0);
        glu_sys::glDepthMask(1);
        glu_sys::glClear(glu_sys::GL_COLOR_BUFFER_BIT | glu_sys::GL_DEPTH_BUFFER_BIT);
        glu_sys::glMatrixMode(glu_sys::GL_PROJECTION);
        glu_sys::glLoadIdentity();
        glu_sys::glViewport(0, 0, W, H);
        glu_sys::gluPerspective(45.0, (W as f32 / H as f32).into(), 1.0, 10.0);
        glu_sys::glTranslatef(0.0, 0.0, -5.0);
        glu_sys::glMatrixMode(glu_sys::GL_MODELVIEW);
        glu_sys::glLoadIdentity();
        glu_sys::glRotatef(*rotangle, 1.0, 1.0, 0.0);
        glu_sys::glColor3f(1.0, 0.0, 0.0);
        glu_sys::glBegin(glu_sys::GL_POLYGON);
        glu_sys::glVertex3f(0.0, 1.0, 0.0);
        glu_sys::glVertex3f(1.0, -1.0, 1.0);
        glu_sys::glVertex3f(-1.0, -1.0, 1.0);
        glu_sys::glEnd();
        glu_sys::glColor3f(0.0, 1.0, 0.0);
        glu_sys::glBegin(glu_sys::GL_POLYGON);
        glu_sys::glVertex3f(0.0, 1.0, 0.0);
        glu_sys::glVertex3f(0.0, -1.0, -1.0);
        glu_sys::glVertex3f(1.0, -1.0, 1.0);
        glu_sys::glEnd();
        glu_sys::glColor3f(0.0, 0.0, 1.0);
        glu_sys::glBegin(glu_sys::GL_POLYGON);
        glu_sys::glVertex3f(0.0, 1.0, 0.0);
        glu_sys::glVertex3f(-1.0, -1.0, 1.0);
        glu_sys::glVertex3f(0.0, -1.0, -1.0);
        glu_sys::glEnd();
        glu_sys::glColor3f(1.0, 0.0, 0.0);
        glu_sys::glBegin(glu_sys::GL_POLYGON);
        glu_sys::glVertex3f(1.0, -1.0, 1.0);
        glu_sys::glVertex3f(0.0, -1.0, -1.0);
        glu_sys::glVertex3f(-1.0, -1.0, 1.0);
        glu_sys::glEnd();
        glu_sys::glLoadIdentity();
        glu_sys::glRasterPos2f(-3.0, -2.0);
    }
}