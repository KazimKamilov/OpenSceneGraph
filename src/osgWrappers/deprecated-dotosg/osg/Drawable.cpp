#include "osg/Drawable"
#include "osg/Notify"

#include "osgDB/Registry"
#include "osgDB/Input"
#include "osgDB/Output"

using namespace osg;
using namespace osgDB;

// forward declare functions to use later.
bool Drawable_readLocalData(Object& obj, Input& fr);
bool Drawable_writeLocalData(const Object& obj, Output& fw);

// register the read and write functions with the osgDB::Registry.
REGISTER_DOTOSGWRAPPER(Drawable)
(   NULL,
    "Drawable",
    "Object Drawable",
    &Drawable_readLocalData,
    &Drawable_writeLocalData
);

bool Drawable_readLocalData(Object& obj, Input& fr)
{
    bool iteratorAdvanced = false;

    Drawable& drawable = static_cast<Drawable&>(obj);

    osg::StateSet* readState = fr.readObjectOfType<osg::StateSet>();
    if (readState)
    {
        drawable.setStateSet(readState);
        iteratorAdvanced = true;
    }

    Shape* shape = fr.readObjectOfType<Shape>();
    if (shape)
    {
        drawable.setShape(shape);
        iteratorAdvanced = true;
    }

    DrawableUpdateCallback* uc = fr.readObjectOfType<DrawableUpdateCallback>();
    if (uc)
    {
        drawable.setUpdateCallback(uc);
        iteratorAdvanced = true;
    }

    DrawableEventCallback* ec = fr.readObjectOfType<DrawableEventCallback>();
    if (ec)
    {
        drawable.setEventCallback(ec);
        iteratorAdvanced = true;
    }

    DrawableCullCallback* cc = fr.readObjectOfType<DrawableCullCallback>();
    if (cc)
    {
        drawable.setCullCallback(cc);
        iteratorAdvanced = true;
    }

    Drawable::DrawCallback* dc = fr.readObjectOfType<Drawable::DrawCallback>();
    if (dc)
    {
        drawable.setDrawCallback(dc);
        iteratorAdvanced = true;
    }

    if (fr.matchSequence("initialBound %f %f %f %f %f %f"))
    {
        BoundingBox bb;
        fr[1].getFloat(bb.xMin());
        fr[2].getFloat(bb.yMin());
        fr[3].getFloat(bb.zMin());
        fr[4].getFloat(bb.xMax());
        fr[5].getFloat(bb.yMax());
        fr[6].getFloat(bb.zMax());
        drawable.setInitialBound(bb);
        fr += 7;
        iteratorAdvanced = true;
    }

    Drawable::ComputeBoundingBoxCallback* cbc = fr.readObjectOfType<Drawable::ComputeBoundingBoxCallback>();
    if (cbc)
    {
        drawable.setComputeBoundingBoxCallback(cbc);
        iteratorAdvanced = true;
    }

    if (fr[0].matchWord("useVertexBufferObjects"))
    {
        if (fr[1].matchWord("TRUE"))
        {
            drawable.setUseVertexBufferObjects(true);
            fr+=2;
            iteratorAdvanced = true;
        }
        else if (fr[1].matchWord("FALSE"))
        {
            drawable.setUseVertexBufferObjects(false);
            fr+=2;
            iteratorAdvanced = true;
        }
    }

    return iteratorAdvanced;
}


bool Drawable_writeLocalData(const Object& obj, Output& fw)
{
    const Drawable& drawable = static_cast<const Drawable&>(obj);

    if (drawable.getStateSet())
    {
        fw.writeObject(*drawable.getStateSet());
    }

    if (drawable.getShape())
    {
        fw.writeObject(*drawable.getShape());
    }

    if (drawable.getUpdateCallback())
    {
        fw.writeObject(*drawable.getUpdateCallback());
    }

    if (drawable.getEventCallback())
    {
        fw.writeObject(*drawable.getEventCallback());
    }

    if (drawable.getCullCallback())
    {
        fw.writeObject(*drawable.getCullCallback());
    }

    if (drawable.getDrawCallback())
    {
        fw.writeObject(*drawable.getDrawCallback());
    }


    if (drawable.getInitialBound().valid())
    {
        const osg::BoundingBox& bb = drawable.getInitialBound();
        fw.indent()<<"initialBound "<<bb.xMin()<<" "<<bb.yMin()<<" "<<bb.zMin()<<" "
                                    <<bb.xMax()<<" "<<bb.yMax()<<" "<<bb.zMax()<<std::endl;
    }

    if (drawable.getComputeBoundingBoxCallback())
    {
        fw.writeObject(*drawable.getComputeBoundingBoxCallback());
    }

    fw.indent()<<"useVertexBufferObjects ";
    if (drawable.getUseVertexBufferObjects()) fw << "TRUE" << std::endl;
    else fw << "FALSE" << std::endl;

    return true;
}
