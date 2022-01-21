use crate::geom::point::Point;
use crate::geom::vector::Vector;
use std::error::Error;
use std::fmt::{Display, Formatter};

#[derive(Debug)]
pub struct MatrixNotInvertible;

impl Display for MatrixNotInvertible {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "Matrix is not invertible")
    }
}

impl Error for MatrixNotInvertible {}

pub struct Matrix {
    pub m: [f32; 16],
}

impl Matrix {
    fn invert(&self) -> Result<Matrix, Box<dyn Error>> {
        // https://stackoverflow.com/a/1148405/1052757
        let m = self.m;
        let mut inv = [0.0; 16];

        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15]
            + m[9] * m[7] * m[14]
            + m[13] * m[6] * m[11]
            - m[13] * m[7] * m[10];

        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15]
            - m[8] * m[7] * m[14]
            - m[12] * m[6] * m[11]
            + m[12] * m[7] * m[10];

        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15]
            + m[8] * m[7] * m[13]
            + m[12] * m[5] * m[11]
            - m[12] * m[7] * m[9];

        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14]
            - m[8] * m[6] * m[13]
            - m[12] * m[5] * m[10]
            + m[12] * m[6] * m[9];

        let mut det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if det == 0.0 {
            return Err(Box::new(MatrixNotInvertible));
        }

        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15]
            - m[9] * m[3] * m[14]
            - m[13] * m[2] * m[11]
            + m[13] * m[3] * m[10];

        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15]
            + m[8] * m[3] * m[14]
            + m[12] * m[2] * m[11]
            - m[12] * m[3] * m[10];

        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15]
            - m[8] * m[3] * m[13]
            - m[12] * m[1] * m[11]
            + m[12] * m[3] * m[9];

        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14]
            + m[8] * m[2] * m[13]
            + m[12] * m[1] * m[10]
            - m[12] * m[2] * m[9];

        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15]
            + m[5] * m[3] * m[14]
            + m[13] * m[2] * m[7]
            - m[13] * m[3] * m[6];

        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15]
            - m[4] * m[3] * m[14]
            - m[12] * m[2] * m[7]
            + m[12] * m[3] * m[6];

        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15]
            + m[4] * m[3] * m[13]
            + m[12] * m[1] * m[7]
            - m[12] * m[3] * m[5];

        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[15]
            - m[4] * m[2] * m[13]
            - m[12] * m[1] * m[6]
            + m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11]
            - m[5] * m[3] * m[10]
            - m[9] * m[2] * m[7]
            + m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11]
            + m[4] * m[3] * m[10]
            + m[8] * m[2] * m[7]
            - m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11]
            - m[4] * m[3] * m[9]
            - m[8] * m[1] * m[7]
            + m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10]
            + m[4] * m[2] * m[9]
            + m[8] * m[1] * m[6]
            - m[8] * m[2] * m[5];

        det = 1.0 / det;

        inv.iter_mut().for_each(|i| *i *= det);
        Ok(Self { m: inv })
    }

    fn transpose(&self) -> Self {
        Self {
            m: (0..4)
                .flat_map(|i| [self.m[i], self.m[i + 4], self.m[i + 8], self.m[i + 12]])
                .collect::<Vec<f32>>()
                .try_into()
                .expect("Should not have occurred; could not create array from vec due to wrong number of elements"),
        }
    }
}

impl std::ops::Mul<Point> for Matrix {
    type Output = Point;

    fn mul(self, rhs: Point) -> Self::Output {
        let arr: [f32; 4] =
            (0..4)
                .map(|i| {
                    self.m[i] * rhs.x
                        + self.m[i + 4] * rhs.y
                        + self.m[i + 8] * rhs.z
                        + self.m[i + 12] * rhs.w
                })
                .collect::<Vec<f32>>()
                .try_into()
                .expect("Should not have occurred; could not create array from vec due to wrong number of elements");
        Point::from(arr)
    }
}

impl std::ops::Mul<Vector> for Matrix {
    type Output = Vector;

    fn mul(self, rhs: Vector) -> Self::Output {
        let arr: [f32; 4] =
            (0..4)
                .map(|i| {
                    self.m[i + 0] * rhs.x
                        + self.m[i + 4] * rhs.y
                        + self.m[i + 8] * rhs.z
                        + self.m[i + 12] * rhs.w
                })
                .collect::<Vec<f32>>()
                .try_into()
                .expect("Should not have occurred; could not create array from vec due to wrong number of elements");
        Vector::from(arr)
    }
}

impl std::ops::Mul<Matrix> for Matrix {
    type Output = Matrix;

    fn mul(self, rhs: Matrix) -> Self::Output {
        let m: [f32; 16] = (0..4)
            .flat_map(|i| {
                let arr: [f32; 4] = (0..4)
                    .map(|j| {
                        self.m[4 * i] * rhs.m[4 * 0 + j]
                            + self.m[1 + 4 * i] * rhs.m[4 * 1 + j]
                            + self.m[2 + 4 * i] * rhs.m[4 * 2 + j]
                            + self.m[3 + 4 * i] * rhs.m[4 * 3 + j]
                    })
                    .collect::<Vec<f32>>()
                    .try_into()
                    .expect("Should not have occurred; could not create array from vec due to wrong number of elements");
                arr
            })
            .collect::<Vec<f32>>()
            .try_into()
            .expect("Should not have occurred; could not create array from vec due to wrong number of elements");

        Self { m }
    }
}
