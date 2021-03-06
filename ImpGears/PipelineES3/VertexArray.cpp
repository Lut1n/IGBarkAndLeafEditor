#include <PipelineES3/VertexArray.h>

IMPGEARS_BEGIN

std::uint32_t VertexArray::_s_count = 0;
std::uint32_t VertexArray::_s_vboCount = 0;

//--------------------------------------------------------------
VertexArray::VertexArray()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    _vao = 0;
    _buffers.resize(Buffer_Count, 0);
    _primitive = Geometry::Primitive_Triangles;
    _useElements = false;
    _instanced = false;
    _count = 0;
    _instanceCount = 0;

    f->glGenVertexArrays(1, &_vao);
    GL_CHECKERROR("gen vao");
    _s_count++;

    glGenBuffers(_buffers.size(), _buffers.data());
    GL_CHECKERROR("gen vbo");
    _s_vboCount += _buffers.size();
}

//--------------------------------------------------------------
VertexArray::~VertexArray()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    use();

    glDeleteBuffers(_buffers.size(), _buffers.data());
    GL_CHECKERROR("delete vbo");
    _s_vboCount -= _buffers.size();

    // glDeleteBuffers(1, &_vao);
    f->glDeleteVertexArrays(1, &_vao);
    GL_CHECKERROR("delete vao");
    _s_count--;
}

//--------------------------------------------------------------
void VertexArray::load(const Geometry& geometry)
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    _useElements = geometry._hasIndices;
    _primitive = geometry.getPrimitive();

    std::vector<float> buf; geometry.fillBuffer( buf );
    loadBuffer<float>(Buffer_Vertices, buf);
    _count = geometry.size();

    if(geometry._hasColors)
    {
        std::vector<float> buf; fillBuffer<float,3,Vec3>(buf, geometry._colors);
        loadBuffer<float>(Buffer_Colors, buf);
    }

    if(geometry._hasNormals)
    {
        std::vector<float> buf; fillBuffer<float,3,Vec3>(buf, geometry._normals);
        loadBuffer<float>(Buffer_Normals, buf);
    }

    if(geometry._hasTexCoords)
    {
        std::vector<float> buf; fillBuffer<float,2,Geometry::TexCoord>(buf, geometry._texCoords);
        loadBuffer<float>(Buffer_TexCoords, buf);
    }

    if(geometry._hasIndices)
    {
        loadBuffer<std::uint32_t>(Buffer_Indices, geometry._indices, true);
        _count = geometry._indices.size();
    }

    const InstancedGeometry* instancedGeo = dynamic_cast<const InstancedGeometry*>( &geometry );
    bool hasInstanceTransforms = false;
    if(instancedGeo)
    {
        _instanced = true;
        _instanceCount = instancedGeo->count();
        hasInstanceTransforms = instancedGeo->hasChanged() && instancedGeo->getTransforms().size()>0;
        std::cout << "instanced geometry detected" << std::endl;

        if(hasInstanceTransforms)
        {
            const std::vector<Matrix4>& bufMat = instancedGeo->getTransforms();
            _instanceCount = std::min(_instanceCount,(int)bufMat.size());

            std::vector<float> buf(_instanceCount * 16);
            for(int i=0;i<_instanceCount;++i)
            {
                const float* data = bufMat[i].data();
                for(int k=0;k<16;++k) buf[i*16+k] = data[k];
            }
            loadBuffer<float>(Buffer_InstTransforms, buf);
        }
    }

    GLuint glVertex = 0, glColor = 1, glNormal = 2, glTexCoord = 3;
    GLuint glInstTransform1 = 4, glInstTransform2 = 5, glInstTransform3 = 6, glInstTransform4 = 7;

    enableAttrib(Buffer_Vertices, glVertex, 3, sizeof(float)*3, 0);
    if(geometry._hasColors)
        enableAttrib(Buffer_Colors, glColor, 3, sizeof(float)*3, 0);
    if(geometry._hasNormals)
        enableAttrib(Buffer_Normals, glNormal, 3, sizeof(float)*3, 0);
    if(geometry._hasTexCoords)
        enableAttrib(Buffer_TexCoords, glTexCoord, 2, sizeof(float)*2, 0);
    if(_instanced && hasInstanceTransforms)
    {
        GLsizei vec4size = sizeof(float)*4;
        enableAttrib(Buffer_InstTransforms, glInstTransform1, 4, 4*vec4size, 0*4);
        enableAttrib(Buffer_InstTransforms, glInstTransform2, 4, 4*vec4size, 1*4);
        enableAttrib(Buffer_InstTransforms, glInstTransform3, 4, 4*vec4size, 2*4);
        enableAttrib(Buffer_InstTransforms, glInstTransform4, 4, 4*vec4size, 3*4);

        f->glVertexAttribDivisor(glInstTransform1, 1);
        f->glVertexAttribDivisor(glInstTransform2, 1);
        f->glVertexAttribDivisor(glInstTransform3, 1);
        f->glVertexAttribDivisor(glInstTransform4, 1);
    }
}

//--------------------------------------------------------------
void VertexArray::enableAttrib(BufferName bufName, GLuint location, int size, int stride, int offset)
{
    if(bufName == Buffer_Indices) return;

    GLenum glType = GL_FLOAT;
    void* addrOffset = (void*) (sizeof(float) * offset);
    use();
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[bufName]);
    GL_CHECKERROR("VertexArray::enableAttrib bind");
    glEnableVertexAttribArray(location);
    GL_CHECKERROR("VertexArray::enableAttrib enable");
    glVertexAttribPointer(location, size, glType, GL_FALSE, stride, addrOffset);
    GL_CHECKERROR("VertexArray::enableAttrib pointer");
}

//--------------------------------------------------------------
void VertexArray::use()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glBindVertexArray(_vao);
    GL_CHECKERROR("VertexArray use");
}

//--------------------------------------------------------------
void VertexArray::draw()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    use();

    GLenum glPrimitive =  GL_TRIANGLES;
    if(_primitive == Geometry::Primitive_Points) glPrimitive = GL_POINTS;
    else if(_primitive == Geometry::Primitive_Lines) glPrimitive = GL_LINES;
    else if(_primitive == Geometry::Primitive_Triangles) glPrimitive = GL_TRIANGLES;

    if(_useElements && _instanced)
        f->glDrawElementsInstanced(glPrimitive,_count,GL_UNSIGNED_INT,(void*)0,_instanceCount);
    else if(_useElements)
        glDrawElements(glPrimitive, _count, GL_UNSIGNED_INT, (void*)0);
    else if(_instanced)
        f->glDrawArraysInstanced(glPrimitive,0,_count,_instanceCount);
    else
        glDrawArrays(glPrimitive, 0, _count);

    GL_CHECKERROR("VertexArray draw");
}


IMPGEARS_END
