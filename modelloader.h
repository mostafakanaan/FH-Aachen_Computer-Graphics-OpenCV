/*
 * ModelLoader class
 * A simple interface to the Assimp library to generate arrays for Qt
 * to be used as Buffer Objects within OpenGL
 *
 * v0.6 beta, 2018
 *
 * krueckel@fh-aachen.de
 *
 * Released under THE BEER-WARE LICENSE (Rev. 42)
 *
*/

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <qopengl.h>
#include <QString>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include <cassert>

#define ML_VERTPOS_COUNT 3
#define ML_NORMAL_COUNT 3
#define ML_TEXCOOR_COUNT 2


class ModelLoader
{
public:
    ModelLoader()
        : _hasScene(false)
        , scene(NULL)
    {}

    /* Note:
     * Assimp::Importer cleans up after itself,
     * freeing the scene it previously returned
    */
    ~ModelLoader() {}

    /* TODO: this method has some weak error handling */
    bool loadObjectFromFile(const QString& name)
    {
        if (_hasScene) {
            qWarning() << "Already loaded a model";
            return false;
        }

        // Open file; this also works for Qt resource containers
        QFile file(name);
        bool success = file.open(QIODevice::ReadOnly);
        if (!success) {
            qWarning() << "Could not open file " << name;
            return false;
        }

        // Read file contents into buffer
        int len = (int)file.size();
        QDataStream in(&file);
        char* buffer = new char[len];
        in.readRawData(buffer, len);
        file.close();

        // Retrieve file extension to give the importer a hint which module to use
        QFileInfo fileInfo(name);
        QByteArray ext = fileInfo.completeSuffix().toLatin1();

        // load scene with triangulation, removing identical vertices
        // and generating normals, if necessary
        scene = importer.ReadFileFromMemory((void*)buffer,
                                            (size_t)len,
                                            aiProcess_Triangulate
                                            | aiProcess_JoinIdenticalVertices
                                            | aiProcess_GenSmoothNormals,
                                            ext.constData());

        // Free memory
        delete[] buffer;

        // check if import was successful
        if(!scene) {
            /* TODO: do something with importer.GetErrorString() */
            qWarning() << "Import failed";
            return false;
        }

        // check if there is at least one mesh
        if (scene->mNumMeshes == 0 || scene->mMeshes[0]->mNumFaces == 0) {
            importer.FreeScene();
            qWarning() << "No mesh found";
            return false;
        }

        // everything checked out
        _hasScene = true;
        return true;
    }

    // Generates a VBO with v1.x, v1.y, v1.z, v1.w, v2.x, ...
    void genSimpleVBO(GLfloat* vbo, unsigned int meshId = 0) const
    {
        return genVBO(vbo, meshId, false, false);
    }

    // Generates a VBO with
    // v1.x, v1.y, v1.z, [n1.x, n1.y, n1.z,] [t1.u, t1.v,] v2.x, ...
    // Note: Querying length and then writing data into pointer works for
    // both std::vector + glBufferData as well as
    // glMapBuffer + glUnmapBuffer
    void genVBO(GLfloat* vbo, unsigned int meshId = 0, bool normals = true, bool texcoords = true) const
    {
        assertHasScene();

        assert(scene->HasMeshes());
        const aiMesh *mesh = scene->mMeshes[meshId];

        // calculate stride
        unsigned int stride = ML_VERTPOS_COUNT;
        if (normals) {
            assert(mesh->HasNormals());
            stride += ML_NORMAL_COUNT;
        }
        if (texcoords) {
            assert(mesh->HasTextureCoords(0));
            stride += ML_TEXCOOR_COUNT;
        }

        // copy array and DON'T add 4th component
        int unsigned n;
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            n = 0;
            vbo[i*stride+n++] = mesh->mVertices[i].x;
            vbo[i*stride+n++] = mesh->mVertices[i].y;
            vbo[i*stride+n++] = mesh->mVertices[i].z;

            if (normals) {
                vbo[i*stride+n++] = mesh->mNormals[i].x;
                vbo[i*stride+n++] = mesh->mNormals[i].y;
                vbo[i*stride+n++] = mesh->mNormals[i].z;
            }

            if (texcoords) {
                /* TODO: auto-selects first UV(W) channel */
                vbo[i*stride+n++] = (mesh->mTextureCoords[0])[i].x;
                vbo[i*stride+n++] = (mesh->mTextureCoords[0])[i].y;
            }
        }
    }

    // Generates am index array with v1, v2, v3, v4, v5, v6, ...
    // to be used with drawElements(GL_TRIANGLES, ...)
    void genIndexArray(GLuint* vbi, unsigned int meshId = 0) const
    {
        assertHasScene();

        assert(scene->HasMeshes());
        const aiMesh *mesh = scene->mMeshes[meshId];

        // copying this whole array takes time, but just referencing it and keeping the other data is worse
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            vbi[i*3+0] = mesh->mFaces[i].mIndices[0];
            vbi[i*3+1] = mesh->mFaces[i].mIndices[1];
            vbi[i*3+2] = mesh->mFaces[i].mIndices[2];
        }
    }

    // These return the complete expected length of the buffer object
    unsigned int lengthOfSimpleVBO(unsigned int meshId = 0) const
    {
        return lengthOfVBO(meshId, false, false);
    }

    unsigned int lengthOfVBO(unsigned int meshId = 0, bool normals = true, bool texcoords = true) const
    {
        assertHasScene();

        assert(scene->HasMeshes());
        const aiMesh *mesh = scene->mMeshes[meshId];

        unsigned int res = mesh->mNumVertices * ML_VERTPOS_COUNT;
        if (normals) {
            assert(mesh->HasNormals());
            res += mesh->mNumVertices * ML_NORMAL_COUNT;
        }
        if (texcoords) {
            assert(mesh->HasTextureCoords(0));
            res += mesh->mNumVertices * ML_TEXCOOR_COUNT;
        }
        return res;
    }

    unsigned int lengthOfIndexArray(unsigned int meshId = 0) const
    {
        assertHasScene();

        assert(scene->HasMeshes());
        const aiMesh *mesh = scene->mMeshes[meshId];

        if (mesh->mNumFaces == 0) {
            return 0;
        }
        return mesh->mNumFaces * mesh->mFaces[0].mNumIndices;
    }

    // Returns whether the model has texture coordinates
    bool hasTextureCoordinates(unsigned int meshId = 0)
    {
        assertHasScene();

        assert(scene->HasMeshes());
        const aiMesh *mesh = scene->mMeshes[meshId];

        /* TODO: assumes first UV(W) channel */
        return mesh->HasTextureCoords(0);
    }

    // Returns whether a model is present
    bool hasScene()
    {
        return _hasScene;
    }

private:
    void assertHasScene() const {
        if (!_hasScene) {
            qWarning() << "No scene available; did you load a model?";
            assert(!"no scene loaded");
        }
    }

    bool _hasScene;
    const aiScene* scene;
    Assimp::Importer importer;
};

#endif // MODELLOADER_H

