use crate::geom::point::Point;

#[derive(Default)]
pub struct Vector {
    pub x: f32,
    pub y: f32,
    pub z: f32,
    pub w: f32,
}

impl Vector {
    pub fn new(x: f32, y: f32, z: f32, w: f32) -> Self {
        Self { x, y, z, w }
    }

    pub fn dot(&self, that: &Self) -> f32 {
        self.x * that.x + self.y * that.y + self.z * that.z
    }

    pub fn cross(&self, that: &Self) -> Self {
        Self {
            x: self.y * that.z - that.y * self.z,
            y: -(self.x * that.z - that.x * self.z),
            z: self.x * that.y - that.x * self.y,
            w: 0.0,
        }
    }

    pub fn length(&self) -> f32 {
        f32::sqrt(self.x.powi(2) + self.y.powi(2) + self.z.powi(2))
    }

    pub fn normalize(&mut self) {
        let len = self.length();
        self.x *= len;
        self.y *= len;
        self.z *= len;
        self.w *= len;
    }

    pub fn to_normalized(&self) -> Self {
        let len = self.length();
        Self {
            x: self.x * len,
            y: self.y * len,
            z: self.z * len,
            w: self.w * len,
        }
    }
}

impl From<Point> for Vector {
    fn from(p: Point) -> Self {
        Self {
            x: p.x,
            y: p.y,
            z: p.z,
            w: p.w,
        }
    }
}

impl From<[f32; 4]> for Vector {
    fn from(p: [f32; 4]) -> Self {
        Self {
            x: p[0],
            y: p[1],
            z: p[2],
            w: p[3],
        }
    }
}

impl std::ops::Add<&Vector> for Vector {
    type Output = Vector;

    fn add(self, rhs: &Vector) -> Self::Output {
        Self {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
            z: self.z + rhs.z,
            w: self.w + rhs.w,
        }
    }
}

impl std::ops::Mul<f32> for Vector {
    type Output = Vector;

    fn mul(self, rhs: f32) -> Self::Output {
        Self {
            x: self.x * rhs,
            y: self.y * rhs,
            z: self.z * rhs,
            w: self.w * rhs,
        }
    }
}
