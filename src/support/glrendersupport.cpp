#include "glrendersupport.h"

#include <QMatrix4x4>
#include <QVector3D>
#include <QColor>

///  static functions
////////////////////////////////////////////

static inline void qSetColor(float colorVec[], QColor c)
{
    colorVec[0] = c.redF();
    colorVec[1] = c.greenF();
    colorVec[2] = c.blueF();
    colorVec[3] = c.alphaF();
}

///  GLGeometry struct
////////////////////////////////////////////

void GLGeometry::loadArrays() const
{
    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glNormalPointer(GL_FLOAT, 0, normals.constData());
}

void GLGeometry::finalize()
{
    // TODO: add vertex buffer uploading here

    // Finish smoothing normals by ensuring accumulated normals are returned
    // to length 1.0.
    for (int i = 0; i < normals.count(); ++i)
        normals[i].normalize();
}

void GLGeometry::appendSmooth(const QVector3D &a, const QVector3D &n, int from)
{
    // Smooth normals are achieved by averaging the normals for faces meeting
    // at a point.  First find the point in geometry already generated
    // (working backwards, since most often the points shared are between faces
    // recently added).
    int v = vertices.count() - 1;
    for ( ; v >= from; --v)
        if (qFuzzyCompare(vertices[v], a))
            break;

    if (v < from) {
        // The vertex was not found so add it as a new one, and initialize
        // its corresponding normal
        v = vertices.count();
        vertices.append(a);
        normals.append(n);
    } else {
        // Vert found, accumulate normals into corresponding normal slot.
        // Must call finalize once finished accumulating normals
        normals[v] += n;
    }

    // In both cases (found or not) reference the vertex via its index
    faces.append(v);
}

void GLGeometry::appendFaceted(const QVector3D &a, const QVector3D &n)
{
    // Faceted normals are achieved by duplicating the vertex for every
    // normal, so that faces meeting at a vertex get a sharp edge.
    int v = vertices.count();
    vertices.append(a);
    normals.append(n);
    faces.append(v);
}

///  GLRenderSupport  class
////////////////////////////////////////////

GLRenderSupport::GLRenderSupport(GLGeometry *g)
   : start(g->faces.count())
   , count(0)
   , initv(g->vertices.count())
   , sm(GLRenderSupport::Smooth)
   , geom(g)
{
    faceColor = new GLfloat[4];
    qSetColor(faceColor, QColor(Qt::darkGray));

    mat = new QMatrix4x4();
}

GLRenderSupport::~GLRenderSupport()
{
    delete[] faceColor;
    delete   mat;
}

void GLRenderSupport::rotate(qreal deg, QVector3D axis)
{
    mat->rotate(deg, axis);
}

void GLRenderSupport::translate(const QVector3D &t)
{
    mat->translate(t);
}

// draw set of vertexes
void GLRenderSupport::draw() const
{
    glPushMatrix();
    glMultMatrixf(mat->constData());
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, faceColor);

    const GLushort *indices = geom->faces.constData();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indices + start);
    glPopMatrix();
}

void GLRenderSupport::addTri(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &n)
{
    QVector3D norm = n.isNull() ? QVector3D::normal(a, b, c) : n;

    if (sm == Smooth) {
        geom->appendSmooth(a, norm, initv);
        geom->appendSmooth(b, norm, initv);
        geom->appendSmooth(c, norm, initv);
    } else {
        geom->appendFaceted(a, norm);
        geom->appendFaceted(b, norm);
        geom->appendFaceted(c, norm);
    }

    count += 3;
}

void GLRenderSupport::addQuad(const QVector3D &a, const QVector3D &b,  const QVector3D &c, const QVector3D &d)
{
    QVector3D norm = QVector3D::normal(a, b, c);

    if (sm == Smooth) {
        addTri(a, b, c, norm);
        addTri(a, c, d, norm);
    } else {
        // If faceted share the two common vertices
        addTri(a, b, c, norm);
        int k = geom->vertices.count();
        geom->appendSmooth(a, norm, k);
        geom->appendSmooth(c, norm, k);
        geom->appendFaceted(d, norm);
        count += 3;
    }
}
