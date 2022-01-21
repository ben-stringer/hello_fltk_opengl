use crate::ui::view_state::ViewState;
use fltk::app::{App, Receiver};
use fltk::window::GlWindow;
use fltk::{prelude::*, *};
use std::borrow::{Borrow, BorrowMut};
use std::cell::{RefCell, RefMut};
use std::rc::Rc;

pub struct GuiApp {
    app: App,
    wind: GlWindow,
    rec: Receiver<i32>,
    state: Rc<RefCell<ViewState>>,
}

impl GuiApp {
    pub fn new() -> Self {
        let app = App::default();
        let mut wind = GlWindow::new(100, 100, 800, 600, "Hello FLTK OpenGL");

        let state = Rc::from(RefCell::from(ViewState::default()));

        let mut state_rc = state.clone();
        wind.draw(move |_| {
            log::debug!("Inside draw()");
            let view_state = state_rc.as_ref().borrow();
            view_state.draw();
        });

        wind.end();
        wind.show();

        let (src, rec) = fltk::app::channel::<i32>();

        wind.handle(move |unk, msg| {
            src.send(0);
            true
        });

        Self {
            app,
            wind,
            rec,
            state,
        }
    }

    pub fn state(&mut self) -> RefMut<ViewState> {
        self.state.as_ref().borrow_mut()
    }

    pub fn run(&self) {
        while self.app.wait() {
            // match self.rec.recv() {
            //     Some(coords) => {
            //         let rand: f32 = ((coords.0 - W / 2) * (coords.1 - H / 2) / 360) as f32;
            //         *rotangle.borrow_mut() += rand;
            //         wind.redraw();
            //     }
            //     None => (),
            // }
        }
    }
}
