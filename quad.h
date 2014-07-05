#ifndef QUAD_H
#define QUAD_H

#include <primitive.h>

class Quad : public Primitive
{    
    Q_OBJECT
public:
    Quad();
    void draw(bufferIDs buffIDs);

public slots:

private:
};

#endif // QUAD_H
