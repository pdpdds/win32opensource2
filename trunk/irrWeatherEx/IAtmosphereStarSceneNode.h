// Copyright (C) Pazystamo
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.

#ifndef __IAtmosphereStarSceneNode_H_INCLUDED__
#define __IAtmosphereStarSceneNode_H_INCLUDED__

#include <irrlicht.h>

//almost all code from CBillboardSceneNode
class IAtmosphereStarSceneNode : public irr::scene::ISceneNode
{
    public:

        IAtmosphereStarSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr,
            irr::s32 id,irr::core::vector3df position,const irr::core::dimension2d<irr::f32>& size);

        //virtual void setSize(const core::dimension2d<f32>& size);

        virtual void OnRegisterSceneNode();

        virtual void render();

        virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

        /// sets the size of the billboard
        void setSize(const irr::core::dimension2d<irr::f32>& size);

        virtual irr::u32 getMaterialCount() const;

        virtual irr::video::SMaterial& getMaterial(irr::u32 i);

        // Change sky texture
        virtual irr::video::SMaterial& setMaterial(irr::video::ITexture* tex);

        // Update UV mapping X coordinate
        const irr::core::dimension2d<irr::f32>& getSize();

        void setMoonPosition(const irr::core::vector3df& newPos) { MoonPosition = newPos; };

    private:
        irr::core::aabbox3d<irr::f32> BBox;
        irr::core::dimension2d<irr::f32> Size;
        irr::video::S3DVertex Vertices[4];
        irr::video::SMaterial Material;
        irr::video::SMaterial MoonMaterial;
        irr::u16 indices[6];
        irr::video::S3DVertex vertices[4];
        irr::core::vector3df MoonPosition;
};

#endif // __IAtmosphereStarSceneNode_H_INCLUDED__
