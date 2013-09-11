#ifndef GLRENDERSUPPORT_H
#define GLRENDERSUPPORT_H

#include <GL/gl.h>
#include <QVector>

class QMatrix4x4;
class QVector3D;

struct GLGeometry
{
    QVector<GLushort> faces;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    void appendSmooth(const QVector3D &a, const QVector3D &n, int from);
    void appendFaceted(const QVector3D &a, const QVector3D &n);
    void finalize();
    void loadArrays() const;
};

class GLRenderSupport
{
public:
    enum Smoothing { Faceted, Smooth };

    GLRenderSupport(GLGeometry *);
    ~GLRenderSupport();

    void setSmoothing(Smoothing s) { sm = s; }
    void translate(const QVector3D &t);
    void rotate(qreal deg, QVector3D axis);

    void draw() const;

    void addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n);
    void addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d);

    GLushort start;
    GLushort count;
    GLushort initv;

    GLfloat    *faceColor; //[4];
    QMatrix4x4 *mat;
    Smoothing   sm;
    GLGeometry *geom;
};

#endif // GLRENDERSUPPORT_H
