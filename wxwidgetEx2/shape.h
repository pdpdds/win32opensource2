#ifndef SHAPE_H
#define SHAPE_H

enum Tetrominoes { NoShape, ZShape, SShape, LineShape, 
                  TShape, SquareShape, LShape, MirroredLShape };

class Shape
{
public:
    Shape() { SetShape(NoShape); }
    void SetShape(Tetrominoes shape);
    void SetRandomShape();

    Tetrominoes GetShape() const { return pieceShape; }
    int x(int index) const { return coords[index][0]; }
    int y(int index) const { return coords[index][1]; }

    int MinX() const;
    int MaxX() const;
    int MinY() const;
    int MaxY() const;

    Shape RotateLeft() const;
    Shape RotateRight() const;

private:
    void SetX(int index, int x) { coords[index][0] = x; }
    void SetY(int index, int y) { coords[index][1] = y; }
    Tetrominoes pieceShape;
    int coords[4][2];
};

#endif