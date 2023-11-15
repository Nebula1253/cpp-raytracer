#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

class material {
    private:
        double ks, kd, reflectivity, refractiveIndex;
        int specularExponent;
        color diffuseColor, specularColor;
        bool isReflective, isRefractive;

    public:
        // generated by copilot
        material() {}
        material(double ks, double kd, double reflectivity, double refractiveIndex, int specularExponent, color diffuseColor, color specularColor, bool isReflective, bool isRefractive) : 
            ks(ks), kd(kd), reflectivity(reflectivity), refractiveIndex(refractiveIndex), specularExponent(specularExponent), diffuseColor(diffuseColor), specularColor(specularColor), isReflective(isReflective), isRefractive(isRefractive) {};

        double getKs() const { return ks; };
        double getKd() const { return kd; };
        double getReflectivity() const { return reflectivity; };
        double getRefractiveIndex() const { return refractiveIndex; };
        int getSpecularExponent() const { return specularExponent; };
        color getDiffuseColor() const { return diffuseColor; };
        color getSpecularColor() const { return specularColor; };
        bool getIsReflective() const { return isReflective; };
        bool getIsRefractive() const { return isRefractive; };

};

#endif