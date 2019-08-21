
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
    return (incoming - 2.0 * (Vector3f::dot(incoming, normal)) * normal).normalized();
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
    float check = 1.0f - (pow(index_n,2) * (1.0f - pow(Vector3f::dot(incoming, normal),2)) / pow(index_nt,2));
    if (check > 0){
        transmitted = ((index_n * (incoming - normal * Vector3f::dot(incoming, normal))) / index_nt) - (normal * pow(check,0.5));
        return true;
    }
    else{
        return false;
    }
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces):m_scene(scene)
{
    g=scene->getGroup();
    m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit ) const
{
    // compute color seen from the origin along the direction
    // computation is recursive for reflected(or transparent) materials
    // max bounces to prevent infinte recursion
    // max recursion depth passed as a command line argument
    //hit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
    
    Vector3f color = Vector3f(0,0,0);
    
    if (g->intersect(ray, hit, tmin)){
        // Shadow
        // 1. set color = ambient light
        color = m_scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
        
        // 2. determine whether we should shade directional light
        for (int i=0; i<m_scene->getNumLights(); i++){
            Light* light = m_scene->getLight(i);
            Vector3f directionToLight;
            Vector3f lightColor;
            float distanceToLight;
            light->getIllumination(ray.pointAtParameter(hit.getT()), directionToLight, lightColor, distanceToLight);

            // 3. cast shadow ray
            Ray shadowRay = Ray(ray.pointAtParameter(hit.getT()), directionToLight);
            
            // 4. init placeholder for hit target. t =  distanceToLight
            Hit shadowHit = Hit(distanceToLight,NULL, NULL);
            
            // 5. check whether any object intersects with shadow ray
//            g->intersect(shadowRay, shadowHit, EPSILON);
            
            if (g->intersect(shadowRay, shadowHit, EPSILON) == false){
                color += hit.getMaterial()->Shade(ray,hit,directionToLight, lightColor);
            }
        }
        
        if (bounces < m_maxBounces){
            // Reflection
            // Cast ray symmetric with the normal: R = V-2(V.N)N
            
            Vector3f reflectDirection = mirrorDirection(hit.getNormal().normalized(), ray.getDirection().normalized());
            Ray reflectionRay = Ray(ray.pointAtParameter(hit.getT()), reflectDirection);
            Hit reflectionHit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
            //g->intersect(reflectionRay, reflectionHit, EPSILON);
            
            Vector3f reflectionColour = hit.getMaterial()->getSpecularColor() * traceRay(reflectionRay, EPSILON, bounces+1, hit.getMaterial()->getRefractionIndex(),reflectionHit);
         
            /*
            if (hit.getMaterial()->getRefractionIndex() > 0){
                // Refraction
                Vector3f incomingRayDirection = ray.getDirection();
                Vector3f normal = hit.getNormal();
                float index_nt;
                if (Vector3f::dot(normal,incomingRayDirection) < 0){
                    // from air to material
                    index_nt = hit.getMaterial()->getRefractionIndex();
                }
                else{
                    // from material to air
                    index_nt = 1.0;
                    normal=-normal;
                }
                
                float index_n = refr_index;
                
                // Cast refraction rays for transparent materials
                Vector3f refractDirection;
                if (transmittedDirection(normal, incomingRayDirection, index_n, index_nt, refractDirection)){
                    Ray refractionRay = Ray(ray.pointAtParameter(hit.getT()),refractDirection.normalized());
                    Hit refractionHit = Hit( FLT_MAX, NULL, Vector3f( 0, 0, 0 ) );
                    //g->intersect(refractionRay, refractionHit, EPSILON);
                    Vector3f refractionColour = hit.getMaterial()->getSpecularColor() * traceRay(refractionRay, EPSILON, bounces+1, index_nt, refractionHit);
                    
                    float c;
                    if (index_n <= index_nt){
                        c = abs(Vector3f::dot(incomingRayDirection, normal));
                    }
                    else{
                        c = abs(Vector3f::dot(refractDirection, normal));
                    }
                    //float Ro = pow(((index_nt-index_n)/(index_nt+index_n)),2);
                    float Ro = ((index_nt-index_n)/(index_nt+index_n)) * ((index_nt-1)/(index_nt+1));
                    float R = Ro + (1-Ro)*pow((1-c),5);
                    color = color + (R*reflectionColour) + ((1-R)*refractionColour);
                }
                else{
                    color+=reflectionColour;
                }
            }
            else{
                color+=reflectionColour;
            }
             */
            color+=reflectionColour;
        }
    }
    else{
        color = m_scene->getBackgroundColor(ray.getDirection());
    }

    return color;
}

