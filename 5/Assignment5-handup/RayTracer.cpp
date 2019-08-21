#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include <limits>
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{//r = v-2(vdirect . normal)normal
    return (incoming- 2.0f * Vector3f::dot(incoming, normal) * (normal)).normalized();
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt,  //refraction index 1 and refraction index 2
        Vector3f& transmitted)
{
    // we need to find the refracted direction to check if it's valid.
    
    float ratio = index_n / index_nt;
    if (index_nt <= 0) {
        return false;
    }
    float ting = 1 -ratio*ratio* (1 - Vector3f::dot(normal, incoming)* Vector3f::dot(normal, incoming));//1minus sin ditter thing
    // we return the truefalse.
    if (ting < 0) { //imaginary squareroothere...
        return false; //total internal reflection
    }
    else {
        transmitted = ratio * (incoming - normal * Vector3f::dot(normal, incoming)) - normal * sqrt(ting); //refracted direction
        //transmitted.print();
        transmitted.normalize();
        return true;
    }
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces 
  //more arguments if you need...
                      ) :
  m_scene(scene)

{
  g=scene->getGroup();
  this->m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit ) const
{
    Vector3f direction = ray.getDirection();
    Vector3f origin = ray.getOrigin();
    int totallights = m_scene->getNumLights();
    Vector3f ambient = m_scene->getAmbientLight();
    Vector3f color;
    float directional_constant_refraction = 1;
    float refractiveConstant;
    //cout << bounces << endl;
    if (this->m_scene->getGroup()->intersect(ray, hit, tmin)) { //i.e intersection.
        color = this->m_scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
        bool dontshade = false;
        for (int k = 0; k < totallights; k++) {
            //cout << k<< endl;
            Light* light = this->m_scene->getLight(k);
            Vector3f direction_to_light;
            Vector3f lightColor;
            float distance;
            light->getIllumination(ray.pointAtParameter(hit.getT()), direction_to_light, lightColor, distance);
            Ray shadowray = Ray(ray.pointAtParameter(hit.getT()), direction_to_light); //epsilon missing? 
            //MAKE SURE IT STARTS AT WHERE THE RAY HITS THE REFERENCE OBJECT!
            //extending from shadow ray to check for inteference....

            Hit newhit = Hit(distance,NULL,NULL);
            if (!this->m_scene->getGroup()->intersect(shadowray, newhit, EPSILON)) { //i.e we didn't hit anything at the top
                //if (newhit.getT()>=distance){ //unchanged t.
                    color += hit.getMaterial()->Shade(ray, hit, direction_to_light, lightColor);
                //}
                //Else don't color...
            }
                

            //Vector3f temp = hit.getMaterial()->Shade(ray, hit, direction, lightColor); //call for a shadey boi, based off materials.
            //color += temp;
        }
        //cout << screen_w_pixel << "strick " << screen_h_pixel << endl;
        Vector3f reflectionColor;
        Vector3f refractionColor;
        float weightage = 1;
        
        if (bounces > 0) {

            Vector3f reflectdir = mirrorDirection(hit.getNormal().normalized(), ray.getDirection().normalized());
            //get reflection direction
            Vector3f rayhitcoords = ray.pointAtParameter(hit.getT());
            //we get the point where the ray hits the item.
            Ray reflectionray = Ray(rayhitcoords, reflectdir);
            Hit reflecthit = Hit(std::numeric_limits<float>::max(), NULL, Vector3f(0, 0, 0));

            //this->m_scene->getGroup()->intersect(reflectionray, reflecthit, 0.0f);

            reflectionColor = traceRay(reflectionray, EPSILON, bounces - 1,
                hit.getMaterial()->getRefractionIndex(), reflecthit)
                * hit.getMaterial()->getSpecularColor();
            
            float hit_refractive_index = hit.getMaterial()->getRefractionIndex(); // the surface that was hit's refractive index
            if (hit_refractive_index > 0) {

                //multiply by the current this dude's hit color and other properties
                Vector3f normaldirection = hit.getNormal();


                //get refract direction

                if (Vector3f::dot(ray.getDirection(), hit.getNormal()) > 0) {
                    //if they have a shared line, it implies they are along the same line.
                    hit_refractive_index = 1.0f;// the surface that was hit's refractive index is reset to 1 since it does not matter.
                    normaldirection *= -1.0f; //if the ray inside the object, make it point the other way.
                }


                //REFRACTED


                Vector3f refractionDirection; //edited within
                bool hold = transmittedDirection(normaldirection, ray.getDirection(), refr_index, hit_refractive_index, 
                    refractionDirection);


                if (hold) {//refracted!
                    //cout << "bob" << endl;
                    Ray refracted_ray = Ray(ray.pointAtParameter(hit.getT()), refractionDirection.normalized());
                    //where does it intersect and where does it head from there.
                    Hit refractionnewhit = Hit();
                    refractionColor =hit.getMaterial()->getSpecularColor()* traceRay(refracted_ray, EPSILON, bounces - 1, hit_refractive_index, refractionnewhit);
                    // refractive index that was input <=the surface that was hit's refractive index   
                    //normaldirection.print();
                    //ray.getDirection().print();
                    //refracted_ray.getDirection().print();
                    if (refr_index <= hit_refractive_index) { //we need to check behaviour
                        directional_constant_refraction = Vector3f::dot(ray.getDirection(), normaldirection); //lower refractive index, go into the material
                    }
                    else {
                        directional_constant_refraction = Vector3f::dot(refracted_ray.getDirection(), normaldirection); //higher refractive index, go OUT of the material,
                        //towards the normal.
                    }

                    refractiveConstant = ((hit_refractive_index - refr_index) / (hit_refractive_index + refr_index))
                        * ((hit_refractive_index - 1) / (hit_refractive_index + 1));


                    //FRESNEL REFLECTION that took forever tofind tho
                    //cout << refractiveConstant << endl;
                    //reflectionColor.print();
                    //refractionColor.print();

                    weightage = refractiveConstant + (1.0f - refractiveConstant) * pow((1.0f - abs(directional_constant_refraction)), 5);
                    //cout << bounces << endl;
                    //cout << weightage << endl;
                    color += weightage * reflectionColor + (1.0f - weightage) * refractionColor;
                }
                else {
                    color += reflectionColor;
                }
            }
            else {
                color += reflectionColor;
            }
        }
        else { //for bounces
            color += reflectionColor;
        }
            
    }


    else {
        //cout << screen_w_pixel << " no strik " << screen_h_pixel << endl;
        color = this->m_scene->getBackgroundColor(ray.getDirection());
    }

    return color;
}
