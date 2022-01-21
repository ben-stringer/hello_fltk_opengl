use crate::geom::point::Point;
use crate::geom::vertex::Vertex;

#[derive(Default)]
pub struct BoundingBox {
    pub minP: Point,
    pub maxP: Point,
}

impl BoundingBox {
    pub fn bounding(vertexes: &Vec<Vertex>) -> Self {
        let mut minx = f32::MAX;
        let mut miny = f32::MAX;
        let mut minz = f32::MAX;
        let mut maxx = -f32::MAX;
        let mut maxy = -f32::MAX;
        let mut maxz = -f32::MAX;
        for v in vertexes {
            let p = &v.point;
            minx = f32::min(minx, p.x);
            miny = f32::min(miny, p.y);
            minz = f32::min(minz, p.z);
            maxx = f32::max(maxx, p.x);
            maxy = f32::max(maxy, p.y);
            maxz = f32::max(maxz, p.z);
        }
        BoundingBox {
            minP: Point {
                x: minx,
                y: miny,
                z: minz,
                w: 0.0,
            },
            maxP: Point {
                x: maxx,
                y: maxy,
                z: maxz,
                w: 0.0,
            },
        }
    }

    pub fn draw(&self) {
        unsafe {
            glu_sys::glBegin(glu_sys::GL_LINE_LOOP);
            glu_sys::glVertex3f(self.minP.x, self.minP.y, self.minP.z);
            glu_sys::glVertex3f(self.maxP.x, self.minP.y, self.minP.z);
            glu_sys::glVertex3f(self.maxP.x, self.maxP.y, self.minP.z);
            glu_sys::glVertex3f(self.minP.x, self.maxP.y, self.minP.z);
            glu_sys::glEnd();

            glu_sys::glBegin(glu_sys::GL_LINE_LOOP);
            glu_sys::glVertex3f(self.minP.x, self.minP.y, self.maxP.z);
            glu_sys::glVertex3f(self.maxP.x, self.minP.y, self.maxP.z);
            glu_sys::glVertex3f(self.maxP.x, self.maxP.y, self.maxP.z);
            glu_sys::glVertex3f(self.minP.x, self.maxP.y, self.maxP.z);
            glu_sys::glEnd();

            glu_sys::glBegin(glu_sys::GL_LINES);

            glu_sys::glVertex3f(self.minP.x, self.minP.y, self.minP.z);
            glu_sys::glVertex3f(self.minP.x, self.minP.y, self.maxP.z);

            glu_sys::glVertex3f(self.maxP.x, self.minP.y, self.minP.z);
            glu_sys::glVertex3f(self.maxP.x, self.minP.y, self.maxP.z);

            glu_sys::glVertex3f(self.maxP.x, self.maxP.y, self.minP.z);
            glu_sys::glVertex3f(self.maxP.x, self.maxP.y, self.maxP.z);

            glu_sys::glVertex3f(self.minP.x, self.maxP.y, self.minP.z);
            glu_sys::glVertex3f(self.minP.x, self.maxP.y, self.maxP.z);

            glu_sys::glEnd();
        }
    }
}
