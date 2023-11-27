import json
import subprocess

class Material:
    def __init__(self, ks, kd, specularexponent, diffusecolor, specularcolor, isreflective, reflectivity, isrefractive, refractiveindex):
        self.ks = ks
        self.kd = kd
        self.specularexponent = specularexponent
        self.diffusecolor = diffusecolor
        self.specularcolor = specularcolor
        self.isreflective = isreflective
        self.reflectivity = reflectivity
        self.isrefractive = isrefractive
        self.refractiveindex = refractiveindex

    def to_json(self):
        return {
            'ks': self.ks,
            'kd': self.kd,
            'specularexponent': self.specularexponent,
            'diffusecolor': self.diffusecolor,
            'specularcolor': self.specularcolor,
            'isreflective': self.isreflective,
            'reflectivity': self.reflectivity,
            'isrefractive': self.isrefractive,
            'refractiveindex': self.refractiveindex
        }
    
class MaterialTextured:
    def __init__(self, ks, kd, specularexponent, diffusecolor, specularcolor, isreflective, reflectivity, isrefractive, refractiveindex, texture):
        self.ks = ks
        self.kd = kd
        self.specularexponent = specularexponent
        self.diffusecolor = diffusecolor
        self.specularcolor = specularcolor
        self.isreflective = isreflective
        self.reflectivity = reflectivity
        self.isrefractive = isrefractive
        self.refractiveindex = refractiveindex
        self.texture = texture

    def to_json(self):
        return {
            'ks': self.ks,
            'kd': self.kd,
            'specularexponent': self.specularexponent,
            'diffusecolor': self.diffusecolor,
            'specularcolor': self.specularcolor,
            'isreflective': self.isreflective,
            'reflectivity': self.reflectivity,
            'isrefractive': self.isrefractive,
            'refractiveindex': self.refractiveindex,
            'texturefile': self.texture
        }

class Sphere:
    def __init__(self, radius, center, material: Material, materialTextured: MaterialTextured = None):
        self.radius = radius
        self.center = center
        self.material = material
        self.materialTextured = materialTextured

    def to_json(self):
        if self.materialTextured is not None:
            return {
                'type': 'sphere',
                'radius': self.radius,
                'center': self.center,
                'materialTextured': self.materialTextured.to_json()
            }

        return {
            'type': 'sphere',
            'radius': self.radius,
            'center': self.center,
            'material': self.material.to_json()
        }

class Cylinder:
    def __init__(self, radius, height, center, axis, material: Material, materialTextured: MaterialTextured = None):
        self.radius = radius
        self.height = height
        self.center = center
        self.axis = axis
        self.material = material
        self.materialTextured = materialTextured

    def to_json(self):
        if self.materialTextured is not None:
            return {
                'type': 'cylinder',
                'radius': self.radius,
                'height': self.height,
                'center': self.center,
                'axis': self.axis,
                'materialTextured': self.materialTextured.to_json()
            }
        
        return {
            'type': 'cylinder',
            'radius': self.radius,
            'height': self.height,
            'center': self.center,
            'axis': self.axis,
            'material': self.material.to_json()
        }

class Triangle:
    def __init__(self, v0, v1, v2, material: Material, materialTextured: MaterialTextured = None):
        self.v0 = v0
        self.v1 = v1
        self.v2 = v2
        self.material = material
        self.materialTextured = materialTextured

    def to_json(self):
        if self.materialTextured is not None:
            return {
                'type': 'triangle',
                'v0': self.v0,
                'v1': self.v1,
                'v2': self.v2,
                'materialTextured': self.materialTextured.to_json()
            }
        
        return {
            'type': 'triangle',
            'v0': self.v0,
            'v1': self.v1,
            'v2': self.v2,
            'material': self.material.to_json()
        }
    
class Camera:
    def __init__(self, type, width, height, position, lookAt, upVector, fov, exposure):
        self.type = type
        self.width = width
        self.height = height
        self.position = position
        self.lookAt = lookAt
        self.upVector = upVector
        self.fov = fov
        self.exposure = exposure

    def to_json(self):
        return {
            'type': self.type,
            'width': self.width,
            'height': self.height,
            'position': self.position,
            'lookAt': self.lookAt,
            'upVector': self.upVector,
            'fov': self.fov,
            'exposure': self.exposure
        }

class LightSource:
    def __init__(self, type, position, intensity):
        self.type = type
        self.position = position
        self.intensity = intensity

    def to_json(self):
        return {
            'type': self.type,
            'position': self.position,
            'intensity': self.intensity
        }
    
class Scene:
    def __init__(self, backgroundcolor, lightsources, shapes):
        self.backgroundcolor = backgroundcolor
        self.lightsources = lightsources
        self.shapes = shapes  # Assuming shapes is a list of Shape objects

    def to_json(self):
        return {
            'backgroundcolor': self.backgroundcolor,
            'lightsources': [light.to_json() for light in self.lightsources],
            'shapes': [shape.to_json() for shape in self.shapes]
        }
    
class Raytracer:
    def __init__(self, nbounces, rendermode, camera, scene):
        self.nbounces = nbounces
        self.rendermode = rendermode
        self.camera = camera
        self.scene = scene

    def to_json(self):
        return json.dumps({
            'nbounces': self.nbounces,
            'rendermode': self.rendermode,
            'camera': self.camera.to_json(),
            'scene': self.scene.to_json()
        }, indent=4)

# material definitions
floorMaterial = Material(0.1, 0.9, 20, [0.5, 0.8, 0.5], [1.0,1.0,1.0], False, 1.0, False, 1.0)

waterMaterial = MaterialTextured(0.1, 0.9, 20, [0.5, 0.5, 0.8], [1.0,1.0,1.0], False, 1.0, False, 1.0, 
                                    "C:\\Users\\Neel Amonkar\\OneDrive - University of Edinburgh\\CG - Rendering\\CGRCW2\\Textures\\64tex\\waterf1.ppm") 

lavaMaterial = MaterialTextured(0.1, 0.9, 20, [0.8, 0.5, 0.5], [1.0,1.0,1.0], False, 1.0, False, 1.0,
                                    "C:\\Users\\Neel Amonkar\\OneDrive - University of Edinburgh\\CG - Rendering\\CGRCW2\\Textures\\64tex\\lavaf1.ppm")
goldMaterial = MaterialTextured(0.4, 0.6, 20, [0.8, 0.6, 0.2], [1.0,1.0,1.0], False, 1.0, False, 1.0,
                                "C:\\Users\\Neel Amonkar\\OneDrive - University of Edinburgh\\CG - Rendering\\CGRCW2\\Textures\\64tex\\gold0.ppm")
reflectiveMaterial = Material(0.3, 0.9, 2, [0.5, 0.5, 0.5], [1.0,1.0,1.0], True, 0.9, False, 1.0)
refractiveMaterial = Material(0.1, 0.9, 20, [0.5, 0.5, 0.8], [1.0,1.0,1.0], False, 1.0, True, 1.5)

# shape definitions
floorTri1 = Triangle([-1.5,-1.5,3.25],[1.5,-1.5,3.25],[-1.5,-1.5,1.75], goldMaterial)
floorTri2 = Triangle([1.5,-1.5,1.75],[-1.5,-1.5,1.75],[1.5,-1.5,3.25], goldMaterial)
# lavaSphere = Sphere(0.25, [-0.65,-1.25,2.15], lavaMaterial)
# waterCylinder = Cylinder(0.1, 0.5, [0, -1, 2.15], [0,1,0], waterMaterial)
# refractiveSphere = Sphere(0.33, [0.65, -1.17, 2.15], refractiveMaterial)
# reflectiveCylinder = Cylinder(0.2, 0.6, [-0.4, -1.1, 3], [0,1,0], reflectiveMaterial)

# shapes = [floorTri1, floorTri2, lavaSphere, waterCylinder, refractiveSphere, reflectiveCylinder]

# # scene definition
# lightSource1 = LightSource("point", [1,0.5,4], [0.75,0.75,0.75])
# lightSource2 = LightSource("point", [0,0.5,4], [0.5,0.5,0.5])
# lightSources = [lightSource1, lightSource2]
# scene = Scene([0.2,0.2,0.2], lightSources, shapes)

# camera = Camera("pinhole", 1200, 800, [0,0,0], [0,-1,2.5], [0,1,0], 45, 1.0)

# # write to file
# raytracer = Raytracer(8, "phong", camera, scene)

# with open('C:\\Users\\Neel Amonkar\\OneDrive - University of Edinburgh\\CG - Rendering\\CGRCW2\\Video\\temp.json', 'w') as outfile:
#     outfile.write(raytracer.to_json())

# # run raytracer
# subprocess.call(["./build/CGRCW2.exe", "Video/temp.json", "Video/test.ppm"])

refractiveIndexStart = 0.1
refractiveIndexEnd = 1.5
refractiveIndexStep = (refractiveIndexEnd - refractiveIndexStart) / 150

waterCylinderRadiusStart = 0.5
waterCylinderRadiusEnd = 0.1
waterCylinderRadiusStep = (waterCylinderRadiusEnd - waterCylinderRadiusStart) / 150

waterCylinderHeightStart = 0.1
waterCylinderHeightEnd = 0.5
waterCylinderHeightStep = (waterCylinderHeightEnd - waterCylinderHeightStart) / 150

lavaSphereRadiusStart = 0.1
lavaSphereRadiusEnd = 0.4
lavaSphereRadiusStep = (lavaSphereRadiusEnd - lavaSphereRadiusStart) / 150

reflectiveCylinderAxisStep = 1/150

lightSource1PositionXStart = 1
lightSource1PositionXEnd = -1
lightSource1PositionXStep = (lightSource1PositionXEnd - lightSource1PositionXStart) / 150

lightSource2PositionZStart = 4
lightSource2PositionZEnd = 1
lightSource2PositionZStep = (lightSource2PositionZEnd - lightSource2PositionZStart) / 150

cameraPositionXStart = -1.5
cameraPositionXEnd = 1.5
cameraPositionXStep = (cameraPositionXEnd - cameraPositionXStart) / 150

for i in range(1,151):
    refractiveMaterial = Material(0.1, 0.9, 20, [0.5, 0.5, 0.8], [1.0,1.0,1.0], False, 1.0, True, refractiveIndexStart + refractiveIndexStep * i)
    refractiveSphere = Sphere(0.33, [0.65, -1.17, 2.15], refractiveMaterial)

    waterCylinderRadius = waterCylinderRadiusStart + waterCylinderRadiusStep * i
    waterCylinderHeight = waterCylinderHeightStart + waterCylinderHeightStep * i
    waterCylinder = Cylinder(waterCylinderRadius, waterCylinderHeight, [0, -1.5 + waterCylinderHeight, 2.15], [0,1,0], waterMaterial)

    lavaSphereRadius = lavaSphereRadiusStart + lavaSphereRadiusStep * i
    lavaSphere = Sphere(lavaSphereRadius, [-0.65,-1.5 + lavaSphereRadius,2.15], lavaMaterial)

    # reflectiveCylinderCenterY = reflectiveCylinderCenterYStart + reflectiveCylinderCenterYStep * i
    # reflectiveCylinder = Cylinder(0.2, 0.6, [-0.4, reflectiveCylinderCenterY, 3], [0,1,0], reflectiveMaterial)
    reflectiveCylinderAxis = [0 + (reflectiveCylinderAxisStep * i), (1 - reflectiveCylinderAxisStep * i), 0]
    reflectiveCylinder = Cylinder(0.3, 0.4, [-0.4, -1.1, 3], reflectiveCylinderAxis, reflectiveMaterial)

    lightSource1PositionX = lightSource1PositionXStart + lightSource1PositionXStep * i
    lightSource1 = LightSource("point", [lightSource1PositionX,0.5,4], [0.75,0.75,0.75])

    # lightSource2PositionY = lightSource2PositionYStart + lightSource2PositionYStep * i
    lightSource2PositionZ = lightSource2PositionZStart + lightSource2PositionZStep * i
    lightSource2 = LightSource("point", [0,0.5,lightSource2PositionZ], [0.5,0.5,0.5])

    lightSources = [lightSource1, lightSource2]
    shapes = [floorTri1, floorTri2, lavaSphere, waterCylinder, refractiveSphere, reflectiveCylinder]
    scene = Scene([0.2,0.2,0.2], lightSources, shapes)

    cameraPositionX = cameraPositionXStart + cameraPositionXStep * i
    camera = Camera("pinhole", 1200, 800, [cameraPositionX,0,0], [0,-1,2.5], [0,1,0], 45, 1.0)

    raytracer = Raytracer(8, "phong", camera, scene)

    with open('C:\\Users\\Neel Amonkar\\OneDrive - University of Edinburgh\\CG - Rendering\\CGRCW2\\Video\\temp.json', 'w') as outfile:
        outfile.write(raytracer.to_json())

    filename = str(i) + ".ppm"
    subprocess.call(["./build/CGRCW2.exe", "Video/temp.json", "Video/" + filename])
