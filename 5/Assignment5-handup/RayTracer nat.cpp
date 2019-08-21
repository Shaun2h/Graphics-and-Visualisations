#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
    Vector3f mirror = incoming - 2 * Vector3f::dot(incoming, normal)*normal;
    return mirror;
}

//must negate direction
bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
    float nr = index_n/index_nt;
    float nDoti = Vector3f::dot(normal, incoming);
    float squareRoot = 1 - (nr*nr*(1-(nDoti*nDoti)));
    if (squareRoot >= 0){
        transmitted = (nr*nDoti-sqrt(squareRoot))*normal - nr*incoming;
        return true;
    }
    else{
        return false;
    }
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces ) :
  m_scene(scene)

{
  g=scene->getGroup();
    
  m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

//this is the recursive function
float schlickWeight(Vector3f normal, Vector3f rayDirection, Vector3f transmitted, float ni, float nt){
    float c;
    float dN = abs(Vector3f::dot(rayDirection, normal));
    float tN = abs(Vector3f::dot(transmitted, normal));
    if (ni<=nt){
        c = dN;
    }
    else if (ni>nt){
        c = tN;
    }
    float Ro = ((nt-ni)/(nt+ni)) * ((nt-1)/(nt+1));
    float R = Ro  + (1-Ro) * pow((1-c), 5);
    return R;
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit ) const
{
    if (bounces > m_maxBounces){
        return Vector3f(0,0,0); //no more tracing, it ends here
    }
    //if there's intersection
    if (g->intersect(ray, hit, tmin)){
        Vector3f color = m_scene->getAmbientLight();
        //STEP 1 check if has texture and if the texture material is valid
        if (hit.hasTex && hit.getMaterial()->t.valid()){
            color = color * hit.getMaterial()->t(hit.texCoord[0],hit.texCoord[1]); //multiply ambient with the texCoord, only ambient gets multiplied
        }
        else{
            color = color * hit.getMaterial()->getDiffuseColor();
        }
        

        
        for (int i = 0; i<m_scene->getNumLights(); i++){
            Light* L = m_scene->getLight(i);
            Vector3f lightDir;
            Vector3f lightColor;
            float distance;
            
            L->getIllumination(ray.pointAtParameter(hit.getT()), lightDir, lightColor, distance);
            
            if (true){
                 Ray shadow = Ray(ray.pointAtParameter(hit.getT()), lightDir); //make a ray shadow
                 Hit shadowHit = Hit(distance, NULL, NULL); // can i just initialize empty hit here?
                if (g->intersect(shadow, shadowHit, EPSILON) == false){ // if ray doesnt intersect any object, then get the contribution

                    color += hit.getMaterial()->Shade(ray, hit, lightDir, lightColor); //shade the material
                    
//                    //toon
//                    //segmentize the color, can be move inside "Shade" as well
//                    float intensity = Vector3f::dot(lightDir, hit.getNormal());
//
//                    // Discretize the intensity, based on a few cutoff points
//                    if (intensity > 0.95)
//                        continue;
//                    else if (intensity > 0.5)
//                        color = 0.7 * color;
//                    else if (intensity > 0.05)
//                        color = 0.35*color;
//                    else
//                        color = 0.1*color;
                    
                    
                }
            }
        }
        
        //Doing Reflection, get reflection ray direction
        Ray reflectionRay = Ray(ray.pointAtParameter(hit.getT()), mirrorDirection(hit.getNormal().normalized(), ray.getDirection()).normalized());
        
        //Prepare the hit for next traceRay to store
        Hit reflectionHit = Hit(FLT_MAX, NULL, Vector3f(0,0,0));

        //get reflected Color from recurs
        Vector3f reflectedColor = traceRay(reflectionRay, EPSILON, bounces + 1, refr_index, reflectionHit)*hit.getMaterial()->getSpecularColor();

        //Doing refraction, if theres refraction
        if (hit.getMaterial()->getRefractionIndex() > 0){
            
            //Check if it's going to material or going out to air
            Vector3f normal = hit.getNormal();
            bool in = Vector3f::dot(hit.getNormal(), ray.getDirection()) < 0;

            float nt;
            if (in){
                //from air to material
                nt = hit.getMaterial()->getRefractionIndex();
            }
            else{
                //from material to air
                nt = 1.0;
                //flip the normal
                normal = -normal;
            }
            Vector3f transmitted;
            
            bool refraction = transmittedDirection(normal, -1*ray.getDirection(), refr_index, nt, transmitted);
            float R = schlickWeight(normal, ray.getDirection(), transmitted, refr_index, nt);
            
            if (refraction){
                // do recursion if there's refraction now
                Hit refractHit = Hit();
                Ray refractRay = Ray(ray.pointAtParameter(hit.getT()), transmitted.normalized());
                Vector3f refractedColor = hit.getMaterial()->getSpecularColor() * traceRay(refractRay, EPSILON, bounces + 1, nt, refractHit);
                color += reflectedColor * R + (1-R) * refractedColor;
            }
            else{
                color += reflectedColor;
            }

        }
        else{
            color += reflectedColor;
        }
        // discretise the color based on incidence angle


        return color;
        
        
    }
    else{
        return m_scene->getBackgroundColor(ray.getDirection());
    }
   
}
