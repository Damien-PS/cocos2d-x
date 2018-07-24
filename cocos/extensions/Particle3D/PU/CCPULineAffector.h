/****************************************************************************
 Copyright (C) 2013 Henry van Merode. All rights reserved.
 Copyright (c) 2015 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef CC_EXTENSIONS_PARTICLE3D_PU_LINEAFFECTOR_H
#define CC_EXTENSIONS_PARTICLE3D_PU_LINEAFFECTOR_H

#include <cocos/base/ccTypes.h>
#include <cocos/extensions/Particle3D/PU/CCPUAffector.h>

NS_CC_BEGIN

class CC_DLL PULineAffector : public PUAffector
{
public:
    // Constants
    static const float DEFAULT_MAX_DEVIATION;
    static const float DEFAULT_TIME_STEP;
    static const Vec3 DEFAULT_END;
    static const float DEFAULT_DRIFT;

    static PULineAffector* create();

    /**
     */
    void notifyRescaled(const Vec3& scale) override;
    void preUpdateAffector(float deltaTime) override;
    void updatePUAffector(PUParticle3D* particle, float deltaTime) override;
    void postUpdateAffector(float deltaTime) override;
    void firstParticleUpdate(PUParticle3D* particle, float deltaTime) override;
    /**
     */
    float getMaxDeviation() const;
    void setMaxDeviation(float maxDeviation);

    /**
     */
    const Vec3& getEnd() const;
    void setEnd(const Vec3& end);

    /**
     */
    float getTimeStep() const;
    void setTimeStep(float timeStep);

    /**
     */
    float getDrift() const;
    void setDrift(float drift);

    void copyAttributesTo(PUAffector* affector) override;

    CC_CONSTRUCTOR_ACCESS : PULineAffector();
    ~PULineAffector() override;

protected:
    float _maxDeviation;
    float _scaledMaxDeviation;
    Vec3 _end;
    float _timeSinceLastUpdate;
    float _timeStep;
    float _drift;
    float _oneMinusDrift;
    bool _update;
    bool _first;
};
NS_CC_END

#endif // CC_EXTENSIONS_PARTICLE3D_PU_LINEAFFECTOR_H
